/* ordinateur.h */
#ifndef ORDINATEUR_H
#define ORDINATEUR_H

#define MAX_PARC 300
#define MAX_STR  50

typedef struct {
    int    carte;
    char   specialite[MAX_STR];
    char   serie[MAX_STR];
    char   marque[MAX_STR];
    float  cpu;      /* GHz */
    int    ram;      /* Go  */
    int    disque;   /* Go  */
    int    prix;     /* F CFA */
} Ordinateur;

/* Parc = tableau dynamique de pointeurs */
typedef struct {
    Ordinateur **tab;
    int          nb;
} Parc;

/* Prototypes */
Parc       *creer_parc(void);
void        liberer_parc(Parc *p);

void        ajouter(Parc *p);
void        lister(const Parc *p);
void        afficher_details(const Parc *p);
void        supprimer(Parc *p);

void        repartir_specialite(const Parc *p);
void        repartir_marque(const Parc *p);
void        classer_cpu(Parc *p);
void        valeur_totale(const Parc *p);

void        menu(Parc *p);

#endif