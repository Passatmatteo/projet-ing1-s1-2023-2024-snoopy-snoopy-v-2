#include <stdio.h>
#include <stdlib.h>
#include <conio.h> // Cette bibliothèque est utilisée pour la fonction getch() sous Windows
#include <time.h> // Cette bibliothèque est utilisée pour mesurer le temps
#include <windows.h>

void Regles() {
    printf("Regles du jeu : Vous devez attraper tous les oiseaux pour gagner \n");
    // Ajoutez ici les règles du jeu
    printf("Appuyez sur Entree pour revenir au menu.\n");
    getchar(); // Attend que l'utilisateur appuie sur Entrée
}

void NouvellePartie() {
    printf("Nouveau jeu lance à partir du niveau 1.\n");{
#define LONGUEUR 20
#define LARGEUR 10
#define TEMPS_IMPARTI 120 // Temps imparti en secondes

    typedef struct Jeu {
        char plateau[LARGEUR][LONGUEUR];
        int joueurX;
        int joueurY;
        int nbOiseaux;
        int score;



    }Jeu;
    void Color(int couleurDuTexte,int couleurDeFond)
    {
        HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(H,couleurDeFond*16+couleurDuTexte);
    }


    typedef struct Bouton{
        int codeBouton;
        int precedent;
        int actuel;
    } Bouton;

// Fonction pour afficher le plateau de jeu
    void afficherPlateau(Jeu* monjeu) {
        COORD pos = {0, 0};
        HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(output, pos);
        for(int j=0; j<LONGUEUR+2; j++){
            printf("#");
        }
        printf("\n");
        for (int i = 0; i < LARGEUR; i++) {
            printf("#");
            for (int j = 0; j < LONGUEUR; j++) {
                char code = monjeu->plateau[i][j];
                char aff;
                if(monjeu->joueurX == j && monjeu->joueurY == i){
                    aff = '@';
                }
                else{
                    switch(code){
                        case '0': aff=' '; break;
                        case '1': aff='$'; break;
                        case '2': aff ='K'; break;
                        case '4': aff ='*'; break;
                        case '9': aff ='V'; break;
                        default: aff=' '; break;
                    }

                }

                printf("%c", aff);

            }
            printf("#");
            printf("\n");
        }
        for(int j=0; j<LONGUEUR+2; j++){
            printf("#");
        }
        printf("\nNb Oiseaux:%d",monjeu->nbOiseaux);
        printf("\nScore: %d",monjeu->score);


    }

    void charger(char* chemin, Jeu* jeu){
        jeu->nbOiseaux=0;
        FILE* fichier;
        fichier = fopen(chemin, "r");
        if(!fichier){
            printf("ERREUR!");
            return;
        }

        fscanf(fichier, "%d", &(jeu->joueurX));
        fscanf(fichier, "%d", &(jeu->joueurY));
        char c;
        fscanf(fichier,"%c",&c);
        for (int i = 0; i < LARGEUR; i++) {
            for (int j = 0; j < LONGUEUR; j++) {
                char c;
                fscanf(fichier,"%c",&c);
                jeu->plateau[i][j]=c;
                if(c=='9'){
                    jeu->nbOiseaux+=1;
                }

            }

            fscanf(fichier,"%c",&c);
        }

    }




    int presse(Bouton clavier[], int num){
        Bouton b = clavier[num];
        if(b.precedent ==0 && b.actuel !=0){
            return 1;
        }
        {
            return 0;
        }

    }


    void majClavier(Bouton clavier[]){       //fonction mise à jour automatique du clavier
        for(int i=0; i<5; i++){

            Bouton b = clavier[i];
            b.precedent = b.actuel;
            b.actuel =GetAsyncKeyState(b.codeBouton);
            //printf("%d",b.actuel);
            clavier[i] = b;

        }
    }

    void masquerCurseur(){            //masquer le curseur durant l'actualisation des phases de jeu
        HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO info;
        info.dwSize = 100;
        info.bVisible = FALSE;
        SetConsoleCursorInfo(h, &info);
    }


    int main() {
        masquerCurseur();

        enum directions{GAUCHE, DROITE, HAUT, BAS, ESPACE};
        Bouton clavier[5]={
                {.codeBouton = VK_LEFT, .precedent=0, .actuel=0},
                {.codeBouton = VK_RIGHT, .precedent=0, .actuel=0},
                {.codeBouton = VK_UP, .precedent=0, .actuel=0},
                {.codeBouton = VK_DOWN, .precedent=0, .actuel=0},
                {.codeBouton = VK_SPACE, .precedent=0, .actuel=0}
        };


        Jeu* jeu = malloc(sizeof(Jeu));     //allocation de mémoire


        charger("snoopy1.txt",jeu);


        time_t debut = time(NULL); // Temps de départ du jeu

        char mouvement;
        int tempsRestant = TEMPS_IMPARTI;

        while (1) {
            majClavier(clavier);


            //mise à jour de la position en fonction des appuis clavier
            int dx = 0;
            int dy = 0;
            if(presse(clavier,GAUCHE)){
                dx=-1;
            }
            if(presse(clavier,DROITE)){
                dx=1;
            }
            if(presse(clavier,HAUT)){
                dy=-1;
            }
            if(presse(clavier,BAS)){
                dy=1;
            }
            if(presse(clavier,ESPACE)){
                jeu->plateau[jeu->joueurY][jeu->joueurX]='7';
            }
            //Calcul de la nouvelle position potentielle
            int nouvX = jeu->joueurX+dx;
            int nouvY = jeu->joueurY+dy;
            //Vériifier si il y a une collision
            if(nouvX<0 || nouvX>=LONGUEUR || nouvY<0 || nouvY>=LARGEUR){
                //Collision avec le bord
            }
            else{
                //Voir si le joueur tomberait sur une case déjà occupée
                char code = jeu->plateau[nouvY][nouvX];
                if(code!='1' && code!='4')
                {
                    if(code=='9'){
                        jeu->plateau[nouvY][nouvX] = '0';
                        jeu->nbOiseaux-=1;
                    }

                    jeu->joueurX = nouvX;
                    jeu->joueurY = nouvY;
                }
            }


            afficherPlateau(jeu);
        }



        return 0;
    }
}

void ChargerPartie() {
    printf("Charger une partie - Non implémente.\n");
    // Ajoutez ici le code pour charger une partie
}

void afficherMotDePasse() {
    printf("Mot de passe - Non implémente.\n");
    // Ajoutez ici le code pour gérer les mots de passe
}

void Scores() {
    printf("Scores - Non implémenté.\n");
    // Ajoutez ici le code pour afficher les scores
}

int main() {
    int choix;

    do {
        // Afficher le menu
        printf("Menu :\n");
        printf("1. Regles du jeu\n");
        printf("2. Lancer un nouveau jeu\n");
        printf("3. Charger une partie\n");
        printf("4. Mot de passe\n");
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
                break;
            case 3:
                ChargerPartie();
                break;
            case 4:
                afficherMotDePasse();
                break;
            case 5:
                Scores();
                break;
            case 6:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix non valide. Veuillez réessayer.\n");
        }
    } while (choix != 6);