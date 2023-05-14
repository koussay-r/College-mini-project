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
    afficherTableauxJeu( nbfichesTypesUnites,  nbJoueurs, nbUnitesMagasin, nbVillages, fichesTypesUnites, joueurs, carte, unitesMagasin, villages, typesTerrains, periodes, unitesJoueurs);
    afficherJoueurJeu(48,2,joueurs);
    afficherUnitesJoueursJeu(1, 1,unitesJoueurs);
    int ligne,colonne;
    printf("\nentrer la ligne et la colone du joueur pour marker les deplacements possible\n");
    scanf("%d %d",&ligne,&colonne);
    marquerDeplacementsPossibles( carte,ligne,colonne,4);
    afficherJoueurJeu(48,2,joueurs);
    afficherUnitesJoueursJeu(1, 1,unitesJoueurs);
    afficherCarteJeu( carte,  nbJoueurs, joueurs,nbVillages,villages,unitesJoueurs);
    printf("\nclickez 1 si vous voulez effacer les deplacement possibles 0 si vous voulez continuer 2 si vous voulez deplacer un joueur \n");
    int effaceDeplacement;
    scanf("%d",&effaceDeplacement);
    if(effaceDeplacement==1){
        effacerDeplacementsPossibles(carte);
            afficherJoueurJeu(48,2,joueurs);
    afficherUnitesJoueursJeu(1, 1,unitesJoueurs);
    afficherCarteJeu( carte,  nbJoueurs, joueurs,nbVillages,villages,unitesJoueurs);
    sauvegarderJeuComplet( nbVillages, nbJoueurs, villages, joueurs, unitesJoueurs);
    }
    else if (effaceDeplacement==0){
            afficherJoueurJeu(48,2,joueurs);
    afficherUnitesJoueursJeu(1, 1,unitesJoueurs);
    afficherCarteJeu( carte,  nbJoueurs, joueurs,nbVillages,villages,unitesJoueurs);
    sauvegarderJeuComplet( nbVillages, nbJoueurs, villages, joueurs, unitesJoueurs);
    }
    else if(effaceDeplacement==2){
        int nouvelle_colonne,nouvelle_ligne;
        printf("\nentrer la ligne et la colonne\n");
        scanf("%d %d",&nouvelle_ligne,&nouvelle_colonne);
        for(int i=0;i<MAX_LIGNES_UNITESJOUEURS;i++){
            if(unitesJoueurs[i].ligne==nouvelle_ligne&&unitesJoueurs[i].colonne==nouvelle_colonne){
        deplacerUnite(1,i,unitesJoueurs[i].idUnite, unitesJoueurs[i].ligne, unitesJoueurs[i].colonne, nouvelle_ligne,nouvelle_colonne,nbJoueurs,nbVillages,carte,joueurs,unitesJoueurs,villages);
            }
        }
    effacerDeplacementsPossibles(carte);
        afficherJoueurJeu(48,2,joueurs);
    afficherUnitesJoueursJeu(1, 1,unitesJoueurs);
    afficherCarteJeu( carte,  nbJoueurs, joueurs,nbVillages,villages,unitesJoueurs);
    sauvegarderJeuComplet( nbVillages, nbJoueurs, villages, joueurs, unitesJoueurs);
    }
    int tour;
    printf("\nentrer 1 si vous voulez reinitialiser Tours ou 0 si vous voulez verifier\n");
    scanf("%d",&tour);
    if(tour==0){
         verifierFinTours(1, unitesJoueurs);
         afficherJoueurJeu(48,2,joueurs);
    afficherUnitesJoueursJeu(1, 1,unitesJoueurs);
    afficherCarteJeu( carte,  nbJoueurs, joueurs,nbVillages,villages,unitesJoueurs);
    sauvegarderJeuComplet( nbVillages, nbJoueurs, villages, joueurs, unitesJoueurs);

    }else if(tour==1){
         reinitialiserTours( 1, nbJoueurs, joueurs, unitesJoueurs);
         afficherJoueurJeu(48,2,joueurs);
    afficherUnitesJoueursJeu(1, 1,unitesJoueurs);
    afficherCarteJeu( carte,  nbJoueurs, joueurs,nbVillages,villages,unitesJoueurs);
    sauvegarderJeuComplet( nbVillages, nbJoueurs, villages, joueurs, unitesJoueurs);
    }
    }
    else if(menu==2){
    chargerJeuComplet(&nbfichesTypesUnites, &nbJoueurs, &nbUnitesMagasin,&nbVillages, fichesTypesUnites, joueurs, carte, unitesMagasin, villages, typesTerrains, periodes, unitesJoueurs);
    }
}