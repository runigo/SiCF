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

#include "pendule.h"


	// Force de gravitation
double gravitationPendul(penduleT * pendul, int choix);
	// dt * vitesse du pendule
double vitessePendul(penduleT * pendul);
	// Somme des forces
double forcesPendul(penduleT * pendul, int choix, float courantJosephson);

// Variation des parametres reduits
void changeAlpha(penduleT * pendul, float facteur);
void changeKapa(penduleT * pendul, float facteur);
void changeGamma(penduleT * pendul, float facteur);


void penduleJauge(penduleT * pendule, float jauge)
	{
	(*pendule).ancien = (*pendule).ancien + jauge;
	(*pendule).actuel = (*pendule).actuel + jauge;
	(*pendule).nouveau = (*pendule).nouveau + jauge;
	return;
	}

// Initialisation des parametres
void penduleInitialiseParametre(penduleT * pendul, float masse, float longueur, float dissipation)
	{
	(*pendul).masse = masse;
	(*pendul).longueur = longueur;
	(*pendul).dissipation = dissipation;
	return;
	}
void penduleInitialiseExterieur(penduleT * pendul, float couplage, float gravitation, float dt)
	{
	(*pendul).couplage = couplage;
	(*pendul).gravitation = gravitation;
	penduleInitialiseKapa(pendul, couplage, dt);
	penduleInitialiseGamma(pendul, gravitation, dt);
	penduleInitialiseAlpha(pendul, (*pendul).dissipation, dt);
	return;
	}
void penduleReinitialiseMasse(penduleT * pendul, float masse, float dt)
	{
	(*pendul).masse = masse;
	penduleInitialiseKapa(pendul, (*pendul).couplage, dt);
	penduleInitialiseGamma(pendul, (*pendul).gravitation, dt);
	penduleInitialiseAlpha(pendul, (*pendul).dissipation, dt);
	return;
	}
void penduleInitialisePosition(penduleT * pendul, float ancien, float actuel)
	{
	((*pendul).ancien) = ancien;
	((*pendul).actuel) = actuel;
	((*pendul).nouveau) = 2 * actuel - ancien;
	return;
	}
void penduleInitialiseDephasage(penduleT * pendul, float dephasage)
	{
	((*pendul).dephasage)=dephasage;
	return;
	}

// Initialisation des parametres reduits
void penduleInitialiseAlpha(penduleT * pendul, float dissipation, float dt)
	{
	if((*pendul).masse!=0.0)
		{
		(*pendul).alpha = - dt * dissipation / (*pendul).masse / (*pendul).longueur;
		}
	else
		{// si masse nulle, nouvelle définition de alpha
		(*pendul).alpha = - dt * dissipation;
		}
	return;
	}
void penduleInitialiseKapa(penduleT * pendul, float couplage, float dt)
	{
	if((*pendul).masse!=0.0)
		{
		(*pendul).kapa = - dt * dt * couplage / (*pendul).masse;
		}
	else
		{// si masse nulle, nouvelle définition de kapa
		(*pendul).kapa = - dt * dt * couplage;
		}
	return;
	}
void penduleInitialiseGamma(penduleT * pendul, float gravitation, float dt)
	{
	if((*pendul).longueur!=0.0)
		{
		(*pendul).gamma = - dt * dt * gravitation / (*pendul).longueur;
		}
	else
		{// si longueur nulle, nouvelle définition de gamma
		(*pendul).gamma = - dt * dt * gravitation;
		}
	return;
	}



// Variation des parametres
void penduleChangeMasse(penduleT * pendul, float facteur)
	{
	if(facteur != 0.0)
		{
		(*pendul).masse = (*pendul).masse * facteur;
		(*pendul).alpha = (*pendul).alpha / facteur;
		(*pendul).kapa = (*pendul).kapa / facteur;
		}
	return;
	}
void penduleChangeLongueur(penduleT * pendul, float facteur)
	{
	if(facteur != 0.0)
		{
		(*pendul).longueur = (*pendul).longueur * facteur;
		(*pendul).gamma = (*pendul).gamma / facteur;
		}
	return;
	}
void penduleChangeDissipation(penduleT * pendul, float facteur)
	{
	(*pendul).alpha = (*pendul).alpha * facteur;
	return;
	}
void penduleChangeCouplage(penduleT * pendul, float facteur)
	{
	(*pendul).couplage = (*pendul).couplage * facteur;
	(*pendul).kapa = (*pendul).kapa * facteur;
	return;
	}
void penduleChangeGravitation(penduleT * pendul, float facteur)
	{
	(*pendul).gamma = (*pendul).gamma * facteur;
	return;
	}
void penduleAjouteDephasage(penduleT * pendul, float dephasage)
	{
	(*pendul).dephasage = (*pendul).dephasage + dephasage;
	return;
	}

// Evolution temporelle du pendule

void penduleIncremente(penduleT * pendul)
	{// incremente la position
	((*pendul).ancien)=((*pendul).actuel);
	((*pendul).actuel)=((*pendul).nouveau);
	return;
	}

void penduleInertie(penduleT * pendul, int choix, float courantJosephson)
	{// application du principe d'inertie
	(*pendul).nouveau = forcesPendul(pendul, choix, courantJosephson) + 2*((*pendul).actuel) - (*pendul).ancien;
	return;
	}

double forcesPendul(penduleT * pendul, int choix, float courantJosephson)
	{// somme des forces sur le pendule
	return ((*pendul).forceCouplage + gravitationPendul(pendul, choix) + ((*pendul).alpha)*vitessePendul(pendul) + courantJosephson);
	}

void penduleCouplage(penduleT * m1, penduleT * m2, penduleT * m3)
	{// calcul des forces de couplage
	double gauche, droite;

	gauche = (*m1).kapa * ( (*m2).actuel - (*m1).actuel + (*m1).dephasage );
	droite = (*m2).kapa * ( (*m2).actuel - (*m3).actuel - (*m2).dephasage );

	(*m2).forceCouplage = gauche + droite;

	return;
	}

double gravitationPendul(penduleT * pendul, int choix)
	{// Calcul de la FORCE DE RAPPEL
	double forceRappel;
	switch(choix)
		{
		case 1:// gravitation
			forceRappel = (*pendul).gamma * sin((*pendul).actuel);
		break;
		case 2:// linearisation harmonique
			forceRappel = (*pendul).gamma * (*pendul).actuel;
		break;
		case 3:// corde vibrante
			forceRappel = 0.0;
		break;
		default:// corde vibrante
			forceRappel = 0.0;
		break;
		}
	return forceRappel;
	}

double vitessePendul(penduleT * pendul)
	{// Retourne vdt
	return (*pendul).actuel - (*pendul).ancien;
	}


void penduleAffiche(penduleT * pendul)
	{// Affichage de la position et des parametres
	printf("   ancien    %f\n",(*pendul).ancien);
	printf("   actuel    %f\n",(*pendul).actuel);
	printf("   nouveau   %f\n",(*pendul).nouveau);

	printf("    masse     %f\n",(*pendul).masse);
	printf("    longueur  %f\n",(*pendul).longueur);
	printf("    alpha    %f\n",(*pendul).alpha);
	printf("    kapa     %f\n",(*pendul).kapa);
	printf("    gamma    %f\n",(*pendul).gamma);

	printf("forceCouplage %f\n",(*pendul).forceCouplage);
	printf("  forceTotale %f\n",(*pendul).forceTotale);

	return;
	}

////////////////////////////////////////////////////////////
