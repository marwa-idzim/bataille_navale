/* bateau_type : programme test
*/

#include "bateau_type.h"

#include <stdio.h>					/* pour affichages					*/
#include <stdlib.h>					/* pour EXIT_SUCCESS					*/

/* ____________________ liste des fonctions */

/* Fonction d'affichage faisant appel aux accesseurs */
void bateau_type_test_afficher(bateau_type_cle cle);

/* Fonction principale */
int main(void);

/* ____________________ definition des fonctions */

/* Fonction d'affichage faisant appel aux accesseurs */
void bateau_type_test_afficher(bateau_type_cle cle) {
	printf("Accesseurs bateau_type pour la cle %d :\n", cle);
	printf("\tbateau_type_get_nom(%d) == %s\n", cle, bateau_type_get_nom(cle));
	printf("\tbateau_type_get_alias(%d) == %s\n", cle, bateau_type_get_alias(cle));
	printf("\tbateau_type_get_longueur(%d) == %d\n\n", cle, bateau_type_get_longueur(cle));
}

/* Fonction principale */
int main(void) {
	bateau_type_cle cle;

	printf("\n________ TEST1 : cle correcte\n");

	for (cle =BATEAU_TYPE_CLE_NONE +1; cle <BATEAU_TYPE_CLE_NB; cle ++)
		bateau_type_test_afficher(cle);

	getchar();

	printf("________ TEST2 : cle incorrecte\n");

	cle =BATEAU_TYPE_CLE_NONE;
	bateau_type_test_afficher(cle);

	cle =BATEAU_TYPE_CLE_NB;
	bateau_type_test_afficher(cle);

	return EXIT_SUCCESS;
}
