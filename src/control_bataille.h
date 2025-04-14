#include <gtk/gtk.h>
#include "jeu.h"
#include "vue.h"
#include <stdlib.h>				/* pour EXIT_SUCCESS						*/
#include <stdio.h>				/* pour affichages						*/
#include <string.h>				/* pour manipulation chaines (buffer)	*/
#include <time.h>				/* pour placement aleatoire				*/
 
#include <stdlib.h>				/* pour EXIT_SUCCESS						*/
#include <stdio.h>				/* pour affichages						*/

#include <signal.h>	
#ifndef CONTROL_BATAILLE_H

#define CONTROL_BATAILLE_H



typedef struct control_jeu control_jeu;


typedef struct control_case{
  int ligne;
  int colonne;
  control_jeu* parent;
}control_case;

typedef struct control_bateau_bouton{
  int ind;
  control_jeu* parent;
}control_bateau_bouton;



typedef struct control_jeu{
  vue_jeu* vue;
  jeu * modele;
  control_case ma_case[10][10];
  control_case sa_case[10][10];
  control_bateau_bouton bateau[5];
}control_jeu;

control_jeu* control_initialiser();

//pour que le joueur tape son pseudo
void control_choisir_pseudo(control_jeu* control);

//fonctions pour la connexion
void control_connexion(control_jeu* control);
void control_connecter(control_jeu* control) ;
void control_connexion_serveur(control_jeu* control);
void control_connexion_serveur_suite(control_jeu* control);
void control_connexion_client(control_jeu* control);
void control_connexion_transmission_nom(control_jeu* control);
void control_connexion_transmission_ip(control_jeu* control);
void control_connexion_transmission_nom_maj(control_jeu* control);
void control_connexion_transmission_ip_maj(control_jeu* control);

//lancement du jeu
void control_lancer_le_jeu(control_jeu* control);

//etape du placements de bateaux
void control_placer_bateau(control_jeu* control);
void control_bouton_valider_presse(control_jeu* control);
int control_bateau_orien_case_choisis(control_jeu* control);
void control_bouton_ma_case_presse(control_case* ma_case);
void control_bouton_verticale_presse(control_jeu* control);
void control_bouton_horizontale_presse(control_jeu* control);
void control_bouton_bateau_presse(control_bateau_bouton* bateau);
int control_get_ligne_case_selec(control_jeu* control);
int control_get_colonne_case_selec(control_jeu* control);
int control_get_ind_bateau_selec(control_jeu* control);
int control_get_orientation_selec(control_jeu* control);
void control_vue_placer_bateau(control_jeu* control);

//etape attaque
void control_partie_en_cours(control_jeu* control);
void control_tour_adversaire(control_jeu* control);
void control_mon_tour(control_case* sa_case);

//maj la vue apres chaque attaque en fonction des donnees du modele
void control_maj_ma_grille(control_jeu* control);
void control_maj_sa_grille(control_jeu* control);

void control_desactiver_sa_grille(control_jeu* control);
void control_activer_sa_grille(control_jeu* control);

//permettre au joueur de quitter ou rejouer apres la fin de la partie
void control_partie_perdue(control_jeu* control);
void control_partie_gagnee(control_jeu* control);

//liberer la memoire
void control_detruire(control_jeu* control);


#endif

