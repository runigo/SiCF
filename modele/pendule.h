/*
Copyright novembre 2017, Stephan Runigo
runigo@free.fr
SiCP 1.4.1 simulateur de chaîne de pendules
SiCF 1.2.1  simulateur de corde vibrante et spectre
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
offert aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons,
seule une responsabilité restreinte pèse sur l'auteur du programme, le
titulaire des droits patrimoniaux et les concédants successifs.
A cet égard  l'attention de l'utilisateur est attirée sur les risques
associés au chargement, à l'utilisation, à la modification et/ou au
développement et à la reproduction du logiciel par l'utilisateur étant
donné sa spécificité de logiciel libre, qui peut le rendre complexe à
manipuler et qui le réserve donc à des développeurs et des professionnels
avertis possédant des connaissances informatiques approfondies. Les
utilisateurs sont donc invités à charger et tester l'adéquation du
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

typedef struct PenduleT penduleT;
	struct PenduleT
		{
		double ancien, actuel, nouveau; // Position

		float dephasage; // Déphasage avec le précédent

		float masse;
		float longueur;
		float couplage;
		float gravitation;
		float dissipation;

		float absorbance; // Mémoire pour l'extrémité absorbante

		float alpha;	// dt * constante de frottement / masse / longueur
		float kapa;	// dt2 * constante de couplage / masse
		float gamma;	// dt2 * accélération gravitationnelle / longueur

		double forceCouplage;	// force de couplage avec les pendules voisins
		double forceTotale;	// somme des forces appliquées au pendule : 
					// (couplage + gravitation + dissipation + courantJosephson)
		};

	// Initialisation du pendule

void penduleInitialiseParametre(penduleT * pendul, float masse, float longueur, float dissipation);
void penduleInitialiseExterieur(penduleT * pendul, float couplage, float gravitation, float dt);
void penduleInitialiseCouplage(penduleT * pendul, float couplage, float dt);
void penduleInitialisePosition(penduleT * pendul, float ancien, float actuel);
void penduleInitialiseDephasage(penduleT * pendul, float dephasage);


	// Initialisation des parametres reduits

void penduleInitialiseAlpha(penduleT * pendul, float dissipation, float dt);
void penduleInitialiseKapa(penduleT * pendul, float couplage, float dt);
void penduleInitialiseGamma(penduleT * pendul, float gravitation, float dt);


	// Variation des parametres

void penduleReinitialiseMasse(penduleT * pendul, float masse, float dt);
void penduleChangeMasse(penduleT * pendul, float facteur);
void penduleChangeLongueur(penduleT * pendul, float facteur);
void penduleChangeCouplage(penduleT * pendul, float facteur);
void penduleChangeDissipation(penduleT * pendul, float facteur);
void penduleChangeGravitation(penduleT * pendul, float facteur);
void penduleAjouteDephasage(penduleT * pendul, float dephasage);

void penduleJauge(penduleT * pendule, float jauge);

	// Evolution temporelle du pendule

void penduleIncremente(penduleT * pendul);
void penduleInertie(penduleT * pendul, int choix, float courantJosephson);
void penduleCouplage(penduleT * m1, penduleT * m2, penduleT * m3);

int penduleMoteurLimite(penduleT * pendul, float amplitude, float phase, int etat);


	// Affichage des paramètres

void penduleAffiche(penduleT * pendul);

#endif
