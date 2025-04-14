#include "jeu.h"

#include <stdlib.h>				/* pour EXIT_SUCCESS						*/
#include <stdio.h>				/* pour affichages						*/
#include <string.h>				/* pour manipulation chaines (buffer)	*/
#include <time.h>				/* pour placement aleatoire				*/
 
#include <stdlib.h>				/* pour EXIT_SUCCESS						*/
#include <stdio.h>				/* pour affichages						*/

#include <signal.h>	
#include "control_bataille.h"


int main(){
  srand(time(NULL));

  control_jeu* control=control_initialiser();
  g_signal_connect_swapped(G_OBJECT(control->vue->rejouer->rejouer), "clicked", G_CALLBACK(control_lancer_le_jeu), control);
  g_signal_connect(G_OBJECT(control->vue->rejouer->quitter), "clicked", gtk_main_quit, control);
  control_choisir_pseudo(control);
 
  gtk_main();
  control_detruire(control);
}
