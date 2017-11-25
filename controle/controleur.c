/*
Copyright décembre 2017, Stephan Runigo
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

#include "controleur.h"

int controleurTraiteEvenement(controleur * control);
int controleurClavier(controleur * control);
int controleurClavierMaj(controleur * control);
int controleurClavierCtrl(controleur * control);
int controleurSouris(controleur * control);
void controleurBoutonSouris(controleur * control, int appui);
void controleurChangeMode(controleur * control);
void controleurChangeVitesse(controleur * control, float facteur);
void controleurChangeSensibilite(controleur * control, float facteur);

void controleurSimulationGraphique(controleur * control)
	{
			//fprintf(stderr, "Entrée dans la boucle SDL\n");
	do	{
			//fprintf(stderr, "Projection du systeme sur la représentation graphique\n");
		controleurProjection(control);

			//fprintf(stderr, "Projection du systeme sur la représentation graphique\n");
		controleurEvolutionSysteme(control);

			//fprintf(stderr, "Mise à jourde la fenêtre graphique et pause\n");
		controleurConstructionGraphique(control);

			//fprintf(stderr, "Prise en compte des actions clavier\n");
		controleurActionClavier(control);

		}
	while((*control).evenement.type != SDL_QUIT);
			//fprintf(stderr, "Sortiee de la boucle SDL\n");
	return;
	}

int controleurProjection(controleur * control)
	{
		//	Projection des fonctions sur les graphes

	projectionSystemeGraphe(&(*control).system, &(*control).graphCorde);
	projectionFonctionGraphe(&(*control).spectreG, &(*control).graphSpectre, 1);
	projectionFonctionGraphe(&(*control).spectreD, &(*control).graphSpectre, 2);

	//projectionSystemPendule(&(*control).system, &(*control).graph);	SiCP

	return (*control).option.sortie;
	}

int controleurEvolutionSysteme(controleur * control)
	{
		//fprintf(stderr, "Evolution temporelle du system\n");
	systemeEvolution(&(*control).system, (*control).option.duree);

#ifndef SiCP
	//  Calcul des spectres
	projectionSystemeFonction(&(*control).system, &(*control).spectreG, &(*control).spectreD);
	fourierCalcule(&(*control).spectreG, 0, Ne);
	fourierCalcule(&(*control).spectreD, 0, Ne);
#endif

	return 0;
	}

int controleurConstructionGraphique(controleur * control)
	{
		//fprintf(stderr, "Dessin des graphes\n");
	//graphiquePendule(&(*control).graph);	SiCP

	//	Dessin des graphes;
	graphiqueDessineCorde(&(*control).graphCorde);
	graphiqueDessineSpectre(&(*control).graphSpectre);

	//	Incrémentation des graphes;
	grapheIncremente(&(*control).graphCorde);
	grapheIncremente(&(*control).graphSpectre);

		//fprintf(stderr, "Mise à jour de l'affichage\n");
	graphiqueMiseAJour();

		// Pause
	SDL_Delay((*control).option.pause);

		//fprintf(stderr, "Incrémentation des graphes\n");
	//grapheIncremente(&(*control).graph);	SiCP

	return (*control).option.sortie;
	}

int controleurActionClavier(controleur * control)
	{
	int sortie = 0;
		//fprintf(stderr, "Traitement des évenements, mode %d\n", (*control).mode);
	if((*control).option.mode<0)
		{
				// Attente d'un évenement SDL
		if(SDL_WaitEvent(&(*control).evenement))
			{
			sortie += controleurTraiteEvenement(control);
			}
		}
	else
		{
			// Sans attente
		if( SDL_PollEvent(&(*control).evenement) )
			{
			sortie += controleurTraiteEvenement(control);
			}
		}

	if((*control).evenement.type == SDL_QUIT) sortie += 1;

	(*control).option.sortie += sortie;

	return (*control).option.sortie;
	}

int controleurTraiteEvenement(controleur * control)
	{
	int sortie = 0;

	switch((*control).evenement.type)
		{
		case SDL_QUIT:
			sortie = 1;break;
		case SDL_MOUSEMOTION:
			sortie = controleurSouris(control);break;
		case SDL_MOUSEBUTTONDOWN:
			controleurBoutonSouris(control, 1);break;
		case SDL_MOUSEBUTTONUP:
			controleurBoutonSouris(control, 0);break;
		case SDL_KEYDOWN:
			if ((((*control).evenement.key.keysym.mod & KMOD_LSHIFT) == KMOD_LSHIFT)
			|| (((*control).evenement.key.keysym.mod & KMOD_RSHIFT) == KMOD_RSHIFT))
				{
				sortie = controleurClavierMaj(control);break;
				}
			else
				{
				if ((((*control).evenement.key.keysym.mod & KMOD_LSHIFT) == KMOD_LCTRL)
				|| (((*control).evenement.key.keysym.mod & KMOD_RSHIFT) == KMOD_RCTRL))
					{
					sortie = controleurClavierCtrl(control);break;
					}
				else
					{
					sortie = controleurClavier(control);break;
					}
				}
			/*if (((*control).evenement.key.keysym.mod & KMOD_LSHIFT) == KMOD_LSHIFT)
				{sortie = controleurClavierMaj(control);break;}
			else
				{
				if (((*control).evenement.key.keysym.mod & KMOD_RSHIFT) == KMOD_RSHIFT)
					{sortie = controleurClavierMaj(control);break;}
				else {sortie = controleurClavier(control);break;}
				}*/
		//case SDL_KEYDOWN:
			//sortie = controleurClavier(control);break;
		}

	if(sortie != 0)
		{
		fprintf(stderr, "sortie <> 0\n");
		(*control).option.sortie = 1;
		}
	return sortie;
	}

void controleurChangeMode(controleur * control)
	{
	(*control).option.mode=-(*control).option.mode;

	return;
	}

void controleurChangeVitesse(controleur * control, float facteur)
	{
	if( (*control).option.duree > 999 && facteur > 1 )
		{
		fprintf(stderr, "duree maximale atteinte");
		}
	else
		{
		if( (*control).option.duree > 11 )
			{
			(*control).option.duree = (*control).option.duree * facteur;
			}
		else
			{
			if( facteur > 1)
				{
				(*control).option.duree ++;
				}
			else
				{
				if( (*control).option.duree > 1 )
					{
					(*control).option.duree --;
					}
				else
					{
					fprintf(stderr, "duree minimale atteinte");
					}
				}
			}
		}
	fprintf(stderr, "duree = %d\n", (*control).option.duree);
	return;
	}

void controleurChangeSensibilite(controleur * control, float facteur)
	{
	float nouveau = (*control).option.sensibilite * facteur;
	if(facteur > 1 )
		{
		if( (*control).option.sensibilite > 7)
			{
			fprintf(stderr, "sensibilite maximale atteinte");
			}
		else
			{
			(*control).option.sensibilite = nouveau;
			(*control).option.augmente = 1+nouveau;
			(*control).option.diminue = 1/(*control).option.augmente;
			fprintf(stderr, "\nsensibilite = %f \n", (*control).option.sensibilite);
			fprintf(stderr, "  augmente = %f \n", (*control).option.augmente);
			fprintf(stderr, "  diminue = %f \n", (*control).option.diminue);
			}
		}

	if(facteur < 1 )
		{
		if( (*control).option.sensibilite < 0.01)
			{
			fprintf(stderr, "sensibilite minimale atteinte");
			}
		else
			{
			(*control).option.sensibilite = nouveau;
			(*control).option.augmente = 1+nouveau;
			(*control).option.diminue = 1/(*control).option.augmente;
			fprintf(stderr, "\nsensibilite = %f \n", (*control).option.sensibilite);
			fprintf(stderr, "  augmente = %f \n", (*control).option.augmente);
			fprintf(stderr, "  diminue = %f \n", (*control).option.diminue);
			}
		}

	return;
	}

int controleurClavier(controleur * control)
	{
	switch ((*control).evenement.key.keysym.sym)
		{
	// Sortie
		case SDLK_ESCAPE:
			(*control).option.sortie = 1;break;

	// Mode : attente d'un evenement / pas d'attente
		case SDLK_RETURN:
			controleurChangeMode(control);break;

	// Vitesse de la simulation
		case SDLK_KP_PLUS:
			controleurChangeVitesse(control, 1.1);break;
		case SDLK_KP_MINUS:
			controleurChangeVitesse(control, 0.91);break;
		case SDLK_F9:
			controleurChangeVitesse(control, 0.32);break;
		case SDLK_F10:
			controleurChangeVitesse(control, 0.91);break;
		case SDLK_F11:
			controleurChangeVitesse(control, 1.1);break;
		case SDLK_F12:
			controleurChangeVitesse(control, 3.1);break;

	// Conditions aux limites
		case SDLK_y:
			changeDephasage(&(*control).system, 2*PI);break;
		case SDLK_h:
			changeDephasage(&(*control).system, -2*PI);break;
		case SDLK_w:
			changeConditionsLimites(&(*control).system, 0); // periodiques
			break;
		case SDLK_x:
			changeConditionsLimites(&(*control).system, 1); // libres
			break;
		case SDLK_c:
			changeConditionsLimites(&(*control).system, 2); // fixes
			break;
		case SDLK_b:
			changeConditionsLimites(&(*control).system, 3); // libre fixe
			break;
		case SDLK_n:
			changeConditionsLimites(&(*control).system, 4); // fixe libre
			break;


	// Dissipation
		case SDLK_d:
			changeDissipation(&(*control).system, 1.3);break;
		case SDLK_e:
			changeDissipation(&(*control).system, 0.7);break;
		case SDLK_r:
			changeFormeDissipation(&(*control).system, 0);break;
		case SDLK_f:
			changeFormeDissipation(&(*control).system, 1);break;
		case SDLK_v:
			changeFormeDissipation(&(*control).system, 2);break;

	// Couplage
		case SDLK_a:
			changeCouplage(&(*control).system, 1.3);break;
			//changeCouplage(&(*control).system, 1.1);break;
		case SDLK_q:
			changeCouplage(&(*control).system, 0.7);break;
			//changeCouplage(&(*control).system, 0.9);break;

	// Masse
		case SDLK_z:
			changeMasse(&(*control).system, (*control).option.augmente);break;
		case SDLK_s:
			changeMasse(&(*control).system, (*control).option.diminue);break;

	// Gravitation
		case SDLK_t:
			changeGravitation(&(*control).system, (*control).option.augmente);break;
		case SDLK_g:
			changeGravitation(&(*control).system, (*control).option.diminue);break;

	// Moteur jonction Josephson
		case SDLK_UP:
			moteursChangeJosephson(&(*control).system.moteur,1.1);break;
		case SDLK_DOWN:
			moteursChangeJosephson(&(*control).system.moteur,0.91);break;
		case SDLK_LEFT:
			moteursChangeJosephson(&(*control).system.moteur,-1.0);break;
		case SDLK_RIGHT:
			moteursChangeJosephson(&(*control).system.moteur,0.0);break;

	// Moteur générateur de signaux

		case SDLK_p:
			moteursChangeFrequence(&(*control).system.moteur,1.1);break;
		case SDLK_m:
			moteursChangeFrequence(&(*control).system.moteur,0.91);break;
		case SDLK_u:
			moteursChangeAmplitude(&(*control).system.moteur,1.1);break;
		case SDLK_j:
			moteursChangeAmplitude(&(*control).system.moteur,0.91);break;
		case SDLK_o:
			moteursChangeGenerateur(&(*control).system.moteur, -1);break;
		case SDLK_i:
			moteursChangeGenerateur(&(*control).system.moteur, 3);break;
		case SDLK_l:
			moteursChangeGenerateur(&(*control).system.moteur, 2);break;


	// Choix de l'equation

		case SDLK_F1: // Pendules
			changeEquation(&(*control).system, 1);break;
		case SDLK_F2: // Harmoniques
			changeEquation(&(*control).system, 2);break;
		case SDLK_F3: // Corde
			changeEquation(&(*control).system, 3);break;
		case SDLK_F4: // Corde asymétrique
			changeEquation(&(*control).system, 4);break;


	// Choix de l'equation

		case SDLK_F5: // Pendules
			observablesAfficheEnergie(&(*control).system);break;

		case SDLK_F6: // Pendules
			systemeAffiche(&(*control).system);break;

		default:
			;
		}
	return (*control).option.sortie;
	}

int controleurClavierMaj(controleur * control)
	{
	switch ((*control).evenement.key.keysym.sym)
		{

	// Sortie

		case SDLK_ESCAPE:
			(*control).option.sortie = 1;break;

    // Mode : attente d'un evenement / pas d'attente

		case SDLK_RETURN:
			controleurChangeMode(control);break;
		case SDLK_BACKSPACE:
			controleurChangeMode(control);break;


	// Réinitialisation du système
		// Lecture des fichier
		case SDLK_a:
			fprintf(stderr, "Réinitialisation du système\n");
			fichierLecture(&(*control).system, 0);break;
		case SDLK_z:
			fprintf(stderr, "Réinitialisation du système\n");
			fichierLecture(&(*control).system, 1);break;
		case SDLK_e:
			fprintf(stderr, "Réinitialisation du système\n");
			//fichierLecture(&(*control).system, 2);break;
			fichierFonction(&(*control).system, 2);break;//Triangle
		case SDLK_r:
			fprintf(stderr, "Réinitialisation du système\n");
			//fichierLecture(&(*control).system, 3);break;
			fichierFonction(&(*control).system, 3);break;//Triangle
		case SDLK_t:
			fprintf(stderr, "Réinitialisation du système\n");
			//fichierLecture(&(*control).system, 4);break;
			fichierFonction(&(*control).system, 4);break;//Triangle
		case SDLK_y:
			fprintf(stderr, "Réinitialisation du système\n");
			//fichierLecture(&(*control).system, 5);break;
			fichierFonction(&(*control).system, 5);break;//Triangle
		case SDLK_u:
			fprintf(stderr, "Réinitialisation du système\n");
			//fichierLecture(&(*control).system, 6);break;
			fichierFonction(&(*control).system, 6);break;//Triangle
		case SDLK_i:
			fprintf(stderr, "Réinitialisation du système\n");
			//fichierLecture(&(*control).system, 7);break;
			fichierFonction(&(*control).system, 7);break;//Triangle


		case SDLK_q:
			fprintf(stderr, "Réinitialisation du système\n");
			fichierLecture(&(*control).system, 10);break;
		case SDLK_s:
			fprintf(stderr, "Réinitialisation du système\n");
			fichierLecture(&(*control).system, 11);break;
		case SDLK_d:
			fprintf(stderr, "Réinitialisation du système\n");
			fichierLecture(&(*control).system, 12);break;
		case SDLK_f:
			fprintf(stderr, "Réinitialisation du système\n");
			fichierLecture(&(*control).system, 13);break;
		case SDLK_g:
			fprintf(stderr, "Réinitialisation du système\n");
			fichierLecture(&(*control).system, 14);break;
		case SDLK_h:
			fprintf(stderr, "Réinitialisation du système\n");
			fichierLecture(&(*control).system, 15);break;
		case SDLK_j:
			fprintf(stderr, "Réinitialisation du système\n");
			fichierLecture(&(*control).system, 16);break;
		case SDLK_k:
			fprintf(stderr, "Réinitialisation du système\n");
			fichierLecture(&(*control).system, 17);break;

		case SDLK_o:
			//fprintf(stderr, "Réinitialisation du système\n");
			//fichierLecture(&(*control).system, 8);break;
		case SDLK_p:
			//fprintf(stderr, "Réinitialisation du système\n");
			controleurChangeSensibilite(&(*control), 1.1);break;
			//fprintf(stderr, "Réinitialisation du système\n");
			//fichierLecture(&(*control).system, 9);break;
		case SDLK_l:
			//fprintf(stderr, "Réinitialisation du système\n");
			//fichierLecture(&(*control).system, 18);break;
		case SDLK_m:
			controleurChangeSensibilite(&(*control), 0.91);break;
			//fprintf(stderr, "Réinitialisation du système\n");
			//fichierLecture(&(*control).system, 19);break;


		// Ecriture des fichiers
		// Ecriture des fichiers
		case SDLK_w:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 10);break;
		case SDLK_x:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 11);break;
		case SDLK_c:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 12);break;
		case SDLK_v:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 13);break;
		case SDLK_b:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 14);break;
		case SDLK_n:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 15);break;
		/*case SDLK_d:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierLecture(&(*control).system, );break;
		case SDLK_f:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierLecture(&(*control).system, );break;
		case SDLK_g:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierLecture(&(*control).system, );break;
		case SDLK_:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierLecture(&(*control).system, );break;
		case SDLK_:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierLecture(&(*control).system, );break;*/

		default:
			;
		}
	return (*control).option.sortie;
	}

int controleurClavierCtrl(controleur * control)
	{
	switch ((*control).evenement.key.keysym.sym)
		{
	// Sortie
		case SDLK_ESCAPE:
			(*control).option.sortie = 1;break;
	// Mode : attente d'un evenement / pas d'attente
		case SDLK_RETURN:
			controleurChangeMode(control);break;
		case SDLK_BACKSPACE:
			controleurChangeMode(control);break;

	// Enregistrement
		// Sauvegarde du système fichierEcriture sans effet
		case SDLK_a:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 0);break;
		case SDLK_z:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 1);break;
		case SDLK_e:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 2);break;
		case SDLK_r:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 3);break;
		case SDLK_t:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 4);break;
		case SDLK_y:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 5);break;
		case SDLK_u:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 6);break;
		case SDLK_i:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 7);break;
		case SDLK_o:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 8);break;
		case SDLK_p:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 9);break;
		case SDLK_q:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 10);break;
		case SDLK_s:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 11);break;
		case SDLK_d:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 12);break;
		case SDLK_f:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 13);break;
		case SDLK_g:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 14);break;
		case SDLK_h:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 15);break;


		// Ecriture des fichiers
		case SDLK_w:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 10);break;
		case SDLK_x:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 11);break;
		case SDLK_c:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 12);break;
		case SDLK_v:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 13);break;
		case SDLK_b:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 14);break;
		case SDLK_n:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, 15);break;
		/*case SDLK_d:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, );break;
		case SDLK_f:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, );break;
		case SDLK_g:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, );break;
		case SDLK_:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, );break;
		case SDLK_:
			fprintf(stderr, "Sauvegarde du système\n");
			fichierEcriture(&(*control).system, );break;*/

		default:
			;
		}
	return (*control).option.sortie;
	}

int controleurSouris(controleur * control)
	{
	float x, y;
	if((*control).appui==1)
		{
		x=-0.01*(float)((*control).evenement.motion.xrel);
		y=0.1*HAUTEUR*(float)((*control).evenement.motion.yrel);
		//fprintf(stderr, "controleurSouris yrel = %d , x = %f\n", (*control).evenement.motion.yrel, x);
		//fprintf(stderr, "controleurSouris xrel = %d , y = %f\n", (*control).evenement.motion.xrel, y);
		(void)x;(void)y;
		}
	return (*control).option.sortie;
	}

void controleurBoutonSouris(controleur * control, int appui)
	{
	(*control).appui=appui;
	return;
	}
//////////////////////////////////////////////////////////////////////////////////////
