#include "polynome.h"

// Création et initialisation de la liste
Tlist* creerliste() {
    Tlist* liste = (Tlist*)malloc(sizeof(Tlist));
    if (liste == NULL) {
        printf("Erreur d'allocation !!!");
        exit(-1);
    }
    liste->debut = NULL;
    liste->fin = NULL;
    liste->taille = 0;
    return liste;
}

// Saisie d'un monôme
void creermonome(int* degre, int* coeff) {
    printf("Entrez le degre du monome :\n ");
    scanf("%d", degre);
    printf("Entrez le coefficient :\n ");
    scanf("%d", coeff);
}

// Insertion en fin de liste (utilisée uniquement en interne par triliste)
void insliste(Tlist* liste, int degre, int coeff, char var) {
    TElement* new = (TElement*)malloc(sizeof(TElement));
    if (new == NULL) {
        printf("Erreur d'allocation !!!");
        exit(-1);
    }
    new->data.degre = degre;
    new->data.coeff = coeff;
    new->data.var = var;
    new->next = NULL;

    if (liste->taille == 0) {
        liste->debut = new;
        liste->fin = new;
    } else {
        liste->fin->next = new;
        liste->fin = new;
    }
    liste->taille++;
}

// Affichage du polynôme (ordre décroissant des degrés)
void affichage(Tlist* liste) {
    TElement* courant = liste->debut;
    if (courant == NULL) {
        printf("0");
        return;
    }

    while (courant != NULL) {
        int coeff = courant->data.coeff;
        int degre = courant->data.degre;
        char var = courant->data.var;

        if (courant != liste->debut && coeff > 0) {
            printf("+");
        }

        if (coeff == 0) {
            // Ne pas afficher
        }
        else if (degre == 0) {
            printf("%d", coeff);
        }
        else if (degre == 1) {
            if (coeff == 1)       printf("%c", var);
            else if (coeff == -1) printf("-%c", var);
            else                  printf("%d%c", coeff, var);
        }
        else {
            if (coeff == 1)       printf("%c^%d", var, degre);
            else if (coeff == -1) printf("-%c^%d", var, degre);
            else                  printf("%d%c^%d", coeff, var, degre);
        }

        courant = courant->next;
    }
}

// Insertion triée (ordre décroissant des degrés) avec fusion des monômes de même degré
void triliste(Tlist *liste,int degre,int coeff,char var)
{
    TElement *prec = NULL;
    TElement *courant = liste->debut;

    while(courant && courant->data.degre > degre)
    {
        prec = courant;
        courant = courant->next;
    }

    if(courant && courant->data.degre == degre)
    {
        courant->data.coeff += coeff;
        return;
    }

    TElement *new = malloc(sizeof(TElement));

    if(new == NULL)
    {
        printf("Erreur allocation\n");
        exit(EXIT_FAILURE);
    }

    new->data.coeff = coeff;
    new->data.degre = degre;
    new->data.var = var;

    if(prec == NULL)
    {
        new->next = liste->debut;
        liste->debut = new;

        if(liste->fin == NULL)
            liste->fin = new;
    }
    else
    {
        new->next = courant;
        prec->next = new;

        if(courant == NULL)
            liste->fin = new;
    }

    liste->taille++;
}

// Saisie interactive d'un polynôme
void saisirpolynome(Tlist* liste, char var) {
    int n, degre, coeff;
    printf("Entrez le nombre de termes :\n ");
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        creermonome(&degre, &coeff);
        triliste(liste, degre, coeff, var);
        printf("P(%c)_%d = ", var, i+1);
        affichage(liste);
        printf(" ");
    }
}

// Addition de deux polynômes (listes triées en degré décroissant)
Tlist* addition(Tlist* pol1, Tlist* pol2) {
    TElement* temp1 = pol1->debut;
    TElement* temp2 = pol2->debut;
    Tlist* resultat = creerliste();
    TElement** courant = &resultat->debut;

    while (temp1 != NULL && temp2 != NULL) {
        int cree = 0;
        if (temp1->data.degre > temp2->data.degre) {
            *courant = (TElement*)malloc(sizeof(TElement));
            (*courant)->data = temp1->data;
            (*courant)->next = NULL;
            temp1 = temp1->next;
            cree = 1;
        }
        else if (temp1->data.degre < temp2->data.degre) {
            *courant = (TElement*)malloc(sizeof(TElement));
            (*courant)->data = temp2->data;
            (*courant)->next = NULL;
            temp2 = temp2->next;
            cree = 1;
        }
        else {
            int som = temp1->data.coeff + temp2->data.coeff;
            if (som != 0) {
                *courant = (TElement*)malloc(sizeof(TElement));
                (*courant)->data.coeff = som;
                (*courant)->data.degre = temp1->data.degre;
                (*courant)->data.var = temp1->data.var;
                (*courant)->next = NULL;
                cree = 1;
            }
            temp1 = temp1->next;
            temp2 = temp2->next;
        }

        if (cree) {
            resultat->taille++;
            courant = &(*courant)->next;
        }
    }

    // Copie des termes restants
    while (temp1 != NULL) {
        *courant = (TElement*)malloc(sizeof(TElement));
        (*courant)->data = temp1->data;
        (*courant)->next = NULL;
        courant = &(*courant)->next;
        resultat->taille++;
        temp1 = temp1->next;
    }

    while (temp2 != NULL) {
        *courant = (TElement*)malloc(sizeof(TElement));
        (*courant)->data = temp2->data;
        (*courant)->next = NULL;
        courant = &(*courant)->next;
        resultat->taille++;
        temp2 = temp2->next;
    }

    // Mise à jour de fin
    if (resultat->debut == NULL) {
        resultat->fin = NULL;
    } else {
        TElement* p = resultat->debut;
        while (p->next != NULL) p = p->next;
        resultat->fin = p;
    }

    return resultat;
}

// Addition de plusieurs polynômes (tableau de pointeurs, n >= 1)
Tlist* addition_multiple(Tlist** tab, int n) {
    if (n == 0) return NULL;
    if (n == 1) {
        // Copie du seul polynôme
        Tlist* copie = creerliste();
        TElement* e = tab[0]->debut;
        TElement** pp = &copie->debut;
        while (e != NULL) {
            *pp = (TElement*)malloc(sizeof(TElement));
            (*pp)->data = e->data;
            (*pp)->next = NULL;
            copie->taille++;
            pp = &(*pp)->next;
            e = e->next;
        }
        if (copie->debut != NULL) {
            TElement* p = copie->debut;
            while (p->next != NULL) p = p->next;
            copie->fin = p;
        }
        return copie;
    }

    // Initialisation avec le premier polynôme (copie)
    Tlist* somme = creerliste();
    TElement* e = tab[0]->debut;
    TElement** pp = &somme->debut;
    while (e != NULL) {
        *pp = (TElement*)malloc(sizeof(TElement));
        (*pp)->data = e->data;
        (*pp)->next = NULL;
        somme->taille++;
        pp = &(*pp)->next;
        e = e->next;
    }
    if (somme->debut != NULL) {
        TElement* p = somme->debut;
        while (p->next != NULL) p = p->next;
        somme->fin = p;
    }

    // Additionner les polynômes suivants
    for (int i = 1; i < n; i++) {
        Tlist* tmp = addition(somme, tab[i]);

        // Libération de l'ancienne somme
        TElement* courant = somme->debut;
        while (courant != NULL) {
            TElement* suivant = courant->next;
            free(courant);
            courant = suivant;
        }
        free(somme);

        somme = tmp;
    }

    return somme;
}
