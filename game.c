#include <stdio.h>
#include <string.h>
#include <math.h>

// constants
#define ID_TYPE_TERRAIN_CHATEAU 1
#define ID_TYPE_TERRAIN_VILLAGE 16
#define ID_TYPE_TERRAIN_DONJON 17
#define MAX_ELOIGNEMENT_CHATEAU_DONJON 2
#define NB_LIGNES_CARTE 30
#define NB_COLONNES_CARTE 38
#define MAX_LIGNES_JOUEURS 5
#define MAX_LIGNES_UNITESJOUEURS 50
#define MAX_LIGNES_FICHES_TYPES_UNITES 10
#define MAX_LIGNES_UNITESMAGASIN 8
#define MAX_LIGNES_VILLAGES 14
#define NB_LIGNES_PERIODES 6
#define NB_UNITES_RANG_INF_SUP 3
#define MAX_ATTAQUES 3
#define NB_RESISTANCES 6
#define NB_TYPES_TERRAINS 17
#define MAX_CAPACITES 3
#define MAX_TRAITS 3
#define MAX_EFFETS_ATTAQUES_SUBITES 10
// objects
typedef struct
{
    int idAttaque; // Ces id sont des numéros d'ordre 1,2... Mettre 0 dans la case où on n’a pas d'attaque
    char nomAttaque[20];
    int degat;
    int nombreAttaques;
    int porteeAttaque;      // 0 : melee , 1 : ranged
    int typeAttaque;        // 0 : blade, 1 : pierce, 2 : Impact, 3 : fire, 4 : cold, 5 : arcane
    int idAttaqueSpeciale1; // voir la partie des attaques spéciales plus haut
    int idAttaqueSpeciale2; // voir la partie des attaques spéciales plus haut
} Attaque;
typedef struct
{
    int idTypeTerrain;
    int coutMouvement;
    int defense;
} RelationTerrain;
typedef struct
{
    int idFicheTypeUnite;
    int race;     // humain, elfe, nain... voir la partie des races plus haut
    char nom[20]; // archer, cavalier...
    int prix;
    int pvMax;         // points de vie de cette unité (HP)
    int mouvementsMax; // (MP)
    int xpRequise;     // expérience requise pour passer au niveau suivant
    int niveau;
    int alignement;                                       // 0 : loyal (lawful), 1 : chaotique (chaotic), 2 : liminal (liminal) ou 3 : neutre (neutral)
    int idFichesTypeUniteRangInf[NB_UNITES_RANG_INF_SUP]; // 0 si non existant
    int idFichesTypeUniteRangSup[NB_UNITES_RANG_INF_SUP]; // 0 si non existant
    Attaque attaques[MAX_ATTAQUES];
    int resistances[NB_RESISTANCES];                      // ordres des cases // blade, pierce, impact, fire, cold, arcane
    RelationTerrain relationsTerrains[NB_TYPES_TERRAINS]; // suivant les id : voir la partie terrains
    int idCapacites[MAX_CAPACITES];                       // voir la partie des capacités plus haut, 0 sinon existante
} FicheTypeUnite;

typedef struct
{
    int idUniteMagasin; // Ces id sont des numéros d'ordre 1,2... Mettre 0 dans la case où on n’a pas d'unité
    int idFicheTypeUnite;
    int idJoueurAutorise;
} UniteMagasin;
typedef struct
{
    int idVillage; // ordonnées dans l’ordre de parcours de la carte de haut en bas et de gauche à droite
    int ligne;
    int colonne;
    int idJoueurProprietaire; // idJoueur qui possède le village, 0 sinon
} Village;
typedef struct
{
    int idTypeTerrain;
    char symboleTerrain;
    int codeAffichageTerrain;
    char nomTerrain[20];
} TypeTerrain;

typedef struct
{
    int idJoueur;
    char symbole; // cas par défaut : J : joueur humain, E : joueur ennemi
    int or ;
    int type;
    int groupe_allies; // 1 : alliés joueur humain, 2 : alliés joueur pc ennemi
    int nombreVillages;
    int nombreUnites;
    int entretien;
    int revenu;
} Joueur;
typedef struct {
int idAttaqueSpeciale;
int toursRestants;
} EffetAttaqueSubite;
typedef struct
{
    int idUnite; // Ces id sont des numéros d'ordre 1,2... Mettre 0 dans la case où on n’a pas d'unité
    int idFicheTypeUnite;
    int idJoueur;
    char nomUnite[20];
    int active; // 1 si l’unité participe au jeu courant, 0 si l’unité ne participe pas on peut alors la rappeler
    int rang;   // 0 : unité normale, 1 : unité chef
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
    int alignement; // 0 : loyal (lawful), 1 : chaotique (chaotic), 2 : liminal (liminal) ou 3 : neutre (neutral)
    int finTour;
    EffetAttaqueSubite effetsAttaquesSubites[MAX_EFFETS_ATTAQUES_SUBITES];
} UniteJoueur;
typedef struct
{
    int idUnite;
    int idTypeTerrain;
    int codeAffichageTerrain; // code clavier du caractère à afficher dans la carte
    char symboleJoueur;
    int marque;
} CelluleCarte;
typedef struct
{
    int numOrdre;
    char moment[20];
    int bonus[3]; // cases : 0 -> bonus lawful, 1 -> bonus chaotic, 2 -> bonus liminal, 3 -> bonus neutral
} Periode;

// functions
// recherche functions
int chercherIndiceJoueur(int idJoueur, int nbLignesReel, Joueur joueurs[MAX_LIGNES_JOUEURS])
{
    int IndiceJoueur = -1;
    int VarParcour = 0;
    while (VarParcour < nbLignesReel && IndiceJoueur == -1)
    {
        if (joueurs[VarParcour].idJoueur == idJoueur)
        {
            IndiceJoueur = VarParcour;
        }
        else
        {
            VarParcour++;
        }
    }
    return IndiceJoueur;
}
int chercherIndiceRelationTerrain(int idTypeTerrain, RelationTerrain relationsTerrains[NB_TYPES_TERRAINS])
{
    int IndiceJoueur = -1;
    int VarParcour = 0;
    while (VarParcour < NB_TYPES_TERRAINS && IndiceJoueur == -1)
    {
        if (relationsTerrains[VarParcour].idTypeTerrain == idTypeTerrain)
        {
            IndiceJoueur = VarParcour;
        }
        else
        {
            VarParcour++;
        }
    }
    return IndiceJoueur;
}
int chercherIndiceTypeTerrain(char symboleTerrain, TypeTerrain typesTerrains[NB_TYPES_TERRAINS])
{
    int IndiceJoueur = -1;
    int VarParcour = 0;
    while (VarParcour < NB_TYPES_TERRAINS && IndiceJoueur == -1)
    {
        if (typesTerrains[VarParcour].symboleTerrain == symboleTerrain)
        {
            IndiceJoueur = VarParcour;
        }
        else
        {
            VarParcour++;
        }
    }
    return IndiceJoueur;
}
int chercherIndiceUniteJoueur(int idUnite, int idJoueur, int active, UniteJoueur Unitejoueurs[MAX_LIGNES_JOUEURS])
{
    int IndiceJoueur = -1;
    int VarParcour = 0;
    while (VarParcour < MAX_LIGNES_JOUEURS && IndiceJoueur == -1)
    {
        if (Unitejoueurs[VarParcour].idUnite == idUnite && Unitejoueurs[VarParcour].idJoueur == idJoueur && Unitejoueurs[VarParcour].active == active)
        {
            IndiceJoueur = VarParcour;
        }
        else
        {
            VarParcour++;
        }
    }
    return IndiceJoueur;
}
int chercherIndiceFicheTypeUnite(int idFicheTypeUnite, int nbFichesTypesUnites, FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES])
{
    int IndiceJoueur = -1;
    int VarParcour = 0;
    while (VarParcour < nbFichesTypesUnites && IndiceJoueur == -1)
    {
        if (fichesTypesUnites[VarParcour].idFicheTypeUnite == idFicheTypeUnite)
        {
            IndiceJoueur = VarParcour;
        }
        else
        {
            VarParcour++;
        }
    }
    return IndiceJoueur;
}
int chercherGroupeAllies(char symbolJoueur, int nbJoueurs, Joueur joueurs[MAX_LIGNES_JOUEURS]){
    int IndiceJoueur = -1;
    int VarParcour = 0;
    while (VarParcour < nbJoueurs && IndiceJoueur == -1)
    {
        if (joueurs[VarParcour].symbole == symbolJoueur)
        {
            IndiceJoueur = joueurs[VarParcour].groupe_allies;
        }
        else
        {
            VarParcour++;
        }
    }
    return IndiceJoueur;
}
int possedeTrait(int trait, int traits[MAX_TRAITS]){
    int possedeTrait=0;
    for(int i=0;i<MAX_TRAITS;i++){
        if(traits[i]==trait){
            possedeTrait=1;
        }
    }
    return possedeTrait;
}
int chercherIndiceJoueurHumain(int nbJoueurs, Joueur joueurs[MAX_LIGNES_JOUEURS]){
    int indice=-1;
    for (int i = 0; i < nbJoueurs; i++)
    {
        if(joueurs[i].idJoueur==1){
            indice=i;

        }
    }
    return indice;
    
}
int chercherIndiceVillage(int ligne, int colonne, int nbVillages, Village villages[MAX_LIGNES_VILLAGES]){
    int indice=-1;
    for (int i = 0; i < nbVillages; i++)
    {
        if(villages[i].colonne==colonne&&villages[i].ligne==ligne){
            indice=i;
        }
    }
    return indice;    
}
int max(int a,int b){
    if(a>b){
        return a;
    }
    else if(b>=a){
        return b;
    }
}
void miseAJourJoueurApresOccupationVillage(int indiceJoueur, Joueur joueurs[MAX_LIGNES_JOUEURS]){
    joueurs[indiceJoueur].revenu=2+joueurs[indiceJoueur].nombreVillages-(max(0,joueurs[indiceJoueur].entretien-joueurs[indiceJoueur].nombreVillages));
}
int chercherMaxIdUnitesJoueurs(UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS]){
    int max=0;
    for(int i=0;i<MAX_LIGNES_UNITESJOUEURS;i++){
        if(unitesJoueurs[i].idUnite>max){
            max=unitesJoueurs[i].idUnite;
        }
    }
    return max;
}
int chercherIndiceChefUnitesJoueurs(int idJoueur,UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS]){
    int pos=0;
    while(idJoueur!=unitesJoueurs[pos].idJoueur){
        pos++;
    }
    if(pos==MAX_LIGNES_UNITESJOUEURS){
        return -1;
    }
    else{
        return pos;
    }
}
int verifierFinTours(int idJoueur, UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS]){
    /*Si fintour = 1, alors toutes les unités ont fini leur tour*/    
    /*mchk m3aytlha fel main*/
    int fintour=1;
    for(int i=0;i<MAX_LIGNES_UNITESJOUEURS;i++){
        if(unitesJoueurs[i].idJoueur==idJoueur){
            if(unitesJoueurs[i].finTour==0){
                fintour=0;
            }
        }
    }
    return fintour;
}
void reinitialiserTours(int idJoueur, int nbJoueurs, Joueur joueurs[MAX_LIGNES_JOUEURS], UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS]){
    /*mchk m3aytlha fel main*/
    for(int i=0;i<MAX_LIGNES_UNITESJOUEURS;i++){
        unitesJoueurs[i].finTour=0;
        unitesJoueurs[i].mouvements=unitesJoueurs[i].mouvementsMax;
    }
    for(int j=0;j<nbJoueurs;i++){
        if(joueurs[j].idJoueur==idJoueur){
            joueurs[j].or=joueurs[j].or+8;
        }
    }
}
int chercherIndiceUniteMagasin(int idUnite, int nbUnitesMagasin, UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN]){

}
// charger functions
int chargerUnitesMagasinVersTableau(UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN], char *nomFichier)
{
    FILE *file;
    int Numbr_lignes = 0;
    file = fopen(nomFichier, "r");
    if (file == NULL)
    {
        printf("erreur loading file");
        return 0;
    }
    else
    {
        while (!feof(file))
        {
            fscanf(file, "%d %d %d", &unitesMagasin[Numbr_lignes].idUniteMagasin, &unitesMagasin[Numbr_lignes].idFicheTypeUnite, &unitesMagasin[Numbr_lignes].idJoueurAutorise);
            Numbr_lignes++;
        }
    }
    fclose(file);
    return Numbr_lignes;
}
int chargerVillagesVersTableau(Village villages[MAX_LIGNES_VILLAGES], char *nomFichier)
{
    FILE *file;
    file = fopen(nomFichier, "r");
    int Numbr_lignes = 0;
    if (file == NULL)
    {
        printf("erreur loading file");
        return 0;
    }
    else
    {
        while (!feof(file))
        {
            fscanf(file, "%d %d %d %d", &villages[Numbr_lignes].idVillage, &villages[Numbr_lignes].ligne, &villages[Numbr_lignes].colonne, &villages[Numbr_lignes].idJoueurProprietaire);
            Numbr_lignes++;
        }
    }
    fclose(file);

    return Numbr_lignes;
}
void chargerTypesTerrainsVersTableau(TypeTerrain typesTerrains[NB_TYPES_TERRAINS], char *nomFichier)
{
    FILE *file;
    file = fopen(nomFichier, "r");
    int Numbr_lignes = 0;
    if (file == NULL)
    {
        printf("erreur loading file");
    }
    else
    {
        while (!feof(file))
        {
            fscanf(file, "%d %c %d %s", &typesTerrains[Numbr_lignes].idTypeTerrain, &typesTerrains[Numbr_lignes].symboleTerrain, &typesTerrains[Numbr_lignes].codeAffichageTerrain, &typesTerrains[Numbr_lignes].nomTerrain);
            Numbr_lignes++;
        }
    }
    fclose(file);

}
void chargerPeriodesVersTableau(Periode periodes[NB_LIGNES_PERIODES], char *nomFichier)
{
    FILE *file;
    file = fopen(nomFichier, "r");
    int Numbr_lignes = 0;
    if (file == NULL)
    {
        printf("erreur loading file");
    }
    else
    {
        while (!feof(file))
        {
            fscanf(file, "%d %s %d %d %d", &periodes[Numbr_lignes].numOrdre, periodes[Numbr_lignes].moment, &periodes[Numbr_lignes].bonus[0], &periodes[Numbr_lignes].bonus[1], &periodes[Numbr_lignes].bonus[2]);
            Numbr_lignes++;
        }
    }
    fclose(file);
}
int chargerJoueursVersTableau(Joueur joueurs[MAX_LIGNES_JOUEURS], char *nomFichier)
{
    FILE *file;
    file = fopen(nomFichier, "r");
    int Numbr_lignes = 0;
    if (file == NULL)
    {
        printf("erreur loading file");
        return 0;
    }
    else
    {
        while (!feof(file))
        {
            fscanf(file, "%d %c %d %d %d", &joueurs[Numbr_lignes].idJoueur, &joueurs[Numbr_lignes].symbole, &joueurs[Numbr_lignes].or,&joueurs[Numbr_lignes].type, &joueurs[Numbr_lignes].groupe_allies);
            Numbr_lignes++;
        }
    }
    fclose(file); 
    return Numbr_lignes;
}
int chargerFichesTypesUnitesVersTableau(FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES], char *nomFichier)
{
    FILE *file;
    file = fopen(nomFichier, "r");
    int Numbr_lignes = 0;
    if (file == NULL)
    {
        printf("erreur loading file");
        return 0;
    }
    else
    {
        while (!feof(file))
        {
            fscanf(file, "%d %d %s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", &fichesTypesUnites[Numbr_lignes].idFicheTypeUnite, &fichesTypesUnites[Numbr_lignes].race, fichesTypesUnites[Numbr_lignes].nom, &fichesTypesUnites[Numbr_lignes].prix, &fichesTypesUnites[Numbr_lignes].pvMax, &fichesTypesUnites[Numbr_lignes].mouvementsMax, &fichesTypesUnites[Numbr_lignes].xpRequise, &fichesTypesUnites[Numbr_lignes].niveau, &fichesTypesUnites[Numbr_lignes].alignement, &fichesTypesUnites[Numbr_lignes].idFichesTypeUniteRangInf[0], &fichesTypesUnites[Numbr_lignes].idFichesTypeUniteRangInf[1], &fichesTypesUnites[Numbr_lignes].idFichesTypeUniteRangInf[2], &fichesTypesUnites[Numbr_lignes].idFichesTypeUniteRangSup[0], &fichesTypesUnites[Numbr_lignes].idFichesTypeUniteRangSup[1], &fichesTypesUnites[Numbr_lignes].idFichesTypeUniteRangSup[2], &fichesTypesUnites[Numbr_lignes].attaques[0].idAttaque, &fichesTypesUnites[Numbr_lignes].attaques[1].idAttaque, &fichesTypesUnites[Numbr_lignes].attaques[2].idAttaque, &fichesTypesUnites[Numbr_lignes].resistances[0], &fichesTypesUnites[Numbr_lignes].resistances[1], &fichesTypesUnites[Numbr_lignes].resistances[2], &fichesTypesUnites[Numbr_lignes].resistances[3], &fichesTypesUnites[Numbr_lignes].resistances[4], &fichesTypesUnites[Numbr_lignes].resistances[5], &fichesTypesUnites[Numbr_lignes].relationsTerrains[0].idTypeTerrain, &fichesTypesUnites[Numbr_lignes].idCapacites[0], &fichesTypesUnites[Numbr_lignes].idCapacites[1], &fichesTypesUnites[Numbr_lignes].idCapacites[2]);
            Numbr_lignes++;
        }
    }
    fclose(file);

    return Numbr_lignes;
}
int chargerUnitesJoueursVersTableau(UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS], char *nomFichier)
{
    FILE *file;
    file = fopen(nomFichier, "r");
    int Numbr_lignes = 0;
    if (file == NULL)
    {
        printf("erreur loading file");
    }
    else
    {
        while (!feof(file))
        {
            fscanf(file,"%d %d %d %s %d %d %d %d %d %d %d",&unitesJoueurs[Numbr_lignes].idUnite,&unitesJoueurs[Numbr_lignes].idFicheTypeUnite,&unitesJoueurs[Numbr_lignes].idJoueur,unitesJoueurs[Numbr_lignes].nomUnite,&unitesJoueurs[Numbr_lignes].active,&unitesJoueurs[Numbr_lignes].rang,&unitesJoueurs[Numbr_lignes].ligne,&unitesJoueurs[Numbr_lignes].colonne,&unitesJoueurs[Numbr_lignes].traits[0],&unitesJoueurs[Numbr_lignes].traits[1],&unitesJoueurs[Numbr_lignes].traits[2]);
            Numbr_lignes++;
        }
    }
    fclose(file);
    return Numbr_lignes;
}
void chargerCarteVersTableau(CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE], TypeTerrain typesTerrains[NB_TYPES_TERRAINS], char *nomFichier)
{
    FILE *file;
    file = fopen(nomFichier, "r");
    for (int i = 0; i < NB_LIGNES_CARTE; i++)
    {
        char ligne[100];
        fgets(ligne, 100, file);
        for (int j = 0; j < (NB_COLONNES_CARTE * 2) - 1; j++)
        {
            if (strcmp(&ligne[j], " ") != 0)
            {
                carte[i][j].idTypeTerrain = typesTerrains[chercherIndiceTypeTerrain(ligne[j], typesTerrains)].idTypeTerrain;
                carte[i][j].codeAffichageTerrain = typesTerrains[chercherIndiceTypeTerrain(ligne[j], typesTerrains)].codeAffichageTerrain;
            }
        }
    }
    fclose(file);
}
void chargerUnitesJoueursSauvegardeesVersTableau(UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS], char* nomFichier){
    FILE *file;
    file = fopen(nomFichier,"r");
    int Numbr_lignes = 0;
    if (file == NULL)
    {
        printf("erreur loading file");
    }
    else
    {
        while (!feof(file))
        {
            fscanf(file,"%d %d %d %s %d %d %d %d %d %d %d", &unitesJoueurs[Numbr_lignes].idUnite, &unitesJoueurs[Numbr_lignes].idFicheTypeUnite, &unitesJoueurs[Numbr_lignes].idJoueur, unitesJoueurs[Numbr_lignes].nomUnite, &unitesJoueurs[Numbr_lignes].active, &unitesJoueurs[Numbr_lignes].rang, &unitesJoueurs[Numbr_lignes].ligne, &unitesJoueurs[Numbr_lignes].colonne, &unitesJoueurs[Numbr_lignes].traits[0], &unitesJoueurs[Numbr_lignes].traits[1], &unitesJoueurs[Numbr_lignes].traits[2]);
            Numbr_lignes++;

        }
    }
    fclose(file);
}
// affichage functions
void afficherUnitesMagasin(int nbUnitesMagasin, UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN])
{
    for (int i = 0; i < nbUnitesMagasin; i++)
    {
        printf("\n%d %d %d \n", unitesMagasin[i].idUniteMagasin, unitesMagasin[0].idFicheTypeUnite, unitesMagasin[i].idJoueurAutorise);
    }
    printf("\n");
}
void afficherVillages(int nbVillages, Village villages[MAX_LIGNES_VILLAGES])
{
    for (int i = 0; i < nbVillages; i++)
    {
        printf("%d %d %d \n", villages[i].idVillage, villages[0].ligne, villages[i].colonne, villages[i].idJoueurProprietaire);
    }
    printf("\n");
}
void afficherTypesTerrains(TypeTerrain typesTerrains[NB_TYPES_TERRAINS])
{
    for (int i = 0; i < NB_TYPES_TERRAINS; i++)
    {
        printf("%d %c %d %s \n", typesTerrains[i].idTypeTerrain, typesTerrains[0].symboleTerrain, typesTerrains[i].codeAffichageTerrain, typesTerrains[i].nomTerrain);
    }
    printf("\n");
}
void afficherPeriodes(Periode periodes[NB_LIGNES_PERIODES])
{
    for (int i = 0; i < NB_LIGNES_PERIODES; i++)
    {
        printf("%d %s %d %d %d\n", periodes[i].numOrdre, periodes[i].moment, periodes[i].bonus[0], periodes[i].bonus[1], periodes[i].bonus[2]);
    }
    printf("\n");
}
void afficherJoueurs(int nbJoueurs, Joueur joueurs[MAX_LIGNES_JOUEURS])
{
    for (int i = 0; i < nbJoueurs; i++)
    {
        printf("%d %c %d %d\n", joueurs[i].idJoueur, joueurs[i].symbole, joueurs[i].or, joueurs[i].groupe_allies);
    }
    printf("\n");
}
void afficherFichesTypesUnites(int nbFichesTypesUnites, FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES])
{
    for (int i = 0; i < nbFichesTypesUnites; i++)
    {
        printf("%d %d %s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", fichesTypesUnites[i].idFicheTypeUnite, fichesTypesUnites[i].race, fichesTypesUnites[i].nom, fichesTypesUnites[i].prix, fichesTypesUnites[i].pvMax, fichesTypesUnites[i].mouvementsMax, fichesTypesUnites[i].xpRequise, fichesTypesUnites[i].niveau, fichesTypesUnites[i].alignement, fichesTypesUnites[i].idFichesTypeUniteRangInf[0], fichesTypesUnites[i].idFichesTypeUniteRangInf[1], fichesTypesUnites[i].idFichesTypeUniteRangInf[2], fichesTypesUnites[i].idFichesTypeUniteRangSup[0], fichesTypesUnites[i].idFichesTypeUniteRangSup[1], fichesTypesUnites[i].idFichesTypeUniteRangSup[2], fichesTypesUnites[i].attaques[0].idAttaque, fichesTypesUnites[i].attaques[1].idAttaque, fichesTypesUnites[i].attaques[2].idAttaque, fichesTypesUnites[i].resistances[0], fichesTypesUnites[i].resistances[1], fichesTypesUnites[i].resistances[2], fichesTypesUnites[i].resistances[3], fichesTypesUnites[i].resistances[4], fichesTypesUnites[i].resistances[5], fichesTypesUnites[i].relationsTerrains[0].idTypeTerrain, fichesTypesUnites[i].idCapacites[0], fichesTypesUnites[i].idCapacites[1], fichesTypesUnites[i].idCapacites[2]);
    }
    printf("\n");
}
void afficherUniteJoueur(UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS])
{
    for (int i = 0; i < MAX_LIGNES_UNITESJOUEURS; i++)
    {
        printf("%d %d %d %s %d %d %d %d %d %d %d\n",unitesJoueurs[i].idUnite,unitesJoueurs[i].idFicheTypeUnite,unitesJoueurs[i].idJoueur,unitesJoueurs[i].nomUnite,unitesJoueurs[i].active,unitesJoueurs[i].rang,unitesJoueurs[i].ligne,unitesJoueurs[i].colonne,unitesJoueurs[i].traits[0],unitesJoueurs[i].traits[1],unitesJoueurs[i].traits[2]);
    }
    printf("\n");
}
void afficherCarte(CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE])
{
    for (int j = 0; j < NB_LIGNES_CARTE; j++)
    {
        for (int i = 0; i < NB_COLONNES_CARTE; i++)
        {
            printf("%d %c ||",carte[i][j].idTypeTerrain, carte[i][j].codeAffichageTerrain);
        }
        printf("\n");
    }
}
void afficherJoueurJeu(int idJoueur, int nbJoueurs, Joueur joueurs[MAX_LIGNES_JOUEURS]){
    printf("*** JOUEUR ***\n-----------------------------------------------------------------------------------------\n| idJoueur | symbole|   or   |  type |grallies|nbVillages|nbUnites|entretien| revenu |\n-----------------------------------------------------------------------------------------\n|    %d    |   %c    |   %d  |   %d   |   %d    |    %d    |   %d   |   %d   |   %d   |\n-----------------------------------------------------------------------------------------\n",joueurs[chercherIndiceJoueur(idJoueur,nbJoueurs,joueurs)].idJoueur,joueurs[chercherIndiceJoueur(idJoueur,nbJoueurs,joueurs)].symbole,joueurs[chercherIndiceJoueur(idJoueur,nbJoueurs,joueurs)].or,joueurs[chercherIndiceJoueur(idJoueur,nbJoueurs,joueurs)].type,joueurs[chercherIndiceJoueur(idJoueur,nbJoueurs,joueurs)].groupe_allies,joueurs[chercherIndiceJoueur(idJoueur,nbJoueurs,joueurs)].nombreVillages,joueurs[chercherIndiceJoueur(idJoueur,nbJoueurs,joueurs)].nombreUnites,joueurs[chercherIndiceJoueur(idJoueur,nbJoueurs,joueurs)].entretien,joueurs[chercherIndiceJoueur(idJoueur,nbJoueurs,joueurs)].revenu);
}
void afficherUnitesJoueursJeu(int idJoueur, int active, UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS]){
        printf("*** UNITES ***\n--------------------------------------------------------------------------------------------------------------------------------------------------------------\n|idUnite|idFicheUni|idJoueur|nomUnite|active|rang|ligne|colonne|trait1|trait2|trait3|pv|pvMax|mouvements|mouvementsMax|xp|xpRequise|niveau|alignement|finTour|\n");
        for(int i=0;i<MAX_LIGNES_UNITESJOUEURS;i++){
            if(idJoueur==unitesJoueurs[i].idJoueur&&active==unitesJoueurs[i].active){
        printf("--------------------------------------------------------------------------------------------------------------------------------------------------------------\n|  %d   |   %d    |   %d   |   %s   |  %d   | %d |  %d |  %d   |  %d  |  %d  |  %d  | %d | %d |    %d    |    %d    |   %d   |   %d   |   %d   |   %d   |   %d   |\n",unitesJoueurs[i].idUnite,unitesJoueurs[i].idFicheTypeUnite,unitesJoueurs[i].idJoueur,unitesJoueurs[i].nomUnite,unitesJoueurs[i].active,unitesJoueurs[i].rang,unitesJoueurs[i].ligne,unitesJoueurs[i].colonne,unitesJoueurs[i].traits[0],unitesJoueurs[i].traits[1],unitesJoueurs[i].traits[2],unitesJoueurs[i].pv,unitesJoueurs[i].pvMax,unitesJoueurs[i].mouvements,unitesJoueurs[i].mouvementsMax,unitesJoueurs[i].xp,unitesJoueurs[i].xpRequise,unitesJoueurs[i].niveau,unitesJoueurs[i].alignement,unitesJoueurs[i].finTour);
            }
        }
}
void afficherCarteJeu(CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE], int nbJoueurs, Joueur joueurs[MAX_LIGNES_JOUEURS], int nbVillages, Village villages[MAX_LIGNES_VILLAGES], UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS]){
    for(int f=0;f<NB_COLONNES_CARTE;f++){
        printf("%d       ",f+1);
    }
    for(int i=0;i<NB_LIGNES_CARTE;i++){
            printf("----------------------------------------------------------------------------------------------------------------------------------------------\n");
            printf("%d",i+1);
        for(int j=0;j<NB_COLONNES_CARTE;j++){
            if(carte[i][j].idUnite==0&&carte[i][j].marque==0){
            printf("|%c    ",carte[i][j].codeAffichageTerrain);
            }
            else if(carte[i][j].marque==1&&carte[i][j].idUnite==0){
            printf("|%c  ++",carte[i][j].codeAffichageTerrain);
            }
            else{
                printf("|%c %c%d",carte[i][j].codeAffichageTerrain,carte[i][j].symboleJoueur,carte[i][j].idUnite);
            }
        }
    }
}
//appliquer et placer
void appliquerTrait(int indiceUniteJoueur, int trait, UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS]){

}
void marquerDeplacementsPossibles(CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],int ligneCourante, int colonneCourante, int mouvements){
    int pos=0;
    int flip=0;
        for(int i=ligneCourante-4;i<ligneCourante+5;i++){
            for(int j=colonneCourante-pos;j<colonneCourante+pos+1;j++){
                carte[i][j].marque=1;
            }
            if(pos<5&&flip==0){
            pos++;
            }
            else{
                flip=1;
                pos--;
            }
        }
    }
void effacerDeplacementsPossibles(CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE]){
    for(int i=0;i<NB_LIGNES_CARTE;i++){
        for(int j=0;j<NB_COLONNES_CARTE;j++){
            if(carte[i][j].marque==1){
                carte[i][j].marque=0;
            }
        }
    }
}
int rechercheIdJoueur(int idJoueur,int size,Joueur joueurs[MAX_LIGNES_JOUEURS]){
    for(int i=0;i<size;i++){
        if(joueurs[i].idJoueur==idJoueur){
            return joueurs[i].symbole;
        }
    }
    
}
void placerUnitesDansCarte(UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS], int nbJoueurs, Joueur joueurs[MAX_LIGNES_JOUEURS],CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE]){
    for(int i=0;i<nbJoueurs;i++){
        carte[unitesJoueurs[i].ligne][unitesJoueurs[i].colonne].idUnite=unitesJoueurs[i].idUnite;
        carte[unitesJoueurs[i].ligne][unitesJoueurs[i].colonne].symboleJoueur=rechercheIdJoueur(unitesJoueurs[i].idJoueur,MAX_LIGNES_JOUEURS,joueurs);
    }
}
void deplacerUnite(int idJoueur, int indiceDansUnitesJoueurs, int numUnite, int ligne_courante, int colonne_courante, int nouvelle_ligne, int nouvelle_colonne, int nbJoueurs, int nbVillages, CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE], Joueur joueurs[MAX_LIGNES_JOUEURS], UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS], Village villages[MAX_LIGNES_VILLAGES]){
    /*a3mllelha appel fy main o zyd  deposeha*/
    if(carte[nouvelle_ligne][nouvelle_colonne].marque==1&&carte[ligne_courante][colonne_courante].idUnite!=0&&carte[nouvelle_ligne][nouvelle_colonne].idUnite==0){
        for(int i=0;i<nbVillages;i++){
            if(villages[i].ligne==nouvelle_ligne&&villages[i].colonne==nouvelle_colonne){
                if(villages[i].idJoueurProprietaire==0){
                    miseAJourJoueurApresOccupationVillage(idJoueur,joueurs);
                }
            }
        }
        unitesJoueurs[indiceDansUnitesJoueurs].ligne=nouvelle_ligne;
        unitesJoueurs[indiceDansUnitesJoueurs].colonne=nouvelle_colonne;
    }
}
void completerInitialisationUnitesJoueurs(UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS],int nbFichesTypesUnites, FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES]){
    for (int i=0;i<nbFichesTypesUnites;i++){
        if(unitesJoueurs[i].idUnite!=0){
            unitesJoueurs[i].pv=fichesTypesUnites[chercherIndiceFicheTypeUnite(unitesJoueurs[i].idFicheTypeUnite,nbFichesTypesUnites,fichesTypesUnites)].pvMax;
            unitesJoueurs[i].pvMax=fichesTypesUnites[chercherIndiceFicheTypeUnite(unitesJoueurs[i].idFicheTypeUnite,nbFichesTypesUnites,fichesTypesUnites)].pvMax;
            unitesJoueurs[i].mouvements=fichesTypesUnites[chercherIndiceFicheTypeUnite(unitesJoueurs[i].idFicheTypeUnite,nbFichesTypesUnites,fichesTypesUnites)].mouvementsMax;
            unitesJoueurs[i].mouvementsMax=fichesTypesUnites[chercherIndiceFicheTypeUnite(unitesJoueurs[i].idFicheTypeUnite,nbFichesTypesUnites,fichesTypesUnites)].mouvementsMax;
            unitesJoueurs[i].xpRequise=fichesTypesUnites[chercherIndiceFicheTypeUnite(unitesJoueurs[i].idFicheTypeUnite,nbFichesTypesUnites,fichesTypesUnites)].xpRequise;
            unitesJoueurs[i].niveau=fichesTypesUnites[chercherIndiceFicheTypeUnite(unitesJoueurs[i].idFicheTypeUnite,nbFichesTypesUnites,fichesTypesUnites)].niveau;
            unitesJoueurs[i].alignement=fichesTypesUnites[chercherIndiceFicheTypeUnite(unitesJoueurs[i].idFicheTypeUnite,nbFichesTypesUnites,fichesTypesUnites)].alignement;        
        }
    }
}
void completerInitialisationJoueurs(int nbJoueurs, int nbVillages,Joueur joueurs[MAX_LIGNES_JOUEURS],Village villages[MAX_LIGNES_VILLAGES],UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS]){
    for (int i = 0; i < nbJoueurs; i++)
    {
        int nbvillage=0;
        for (int j = 0; j < nbVillages; j++)
        {
            if(villages[j].idJoueurProprietaire==joueurs[i].idJoueur){
                nbvillage++;
            }
        }
        joueurs[i].nombreVillages=nbvillage;  
        int nbunites=0;
        for (int j = 0; j < MAX_LIGNES_JOUEURS; j++)
        {
            if((unitesJoueurs[j].idJoueur==joueurs[i].idJoueur)&&unitesJoueurs[j].idUnite!=0){
                nbunites++;
            }
        }
        joueurs[i].nombreUnites=nbunites-1;

    }
    
}
// sauvegrader functions
void sauvegarderVillages(int nbVillages, Village villages[MAX_LIGNES_VILLAGES], char *nomFichier)
{
    FILE *file = fopen(nomFichier, "w");
    for (int i = 0; i < nbVillages; i++)
    {
        fprintf(file,"%d %d %d", villages[i].idVillage, villages[i].ligne, villages[i].colonne, villages[i].idJoueurProprietaire);
        if(i!=nbVillages-1){
        fprintf(file,"\n");
        }
    }
    fclose(file);
}
void sauvegarderJoueurs(int nbJoueurs, Joueur joueurs[MAX_LIGNES_JOUEURS], char *nomFichier)
{
    FILE *file = fopen(nomFichier, "w");
    for (int i = 0; i < nbJoueurs; i++)
    {
        fprintf(file,"%d %c %d %d", joueurs[i].idJoueur, joueurs[i].symbole, joueurs[i].or, joueurs[i].groupe_allies);
        if(i!=nbJoueurs-1){
        fprintf(file,"\n");
        }
    }
    fclose(file);
}
void sauvegarderUniteJoueur(UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS], char *nomFichier)
{
    FILE *file = fopen(nomFichier, "w");
    for (int i = 0; i < MAX_LIGNES_UNITESJOUEURS; i++)
    {
        fprintf(file,"%d %d %d %s %d %d %d %d %d %d %d", unitesJoueurs[i].idUnite, unitesJoueurs[i].idFicheTypeUnite, unitesJoueurs[i].idJoueur, unitesJoueurs[i].nomUnite, unitesJoueurs[i].active, unitesJoueurs[i].rang, unitesJoueurs[i].ligne, unitesJoueurs[i].colonne, unitesJoueurs[i].traits[0], unitesJoueurs[i].traits[1], unitesJoueurs[i].traits[2]);
        if(i!=MAX_LIGNES_UNITESJOUEURS-1){
        fprintf(file,"\n");
        }
    }
    fclose(file);
}
//initiatlisation
void initialiserNouveauJeu(int *nbfichesTypesUnites, int *nbJoueurs, int *nbUnitesMagasin,int *nbVillages,FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES],Joueur joueurs[MAX_LIGNES_JOUEURS],CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN],Village villages[MAX_LIGNES_VILLAGES],TypeTerrain typesTerrains[NB_TYPES_TERRAINS],Periode periodes[NB_LIGNES_PERIODES],UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS]){
    /*mch depose*/
    int numbr_unites;
    *nbUnitesMagasin = chargerUnitesMagasinVersTableau(unitesMagasin, "unitesMagasin_original.txt");
    *nbVillages = chargerVillagesVersTableau(villages, "villages_original.txt");
    numbr_unites=chargerUnitesJoueursVersTableau( unitesJoueurs,"unitesJoueurs_original.txt");
    chargerTypesTerrainsVersTableau(typesTerrains, "typesTerrains_original.txt");
    chargerPeriodesVersTableau(periodes, "periodes_original.txt");
    *nbJoueurs = chargerJoueursVersTableau(joueurs, "joueurs_originals.txt");
    *nbfichesTypesUnites = chargerFichesTypesUnitesVersTableau(fichesTypesUnites, "fichesTypesUnites_original.txt");
    chargerCarteVersTableau(carte, typesTerrains, "carte_original.txt");
    placerUnitesDansCarte( unitesJoueurs,  numbr_unites,  joueurs, carte);
    completerInitialisationUnitesJoueurs(unitesJoueurs,*nbfichesTypesUnites, fichesTypesUnites);
    completerInitialisationJoueurs(*nbJoueurs,*nbVillages, joueurs,villages,unitesJoueurs);
}
void chargerJeuComplet(int *nbFichesTypesUnites, int *nbJoueurs, int *nbUnitesMagasin,int *nbVillages,FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES],Joueur joueurs[MAX_LIGNES_JOUEURS],CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN],Village villages[MAX_LIGNES_VILLAGES],TypeTerrain typesTerrains[NB_TYPES_TERRAINS],Periode periodes[NB_LIGNES_PERIODES],UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS]){
     *nbUnitesMagasin = chargerUnitesMagasinVersTableau(unitesMagasin, "unitesMagasin_original.txt");
    *nbVillages = chargerVillagesVersTableau(villages, "villages_sauvegarde.txt");
    chargerTypesTerrainsVersTableau(typesTerrains, "typesTerrains_original.txt");
    chargerPeriodesVersTableau(periodes, "periodes_original.txt");
    chargerUnitesJoueursSauvegardeesVersTableau( unitesJoueurs,"unitesJoueurs_sauvegarde.txt");
    *nbJoueurs = chargerJoueursVersTableau(joueurs, "joueurs_sauvegarde.txt");
    *nbFichesTypesUnites = chargerFichesTypesUnitesVersTableau(fichesTypesUnites, "fichesTypesUnites_original.txt");
    chargerCarteVersTableau(carte, typesTerrains, "carte_original.txt");
    placerUnitesDansCarte( unitesJoueurs,  *nbJoueurs,  joueurs, carte);
    completerInitialisationJoueurs(*nbJoueurs,*nbVillages, joueurs,villages,unitesJoueurs);
}
void afficherTableauxJeu(int nbLignesFichesTypesUnites, int nbLignesJoueurs,int nbLignesUnitesMagasin, int nbLignesVillages,FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES],Joueur joueurs[MAX_LIGNES_JOUEURS],CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE],UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN],Village villages[MAX_LIGNES_VILLAGES],TypeTerrain typesTerrains[NB_TYPES_TERRAINS],Periode periodes[NB_LIGNES_PERIODES],UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS]){
    afficherUnitesMagasin(nbLignesUnitesMagasin, unitesMagasin);
    afficherVillages(nbLignesVillages, villages);
    afficherTypesTerrains(typesTerrains);
    afficherPeriodes(periodes);
    afficherJoueurs(nbLignesJoueurs, joueurs);
    afficherFichesTypesUnites(nbLignesFichesTypesUnites, fichesTypesUnites);
    afficherUniteJoueur(unitesJoueurs);
    afficherCarte(carte);
}
void sauvegarderJeuComplet(int nbLignesVillages, int nbLignesJoueurs,Village villages[MAX_LIGNES_VILLAGES],Joueur joueurs[MAX_LIGNES_JOUEURS],UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS]){
    sauvegarderVillages(nbLignesVillages, villages, "villages_sauvegarde.txt");
    sauvegarderJoueurs(nbLignesJoueurs, joueurs, "joueurs_sauvegarde.txt");
    sauvegarderUniteJoueur(unitesJoueurs, "unitesJoueurs_sauvegarde.txt");
}
void main()
{
    // tables declaration
    UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN];
    TypeTerrain typesTerrains[NB_TYPES_TERRAINS];
    Village villages[MAX_LIGNES_VILLAGES];
    Periode periodes[NB_LIGNES_PERIODES];
    Joueur joueurs[MAX_LIGNES_JOUEURS];
    RelationTerrain relationsTerrains[NB_TYPES_TERRAINS];
    FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES];
    UniteJoueur Unitejoueurs[MAX_LIGNES_JOUEURS];
    UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS];
    CelluleCarte carte[NB_LIGNES_CARTE][NB_COLONNES_CARTE];
    // varriables declaration
    int nbUnitesMagasin, IndiceJoueur1, nbVillages, IndiceTypeTerrain, IndiceRelationTerrain, nbJoueurs, nbfichesTypesUnites, IndiceJoueur;
    // function calls
    int menu=0;
    printf("entre un nombre:\n 1-initialiser Nouveau Jeu\n 2-charger Jeu Complet\n");
    scanf("%d",&menu);
    if(menu==1){
    initialiserNouveauJeu(&nbfichesTypesUnites,&nbJoueurs,&nbUnitesMagasin,&nbVillages,fichesTypesUnites, joueurs, carte, unitesMagasin, villages, typesTerrains, periodes, unitesJoueurs);
    afficherJoueurJeu(48,2,joueurs);
    afficherUnitesJoueursJeu(1, 1,unitesJoueurs);
    int ligne,colonne;
    printf("\nentrer la ligne et la colone du joueur pour marker les deplacements possible\n");
    scanf("%d %d",&ligne,&colonne);
    marquerDeplacementsPossibles( carte,ligne,colonne,4);
    afficherCarteJeu( carte,  nbJoueurs, joueurs,nbVillages,villages,unitesJoueurs);
    printf("\nclickez 1 si vous voulez effacer les deplacement possibles 0 si vous voulez continuer\n");
    int effaceDeplacement;
    scanf("%d",&effaceDeplacement);
    if(effaceDeplacement==1){
        effacerDeplacementsPossibles(carte);
    }
    else{
    afficherCarteJeu( carte,  nbJoueurs, joueurs,nbVillages,villages,unitesJoueurs);
    sauvegarderJeuComplet( nbVillages, nbJoueurs, villages, joueurs, unitesJoueurs);
    }
    }
    else if(menu==2){
    chargerJeuComplet(&nbfichesTypesUnites, &nbJoueurs, &nbUnitesMagasin,&nbVillages, fichesTypesUnites, joueurs, carte, unitesMagasin, villages, typesTerrains, periodes, unitesJoueurs);
    }
    afficherTableauxJeu( nbfichesTypesUnites,  nbJoueurs, nbUnitesMagasin, nbVillages, fichesTypesUnites, joueurs, carte, unitesMagasin, villages, typesTerrains, periodes, unitesJoueurs);
}