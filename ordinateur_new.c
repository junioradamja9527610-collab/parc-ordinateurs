/* ordinateur.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ordinateur.h"

/* ── Utilitaires ─────────────────────────────────────────── */
static void vider_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void lire_chaine(const char *msg, char *dest, int taille) {
    printf("%s", msg);
    if (fgets(dest, taille, stdin))
        *(dest + strcspn(dest, "\n")) = '\0';
}

static int lire_int(const char *msg) {
    int v;
    printf("%s", msg);
    scanf("%d", &v);
    vider_stdin();
    return v;
}

static float lire_float(const char *msg) {
    float v;
    printf("%s", msg);
    scanf("%f", &v);
    vider_stdin();
    return v;
}

/* ── Parc ────────────────────────────────────────────────── */
Parc *creer_parc(void) {
    Parc *p = malloc(sizeof(Parc));
    if (!p) { perror("malloc parc"); exit(EXIT_FAILURE); }
    p->tab = malloc(MAX_PARC * sizeof(Ordinateur *));
    if (!p->tab) { perror("malloc tab"); exit(EXIT_FAILURE); }
    p->nb = 0;
    return p;
}

void liberer_parc(Parc *p) {
    for (int i = 0; i < p->nb; i++)
        free(*(p->tab + i));
    free(p->tab);
    free(p);
}

/* ── CRUD ────────────────────────────────────────────────── */
void ajouter(Parc *p) {
    if (p->nb >= MAX_PARC) { puts("Parc plein."); return; }

    Ordinateur *o = malloc(sizeof(Ordinateur));
    if (!o) { perror("malloc ordinateur"); return; }

    o->carte = lire_int("Numéro de carte    : ");
    lire_chaine("Spécialité         : ", o->specialite, MAX_STR);
    lire_chaine("Numéro de série    : ", o->serie,      MAX_STR);
    lire_chaine("Marque             : ", o->marque,     MAX_STR);
    o->cpu    = lire_float("Vitesse CPU (GHz)  : ");
    o->ram    = lire_int  ("RAM (Go)           : ");
    o->disque = lire_int  ("Disque dur (Go)    : ");
    o->prix   = lire_int  ("Prix (F CFA)       : ");

    *(p->tab + p->nb) = o;
    p->nb++;
    puts("Ordinateur ajouté.");
}

void lister(const Parc *p) {
    if (!p->nb) { puts("Parc vide."); return; }
    printf("\n%-4s %-10s %-12s %-10s %6s %5s %7s %10s\n",
           "No", "Carte", "Spécialité", "Marque",
           "CPU", "RAM", "Disque", "Prix CFA");
    puts("--------------------------------------------------------------"
         "----------------");
    for (int i = 0; i < p->nb; i++) {
        Ordinateur *o = *(p->tab + i);
        printf("%-4d %-10d %-12s %-10s %5.2f %4dGo %6dGo %10d\n",
               i + 1, o->carte, o->specialite, o->marque,
               o->cpu, o->ram, o->disque, o->prix);
    }
}

void afficher_details(const Parc *p) {
    if (!p->nb) { puts("Parc vide."); return; }
    int idx = lire_int("Numéro de l'ordinateur : ") - 1;
    if (idx < 0 || idx >= p->nb) { puts("Indice invalide."); return; }
    Ordinateur *o = *(p->tab + idx);
    printf("\n--- Détails ---\n"
           "Carte      : %d\n"
           "Spécialité : %s\n"
           "Série      : %s\n"
           "Marque     : %s\n"
           "CPU        : %.2f GHz\n"
           "RAM        : %d Go\n"
           "Disque     : %d Go\n"
           "Prix       : %d F CFA\n",
           o->carte, o->specialite, o->serie, o->marque,
           o->cpu, o->ram, o->disque, o->prix);
}

void supprimer(Parc *p) {
    if (!p->nb) { puts("Parc vide."); return; }
    int idx = lire_int("Numéro à supprimer : ") - 1;
    if (idx < 0 || idx >= p->nb) { puts("Indice invalide."); return; }
    free(*(p->tab + idx));
    for (int i = idx; i < p->nb - 1; i++)
        *(p->tab + i) = *(p->tab + i + 1);
    p->nb--;
    puts("Ordinateur supprimé.");
}

/* ── Répartitions ────────────────────────────────────────── */
void repartir_specialite(const Parc *p) {
    if (!p->nb) { puts("Parc vide."); return; }

    char specs[MAX_PARC][MAX_STR];
    int  counts[MAX_PARC];
    int  ns = 0;

    for (int i = 0; i < p->nb; i++) {
        const char *sp = (*(p->tab + i))->specialite;
        int found = 0;
        for (int j = 0; j < ns; j++) {
            if (strcmp(*(specs + j), sp) == 0) {
                *(counts + j) += 1;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(*(specs + ns), sp);
            *(counts + ns) = 1;
            ns++;
        }
    }
    puts("\nRépartition par spécialité :");
    for (int j = 0; j < ns; j++)
        printf("  %-20s : %d\n", *(specs + j), *(counts + j));
}

void repartir_marque(const Parc *p) {
    if (!p->nb) { puts("Parc vide."); return; }

    char marqs[MAX_PARC][MAX_STR];
    int  counts[MAX_PARC];
    int  nm = 0;

    for (int i = 0; i < p->nb; i++) {
        const char *mk = (*(p->tab + i))->marque;
        int found = 0;
        for (int j = 0; j < nm; j++) {
            if (strcmp(*(marqs + j), mk) == 0) {
                *(counts + j) += 1;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(*(marqs + nm), mk);
            *(counts + nm) = 1;
            nm++;
        }
    }
    puts("\nRépartition par marque :");
    for (int j = 0; j < nm; j++)
        printf("  %-15s : %d\n", *(marqs + j), *(counts + j));
}

/* ── Tri (bulles) par CPU croissant ──────────────────────── */
void classer_cpu(Parc *p) {
    for (int i = 0; i < p->nb - 1; i++)
        for (int j = 0; j < p->nb - 1 - i; j++)
            if ((*(p->tab + j))->cpu > (*(p->tab + j + 1))->cpu) {
                Ordinateur *tmp   = *(p->tab + j);
                *(p->tab + j)     = *(p->tab + j + 1);
                *(p->tab + j + 1) = tmp;
            }
    puts("Liste classée par vitesse CPU (croissant).");
    lister(p);
}

/* ── Valeur totale ───────────────────────────────────────── */
void valeur_totale(const Parc *p) {
    long total = 0;
    for (int i = 0; i < p->nb; i++)
        total += (*(p->tab + i))->prix;
    printf("\nValeur totale du parc : %ld F CFA\n", total);
}
