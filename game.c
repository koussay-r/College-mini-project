#include <stdio.h>
//constants
#define NB_LIGNES_CARTE 30
#define NB_COLONNES_CARTE 38
#define MAX_LIGNES_JOUEURS 5
#define MAX_LIGNES_UNITESJOUEURS 50
#define MAX_LIGNES_FICHES_TYPES_UNITES 10
#define MAX_LIGNES_UNITESMAGASIN 8
#define MAX_LIGNES_VILLAGES 14
#define NB_LIGNES_PERIODES 10
#define NB_UNITES_RANG_INF_SUP 3
#define MAX_ATTAQUES 3
#define NB_RESISTANCES 6
#define NB_TYPES_TERRAINS 17
#define MAX_CAPACITES 3
#define MAX_TRAITS 3
//objects
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
typedef struct {
int idTypeTerrain;
int coutMouvement;
int defense;
} RelationTerrain;
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
int idFichesTypeUniteRangInf[NB_UNITES_RANG_INF_SUP]; // 0 si non existant
int idFichesTypeUniteRangSup[NB_UNITES_RANG_INF_SUP]; // 0 si non existant
Attaque attaques [MAX_ATTAQUES];
int resistances[NB_RESISTANCES] ; // ordres des cases // blade, pierce, impact, fire, cold, arcane
RelationTerrain relationsTerrains[NB_TYPES_TERRAINS]; // suivant les id : voir la partie terrains
int idCapacites[MAX_CAPACITES]; // voir la partie des capacités plus haut, 0 sinon existante
} FicheTypeUnite;

typedef struct {
int idUniteMagasin;// Ces id sont des numéros d'ordre 1,2... Mettre 0 dans la case où on n’a pas d'unité
int idFicheTypeUnite;
int idJoueurAutorise;
} UniteMagasin;
typedef struct {
int idVillage; // ordonnées dans l’ordre de parcours de la carte de haut en bas et de gauche à droite
int ligne;
int colonne;
int idJoueurProprietaire; // idJoueur qui possède le village, 0 sinon
} Village;
typedef struct{
int idTypeTerrain;
char symboleTerrain;
int codeAffichageTerrain;
char nomTerrain[20];
} TypeTerrain;

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
//functions
//charger functions
int chargerUnitesMagasinVersTableau(UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN], char* nomFichier){
    FILE *file;
    int Numbr_lignes=0;
    file=fopen(nomFichier,"r");
    if(file==NULL){
        printf("erreur loading file");
        return 0;
    }
    else{
        while(!feof(file)){
            fscanf(file,"%d %d %d",&unitesMagasin[Numbr_lignes].idUniteMagasin,&unitesMagasin[Numbr_lignes].idFicheTypeUnite,&unitesMagasin[Numbr_lignes].idJoueurAutorise);
            Numbr_lignes++;
        }
        
    }
    fclose(file);
    return Numbr_lignes;
}
int chargerVillagesVersTableau(Village villages[MAX_LIGNES_VILLAGES], char* nomFichier){
    FILE *file;
    file=fopen(nomFichier,"r");
    int Numbr_lignes=0;
    if(file==NULL){
        printf("erreur loading file");
        return 0;
    }
    else{
        while(!feof(file)){
            fscanf(file,"%d %d %d %d",&villages[Numbr_lignes].idVillage,&villages[Numbr_lignes].ligne,&villages[Numbr_lignes].colonne,&villages[Numbr_lignes].idJoueurProprietaire);
            Numbr_lignes++;
        }
    }
    fclose(file);

    return Numbr_lignes;
}
int chargerTypesTerrainsVersTableau(TypeTerrain typesTerrains[NB_TYPES_TERRAINS],char* nomFichier){
    FILE *file;
    file=fopen(nomFichier,"r");
    int Numbr_lignes=0;
    if(file==NULL){
        printf("erreur loading file");
        return 0;
    }
    else{
        while(!feof(file)){
            fscanf(file,"%d %c %d %s",&typesTerrains[Numbr_lignes].idTypeTerrain,&typesTerrains[Numbr_lignes].symboleTerrain,&typesTerrains[Numbr_lignes].codeAffichageTerrain,&typesTerrains[Numbr_lignes].nomTerrain);
            Numbr_lignes++;
        }
    }
    fclose(file);

    return Numbr_lignes;
}
int chargerPeriodesVersTableau(Periode periodes[NB_LIGNES_PERIODES], char* nomFichier){
    FILE *file;
    file=fopen(nomFichier,"r");
    int Numbr_lignes=0;
    if(file==NULL){
        printf("erreur loading file");
        return 0;
    }
    else{
        while(!feof(file)){     
            fscanf(file,"%d %s %d %d %d",&periodes[Numbr_lignes].numOrdre,periodes[Numbr_lignes].moment,&periodes[Numbr_lignes].bonus[0],&periodes[Numbr_lignes].bonus[1],&periodes[Numbr_lignes].bonus[2]);
            Numbr_lignes++;
        }
    }
    fclose(file);

    return Numbr_lignes;
}
// affichage functions
void afficherUnitesMagasin(int nb_lignes,UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN]){
    for(int i=0;i<nb_lignes;i++){
        printf("%d %d %d \n",unitesMagasin[i].idUniteMagasin,unitesMagasin[0].idFicheTypeUnite,unitesMagasin[i].idJoueurAutorise);
    }
    printf("\n");

}
void afficherVillages(int nb_lignes, Village villages[MAX_LIGNES_VILLAGES]){
    for(int i=0;i<nb_lignes;i++){
        printf("%d %d %d \n",villages[i].idVillage,villages[0].ligne,villages[i].colonne,villages[i].idJoueurProprietaire);
    }
    printf("\n");

}
void afficherTypesTerrains(TypeTerrain typesTerrains[NB_TYPES_TERRAINS]){
    for(int i=0;i<NB_TYPES_TERRAINS;i++){
        printf("%d %c %d %s \n",typesTerrains[i].idTypeTerrain,typesTerrains[0].symboleTerrain,typesTerrains[i].codeAffichageTerrain,typesTerrains[i].nomTerrain);
    }
    printf("\n");

}
void afficherPeriodes( int number_ligne_periode,Periode periodes[NB_LIGNES_PERIODES]){
    for(int i=0;i<number_ligne_periode;i++){
        printf("%d %s %d %d %d\n",periodes[i].numOrdre,periodes[i].moment,periodes[i].bonus[0],periodes[i].bonus[1],periodes[i].bonus[2]);
    }
    printf("\n");

}
//sauvegrader functions
void sauvegarderUnitesMagasin(int nb_lignes,UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN], char* nomFichier){
    FILE *file=fopen(nomFichier,"w");
    for(int i=0;i<nb_lignes;i++){
        fprintf(file,"%d %d %d\n",unitesMagasin[i].idUniteMagasin,unitesMagasin[i].idFicheTypeUnite,unitesMagasin[i].idJoueurAutorise);
    }
    fclose(file);

}
void sauvegarderVillages(int nbVillages, Village villages[MAX_LIGNES_VILLAGES], char* nomFichier){
    FILE *file=fopen(nomFichier,"w");
    for(int i=0;i<nbVillages;i++){
        fprintf(file,"%d %d %d\n",villages[i].idVillage,villages[i].ligne,villages[i].colonne,villages[i].idJoueurProprietaire);
    }
    fclose(file);

}
void sauvegarderTypesTerrains(int nb_lignes, TypeTerrain typesTerrains[NB_TYPES_TERRAINS],char* nomFichier){
    FILE *file=fopen(nomFichier,"w");
    for(int i=0;i<nb_lignes;i++){
        fprintf(file,"%d %d %d %s\n",typesTerrains[i].idTypeTerrain,typesTerrains[i].symboleTerrain,typesTerrains[i].codeAffichageTerrain,typesTerrains[i].nomTerrain);
    }
    fclose(file);

}
void sauvegarderPeriodes(int nbLignes, Periode periodes[NB_LIGNES_PERIODES], char*nomFichier){
    FILE *file=fopen(nomFichier,"w");
    for(int i=0;i<nbLignes;i++){
        fprintf(file,"%d %s %d %d %d \n",periodes[i].numOrdre,periodes[i].moment,periodes[i].bonus[0],periodes[i].bonus[1],periodes[i].bonus[2],periodes[i].bonus[3]);
    }
    fclose(file);

}
//recherche functions
int chercherIndiceJoueur(int idJoueur, int nbLignesReel, Joueur joueurs[MAX_LIGNES_JOUEURS]){
    int IndiceJoueur=-1;
    int VarParcour=0;
    while (VarParcour<nbLignesReel&&IndiceJoueur==-1)
    {
        if(joueurs[VarParcour].idJoueur==idJoueur){
            IndiceJoueur=VarParcour;
        }
        else{
            VarParcour++;
        }
    }
    return IndiceJoueur;
    
}
int chercherIndiceRelationTerrain(int idTypeTerrain, RelationTerrain relationsTerrains[NB_TYPES_TERRAINS]){
    int IndiceJoueur=-1;
    int VarParcour=0;
    while (VarParcour<NB_TYPES_TERRAINS&&IndiceJoueur==-1)
    {
        if(relationsTerrains[VarParcour].idTypeTerrain==idTypeTerrain){
            IndiceJoueur=VarParcour;
        }
        else{
            VarParcour++;
        }
    }
    return IndiceJoueur;
}
int chargerJoueursVersTableau(Joueur joueurs[MAX_LIGNES_JOUEURS], char* nomFichier){
    FILE *file;
    file=fopen(nomFichier,"r");
    int Numbr_lignes=0;
    if(file==NULL){
        printf("erreur loading file");
        return 0;
    }
    else{
        while(!feof(file)){     
            fscanf(file,"%d %c %d %d",&joueurs[Numbr_lignes].idJoueur,&joueurs[Numbr_lignes].symbole,&joueurs[Numbr_lignes].or,&joueurs[Numbr_lignes].groupe_allies);
            Numbr_lignes++;
        }
    }
    fclose(file);

    return Numbr_lignes;
}
int chargerFichesTypesUnitesVersTableau(FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES], char* nomFichier){
    FILE *file;
    file=fopen(nomFichier,"r");
    int Numbr_lignes=0;
    if(file==NULL){
        printf("erreur loading file");
        return 0;
    }
    else{
        while(!feof(file)){     
            fscanf(file,"%d %d %s %d %d %d %d %d %d %d %d %d %d %d %s %d %d %d %d %d %d %d %d %d %d %d %d %d",&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].idFicheTypeUnite,&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].race,fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].nom,&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].prix,&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].pvMax,&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].mouvementsMax,&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].xpRequise,&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].niveau,&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].alignement,&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].idFichesTypeUniteRangInf[0],&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].idFichesTypeUniteRangInf[1],&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].idFichesTypeUniteRangInf[2],&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].idFichesTypeUniteRangSup[0],&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].idFichesTypeUniteRangSup[1],&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].idFichesTypeUniteRangSup[2],&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].attaques[MAX_LIGNES_FICHES_TYPES_UNITES].idAttaque,&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].attaques[MAX_LIGNES_FICHES_TYPES_UNITES].nomAttaque,&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].attaques[MAX_LIGNES_FICHES_TYPES_UNITES].degat,&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].resistances[0],&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].resistances[1],&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].resistances[2],&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].resistances[3],&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].resistances[4],&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].resistances[5],&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].relationsTerrains[MAX_LIGNES_FICHES_TYPES_UNITES].idTypeTerrain,&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].relationsTerrains[MAX_LIGNES_FICHES_TYPES_UNITES].coutMouvement,&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].relationsTerrains[MAX_LIGNES_FICHES_TYPES_UNITES].defense,&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].idCapacites[0],&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].idCapacites[1],&fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES].idCapacites[2]);
            Numbr_lignes++;
        }
    }
    fclose(file);

    return Numbr_lignes;
}
void main(){
    char nomFichier;
    //tables declaration
    UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN];
    TypeTerrain typesTerrains[NB_TYPES_TERRAINS];
    Village villages[MAX_LIGNES_VILLAGES];
    Periode periodes[NB_LIGNES_PERIODES];
    Joueur joueurs[MAX_LIGNES_JOUEURS];
    RelationTerrain relationsTerrains[NB_TYPES_TERRAINS];
    FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES];
    // varriables declaration
    int nbUnitesMagasin,nbVillages,number_ligne_type_terrain,IndiceRelationTerrain,number_ligne_periode,NbJoueur,NbficheTypesUnites,IndiceJoueur;
    //function calls
    nbUnitesMagasin=chargerUnitesMagasinVersTableau(unitesMagasin,"./files/unitesMagasin_original.txt");
    nbVillages=chargerVillagesVersTableau(villages,"./files/villages_original.txt");
    number_ligne_type_terrain=chargerTypesTerrainsVersTableau( typesTerrains,"./files/typesTerrains_original.txt");
    number_ligne_periode=chargerPeriodesVersTableau( periodes,"./files/periodes_original.txt");
    afficherUnitesMagasin(nbUnitesMagasin,unitesMagasin);
    afficherVillages(nbVillages, villages);
    afficherTypesTerrains(typesTerrains);
    afficherPeriodes(number_ligne_periode,periodes);
    sauvegarderVillages(nbVillages,villages,"villages_sauvegarde.txt");
    NbJoueur=chargerJoueursVersTableau(joueurs,"./files/joueurs_originals.txt");
    IndiceJoueur=chercherIndiceJoueur(1,2,joueurs);
    NbficheTypesUnites=chargerFichesTypesUnitesVersTableau(fichesTypesUnites,"./files/fichesTypesUnites_original.txt");
    IndiceRelationTerrain=chercherIndiceRelationTerrain(1,relationsTerrains);

}