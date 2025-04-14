# Un fichier 'Makefile' est un ensemble de cibles
# Pour chque cible, il faut donner les regles (commande(s)) de generation de la cible
# Pour chque cible, on indique ses dependances fichiers sources ou autres cibles qui, si elles ont ete mises a jour, necessitent de regenerer la cible)

# ____________________________________________________
#
# ____ Elements de syntaxe :
#
# Marque de commentaires : caractere # precedant la ligne a commenter
#
# Macros :
# 	definition :		MACRO = valeur
# 	utilisation :		$(MACRO)
#
# Cibles :
#
# cible: dependances			#liste des dependances separees par un caractere d'espacement
#	regle1 de generation		#/!\ chaque commande necessite 1 passage a la ligne suivi d'une tabulation /!\
#	regle2 de generation
#	...
#
# ____ Variables generiques :
#
# $@		:	cible courante
# $<		:	premiere dependance de la cible courante
# $^		:	toutes les dependances de la cible courante
#
# ____________________________________________________

# ____ general
# macros : variable CFLAGS pour les options de compilation
CFLAGS = -Wall -pedantic -Wextra	#options de compilation

# regles generiques : 1 regle pour la generation des objets (.o) a partir des sources (.c)
.SUFFIXES: .c
.c.o:
	gcc $(CFLAGS) -c  $<

# ____ liste des executables

all: main.exe

main.exe: bateau_type.o placement.o flotte.o case_grille.o grille.o joueur.o client_serveur.o jeu.o vue.o control_bataille.o main.o
	gcc $^ -o main.exe `pkg-config gtk+-3.0 --libs`

main.o: main.c control_bataille.h vue.h jeu.h client_serveur.h joueur.h bateau_type.h placement.h flotte.h case_grille.h grille.h
	gcc $(CFLAGS) -c  main.c `pkg-config gtk+-3.0 --cflags`

# ____ dependances pour la mise a jour des objets
control_bataille.o: control_bataille.c control_bataille.h jeu.h vue.h
	gcc $(CFLAGS) -c  control_bataille.c `pkg-config gtk+-3.0 --cflags`
bateau_type.o: bateau_type.c bateau_type.h global.h
placement.o: placement.c placement.h global.h
flotte.o: flotte.c flotte.h bateau_type.h placement.h global.h
case_grille.o: case_grille.c case_grille.h bateau_type.h global.h
grille.o: grille.c grille.h case_grille.h placement.h bateau_type.h global.h
joueur.o: joueur.c joueur.h bateau_type.h placement.h flotte.h case_grille.h grille.h global.h
client_serveur.o: client_serveur.c client_serveur.h global.h
jeu.o: jeu.c jeu.h client_serveur.h joueur.h bateau_type.h placement.h flotte.h case_grille.h grille.h global.h
vue.o:vue.c vue.h
	gcc $(CFLAGS) -c  vue.c `pkg-config gtk+-3.0 --cflags`

# ____ cibles speciales : nettoyage
clean:
	rm *.o *.exe

