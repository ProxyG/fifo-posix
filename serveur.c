#include "serv_cli_fifo.h"
#include "Handlers_Serv.h"

int main() {
    Question q;
    Reponse r;
    int fd1, fd2;
    struct sigaction sa_reveil, sa_fin;
    
    printf("Démarrage du serveur (PID: %d)\n", getpid());
    
    /* Création des tubes nommés */
    unlink(FIFO1);
    unlink(FIFO2);
    if (mkfifo(FIFO1, 0666) == -1 || mkfifo(FIFO2, 0666) == -1) {
        perror("Erreur mkfifo");
        exit(1);
    }
    
    /* Initialisation du générateur de nombres aléatoires */
    srand(getpid());
    
    /* Installation des Handlers */
    sa_reveil.sa_handler = hand_reveil;
    sigemptyset(&sa_reveil.sa_mask);
    sa_reveil.sa_flags = 0;
    sigaction(SIGUSR1, &sa_reveil, NULL);
    
    /* Ignorer tous les autres signaux (sauf SIGKILL et SIGSTOP) */
    sa_fin.sa_handler = SIG_IGN;
    sigemptyset(&sa_fin.sa_mask);
    sa_fin.sa_flags = 0;
    
    for (int i = 1; i < 32; i++) {
        if (i != SIGUSR1 && i != SIGKILL && i != SIGSTOP) {
            sigaction(i, &sa_fin, NULL);
        }
    }
    
    /* Ouverture des tubes nommés */
    fd1 = open(FIFO1, O_RDONLY);
    fd2 = open(FIFO2, O_WRONLY);
    
    if (fd1 == -1 || fd2 == -1) {
        perror("Erreur ouverture FIFO");
        exit(1);
    }
    
    printf("Serveur prêt. En attente de questions...\n");
    
    while(1) {
        /* Lecture d'une question */
        if (read(fd1, &q, sizeof(Question)) > 0) {
            printf("Question reçue du client %d: nombre = %d\n", 
                   q.pid_client, q.nombre);
            
            /* Construction de la réponse */
            r.resultat = rand() % NMAX;
            printf("Réponse générée: %d\n", r.resultat);
            
            /* Envoi de la réponse */
            write(fd2, &r, sizeof(Reponse));
            
            /* Envoi du signal SIGUSR1 au client concerné */
            kill(q.pid_client, SIGUSR1);
            printf("Signal envoyé au client %d\n\n", q.pid_client);
        }
    }
    
    close(fd1);
    close(fd2);
    return 0;
}