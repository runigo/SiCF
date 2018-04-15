/*
Copyright avril 2018, Stephan Runigo
runigo@free.fr
SiCF 1.4  simulateur de corde vibrante et spectre
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

// Librement inspiré de 
// http://piconano2015.wixsite.com/soft/code
// Copyright 2015 par PicoSoft.

#include "horloge.h"

Uint32 callTimer(Uint32 it, void *para);

int horlogeCreation(horlogeT * horloge)
	{
		//fprintf(stderr, " Initialisation du timer, fond = %d\n", fond);
		// définition d'un User Event
	(*horloge).evenement.type=SDL_USEREVENT;

		// Lancement du Timer principal
	//(*horloge).horloge = SDL_AddTimer(TEMPS_AFFICHAGE, horlogeEvenement, horloge);
	(*horloge).horloge = SDL_AddTimer(TEMPS_AFFICHAGE, callTimer, horloge);

		//int *parametre;

	(*horloge).date = 0;          // la référence de horloge du programme (nombre de période timer principal)
	(*horloge).dateActuel = 0;          // 
	(*horloge).datePrecedente = 0;         //

	return 0;
	}

Uint32 callTimer(Uint32 it, void *para)
{   // Callback du timer principal
    // on créé un event pour passer le wait
SDL_Event user_event;
    // définition d'un User Event
    user_event.type=SDL_USEREVENT;
    SDL_PushEvent(&user_event);
	(void) para;
    return it;
}

/*
Uint32 horlogeEvenement(Uint32 it, horlogeT * horloge)
	{   // Rappel automatique du timer principal
		// on crée un évenement pour passer le wait
	SDL_PushEvent(&(*horloge).evenement);

	return it;
	}
*/
int horlogeSuppression(horlogeT * horloge)
	{
	SDL_RemoveTimer((*horloge).horloge);  // arret timer

	return 0;
	}

//////////////////////////////////////////////////////////////////////////////
