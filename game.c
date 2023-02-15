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
        while(fgets(line,sizeof(line),file)!=NULL){
            sscanf(line,"%d %d %d",&unitesMagasin[Numbr_lignes].idUniteMagasin,&unitesMagasin[Numbr_lignes].idFicheTypeUnite,&unitesMagasin[Numbr_lignes].idJoueurAutorise);
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
        while(fgets(line,sizeof(line),file)!=NULL){
            sscanf(line,"%d %d %d %d",&villages[Numbr_lignes].idVillage,&villages[Numbr_lignes].ligne,&villages[Numbr_lignes].colonne,&villages[Numbr_lignes].idJoueurProprietaire);
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
        while(fgets(line,sizeof(line),file)!=NULL){
            sscanf(line,"%d %c %d %s",&typesTerrains[Numbr_lignes].idTypeTerrain,&typesTerrains[Numbr_lignes].symboleTerrain,&typesTerrains[Numbr_lignes].codeAffichageTerrain,&typesTerrains[Numbr_lignes].nomTerrain);
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
        char ligne[100];
        while(fgets(ligne,sizeof(ligne),file)!=NULL){
            char line=ligne[sizeof(ligne-6)];
            for(int i=sizeof(ligne-6);i<sizeof(ligne);i++){
                //i stoped here
            }
            sscanf(line,"%d %s",&periodes[Numbr_lignes].numOrdre,&periodes[Numbr_lignes].moment);
            Numbr_lignes++;
        }
    }
    return Numbr_lignes;
}
void main(){
    char nomFichier;
    UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN];
    TypeTerrain typesTerrains[NB_TYPES_TERRAINS];
    Village villages[MAX_LIGNES_VILLAGES];
    int number_lignes_unites_magasin,number_ligne_village,number_ligne_type_terrain;
    number_lignes_unites_magasin=chargerUnitesMagasinVersTableau(unitesMagasin,"unitesMagasin_original.txt");
    number_ligne_village=chargerVillagesVersTableau(villages,"villages_original.txt");
    number_ligne_type_terrain=chargerTypesTerrainsVersTableau( typesTerrains,"typesTerrains_original.txt");
    printf("%d",number_ligne_type_terrain);
    
    

}