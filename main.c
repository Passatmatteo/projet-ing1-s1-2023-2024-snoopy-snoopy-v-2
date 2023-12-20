#include <stdio.h> //
#include <conio.h> // Cette bibliothèque est utilisée pour la fonction getch() sous Windows
#include <time.h> // Cette bibliothèque est utilisée pour mesurer le temps
#include <windows.h>
#include <strings.h>
#include "main.h"
#include <utilapiset.h>


int frame;
int pause=0;
int niv;
#define NOMBRE_NIVEAUX 3
char* niveaux[] = {"../snoopy1.txt", "../snoopy2.txt", "../snoopy3.txt"};
char* passwords[] = {"CHARLIE", "LINUS", "WOODSTOCK"};


//https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
Bouton clavier[7]={
        {.codeBouton = VK_LEFT, .precedent=0, .actuel=0},
        {.codeBouton = VK_RIGHT, .precedent=0, .actuel=0},
        {.codeBouton = VK_UP, .precedent=0, .actuel=0},
        {.codeBouton = VK_DOWN, .precedent=0, .actuel=0},
        {.codeBouton = VK_SPACE, .precedent=0, .actuel=0},
        {.codeBouton = 0x50, .precedent=0, .actuel=0},
        {.codeBouton = 0x53, .precedent=0, .actuel=0}
};


void Color(int couleurDuTexte,int couleurDeFond)
{
HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
SetConsoleTextAttribute(H,couleurDeFond*16+couleurDuTexte);
}




void gotoXY(int x,int y){
    COORD pos = {x, y};
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, pos);
}


void nettoyerMemoireClavier(){
    https://stackoverflow.com/questions/7898215/how-can-i-clear-an-input-buffer-in-c
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}


void sonExplosion(){
    Beep(300,50);
    Beep(100,50);
}

void sonOiseau(){
    Beep(1000,20);
    Beep(1500,20);
    Beep(2500,20);
}


// Fonction pour afficher le plateau de jeu

void afficherPlateau(Jeu* monjeu, Codejeu phase) {
    if(phase==PERDU){
        Color(7,0);
        gotoXY(30,4);printf("                             ");
        gotoXY(30,5);printf("    _____           _        ");
        gotoXY(30,6);printf("   |  _  |___ ___ _| |_ _    ");
        gotoXY(30,7);printf("   |   __| -_|  _| . | | |   ");
        gotoXY(30,8);printf("   |__|  |___|_| |___|___|   ");
        gotoXY(30,9);printf("                             ");

        return;
    }

    if(phase==GAGNE){
        Color(7,0);
        gotoXY(30,4);printf("                             ");
        gotoXY(30,5);printf("   Bravo, vous avez gagne    ");
        gotoXY(30,6);printf("   le niveau %d              ", niv+1);
        gotoXY(30,7);printf("                             ");
        if(niv<NOMBRE_NIVEAUX-1){
            gotoXY(30,8);printf("   Veuillez noter le mot de passe suivant:   ");
            gotoXY(30,9);printf("     %s", passwords[niv+1]);
        }


        return;
    }


    gotoXY(30,5);
    if(monjeu->temps>=10){
        Color(7,0);
    }
    else{
        Color(0,4);
    }

    printf("Temps: %d ",monjeu->temps);
    gotoXY(30,6);
    if(pause){
        printf("PAUSE (appuyer sur P pour reprendre)");
    }
    else{
        printf("                                     ");
    };
    Color(11,0);
    gotoXY(0,0);
    for(int j=0; j<LONGUEUR+2; j++){
        Color(11,11);
        printf("#");
        Color(11,0);
    }
    printf("\n");
    for (int i = 0; i < LARGEUR; i++) {
        Color(11,11);
        printf("#");
        Color(11,0);

        for (int j = 0; j < LONGUEUR; j++) {
            char code = monjeu->plateau[i][j];
            char aff;
            int fond=0;
            int pplan=11;
            if(monjeu->joueurX == j && monjeu->joueurY == i){
                //Si Snoopy n'est pas invincible, l'afficher de façon normale
                if(monjeu->invincibilite==0){
                    aff = '@';
                    pplan=10;
                }
                //sinon, le faire clignoter (en utilisant la variable globale "frame")
                else{
                    if(frame%2==0){
                        aff = '@';
                    }
                    else{
                        aff = ' ';
                    }

                    pplan=10;
                }


            }
            else if(monjeu->balle.x == j && monjeu->balle.y == i){
                aff = 'O';
                fond=0;
                pplan=12;
            }
            else{
                switch(code){

                    case '0': aff=' '; break;   //case vide
                    case '1': aff='X';fond=11;pplan=0; break;   //bloc cassable
                    case '2': aff ='*';fond=5;pplan=5; break;  //bloc poussable
                    case '4': aff ='*';fond=11;pplan=11; break;  //bloc invincible
                    case '9':
                        if(frame%10<5){
                               aff ='V';
                        }
                        else{
                            aff ='^';
                        }
                            pplan=6;
                    break;  //oiseaux
                    case '3':aff ='!';fond=9;pplan=0; break;  //bloc piégé
                    default: aff=' '; break;
                }

            }
            Color(pplan,fond);
            printf("%c", aff);
            Color(11,0);

        }
        Color(11,11);
        printf("#");
        Color(11,0);
        printf("\n");
    }
    for(int j=0; j<LONGUEUR+2; j++){
        Color(11,11);
        printf("#");
        Color(11,0);
    }
    printf("\nNb Oiseaux:%d",monjeu->nbOiseaux);
    printf("\nScore: %d",monjeu->score);
    printf("\nVies: %d",monjeu->nbVies);


}

void charger(char* chemin, Jeu* jeu, Mode mode){
    jeu->nbOiseaux=0;
    jeu->temps=TEMPS_IMPARTI;
    FILE* fichier;
    fichier = fopen(chemin, "r");
    if(!fichier){
        printf("ERREUR!");
        return;
    }

    if(mode==DEPUIS_DEBUT){

        jeu->nbVies=3;
        int a;
        fscanf(fichier, "%d", &a);
    }
    else{
        fscanf(fichier, "%d", &niv);
    }
    fscanf(fichier, "%d", &(jeu->joueurX));
    fscanf(fichier, "%d", &(jeu->joueurY));
    fscanf(fichier, "%d", &(jeu->balle.x));
    fscanf(fichier, "%d", &(jeu->balle.y));

    if(mode==DEPUIS_DEBUT){
        int a;
        fscanf(fichier, "%d", &a);
        fscanf(fichier, "%d", &a);
        fscanf(fichier, "%d", &a);
    }
    else{
        fscanf(fichier, "%d", &(jeu->temps));
        fscanf(fichier, "%d", &(jeu->nbVies));
        fscanf(fichier, "%d", &(jeu->score));
    }



    jeu->balle.vx=1;
    jeu->balle.vy=1;
    jeu->balle.timer = DELAI_BALLE;
    jeu->invincibilite = 0;
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
    fclose(fichier);
}




int presse(Bouton clavier[], int num){
    Bouton b = clavier[num];
    if(b.precedent&0x8000 && !(b.actuel&0x8000)){
        return 1;
    }
    {
        return 0;
    }

}


void majClavier(Bouton clavier[]){       //fonction mise à jour automatique du clavier
    for(int i=0; i<7; i++){
        Bouton b = clavier[i];
        b.precedent = b.actuel;
        b.actuel =GetAsyncKeyState(b.codeBouton);
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


void majBalle(Balle* balle){
    balle->timer--;
    if(balle->timer>0){
        return;
    }
    balle->timer=DELAI_BALLE;
    balle->x += balle->vx;
    if(balle->x>19){
        balle->x=18;
        balle->vx*=-1;
    }
    else if(balle->x<0){
        balle->x=1;
        balle->vx*=-1;
    }
    balle->y += balle->vy;
    if(balle->y>9){
        balle->y=8;
        balle->vy*=-1;

    }
    else if(balle->y<0){
        balle->y=1;
        balle->vy*=-1;
    }

}

char codeTuile(Jeu* jeu,int x,int y){
    if(x<0 || x>19 || y<0 || y>9){
        return '0';
    }
    else{
        return jeu->plateau[y][x];
    }
}

int verifMotDePasse(char* mdp){
    for(int i=0; i<NOMBRE_NIVEAUX; i++){
        if(strcasecmp(mdp,passwords[i])==0){
            niv = i;
            jouer("");
            return 1;
        }

    }
    return 0;
}

void sauvegarder(Jeu* jeu){
    char chemin[50]="../";
    char entree[45];
    Color(7,1);
    gotoXY(5,5);printf("                               ");
    gotoXY(5,6);printf("   Nom de la sauvegarde:       ");
    gotoXY(5,7);printf("                               ");
    gotoXY(5,8);printf("                               ");
    gotoXY(8,7); scanf("%s", entree);
    strcat(chemin,entree);



    FILE* fichier;
    fichier = fopen(chemin, "w");
    if(!fichier){
        printf("ERREUR!");
        return;
    }

    int a;
    fprintf(fichier, "%d\n", niv);
    fprintf(fichier, "%d %d\n", jeu->joueurX, jeu->joueurY);
    fprintf(fichier, "%d %d\n", jeu->balle.x, jeu->balle.y);
    fprintf(fichier, "%d %d %d\n", jeu->temps, jeu->nbVies, jeu->score);
    for (int i = 0; i < LARGEUR; i++) {
        for (int j = 0; j < LONGUEUR; j++) {
                fprintf(fichier,"%c",jeu->plateau[i][j]);
        }
        fprintf(fichier, "\n");



    }
    fclose(fichier);
    Color(7,0);
    system("cls");
}




int jouer(char* cheminSauvegarde) {
    frame=0;
    masquerCurseur();
    system("cls");


    Jeu* jeu = malloc(sizeof(Jeu));     //allocation de mémoire
    Balle b;
    jeu->balle=b;
    jeu->score=0;

    if(strcmp(cheminSauvegarde,"")==0){
        niv = 0;
        charger(niveaux[niv],jeu, DEPUIS_DEBUT);
    }
    else{
        //charger(niveaux[niv],jeu);
        charger(cheminSauvegarde, jeu, DEPUIS_SAUVEGARDE);
    }

    // Temps de départ du jeu
    while(boucleDeJeu(jeu)==GAGNE && niv<NOMBRE_NIVEAUX-1){
        niv++;
        charger(niveaux[niv],jeu, DEPUIS_DEBUT);
    }



}



Codejeu boucleDeJeu(Jeu* jeu){
    system("cls");
    Codejeu phase = EN_COURS;
    time_t debutTransition=0;
    time_t debut = time(NULL);
    debut = time(NULL)+jeu->temps-TEMPS_IMPARTI;
    while (1) {
        while(debutTransition){

            if(difftime(time(NULL),debutTransition)>=DELAI_TRANSITION){
                system("cls");
                return phase;
            }
            afficherPlateau(jeu,phase);
        }

        while(pause){
            debut = time(NULL)+jeu->temps-TEMPS_IMPARTI;
            majClavier(clavier);
            if(presse(clavier,PAUSE)){
                pause=0;
            }
            afficherPlateau(jeu,phase);
        }


        frame++;
        time_t maintenant = time(NULL);
        double tempsEcoule = difftime(maintenant,debut);
        jeu->temps = TEMPS_IMPARTI - tempsEcoule;


        majBalle(&(jeu->balle));
        if(jeu->invincibilite>0){
            jeu->invincibilite--;
        }
//printf("%d.....",jeu->joueurX);
        //mise à jour de la position en fonction des appuis clavier
        int dx = 0;
        int dy = 0;

        majClavier(clavier);
        if(presse(clavier,SAUVEGARDE)){
            sauvegarder(jeu);
            debut = time(NULL)+jeu->temps-TEMPS_IMPARTI;
        }
        if(presse(clavier,PAUSE)){
            pause=1;
        }
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
            int x = jeu->joueurX;
            int y = jeu->joueurY;
            int haut=codeTuile(jeu,x,y-1);
            if(haut=='1'){
                jeu->plateau[y-1][x]='0';
            }
            int bas=codeTuile(jeu,x,y+1);
            if(bas=='1'){
                jeu->plateau[y+1][x]='0';
            }
            int gauche=codeTuile(jeu,x-1,y);
            if(gauche=='1'){
                jeu->plateau[y][x-1]='0';
            }
            int droite=codeTuile(jeu,x+1,y);
            if(droite=='1'){
                jeu->plateau[y][x+1]='0';
            }

        }
        //Calcul de la nouvelle position potentielle
        int nouvX = jeu->joueurX+dx;
        int nouvY = jeu->joueurY+dy;
        //Vériifier si il y a une collision
        if(nouvX<0 || nouvX>=LONGUEUR || nouvY<0 || nouvY>=LARGEUR){
            //Collision avec le bord: ne rien faire
        }
        else{
            //Voir si le joueur tomberait sur une case déjà occupée
            char code = jeu->plateau[nouvY][nouvX];
            if(code!='1' && code!='4' && code!='2')
            {
                if(code=='9'){
                    sonOiseau();
                    jeu->plateau[nouvY][nouvX] = '0';
                    jeu->nbOiseaux-=1;
                }
                if(code=='3'){
                    sonExplosion();
                    jeu->plateau[nouvY][nouvX] = '0';
                    if(jeu->invincibilite ==0){
                        jeu->nbVies--;
                        jeu->invincibilite=DELAI_INVINCIBILITE;
                    }

                }

                jeu->joueurX = nouvX;
                jeu->joueurY = nouvY;
            }
                //Bloc poussable
            else if(code=='2')
            {
                //Nouvelle position du bloc poussable
                int nx=nouvX+dx;
                int ny=nouvY+dy;
                if(nx>=0 && nx<=19 && ny>=0 && ny<=9 && codeTuile(jeu,nx,ny)=='0'){
                    jeu->joueurX = nouvX;
                    jeu->joueurY = nouvY;
                    jeu->plateau[nouvY][nouvX]='0';
                    jeu->plateau[ny][nx]='4';

                }
            }
        }

        //Tester la collision entre Snoopy et la balle
        if(jeu->invincibilite ==0 && jeu->joueurX == jeu->balle.x && jeu->joueurY == jeu->balle.y){
            sonExplosion();
            jeu->nbVies--;
            jeu->invincibilite=DELAI_INVINCIBILITE;
        }
        if (jeu->nbVies == 0 ){
            debutTransition = time(NULL);
            phase = PERDU;

        }
        if (jeu->nbOiseaux == 0 ){
            debutTransition = time(NULL);
            phase = GAGNE;
            jeu->score += jeu->temps*100;
        }
        afficherPlateau(jeu,phase);
    }
}


