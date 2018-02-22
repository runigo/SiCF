/*
Copyright février 2018, Stephan Runigo
runigo@free.fr
SiCF 1.2.3  simulateur de corde vibrante et spectre
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

void changeLimite(systemeT * systeme);
void changeDioptre(systemeT * systeme);

void changeCouplage(systemeT * systeme, float facteur)
	{// Multiplie le couplage par facteur
	int i;
	float nouveau = (*systeme).couplage*facteur;
	if(facteur > 1)
		{
		if(nouveau < COUPLAGE_MAX)
			{
			(*systeme).couplage=nouveau;
			for(i=0;i<N;i++)
				{
				penduleChangeCouplage(&(*systeme).pendule[i], facteur);
				}

			printf("Couplage = %6.3f\n", (*systeme).couplage);
			}
		else
			{
			printf("Couplage maximal atteint : ");
			printf("Couplage = %6.3f\n", (*systeme).couplage);
			}
		}

	if(facteur < 1 )
		{
		if(nouveau > COUPLAGE_MIN)
			{
			(*systeme).couplage=nouveau;
			for(i=0;i<N;i++)
				{
				penduleChangeCouplage(&(*systeme).pendule[i], facteur);
				}

			printf("Couplage = %6.3f\n", (*systeme).couplage);
			}
		else
			{
			printf("Couplage minimal atteint : ");
			printf("Couplage = %6.3f\n", (*systeme).couplage);
			}
		}

	return;
	}

void changeGravitation(systemeT * systeme, float facteur)
	{
	int i;
	float nouveau = (*systeme).gravitation*facteur;
	if(facteur > 1)
		{
		if(nouveau < GRAVITATION_MAX)
			{
			(*systeme).gravitation=nouveau;
			for(i=0;i<N;i++)
				{
				penduleChangeGravitation(&(*systeme).pendule[i], facteur);
				}

			printf("Gravitation = %6.3f\n", (*systeme).gravitation);
			}
		else
			{
			printf("Gravitation maximal atteint : ");
			printf("gravitation = %6.3f\n", (*systeme).gravitation);
			}
		}

	if(facteur < 1 )
		{
		if(nouveau > GRAVITATION_MIN)
			{
			(*systeme).gravitation=nouveau;
			for(i=0;i<N;i++)
				{
				penduleChangeGravitation(&(*systeme).pendule[i], facteur);
				}

			printf("Gravitation = %6.3f\n", (*systeme).couplage);
			}
		else
			{
			printf("Gravitation minimal atteint : ");
			printf("gravitation = %6.3f\n", (*systeme).couplage);
			}
		}

	return;
	}

void changeDioptre(systemeT * systeme)
	{
	int i;
	if((*systeme).equation==4)
		{
		for(i=N/2;i<N;i++)
			{
			penduleReinitialiseMasse(&(*systeme).pendule[i], (*systeme).masseDroite, (*systeme).moteur.dt);
			}
		}
	else
		{
		for(i=0;i<N;i++)
			{
			penduleReinitialiseMasse(&(*systeme).pendule[i], (*systeme).masseGauche, (*systeme).moteur.dt);
			}
		}
	return;
	}
void changeMasse(systemeT * systeme, float facteur)
	{
	float droite = (*systeme).masseDroite;
	float gauche = (*systeme).masseGauche;
	if((*systeme).equation==4)
		{
		float masse = droite*facteur;
		if(masse < MASSE_MAX && masse > MASSE_MIN)
			{
			(*systeme).masseDroite=masse;
			changeDioptre(systeme);
			printf("Masse à gauche = %6.3f, masse à droite = %6.3f\n", (*systeme).masseGauche, masse);
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
			(*systeme).masseGauche=masse;
			changeDioptre(systeme);
			printf("Masse = %6.3f\n", masse);
			}
		else
			{
			printf("Masse limite atteinte\n");
			}
		}

	return;
	}

void changeDissipation(systemeT * systeme, float facteur)
	{
	int i;

	float dissipation = (*systeme).dissipation * facteur;
	float dissipationMaximale = DISSIPATION_MAX_DT/(*systeme).moteur.dt;

	if(dissipation < dissipationMaximale && dissipation > DISSIPATION_MIN)
		{
		// Multiplie la dissipation du système par facteur <> 0
		// Conserve en mémoire la dissipation du système si = 0
		if(facteur!=0.0)
			{
			(*systeme).dissipation = (*systeme).dissipation * facteur;
			}
		// Multiplie la dissipation des pendules par facteur
		for(i=0;i<N;i++)
			{
			penduleChangeDissipation(&(*systeme).pendule[i], facteur);
			}
		}
	else
		{
		printf("dissipation limite atteinte\n");
		}
	printf("Dissipation = %6.3f\n", (*systeme).dissipation);

	return;
	}

void changeFormeDissipation(systemeT * systeme, int forme)
	{// initialise une dissipation nulle(0), uniforme(1) ou extremite absorbante(2)
	float dissipation = 0.99;

	if ( forme == 0 )
		dissipation = 0.0;

	if (  forme == 1 )
		{
		if ( (*systeme).dissipation != 0.0 )
			{
			dissipation = (*systeme).dissipation;
			}
		else
			{
			(*systeme).dissipation = dissipation;
			}
		}

	// Reinitialisation de alpha
	int i;
	for(i=0;i<N;i++)
		{
		if ( forme == 2 )
			{
			dissipation = (*systeme).pendule[i].dissipation;
			}
		penduleInitialiseAlpha(&(*systeme).pendule[i], dissipation, (*systeme).moteur.dt);
		}

	if ( forme == 2 )
		{
		printf("Dissipation premier= %6.3f\n", (*systeme).pendule[0].dissipation);
		printf("Dissipation dernier= %6.3f\n", (*systeme).pendule[N-1].dissipation);
		}
	else
		{ // Cas uniforme (forme=1)
		printf("Dissipation = %6.3f\n", dissipation);
		}

	return;
	}

void changeEquation(systemeT * systeme, int equation)
	{// Choix de l'equation

	switch(equation)
		{
		case 1: // Pendules
			(*systeme).equation = 1;
			printf("equation = Pendules\n");break;
		case 2: // Harmoniques
			(*systeme).equation = 2;
			printf("equation = Harmoniques\n");break;
		case 3: // Corde
			(*systeme).equation = 3;
			changeDioptre(systeme);
			printf("equation = Corde\n");break;
		case 4: // Corde asymétrique
			(*systeme).equation = 4;
			changeDioptre(systeme);
			printf("equation = Corde asymétrique\n");break;
		default:
			;
		}

	printf("equation = %d \n\n", (*systeme).equation);
	return;
	}

void changeConditionsLimites(systemeT * systeme, int libreFixe)
	{
	(*systeme).libreFixe=libreFixe;
	printf("libreFixe = %d, ", (*systeme).libreFixe);

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

	changeLimite(systeme);

	return;
	}

void changeLimite(systemeT * systeme)
	{// Initialise les conditions aux limites
	float couplage=0.0;
	if ((*systeme).libreFixe==0 || (*systeme).libreFixe==2)
		{
		couplage=(*systeme).couplage;
		}

	penduleInitialiseKapa(&(*systeme).pendule[N-1], couplage, (*systeme).moteur.dt);

	printf("Couplage dernier = %6.3f\n", couplage);

	return;
	}

void changeFormeDioptre(systemeT * systeme, float facteur)
	{
	int i;
	for(i=N/2;i<N;i++)
		{
		penduleChangeMasse(&((*systeme).pendule[i]), facteur);
		}
	return;
	}

void changeDephasage(systemeT * systeme, float dephasage)
	{
	penduleAjouteDephasage(&(*systeme).pendule[N-1], dephasage);

	printf("Déphasage dernier = %6.3f\n", (*systeme).pendule[N-1].dephasage);

	}

/*--------------------------------------------------------------*/

//////////////////////////////////////////////////////////////////////////
