#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <fileapi.h>
#include "main.h"



void Regles() { Color(5,0);
    printf("Regles du jeu : \n\n\n -Vous devez attraper tous les oiseaux pour gagner \n -Vous avez 3 vies \n -Vous perdez une vie si vous touchez la balle \n -Appuyez sur la touche P pour mettre en pause \n\n");
    printf("Bloc cassable (cassez le avec la touche Espace):  ");
    Color(0,11);
    printf("X\n");
    Color(5,0);
    printf("Bloc poussable :  ");
    Color(5,5);
    printf("#\n");
    Color(5,0);
    printf("Bloc piege : ");
    Color(0,9);
    printf("!\n");
    Color(5,0);

    printf("Appuyez sur Entree pour revenir au menu.\n");
    getchar(); // Attend que l'utilisateur appuie sur Entrée
}

void NouvellePartie() {Color(5,0);
    jouer("");
}

void ChargerPartie() {Color(5,0);
    char chemin[50]="../";
    char entree[45];
    printf("Nom de la sauvegarde: ");
    scanf("%s",entree);
    nettoyerMemoireClavier();
    strcat(chemin,entree);
    if(GetFileAttributesA(chemin)==INVALID_FILE_ATTRIBUTES){
        printf("\nCe fichier n'existe pas\n\n");
    }
    else{
        jouer(chemin);
    }

}

void entrerMotDePasse() {Color(5,0);
    char mdp[30];
    printf("Mot de passe: ");
    scanf("%s", mdp);
    nettoyerMemoireClavier();
    int retour = verifMotDePasse(mdp);
    if(retour==0){
        printf("Mauvais mot de passe\n");
    }
    else{
        main();
    }
}

void Scores() {Color(5,0);
    printf("Scores - Non implemente.\n");
    // Ajoutez ici le code pour afficher les scores
}





int main() {
    system("cls");
    COORD pos = {0, 0};
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos);
    Color(5,0);
    printf("  _____   _   _    ____     ____    _____   __     __\n");
    printf(" / ____| | \\ | |  / __ \\   / __ \\  |  __ \\  \\ \\   / /\n");
    printf("| (___   |  \\| | | |  | | | |  | | | |__) |  \\ \\_/ / \n");
    printf(" \\___ \\  | . ` | | |  | | | |  | | |  ___/    \\   /  \n");
    printf(" ____) | | |\\  | | |__| | | |__| | | |         | |   \n");
    printf("|_____/  |_| \\_|  \\____/   \\____/  |_|         |_|   \n");
    printf("\n");
    printf("\n");
    int choix;
    do {


        // Afficher le menu
        Color(11,0);
        printf("1. Regles du jeu\n");
        printf("2. Lancer un nouveau jeu\n");
        printf("3. Charger une partie\n");
        printf("4. Mot de passe\n");;
        printf("5. Scores\n");
        printf("6. Quitter\n");

        // Demander à l'utilisateur de faire un choix
        printf("Choix : ");
        scanf("%d", &choix);

        // Vider le tampon d'entrée pour éviter les problèmes avec getchar
        while (getchar() != '\n');

        // Exécuter l'action correspondante
        switch (choix) {
            case 1:
                Regles();
                break;
            case 2:
                NouvellePartie();
                main();
                break;
            case 3:
                ChargerPartie();
                break;
            case 4:
                entrerMotDePasse();
                break;
            case 5:
                Scores();
                break;
            case 6:
                printf("Au revoir !\n");
                exit(0);
                break;
            default:
                printf("Choix non valide. Veuillez reessayer.\n");
        }
    } while (choix != 6);
    return 0;
}