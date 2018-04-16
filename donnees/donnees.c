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

#include "donnees.h"

	//		Valeurs initiales des données physiques du simulateur,
	//						et création du système

	//		Valeur initiale et création du graphe

	//		Initialisation de la SDL


int donneesSysteme(systemeT * systeme, options * option);
int donneesGraphe(graphe * graph, options * option);

int donneesControleur(controleurT * controleur)
	{

	(*controleur).sortie = 0;	// Sortie de SiCP si > 0

		fprintf(stderr, " Initialisation du système\n");
	donneesSysteme(&(*controleur).systeme, &(*controleur).option);

		//fprintf(stderr, " Création du système\n");
	//systemeCreation(&(*controleur).systeme);

		fprintf(stderr, " Initialisation des graphe\n");
	donneesGraphe(&(*controleur).graphCorde, &(*controleur).option);
	donneesGraphe(&(*controleur).graphSpectre, &(*controleur).option);

		fprintf(stderr, " Initialisation SDL\n");
	graphiqueInitialise((*controleur).option.fond);

		fprintf(stderr, " Initialisation horloge SDL\n");
	horlogeCreation(&(*controleur).horloge);

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

	(*option).sensibilite=0.11;	// sensibilité des commandes
	(*option).augmente = 1+(*option).sensibilite;	// sensibilité des commandes
	(*option).diminue = 1/(*option).augmente;	// sensibilité des commandes

	(*option).dt=0.0006;	// discrétisation du temps
	(*option).moteur=3;	// État du moteur
							// 25 images par seconde, SDL_Delay(30);
							// dt*duree = 0.004

	(*option).dephasage=0*PI;

	(*option).frequence=11.1;	// Fréquence du générateur de signaux

	(*option).dissipation=0.0;	// Fréquence du générateur de signaux

	return 0;
	}

int donneesSysteme(systemeT * systeme, options * option)
	{

		// Initialisation du moteurs

	(*systeme).moteur.dt = (*option).dt;	// discrétisation du temps
	//(*systeme).moteur.horloge = 0.0;
	(*systeme).moteur.chrono = 0.0;

	(*systeme).moteur.courant=15.0;		// Mémoire courant Josephson si = 0
	(*systeme).moteur.josephson=0.0;

	(*systeme).moteur.generateur = (*option).moteur;	// éteint, sinus, carre, impulsion
	(*systeme).moteur.amplitude=0.57;		// Amplitude du générateur de signaux
	(*systeme).moteur.frequence=(*option).frequence;	// Fréquence du générateur de signaux
	(*systeme).moteur.phi=0;


		// Caractéristique de la chaîne
	(*systeme).libreFixe = 2;	// 0 periodique, 1 libre, 2 fixe
	(*systeme).equation = (*option).equation;	// 1 : pendule pesant
												// 2 : linéarisation
												// 3 : corde
												// 4 : dioptre

		// Paramètres physiques
	(*systeme).gravitation = 9.81;
	(*systeme).masseGauche = 1.0;
	(*systeme).masseDroite = 3.0;
	(*systeme).longueur = 9.81/4/PI/PI; // = 25 cm => période = 1 s
	(*systeme).dissipation = (*option).dissipation;
	(*systeme).couplage = 100321.1;
	(*systeme).dephasage = (*option).dephasage;




	//if((*systeme).equation == 3 || (*systeme).equation == 4)
	//	{		 // donneeCorde;
	//	(*systeme).couplage = (*systeme).couplage * 10.0;
	//	(*systeme).gravitation = 0.0;
	//	(*systeme).libreFixe = 2;
	//	(*systeme).moteur.josephson=0.0;
	//	}

	systemeInitialise(systeme);
	changeConditionsLimites(systeme, (*systeme).libreFixe);
	// fichier.c
	//
	//fichierLecture(systeme);

	return 0;
	}

//////////////////////////////////////////////////////////////////////////
