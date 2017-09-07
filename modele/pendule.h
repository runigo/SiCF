/*
Copyright décembre 2016, Stephan Runigo
runigo@free.fr
SiCF 1.1  simulateur de chaîne de pendules
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

#ifndef _PENDULE_
#define _PENDULE_

#include "../donnees/constantes.h"

typedef struct Pendule pendule;
	struct Pendule
		{
		// Position et vitesse du pendule
		double ancien, actuel, nouveau;

		// Paramètres physiques
		float masse;
		float longueur;
		float couplage;
		float dissipation;	// Mémoire pour l'extrémité absorbante

		// Déphasage avec le pendule suivant
		float dephasage;


		float alpha;	// dt * constante de frottement / masse
		float kapa;	// dt2 * constante de couplage / masse
		float gamma;	// dt2 * accélération gravitationnelle

		// force de couplage avec le suivant
		double forceCouplage;

		// somme des forces appliquées au pendule :
		//couplage + gravitation + dissipation + courantJosephson)
		double forceTotale;
		};


	// Initialisation du pendule

void penduleInitialiseParametre(pendule * pendul, float masse, float longueur, float dissipation);
void penduleInitialiseExterieur(pendule * pendul, float couplage, float gravitation, float dt);
void penduleInitialisePosition(pendule * pendul, float ancien, float actuel);
void penduleInitialiseDephasage(pendule * pendul, float dephasage);
void penduleInitialiseCouplage(pendule * pendul, float couplage, float dt);


	// Initialisation des parametres reduits

void penduleInitialiseAlpha(pendule * pendul, float dissipation, float dt);
void penduleInitialiseKapa(pendule * pendul, float couplage, float dt);
void penduleInitialiseGamma(pendule * pendul, float gravitation, float dt);


	// Variation des parametres

void penduleChangeMasse(pendule * pendul, float facteur);
void penduleChangeLongueur(pendule * pendul, float facteur);
void penduleChangeCouplage(pendule * pendul, float facteur);
void penduleChangeDissipation(pendule * pendul, float facteur);
void penduleChangeGravitation(pendule * pendul, float facteur);
void penduleAjouteDephasage(pendule * pendul, float dephasage);


	// Evolution temporelle du pendule

void penduleIncremente(pendule * pendul);
void penduleInertie(pendule * pendul, int choix, float courantJosephson);
void penduleCouplage(pendule * pendulPrecedent, pendule * pendul, pendule * pendulSuivant);

//int penduleMoteurLimite(pendule * pendul, float amplitude, float phase, int etat);


	// Normalisation de la moyenne

void penduleJauge(pendule * pendul, float jauge);

	// Affichage des paramètres

void penduleAffiche(pendule * pendul);

#endif
