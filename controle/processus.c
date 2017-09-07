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

#include "processus.h"

pthread_cond_t conditionSysteme = PTHREAD_COND_INITIALIZER; /* Création de la condition */
pthread_mutex_t mutexSysteme = PTHREAD_MUTEX_INITIALIZER; /* Création du mutex */

void* processusCondition (void* control);
void* processusAttente (void* control);

int processusSimulationGraphique (controleur * control)
{
	pthread_t processusC;
	pthread_t processusA;

	pthread_create (&processusC, NULL, processusCondition, (void *) control);
	pthread_create (&processusA, NULL, processusAttente, (void *) control); /* Création des threads */

	pthread_join (processusC, NULL);
	pthread_join (processusA, NULL); /* Attente de la fin des threads */

	return 0;
}


void* processusCondition (void* control)
	{
	int sortie=0;
	do
		{
			//printf("\n Verrouillage dans threadCondition");
		pthread_mutex_lock (&mutexSysteme);

			//printf("\n Fonctions du controleur");
		sortie += controleurEvolutionSysteme(control);
		sortie += controleurProjection(control);

			//printf("\nSignal");
		pthread_cond_signal (&conditionSysteme);

			//printf("\n Déverrouillage dans threadCondition");
		pthread_mutex_unlock (&mutexSysteme);
		}
	while(sortie==0);
	printf("\n Sortie du processus condition\n");
	pthread_exit(NULL); /* Fin du thread */	
	}

void* processusAttente (void* control)
	{
	int sortie=0;
	//while(1) /* Boucle infinie */
	do
		{
			//printf("\n Verrouillage dans threadAttente");
		pthread_mutex_lock(&mutexSysteme);

			//printf("\n Fonctions du controleur");
		sortie += controleurActionClavier(control);
		sortie += controleurConstructionGraphique(control);

			//printf("\nAttente");
		pthread_cond_wait (&conditionSysteme, &mutexSysteme);

			//printf("\n Déverrouillage dans threadAttente"); 
		pthread_mutex_unlock(&mutexSysteme);
		}
	while(sortie==0);
	printf("\n Sortie du processus attente\n");
	pthread_exit(NULL); /* Fin du thread */
	}
/////////////////////////////////////////////////////////////////
