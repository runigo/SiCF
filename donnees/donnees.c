/*
Copyright janvier 2017, Stephan Runigo
runigo@free.fr
SiCF 1.1.1  simulateur de corde vibrante et spectre
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

#include "donnees.h"

	//		Valeurs initiales des données physiques du simulateur,
	//						et création du système

	//		Valeur initiale et création du graphe

	//		Initialisation de la SDL


int donneesSysteme(systeme * system, options * option);
int donneesGraphe(graphe * graph, options * option);

int donneesControleur(controleur * control)
	{

	(*control).option.sortie = 0;	// Sortie de SiCP si > 0

		fprintf(stderr, " Initialisation du système\n");
	donneesSysteme(&(*control).system, &(*control).option);

		//fprintf(stderr, " Création du système\n");
	//systemeCreation(&(*control).system);

		fprintf(stderr, " Initialisation des graphe\n");
	donneesGraphe(&(*control).graphCorde, &(*control).option);
	donneesGraphe(&(*control).graphSpectre, &(*control).option);

		fprintf(stderr, " Initialisation SDL\n");
	graphiqueInitialise((*control).system.equation, (*control).option.fond);

	return 0;
	}

int donneesGraphe(graphe * graph, options * option)
	{
	(*graph).rouge=20;
	(*graph).bleu=150;
	(*graph).vert=200;
	(*graph).fond=(*option).fond;	//	Couleur du fond
	(*graph).echelle=0.003;
	return 0;
	}

int donneesOptions(options * option)
	{
		// Préréglage des valeurs optionnelles

	(*option).fond=240;		// couleur du fond de l'affichage
	(*option).mode = 1;	// -1 : Wait, 1 : Poll
	(*option).pause=25;		// temps de pause SDL en ms

	(*option).duree = 100;	// 100 : temps réèl. Voir options.c

	(*option).equation=3;	// 1 : pendule, 2 : linéarisation,
							//	 3 : corde, 4 : dioptre

	(*option).dt=0.0006;	// discrétisation du temps
							// 25 images par seconde, SDL_Delay(30);
							// dt*duree = 0.004

	(*option).dephasage=0*PI;

	(*option).frequence=11.1;	// Fréquence du générateur de signaux

	(*option).dissipation=0.0;	// Fréquence du générateur de signaux

	(*option).thread=0;		// 0 : un processus, 1 : deux threads

	return 0;
	}

int donneesSysteme(systeme * system, options * option)
	{

		// Initialisation du moteurs

	(*system).moteur.dt = (*option).dt;	// discrétisation du temps
	(*system).moteur.horloge = 0.0;
	(*system).moteur.chrono = 0.0;

	(*system).moteur.courant=15.0;		// Mémoire courant Josephson si = 0
	(*system).moteur.josephson=(*system).moteur.courant*(*system).moteur.dt*(*system).moteur.dt;

	(*system).moteur.generateur = 1;	// éteint, sinus, carre, impulsion
	(*system).moteur.amplitude=0.57;		// Amplitude du générateur de signaux
	(*system).moteur.frequence=(*option).frequence;	// Fréquence du générateur de signaux
	(*system).moteur.phi=0;


		// Caractéristique de la chaîne
	(*system).libreFixe = 2;	// 0 periodique, 1 libre, 2 fixe
	(*system).equation = (*option).equation;	// 1 : pendule pesant
												// 2 : linéarisation
												// 3 : corde
												// 4 : dioptre

		// Paramètres physiques
	(*system).gravitation = 9.81;
	(*system).masse = 1.0;
	(*system).longueur = 9.81/4/PI/PI; // = 25 cm => période = 1 s
	(*system).dissipation = (*option).dissipation;
	(*system).couplage = 10321.1;
	(*system).dephasage = (*option).dephasage;




	if((*system).equation == 3 || (*system).equation == 4)
		{		 // donneeCorde;
		(*system).couplage = (*system).couplage * 10.0;
		(*system).gravitation = 0.0;
		(*system).libreFixe = 2;
		(*system).moteur.josephson=0.0;
		}

	systemeInitialise(system);
	// fichier.c
	//
	//fichierLecture(system);

	return 0;
	}

//////////////////////////////////////////////////////////////////////////
