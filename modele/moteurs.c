/*
Copyright novembre 2017, Stephan Runigo
runigo@free.fr
SiCP 1.4 simulateur de chaîne de pendules
SiCF 1.2  simulateur de corde vibrante et spectre
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

void moteursAffiche(moteursT * moteur);
void moteursInverseGenerateur(moteursT * moteur);
void moteursImpulsion(moteursT * moteur);

float moteursGenerateur(moteursT * moteur)
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

float moteurJaugeZero(moteursT * moteur)
	{
			// Normalise la phase et le chrono
	if((*moteur).phi > DEUXPI)
		{
		do
			{
			//printf("(*moteur).phi = %f\n",(*moteur).phi);
			//(*moteur).phi = (*moteur).phi - ((int)((*moteur).phi/DEUXPI))*DEUXPI;
			(*moteur).phi = (*moteur).phi - DEUXPI;
			}
		while((*moteur).phi > DEUXPI);
		}
	else 
		{
			if((*moteur).phi < - DEUXPI)
			{
			do
				{
				//printf("(*moteur).phi = %f\n",(*moteur).phi);
				//(*moteur).phi = (*moteur).phi - ((int)((*moteur).phi/DEUXPI))*DEUXPI;
				(*moteur).phi = (*moteur).phi + DEUXPI;
				}
			while((*moteur).phi < - DEUXPI);
			}
		}

	float phase = (*moteur).frequence * (*moteur).chrono;
	if(phase > DEUXPI)
		{
		do
			{
			//printf("(*moteur).frequence * (*moteur).chrono = %f\n",phase);
			(*moteur).chrono = (*moteur).chrono - ( (int)(phase/DEUXPI) ) * DEUXPI/(*moteur).frequence;
			}
		while((*moteur).frequence * (*moteur).chrono > DEUXPI);
		}
	else 
		{
		if(phase < - DEUXPI)
			{
			do
				{
				//printf("(*moteur).frequence * (*moteur).chrono = %f\n",phase);
				(*moteur).chrono = (*moteur).chrono - ( (int)(phase/DEUXPI) ) * DEUXPI/(*moteur).frequence;
				}
			while((*moteur).frequence * (*moteur).chrono < - DEUXPI);
			}
		}

	return 0;
	}

void moteursChangeGenerateur(moteursT * moteur, int i)

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

void moteursImpulsion(moteursT * moteur)

			// Initialise l'état impulsion
	{
	(*moteur).generateur=3;
	(*moteur).chrono=0.0;
	(*moteur).phi=0.0;

	printf("impulsion\n");
	return;
	}

void moteursChangeFrequence(moteursT * moteur, float facteur)

			//	Change la fréquence du signal
	{
	float phase;

	if((*moteur).frequence * facteur < FREQUENCE_MAX && (*moteur).frequence * facteur > FREQUENCE_MIN)
		{
		phase = (*moteur).phi + (*moteur).frequence * (*moteur).chrono;

		(*moteur).frequence = (*moteur).frequence * facteur;

		(*moteur).phi = phase - (*moteur).frequence * (*moteur).chrono;

		}
	else
		{
		printf("Fréquence limite atteinte. ");
		}
	printf("Fréquence générateur = %6.3f\n", (*moteur).frequence);

	return;
	}

void moteursChangeAmplitude(moteursT * moteur, float facteur)

			//	Change l'amplitude du signal
	{
	float amplitude = (*moteur).amplitude * facteur;
	if(amplitude < AMPLITUDE_MAX && amplitude > AMPLITUDE_MIN)
		{
		(*moteur).amplitude = (*moteur).amplitude * facteur;
		}
	else
		{
		printf("Amplitude limite atteinte. ");
		}
	printf("Amplitude générateur = %6.3f\n", (*moteur).amplitude);
	return;
	}

void moteursChangeJosephson(moteursT * moteur, float facteur)

				// Règle la valeur du courant Josephson
	{
	if(facteur == 0) // Allume / éteint le courant Josephson
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
		if(facteur < 0) // Inverse le sens du courant Josephson
			{
			(*moteur).josephson = - (*moteur).josephson;
			(*moteur).courant = - (*moteur).courant;
			}
		else
		{
		float courant = (*moteur).josephson * facteur / (*moteur).dt / (*moteur).dt;
		if(courant < 0) courant = -courant;
			printf("courant = %6.3f\n",courant);
		if(courant < JOSEPHSON_MAX && courant > JOSEPHSON_MIN)
			{
			(*moteur).josephson = ((*moteur).josephson) * facteur;
			(*moteur).courant = ((*moteur).courant) * facteur;
			}
		else
			{
			printf("Courant Josephson limite atteint. ");
			}
		}

	printf("Courant Josephson = %6.3f\n", (*moteur).josephson / (*moteur).dt / (*moteur).dt);

	return;
	}

void moteursAfficheHorloge(moteursT * m)

			// 	Affiche les paramètres de l'horloge
	{
	//printf("(*moteur).horloge = %6.3f\n", (*m).horloge);		//	Somme des dt
	printf("(*moteur).chrono = %6.3f\n", (*m).chrono);		//	Remis à zéro

	return;
	}

void moteursAffiche(moteursT * moteur)

			// 	Affiche la valeur des paramètres du moteur
	{
	printf("courant Josephson = %6.3f\n", (*moteur).josephson / (*moteur).dt / (*moteur).dt);
	//printf("(*moteur).horloge = %6.3f\n", (*moteur).horloge);		//	Somme des dt
	printf("(*moteur).chrono = %6.3f\n", (*moteur).chrono);		//	Remis à zéro

	printf("(*moteur).generateur = %d", (*moteur).generateur);
	printf("  ( 0:eteint, 1:sinus, 2:carre, 3:impulsion )\n");
	printf("(*moteur).amplitude = %6.3f\n", (*moteur).amplitude);	//	Amplitude du moteurs
	printf("(*moteur).frequence = %6.3f\n", (*moteur).frequence);	//	Fréquence du moteurs
	printf("(*moteur).phi = %6.3f\n", (*moteur).phi);			//	Dephasage

	return;
	}

void moteursInverseGenerateur(moteursT * moteur)
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
