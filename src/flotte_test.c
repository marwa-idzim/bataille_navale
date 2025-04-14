/* flotte : programme test

	Fonctions testees :

		main :
			flotte* flotte_construire(unsigned int tab_nb_par_type[FLOTTE_NB_TYPE_BATEAU]);
			void flotte_detruire(flotte** f);
			flotte* flotte_dimensionner(flotte* f, unsigned int tab_nb_par_type[FLOTTE_NB_TYPE_BATEAU]);
			void flotte_initialiser(flotte* f);
			void flotte_set_bateau_position(flotte* f, int cleType, unsigned int indBateau, unsigned int x, unsigned int y, placement_type orientation);

		flotte_test_afficher :
			unsigned int flotte_get_nb_bateaux(flotte* f);
			unsigned int flotte_get_taille(flotte* f);
			unsigned int flotte_get_type_nb_bateaux(flotte* f, int cleType);
			unsigned int flotte_get_nb_bateaux_en_mer(flotte* f);
			void flotte_get_bateau_position(flotte* f, int cleType, unsigned int indBateau, int* xD, int* yD, int* xF, int* yF);
			int flotte_is_bateau_en_mer(flotte* f, int cleType, unsigned int indBateau);
*/

#include "flotte.h"

#include <stdlib.h>				/* pour EXIT_SUCCESS		*/
#include <stdio.h>				/* pour affichages		*/

/* ____________________ liste des fonctions */

/* Fonction d'affichage tableau de dimensions :: */
void flotte_test_tab_dimension_afficher(unsigned int tab_dim[FLOTTE_NB_TYPE_BATEAU]);

/* Fonction d'affichage flotte faisant appel aux accesseurs :: */
void flotte_test_afficher(flotte* f);

/* ____ Fonction principale :: */
int main(void);

/* ____________________ definition des fonctions */

/* ____ Fonctions d'affichage :: */

/* Fonction d'affichage tableau de dimensions :: */
void flotte_test_tab_dimension_afficher(unsigned int tab_dim[FLOTTE_NB_TYPE_BATEAU]) {
	unsigned int indType;

	printf("tableau de dimensions\t");
	for (indType = 0 ; indType < FLOTTE_NB_TYPE_BATEAU ; indType ++)
		printf(" %d", tab_dim[indType]);
	printf("\n");
}

/* Fonction d'affichage flotte faisant appel aux accesseurs :: */
void flotte_test_afficher(flotte* f) {
	unsigned int indType, indBateau;
	int cleType, xD, yD, xF, yF;

	printf("Affichage flotte %p ::\n", (void*)f);
	if (f == NULL)
		printf("\tflotte non instanciee.\n");
	else  {
		/* __ accesseurs flotte */
		printf("\tla flotte est constituee des %u bateaux suivants (taille total %u) dont %u sont en mer :\n", flotte_get_nb_bateaux(f), flotte_get_taille(f), flotte_get_nb_bateaux_en_mer(f));

		for (cleType = BATEAU_TYPE_CLE_NONE +1 ; cleType <= BATEAU_TYPE_CLE_NB -1 ; cleType ++) {
			/* __ accesseurs types de bateaux de la flotte */
			printf("\t\t%u bateau(x) de type %s de longueur %u :\n", flotte_get_type_nb_bateaux(f,cleType), bateau_type_get_nom(cleType), bateau_type_get_longueur(cleType));

			indType = (unsigned int) (cleType -BATEAU_TYPE_CLE_NONE -1);
			for (indBateau = 0 ; indBateau < f->tab_nb_par_type[indType] ; indBateau ++) {
				/* __ accesseurs bateaux de la flotte */
				if (flotte_is_bateau_en_mer(f, cleType, indBateau) == 0)
					printf("\t\t\tbateau d'indice %u a quai\n", indBateau);
				else {
					flotte_get_bateau_position(f, cleType, indBateau, &xD, &yD, &xF, &yF);
					printf("\t\t\tbateau d'indice %u en mer place des coordonnees (%d,%d) aux coordonnes (%d,%d)\n", indBateau, xD, yD, xF, yF);
				}
			}
		}
	}
}

/* ____ Fonctions principale :: */

/* Fonction principale */
int main(void) {
	flotte* f = NULL;
	int cleType;
	unsigned int indBateau, x, y, tab_dim1[FLOTTE_NB_TYPE_BATEAU] = {1,1,1,1,1}, tab_dim2[FLOTTE_NB_TYPE_BATEAU] = {2,0,1,0,3};
	placement_type tp;

	/* ____ construction */	
	printf("________ TEST 1 : construction\n");

	f = flotte_construire(tab_dim1);

	printf("\nAppel %p =flotte_construire(%p)\t", (void*)f, (void*)tab_dim1);
	flotte_test_tab_dimension_afficher(tab_dim1);
	flotte_test_afficher(f);

	getchar();

	/* ____ affectation */	

	/* affectation #1 : placement horizontal */	
	printf("________ TEST 2 : affectation (placement horizontal)\n");

	tp = PLACEMENT_TYPE_H;
	x=2;
	y=3;
	for (cleType = BATEAU_TYPE_CLE_NONE +1 ; cleType < BATEAU_TYPE_CLE_NB ; cleType ++) 
		for (indBateau = 0 ; indBateau < flotte_get_type_nb_bateaux(f, cleType) ; indBateau ++)
			flotte_set_bateau_position(f, cleType, indBateau, x, y, tp);

	printf("\nAppel flotte_set_bateau_position sur tout bateau de %p, placement horizontal (%d) de coordonnes (%u,%u) :\n", (void*)f, tp, x, y);
	flotte_test_afficher(f);

	getchar();

	/* affectation #2 : placement vertical */	
	printf("________ TEST 3 : affectation (placement vertical)\n");

	tp = PLACEMENT_TYPE_V;
	x=3;
	y=2;
	for (cleType = BATEAU_TYPE_CLE_NONE +1 ; cleType < BATEAU_TYPE_CLE_NB ; cleType ++)
		for (indBateau = 0 ; indBateau < flotte_get_type_nb_bateaux(f, cleType) ; indBateau ++)
			flotte_set_bateau_position(f, cleType, indBateau, x, y, tp);

	printf("\nAppel flotte_set_bateau_position sur tout bateau de %p, placement vertical (%d) de coordonnes (%u,%u) :\n", (void*)f, tp, x, y);
	flotte_test_afficher(f);

	getchar();

	/* affectation #3 : initialisation */	
	printf("________ TEST 4 : affectation (initialisation)\n");

	flotte_initialiser(f);
	printf("\nAppel flotte_initialiser(%p) :\n", (void*)f);
	flotte_test_afficher(f);

	getchar();

	/* ____ redimensionnement */	
	printf("________ TEST 5 : redimensionnement\n");

	/* on met quelque chose pour verifier reinitialisation */
	for (cleType = BATEAU_TYPE_CLE_NONE +1 ; cleType < BATEAU_TYPE_CLE_NB ; cleType ++)
		for (indBateau = 0 ; indBateau < flotte_get_type_nb_bateaux(f, cleType) ; indBateau ++)
			flotte_set_bateau_position(f, cleType, indBateau, x, y, tp);

	printf("\nFlotte initiale :\n");
	flotte_test_afficher(f);

	printf("\nAppel f = flotte_dimensionner(%p, %p)", (void*)f, (void*)tab_dim2);
	flotte_test_tab_dimension_afficher(tab_dim2);
	f =flotte_dimensionner(f, tab_dim2);
	flotte_test_afficher(f);

	getchar();

	/* ____ destruction */	
	printf("________ TEST 6 : destruction\n");

	/* destruction #1 */
	flotte_detruire(&f);
	printf("\nResultat appel flotte_detruire(%p) : f == %p\n", (void*)&f, (void*)f);
	flotte_test_afficher(f);

	/* destruction #2 */
	flotte_detruire(&f);
	printf("\nResultat appel flotte_detruire(%p) : f == %p\n", (void*)&f, (void*)f);
	flotte_test_afficher(f);

	return EXIT_SUCCESS;
}
