/* placement : programme test
*/

#include "placement.h"

#include <stdlib.h>				/* pour EXIT_SUCCESS		*/
#include <stdio.h>				/* pour affichages		*/

/* ____________________ liste des fonctions */

/* Fonction d'affichage faisant appel aux accesseurs */
void placement_test_afficher(placement* p);

/* Fonction principale */
int main(void);

/* ____________________ deinition des fonctions */

/* Fonction d'affichage faisant appel aux accesseurs */
void placement_test_afficher(placement* p) {
	int xF, yF;
	unsigned int longueur = 2;

	printf("Affichage placement %p :\n\t", (void*)p);
	if (p == NULL)
		printf("le placement n'est pas instancie\n");
	else {
		printf("placement_get_x(%p) == %d\t", (void*)p, placement_get_x(p));
		printf("placement_get_y(%p) == %d\t", (void*)p, placement_get_y(p));
		printf("placement_get_position(%p) == %d\n", (void*)p, placement_get_orientation(p));

		placement_get_fin(p,longueur,&xF,&yF);
		printf("\tplacement_get_fin applique a %p pour une longueur %u : (xF,yF) == (%d,%d)\n", (void*)p, longueur, xF, yF);
	}
}

/* Fonction principale */
int main(void) {
	placement* p = NULL;
	placement_type tp;
	int x, y;

	/* ____ construction & get */
	printf("\n________ TEST1 : construction ::\n");

	p = placement_construire();
	printf("appel %p=placement_construire() :\n", (void*)p);
	placement_test_afficher(p);

	getchar();

	/* ____ affectation */

	printf("________ TEST2 : affectation (dont initialisation) ::\n");

	/* affectation #1 : placement horizontal */
	x=1;
	y=2;
	tp = PLACEMENT_TYPE_H;

	placement_set(p,x,y,tp);
	printf("\nPlacement horizontal : appel placement_set(%p,%u,%u,%d) :\n", (void*)p, x, y, tp);
	placement_test_afficher(p);

	/* affectation #2 : placement vertical */
	x++;
	y++;
	tp = PLACEMENT_TYPE_V;

	placement_set(p,x,y,tp);
	printf("\nPlacement vertical : appel placement_set(%p,%u,%u,%d) :\n", (void*)p, x, y, tp);
	placement_test_afficher(p);

	/* initialisation */

	placement_initialiser(p);
	printf("\nInitialisation placement : appel placement_initialiser(%p) :\n", (void*)p);
	placement_test_afficher(p);

	/* ____ destruction */
	printf("\n________ TEST3 : destruction ::\n");

	/* destruction #1 */
	placement_detruire(& p);
	printf("appel placement_detruire(%p) : p == %p :\n", (void*)&p, (void*)p);
	placement_test_afficher(p);

	/* destruction #2 */
	placement_detruire(& p);
	printf("appel placement_detruire(%p) : p == %p :\n", (void*)&p, (void*)p);
	placement_test_afficher(p);

	return EXIT_SUCCESS;
}
