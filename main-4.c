#include "polynome.h"

int main() {
    int n, x;
    char var;

    printf("\n========== Bienvenue ========\n ");
    printf("\n Programme d'addition de polynomes\n ");
    printf("1- Continuer\n  2- Quitter\n  Choix : ");
    scanf("%d", &x);

    if (x == 1) {
        printf("Entrez la variable du polynome : ");
        scanf(" %c", &var);
        printf("Entrez le nombre de polynomes a additionner : ");
        scanf("%d", &n);
        if (n <= 0) {
            printf("Nombre invalide.");
            return 1;
        }

        // Allocation dynamique du tableau de pointeurs
        Tlist** tab_liste = (Tlist**)malloc(n * sizeof(Tlist*));
        if (tab_liste == NULL) {
            printf("Erreur d'allocation mémoire.");
            return 1;
        }

        for (int i = 0; i < n; i++) {
            printf("==== Polynome %d ====\n ", i + 1);
            tab_liste[i] = creerliste();
            saisirpolynome(tab_liste[i], var);
        }

        Tlist* somme = addition_multiple(tab_liste, n);

        printf(" La somme est Q(%c) = ", var);
        affichage(somme);
        printf(" ");

        // Libération mémoire
        for (int i = 0; i < n; i++) {
            TElement* courant = tab_liste[i]->debut;
            while (courant != NULL) {
                TElement* suivant = courant->next;
                free(courant);
                courant = suivant;
            }
            free(tab_liste[i]);
        }
        free(tab_liste);

        TElement* courant = somme->debut;
        while (courant != NULL) {
            TElement* suivant = courant->next;
            free(courant);
            courant = suivant;
        }
        free(somme);

    } else {
        printf("--- Merci ---\n");
    }

    return 0;
}
