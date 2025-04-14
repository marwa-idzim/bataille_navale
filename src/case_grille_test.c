/* case grille : programme test
*/

#include "case_grille.h"
#include "placement.h"			/* pour */

#include <stdlib.h>				/* pour EXIT_SUCCESS		*/
#include <stdio.h>				/* pour affichages		*/

/* ____________________ liste des fonctions */

/* Fonction d'affichage case_grille faisant appel aux accesseurs */
void case_grille_test_afficher(case_grille* c);

/* Fonction principale */
int main(void);

/* ____________________ definition des fonctions */

/* Fonction d'affichage case_grille faisant appel aux accesseurs */
void case_grille_test_afficher(case_grille* c) {
	printf("Affichage case grille %p ::\t", (void*)c);
	if (c == NULL)
		printf("case non instanciee.\n");
	else {
		/* __ champs case grille n'ayant pas d'accesseur */
		printf("\tgrille d'appartenance %p\n", (void*) c->parent);

		/* __ accesseurs case grille */
		printf("\tcase_grille_get_etat(%p) == %d\t(%d:non jouee -- [%d,%d]:jouee)\n", (void*)c, case_grille_get_etat(c), CASE_GRILLE_ETAT_NONE, CASE_GRILLE_ETAT_NONE +1, CASE_GRILLE_ETAT_NB -1);
		printf("\tcase_grille_get_type_bateau(%p) == %d\t(%d:pas de bateau -- [%d,%d]:type de %s a %s)\n", (void*)c, case_grille_get_type_bateau(c), BATEAU_TYPE_CLE_NONE, BATEAU_TYPE_CLE_NONE +1, BATEAU_TYPE_CLE_NB -1, bateau_type_get_nom(BATEAU_TYPE_CLE_NONE +1), bateau_type_get_nom(BATEAU_TYPE_CLE_NB -1));
		printf("\tcase_grille_get_id_bateau_type(%p) == %d\t(pas d'identifiant de bateau : %d)\n", (void*)c, case_grille_get_id_bateau_type(c), CASE_GRILLE_BATEAU_NONE);
	}
}

/* Fonction principale */
int main(void) {
	case_grille* c = NULL;
	void* parent = NULL;
	int etat_case = CASE_GRILLE_ETAT_JOUEE_EAU, cleType = BATEAU_TYPE_CLE_CONTRE_TORPILLEUR;
	unsigned int indBateau = 1;

	/* ____ construction */
	printf("\n________ TEST 1 : construction\n");

	/* construction #1 */
	c = case_grille_construire((struct s_grille *) parent);
	printf("\nAppel (incorrect) %p = case_construire(%p) :\n", (void*)c, (void*)parent);
	case_grille_test_afficher(c);

	/* construction #2 */
	c = case_grille_construire((struct s_grille *) &parent);
	printf("\nAppel (correct) %p = case_construire(%p) :\n", (void*)c, (void*) &parent);
	case_grille_test_afficher(c);

	getchar();

	/* ____ affectation */
	printf("________ TEST 2 : affectation (accesseurs set et initialisation)\n");

	/* affectation #1 : accesseurs set champ par champ */
	case_grille_set_etat(c, etat_case);
	case_grille_set_type_bateau(c, cleType);
	printf("\nAppels case_grille_set_etat(%p, %d), case_grille_set_type_bateau(%p, %d) :\n", (void*)c, etat_case, (void*)c, cleType);
	case_grille_test_afficher(c);

	/* affectation #2 : accesseur set type + id bateau */
	cleType = BATEAU_TYPE_CLE_TORPILLEUR;
	case_grille_set_bateau(c, cleType, indBateau);
	printf("\nAppels case_grille_set_bateau(%p, %d, %u) :\n", (void*)c, cleType, indBateau);
	case_grille_test_afficher(c);

	/* affectation #3 : initialisation */
	case_grille_initialiser(c);
	printf("\nResultat appel case_grille_initialiser(%p) :\n", (void*)c);
	case_grille_test_afficher(c);

	getchar();

	/* ____ destruction */
	printf("________ TEST 3 : destruction\n");

	/* destruction #1 */
	case_grille_detruire(&c);
	printf("\nAppel case_grille_detruire(%p) : c == %p\n", (void*) &c, (void*)c);
	case_grille_test_afficher(c);

	/* destruction #2 */
	case_grille_detruire(&c);
	printf("\nAppel case_grille_detruire(%p) : c == %p\n", (void*) &c, (void*)c);
	case_grille_test_afficher(c);

	return EXIT_SUCCESS;
}
