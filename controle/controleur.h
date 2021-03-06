/*
Copyright avril 2018, Stephan Runigo
runigo@free.fr
SiCF 1.4  simulateur de corde vibrante et spectre
Ce logiciel est un programme informatique servant à simuler l'équation
d'une corde vibrante, à calculer sa transformée de fourier, et à donner
une représentation graphique de ces fonctions. 
Ce logiciel est régi par la licence CeCILL soumise au droit français et
respectant les principes de diffusion des logiciels libres. Vous pouvez
utiliser, modifier et/ou redistribuer ce programme sous les conditions
de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
sur le site "http://www.cecill.info".
En contrepartie de l'accessibilité au code source et des droits de copie,
de modification et de redistribution accordés par cette licence, il n'est
offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
seule une responsabilité restreinte pèse sur l'auteur du programme, le
titulaire des droits patrimoniaux et les concédants successifs.
A cet égard  l'attention de l'utilisateur est attirée sur les risques
associés au chargement,  à l'utilisation,  à la modification et/ou au
développement et à la reproduction du logiciel par l'utilisateur étant
donné sa spécificité de logiciel libre, qui peut le rendre complexe à
manipuler et qui le réserve donc à des développeurs et des professionnels
avertis possédant  des  connaissances  informatiques approfondies. Les
utilisateurs sont donc invités à charger  et  tester  l'adéquation du
logiciel à leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement,
à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
pris connaissance de la licence CeCILL, et que vous en avez accepté les
termes.
*/

#ifndef _CONTROLEUR_
#define _CONTROLEUR_

#include "../donnees/options.h"			//	Options de la ligne de commande
#include "../modele/change.h"			//	Changement des parametres du système
#include "../modele/observables.h"		//	Calculs énergétiques
#include "../projection/projection.h"	//	Projection entre les représentations
#include "../graphisme/graphique.h"		//	Librairie SDL
#include "../graphisme/horloge.h"		//	Librairie SDL
#include "../donnees/fichier.h"

typedef struct ControleurT controleurT;
	struct ControleurT
		{

		options option;		//	Options de la ligne de commande

		systemeT systeme;		//	Modélisation physique de la chaîne

		fonction spectreG;	//	Spectre de la partie gauche
		fonction spectreD;	//	Spectre de la partie droite

		graphe graphCorde;		//	Représentation graphique de la corde
		graphe graphSpectre;	//	Représentation graphique des spectres

		SDL_Event evenement;	//	Évenement SDL

		horlogeT horloge; // Horloge SDL

		int sortie; // sortie de SiCP si > 0

		int appui;	//	1 si le bouton de la souris est appuyé, 0 sinon.

		};

		// Fonction utilisée par principale.c
int controleurSimulationGraphique(controleurT * controleur);



#endif

