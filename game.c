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
typedef struct
{
int numOrdre;
char moment[20];
int bonus[4]; // cases : 0 -> bonus lawful, 1 -> bonus chaotic, 2 -> bonus liminal, 3 -> bonus neutral
} Periode;
int chargerUnitesMagasinVersTableau(UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN], char* nomFichier){
    FILE *file;
    int Numbr_lignes=0;
    file=fopen(nomFichier,"r");
    if(file==NULL){
        printf("erreur loading file");
        return 0;
    }
    else{
        char line[100];
        while(!feof(file)){
            fscanf(file,"%d %d %d",&unitesMagasin[Numbr_lignes].idUniteMagasin,&unitesMagasin[Numbr_lignes].idFicheTypeUnite,&unitesMagasin[Numbr_lignes].idJoueurAutorise);
            Numbr_lignes++;
        }
        return Numbr_lignes;
    }
    fclose(file);
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
        char line[100];
        while(!feof(file)){
            fscanf(file,"%d %d %d %d",&villages[Numbr_lignes].idVillage,&villages[Numbr_lignes].ligne,&villages[Numbr_lignes].colonne,&villages[Numbr_lignes].idJoueurProprietaire);
            Numbr_lignes++;
        }
    }
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
        char line[100];
        while(!feof(file)){
            fscanf(file,"%d %c %d %s",&typesTerrains[Numbr_lignes].idTypeTerrain,&typesTerrains[Numbr_lignes].symboleTerrain,&typesTerrains[Numbr_lignes].codeAffichageTerrain,&typesTerrains[Numbr_lignes].nomTerrain);
            Numbr_lignes++;
        }
    }
    return Numbr_lignes;
}
int chargerPeriodesVersTableau(Periode periodes[MAX_LIGNES_PERIODES], char* nomFichier){
    FILE *file;
    file=fopen(nomFichier,"r");
    int Numbr_lignes=0;
    if(file==NULL){
        printf("erreur loading file");
        return 0;
    }
    else{
        char line[100];
        while(!feof(file)){     
            fscanf(file,"%d %s %d %d %d",&periodes[Numbr_lignes].numOrdre,&periodes[Numbr_lignes].moment,&periodes[Numbr_lignes].bonus[0],&periodes[Numbr_lignes].bonus[1],&periodes[Numbr_lignes].bonus[2]);
            Numbr_lignes++;
        }
    }
    return Numbr_lignes;
}
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
void afficherTypesTerrains(int nb_lignes, TypeTerrain typesTerrains[NB_TYPES_TERRAINS]){
    for(int i=0;i<nb_lignes;i++){
        printf("%d %c %d %s \n",typesTerrains[i].idTypeTerrain,typesTerrains[0].symboleTerrain,typesTerrains[i].codeAffichageTerrain,typesTerrains[i].nomTerrain);
    }
    printf("\n");
}
void afficherPeriodes(int nbLignes, Periode periodes[MAX_LIGNES_PERIODES]){
    for(int i=0;i<nbLignes;i++){
        printf("%d %s %d \n",periodes[i].numOrdre,periodes[0].moment,periodes[i].bonus[0],periodes[i].bonus[1],periodes[i].bonus[2]);
    }
    printf("\n");
}
void main(){
    char nomFichier;
    UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN];
    TypeTerrain typesTerrains[NB_TYPES_TERRAINS];
    Village villages[MAX_LIGNES_VILLAGES];
    Periode periodes[MAX_LIGNES_PERIODES];
    int number_lignes_unites_magasin,number_ligne_village,number_ligne_type_terrain,number_ligne_periode;
    number_lignes_unites_magasin=chargerUnitesMagasinVersTableau(unitesMagasin,"./files/unitesMagasin_original.txt");
    number_ligne_village=chargerVillagesVersTableau(villages,"./files/villages_original.txt");
    number_ligne_type_terrain=chargerTypesTerrainsVersTableau( typesTerrains,"./files/typesTerrains_original.txt");
    number_ligne_periode=chargerPeriodesVersTableau( periodes,"./files/periodes_original.txt");
    afficherUnitesMagasin(number_lignes_unites_magasin,unitesMagasin);
    afficherVillages(number_ligne_village, villages);
    afficherTypesTerrains(number_ligne_type_terrain,typesTerrains);
    afficherPeriodes(number_ligne_periode,periodes);

}