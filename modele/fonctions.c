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

#include "fonctions.h"

double fonctionMaximum(fonction * f);

void fonctionInitialise(fonction * f)
	{
	int i;
	double frequence = 4.444;
	double module = 0.1;

	for(i=0;i<N;i++)
		{
		(*f).reel[i] = module * cos(frequence*i);
		(*f).imag[i] = module * sin(frequence*i);
		}
	return;
	}

void fonctionEgale(fonction * onde, fonction * spectre)
	{
	int i;
	for(i=0;i<N;i++)
		{
		(*spectre).reel[i] = (*onde).reel[i];
		(*spectre).imag[i] = (*onde).imag[i];
		}
	return;
	}

void fonctionReplier(fonction * spectre, int diviseur)
	{// Effectue un repliement de spectre
	int i;
	fonction tmp;

	if (diviseur<1) diviseur = 1;

	for(i=0;i<N/2;i++)
		{
		tmp.reel[2*i] = (*spectre).reel[i];
		tmp.imag[2*i] = (*spectre).imag[i];
		tmp.reel[2*i+1] = (*spectre).reel[N-1-i];
		tmp.imag[2*i+1] = (*spectre).imag[N-1-i];
		}

	for(i=0;i<N;i++)
		{
		(*spectre).reel[i] = tmp.reel[N-1-i]/diviseur;
		(*spectre).imag[i] = tmp.imag[N-1-i]/diviseur;
		}

	return;
	}

void fonctionNormale(fonction * f, fonction * nf)
	{
	int i;
	double module;

	module = sqrt(fonctionSommeModuleCarre(f));

	for(i=0;i<N;i++)
		{
		(*nf).reel[i]=(*f).reel[i]/module;
		(*nf).imag[i]=(*f).imag[i]/module;
		}

	return;
	}

double fonctionMaximum(fonction * f)
	{
	int i;
	double x,y;
	double max=0.0;
	double module;

	for(i=0;i<N;i++)
		{
		x=(*f).reel[i];
		y=(*f).imag[i];
		module=sqrt(x*x+y*y);
		if(module > max)
			{
			max=module;
			}
		}

	return max;
	}

double fonctionSommeModuleCarre(fonction * f)
	{
	int i;
	double somme = 0.0;

	for(i=0;i<N;i++)
		{
		somme += (*f).reel[i] * (*f).reel[i] + (*f).imag[i] * (*f).imag[i];
		}

	return somme;
	}

void fonctionModuleCarre(fonction * f, fonction * modCar)
	{
	int i;

	for(i=0;i<N;i++)
		{
		(*modCar).reel[i] = (*f).reel[i] * (*f).reel[i] + (*f).imag[i] * (*f).imag[i];
		}

	return;
	}

void fonctionModuleCarreMax1(fonction * f, fonction * modCar)
	{
	int i;
	double x;
	double max=0.0;

	fonctionModuleCarre(f, modCar);

	for(i=0;i<N;i++)
		{
		x=(*modCar).reel[i];
		if(x > max)
			{
			max=x;
			}
		}

	for(i=0;i<N;i++)
		{
		(*modCar).imag[i] = (*modCar).reel[i] / max;
		}

	return;
	}

void fonctionAffiche(fonction * f)
  {
	int i;
	printf("fonction complexe\n");
	for(i=0;i<N;i++)
		{
		printf("  re[%d]:%f", i, (*f).reel[i]);
		printf("  im[%d]:%f\n", i, (*f).imag[i]);
		}
	return;
	}

void fonctionTest(fonction * f)
	{
	fonctionAffiche(f);
	return;
	}

////////////////////////////////////////////////////////////////
