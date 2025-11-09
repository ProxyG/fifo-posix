#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

pid_t server_pid = 0;

void print_menu() {
    printf("\n");
    printf("╔════════════════════════════════════════════╗\n");
    printf("║   SYSTÈME CLIENT/SERVEUR FIFO              ║\n");
    printf("╚════════════════════════════════════════════╝\n");
    printf("\n");
    
    if (server_pid > 0) {
        printf("  Serveur: \033[32mACTIF\033[0m (PID: %d)\n", server_pid);
    } else {
        printf("  Serveur: \033[31mINACTIF\033[0m\n");
    }
    
    printf("\n");
    printf("  1. Démarrer le serveur\n");
    printf("  2. Lancer un client\n");
    printf("  3. Arrêter le serveur (SIGKILL)\n");
    printf("  4. Quitter\n");
    printf("\n");
    printf("Choix: ");
}

void start_server() {
    if (server_pid > 0) {
        printf("\n\033[33m⚠️  Le serveur est déjà démarré (PID: %d)\033[0m\n", server_pid);
        return;
    }
    
    server_pid = fork();
    
    if (server_pid == 0) {
        // Processus enfant - lance le serveur
        execl("./serveur", "serveur", NULL);
        perror("Erreur lors du lancement du serveur");
        exit(1);
    } else if (server_pid > 0) {
        // Processus parent
        sleep(1); // Laisser le temps au serveur de démarrer
        printf("\n\033[32m✓ Serveur démarré avec succès (PID: %d)\033[0m\n", server_pid);
    } else {
        perror("Erreur fork");
    }
}

void launch_client() {
    if (server_pid == 0) {
        printf("\n\033[31m✗ Erreur: Le serveur doit être démarré d'abord\033[0m\n");
        return;
    }
    
    int nombre;
    printf("\nEntrez un nombre: ");
    if (scanf("%d", &nombre) != 1) {
        printf("\033[31m✗ Nombre invalide\033[0m\n");
        while(getchar() != '\n'); // Vider le buffer
        return;
    }
    while(getchar() != '\n'); // Vider le buffer
    
    pid_t client_pid = fork();
    
    if (client_pid == 0) {
        // Processus enfant - lance le client
        char server_pid_str[32];
        char nombre_str[32];
        
        sprintf(server_pid_str, "%d", server_pid);
        sprintf(nombre_str, "%d", nombre);
        
        execl("./client", "client", server_pid_str, nombre_str, NULL);
        perror("Erreur lors du lancement du client");
        exit(1);
    } else if (client_pid > 0) {
        // Processus parent
        printf("\033[32m✓ Client lancé (PID: %d) avec nombre=%d\033[0m\n", client_pid, nombre);
        
        // Attendre que le client se termine
        int status;
        waitpid(client_pid, &status, 0);
        printf("\033[36m→ Client %d terminé\033[0m\n", client_pid);
    } else {
        perror("Erreur fork");
    }
}

void stop_server() {
    if (server_pid == 0) {
        printf("\n\033[33m⚠️  Aucun serveur en cours d'exécution\033[0m\n");
        return;
    }
    
    printf("\n\033[31m⚠️  Arrêt du serveur (PID: %d) avec SIGKILL...\033[0m\n", server_pid);
    
    if (kill(server_pid, SIGKILL) == 0) {
        waitpid(server_pid, NULL, 0);
        printf("\033[32m✓ Serveur arrêté avec succès\033[0m\n");
        server_pid = 0;
    } else {
        perror("Erreur lors de l'arrêt du serveur");
    }
}

void cleanup() {
    if (server_pid > 0) {
        printf("\n\033[33mNettoyage: arrêt du serveur...\033[0m\n");
        kill(server_pid, SIGKILL);
        waitpid(server_pid, NULL, 0);
    }
}

int main() {
    int choix;
    
    // Enregistrer la fonction de nettoyage
    atexit(cleanup);
    
    printf("\033[2J\033[H"); // Effacer l'écran
    
    while (1) {
        print_menu();
        
        if (scanf("%d", &choix) != 1) {
            printf("\033[31m✗ Choix invalide\033[0m\n");
            while(getchar() != '\n'); // Vider le buffer
            sleep(1);
            continue;
        }
        while(getchar() != '\n'); // Vider le buffer
        
        switch (choix) {
            case 1:
                start_server();
                sleep(2);
                break;
                
            case 2:
                launch_client();
                sleep(2);
                break;
                
            case 3:
                stop_server();
                sleep(2);
                break;
                
            case 4:
                printf("\n\033[36m👋 Au revoir!\033[0m\n\n");
                return 0;
                
            default:
                printf("\n\033[31m✗ Choix invalide\033[0m\n");
                sleep(1);
        }
    }
    
    return 0;
}