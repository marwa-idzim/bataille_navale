/* client serveur : programme test

	Utilisation :
		- lancer une instance en faisant le choix client + 1 instance en faisant le choix serveur
		- dans l'instance client, indiquer le nom de l'hote de l'instance serveur

	Fonctions du modele :																testee dans:

		Client :
			void client_initialiser(client* c);											main()
			void client_connexion_par_nom(client* c, const char* nom_hote);				client_connexion()
			void client_connexion_par_ip(client* c, char ip[IP_TAILLE_MAX +1]);			client_connexion()
			void client_delete(client* c);												client_deconnexion(), nettoyer_socket()
			char* client_get_buffer(client* c);											client_afficher(), client_receptions()
			void client_set_buffer(client* c, const char* texte);						client_emissions()
			int client_est_connecte(client* c);											client_afficher(), client_main()
			int client_recevoir(client* c);												client_receptions()
			int client_emettre(client* c);												client_emissions()

		Serveur :
			void serveur_initialiser(serveur* s);										main()
			void serveur_creer(serveur* s);												serveur_connexion()
			void serveur_connecter_client(serveur* s);									serveur_connexion()
			void serveur_deconnecter_client(serveur* s);									serveur_deconnexion()
			void serveur_delete(serveur* s);												serveur_deconnexion(), nettoyer_socket()
			const char* serveur_get_nom_hote(serveur* s);								serveur_afficher(), serveur_connexion()
			const char* serveur_get_ip(serveur* s);										serveur_afficher(), serveur_connexion()
			char* serveur_get_buffer(serveur* s);										serveur_afficher(), serveur_receptions()
			void serveur_set_buffer(serveur* s, const char* texte);						serveur_emissions()
			int serveur_est_connecte(serveur* s);										serveur_afficher(), serveur_connexion(), serveur_deconnexion()
			int serveur_est_client_connecte(serveur* s);									serveur_afficher(), serveur_deconnexion(), serveur_main()
			int serveur_recevoir(serveur* s);											serveur_receptions()
			int serveur_emettre(serveur* s); 											serveur_emissions()

	TODO :
		- Tests fonctions non testees
		- Scenarii alternatives
*/

#include <stdlib.h>		/* pour EXIT_FAILURE, EXIT_SUCCESS			*/
#include <stdio.h>		/* pour affichages							*/
#include <errno.h>		/* pour erreurs systeme						*/
#include <string.h>		/* pour manipulation chaines (dont buffer)	*/

#include "client_serveur.h"

#include <sys/socket.h>	/*	struct sockaddr, fonctions connect, bind, socket	*/
#include <netdb.h>		/*	struct hostent, fonction gethostbyname			*/
#include <unistd.h>		/*	fonctions close, gethostname						*/
#include <arpa/inet.h>	/* 	fonction inet_ntoa 								*/

/* ____________________ liste des fonctions */

/* Affichages */
void client_afficher(client* c);
void serveur_afficher(serveur* s);

/* Sous-routine lecture messages buffer  */
int buffer_lire(char* buf);			/* 	Pre-condition : buf contient une chaine parmi "", "STOP", "nb=x1 ...nb=xp "
															ou x1,...,xp sont des chiffres, "nb=x1 ...nb=xp STOP" ou x1,...,xp sont des chiffres
										Post-condition : buf == "" (tous les messages interpretes)													 	*/

/* Sous-routines scenario de test client  */
void client_connexion(client* c);		/* Pre-condition : client initialise (pas encore de socket creee => c->id == -1)										*/
void client_emissions(client* c);
void client_receptions(client* c);
void client_deconnexion(client* c);

/* Sous-routines scenario de test serveur  */
void serveur_connexion(serveur* s);	/* Pre-condition : serveur initialise (pas encore de socket creee => s->id == s->client.id == -1)					*/
void serveur_emissions(serveur* s);
void serveur_receptions(serveur* s);
void serveur_deconnexion(serveur* s);

/* Scenario test */
void client_main(client* c);			/* Pre-condition : client initialise (pas encore de socket creee => c->id == -1)										*/
void serveur_main(serveur* s);		/* Pre-condition : serveur initialise (pas encore de socket creee => s->id == s->client.id == -1)					*/
int main(void);

/* ____________________ Capture signal interruption (pour supprimer socket(s) en cas d'interruption)

	signal capture  :  	SIGINT, qui est envoye lorsque l'on tappe CTRL+C
	action associee :	fonction nettoyer_socket() qui detruit les sockets eventuellement crees avant de sortir du programme

	Pourquoi le faire : sinon les sockets ne sont pas detruites. Par exemple, on ne peut relancer alors le serveur.
	Contrainte : puisqu'on ne peut passer de parametre aux fonctions de rappel, cela oblige a definir des variables globales dans lesquelles memorise alors les clients et serveurs eventuellement crees
*/

#include <signal.h>				/* signaux & structure sigaction					*/

/* sockets a supprimer eventuellement */
client* g_c = NULL;
serveur* g_s = NULL;

/* fonction faisant appel aux destructeurs client et serveur sur g_c et g_s avant de sortir */
void nettoyer_socket(void) {
	printf("\n%s IN (g_c == %p, g_s == %p)\n", __func__, (void*)g_c, (void*)g_s);

	if (g_c != NULL) {
		printf("\n\tsuppression du client\n");
		client_afficher(g_c);
		client_delete(g_c);
	}

	if (g_s != NULL) {
		printf("\n\tsuppression du serveur\n");
		serveur_afficher(g_s);
		serveur_delete(g_s);
	}

	printf("%s OUT (g_c == %p, g_s == %p)\n", __func__, (void*)g_c, (void*)g_s);

	/* on sort du progamme */
	exit(EXIT_FAILURE);
}

/* ____________________ Definitions des fonctions */

/* ____ Fonctions d'affichage */

/* Affichage client */
void client_afficher(client* c) {
	printf("Client %p:\t", (void*)c);
	if (c == NULL)
		printf("non instancie.\n");
	else if (! client_est_connecte(c))
		printf("non connecte (c->id == %d).\n", c->id);
	else
		printf ("id %d, buffer '%s'.\n", c->id, client_get_buffer(c));
}

/* Affichage serveur */
void serveur_afficher(serveur* s) {
	printf("Serveur %p:\t", (void*)s);
	if (s == NULL)
		printf("non instancie.\n");
	else if (! serveur_est_connecte(s)) {
		printf("sur hote '%s' (nom) -- '%s' (ip) :\t", serveur_get_nom_hote(s), serveur_get_ip(s));
		printf("non connecte (s->id == %d).\n", s->id);
	}
	else {
		printf ("id %d, buffer '%s' :\t", s->id, serveur_get_buffer(s));

		if (! serveur_est_client_connecte(s))
			printf("pas de client non connecte (s->client.id == %d).\n", s->client.id);
		else {
			printf ("client :\t");
			client_afficher(& (s->client));
		}
	}
}

/* ____ Sous-routine lecture messages buffer
	Pre-condition : buf contient une chaine parmi "", "STOP", "nb=x1 ...nb=xp " ou x1,...,xp sont des chiffres, "nb=x1 ...nb=xp STOP" ou x1,...,xp sont des chiffres
	Post-condition : buf == "" (tous les messages interpretes)
*/
int buffer_lire(char* buf) {
	int stop = 0, nb = 0, nbElementsLus = 0;
	char tmp[100] = "";

	/* interpretation message(s) buffer */
	while(strlen(buf) > 0) {
		/* on tente de lire 'nb=x' pour x chiffre (soit dans {0,...,9}) */
		nbElementsLus = sscanf(buf, "nb=%d", &nb);
		if (nbElementsLus == 1) {
			printf("\t\tmessage recu 'nb=%d'\n", nb);
			strcpy(tmp, buf);
			strcpy(buf, tmp +5);
		}
		/* on tente de lire 'STOP' */
		else if (strcmp(buf, "STOP") == 0) {
			printf("\t\tmessage recu 'STOP'\n");
			stop = 1;
			strcpy(buf, "");
		}
	}

	printf("\tsortie %s: stop == %d, buffer == '%s'\n", __func__, stop, buf);
	return stop;
}

/* ____ Sous-routines scenario de test client  */

/* Connexion client
	Pre-condition : client initialise (pas encore de socket creee => c->id == -1) */
void client_connexion(client* c) {
	int saisie = 0;
	char info_saisie[(NOM_HOTE_TAILLE_MAX >= IP_TAILLE_MAX ? NOM_HOTE_TAILLE_MAX : IP_TAILLE_MAX)+1];

	/* choix info a renseigner */
	while(saisie < 1 || saisie > 2) {
		printf("Saisie info hote serveur : taper 1 pour se connecter par saisie du nom, 2 pour se connecter par saisie de l'ip :\t");
		scanf("%d", &saisie);
		printf("\n");
	}

	/* saisie info */
	printf("Saisir %s :\t", (saisie == 1 ? "nom hote serveur" : "ip serveur"));
	scanf("%s", info_saisie);

	printf("\ttentative connexion du client pour info serveur '%s'\n", info_saisie);

	/* connexion */
	if (saisie == 1)
		client_connecter_par_nom(c, info_saisie);
	else
		client_connecter_par_ip(c, info_saisie);
}

/* Reception messages client */
void client_receptions(client* c) {
	int res = SYSTEME_OK, stop = 0, nb = 0;
	char* buf = client_get_buffer(c);

	while((res != SYSTEME_KO) && (stop == 0)) {
		res = client_recevoir(c);
		nb ++;

		printf("\tclient_recevoir() #%d : %d octets recus sur le buffer de valeur '%s' -- chaine attendue '%s'\n", nb, res, buf, "STOP");
		stop = buffer_lire(buf);
	}
}

/* Emission messages client */
void client_emissions(client* c) {
	int res = SYSTEME_OK, nb = 0;
	char buf[BUFFER_TAILLE] = "";

	while(nb <= 4 && res != SYSTEME_KO) {
		if (nb < 4) {
			buf[0] = '\0';
			sprintf(buf, "nb=%d ", nb);
		}
		else
			strcpy(buf, "STOP");

		client_set_buffer(c, buf);
		res = client_emettre(c);
		nb++;

		printf("\tclient_emettre() #%d : %d octets transmis pour le message '%s'\n", nb, res, buf);
	}
}

/* Suppression client */
void client_deconnexion(client* c) {
	/* __ destruction */
	printf(" suppression client # 1: \n");
	client_delete(c);
	printf(" suppression client # 2: \n");
	client_delete(c);
}

/* ____ Sous-routines scenario de test serveur  */

/* Construction & connexion d'un client
	Pre-condition : serveur initialise (pas encore de socket creee => s->id == s->client.id == -1) */
void serveur_connexion(serveur* s) {
	printf("\ttentative creation serveur (nouvelle socket)\n");
	serveur_creer(s);

	if (serveur_est_connecte(s)) {
		printf("\t-> succes. Affichage serveur :\n");
		serveur_afficher(s);

		printf("\tmise en attente connexion client\n");
		serveur_connecter_client(s);
	}
	else
		printf("\t-> echec.\n");
}

/* Emission messages serveur */
void serveur_receptions(serveur* s) {
	int res = SYSTEME_OK, stop = 0, nb = 0;
	char* buf = serveur_get_buffer(s);

	while((res != SYSTEME_KO) && (stop == 0)) {
		res = serveur_recevoir(s);
		nb ++;

		printf("\tserveur_recevoir() #%d : %d octets recus sur le buffer de valeur '%s' -- chaine attendue '%s'\n", nb, res, serveur_get_buffer(s), "STOP");
		stop = buffer_lire(buf);
	}
}

/* Reception messages serveur */
void serveur_emissions(serveur* s) {
	int res = SYSTEME_OK, nb = 0;
	char buf[BUFFER_TAILLE] = "";

	while(nb <= 4 && res != SYSTEME_KO) {
		if (nb < 4) {
			buf[0] = '\0';
			sprintf(buf, "nb=%d ", nb);
		}
		else
			strcpy(buf, "STOP");

		serveur_set_buffer(s, buf);
		res = serveur_emettre(s);
		nb++;

		printf("\tserveur_emettre() #%d : %d octets transmis pour le message '%s'\n", nb, res, buf);
	}
}

/* Deconnexion du client & suppression serveur */
void serveur_deconnexion(serveur* s) {
	/* __ deconnexion */
	printf("\tdeconnexion client # 1 -- intialement serveur_est_client_connecte(s) == %d: \n", serveur_est_client_connecte(s));
	serveur_deconnecter_client(s);
	printf("\tdeconnexion client # 2 -- intialement serveur_est_client_connecte(s) == %d: \n", serveur_est_client_connecte(s));
	serveur_deconnecter_client(s);

	/* __ destruction */
	printf("\tsuppression serveur # 1 -- intialement serveur_est_connecte(s) == %d: \n", serveur_est_connecte(s));
	serveur_delete(s);
	printf("\tsuppression serveur # 2 -- intialement serveur_est_connecte(s) == %d: \n", serveur_est_connecte(s));
	serveur_delete(s);
}

/* ____ Scenario test */

/* scenario test client
	Pre-condition : client initialise (pas encore de socket creee => c->id == -1) */
void client_main(client* c) {
	/* __ construction & connexion */
	printf("____ TEST 1 : construction client & connexion serveur : \n");
	client_connexion(c);
	printf("\taffichage client apres apres appel client_connexion() :\n");
	client_afficher(c);

	/* __ echange messages si connexion reussie */
	if (client_est_connecte(c)) {
		/* ____ envoi messages */
		printf("____ TEST2 : envoyer messages (5 messages, le dernier 'STOP') :\n");
		client_emissions(c);

		/* __ reception messages */
		printf("____ TEST3 : recevoir messages (tant que 'STOP' non recu) :\n");
		client_receptions(c);
	}

	/* ____ deconnexion & suppression */
	printf("____ TEST4 : deconnexion & suppression client : \n");
	client_deconnexion(c);
	printf("\taffichage client apres apres appel client_deconnexion() :\n");
	client_afficher(c);
}

/* scenario test serveur
	Pre-condition : serveur initialise (pas encore de socket creee => s->id == s->client.id == -1) */
void serveur_main(serveur* s) {
	/* __ construction & connexion */
	printf("____ TEST 1 : construction serveur & connexion client : \n");
	serveur_connexion(s);
	printf("\taffichage serveur apres apres appel serveur_connexion() :\n");
	serveur_afficher(s);

	/* __ echange messages si connexion reussie */
	if (serveur_est_client_connecte(s)) {
		/* __ reception messages */
		printf("____ TEST2 : recevoir messages (tant que 'STOP' non recu) :\n");
		serveur_receptions(s);

		/* ____ envoi messages */
		printf("____ TEST3 : envoyer messages (5 messages, le dernier 'STOP') :\n");
		serveur_emissions(s);
	}

	/* ____ deconnexion & suppression */
	printf("____ TEST4 : deconnexion & suppression serveur : \n");
	serveur_deconnexion(s);
	printf("\taffichage serveur apres apres appel serveur_deconnexion() :\n");
	serveur_afficher(s);
}

/* Fonction principale */
int main(void) {
	int saisie = 0;
	client c;
	c.id = SYSTEME_OK;
	serveur s;
	s.id = SYSTEME_OK;

	/* ____ capture signal CTRL +C */
	struct sigaction handler_interruption;
	g_c = &c;
	g_s = &s;
	handler_interruption.sa_handler =(void (*)(int)) nettoyer_socket;
	sigaction(SIGINT, &handler_interruption, NULL);

	/* ____ initialisation */
	printf("____ TEST0 : initialisations : \n");

	client_initialiser(& c);
	printf("\taffichage client apres apres appel client_initialiser() :\n");
	client_afficher(& c);

	serveur_initialiser(& s);
	printf("\taffichage serveur apres apres appel serveur_initialiser() :\n");
	serveur_afficher(& s);

	/* ____ client ou serveur */
	while(saisie != 1 && saisie != 2) {
		printf("Taper 1 pour lancer le client et 2 pour lancer le serveur :\t");
		scanf("%d", &saisie);
	}

	/* ____ client */
	if (saisie == 1)
		client_main(& c);
	else
		serveur_main(& s);

	return EXIT_SUCCESS;
}
