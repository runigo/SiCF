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

#include "observables.h"

double observablesEnergieCinetiquePendule(pendule * pendul, double dt);
double observablesEnergieGravitationPendule(pendule * pendul, double dt);
double observablesEnergieCouplagePendule(pendule * pendul, pendule * suivant, double dt);
double observablesEnergieCinetiqueSysteme(systeme * system);
double observablesEnergieGravitationSysteme(systeme * system);
double observablesEnergieCouplageSysteme(systeme * system);

double observablesEnergieCinetiquePendule(pendule * pendul, double dt)
	{
	double vitesse;
	vitesse = (*pendul).actuel - (*pendul).ancien;
	return (0.5 * (*pendul).masse * vitesse*vitesse/dt/dt);
	}

double observablesEnergieGravitationPendule(pendule * pendul, double dt)
	{
	double h;
	h=(1.0-cos((*pendul).actuel))*(*pendul).longueur;
	return ( - (*pendul).masse * (*pendul).gamma * h / dt / dt);
	}

double observablesEnergieCouplagePendule(pendule * pendul, pendule * suivant, double dt)
	{
	double dx;
	dx = (*pendul).actuel - (*suivant).actuel;
	return ( - 0.5 * (*pendul).masse * (*pendul).kapa * dx * dx / dt / dt);
	}

double observablesEnergieCinetiqueSysteme(systeme * system)
	{
	int i;
	double ectotal=0.0;
	float dt = (*system).moteur.dt;
	for(i=0;i<N;i++)
		{
		ectotal=ectotal+observablesEnergieCinetiquePendule(&((*system).pendul[i]), dt);
		}
	return ectotal;
	}

double observablesEnergieCouplageSysteme(systeme * system)
	{
	int i;
	double ectotal=0.0;
	float dt = (*system).moteur.dt;
	for(i=1;i<N;i++)
		{
		ectotal=ectotal+observablesEnergieCouplagePendule(&((*system).pendul[i-1]), &((*system).pendul[i]), dt);
		}
	if((*system).libreFixe==0)
		ectotal=ectotal+observablesEnergieCouplagePendule(&((*system).pendul[N-1]), &((*system).pendul[0]), dt);
	return ectotal;
	}

double observablesEnergieGravitationSysteme(systeme * system)
	{
	int i;
	double ectotal=0.0;
	float dt = (*system).moteur.dt;
	for(i=0;i<N;i++)
		{
		ectotal=ectotal+observablesEnergieGravitationPendule(&((*system).pendul[i]), dt);
		}
	return ectotal;
	}

void observablesAfficheEnergie(systeme * system)
	{
	double eCouplage, eGravitation, eCinetique;

		//printf("\nCalcul de l'énergie cinétique\n");
	eCinetique=observablesEnergieCinetiqueSysteme(system);
	printf("\n  Energie cinétique    %12.9f\n", eCinetique);

		//printf("Calcul de l'énergie de couplage\n");
	eCouplage=observablesEnergieCouplageSysteme(system);
	printf("  Energie de couplage  %12.9f\n", eCouplage);

		//printf("Calcul de l'énergie de gravitation\n");
	eGravitation=observablesEnergieGravitationSysteme(system);
	printf("  Energie de gravitation  %12.9f\n", eGravitation);

	printf("\nEnergie totale      %12.9f\n\n", eCouplage + eGravitation + eCinetique);
	return;
	}
//////////////////////////////////////////////////////////////////
