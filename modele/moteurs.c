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

#include "moteurs.h"

void moteursAffiche(moteurs * moteur);
void moteursInverseGenerateur(moteurs * moteur);
void moteursImpulsion(moteurs * moteur);

float moteursGenerateur(moteurs * moteur)
	{
			// retourne la position du générateur de signaux

	float amplitude;
	float phase;

	phase = (*moteur).phi + (*moteur).frequence * (*moteur).chrono;
	amplitude = (*moteur).amplitude * sin(phase);

	if((*moteur).generateur==3)
		{
		if(phase>2*PI) (*moteur).generateur=0;
		}

	if((*moteur).generateur==2)
		{
		if(amplitude>0) amplitude = (*moteur).amplitude;
		else amplitude = - (*moteur).amplitude;
		}

	return amplitude;
	}

void moteursChangeGenerateur(moteurs * moteur, int i)

			// Change la forme du signal
			// 0 : éteint, -1 : allume-éteint, 1 : allume, 3 : impulsion
	{

	// Réinitialisation
	(*moteur).chrono = 0.0;
	(*moteur).phi = 0.0;

	switch(i)
		{
		case 0:	// désactive le moteur
			(*moteur).generateur=0;break;

		case 1:	// active le moteur
			(*moteur).generateur=1;break;

		case -1:	// active/désactive le moteur
			moteursInverseGenerateur(moteur);break;

		case 2:	// active le signal carré
			(*moteur).generateur=2;break;

		case 3:	// Lance une impulsion
			moteursImpulsion(moteur);break;

		default:
			;
		}

	printf("etat générateur %d\n", (*moteur).generateur);

	return;
	}

void moteursImpulsion(moteurs * moteur)

			// Initialise l'état impulsion
	{
	(*moteur).generateur=3;
	(*moteur).chrono=0.0;
	(*moteur).phi=0.0;

	printf("impulsion\n");
	return;
	}

void moteursChangeFrequence(moteurs * moteur, float facteur)

			//	Change la fréquence du signal
	{
	float phase;

	phase = (*moteur).phi + (*moteur).frequence * (*moteur).chrono;

	(*moteur).frequence = (*moteur).frequence * facteur;

	(*moteur).phi = phase - (*moteur).frequence * (*moteur).chrono;

	printf("frequence générateur = %6.3f\n", (*moteur).frequence);
	return;
	}

void moteursChangeAmplitude(moteurs * moteur, float facteur)

			//	Change l'amplitude du signal
	{
	(*moteur).amplitude = (*moteur).amplitude * facteur;
	printf("amplitude générateur = %6.3f\n", (*moteur).amplitude);
	return;
	}

void moteursChangeJosephson(moteurs * moteur, float facteur)

				// Règle la valeur du courant Josephson
	{
	if(facteur == 0)
		{
		if((*moteur).josephson == 0)
			{
			(*moteur).josephson = (*moteur).courant * (*moteur).dt * (*moteur).dt;
			}
		else
			{
			(*moteur).josephson = 0.0;
			}
		}
	else
		{
		(*moteur).josephson = ((*moteur).josephson) * facteur;
		(*moteur).courant = ((*moteur).courant) * facteur;
		}

	printf("courant Josephson = %6.3f\n", (*moteur).josephson / (*moteur).dt / (*moteur).dt);

	return;
	}

void moteursAffiche(moteurs * moteur)

			// 	Affiche la valeur des paramètres du moteur
	{
	printf("courant Josephson = %6.3f\n", (*moteur).josephson / (*moteur).dt / (*moteur).dt);
	printf("(*moteur).horloge = %6.3f\n", (*moteur).horloge);		//	Somme des dt
	printf("(*moteur).chrono = %6.3f\n", (*moteur).chrono);		//	Remis à zéro

	printf("(*moteur).generateur = %d", (*moteur).generateur);
	printf("  ( 0:eteint, 1:sinus, 2:carre, 3:impulsion )\n");
	printf("(*moteur).amplitude = %6.3f\n", (*moteur).amplitude);	//	Amplitude du moteurs
	printf("(*moteur).frequence = %6.3f\n", (*moteur).frequence);	//	Fréquence du moteurs
	printf("(*moteur).phi = %6.3f\n", (*moteur).phi);			//	Dephasage

	return;
	}

void moteursInverseGenerateur(moteurs * moteur)
	{
	/*	Allume le générateur s'il est éteint,
			Éteint le générateur sinon		*/

	switch((*moteur).generateur)
		{
		case 0:	// désactive le moteur
			(*moteur).generateur=1;break;

		case 1:	// active le moteur
			(*moteur).generateur=0;break;

		default:
			(*moteur).generateur=0;
		} 
	return;
	}
//////////////////////////////////////////////////////////////////////////
