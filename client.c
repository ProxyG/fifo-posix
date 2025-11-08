#include "serv_cli_fifo.h"
#include "Handlers_Cli.h"

int main(int argc, char *argv[]) {
    Question q;
    Reponse r;
    int fd1, fd2;
    struct sigaction sa;
    pid_t pid_serveur;
    
    if (argc != 3) {
        printf("Usage: %s <PID_serveur> <nombre>\n", argv[0]);
        exit(1);
    }
    
    pid_serveur = atoi(argv[1]);
    q.nombre = atoi(argv[2]);
    q.pid_client = getpid();
    
    printf("Client (PID: %d) démarre...\n", getpid());
    
    /* Installation des Handlers */
    sa.sa_handler = hand_reveil;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);
    
    /* Ignorer tous les autres signaux (sauf SIGKILL et SIGSTOP) */
    struct sigaction sa_ign;
    sa_ign.sa_handler = SIG_IGN;
    sigemptyset(&sa_ign.sa_mask);
    sa_ign.sa_flags = 0;
    
    for (int i = 1; i < 32; i++) {
        if (i != SIGUSR1 && i != SIGKILL && i != SIGSTOP) {
            sigaction(i, &sa_ign, NULL);
        }
    }
    
    /* Ouverture des tubes nommés */
    fd1 = open(FIFO1, O_WRONLY);
    fd2 = open(FIFO2, O_RDONLY);
    
    if (fd1 == -1 || fd2 == -1) {
        perror("Erreur ouverture FIFO");
        exit(1);
    }
    
    /* Construction et envoi d'une question */
    printf("Envoi de la question: nombre = %d\n", q.nombre);
    write(fd1, &q, sizeof(Question));
    
    /* Attente de la réponse */
    reveil = 0;
    while (!reveil) {
        pause();
    }
    
    /* Lecture de la réponse */
    read(fd2, &r, sizeof(Reponse));
    printf("Réponse reçue: %d\n", r.resultat);
    
    /* Envoi du signal SIGUSR1 au serveur */
    kill(pid_serveur, SIGUSR1);
    
    /* Traitement local de la réponse */
    printf("Traitement terminé.\n");
    
    close(fd1);
    close(fd2);
    return 0;
}