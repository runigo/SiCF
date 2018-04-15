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

#include "graphique.h"

void graphiqueAffichePixel (int x, int y, Uint32 couleur);
int graphiqueAffichePixelVerif(int x, int y, Uint32 couleur);
void graphiqueLigneDroite(int X, int Y, int x, int y, Uint32 couleur);
void graphiqueCercle(int X, int Y, int rayon, Uint32 couleur);
void graphiqueDisque(int X, int Y, int rayon, Uint32 couleur);

SDL_Surface* affichage;

void graphiqueInitialise(int fond)
	{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
		{
		fprintf(stderr, "Erreur à l'initialisation de la SDL : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
		}
	else fprintf(stderr, "SDL initialisé\n");

	atexit(SDL_Quit);

	affichage = SDL_SetVideoMode(LARGEUR, HAUTEUR, 32, SDL_SWSURFACE);

	if (affichage == NULL)
		{
		fprintf(stderr, "Impossible d'activer le mode graphique : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
		}
	else fprintf(stderr, "Mode graphique activé\n");

	SDL_WM_SetCaption("SiCF", NULL);

	Uint32 clrFond = SDL_MapRGB(affichage->format, fond, fond, fond);

	SDL_FillRect(affichage, NULL, clrFond);

	return;
	}

int graphiqueNettoyage(int couleurFond)
	{
	Uint32 couleurF=SDL_MapRGB(affichage->format,couleurFond,couleurFond,couleurFond);
	SDL_FillRect(affichage, NULL, couleurF);
	return 0;
	}

void graphiqueMiseAJour(void)
	{
	SDL_UpdateRect(affichage, 0, 0, 0, 0);
	}

void graphiqueAffichePixel(int x, int y, Uint32 couleur)
	{
	*((Uint32*)(affichage->pixels) + x + y * affichage->w) = couleur;
	return;
	}

int graphiqueAffichePixelVerif(int x, int y, Uint32 couleur)
	{
	if (x>=0 && x < affichage->w && y>=0 && y < affichage->h)
		{
		graphiqueAffichePixel(x, y, couleur);
		}
	else
		{
		return 1;
		}
	return 0;
	}

void graphiqueLigneDroite(int X, int Y, int x, int y, Uint32 couleur)
	{
	int i, abs, ord;
	int p;
	float a;
	p = sqrt((X-x)*(X-x)+(Y-y)*(Y-y));
	for(i=0;i<p;i++)
		{
		a=i/(float)p;
		abs=(X+(int)(a*(x-X)));
		ord=(Y+(int)(a*(y-Y)));
		graphiqueAffichePixelVerif(abs, ord, couleur);
		}
	return;
	}

void graphiqueCercle(int X, int Y, int rayon, Uint32 couleur)
	{
	int i, abs, ord;

	int p = 2*PI*rayon;

	for(i=0;i<p;i++)
		{
		abs=(X+rayon*sin(i/(float)rayon));
		ord=(Y+rayon*cos(i/(float)rayon));
		graphiqueAffichePixelVerif(abs, ord, couleur);
		}
	return;
	}

void graphiqueDisque(int X, int Y, int rayon, Uint32 couleur)
	{
	int i;

	for(i=1;i<rayon;i++)
		{
		graphiqueCercle(X, Y, i, couleur);
		}

	return;
	}

int graphiqueDessineCorde(graphe * graph)
	{
	int i, x, y, X, Y;
	//Uint32 clrFond = SDL_MapRGB(affichage->format, (*graph).fond, (*graph).fond, (*graph).fond);
	Uint32 couleur = SDL_MapRGB(affichage->format, (*graph).rouge, (*graph).bleu, (*graph).vert);
	//Uint32 couleurNouveau = SDL_MapRGB(affichage->format, (*graph).rouge/2, (*graph).bleu/2, (*graph).vert/2);
	for(i=1;i<N;i++)
		{
	/*	X = (*graph).ancienAbscisse[i-1];
		Y = (*graph).ancienOrdonnee[i-1];
		x = (*graph).ancienAbscisse[i];
		y = (*graph).ancienOrdonnee[i];
		graphiqueLigneDroite(X, Y, x, y, couleurAncien);
		graphiqueLigneDroite(X, Y+1, x, y+1, couleurAncien); */
		X = (*graph).nouvelAbscisse[i-1];
		Y = (*graph).nouvelOrdonnee[i-1];
		x = (*graph).nouvelAbscisse[i];
		y = (*graph).nouvelOrdonnee[i];
		graphiqueLigneDroite(X, Y, x, y, couleur);
		graphiqueLigneDroite(X, Y+1, x, y+1, couleur);
		}

	return 0;
	}

int graphiqueDessineSpectre(graphe * spectr)
	{
	int i, x, y, X, Y;
	//Uint32 clrFond = SDL_MapRGB(affichage->format, (*spectr).fond, (*spectr).fond, (*spectr).fond);
	Uint32 couleur = SDL_MapRGB(affichage->format, (*spectr).vert, (*spectr).rouge, (*spectr).bleu);
	for(i=0;i<N;i++)
		{
		X=(*spectr).fixeAbscisse[i];
		Y=(*spectr).fixeOrdonnee[i];
	/*	x=(*spectr).ancienAbscisse[i];
		y=(*spectr).ancienOrdonnee[i];
		graphiqueLigneDroite(X, Y, x, y, clrFond); */
		x=(*spectr).nouvelAbscisse[i];
		y=(*spectr).nouvelOrdonnee[i];
		graphiqueLigneDroite(X, Y, x, y, couleur);
		}

	return 0;
	}

//////////////////////////////////////////////////////////////////////////////////////
