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

#include "projection.h"

void projectionMoyenneGraphe(graphe * spectr)
	{
	int i;
	for(i=0;i<N;i++)
		{
		(*spectr).nouvelOrdonnee[i] = (*spectr).nouvelOrdonnee[i] + 2 *  (*spectr).ancienOrdonnee[i];
		}
	return;
	}

void projectionSystemeGraphe(systeme * system, graphe * graph)
	{
	//		Projette le systeme sur le graphe "corde"
	int i, x, y;
	double max, min, psi;
	int imin, imax, iamp;

	// Recherche du maximum et du minimum

	max = (*system).pendul[0].nouveau;
	min = (*system).pendul[0].nouveau;

	for(i=1;i<N;i++)
		{
		if((*system).pendul[i].nouveau > max)
			{
			max = (*system).pendul[i].nouveau;
			}
		if((*system).pendul[i].nouveau < min)
			{
			min = (*system).pendul[i].nouveau;
			}
		}

	// Amplitude des valeurs à dessiner

	imin = (int)(min);
	imax = (int)(max);
	iamp = imax-imin;

	// Projection de la fonction

	for(i=0;i<N;i++)
		{
		// Position horizontale

		x = LARGEUR/20 + (i*LARGEUR*9) / (N) / 10;

		// Position verticale

		psi = (*system).pendul[i].nouveau;

		y = HAUTEUR/4; // Position moyenne du graphe

		if(iamp>3) // Echelle du graphe
			{
			y = y - ( psi / (iamp/3) ) * (HAUTEUR/15);
			}
		else
			{
			y = y - ( psi ) * (HAUTEUR/5);
			}

		// Affectation des coordonnées

		(*graph).nouvelAbscisse[i] = x;
		(*graph).nouvelOrdonnee[i] = y;

		}
	return;
	}

void projectionSystemeFonction(systeme * system, fonction * spectreG, fonction * spectreD)
	{
	//	Projette les parties gauche et droite du systeme sur deux fonctions
	int i;
	for(i=0;i<Ne;i++)
		{

		(*spectreG).reel[i]=(*system).pendul[i].nouveau;
		(*spectreG).imag[i]=0.0;

		(*spectreD).reel[i]=(*system).pendul[i+Ne].nouveau;
		(*spectreD).imag[i]=0.0;

		}
	return;
	}

void projectionSpectreGraphe(fonction * spectreG, fonction * spectreD, graphe * modul)
	{
	//	Projette les fonctions sur le graphe "fourier"

	projectionFonctionGraphe(spectreG, modul, 1);
	projectionFonctionGraphe(spectreD, modul, 2);

	return;

	}

void projectionFonctionGraphe(fonction * fourier, graphe * spectre, int position)
	{

	//		Projette une fonction positive sur un demi graphe :
	//			position 1 : partie gauche,
	//			position 2 : partie droite

	//		la projection opère un retournement
	//		du spectre, l'algorithme de fourier
	//		fournissant un spectre symétrisé

	int i, x, xs, y, ys;
	double reel, imag;
	double max;
	fonction module;

	// Calcul du module

	for(i=0;i<Ne;i++)
		{
		reel = (*fourier).reel[i];
		imag = (*fourier).imag[i];
		module.reel[i] = sqrt( reel * reel + imag * imag);
		}

	// Résolution du problème à l'origine
	// Supprime la "moyenne" ? 

	module.reel[0] = 0;
	module.reel[Ne-1] = 0;

	// Recherche du maximum

	max = 0.1;
	for(i=0;i<Ne;i++)
		{
		if(module.reel[i] > max)
			{
			max = module.reel[i];
			}
		}
	//fprintf(stderr, " Maximum partie %d = %f\n", position, max);
	// Projection de la fonction

	for(i=0;i<Ne/2;i++)
		{
		// Position horizontale
		x = (2-position)*LARGEUR/20 + (position-1)*LARGEUR/2;
		xs = x;

		x = x + (2*i*LARGEUR*3) / (Ne) / 10;
		xs = xs + ((2*i+1)*LARGEUR*3) / (Ne) / 10;

		(*spectre).fixeAbscisse[2*i + (position-1)*Ne] = x;
		(*spectre).fixeAbscisse[(2*i+1) + (position-1)*Ne] = xs;

		// Position verticale

		y = (HAUTEUR*9)/10;
		ys = y;

		(*spectre).fixeOrdonnee[2*i + (position-1)*Ne] = y;
		(*spectre).fixeOrdonnee[2*i+1 + (position-1)*Ne] = ys;

		y = y - (( module.reel[i] / max )*(HAUTEUR*7)) / 20;
		ys = ys - (( module.reel[Ne-i] / max )*(HAUTEUR*7)) / 20;

		// Affectation des coordonees

		(*spectre).nouvelAbscisse[2*i + (position-1)*Ne] = x;
		(*spectre).nouvelOrdonnee[2*i + (position-1)*Ne] = y;
		(*spectre).nouvelAbscisse[2*i+1 + (position-1)*Ne] = xs;
		(*spectre).nouvelOrdonnee[2*i+1 + (position-1)*Ne] = ys;

		}
	return;
	}

////////////////////////////////////////////////////////////////////////////////////
