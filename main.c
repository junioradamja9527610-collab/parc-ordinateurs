/* main.c */
#include <stdio.h>
#include <stdlib.h>
#include "ordinateur.h"

void menu(Parc *p) {
    int choix;
    do {
        puts("\n===== GESTION DU PARC =====");
        puts("1. Ajouter un ordinateur");
        puts("2. Lister les ordinateurs");
        puts("3. Afficher les détails");
        puts("4. Supprimer un ordinateur");
        puts("5. Répartition par spécialité");
        puts("6. Répartition par marque");
        puts("7. Classer par vitesse CPU");
        puts("8. Valeur totale du parc");
        puts("0. Quitter");
        printf("Choix : ");
        scanf("%d", &choix);
        while (getchar() != '\n');   /* vider stdin */

        switch (choix) {
            case 1: ajouter(p);            break;
            case 2: lister(p);             break;
            case 3: afficher_details(p);   break;
            case 4: supprimer(p);          break;
            case 5: repartir_specialite(p);break;
            case 6: repartir_marque(p);    break;
            case 7: classer_cpu(p);        break;
            case 8: valeur_totale(p);      break;
            case 0: puts("Au revoir.");    break;
            default:puts("Choix invalide.");
        }
    } while (choix != 0);
}

int main(void) {
    Parc *parc = creer_parc();
    menu(parc);
    liberer_parc(parc);
    return EXIT_SUCCESS;
}