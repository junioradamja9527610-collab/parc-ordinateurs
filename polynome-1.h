#ifndef POLYNOME_H
#define POLYNOME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TData TData;
typedef struct TElement TElement;
typedef struct Tlist Tlist;

struct TData {
    int coeff;
    int degre;
    char var;
};

struct TElement {
    TData data;
    TElement* next;
};

struct Tlist {
    TElement* debut;
    TElement* fin;
    int taille;
};

// Prototypes des fonctions :
Tlist* creerliste();
void creermonome(int* degre, int* coeff);
void insliste(Tlist* liste, int degre, int coeff, char var);
void affichage(Tlist* liste);
void triliste(Tlist* liste, int degre, int coeff, char var);
void saisirpolynome(Tlist* liste, char var);
Tlist* addition(Tlist* pol1, Tlist* pol2);
Tlist* addition_multiple(Tlist** tab, int n);

#endif
