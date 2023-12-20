typedef struct {
    int x, y;  // Position de la balle sur le plateau
    int vx, vy; // Vitesse de la balle selon les axes x et y
} Balle;

// Fonction principale
int main() {
    Balle balle = {4, 4, 1, 1};  // Position initiale et vitesse
// Boucle principale
    while (1) {

// Mettre à jour la position de la balle
        balle.x += balle.vx;
        balle.y += balle.vy;

// Gérer les rebonds sur les bords
        printf("%c",0x01);
        if (balle.x < 0 || balle.x >= LARGEUR_PLATEAU) {
            balle.vx = -balle.vx;
        }
        if (balle.y < 0 || balle.y >= HAUTEUR_PLATEAU) {
            balle.vy = -balle.vy;
        }
// Pause pour ralentir le mouvement
        for (int i = 0; i < 10000000; i++) {
        }
    }

}
