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

#include "systeme.h"

void systemeInitialisePendul(systeme * system);
void systemeCouplage(systeme * system);
void systemeInertie(systeme * system);
void systemeIncremente(systeme * system);
void systemeInitialiseLimiteInfini(systeme * system);
void systemeChangeLimite(systeme * system);
void systemeFormeDioptre(systeme * system, float facteur);
float systemeMoteur(systeme * system);

// normalise la moyenne du systeme à zéro
void systemeJauge(systeme * system);
double systemeMoyenne(systeme * system);




// normalise la moyenne du système à zéro
void systemeJauge(systeme * system)
	{
	double moyenne = systemeMoyenne(system);
	if(moyenne>(MOYENNE_MAX))
		{
		int i;
		for(i=0;i<N;i++)
			{
			penduleJauge(&(*system).pendul[i], -MOYENNE_MAX);
			}
		fprintf(stderr, "Moyenne = %f\n", moyenne);
		}
	if(moyenne<(-MOYENNE_MAX))
		{
		int i;
		for(i=0;i<N;i++)
			{
			penduleJauge(&(*system).pendul[i], MOYENNE_MAX);
			}
		fprintf(stderr, "Moyenne = %f\n", moyenne);
		}
	return;
	}

double systemeMoyenne(systeme * system)
	{
	double moyenne = 0.0;

	int i;
	for(i=0;i<N;i++)
		{
		moyenne = moyenne + (*system).pendul[i].nouveau;
		}

	return ( moyenne / N );
	}

void systemeInitialise(systeme * system)
	{
	// Initialisation des pendules
	systemeInitialisePendul(system);

	// Initialise les conditions aux limites
	//systemeChangeLimite(system);

	// Extrémité absorbante
	systemeInitialiseLimiteInfini(system);
/*
	// Dissipation initiale Corde
	if((*system).equation == 3 || (*system).equation == 4)
		{
		changeFormeDissipation(system, 2);
		}

	// Masse initiale Dioptre
	if((*system).equation == 4)
		{
		changeFormeDioptre(system, 1.99);
		}
*/
	printf("Systeme initialisé\n");
	printf("	Couplage = %6.3f\n", (*system).couplage);
	printf("	Gravitation = %6.3f\n", (*system).gravitation);

	return;
	}

void systemeEvolution(systeme * system, int duree)
	{//	Fait évoluer le systeme pendant duree * dt
	int i;
	for(i=0;i<duree;i++)
		{
		systemeCouplage(system);
		systemeInertie(system);
		systemeIncremente(system);
		}
	systemeJauge(system);
	return;
	}

void systemeInitialisePendul(systeme * system)
	{
	float m=(*system).masse;
	float l=(*system).longueur;
	float d=(*system).dissipation;
	float c=(*system).couplage;
	float g=(*system).gravitation;
	float t=(*system).moteur.dt;

	int i;

	for(i=0;i<N;i++)
		{
		penduleInitialiseParametre(&(*system).pendul[i], m, l, d);
		penduleInitialiseExterieur(&(*system).pendul[i], c, g, t);
		penduleInitialisePosition(&(*system).pendul[i], 0.0, 0.0);
		penduleInitialiseDephasage(&(*system).pendul[i], 0.0);
		}

	penduleInitialiseDephasage(&(*system).pendul[0], (*system).dephasage);

	return;
	}

void systemeInitialiseLimiteInfini(systeme * system)
	{
	int i;

	for(i=0;i<N;i++)
		{
		(*system).pendul[i].dissipation = 0.0;
		}

	for(i=1;i<N/6;i++)
		{
		(*system).pendul[N-i].dissipation = 10*(1.0-i/(float)(N/6));
		}


	return;
	}

void systemeCouplage(systeme * system)
	{//	Calcul des forces de couplage

  // SiCF-1.0 inaugure penduleCouplage(m0, m0, m1)
  // nécessitant un dephasage nul et traitant l'extrémité libre
	int libreFixe = (*system).libreFixe;

	int i;
	for(i=1;i<(N-1);i++)
		{
		penduleCouplage(&(*system).pendul[i-1], &(*system).pendul[i], &(*system).pendul[i+1]);
		}

  if(libreFixe!=0)
    {
    //penduleInitialiseDephasage(&(*system).pendul[1], 0.0);
  	penduleCouplage(&(*system).pendul[0], &(*system).pendul[0], &(*system).pendul[1]);
	  penduleCouplage(&(*system).pendul[N-2], &(*system).pendul[N-1], &(*system).pendul[N-1]);
	  }
  else // Limites périodiques
    {
  	penduleCouplage(&(*system).pendul[N-1], &(*system).pendul[0], &(*system).pendul[1]);
	  penduleCouplage(&(*system).pendul[N-2], &(*system).pendul[N-1], &(*system).pendul[0]);
	  }

	return;
	}

void systemeInertie(systeme * system)
	{//	Principe d'inertie applique au systeme

  // Version SiCF-1.0, extrémité moteur fixe.

	int equation = (*system).equation;
	int libreFixe = (*system).libreFixe;
	int etatMoteur = (*system).moteur.generateur;
	float courantJosephson = (*system).moteur.josephson;

	float moteur = moteursGenerateur(&(*system).moteur);

	// Cas du premier et du dernier pendule
	//  0 : periodiques 1 : libres, 2 : fixes, 3 libre-fixe, 4 fixe-libre

	if (libreFixe==0 || libreFixe==1 || libreFixe==3)
		{
		//penduleInertie(&(*system).pendul[0], equation, courantJosephson + moteur * (*system).moteur.dt * (*system).moteur.dt);
		penduleInertie(&(*system).pendul[0], equation, courantJosephson);
		}
	if (libreFixe==0 || libreFixe==1 || libreFixe==4)
		{
		penduleInertie(&(*system).pendul[N-1], equation, courantJosephson);
		}

	if(etatMoteur!=0) // moteur allumé
		{
		penduleInitialisePosition(&(*system).pendul[0], moteur, moteur);
		}
	/*else // SiCF-1.0 : extrémité moteur fixe
		{
		penduleInitialisePosition(&(*system).pendul[0], 0, 0);
		}*/

						// Cas des autres pendules
	int i;
	for(i=1;i<(N-1);i++)
		{
		penduleInertie(&((*system).pendul[i]), (*system).equation, courantJosephson);
		}
	return;
	}

void systemeIncremente(systeme * system)
	{//	incremente l'horloge, l'ancien et l'actuel etat du systeme

	(*system).moteur.horloge = (*system).moteur.horloge + (*system).moteur.dt;
	(*system).moteur.chrono = (*system).moteur.chrono + (*system).moteur.dt;

	int i;
	for(i=0;i<N;i++)
		{
		penduleIncremente(&((*system).pendul[i]));
		}

	return;
	}

//////////////////////////////////////////////////////////////////////
