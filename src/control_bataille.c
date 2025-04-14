#include "jeu.h"
#include <gtk/gtk.h>

# include <stdlib.h>
# include <stdio.h>
#include <time.h>


#include "control_bataille.h"

control_jeu* control_initialiser(){
  //initialiser la vue
    vue_jeu* vue=vue_construire(); 
    
  //initialiser le modele
    jeu* modele=jeu_construire();
    printf("\n model: %p\n", (void*)modele);

  //initialiser controleur
    control_jeu* control=(control_jeu*)malloc(sizeof(control_jeu));
    if(control==NULL){
        printf("Échec d'allocation de mémoire\n");
        return NULL;
    }
    control->vue=vue;
    control->modele=modele;
    for(int i=0; i<10; i++){
      for(int k=0; k<10; k++){
        control->ma_case[i][k].ligne=i;
        control->ma_case[i][k].colonne=k;
        control->ma_case[i][k].parent=control;
      }
    }
    for(int i=0; i<10; i++){
      for(int k=0; k<10; k++){
        control->sa_case[i][k].ligne=i;
        control->sa_case[i][k].colonne=k;
        control->sa_case[i][k].parent=control;
      }
    }
    for(int i=0; i<5; i++){
      control->bateau[i].ind=i;
      control->bateau[i].parent=control;
    }
    return control;
}





void control_choisir_pseudo(control_jeu* control){
    vue_afficher_pseudo(control->vue);
    g_signal_connect_swapped(G_OBJECT(get_bouton_pseudo_valider(control->vue)),"clicked", G_CALLBACK(control_connexion),control);   
}

void control_connexion(control_jeu* control){
      char pseudo[JOUEUR_PSEUDO_TAILLE +1];
      strncpy(pseudo, gtk_entry_get_text(get_entry_pseudo(control->vue)), JOUEUR_PSEUDO_TAILLE);
      pseudo[JOUEUR_PSEUDO_TAILLE] = '\0';
      jeu_set_pseudo(control->modele, pseudo);
      control_connecter(control);
}


void control_connecter(control_jeu* control) {
    vue_afficher_connexion(control->vue);
    g_signal_connect_swapped(G_OBJECT(get_bouton_connexion_client(control->vue)),"clicked", G_CALLBACK(control_connexion_client),control);
    g_signal_connect_swapped(G_OBJECT(get_bouton_connexion_serveur(control->vue)),"clicked", G_CALLBACK(control_connexion_serveur),control);
}

void control_connexion_serveur(control_jeu* control){
    jeu_creer_partie(control->modele);
     printf("Vous pouvez transmettre le nom '%s' ou l'ip '%s' a votre adversaire pour pouvoir jouer.\n", jeu_get_nom_hote(control->modele), jeu_get_ip(control->modele));
    
    if (jeu_get_etat_partie(control->modele) == JEU_ETAT_ATTENTE_JOUEUR) {
          char texte[1500]; 
          sprintf(texte,"Vous pouvez transmettre le nom '%s' ou l'ip '%s' a votre adversaire pour pouvoir jouer.", jeu_get_nom_hote(control->modele), jeu_get_ip(control->modele));
          vue_connexion_maj_label(control->vue, texte);
          g_signal_connect_swapped(G_OBJECT(get_bouton_connexion_nom(control->vue)),"clicked", G_CALLBACK(control_connexion_serveur_suite),control);
          g_signal_emit_by_name(G_OBJECT(get_bouton_connexion_nom(control->vue)), "clicked");
    }
}

void control_connexion_serveur_suite(control_jeu* control){
    jeu_etre_rejoint_partie(control->modele);
    if(jeu_est_partage(control->modele))
        control_lancer_le_jeu(control);

}


void control_connexion_client(control_jeu* control){
  vue_connexion_maj_client(control->vue);
  g_signal_connect_swapped(G_OBJECT(get_bouton_connexion_nom(control->vue)),"clicked", G_CALLBACK(control_connexion_transmission_nom),control);
  g_signal_connect_swapped(G_OBJECT(get_bouton_connexion_ip(control->vue)),"clicked", G_CALLBACK(control_connexion_transmission_ip),control);
}


void control_connexion_transmission_nom(control_jeu* control){
  vue_connexion_maj_label(control->vue, "Saisir nom hote de votre adversaire et valider");
  vue_connexion_afficher_entry(control->vue);
  gtk_entry_set_text(GTK_ENTRY(get_entry_connexion(control->vue)), "tapez le nom hote");
  g_signal_connect_swapped(G_OBJECT(get_bouton_connexion_valider(control->vue)),"clicked", G_CALLBACK(control_connexion_transmission_nom_maj),control);   
}

void control_connexion_transmission_ip(control_jeu* control){
  vue_connexion_maj_label(control->vue, "Saisir adresse ip de votre adversaire et valider");
  vue_connexion_afficher_entry(control->vue);
  gtk_entry_set_text(GTK_ENTRY(get_entry_connexion(control->vue)), "tapez l'adresse ip");
  g_signal_connect_swapped(G_OBJECT(get_bouton_connexion_valider(control->vue)),"clicked", G_CALLBACK(control_connexion_transmission_ip_maj),control);   
}

void control_connexion_transmission_nom_maj(control_jeu* control){
          char  info_hote_adversaire[(NOM_HOTE_TAILLE_MAX >= IP_TAILLE_MAX ? NOM_HOTE_TAILLE_MAX : IP_TAILLE_MAX) +1] = "";
          strncpy(info_hote_adversaire, gtk_entry_get_text(get_entry_connexion(control->vue)), (NOM_HOTE_TAILLE_MAX >= IP_TAILLE_MAX ? NOM_HOTE_TAILLE_MAX : IP_TAILLE_MAX));
          info_hote_adversaire[(NOM_HOTE_TAILLE_MAX >= IP_TAILLE_MAX ? NOM_HOTE_TAILLE_MAX : IP_TAILLE_MAX)] = '\0';
          jeu_rejoindre_partie_par_nom_hote(control->modele, info_hote_adversaire);
          vue_connexion_maj_label(control->vue, "  echec de connexion, reessayez  ");
          gtk_entry_set_text(GTK_ENTRY(get_entry_connexion(control->vue)), "tapez le nom hote");
          if(jeu_est_partage(control->modele))
              control_lancer_le_jeu(control);
}

void control_connexion_transmission_ip_maj(control_jeu* control){
          char  info_hote_adversaire[(NOM_HOTE_TAILLE_MAX >= IP_TAILLE_MAX ? NOM_HOTE_TAILLE_MAX : IP_TAILLE_MAX) +1] = "";
          strncpy(info_hote_adversaire, gtk_entry_get_text(get_entry_connexion(control->vue)), (NOM_HOTE_TAILLE_MAX >= IP_TAILLE_MAX ? NOM_HOTE_TAILLE_MAX : IP_TAILLE_MAX));
          info_hote_adversaire[(NOM_HOTE_TAILLE_MAX >= IP_TAILLE_MAX ? NOM_HOTE_TAILLE_MAX : IP_TAILLE_MAX)] = '\0';
          jeu_rejoindre_partie_par_ip(control->modele, info_hote_adversaire);
          vue_connexion_maj_label(control->vue, "  echec de connexion, reessayez  ");
          gtk_entry_set_text(GTK_ENTRY(get_entry_connexion(control->vue)), "tapez l'adresse ip");
          if(jeu_est_partage(control->modele))
              control_lancer_le_jeu(control);
}


void control_lancer_le_jeu(control_jeu* control){
    if(jeu_est_partage(control->modele)){
          jeu_nouvelle_partie(control->modele);	
          vue_reinitialiser_partie(control->vue);
          vue_maj_pseudo_joueur_adverse(control->vue, jeu_get_pseudo_adversaire(control->modele));
          vue_maj_nbr_parties_jouees(control->vue, jeu_get_num_partie(control->modele));
          vue_maj_nbr_parties_gagnee(control->vue, control->modele->nb_parties_gagnees);
	        if(jeu_get_etat_partie(control->modele) == JEU_ETAT_PLACEMENT){
              printf("placement\n");
          }
	        vue_afficher_partie(control->vue);
          control_placer_bateau(control);
    }
}
void control_placer_bateau(control_jeu* control){
  //choix de case
    for(int i=0; i<10; i++){
      for(int k=0; k<10; k++){
        g_signal_connect_swapped(G_OBJECT(get_bouton_partie_ma_case(control->vue,i,k)),"pressed", G_CALLBACK(control_bouton_ma_case_presse),&(control->ma_case[i][k]));
      }
    }
  //choix de bateau a placer
    for(int i=0; i<5; i++){
        g_signal_connect_swapped(G_OBJECT(get_bouton_partie_bateau(control->vue,i)),"pressed", G_CALLBACK(control_bouton_bateau_presse),&(control->bateau[i]));
    }
      
  //choix de l'orientation du placement
        g_signal_connect_swapped(G_OBJECT(get_bouton_partie_horizontale(control->vue)),"pressed", G_CALLBACK(control_bouton_horizontale_presse),control);
        g_signal_connect_swapped(G_OBJECT(get_bouton_partie_verticale(control->vue)),"pressed", G_CALLBACK(control_bouton_verticale_presse),control);
  
  //bouton valider presse
        g_signal_connect_swapped(G_OBJECT(get_bouton_partie_valider(control->vue)),"clicked", G_CALLBACK(control_bouton_valider_presse),control);
}


void control_bouton_valider_presse(control_jeu* control){
  //verifier que:
        /*une des cases est choisie
        un des bateaux est choisi
        une orientation est choisie
  et que le placement est possible */
  //si oui alors faire le placement et ne rien faire sinon 
  if(control_bateau_orien_case_choisis(control) 
  && (jeu_get_placement_valide(control->modele, control_get_ind_bateau_selec(control), control_get_ligne_case_selec(control), control_get_colonne_case_selec(control))==control_get_orientation_selec(control)
  || jeu_get_placement_valide(control->modele, control_get_ind_bateau_selec(control), control_get_ligne_case_selec(control), control_get_colonne_case_selec(control))==PLACEMENT_TYPE_NB))
  {
      jeu_placer_bateau(control->modele, control_get_ind_bateau_selec(control), 0, control_get_ligne_case_selec(control),control_get_colonne_case_selec(control), control_get_orientation_selec(control));
      control_vue_placer_bateau(control);
      printf("nombre de bateaux %d\n",joueur_get_flotte_nb_bateaux(control->modele->j));
      printf("etat partie %s\n", jeu_get_etat_string(control->modele));
      if(jeu_get_etat_partie(control->modele)==JEU_ETAT_TOUR_JOUEUR || jeu_get_etat_partie(control->modele)==JEU_ETAT_TOUR_ADVERSAIRE){
          vue_etape_placement_finie(control->vue);
          if(jeu_get_etat_partie(control->modele)==JEU_ETAT_TOUR_JOUEUR ){
              control_activer_sa_grille(control);
              vue_maj_tour(control->vue, "c'est a toi de jouer: clique sur une case");
          }
          if(jeu_get_etat_partie(control->modele)==JEU_ETAT_TOUR_ADVERSAIRE ){
              control_desactiver_sa_grille(control);  
              vue_maj_tour(control->vue, "attente du coup de l'adversaire: patientez");
          }
          while(gtk_events_pending()){
              gtk_main_iteration();
          }
          control_partie_en_cours(control);
      }
  }  
}


int control_bateau_orien_case_choisis(control_jeu* control){
  int acc=0;
  
  //verifier si ya bien une seule case pressee 
  for(int i=0; i<10; i++){
    for(int k=0; k<10; k++){
      if(vue_is_pressed_ma_case(control->vue, i, k))
      {
        ++acc;
      }
    }
  }
  
  if(acc==1){
    acc=0;
    //verifier si ya bien un seul bateau choisi
    for(int i=0; i<5; i++){
        if(vue_is_pressed_bateau(control->vue, i))
        {
          ++acc;
        }
    }
  }
  else
    return 0;
    
  if(acc==1){
    //verifier si ya bien une seule orientation choisie 
    if(vue_is_pressed_verticale(control->vue) && !vue_is_pressed_horizontale(control->vue))
      return  1;
    else if (!vue_is_pressed_verticale(control->vue) && vue_is_pressed_horizontale(control->vue))
        return  1; 
    else 
      return 0;
  }
  else
    return 0;
}

void control_bouton_ma_case_presse(control_case* ma_case){
  //depresser tous les autres boutons 
  for(int i=0; i<10; i++){
    for(int k=0; k<10; k++){
      if(i!=ma_case->ligne || k!=ma_case->colonne)
        vue_maj_pressing_bouton_ma_grille(ma_case->parent->vue, i, k, FALSE);
    }
  }
}

void control_bouton_verticale_presse(control_jeu* control){
  //depresser le bouton horizontale
  vue_maj_pressing_bouton_horizontale(control->vue, FALSE);
}
 
void control_bouton_horizontale_presse(control_jeu* control){
  //depresser le bouton verticale
  vue_maj_pressing_bouton_verticale(control->vue, FALSE);
}

void control_bouton_bateau_presse(control_bateau_bouton* bateau){
  //depresser tous les autres boutons 
  for(int i=0; i<5; i++){
    if(i!=bateau->ind)
      vue_maj_pressing_bouton_bateau(bateau->parent->vue, i, FALSE);
  }
}

int control_get_ligne_case_selec(control_jeu* control){
  for(int i=0; i<10; i++){
    for(int k=0; k<10; k++){
      if(vue_is_pressed_ma_case(control->vue, i, k))
      {
        return i;
      }
    }
  }
  return -1;
}

int control_get_colonne_case_selec(control_jeu* control){
  for(int i=0; i<10; i++){
    for(int k=0; k<10; k++){
      if(vue_is_pressed_ma_case(control->vue, i, k))
      {
        return k;
      }
    }
  }
  return -1;
}

int control_get_ind_bateau_selec(control_jeu* control){
  for(int i=0; i<5; i++){
        if(vue_is_pressed_bateau(control->vue, i))
        {
          return i;
        }
  }
  return -1;
}

int control_get_orientation_selec(control_jeu* control){
  if(vue_is_pressed_verticale(control->vue) && !vue_is_pressed_horizontale(control->vue))
    return PLACEMENT_TYPE_V;
  else if(!vue_is_pressed_verticale(control->vue) && vue_is_pressed_horizontale(control->vue))
    return PLACEMENT_TYPE_H;
  else
    return PLACEMENT_TYPE_NONE;
}

void control_vue_placer_bateau(control_jeu* control){
  int xD=-1;
  int yD=-1;
  int xF=-1;
  int yF=-1;
  int ind_bateau=control_get_ind_bateau_selec(control);
  joueur_get_bateau_position(control->modele->j,ind_bateau , 0, &xD, &yD, &xF, &yF);
  for(int i=xD; i<xF+1; i++){
    for(int k=yD; k<yF+1; k++){
       vue_maj_image_ma_grille( control->vue, i, k, "image/mon_bateau.jpg");
       vue_maj_pressing_bouton_ma_grille(control->vue, i, k, FALSE);
       vue_maj_sensitive_bouton_ma_grille(control->vue, i, k, FALSE);
    }
  }
  vue_maj_pressing_bouton_bateau(control->vue, ind_bateau, FALSE);
  vue_maj_sensitive_bouton_bateau(control->vue,  ind_bateau, FALSE);
}


static gboolean control_event_release(GtkWidget* widget, GdkEventButton* event, gpointer user_data) {
  control_jeu* control = (control_jeu*)user_data;
  while(gtk_events_pending()){
    gtk_main_iteration();
  }
  control_tour_adversaire(control);
  return FALSE; // on laisse GTK continuer l’évènement normalement
}

static gboolean control_event_press(GtkWidget* widget, GdkEventButton* event, gpointer user_data) {
  control_case* sa_case = (control_case*)user_data;
  control_mon_tour(sa_case);
  return FALSE; // on laisse GTK continuer l’évènement normalement
}


void control_partie_en_cours(control_jeu* control){
  for(int i=0; i<10; i++){
    for(int k=0; k<10; k++){
      g_signal_connect(G_OBJECT(get_bouton_partie_sa_case(control->vue, i, k)), "button-press-event", G_CALLBACK(control_event_press), &(control->sa_case[i][k]));
      g_signal_connect(G_OBJECT(get_bouton_partie_sa_case(control->vue, i, k)), "button-release-event", G_CALLBACK(control_event_release), control);
   
    }
  }
  if(jeu_partie_est_en_cours(control->modele) && jeu_get_etat_partie(control->modele)==JEU_ETAT_TOUR_ADVERSAIRE){
      //recup coup adverse et maj la grille adversaire
       control_tour_adversaire(control);
  }
}

void control_tour_adversaire(control_jeu* control){
  //recup coup adverse et maj la grille adversaire
  if(jeu_partie_est_en_cours(control->modele) && jeu_get_etat_partie(control->modele)==JEU_ETAT_TOUR_ADVERSAIRE){
      printf("\nappel tour adversaire\n");
      control_desactiver_sa_grille(control);   
      jeu_tour_adversaire(control->modele);
      control_maj_ma_grille(control);
      control_activer_sa_grille(control);
      vue_maj_tour(control->vue, "c'est a toi de jouer: clique sur une case");
      printf("\nappel tour adversaire fin\n");
    
    if(jeu_get_etat_partie(control->modele)==JEU_ETAT_GAGNEE_JOUEUR){
        control_partie_gagnee(control);
    }
    if(jeu_get_etat_partie(control->modele)==JEU_ETAT_GAGNEE_ADVERSAIRE){
        control_partie_perdue(control);
    }
  }	
}

void control_mon_tour(control_case* sa_case){
  if(jeu_partie_est_en_cours(sa_case->parent->modele) && jeu_get_etat_partie(sa_case->parent->modele)==JEU_ETAT_TOUR_JOUEUR){
      if( jeu_est_coup_valide(sa_case->parent->modele,sa_case->ligne, sa_case->colonne)){
          printf("\nappel tour joueur\n");
          jeu_tour_joueur(sa_case->parent->modele, sa_case->ligne, sa_case->colonne);
          control_maj_sa_grille(sa_case->parent);
          vue_maj_tour(sa_case->parent->vue, "attente du coup de l'adversaire: patientez");
          printf("\nappel tour joueur fin\n");   

          if(jeu_get_etat_partie(sa_case->parent->modele)==JEU_ETAT_GAGNEE_JOUEUR){
            control_partie_gagnee(sa_case->parent);
          }
          if(jeu_get_etat_partie(sa_case->parent->modele)==JEU_ETAT_GAGNEE_ADVERSAIRE){
            control_partie_perdue(sa_case->parent);
          }
      }
  }
}

void control_partie_perdue(control_jeu* control){
    vue_rejouer_maj_resultat(control->vue, "Ta flotte est coulée, T'AS PERDUUUUUUUU !!");
    vue_maj_image_rejouer( control->vue, "image/you_loose.png");
    vue_afficher_rejouer(control->vue);
}

void control_partie_gagnee(control_jeu* control){
    vue_rejouer_maj_resultat(control->vue, "La flotte adversaire est coulée, T'AS GAGNEEEEEEEEE !!");
    vue_maj_image_rejouer( control->vue, "image/you_win.png");
    vue_afficher_rejouer(control->vue);
}

void control_desactiver_sa_grille(control_jeu* control){
    for(int i=0; i<10; i++){
      for(int k=0; k<10; k++){
        vue_maj_sensitive_bouton_sa_grille(control->vue, i, k, FALSE);
      }
    }
}

void control_activer_sa_grille(control_jeu* control){
  for(int i=0; i<10; i++){
    for(int k=0; k<10; k++){
      if(jeu_est_coup_valide(control->modele, i, k))
        vue_maj_sensitive_bouton_sa_grille(control->vue, i, k, TRUE);
    }
  }
}

//maj a jour limage du bouton en fonction de son etat
void control_maj_ma_grille(control_jeu* control){
    case_grille_etat etat=CASE_GRILLE_ETAT_NONE;
    int ind_bateau=BATEAU_TYPE_CLE_NONE;
    for(int i=0; i<10; i++){
      for(int k=0; k<10; k++){
        jeu_get_case_joueur(control->modele, &ind_bateau, &etat, i, k);
        if(etat==CASE_GRILLE_ETAT_JOUEE_TOUCHE || etat==CASE_GRILLE_ETAT_JOUEE_COULE){
          vue_maj_image_ma_grille(control->vue, i, k, "image/mon_bateau_touché.jpg");
        }
      }
    }
}

void control_maj_sa_grille(control_jeu* control){
    case_grille_etat etat=CASE_GRILLE_ETAT_NONE;
    int ind_bateau=BATEAU_TYPE_CLE_NONE;
    for(int i=0; i<10; i++){
      for(int k=0; k<10; k++){
        jeu_get_case_adversaire(control->modele, &ind_bateau, &etat, i, k);
        if(etat==CASE_GRILLE_ETAT_JOUEE_EAU){
          vue_maj_image_sa_grille(control->vue, i, k, "image/flop.jpg");
        }
        if(etat==CASE_GRILLE_ETAT_JOUEE_TOUCHE){
          vue_maj_image_sa_grille(control->vue, i, k, "image/touché.jpg");
        }
        if(etat==CASE_GRILLE_ETAT_JOUEE_COULE){
          vue_maj_image_sa_grille(control->vue, i, k, "image/écoulé.jpg");
        }
      }
    }
}


void control_detruire(control_jeu* control){
	jeu_detruire(&(control->modele));
  vue_detruire(control->vue);
  free(control);
}

