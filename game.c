#include <stdio.h>
#include <string.h>
//constants
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
int chercherIndiceTypeTerrain(char symboleTerrain, TypeTerrain typesTerrains[NB_TYPES_TERRAINS]){
    int IndiceJoueur=-1;
    int VarParcour=0;
    while (VarParcour<NB_TYPES_TERRAINS&&IndiceJoueur==-1)
    {
        if(typesTerrains[VarParcour].symboleTerrain==symboleTerrain){
            IndiceJoueur=VarParcour;
        }
        else{
            VarParcour++;
        }
    }
    return IndiceJoueur;
}
int chercherIndiceUniteJoueur(int idUnite,int idJoueur,int active, UniteJoueur Unitejoueurs[MAX_LIGNES_JOUEURS]){
    int IndiceJoueur=-1;
    int VarParcour=0;
    while (VarParcour<MAX_LIGNES_JOUEURS&&IndiceJoueur==-1)
    {
        if(Unitejoueurs[VarParcour].idUnite==idUnite&&Unitejoueurs[VarParcour].idJoueur==idJoueur&&Unitejoueurs[VarParcour].active==active){
            IndiceJoueur=VarParcour;
        }
        else{
            VarParcour++;
        }
    }
    return IndiceJoueur;
}
int chercherIndiceFicheTypeUnite(int idFicheTypeUnite, int nbFichesTypesUnites, FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES]){
    int IndiceJoueur=-1;
    int VarParcour=0;
    while (VarParcour<nbFichesTypesUnites&&IndiceJoueur==-1)
    {
        if(fichesTypesUnites[VarParcour].idFicheTypeUnite==idFicheTypeUnite){
            IndiceJoueur=VarParcour;
        }
        else{
            VarParcour++;
        }
    }
    return IndiceJoueur;
}
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
            fscanf(file,"%d %d %s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",&fichesTypesUnites[Numbr_lignes].idFicheTypeUnite,&fichesTypesUnites[Numbr_lignes].race,fichesTypesUnites[Numbr_lignes].nom,&fichesTypesUnites[Numbr_lignes].prix,&fichesTypesUnites[Numbr_lignes].pvMax,&fichesTypesUnites[Numbr_lignes].mouvementsMax,&fichesTypesUnites[Numbr_lignes].xpRequise,&fichesTypesUnites[Numbr_lignes].niveau,&fichesTypesUnites[Numbr_lignes].alignement,&fichesTypesUnites[Numbr_lignes].idFichesTypeUniteRangInf[0],&fichesTypesUnites[Numbr_lignes].idFichesTypeUniteRangInf[1],&fichesTypesUnites[Numbr_lignes].idFichesTypeUniteRangInf[2],&fichesTypesUnites[Numbr_lignes].idFichesTypeUniteRangSup[0],&fichesTypesUnites[Numbr_lignes].idFichesTypeUniteRangSup[1],&fichesTypesUnites[Numbr_lignes].idFichesTypeUniteRangSup[2],&fichesTypesUnites[Numbr_lignes].attaques[0].idAttaque,&fichesTypesUnites[Numbr_lignes].attaques[1].idAttaque,&fichesTypesUnites[Numbr_lignes].attaques[2].idAttaque,&fichesTypesUnites[Numbr_lignes].resistances[0],&fichesTypesUnites[Numbr_lignes].resistances[1],&fichesTypesUnites[Numbr_lignes].resistances[2],&fichesTypesUnites[Numbr_lignes].resistances[3],&fichesTypesUnites[Numbr_lignes].resistances[4],&fichesTypesUnites[Numbr_lignes].resistances[5],&fichesTypesUnites[Numbr_lignes].relationsTerrains[0].idTypeTerrain,&fichesTypesUnites[Numbr_lignes].idCapacites[0],&fichesTypesUnites[Numbr_lignes].idCapacites[1],&fichesTypesUnites[Numbr_lignes].idCapacites[2]);
            Numbr_lignes++;
        }
    }
    fclose(file);

    return Numbr_lignes;
}
void chargerUnitesJoueursVersTableau(UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS],char* nomFichier){
     FILE *file;
    file=fopen(nomFichier,"r");
    int Numbr_lignes=0;
    if(file==NULL){
        printf("erreur loading file");
        
    }
    else{
        while(!feof(file)){     
            fscanf(file,"%d %d %d %s %d %d %d %d %d %d %d",&unitesJoueurs[Numbr_lignes].idUnite,&unitesJoueurs[Numbr_lignes].idFicheTypeUnite,&unitesJoueurs[Numbr_lignes].idJoueur,unitesJoueurs[Numbr_lignes].nomUnite,&unitesJoueurs[Numbr_lignes].active,&unitesJoueurs[Numbr_lignes].rang,&unitesJoueurs[Numbr_lignes].ligne,&unitesJoueurs[Numbr_lignes].colonne,&unitesJoueurs[Numbr_lignes].traits[0],&unitesJoueurs[Numbr_lignes].traits[1],&unitesJoueurs[Numbr_lignes].traits[2]);
            Numbr_lignes++;
        }
    }
    fclose(file);
}
void chargerCarteVersTableau(CelluleCarte carte[NB_LIGNES_CARTE][ NB_COLONNES_CARTE], TypeTerrain typesTerrains[NB_TYPES_TERRAINS], char* nomFichier){
    FILE *file;
    file=fopen(nomFichier,"r");
    for (int i = 0; i < NB_LIGNES_CARTE; i++){
    char ligne[100];
    fgets(ligne,100,file);
        for (int j = 0; j < (NB_COLONNES_CARTE*2)-1; j++)
        {
            if(strcmp(&ligne[j]," ")!= 0){
            carte[i][j].idUnite=j+i+1;
            carte[i][j].idTypeTerrain=typesTerrains[chercherIndiceTypeTerrain(ligne[j],typesTerrains)].idTypeTerrain;
            carte[i][j].codeAffichageTerrain=ligne[j];
            }
        }
        
    }
    fclose(file);
     
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
void afficherPeriodes(Periode periodes[NB_LIGNES_PERIODES]){
    for(int i=0;i<NB_LIGNES_PERIODES;i++){
        printf("%d %s %d %d %d\n",periodes[i].numOrdre,periodes[i].moment,periodes[i].bonus[0],periodes[i].bonus[1],periodes[i].bonus[2]);
    }
    printf("\n");

}
void afficherJoueurs(int nbJoueurs, Joueur joueurs[MAX_LIGNES_JOUEURS]){
    for(int i=0;i<nbJoueurs;i++){
        printf("%d %c %d %d\n",joueurs[i].idJoueur,joueurs[i].symbole,joueurs[i].or,joueurs[i].groupe_allies);
    }
    printf("\n");
    
}
void afficherFichesTypesUnites(int nbFichesTypesUnites, FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES]){
    for(int i=0;i<nbFichesTypesUnites;i++){
        printf("%d %d %s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",fichesTypesUnites[i].idFicheTypeUnite,fichesTypesUnites[i].race,fichesTypesUnites[i].nom,fichesTypesUnites[i].prix,fichesTypesUnites[i].pvMax,fichesTypesUnites[i].mouvementsMax,fichesTypesUnites[i].xpRequise,fichesTypesUnites[i].niveau,fichesTypesUnites[i].alignement,fichesTypesUnites[i].idFichesTypeUniteRangInf[0],fichesTypesUnites[i].idFichesTypeUniteRangInf[1],fichesTypesUnites[i].idFichesTypeUniteRangInf[2],fichesTypesUnites[i].idFichesTypeUniteRangSup[0],fichesTypesUnites[i].idFichesTypeUniteRangSup[1],fichesTypesUnites[i].idFichesTypeUniteRangSup[2],fichesTypesUnites[i].attaques[0].idAttaque,fichesTypesUnites[i].attaques[1].idAttaque,fichesTypesUnites[i].attaques[2].idAttaque,fichesTypesUnites[i].resistances[0],fichesTypesUnites[i].resistances[1],fichesTypesUnites[i].resistances[2],fichesTypesUnites[i].resistances[3],fichesTypesUnites[i].resistances[4],fichesTypesUnites[i].resistances[5],fichesTypesUnites[i].relationsTerrains[0].idTypeTerrain,fichesTypesUnites[i].idCapacites[0],fichesTypesUnites[i].idCapacites[1],fichesTypesUnites[i].idCapacites[2]);
    }
    printf("\n");
}
void afficherUniteJoueur( UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS]){
     for(int i=0;i<MAX_LIGNES_UNITESJOUEURS;i++){
        printf("%d %d %d %s %d %d %d %d %d %d %d\n",unitesJoueurs[i].idUnite,unitesJoueurs[i].idFicheTypeUnite,unitesJoueurs[i].idJoueur,unitesJoueurs[i].nomUnite,unitesJoueurs[i].active,unitesJoueurs[i].rang,unitesJoueurs[i].ligne,unitesJoueurs[i].colonne,unitesJoueurs[i].traits[0],unitesJoueurs[i].traits[1],unitesJoueurs[i].traits[2]);
    }
    printf("\n");
            

}
void afficherCarte(CelluleCarte carte[NB_LIGNES_CARTE][ NB_COLONNES_CARTE]){
    for (int j = 0; j < NB_LIGNES_CARTE; j++)
    {
    for(int i=0;i<NB_COLONNES_CARTE;i++){
        printf("%d %d %d ||",carte[i][j].idUnite,carte[i][j].idTypeTerrain,carte[i][j].codeAffichageTerrain);
    }
    printf("\n");       
    }
    
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
        fprintf(file,"%d %s %d %s\n",typesTerrains[i].idTypeTerrain,typesTerrains[i].symboleTerrain,typesTerrains[i].codeAffichageTerrain,typesTerrains[i].nomTerrain);
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
void sauvegarderJoueurs(int nbJoueurs, Joueur joueurs[MAX_LIGNES_JOUEURS] , char* nomFichier){
    FILE *file=fopen(nomFichier,"w");
    for(int i=0;i<nbJoueurs;i++){
        fprintf(file,"%d %c %d %d\n",joueurs[i].idJoueur,joueurs[i].symbole,joueurs[i].or,joueurs[i].groupe_allies);
    }
    fclose(file);
}
void sauvegarderUniteJoueur(UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS] , char* nomFichier){
    FILE *file=fopen(nomFichier,"w");
    for(int i=0;i<MAX_LIGNES_UNITESJOUEURS;i++){
         fprintf(file,"%d %d %d %s %d %d %d %d %d %d %d\n",unitesJoueurs[i].idUnite,unitesJoueurs[i].idFicheTypeUnite,unitesJoueurs[i].idJoueur,unitesJoueurs[i].nomUnite,unitesJoueurs[i].active,unitesJoueurs[i].rang,unitesJoueurs[i].ligne,unitesJoueurs[i].colonne,unitesJoueurs[i].traits[0],unitesJoueurs[i].traits[1],unitesJoueurs[i].traits[2]);
    }
    fclose(file);
}

void main(){
    //tables declaration
    UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN];
    TypeTerrain typesTerrains[NB_TYPES_TERRAINS];
    Village villages[MAX_LIGNES_VILLAGES];
    Periode periodes[NB_LIGNES_PERIODES];
    Joueur joueurs[MAX_LIGNES_JOUEURS];
    RelationTerrain relationsTerrains[NB_TYPES_TERRAINS];
    FicheTypeUnite fichesTypesUnites[MAX_LIGNES_FICHES_TYPES_UNITES];
    UniteJoueur Unitejoueurs[MAX_LIGNES_JOUEURS];
    UniteJoueur unitesJoueurs[MAX_LIGNES_UNITESJOUEURS];
    CelluleCarte carte[NB_LIGNES_CARTE][ NB_COLONNES_CARTE];
    // varriables declaration
    int nbUnitesMagasin,IndiceJoueur1,nbVillages,IndiceTypeTerrain,number_ligne_type_terrain,IndiceRelationTerrain,number_ligne_periode,NbJoueur,NbficheTypesUnites,IndiceJoueur;
    //function calls
    nbUnitesMagasin=chargerUnitesMagasinVersTableau(unitesMagasin,"unitesMagasin_original.txt");
    nbVillages=chargerVillagesVersTableau(villages,"villages_original.txt");
    number_ligne_type_terrain=chargerTypesTerrainsVersTableau( typesTerrains,"typesTerrains_original.txt");
    number_ligne_periode=chargerPeriodesVersTableau( periodes,"periodes_original.txt");
    NbJoueur=chargerJoueursVersTableau(joueurs,"joueurs_originals.txt");
    NbficheTypesUnites=chargerFichesTypesUnitesVersTableau(fichesTypesUnites,"fichesTypesUnites_original.txt");
    chargerUnitesJoueursVersTableau(unitesJoueurs,"unitesJoueurs_original.txt");
    chargerCarteVersTableau(carte,typesTerrains,"carte_original.txt");
    afficherUnitesMagasin(nbUnitesMagasin,unitesMagasin);
    afficherVillages(nbVillages, villages);
    afficherTypesTerrains(typesTerrains);
    afficherPeriodes(periodes);
    afficherJoueurs(NbJoueur,joueurs);
    afficherFichesTypesUnites(NbficheTypesUnites,fichesTypesUnites);
    afficherUniteJoueur(unitesJoueurs);
    afficherCarte(carte);
    sauvegarderVillages(nbVillages,villages,"villages_sauvegarde.txt");
    sauvegarderJoueurs(NbJoueur,joueurs,"joueurs_sauvegarde.txt");
    sauvegarderUniteJoueur(unitesJoueurs,"unitesJoueurs_sauvegarde.txt");
}