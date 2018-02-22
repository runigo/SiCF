/*
Copyright février 2018, Stephan Runigo
runigo@free.fr
SiCF 1.2.3  simulateur de corde vibrante et spectre
Ce logiciel est un programme informatique servant à simuler l'équation
d'une chaîne de pendules et à en donner une représentation graphique en
trois dimensions.
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

#include "fichier.h"

int fichierFonctionNulle(systemeT * systeme);
int fichierFonctionTriangle(systemeT * systeme, int numero);
int fichierFonctionCarre(systemeT * systeme, int numero);
int fichierFonctionSinus(systemeT * systeme, int numero);



void fichierEcriture(systemeT * systeme, int numero)
	{
	FILE *fichier; /* pointeur sur FILE */
	double ancien, actuel;
	int i;

	switch (numero)
		{
		case 10:
			fichier = fopen("./donnees/enregistrement/quanton_q.sicf", "w");break;
		case 11:
			fichier = fopen("./donnees/enregistrement/quanton_s.sicf", "w");break;
		case 12:
			fichier = fopen("./donnees/enregistrement/quanton_d.sicf", "w");break;
		case 13:
			fichier = fopen("./donnees/enregistrement/quanton_f.sicf", "w");break;
		case 14:
			fichier = fopen("./donnees/enregistrement/quanton_g.sicf", "w");break;
		case 15:
			fichier = fopen("./donnees/enregistrement/quanton_h.sicf", "w");break;
		default:
			;
		}
	//fichier = fopen("fluxon.sicp", "w");  /* write */

	for(i=0;i<N;i++)
		{
		//penduleInitialisePosition(&(*systeme).pendule[i], 0.0, 0.0);
		ancien = (*systeme).pendule[i].ancien;
		actuel =(*systeme).pendule[i].nouveau;
		fprintf(fichier, "%f %f\n", ancien, actuel);
		}
	fclose(fichier);

	return;
	}

void fichierLecture(systemeT * systeme, int numero)
	{
	FILE *fichier; /* pointeur sur FILE */
	float ancien, actuel;
	int i;

	switch (numero)
		{
		case 0: // Touche A
			fichier = fopen("./donnees/enregistrement/01_nulle.sicf", "r");break;
		case 1: // Touche Z
			fichier = fopen("./donnees/enregistrement/050_fr-100.sicf", "r");break;
		case 2: // Touche E
			fichier = fopen("./donnees/enregistrement/050_fr-200.sicf", "r");break;
		case 3: // Touche R
			fichier = fopen("./donnees/enregistrement/052_fr-100-200.sicf", "r");break;
		case 4: // Touche T
			fichier = fopen("./donnees/enregistrement/quanton_t.sicf", "r");break;
		case 5: // Touche Y
			fichier = fopen("./donnees/enregistrement/quanton_y.sicf", "r");break;
		case 6: // Touche U
			fichier = fopen("./donnees/enregistrement/quanton_u.sicf", "r");break;
		case 7: // Touche I
			fichier = fopen("./donnees/enregistrement/quanton_i.sicf", "r");break;
		case 8: // Touche O
			fichier = fopen("./donnees/enregistrement/quanton_o.sicf", "r");break;
		case 9: // Touche P
			fichier = fopen("./donnees/enregistrement/quanton_p.sicf", "r");break;
		case 10: // Touche Q
			fichier = fopen("./donnees/enregistrement/quanton_q.sicf", "r");break;
		case 11: // Touche S
			fichier = fopen("./donnees/enregistrement/quanton_s.sicf", "r");break;
		case 12: // Touche D
			fichier = fopen("./donnees/enregistrement/quanton_d.sicf", "r");break;
		case 13: // Touche F
			fichier = fopen("./donnees/enregistrement/quanton_f.sicf", "r");break;
		case 14: // Touche G
			fichier = fopen("./donnees/enregistrement/quanton_g.sicf", "r");break;
		case 15: // Touche H
			fichier = fopen("./donnees/enregistrement/quanton_h.sicf", "r");break;
		case 16: // Touche J
			fichier = fopen("./donnees/enregistrement/quanton_j.sicf", "r");break;
		case 17: // Touche K
			fichier = fopen("./donnees/enregistrement/quanton_k.sicf", "r");break;
		case 18: // Touche L
			fichier = fopen("./donnees/enregistrement/quanton_l.sicf", "r");break;
		case 19: // Touche M
			fichier = fopen("./donnees/enregistrement/quanton_m.sicf", "r");break;
		default:
			;
		}
	//fichier = fopen("fluxon.sicp", "r");  /* read */
	if(fichier == NULL)
		{
		printf("Erreur d'ouverture du fichier de réinitialisation\n");
		printf("	Vérifier le répertoire donnees/enregistrement\n");
		}
	else
		{
		for(i=0;i<N;i++)
			{
			ancien = 0;
			actuel = 0;
			fscanf(fichier, "%f %f\n", &ancien, &actuel);
			penduleInitialisePosition(&(*systeme).pendule[i], ancien, actuel);
			}
		fclose(fichier);
		}

	return;
	}

void fichierFonction(systemeT * systeme, int numero)
	{
	switch (numero)
		{
		case 0: // Touche A
			fichierFonctionNulle(systeme);break;
		case 1: // Touche Z
			fichierFonctionSinus(systeme, 1);break;
		case 2: // Touche E
			fichierFonctionTriangle(systeme, 2);break;
		case 3: // Touche R
			fichierFonctionTriangle(systeme, 3);break;
		case 4: // Touche T
			fichierFonctionTriangle(systeme, 4);break;
		case 5: // Touche Y
			fichierFonctionTriangle(systeme, 5);break;
		case 6: // Touche U
			fichierFonctionTriangle(systeme, 6);break;
		case 7: // Touche I
			fichierFonctionTriangle(systeme, 7);break;
		case 8: // Touche O
			fichierFonctionTriangle(systeme, 8);break;
		case 9: // Touche P
			fichierFonctionTriangle(systeme, 9);break;
		case 10: // Touche Q
			fichierFonctionSinus(systeme, 1);break;
		case 11: // Touche S
			fichierFonctionSinus(systeme, 2);break;
		case 12: // Touche D
			fichierFonctionSinus(systeme, 3);break;
		case 13: // Touche F
			fichierFonctionSinus(systeme, 4);break;
		case 14: // Touche G
			fichierFonctionSinus(systeme, 5);break;
		case 15: // Touche H
			fichierFonctionCarre(systeme, 1);break;
		case 16: // Touche J
			fichierFonctionCarre(systeme, 2);break;
		case 17: // Touche K
			fichierFonctionCarre(systeme, 3);break;
		case 18: // Touche L
			fichierFonctionCarre(systeme, 4);break;
		case 19: // Touche M
			fichierFonctionCarre(systeme, 5);break;
		default:
			;
		}
	return;
	}

int fichierFonctionNulle(systemeT * systeme)
	{
	float ancien, actuel;
	int i;
	for(i=0;i<N;i++)
		{
		ancien = 0.0;
		actuel = 0.0;
		penduleInitialisePosition(&(*systeme).pendule[i], ancien, actuel);
		}

	return 0;
	}

int fichierFonctionTriangle(systemeT * systeme, int numero)
	{
	float ancien, actuel;
	int i;
	fichierFonctionNulle(systeme);
	switch (numero)
		{
		case 1:
			printf("Réinitialisation du système, numero = %d\n", numero);
			for(i=0;i<N/2;i++)
				{
				ancien = (float)(i*numero)/(float)N;
				actuel = ancien;
				penduleInitialisePosition(&(*systeme).pendule[i], ancien, actuel);
				penduleInitialisePosition(&(*systeme).pendule[N-1-i], ancien, actuel);
				}break;
		case 2:
			printf("Réinitialisation du système, numero = %d\n", numero);
			for(i=0;i<N/4;i++)
				{
				ancien = (float)(i*numero)/(float)N;
				actuel = ancien;
				penduleInitialisePosition(&(*systeme).pendule[i], ancien, actuel);
				penduleInitialisePosition(&(*systeme).pendule[N/2-1-i], ancien, actuel);
				}break;
		case 3:
			printf("Réinitialisation du système, numero = %d\n", numero);
			for(i=0;i<N/8;i++)
				{
				ancien = (float)(i*numero)/(float)N;
				actuel = ancien;
				penduleInitialisePosition(&(*systeme).pendule[i], ancien, actuel);
				penduleInitialisePosition(&(*systeme).pendule[N/4-1-i], ancien, actuel);
				}break;
		case 4:
			printf("Réinitialisation du système, numero = %d\n", numero);
			for(i=0;i<N/16;i++)
				{
				ancien = (float)(i*numero)/(float)N;
				actuel = ancien;
				penduleInitialisePosition(&(*systeme).pendule[i], ancien, actuel);
				penduleInitialisePosition(&(*systeme).pendule[N/8-1-i], ancien, actuel);
				}break;
		case 5:
			printf("Réinitialisation du système, numero = %d\n", numero);
			for(i=0;i<N/32;i++)
				{
				ancien = (float)(i*numero)/(float)N;
				actuel = ancien;
				penduleInitialisePosition(&(*systeme).pendule[i], ancien, actuel);
				penduleInitialisePosition(&(*systeme).pendule[N/16-1-i], ancien, actuel);
				}break;
		case 6:
			printf("Réinitialisation du système, numero = %d\n", numero);
			for(i=0;i<N/64;i++)
				{
				ancien = (float)(i*numero)/(float)N;
				actuel = ancien;
				penduleInitialisePosition(&(*systeme).pendule[i], ancien, actuel);
				penduleInitialisePosition(&(*systeme).pendule[N/64-1-i], ancien, actuel);
				}break;
		}

	return 0;
	}

int fichierFonctionCarre(systemeT * systeme, int numero)
	{
	float ancien, actuel;
	int i;
	fichierFonctionNulle(systeme);
	if(numero == 1)
		{
		printf("Réinitialisation du système, numero = 1\n");
		}
	else
		{
		printf("Réinitialisation du système, numero != 1\n");
		for(i=0;i<N/2;i++)
			{
			ancien = i*numero/N;
			actuel = ancien;
			penduleInitialisePosition(&(*systeme).pendule[i], ancien, actuel);
			penduleInitialisePosition(&(*systeme).pendule[N-1-i], ancien, actuel);
			}
		}

	return 0;
	}

int fichierFonctionSinus(systemeT * systeme, int numero)
	{
	float ancien, actuel;
	int i;
	fichierFonctionNulle(systeme);
	if(numero == 1)
		{
		printf("Réinitialisation du système, numero = 1\n");
		}
	else
		{
		printf("Réinitialisation du système, numero != 1\n");
		for(i=0;i<N/2;i++)
			{
			ancien = i*numero/N;
			actuel = ancien;
			penduleInitialisePosition(&(*systeme).pendule[i], ancien, actuel);
			penduleInitialisePosition(&(*systeme).pendule[N-1-i], ancien, actuel);
			}
		}

	return 0;
	}


///////////////////////////////////////////////////////////////////////////////////////////////
