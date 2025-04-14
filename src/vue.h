#ifndef VUE_H
#define VUE_H

typedef struct vue_pseudo{
  GtkWindow* f;
      GtkBox* bv;
          GtkLabel* instru;
          GtkBox* bh;
              GtkEntry* entree;
              GtkButton* valider;
}vue_pseudo;

typedef struct vue_connexion{
  GtkWindow* f;
      GtkBox* bv;
          GtkLabel* instru;
          GtkBox* bh[3];
           
              GtkButton* client; 
              GtkButton* serveur;
              
              GtkButton* ip;
              GtkButton* nom;

              GtkEntry* entree;  
              GtkButton* valider;
}vue_connexion;

typedef struct vue_partie{
  GtkWindow* f;
      GtkBox* bv; 
          GtkBox * bh[3];
              GtkLabel* nbr_partie_jouee; 
              GtkLabel* nbr_partie_gagnee;
              GtkLabel* pseudo_adversaire;


              GtkGrid* ma_grille; 
                  GtkToggleButton* ma_case[JOUEUR_GRILLE_L_DEFAULT][JOUEUR_GRILLE_H_DEFAULT];

              GtkGrid* sa_grille; 
                  GtkButton* sa_case[JOUEUR_GRILLE_L_DEFAULT][JOUEUR_GRILLE_H_DEFAULT];

              GtkImage* def_symbole;



              GtkGrid* bateau_def_grille;
                  GtkToggleButton* bateau[5];
                  GtkImage* taille_bateau[5][5];

              GtkBox* orientation_position_verti;
                  GtkToggleButton* horizontale;
                  GtkToggleButton* verticale;
                  
              GtkButton* valider;
                

          GtkLabel* tour;

}vue_partie;

typedef struct vue_rejouer{
  GtkWindow* f;
      GtkBox* bv;
          GtkLabel* resultat;
          GtkImage* image;
          GtkBox* bh;
              GtkButton* rejouer;
              GtkButton* quitter; 

}vue_rejouer;

typedef struct vue_jeu {
  vue_partie* partie;
  vue_pseudo* pseudo; 
  vue_connexion* connexion;
  vue_rejouer* rejouer;
}vue_jeu;




//construction et initialisation des fenetre
vue_jeu* vue_construire();
void vue_initialiser_pseudo(vue_jeu* j);
void vue_initialiser_partie(vue_jeu* j);
void vue_initialiser_connexion(vue_jeu* j);
void vue_initialiser_rejouer(vue_jeu* j);

//affichage des fenetres
void vue_afficher_pseudo(vue_jeu* j);
void vue_afficher_partie(vue_jeu* j);
void vue_afficher_connexion(vue_jeu* j);
void vue_afficher_rejouer(vue_jeu* j);

//fonctions qui renvoient les widgets de la fenetre de pseudo
GtkButton* get_bouton_pseudo_valider(vue_jeu* j);
GtkEntry* get_entry_pseudo(vue_jeu* j);

//fonctions pour maj la fenetre de connexion
void vue_connexion_maj_label(vue_jeu* j, char* texte);
void vue_connexion_maj_client(vue_jeu* j);
void vue_connexion_afficher_entry(vue_jeu* j);

//fonctions qui renvoient les widget de la fenetre de connexion
GtkButton* get_bouton_connexion_client(vue_jeu* j);
GtkButton* get_bouton_connexion_serveur(vue_jeu* j);
GtkButton* get_bouton_connexion_nom(vue_jeu* j);
GtkButton* get_bouton_connexion_ip(vue_jeu* j);
GtkEntry* get_entry_connexion(vue_jeu* j);
GtkButton* get_bouton_connexion_valider(vue_jeu* j);




//maj la fenetre de fin de partie
void vue_rejouer_maj_resultat(vue_jeu* j, char* resultat);
void vue_maj_image_rejouer( vue_jeu* j, char* chemin_image);

//fonctions pour maj la fenetre de la partie
void vue_reinitialiser_partie(vue_jeu* j);

void vue_maj_nbr_parties_jouees(vue_jeu* j, int nbr_partie);
void vue_maj_nbr_parties_gagnee(vue_jeu* j, int nbr_partie);
void vue_maj_pseudo_joueur_adverse(vue_jeu* j, const char* pseudo );
void vue_maj_tour(vue_jeu* j, char* tour);


//fonctions utilisées lors des placements des bateaux
void vue_maj_pressing_bouton_ma_grille(vue_jeu* j, int i, int k, gboolean boolean);
void vue_maj_sensitive_bouton_ma_grille(vue_jeu* j, int i, int k, gboolean boolean);
void vue_maj_pressing_bouton_horizontale(vue_jeu* j, gboolean boolean);
void vue_maj_sensitive_bouton_horizontale(vue_jeu* j, gboolean boolean);
void vue_maj_pressing_bouton_verticale(vue_jeu* j, gboolean boolean);
void vue_maj_sensitive_bouton_verticale(vue_jeu* j, gboolean boolean);
void vue_maj_sensitive_bouton_valider(vue_jeu* j, gboolean boolean);
void vue_maj_pressing_bouton_bateau(vue_jeu* j, int i, gboolean boolean);
void vue_maj_sensitive_bouton_bateau(vue_jeu* j, int i, gboolean boolean);

int vue_is_pressed_ma_case(vue_jeu* j, int i, int k);
int vue_is_pressed_bateau(vue_jeu* j, int i);
int vue_is_pressed_horizontale(vue_jeu* j);
int vue_is_pressed_verticale(vue_jeu* j);

void vue_etape_placement_finie(vue_jeu* j);

//fonctions appelees au moment de l'attaque
void vue_maj_sensitive_bouton_sa_grille(vue_jeu* j, int i, int k, gboolean boolean);
void vue_maj_image_ma_grille( vue_jeu* j, int i, int k, char* chemin_image);
void vue_maj_image_sa_grille(vue_jeu* j, int i, int k, char* chemin_image);


//fonctions qui renvoient les widgets de la fenetre de la partie
GtkButton* get_bouton_partie_sa_case(vue_jeu* j, int i, int k);
GtkToggleButton* get_bouton_partie_ma_case(vue_jeu* j, int i, int k);
GtkToggleButton* get_bouton_partie_bateau(vue_jeu* j, int i);
GtkToggleButton* get_bouton_partie_horizontale(vue_jeu* j);
GtkToggleButton* get_bouton_partie_verticale(vue_jeu* j);
GtkButton* get_bouton_partie_valider(vue_jeu* j);

//liberer la memoire
void vue_detruire(vue_jeu* j);
void vue_detruire_partie(vue_jeu*j);
void vue_detruire_rejouer(vue_jeu*j);
void vue_detruire_connexion(vue_jeu*j);
void vue_detruire_pseudo(vue_jeu*j);

  #endif
  
