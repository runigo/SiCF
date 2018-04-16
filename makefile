
#Copyright avril 2018, Stephan Runigo
#runigo@free.fr
#SiCF 1.4  simulateur de corde vibrante et spectre
# Ce logiciel est un programme informatique servant à simuler l'équation
# d'une corde vibrante, à calculer sa transformée de fourier, et à donner
# une représentation graphique de ces fonctions. 
# Ce logiciel est régi par la licence CeCILL soumise au droit français et
# respectant les principes de diffusion des logiciels libres. Vous pouvez
# utiliser, modifier et/ou redistribuer ce programme sous les conditions
# de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
# sur le site "http://www.cecill.info".
# En contrepartie de l'accessibilité au code source et des droits de copie,
# de modification et de redistribution accordés par cette licence, il n'est
# offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
# seule une responsabilité restreinte pèse sur l'auteur du programme, le
# titulaire des droits patrimoniaux et les concédants successifs.
# A cet égard  l'attention de l'utilisateur est attirée sur les risques
# associés au chargement,  à l'utilisation,  à la modification et/ou au
# développement et à la reproduction du logiciel par l'utilisateur étant
# donné sa spécificité de logiciel libre, qui peut le rendre complexe à
# manipuler et qui le réserve donc à des développeurs et des professionnels
# avertis possédant  des  connaissances  informatiques approfondies. Les
# utilisateurs sont donc invités à charger  et  tester  l'adéquation du
# logiciel à leurs besoins dans des conditions permettant d'assurer la
# sécurité de leurs systèmes et ou de leurs données et, plus généralement,
# à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
# Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
# pris connaissance de la licence CeCILL, et que vous en avez accepté les
# termes.

CC=gcc
EXEC=SiCF
CFLAGS= -Wall -Wextra -Werror --std=c99
#LDFLAGS= -Wall -Wextra -Werror --std=c99 -lm -lpthread
LDFLAGS= -Wall -Wextra -Werror --std=c99 -lm

OBJDIR = ./obj

all : $(EXEC)

$(EXEC) : $(OBJDIR)/principale.o $(OBJDIR)/options.o $(OBJDIR)/donnees.o $(OBJDIR)/fichier.o $(OBJDIR)/controleur.o $(OBJDIR)/projection.o $(OBJDIR)/horloge.o $(OBJDIR)/graphique.o $(OBJDIR)/graphe.o $(OBJDIR)/fourier.o $(OBJDIR)/observables.o $(OBJDIR)/change.o $(OBJDIR)/systeme.o $(OBJDIR)/moteurs.o $(OBJDIR)/pendule.o
	$(CC) -g $(OBJDIR)/principale.o $(OBJDIR)/options.o $(OBJDIR)/donnees.o $(OBJDIR)/fichier.o $(OBJDIR)/controleur.o $(OBJDIR)/projection.o $(OBJDIR)/horloge.o $(OBJDIR)/graphique.o $(OBJDIR)/graphe.o $(OBJDIR)/fourier.o $(OBJDIR)/observables.o $(OBJDIR)/change.o $(OBJDIR)/systeme.o $(OBJDIR)/moteurs.o $(OBJDIR)/pendule.o ` sdl-config --libs` $(LDFLAGS) -o $(EXEC)

$(OBJDIR)/principale.o : controle/principale.c controle/principale.h
	$(CC) -c -g controle/principale.c $(CFLAGS) -o $@

$(OBJDIR)/options.o : donnees/options.c donnees/options.h
	$(CC) -c -g donnees/options.c $(CFLAGS) -o $@

$(OBJDIR)/donnees.o : donnees/donnees.c donnees/donnees.h
	$(CC) -c -g donnees/donnees.c $(CFLAGS) -o $@

$(OBJDIR)/fichier.o : donnees/fichier.c donnees/fichier.h
	$(CC) -c -g donnees/fichier.c $(CFLAGS) -o $@

$(OBJDIR)/controleur.o : controle/controleur.c controle/controleur.h
	$(CC) -c -g controle/controleur.c $(CFLAGS) -o $@

$(OBJDIR)/projection.o : projection/projection.c projection/projection.h
	$(CC) -c -g projection/projection.c $(CFLAGS) -o $@

$(OBJDIR)/horloge.o : graphisme/horloge.c graphisme/horloge.h
	$(CC) -c -g graphisme/horloge.c $(CFLAGS) -o $@

$(OBJDIR)/graphique.o : graphisme/graphique.c graphisme/graphique.h
	$(CC) -c -g graphisme/graphique.c $(CFLAGS) -o $@

$(OBJDIR)/graphe.o :graphisme/graphe.c graphisme/graphe.h
	$(CC) -c -g graphisme/graphe.c $(CFLAGS) -o $@

$(OBJDIR)/fourier.o : modele/fourier.c modele/fourier.h
	$(CC) -c -g modele/fourier.c $(CFLAGS) -o $@

$(OBJDIR)/observables.o : modele/observables.c modele/observables.h
	$(CC) -c -g modele/observables.c $(CFLAGS) -o $@

$(OBJDIR)/change.o : modele/change.c modele/change.h
	$(CC) -c -g modele/change.c $(CFLAGS) -o $@

$(OBJDIR)/systeme.o : modele/systeme.c modele/systeme.h
	$(CC) -c -g modele/systeme.c $(CFLAGS) -o $@

$(OBJDIR)/moteurs.o : modele/moteurs.c modele/moteurs.h
	$(CC) -c -g modele/moteurs.c $(CFLAGS) -o $@

$(OBJDIR)/pendule.o : modele/pendule.c modele/pendule.h
	$(CC) -c -g modele/pendule.c $(CFLAGS) -o $@

clean :
	rm $(OBJDIR)/*.o

net : clean
	rm $(EXEC)

