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

#include "pendule.h"
#include "stdio.h"



// Force de gravitation
double gravitationPendul(pendule * pendul, int choix);

// dt * vitesse du pendule
double vitessePendul(pendule * pendul);

// Somme des forces
double forcesPendul(pendule * pendul, int choix, float courantJosephson);

// Variation des parametres reduits
void changeAlpha(pendule * pendul, float facteur);
void changeKapa(pendule * pendul, float facteur);
void changeGamma(pendule * pendul, float facteur);



void penduleJauge(pendule * pendul, float jauge)
	{
	(*pendul).ancien = (*pendul).ancien + jauge;
	(*pendul).actuel = (*pendul).actuel + jauge;
	(*pendul).nouveau = (*pendul).nouveau + jauge;
	return;
	}


// Initialisation des parametres
void penduleInitialiseParametre(pendule * pendul, float masse, float longueur, float dissipation)
	{
	(*pendul).masse = masse;
	(*pendul).longueur = longueur;
	(*pendul).dissipation = dissipation;
	return;
	}
void penduleInitialiseExterieur(pendule * pendul, float couplage, float gravitation, float dt)
	{
	(*pendul).couplage = couplage;
	penduleInitialiseAlpha(pendul, (*pendul).dissipation, dt);
	penduleInitialiseKapa(pendul, couplage, dt);
	penduleInitialiseGamma(pendul, gravitation, dt);
	return;
	}
void penduleInitialisePosition(pendule * pendul, float ancien, float actuel)
	{
	((*pendul).ancien) = ancien;
	((*pendul).actuel) = actuel;
	((*pendul).nouveau) = 2 * actuel - ancien;
	return;
	}
void penduleInitialiseDephasage(pendule * pendul, float dephasage)
	{
	((*pendul).dephasage)=dephasage;
	return;
	}


// Variation des parametres
void penduleChangeMasse(pendule * pendul, float facteur)
	{
	if(facteur > 0.1 && facteur < 9.1)
		{
		(*pendul).masse = (*pendul).masse * facteur;
		(*pendul).alpha = (*pendul).alpha / facteur;
		(*pendul).kapa = (*pendul).kapa / facteur;
		}
	else
		{
		fprintf(stderr, "Erreur penduleChangeMasse = %f\n",(*pendul).masse);
		}
	return;
	}
void penduleChangeLongueur(pendule * pendul, float facteur)
	{
	if(facteur > 0.1 && facteur < 9.1)
		{
		(*pendul).longueur = (*pendul).longueur * facteur;
		(*pendul).gamma = (*pendul).gamma / facteur;
		}
	else
		{
		fprintf(stderr, "Erreur penduleChangeLongueur = %f\n",(*pendul).longueur);
		}
	return;
	}
void penduleChangeCouplage(pendule * pendul, float facteur)
	{
	if(facteur > 0.1 && facteur < 9.1)
		{
		(*pendul).couplage = (*pendul).couplage * facteur;
		(*pendul).kapa = (*pendul).kapa * facteur;
		}
	else
		{
		fprintf(stderr, "Erreur penduleAjouteDephasage = %f\n",(*pendul).dephasage);
		}
	return;
	}
void penduleChangeDissipation(pendule * pendul, float facteur)
	{
	if(facteur > 0.1 && facteur < 9.1)
		{
		(*pendul).alpha = (*pendul).alpha * facteur;
		}
	else
		{
		fprintf(stderr, "Erreur penduleAjouteDephasage = %f\n",(*pendul).dephasage);
		}
	return;
	}
void penduleChangeGravitation(pendule * pendul, float facteur)
	{
	if(facteur > 0.1 && facteur < 9.1)
		{
		(*pendul).gamma = (*pendul).gamma * facteur;
		}
	else
		{
		fprintf(stderr, "Erreur penduleAjouteDephasage = %f\n",(*pendul).dephasage);
		}
	return;
	}
void penduleAjouteDephasage(pendule * pendul, float dephasage)
	{
	if(dephasage > -99 && dephasage < 99)
		{
		(*pendul).dephasage = (*pendul).dephasage + dephasage;
		}
	else
		{
		fprintf(stderr, "Erreur penduleAjouteDephasage = %f\n",(*pendul).dephasage);
		}
	return;
	}

// Evolution temporelle du pendule

void penduleIncremente(pendule * pendul)
	{// incremente la position
	((*pendul).ancien)=((*pendul).actuel);
	((*pendul).actuel)=((*pendul).nouveau);
	return;
	}

void penduleInertie(pendule * pendul, int choix, float courantJosephson)
	{// application du principe d'inertie
	(*pendul).nouveau = forcesPendul(pendul, choix, courantJosephson) + 2*((*pendul).actuel) - (*pendul).ancien;
	return;
	}

double forcesPendul(pendule * pendul, int choix, float courantJosephson)
	{// somme des forces sur le pendule
	return ((*pendul).forceCouplage + gravitationPendul(pendul, choix) + ((*pendul).alpha)*vitessePendul(pendul) + courantJosephson);
	}

void penduleCouplage(pendule * m1, pendule * m2, pendule * m3)
	{// calcul des forces de couplage
	double gauche, droite;

	gauche = (*m1).kapa * ( (*m2).actuel - (*m1).actuel + (*m1).dephasage );
	droite = (*m2).kapa * ( (*m2).actuel - (*m3).actuel - (*m2).dephasage );

	(*m2).forceCouplage = gauche + droite;

	return;
	}

double gravitationPendul(pendule * pendul, int equation)
	{// Calcul de la FORCE DE RAPPEL (gamma est négatif)
	double forceRappel;
	switch(equation)
		{
		case 1:// gravitation
			forceRappel = (*pendul).gamma * sin((*pendul).actuel);
		break;
		case 2:// linearisation
			forceRappel = (*pendul).gamma * (*pendul).actuel;
		break;
		case 3:// corde vibrante
			forceRappel = 0.0;
		break;
		case 4:// corde vibrante asymétrique
			forceRappel = 0.0;

		default:// corde vibrante
			forceRappel = 0.0;
		break;
		}
	return forceRappel;
	}

double vitessePendul(pendule * pendul)
	{// Retourne vdt
	return (*pendul).actuel - (*pendul).ancien;
	}

//
// Initialisation des parametres reduits
//
void penduleInitialiseAlpha(pendule * pendul, float dissipation, float dt)
	{
	if((*pendul).masse!=0.0)
		{
		(*pendul).alpha = - dt * dissipation / (*pendul).masse;
		}
	else
		{// si masse nulle, nouvelle définition de alpha
		(*pendul).alpha = - dt * dissipation;
		}
	return;
	}
void penduleInitialiseKapa(pendule * pendul, float couplage, float dt)
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
void penduleInitialiseGamma(pendule * pendul, float gravitation, float dt)
	{
	if((*pendul).longueur>0.000001 &&(*pendul).longueur<10000001)
		{
		(*pendul).gamma = - dt * dt * gravitation / (*pendul).longueur;
		}
	else
		{// si longueur nulle, nouvelle définition de gamma
		(*pendul).gamma = - dt * dt * gravitation;
		fprintf(stderr, "Erreur penduleInitialiseGamma = %f\n",(*pendul).gamma);
		}
	return;
	}


void penduleAffiche(pendule * pendul)
	{// Affichage de la position et des parametres
	fprintf(stderr, "   ancien    %f\n",(*pendul).ancien);
	fprintf(stderr, "   actuel    %f\n",(*pendul).actuel);
	fprintf(stderr, "   nouveau   %f\n",(*pendul).nouveau);

	fprintf(stderr, "    masse     %f\n",(*pendul).masse);
	fprintf(stderr, "    longueur  %f\n",(*pendul).longueur);
	fprintf(stderr, "    alpha    %f\n",(*pendul).alpha);
	fprintf(stderr, "    kapa     %f\n",(*pendul).kapa);
	fprintf(stderr, "    gamma    %f\n",(*pendul).gamma);

	fprintf(stderr, "forceCouplage %f\n",(*pendul).forceCouplage);
	fprintf(stderr, "  forceTotale %f\n",(*pendul).forceTotale);

	return;
	}

////////////////////////////////////////////////////////////
