/*
Copyright novembre 2017, Stephan Runigo
runigo@free.fr
SiCF 1.2  simulateur de corde vibrante et spectre
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

#include "change.h"

void changeLimite(systeme * system);
void changeDioptre(systeme * system);

void changeCouplage(systeme * system, float facteur)
	{// Multiplie le couplage par facteur
	int i;
	if((*system).couplage*facteur < COUPLAGE_MAX && (*system).couplage*facteur > COUPLAGE_MIN)
		{
		(*system).couplage=(*system).couplage*facteur;

		for(i=0;i<N;i++)
			{
			penduleChangeCouplage(&(*system).pendul[i], facteur);
			}

		printf("Couplage = %6.3f\n", (*system).couplage);
		}
	else
		{
		printf("Couplage limite atteint : ");
		printf("Couplage = %6.3f\n", (*system).couplage);
		}

	return;
	}

void changeGravitation(systeme * system, float facteur)
	{
	int i;
	if((*system).gravitation*facteur < COUPLAGE_MAX && (*system).gravitation*facteur > COUPLAGE_MIN)
		{

		// Multiplie la gravitation du systeme par facteur
		(*system).gravitation = (*system).gravitation * facteur;

		// Multiplie la gravitation des pendules par facteur
		for(i=0;i<N;i++)
			{
			penduleChangeGravitation(&(*system).pendul[i], facteur);
			}

		printf("Gravitation = %6.3f\n", (*system).gravitation);
		}
	else
		{
		printf("Gravitation limite atteint : ");
		printf("Gravitation = %6.3f\n", (*system).gravitation);
		}

	return;
	}

void changeDioptre(systeme * system)
	{
	int i;
	if((*system).equation==4)
		{
		for(i=N/2;i<N;i++)
			{
			penduleReinitialiseMasse(&(*system).pendul[i], (*system).masseDroite, (*system).gravitation, (*system).moteur.dt);
			}
		}
	else
		{
		for(i=0;i<N;i++)
			{
			penduleReinitialiseMasse(&(*system).pendul[i], (*system).masseGauche, (*system).gravitation, (*system).moteur.dt);
			}
		}
	return;
	}
void changeMasse(systeme * system, float facteur)
	{
	float droite = (*system).masseDroite;
	float gauche = (*system).masseGauche;
	if((*system).equation==4)
		{
		float masse = droite*facteur;
		if(masse < MASSE_MAX && masse > MASSE_MIN)
			{
			(*system).masseDroite=masse;
			changeDioptre(system);
			printf("Masse à gauche = %6.3f, masse à droite = %6.3f\n", (*system).masseGauche, masse);
			}
		else
			{
			printf("Masse limite atteinte\n");
			}
		}
	else
		{
		float masse = gauche*facteur;
		if(masse < MASSE_MAX && masse > MASSE_MIN)
			{
			(*system).masseGauche=masse;
			changeDioptre(system);
			printf("Masse = %6.3f\n", masse);
			}
		else
			{
			printf("Masse limite atteinte\n");
			}
		}

	return;
	}

void changeDissipation(systeme * system, float facteur)
	{
	int i;

	float dissipation = (*system).dissipation * facteur;
	float dissipationMaximale = DISSIPATION_MAX_DT/(*system).moteur.dt;

	if(dissipation < dissipationMaximale && dissipation > DISSIPATION_MIN)
		{
		// Multiplie la dissipation du systeme par facteur <> 0
		// Conserve en mémoire la dissipation du systeme si = 0
		if(facteur!=0.0)
			{
			(*system).dissipation = (*system).dissipation * facteur;
			}
		// Multiplie la dissipation des pendules par facteur
		for(i=0;i<N;i++)
			{
			penduleChangeDissipation(&(*system).pendul[i], facteur);
			}
		}
	else
		{
		printf("dissipation limite atteinte\n");
		}
	printf("Dissipation = %6.3f\n", (*system).dissipation);

	return;
	}

void changeFormeDissipation(systeme * system, int forme)
	{// initialise une dissipation nulle(0), uniforme(1) ou extremite absorbante(2)
	float dissipation = 0.99;

	if ( forme == 0 )
		dissipation = 0.0;

	if (  forme == 1 )
		{
		if ( (*system).dissipation != 0.0 )
			{
			dissipation = (*system).dissipation;
			}
		else
			{
			(*system).dissipation = dissipation;
			}
		}

	// Reinitialisation de alpha
	int i;
	for(i=0;i<N;i++)
		{
		if ( forme == 2 )
			{
			dissipation = (*system).pendul[i].dissipation;
			}
		penduleInitialiseAlpha(&(*system).pendul[i], dissipation, (*system).moteur.dt);
		}

	if ( forme == 2 )
		{
		printf("Dissipation premier= %6.3f\n", (*system).pendul[0].dissipation);
		printf("Dissipation dernier= %6.3f\n", (*system).pendul[N-1].dissipation);
		}
	else
		{ // Cas uniforme (forme=1)
		printf("Dissipation = %6.3f\n", dissipation);
		}

	return;
	}

void changeEquation(systeme * system, int equation)
	{// Choix de l'equation

	switch(equation)
		{
		case 1: // Pendules
			(*system).equation = 1;
			printf("equation = Pendules\n");break;
		case 2: // Harmoniques
			(*system).equation = 2;
			printf("equation = Harmoniques\n");break;
		case 3: // Corde
			(*system).equation = 3;
			changeDioptre(system);
			printf("equation = Corde\n");break;
		case 4: // Corde asymétrique
			(*system).equation = 4;
			changeDioptre(system);
			printf("equation = Corde asymétrique\n");break;
		default:
			;
		}

	printf("equation = %d \n\n", (*system).equation);
	return;
	}

void changeConditionsLimites(systeme * system, int libreFixe)
	{
	(*system).libreFixe=libreFixe;
	printf("libreFixe = %d, ", (*system).libreFixe);

	switch(libreFixe)
		{
		case 0:
			printf(": conditions aux limites périodiques\n");
			break;
		case 1:
			printf(": extrémités libres\n");
			break;
		case 2:
			printf(": extrémités fixes\n");
			break;
		case 3:
			printf(": premier libre, dernier fixe\n");
			break;
		case 4:
			printf(": premier fixe, dernier libre\n");
			break;
		default:
			;
		}

	changeLimite(system);

	return;
	}

void changeLimite(systeme * system)
	{// Initialise les conditions aux limites
	float couplage=0.0;
	if ((*system).libreFixe==0 || (*system).libreFixe==2)
		{
		couplage=(*system).couplage;
		}

	penduleInitialiseKapa(&(*system).pendul[N-1], couplage, (*system).moteur.dt);

	printf("Couplage dernier = %6.3f\n", couplage);

	return;
	}

void changeFormeDioptre(systeme * system, float facteur)
	{
	int i;
	for(i=N/2;i<N;i++)
		{
		penduleChangeMasse(&((*system).pendul[i]), facteur);
		}
	return;
	}

void changeDephasage(systeme * system, float dephasage)
	{
	penduleAjouteDephasage(&(*system).pendul[N-1], dephasage);
	}

/*--------------------------------------------------------------*/

//////////////////////////////////////////////////////////////////////////
