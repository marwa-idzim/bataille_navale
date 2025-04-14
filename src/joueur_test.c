/* joueur : programme test

	Fonctions testees :

		main :
			joueur* joueur_construire(const char* pseudo, unsigned int tab_nb_par_type[FLOTTE_NB_TYPE_BATEAU], unsigned int nbL, unsigned int nbC);
			void joueur_detruire(joueur** j);
			void joueur_set_pseudo(joueur* j, const char* pseudo);

		joueur_test_afficher :
			const char* joueur_get_pseudo(joueur* j);

		joueur_test_afficher_flotte :
			unsigned int joueur_get_flotte_nb_bateaux(joueur* j);
			unsigned int joueur_get_flotte_taille(joueur* j);
			unsigned int joueur_get_flotte_type_nb_bateaux(joueur* j, int cleType);
			unsigned int joueur_get_nb_bateaux_en_mer(joueur* j);
			int joueur_is_bateau_en_mer(joueur* j, int cleType, unsigned int indBateau);
			void joueur_get_bateau_position(joueur* j, int cleType, unsigned int indBateau, int* xD, int* yD, int* xF, int* yF);

		joueur_test_afficher_grille :
			unsigned int joueur_get_grille_joueur_nbLignes(joueur* j);
			unsigned int joueur_get_grille_joueur_nbColonnes(joueur* j);
			unsigned int joueur_get_grille_adversaire_nbLignes(joueur* j);
			unsigned int joueur_get_grille_adversaire_nbColonnes(joueur* j);

			int joueur_get_grille_joueur_case_etat(joueur* j, unsigned int x, unsigned int y);
			int joueur_get_grille_joueur_case_type_bateau(joueur* j, unsigned int x, unsigned int y);
			int joueur_get_grille_joueur_case_id_bateau_type(joueur* j, unsigned int x, unsigned int y);
			int joueur_get_grille_adversaire_case_etat(joueur* j, unsigned int x, unsigned int y);
			int joueur_get_grille_adversaire_case_type_bateau(joueur* j, unsigned int x, unsigned int y);

		joueur_test_jeu :
			int joueur_get_nb_coups_joues_adversaire(joueur* j);
			int joueur_get_nb_coups_joues_joueur(joueur* j);
			int joueur_get_nb_coups_recus_touche(joueur* j);
			int joueur_get_nb_coups_donnes_touche(joueur* j);
			int joueur_is_flotte_coulee(joueur* j);
			int joueur_is_flotte_adverse_coulee(joueur* j);
			void joueur_recevoir_coup(joueur* j, unsigned int x, unsigned int y);
			void joueur_set_case_adversaire(joueur* j, int cleType, case_grille_etat etat_case, unsigned int x, unsigned int y);
			void joueur_get_case_joueur(joueur* j, int* cleType, case_grille_etat* etat_case, unsigned int x, unsigned int y);
void joueur_set_coup_a_jouer(joueur* j, unsigned int* x, unsigned int* y);
			int joueur_placer_flotte(joueur* j);

	TODO (tests manquant) :

		Scenarios :
			scenarii de placement et de jeu deterministes

		Fonctions :
			void joueur_initialiser(joueur* j);
			joueur* joueur_redimensionner_flotte(joueur* j, unsigned int tab_nb_par_type[FLOTTE_NB_TYPE_BATEAU]);
			void joueur_redimensionner_grilles(joueur* j, unsigned int nbL, unsigned int nbC);

			int joueur_is_bateau_coule(joueur* j, int cleType, unsigned int indBateau);
			int joueur_get_placement_valide(joueur* j, int cleType, unsigned int x, unsigned int y);
			int joueur_is_coup_valide(joueur* j, unsigned int x, unsigned int y);
			int joueur_placer_bateau(joueur* j, int cleType, unsigned int indBateau, unsigned int x, unsigned int y, placement_type orientation);
*/

#include "joueur.h"

#include <stdlib.h>				/* pour EXIT_SUCCESS						*/
#include <stdio.h>				/* pour affichages						*/
#include <time.h>				/* pour fonctions de jeu aleatoires		*/

/* ____________________ liste des fonctions */

/* Fonction d'affichage flotte */
void joueur_test_afficher_flotte(joueur* j);

/* Fonction d'affichage grilles */
void joueur_test_afficher_grilles(joueur* j);

/* Fonction d'affichage joueur faisant appel aux accesseurs :: */
void joueur_test_afficher(joueur* j);

/* Fonction d'affichage all :: */
void joueur_test_afficher_all(joueur* j);

/* Fonction test jeu (jeu aleatoire) */
void joueur_test_jeu(joueur* j1, joueur* j2);

/* Fonction principale */
int main(void);

/* ____________________ Definitions des fonctions */

/* ____ Fonctions d'affichage :: */

/* Fonction d'affichage flotte */
void joueur_test_afficher_flotte(joueur* j) {
	unsigned int indBateau;
	int cleType, xD, yD, xF, yF;

	printf("Affichage flotte joueur %p ::\n", (void*)j);
	if (j == NULL) {
		printf("\tjoueur non instancie.\n");
		return;
	}

	if (j->flotte_joueur == NULL) {
		printf("flotte joueur non instanciee.\n");
		return;
	}

	/* __ accesseurs flotte */
	printf("Affichage flotte %p::\tla flotte est constituee des %u bateaux suivants (taille total %u) dont %u sont en mer :\n", (void*)j->flotte_joueur, joueur_get_flotte_nb_bateaux(j), joueur_get_flotte_taille(j), joueur_get_nb_bateaux_en_mer(j));

	for (cleType = BATEAU_TYPE_CLE_NONE +1 ; cleType <= BATEAU_TYPE_CLE_NB -1 ; cleType ++) {
		/* __ accesseurs types de bateaux de la flotte */
		printf("\t\t%u bateau(x) de type %s de longueur %u :\n", joueur_get_flotte_type_nb_bateaux(j, cleType), bateau_type_get_nom(cleType), bateau_type_get_longueur(cleType));

		for (indBateau = 0 ; indBateau < flotte_get_type_nb_bateaux(j->flotte_joueur, cleType) ; indBateau ++) {
			/* __ accesseurs bateaux de la flotte */
			if (joueur_is_bateau_en_mer(j, cleType, indBateau) == 0) {
				printf("\t\t\tbateau d'indice %u a quai\n", indBateau);
			}
			else {
				joueur_get_bateau_position(j, cleType, indBateau, &xD, &yD, &xF, &yF);
				printf("\t\t\tbateau d'indice %u en mer place des coordonnees (%d,%d) aux coordonnes (%d,%d)\n", indBateau, xD, yD, xF, yF);
			}
		}
	}
}

/* Fonction d'affichage grilles */
void joueur_test_afficher_grilles(joueur* j) {
	if (j == NULL) {
		printf("%s: joueur non instancie.\n", __func__);
	}
	else if (j->grille_joueur == NULL) {
		printf("%s: grille joueur du joueur %p non instanciee.\n", __func__, (void*)j);
	}
	else if (j->grille_adversaire == NULL) {
		printf("%s: grille adversaire du joueur %p non instanciee.\n", __func__, (void*)j);
	}
	else if (grille_get_nbLignes(j->grille_joueur) != grille_get_nbLignes(j->grille_adversaire) || grille_get_nbColonnes(j->grille_joueur) != grille_get_nbColonnes(j->grille_adversaire)) {
		printf("%s: grille du joueur %p de dimensions %ux%u distinctes de grille adversaire de dimensions %ux%u.\n", __func__, (void*)j, joueur_get_grille_joueur_nbLignes(j), joueur_get_grille_joueur_nbColonnes(j), joueur_get_grille_adversaire_nbLignes(j), joueur_get_grille_adversaire_nbColonnes(j));
	}
	else {
		joueur_afficher_grilles(j);
	}
}

/* Fonction d'affichage joueur faisant appel aux accesseurs */
void joueur_test_afficher(joueur* j) {
	printf("Affichage joueur %p ::\n", (void*)j);
	if (j == NULL) {
		printf("\tjoueur non instancie.\n");
	}
	else {
		/* __ accesseurs joueur */
		printf("\tle joueur de pseudo %s:\n", joueur_get_pseudo(j));
	}
}

/* Fonction d'affichage all */
void joueur_test_afficher_all(joueur* j) {
	joueur_test_afficher(j);
	joueur_test_afficher_flotte(j);
	joueur_test_afficher_grilles(j);
}

/* ____ Scenarios */

/* Fonction test jeu (jeu aleatoire) */
void joueur_test_jeu(joueur* j1, joueur* j2) {
	int cleType, etat, tour = 0;
	unsigned int x, y;

	if (flotte_get_nb_bateaux_en_mer(j1->flotte_joueur) < flotte_get_nb_bateaux(j1->flotte_joueur)) {
		printf("%s: SORTIE: flotte joueur %s non placee.\n", __func__, j1->pseudo);
	}
	else if (flotte_get_nb_bateaux_en_mer(j2->flotte_joueur) < flotte_get_nb_bateaux(j2->flotte_joueur)) {
		printf("%s: SORTIE: flotte joueur %s non placee.\n", __func__, j2->pseudo);
	}
	else {
		while( (joueur_is_flotte_coulee(j1) == 0) && (joueur_is_flotte_adverse_coulee(j1) == 0) ) {
			if ((tour % 2) == 0) {
				/* j1 choisit le coup a jouer */
				joueur_set_coup_a_jouer(j1, &x, &y);

				/* j2 applique le coup recu */
				joueur_recevoir_coup(j2, x, y);

				/* recuperation sur j2 des infos de la case jouee a l'issue du coup */
				joueur_get_case_joueur(j2, &cleType, &etat, x, y);

				/* j1 l'applique a sa grille adversaire */
				joueur_set_case_adversaire(j1, cleType, etat, x, y);
			}
			else {
				/* j2 choisit le coup a jouer */
				joueur_set_coup_a_jouer(j2, &x, &y);

				/* j1 applique le coup recu */
				joueur_recevoir_coup(j1, x, y);

				/* recuperation sur j1 des infos de la case jouee a l'issue du coup */
				joueur_get_case_joueur(j1, &cleType, &etat, x, y);

				/* j2 l'applique a sa grille adversaire */
				joueur_set_case_adversaire(j2, cleType, etat, x, y);
			}

			tour++;
			printf("Vue du jeu par le joueur %s a l'issue du tour %d du joueur %s -- coup joue (%u,%u) ::\n", j1->pseudo, tour, (tour%2 == 0 ? j1->pseudo : j2->pseudo), x, y); 
			joueur_test_afficher_grilles(j1);
			getchar();
		}
	}

	printf("%s(%p): jeu termine au bout de %u coups joues par joueur %s qui commence et %u coups joues par l'adversaire avec %u coups gagnants pour joueur %s et %u coups gagnants pour l'adversaire. Flotte %s %s et flotte adversaire %s\n", __func__, (void*)j1, joueur_get_nb_coups_joues_joueur(j1), j1->pseudo, joueur_get_nb_coups_joues_adversaire(j1), joueur_get_nb_coups_donnes_touche(j1), j1->pseudo, joueur_get_nb_coups_recus_touche(j1), j1->pseudo, (grille_is_coule(j1->grille_joueur) ? "coulee" : "pas coulee"), (grille_is_coule(j1->grille_adversaire) ? "coulee" : "pas coulee"));
}

/* Fonction principale */
int main(void) {
	joueur* j1 = NULL;
	joueur* j2 = NULL;
	unsigned int tab_nb_par_type[FLOTTE_NB_TYPE_BATEAU] = JOUEUR_FLOTTE_DIM_DEFAULT;
	unsigned int nbL = JOUEUR_GRILLE_H_DEFAULT, nbC = JOUEUR_GRILLE_L_DEFAULT;
	char pseudo2[] = "Joueur 2";

	/* ____ construction */
	printf("________ TEST1 : construction (2 joueurs memes parametres)\n");

	j1 = joueur_construire(STR_JOUEUR_PSEUDO_DEFAULT, tab_nb_par_type, nbL, nbC);
	j2 = joueur_construire(STR_JOUEUR_PSEUDO_DEFAULT, tab_nb_par_type, nbL, nbC);
	joueur_set_pseudo(j2, "Joueur 2");
	printf("\nAppel joueur_joueur_construire sur j1 == %p et j2 == %p, utilisation des valeurs par defaut, puis j2 renomme %s :\n", (void*)j1, (void*)j2, pseudo2);

	joueur_test_afficher_all(j1);
	getchar();

	joueur_test_afficher_all(j2);
	getchar();

	/* ____ initialisation graine aleatoire */
	srand(time(NULL));

	/* ____ placement */
	printf("________ TEST2 : placement aleatoire (2 joueurs)\n");

	joueur_placer_flotte(j1);
	joueur_placer_flotte(j2);

	joueur_test_afficher_all(j1);
	getchar();

	joueur_test_afficher_all(j2);
	getchar();

	/* ____ jeu */
	printf("________ TEST3 : jeu aleatoire (2 joueurs)\n");
	joueur_test_jeu(j1, j2);

	/* ____ destruction */
	printf("________ TEST4 : destruction\n");

	printf("\nAppel joueur_detruire sur j1 == %p\n", (void*)j1);
	joueur_detruire(& j1);

	printf("\nAppel joueur_detruire sur j1 == %p\n", (void*)j1);
	joueur_detruire(& j1);

	printf("\nAppel joueur_detruire sur j2 == %p\n", (void*)j2);
	joueur_detruire(& j2);

	return EXIT_SUCCESS;
}
