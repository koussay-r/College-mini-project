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
int idUniteMagasin;// Ces id sont des numéros d'ordre 1,2... Mettre 0 dans la case où on n’a pas
//d'unité
int idFicheTypeUnite;
int idJoueurAutorise;
} UniteMagasin;
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
            sscanf(line,"%d %d %d",&unitesMagasin[Numbr_lignes].idFicheTypeUnite,&unitesMagasin[Numbr_lignes].idJoueurAutorise,&unitesMagasin[Numbr_lignes].idUniteMagasin);
            Numbr_lignes++;
        }
        return Numbr_lignes;
    }
    fclose(file);
    

}
void main(){
    char nomFichier;
    UniteMagasin unitesMagasin[MAX_LIGNES_UNITESMAGASIN];
    int number_lignes_unites_magasin;
    number_lignes_unites_magasin=chargerUnitesMagasinVersTableau(unitesMagasin,"unitesMagasin_original.txt");
    printf("%d",number_lignes_unites_magasin);

}