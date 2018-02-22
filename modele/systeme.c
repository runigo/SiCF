/*
Copyright février 2018, Stephan Runigo
runigo@free.fr
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

#include "systeme.h"

void systemeInitialisePendul(systemeT * systeme);
void systemeCouplage(systemeT * systeme);
void systemeInertie(systemeT * systeme);
void systemeIncremente(systemeT * systeme);
void systemeInitialiseLimiteInfini(systemeT * systeme);
void systemeChangeLimite(systemeT * systeme);
void systemeFormeDioptre(systemeT * systeme, float facteur);
float systemeMoteur(systemeT * systeme);

// normalise le premier pendule
void systemeJaugeZero(systemeT * systeme);
// normalise la moyenne du système à zéro
void systemeJauge(systemeT * systeme);
double systemeMoyenne(systemeT * systeme);



/*----------------JAUGE ET NORMALISATION-------------------*/

// normalise le premier pendule
void systemeJaugeZero(systemeT * systeme)
	{
	float position = (*systeme).pendule[0].nouveau;
	float jauge;

	//fprintf(stderr, "\nEntrée systemeJaugeZero, position = %f\n", position);
	//fprintf(stderr, "\nEntrée systemeJaugeZero, position/DEUXPI = %f\n", position/DEUXPI);

	jauge = -DEUXPI*(int)(position/DEUXPI);
	if(jauge>PI || jauge<PI)
		{
		int i;
		for(i=1;i<N;i++)
			{
			penduleJauge(&(*systeme).pendule[i], jauge);
			}
		}


	//fprintf(stderr, "Moyenne = %f\n", systemeMoyenne(systeme));
	//fprintf(stderr, "systemeJaugeZero, sortie\n\n");
	return;
	}


// normalise la moyenne du système à zéro
void systemeJauge(systemeT * systeme)
	{
	double moyenne = systemeMoyenne(systeme);
	if(moyenne>(MOYENNE_MAX))
		{
		int i;
		for(i=0;i<N;i++)
			{
			penduleJauge(&(*systeme).pendule[i], -MOYENNE_MAX);
			}
		fprintf(stderr, "Moyenne = %f\n", moyenne);
		}
	if(moyenne<(-MOYENNE_MAX))
		{
		int i;
		for(i=0;i<N;i++)
			{
			penduleJauge(&(*systeme).pendule[i], MOYENNE_MAX);
			}
		fprintf(stderr, "Moyenne = %f\n", moyenne);
		}
	return;
	}

double systemeMoyenne(systemeT * systeme)
	{
	double moyenne = 0.0;

	int i;
	for(i=0;i<N;i++)
		{
		moyenne = moyenne + (*systeme).pendule[i].nouveau;
		}

	return ( moyenne / N );
	}

void systemeInitialise(systemeT * systeme)
	{
	// Initialisation des pendules
	systemeInitialisePendul(systeme);

	// Initialise les conditions aux limites
	//systemeChangeLimite(systeme);

	// Extrémité absorbante
	systemeInitialiseLimiteInfini(systeme);
/*
	// Dissipation initiale Corde
	if((*systeme).equation == 3 || (*systeme).equation == 4)
		{
		changeFormeDissipation(systeme, 2);
		}
*/

	// Masse initiale Dioptre
	if((*systeme).equation == 4)
		{
	int i;
			for(i=N/2;i<N;i++)
				{
				penduleReinitialiseMasse(&(*systeme).pendule[i], (*systeme).masseDroite, (*systeme).moteur.dt);
				}
		}
	//printf("Systeme initialisé\n");

	return;
	}

void systemeEvolution(systemeT * systeme, int duree)
	{//	Fait évoluer le système pendant duree * dt
	int i;
	for(i=0;i<duree;i++)
		{
		systemeCouplage(systeme);
		systemeInertie(systeme);
		systemeIncremente(systeme);
		}
	systemeJaugeZero(systeme);
	return;
	}

void systemeInitialisePendul(systemeT * systeme)
	{
	float m=(*systeme).masseGauche;
	float l=(*systeme).longueur;
	float d=(*systeme).dissipation;
	float c=(*systeme).couplage;
	float g=(*systeme).gravitation;
	float t=(*systeme).moteur.dt;

	int i;

	for(i=0;i<N;i++)
		{
		penduleInitialiseParametre(&(*systeme).pendule[i], m, l, d);
		penduleInitialiseExterieur(&(*systeme).pendule[i], c, g, t);
		penduleInitialisePosition(&(*systeme).pendule[i], 0.0, 0.0);
		penduleInitialiseDephasage(&(*systeme).pendule[i], 0.0);
		}

	penduleInitialiseDephasage(&(*systeme).pendule[0], (*systeme).dephasage);

	return;
	}

void systemeInitialiseLimiteInfini(systemeT * systeme)
	{
	int i;

	for(i=0;i<N;i++)
		{
		(*systeme).pendule[i].dissipation = 0.0;
		}

	for(i=1;i<N/6;i++)
		{
		(*systeme).pendule[N-i].dissipation = 10*(1.0-i/(float)(N/6));
		}


	return;
	}

void systemeCouplage(systemeT * systeme)
	{//	Calcul des forces de couplage

  // SiCF-1.0 inaugure penduleCouplage(m0, m0, m1)
  // nécessitant un dephasage nul et traitant l'extrémité libre
	int libreFixe = (*systeme).libreFixe;

	int i;
	for(i=1;i<(N-1);i++)
		{
		penduleCouplage(&(*systeme).pendule[i-1], &(*systeme).pendule[i], &(*systeme).pendule[i+1]);
		}

  if(libreFixe!=0)
    {
    //penduleInitialiseDephasage(&(*systeme).pendule[1], 0.0);
  	penduleCouplage(&(*systeme).pendule[0], &(*systeme).pendule[0], &(*systeme).pendule[1]);
	  penduleCouplage(&(*systeme).pendule[N-2], &(*systeme).pendule[N-1], &(*systeme).pendule[N-1]);
	  }
  else // Limites périodiques
    {
  	penduleCouplage(&(*systeme).pendule[N-1], &(*systeme).pendule[0], &(*systeme).pendule[1]);
	  penduleCouplage(&(*systeme).pendule[N-2], &(*systeme).pendule[N-1], &(*systeme).pendule[0]);
	  }

	return;
	}

void systemeInertie(systemeT * systeme)
	{//	Principe d'inertie applique au système

  // Version SiCF-1.0, extrémité moteur fixe.

	int equation = (*systeme).equation;
	int libreFixe = (*systeme).libreFixe;
	int etatMoteur = (*systeme).moteur.generateur;
	float courantJosephson = (*systeme).moteur.josephson;

	float moteur = moteursGenerateur(&(*systeme).moteur);

				// Cas du premier et du dernier pendule

			//  0 : periodiques 1 : libres, 2 : fixes, 3 libre-fixe, 4 fixe-libre

	if (libreFixe==0 || libreFixe==1 || libreFixe==3)
		{
		penduleInertie(&(*systeme).pendule[0], equation, courantJosephson);
		}
	else
		{
		penduleInitialisePosition(&(*systeme).pendule[0], 0.0, 0.0);
		}
	if (libreFixe==0 || libreFixe==1 || libreFixe==4)
		{
		penduleInertie(&(*systeme).pendule[N-1], equation, courantJosephson);
		}
	else
		{
		penduleInitialisePosition(&(*systeme).pendule[N-1], (*systeme).pendule[N-1].dephasage, (*systeme).pendule[N-1].dephasage);
		}

	if(etatMoteur!=0) // moteur allumé
		{
		penduleInitialisePosition(&(*systeme).pendule[0], moteur, moteur);
		}
	/*else // SiCF-1.0 : extrémité moteur fixe
		{
		penduleInitialisePosition(&(*systeme).pendule[0], 0, 0);
		}*/

						// Cas des autres pendules
	int i;
	for(i=1;i<(N-1);i++)
		{
		penduleInertie(&((*systeme).pendule[i]), (*systeme).equation, courantJosephson);
		}
	return;
	}

void systemeIncremente(systemeT * systeme)
	{//	incremente l'horloge, l'ancien et l'actuel etat du système

	//(*systeme).moteur.horloge = (*systeme).moteur.horloge + (*systeme).moteur.dt;
	(*systeme).moteur.chrono = (*systeme).moteur.chrono + (*systeme).moteur.dt;

	int i;
	for(i=0;i<N;i++)
		{
		penduleIncremente(&((*systeme).pendule[i]));
		}

	return;
	}

void systemeAffiche(systemeT * systeme)
	{// Affichage de la position et des parametres
	printf("\nParamètres système\n");
	printf("	Couplage entre les pendules	%4.3f\n", (*systeme).couplage);
	printf("	Longueur des pendules		%4.3f\n",(*systeme).longueur);
	printf("	Intensité de la gravitation	%4.3f\n",(*systeme).gravitation);
	printf("	Masse des pendules à droite : %4.3f, à gauche : %4.3f\n",(*systeme).masseDroite,(*systeme).masseGauche);
	printf("	Masse des pendules \n");
	/*	int equation;		//	Pendule=1, Harmonique=2, Corde=3, Dioptre=4
		float dephasage;	//	déphasage entre les limites
		int libreFixe;		// 0 : périodiques 1 : libres, 2 : fixes, 
	*/					//		3 libre-fixe, 4 fixe-libre
	return;
	}

//////////////////////////////////////////////////////////////////////
