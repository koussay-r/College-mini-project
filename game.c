#include <stdio.h>
#define NB_LIGNES_CARTE 30
#define NB_COLONNES_CARTE 38
#define MAX_LIGNES_JOUEURS 5
#define MAX_LIGNES_UNITESJOUEURS 50
#define MAX_LIGNES_FICHES_TYPES_UNITES 10
#define MAX_LIGNES_UNITESMAGASIN 8
#define MAX_LIGNES_VILLAGES 14
#define MAX_LIGNES_PERIODES 10
#define NB_UNITES_RANG_INF_SUP 3
#define MAX_ATTAQUES 3
#define NB_RESISTANCES 6
#define NB_TYPES_TERRAINS 17
#define MAX_CAPACITES 3
#define MAX_TRAITS 3
typedef struct {
int idFicheTypeUnite;
int race; // humain, elfe, nain... voir la partie des races plus haut
char nom[20]; // archer, cavalier...
int prix;
int pvMax; // points de vie de cette unité (HP)
int mouvementsMax; // (MP)
int xpRequise; // expérience requise pour passer au niveau suivant
int niveau;
int alignement; // 0 : loyal (lawful), 1 : chaotique (chaotic), 2 : liminal (liminal) ou 3 : neutre (neutral)
int idFichesTypeUniteRangInf[0]; // 0 si non existant
int idFichesTypeUniteRangSup[0]; // 0 si non existant
Attaque attaques [MAX_ATTAQUES];
int resistances[NB_RESISTANCES] ; // ordres des cases // blade, pierce, impact, fire, cold, arcane
RelationTerrain relationsTerrains[NB_TYPES_TERRAINS]; // suivant les id : voir la partie terrains
int idCapacites[MAX_CAPACITES]; // voir la partie des capacités plus haut, 0 sinon existante
} FicheTypeUnite;
typedef struct {
int idAttaque; // Ces id sont des numéros d'ordre 1,2... Mettre 0 dans la case où on n’a pas d'attaque
char nomAttaque[20];
int degat;
int nombreAttaques;
int porteeAttaque; // 0 : melee , 1 : ranged
int typeAttaque ; // 0 : blade, 1 : pierce, 2 : Impact, 3 : fire, 4 : cold, 5 : arcane
int idAttaqueSpeciale1; // voir la partie des attaques spéciales plus haut
int idAttaqueSpeciale2; // voir la partie des attaques spéciales plus haut
} Attaque;
typedef struct{
int idTypeTerrain;
char symboleTerrain;
int codeAffichageTerrain;
char nomTerrain[20];
} TypeTerrain;
typedef struct {
int idTypeTerrain;
int coutMouvement;
int defense;
} RelationTerrain;
typedef struct {
int idJoueur;
char symbole; // cas par défaut : J : joueur humain, E : joueur ennemi
int or;
int groupe_allies; // 1 : alliés joueur humain, 2 : alliés joueur pc ennemi
int nombreVillages;
int nombreUnites;
int entretien;
int revenu;
} Joueur;
typedef struct {
int idUnite; // Ces id sont des numéros d'ordre 1,2... Mettre 0 dans la case où on n’a pas d'unité
int idFicheTypeUnite;
int idJoueur;
char nomUnite[20];
int active; // 1 si l’unité participe au jeu courant, 0 si l’unité ne participe pas on peut alors la rappeler
int rang; // 0 : unité normale, 1 : unité chef
int ligne;
int colonne;
int traits[MAX_TRAITS]; // voir la partie des traits plus haut
int pv;
int pvMax;
int mouvements;
int mouvementsMax;
int xp;
int xpRequise;
int niveau;
int finTour;
} UniteJoueur;
typedef struct {
int idUniteMagasin;// Ces id sont des numéros d'ordre 1,2... Mettre 0 dans la case où on n’a pas
//d'unité
int idFicheTypeUnite;
int idJoueurAutorise;
} UniteMagasin;
typedef struct {
int idUnite;
int idTypeTerrain;
int codeAffichageTerrain; // code clavier du caractère à afficher dans la carte
char symboleJoueur;
int marque;
} CelluleCarte;
typedef struct {
int idVillage; // ordonnées dans l’ordre de parcours de la carte de haut en bas et de gauche à droite
int ligne;
int colonne;
int idJoueurProprietaire; // idJoueur qui possède le village, 0 sinon
} Village;
typedef struct
{
int numOrdre;
char moment[20];
int bonus[4]; // cases : 0 -> bonus lawful, 1 -> bonus chaotic, 2 -> bonus liminal, 3 -> bonus neutral
} Periode;
int chargerUnitesMagasinVersTableau(UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN], char* nomFichier){
    FILE *file;
    file=fopen(*nomFichier,"r");
    if(file==NULL){
        printf("erreur loading file");
        return 0;
    }
    else{
        int Numbr_lignes=0;
        while(fgets(unitesMagasin,MAX_LIGNES_UNITESMAGASIN,nomFichier)){
            scanf("%d %d %d",&unitesMagasin[Numbr_lignes].idFicheTypeUnite,&unitesMagasin[Numbr_lignes].idJoueurAutorise,&unitesMagasin[Numbr_lignes].idUniteMagasin);
            Numbr_lignes++;
        }
        return Numbr_lignes;
    }
    

}
void main(){
    
}