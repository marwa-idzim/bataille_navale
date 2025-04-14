#include "jeu.h"
#include <gtk/gtk.h>

# include <stdlib.h>
# include <stdio.h>
#include <time.h>
#include <string.h>	

#include "vue.h"

//construction et initialisation des fenetre
vue_jeu* vue_construire(){
  vue_jeu* j=(vue_jeu*) malloc(sizeof(vue_jeu));
        if (j == NULL) {
          fprintf(stderr, "echec de l'allocation memoire pour vue_jeu\n");
          exit(1);  // Exit avec code d'erreur
         }
        else{
            gtk_init(NULL, NULL);
            j->partie=(vue_partie*) malloc(sizeof(vue_partie));
            j->connexion=(vue_connexion*) malloc(sizeof(vue_connexion));
            j->pseudo=(vue_pseudo*) malloc(sizeof(vue_pseudo));
            j->rejouer=(vue_rejouer*) malloc(sizeof(vue_rejouer));
            
            if(j->partie!=NULL && j->connexion!=NULL && j->pseudo!=NULL && j->rejouer!=NULL){
                vue_initialiser_partie(j);
                vue_initialiser_connexion(j);
                vue_initialiser_pseudo(j);
                vue_initialiser_rejouer(j);
                return j;    
            }
            else{
                fprintf(stderr, "echec de l'allocation memoire\n");
                exit(1);  // Exit avec code d'erreur
            }
              
        }
}
void vue_initialiser_connexion(vue_jeu* j){

      //initialiser la fenetre
        j->connexion->f= (GtkWindow*) gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(j->connexion->f, "bataille navale");
        
      //initialiser le label 
        j->connexion->instru=(GtkLabel*) gtk_label_new("vous voulez lancer le client ou lancer le serveur? ");
      
      //initialiser les cinq boutons 
        j->connexion->client=(GtkButton*) gtk_button_new_with_label("lancer client");
        j->connexion->serveur=(GtkButton*) gtk_button_new_with_label("lancer serveur");
        j->connexion->nom=(GtkButton*) gtk_button_new_with_label("saisir nom");
        j->connexion->ip=(GtkButton*) gtk_button_new_with_label("saisir ip");
        j->connexion->valider=(GtkButton*) gtk_button_new_with_label("valider");
        
      //initialiser le gtk entry
        j->connexion->entree= (GtkEntry*) gtk_entry_new();
        
      //les placer dans une boite 
        j->connexion->bh[0]=(GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL,5);
        j->connexion->bh[1]=(GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL,5);
        j->connexion->bh[2]=(GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL,5);
        
        gtk_box_pack_start(GTK_BOX(j->connexion->bh[0]), GTK_WIDGET(j->connexion->client),TRUE, FALSE, 5);
        gtk_box_pack_start(GTK_BOX(j->connexion->bh[0]), GTK_WIDGET(j->connexion->serveur),TRUE, FALSE, 5);
        gtk_box_pack_start(GTK_BOX(j->connexion->bh[1]), GTK_WIDGET(j->connexion->nom),TRUE, FALSE, 5);
        gtk_box_pack_start(GTK_BOX(j->connexion->bh[1]), GTK_WIDGET(j->connexion->ip),TRUE, FALSE, 5);
        gtk_box_pack_start(GTK_BOX(j->connexion->bh[2]), GTK_WIDGET(j->connexion->entree),TRUE, FALSE, 5);
        gtk_box_pack_start(GTK_BOX(j->connexion->bh[2]), GTK_WIDGET(j->connexion->valider),TRUE, FALSE, 5);
        
        j->connexion->bv=(GtkBox*) gtk_box_new(GTK_ORIENTATION_VERTICAL,5);
        gtk_box_pack_start(GTK_BOX(j->connexion->bv), GTK_WIDGET(j->connexion->instru),TRUE, FALSE, 5);
        gtk_box_pack_start(GTK_BOX(j->connexion->bv), GTK_WIDGET(j->connexion->bh[0]),TRUE, FALSE, 5);
        
        //placer la boite principale dans la fenetre
        gtk_container_add (GTK_CONTAINER(j->connexion->f),GTK_WIDGET(j->connexion->bv));
        
        
        

        g_signal_connect(GTK_WIDGET(j->connexion->f), "destroy", gtk_main_quit, NULL);
        
        
        
}
void vue_initialiser_pseudo(vue_jeu* j){
      //initialiser la fenetre
        j->pseudo->f= (GtkWindow*) gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(j->pseudo->f, "bataille navale");
        
      //initialiser le label 
        j->pseudo->instru=(GtkLabel*) gtk_label_new("taper un pseudo puis valider ");
      
      //initialiser le bouton 
        j->pseudo->valider=(GtkButton*) gtk_button_new_with_label("valider");
        
      //initialiser le gtk entry
        j->pseudo->entree= (GtkEntry*) gtk_entry_new();
        
      //les organiser les widgets dans les deux boites 
        j->pseudo->bh=(GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL,5);
        gtk_box_pack_start(GTK_BOX(j->pseudo->bh), GTK_WIDGET(j->pseudo->entree),TRUE, FALSE, 5);
        gtk_box_pack_start(GTK_BOX(j->pseudo->bh), GTK_WIDGET(j->pseudo->valider),TRUE, FALSE, 5);
        
        j->pseudo->bv=(GtkBox*) gtk_box_new(GTK_ORIENTATION_VERTICAL,5);
        gtk_box_pack_start(GTK_BOX(j->pseudo->bv), GTK_WIDGET(j->pseudo->instru),TRUE, FALSE, 5);
        gtk_box_pack_start(GTK_BOX(j->pseudo->bv), GTK_WIDGET(j->pseudo->bh),TRUE, FALSE, 5);

        //placer la boite principale dans la fenetre
        gtk_container_add (GTK_CONTAINER(j->pseudo->f),GTK_WIDGET(j->pseudo->bv));
        
        
        

        g_signal_connect(GTK_WIDGET(j->pseudo->f), "destroy", gtk_main_quit, NULL);

}
void vue_initialiser_partie(vue_jeu* j){

      //initialiser la fenetre
        j->partie->f= (GtkWindow*) gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(j->partie->f, "bataille navale");
      
      
      //initialiser les labels 
        j->partie->nbr_partie_jouee=(GtkLabel*) gtk_label_new("Nombre de parties joues: 0");
        j->partie->nbr_partie_gagnee=(GtkLabel*) gtk_label_new("Nombre de parties gagnees: 0");
        j->partie->pseudo_adversaire=(GtkLabel*) gtk_label_new("joueur adversaire: en attente");
        j->partie->tour=(GtkLabel*) gtk_label_new("en attente");
        
        
      //initialiser les images 
        j->partie->def_symbole = (GtkImage *) gtk_image_new_from_file("image/définition_symboles.jpg");
      
      //initialiser les boutons, les images et les deux grilles
        j->partie->ma_grille = (GtkGrid*) gtk_grid_new();
        j->partie->sa_grille = (GtkGrid*) gtk_grid_new(); 
        for(int i=0; i<10;i++){
            for(int k=0;k<10;k++){
                j->partie->ma_case[i][k]=(GtkToggleButton*) gtk_toggle_button_new ();
                GtkImage* imagee = (GtkImage *) gtk_image_new_from_file("image/grille.jpg");
                gtk_button_set_image(GTK_BUTTON(j->partie->ma_case[i][k]), GTK_WIDGET(imagee));

                j->partie->sa_case[i][k]=(GtkButton*) gtk_button_new ();
                GtkImage* image = (GtkImage *) gtk_image_new_from_file("image/grille.jpg");
                gtk_button_set_image(GTK_BUTTON(j->partie->sa_case[i][k]), GTK_WIDGET(image));

                gtk_grid_attach(GTK_GRID(j->partie->sa_grille), GTK_WIDGET(j->partie->sa_case[i][k]), k, i, 1, 1);
                gtk_grid_attach(GTK_GRID(j->partie->ma_grille), GTK_WIDGET(j->partie->ma_case[i][k]), k, i, 1, 1);
             }
        }
        
      

      //initialiser la partie placement de bateaux située en bas de la fenetre
        j->partie->bateau_def_grille=(GtkGrid*) gtk_grid_new();
        for(int i=0; i<5; i++){  
          j->partie->bateau[i]=(GtkToggleButton*) gtk_toggle_button_new ();
          gtk_grid_attach(GTK_GRID(j->partie->bateau_def_grille), GTK_WIDGET(j->partie->bateau[i]), 0, i, 3, 1);
        }
        gtk_button_set_label(GTK_BUTTON(j->partie->bateau[0]), "Porte avion");
        gtk_button_set_label(GTK_BUTTON(j->partie->bateau[1]), "Croiseur");
        gtk_button_set_label(GTK_BUTTON(j->partie->bateau[2]), "Contre-torpilleur");
        gtk_button_set_label(GTK_BUTTON(j->partie->bateau[3]), "Sous-marin");
        gtk_button_set_label(GTK_BUTTON(j->partie->bateau[4]), "Torpilleur");
        
        for(int i=0; i<5; i++){
          for(int k=0; k<5; k++){
            if( (i==1 && k==4) || ( (i==2||i==3) && k>2 ) || (i==4 && k>1)){
              j->partie->taille_bateau[i][k]= (GtkImage *) gtk_image_new_from_file("image/grille.jpg");
            }
            else{
              j->partie->taille_bateau[i][k]= (GtkImage *) gtk_image_new_from_file("image/mon_bateau.jpg");
            }
            gtk_grid_attach(GTK_GRID(j->partie->bateau_def_grille), GTK_WIDGET(j->partie->taille_bateau[i][k]), k+3, i, 1, 1);
          }
        }
        
        j->partie->valider=(GtkButton*) gtk_button_new_with_label ("valider");
        
        
        j->partie->orientation_position_verti=(GtkBox*) gtk_box_new(GTK_ORIENTATION_VERTICAL,5);
        j->partie->horizontale=(GtkToggleButton*) gtk_toggle_button_new_with_label ("placer horizontalement");
        j->partie->verticale=(GtkToggleButton*) gtk_toggle_button_new_with_label ("placer verticalement");
        gtk_box_pack_start(GTK_BOX(j->partie->orientation_position_verti), GTK_WIDGET(j->partie->horizontale), TRUE, TRUE, 5);
        gtk_box_pack_start(GTK_BOX(j->partie->orientation_position_verti), GTK_WIDGET(j->partie->verticale), TRUE, TRUE, 5);

        
      
      
      //initialiser les boites
        for(int i=0; i<3;i++){
            j->partie->bh[i]=(GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL,5);
          }
          
        gtk_box_pack_start(GTK_BOX(j->partie->bh[0]), GTK_WIDGET(j->partie->pseudo_adversaire), TRUE, TRUE, 5);
        gtk_box_pack_start(GTK_BOX(j->partie->bh[0]), GTK_WIDGET(j->partie->nbr_partie_jouee), TRUE, TRUE, 5);
        gtk_box_pack_start(GTK_BOX(j->partie->bh[0]), GTK_WIDGET(j->partie->nbr_partie_gagnee), TRUE, TRUE, 5);
        
        gtk_box_pack_start(GTK_BOX(j->partie->bh[1]), GTK_WIDGET(j->partie->ma_grille),TRUE, FALSE, 5);
        gtk_box_pack_start(GTK_BOX(j->partie->bh[1]), GTK_WIDGET(j->partie->sa_grille), TRUE, FALSE, 5);
        gtk_box_pack_start(GTK_BOX(j->partie->bh[1]), GTK_WIDGET(j->partie->def_symbole), TRUE, FALSE, 5);
        
        gtk_box_pack_start(GTK_BOX(j->partie->bh[2]), GTK_WIDGET(j->partie->bateau_def_grille),TRUE, TRUE, 5);
        gtk_box_pack_start(GTK_BOX(j->partie->bh[2]), GTK_WIDGET(j->partie->orientation_position_verti), TRUE, TRUE, 5);
        gtk_box_pack_start(GTK_BOX(j->partie->bh[2]), GTK_WIDGET(j->partie->valider), TRUE, TRUE, 5);
        
        
        j->partie->bv=(GtkBox*) gtk_box_new(GTK_ORIENTATION_VERTICAL,5);

        gtk_box_pack_start(GTK_BOX(j->partie->bv), GTK_WIDGET(j->partie->bh[0]), TRUE, FALSE, 5);
        gtk_box_pack_start(GTK_BOX(j->partie->bv), GTK_WIDGET(j->partie->tour), TRUE, FALSE, 5);
        gtk_box_pack_start(GTK_BOX(j->partie->bv), GTK_WIDGET(j->partie->bh[1]), TRUE, FALSE, 5);
        gtk_box_pack_start(GTK_BOX(j->partie->bv), GTK_WIDGET(j->partie->bh[2]), TRUE, TRUE, 5);


        gtk_container_add (GTK_CONTAINER(j->partie->f),GTK_WIDGET(j->partie->bv));
        
        
        

        g_signal_connect(GTK_WIDGET(j->partie->f), "destroy", gtk_main_quit, NULL);
        

}
void vue_initialiser_rejouer(vue_jeu* j){
      //initialiser la fenetre
        j->rejouer->f= (GtkWindow*) gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(j->pseudo->f, "bataille navale");
        
      //initialiser le label 
        j->rejouer->resultat=(GtkLabel*) gtk_label_new("");
      
      //initialiser les deux boutons 
        j->rejouer->rejouer=(GtkButton*) gtk_button_new_with_label("replay");
        j->rejouer->quitter= (GtkButton*) gtk_button_new_with_label("exit");
        
      //initialiser les images 
        j->rejouer->image= (GtkImage *) gtk_image_new_from_file("image/définition_symboles.jpg");//
        
      //les placer dans une boite 
        j->rejouer->bh=(GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL,5);
        gtk_box_pack_start(GTK_BOX(j->rejouer->bh), GTK_WIDGET(j->rejouer->rejouer),TRUE, FALSE, 5);
        gtk_box_pack_start(GTK_BOX(j->rejouer->bh), GTK_WIDGET(j->rejouer->quitter),TRUE, FALSE, 5);
        
        j->rejouer->bv=(GtkBox*) gtk_box_new(GTK_ORIENTATION_VERTICAL,5);
        gtk_box_pack_start(GTK_BOX(j->rejouer->bv), GTK_WIDGET(j->rejouer->resultat),TRUE, FALSE, 5);
        gtk_box_pack_start(GTK_BOX(j->rejouer->bv), GTK_WIDGET(j->rejouer->image),TRUE, FALSE, 5);
        gtk_box_pack_start(GTK_BOX(j->rejouer->bv), GTK_WIDGET(j->rejouer->bh),TRUE, FALSE, 5);

        gtk_container_add (GTK_CONTAINER(j->rejouer->f),GTK_WIDGET(j->rejouer->bv));
        
        
        

        g_signal_connect(GTK_WIDGET(j->rejouer->f), "destroy", gtk_main_quit, NULL);
        
}



//affichage des fenetres
void vue_afficher_pseudo(vue_jeu* j){
  gtk_widget_hide(GTK_WIDGET(j->connexion->f));
  gtk_widget_hide(GTK_WIDGET(j->partie->f));
  gtk_widget_hide(GTK_WIDGET(j->rejouer->f));
  gtk_widget_show_all(GTK_WIDGET(j->pseudo->f));
}

void vue_afficher_partie(vue_jeu* j){
  gtk_widget_hide(GTK_WIDGET(j->connexion->f));
  gtk_widget_hide(GTK_WIDGET(j->pseudo->f));
  gtk_widget_hide(GTK_WIDGET(j->rejouer->f));
  gtk_widget_show_all(GTK_WIDGET(j->partie->f));
}

void vue_afficher_connexion(vue_jeu* j){
  gtk_widget_hide(GTK_WIDGET(j->pseudo->f));
  gtk_widget_hide(GTK_WIDGET(j->partie->f));
  gtk_widget_hide(GTK_WIDGET(j->rejouer->f));
  gtk_widget_show_all(GTK_WIDGET(j->connexion->f));
}

void vue_afficher_rejouer(vue_jeu* j){
  gtk_widget_hide(GTK_WIDGET(j->pseudo->f));
  gtk_widget_hide(GTK_WIDGET(j->partie->f));
  gtk_widget_hide(GTK_WIDGET(j->connexion->f));
  gtk_widget_show_all(GTK_WIDGET(j->rejouer->f));
}



//fonctions qui renvoient les widgets de la fenetre de pseudo
GtkButton* get_bouton_pseudo_valider(vue_jeu* j){
  return j->pseudo->valider;
}
GtkEntry* get_entry_pseudo(vue_jeu* j){
  return j->pseudo->entree;
}



//fonctions pour maj la fenetre de connexion
void vue_connexion_maj_label(vue_jeu* j, char* texte){
  gtk_label_set_text(GTK_LABEL(j->connexion->instru), texte);
}
void vue_connexion_maj_client(vue_jeu* j){
  gtk_label_set_text(GTK_LABEL(j->connexion->instru), "  Identification hote joueur distant : vous voulez saisir le nom ou l'ip?  ");
  
  gtk_container_remove(GTK_CONTAINER(j->connexion->bv), GTK_WIDGET(j->connexion->bh[0]));
  gtk_box_pack_start(GTK_BOX(j->connexion->bv), GTK_WIDGET(j->connexion->bh[1]),TRUE, FALSE, 5);

  vue_afficher_connexion(j);
}
void vue_connexion_afficher_entry(vue_jeu* j){
  gtk_container_remove(GTK_CONTAINER(j->connexion->bv), GTK_WIDGET(j->connexion->bh[1]));
  gtk_box_pack_start(GTK_BOX(j->connexion->bv), GTK_WIDGET(j->connexion->bh[2]),TRUE, FALSE, 5);
  vue_afficher_connexion(j);
}


//fonctions qui renvoient les widget de la fenetre de connexion
GtkButton* get_bouton_connexion_client(vue_jeu* j){
  return j->connexion->client;
}
GtkButton* get_bouton_connexion_serveur(vue_jeu* j){
  return j->connexion->serveur;
}
GtkButton* get_bouton_connexion_nom(vue_jeu* j){
  return j->connexion->nom;
}
GtkButton* get_bouton_connexion_ip(vue_jeu* j){
  return j->connexion->ip;
}
GtkEntry* get_entry_connexion(vue_jeu* j){
  return j->connexion->entree;
}
GtkButton* get_bouton_connexion_valider(vue_jeu* j){
  return j->connexion->valider;
}




//maj la fenetre de fin de partie
void vue_rejouer_maj_resultat(vue_jeu* j, char* resultat){
    gtk_label_set_text(GTK_LABEL(j->rejouer->resultat), resultat);
}

void vue_maj_image_rejouer( vue_jeu* j, char* chemin_image){
        gtk_image_set_from_file(j->rejouer->image, chemin_image);
}


//fonctions pour maj la fenetre de la partie
void vue_reinitialiser_partie(vue_jeu* j){
  for(int i=0; i<10;i++){
    for(int k=0;k<10;k++){
        vue_maj_image_ma_grille( j, i, k, "image/grille.jpg");
        vue_maj_image_sa_grille( j, i, k, "image/grille.jpg");
        vue_maj_pressing_bouton_ma_grille(j, i, k, FALSE);
        vue_maj_sensitive_bouton_ma_grille( j, i, k, TRUE);
        vue_maj_sensitive_bouton_sa_grille(j, i, k, TRUE);    
    }
  }
  for(int i=0; i<5;i++){
      vue_maj_pressing_bouton_bateau(j, i, FALSE);
      vue_maj_sensitive_bouton_bateau(j, i, TRUE);      
  }
  vue_maj_sensitive_bouton_valider(j, TRUE);
  vue_maj_pressing_bouton_horizontale( j, FALSE);
  vue_maj_sensitive_bouton_horizontale( j, TRUE);
  vue_maj_pressing_bouton_verticale( j,FALSE);
  vue_maj_sensitive_bouton_verticale( j, TRUE);
  vue_maj_tour(j, "placez vos bateaux");
}

//maj des labels de la fenetre de la partie
void vue_maj_nbr_parties_jouees(vue_jeu* j, int nbr_partie){
      char texte[100];
      sprintf(texte,"Nombre de parties joues:<b>%d </b>",nbr_partie);
      gtk_label_set_markup(GTK_LABEL(j->partie->nbr_partie_jouee), texte);
}
void vue_maj_nbr_parties_gagnee(vue_jeu* j, int nbr_partie){
      char texte[100];
      sprintf(texte,"Nombre de parties gagnee:<b>%d </b>",nbr_partie);
      gtk_label_set_markup(GTK_LABEL(j->partie->nbr_partie_gagnee), texte);
}
void vue_maj_pseudo_joueur_adverse(vue_jeu* j, const char* pseudo ){
      char texte[1000];
      sprintf(texte,"joueur:<b>%s </b>",pseudo);
      gtk_label_set_markup(GTK_LABEL(j->partie->pseudo_adversaire), texte);
}
void vue_maj_tour(vue_jeu* j, char* tour){
      gtk_label_set_markup(GTK_LABEL(j->partie->tour), tour);
}


//maj des bouton des deux grilles
    //maj des images des boutons
void vue_maj_image_ma_grille( vue_jeu* j, int i, int k, char* chemin_image){
          GtkImage* image = (GtkImage*)gtk_button_get_image(GTK_BUTTON(j->partie->ma_case[i][k]));
          gtk_image_set_from_file(image, chemin_image);
}
void vue_maj_image_sa_grille(vue_jeu* j, int i, int k, char* chemin_image){
          GtkImage* image = (GtkImage*)gtk_button_get_image(GTK_BUTTON(j->partie->sa_case[i][k]));
          gtk_image_set_from_file(image, chemin_image);
}
    //maj de l'etat de bouton 
void vue_maj_pressing_bouton_ma_grille(vue_jeu* j, int i, int k, gboolean boolean){
          gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(j->partie->ma_case[i][k]), boolean);
}
void vue_maj_sensitive_bouton_ma_grille(vue_jeu* j, int i, int k, gboolean boolean){
          gtk_widget_set_sensitive(GTK_WIDGET(j->partie->ma_case[i][k]), boolean);
}
void vue_maj_sensitive_bouton_sa_grille(vue_jeu* j, int i, int k, gboolean boolean){
          gtk_widget_set_sensitive(GTK_WIDGET(j->partie->sa_case[i][k]), boolean);
}

//maj des boutons "placer horizontalement et verticalement"
void vue_maj_pressing_bouton_horizontale(vue_jeu* j, gboolean boolean){
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(j->partie->horizontale), boolean);
}
void vue_maj_sensitive_bouton_horizontale(vue_jeu* j, gboolean boolean){
      gtk_widget_set_sensitive(GTK_WIDGET(j->partie->horizontale), boolean);
}


void vue_maj_pressing_bouton_verticale(vue_jeu* j, gboolean boolean){
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(j->partie->verticale), boolean);
}
void vue_maj_sensitive_bouton_verticale(vue_jeu* j, gboolean boolean){
      gtk_widget_set_sensitive(GTK_WIDGET(j->partie->verticale), boolean);
}

//maj du bouton valider
void vue_maj_sensitive_bouton_valider(vue_jeu* j, gboolean boolean){
      gtk_widget_set_sensitive(GTK_WIDGET(j->partie->valider), boolean);
}

//maj des boutons des bateaux 
void vue_maj_pressing_bouton_bateau(vue_jeu* j, int i, gboolean boolean){
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(j->partie->bateau[i]), boolean);
}
void vue_maj_sensitive_bouton_bateau(vue_jeu* j, int i, gboolean boolean){
      gtk_widget_set_sensitive(GTK_WIDGET(j->partie->bateau[i]), boolean);
}



int vue_is_pressed_ma_case(vue_jeu* j, int i, int k){
  return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(j->partie->ma_case[i][k]));
}
int vue_is_pressed_bateau(vue_jeu* j, int i){
  return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(j->partie->bateau[i]));
}
int vue_is_pressed_horizontale(vue_jeu* j){
  return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(j->partie->horizontale));
}
int vue_is_pressed_verticale(vue_jeu* j){
  return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(j->partie->verticale));
}

void vue_etape_placement_finie(vue_jeu* j){
  for(int i=0; i<10; i++){
    for(int k=0; k<10; k++){
      if(gtk_widget_is_sensitive(GTK_WIDGET(j->partie->ma_case[i][k]))){
          vue_maj_pressing_bouton_ma_grille(j, i, k, FALSE);
          vue_maj_sensitive_bouton_ma_grille(j, i, k, FALSE);
      }
    }
  }
 
  vue_maj_pressing_bouton_horizontale( j, FALSE);
  vue_maj_sensitive_bouton_horizontale( j, FALSE);
  vue_maj_pressing_bouton_verticale( j, FALSE);
  vue_maj_sensitive_bouton_verticale( j, FALSE);
  vue_maj_sensitive_bouton_valider(j, FALSE);

}


//fonctions qui renvoient les widgets de la fenetre de la partie
GtkButton* get_bouton_partie_sa_case(vue_jeu* j, int i, int k){
  return j->partie->sa_case[i][k];
}

GtkToggleButton* get_bouton_partie_ma_case(vue_jeu* j, int i, int k){
  return j->partie->ma_case[i][k];
}

GtkToggleButton* get_bouton_partie_bateau(vue_jeu* j, int i){
  return j->partie->bateau[i];
}

GtkToggleButton* get_bouton_partie_horizontale(vue_jeu* j){
  return j->partie->horizontale;
}

GtkToggleButton* get_bouton_partie_verticale(vue_jeu* j){
  return j->partie->verticale;
}

GtkButton* get_bouton_partie_valider(vue_jeu* j){
  return j->partie->valider;
}


//detruire 
void vue_detruire(vue_jeu* j){
  vue_detruire_connexion(j);
  vue_detruire_rejouer(j);
  vue_detruire_partie(j);
  vue_detruire_pseudo(j);
}
void vue_detruire_partie(vue_jeu*j){
  free(j->partie);
}
void vue_detruire_rejouer(vue_jeu*j){
  free(j->rejouer);
}
void vue_detruire_connexion(vue_jeu*j){
  free(j->connexion);
}
void vue_detruire_pseudo(vue_jeu*j){
  free(j->pseudo);
}