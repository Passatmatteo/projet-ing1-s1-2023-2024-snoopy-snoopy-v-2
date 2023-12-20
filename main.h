//
// Created by ramer on 14/12/2023.
//

#ifndef UNTITLED4_MAIN_H
#define UNTITLED4_MAIN_H


#define LONGUEUR 20
#define LARGEUR 10
#define DELAI_BALLE 2
#define DELAI_INVINCIBILITE 30
#define DELAI_TRANSITION 5 //secondes
#define TEMPS_IMPARTI 120 // Temps imparti en secondes


typedef enum Codejeu{EN_COURS,GAGNE, PERDU} Codejeu;
typedef enum Mode{DEPUIS_DEBUT, DEPUIS_SAUVEGARDE} Mode;


enum directions{GAUCHE, DROITE, HAUT, BAS, ESPACE, PAUSE, SAUVEGARDE};
typedef struct Bouton{
    int codeBouton;
    int precedent;
    int actuel;
} Bouton;


typedef struct {
    int x, y;  // Position de la balle sur le plateau
    int vx, vy; // Vitesse de la balle selon les axes x et y
    int timer;
} Balle;

typedef struct Jeu {
    char plateau[LARGEUR][LONGUEUR];
    int joueurX;
    int joueurY;
    int nbOiseaux;
    int score;
    int temps;
    int nbVies;
    int invincibilite;
    Balle balle;


}Jeu;

int boucle();
void Color(int couleurDuTexte,int couleurDeFond);
void nettoyerMemoireClavier();
int verifMotDePasse(char* mdp);
Codejeu boucleDeJeu(Jeu* jeu);
int jouer(char* cheminSauvegarde);

#endif //UNTITLED4_MAIN_H
