/*
Copyright novembre 2017, Stephan Runigo
runigo@free.fr
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

#include "options.h"

void optionsDt(options * option, char *opt);
void optionsMoteur(options * option, char *opt);
void optionsFrequence(options * option, char *opt);
void optionsDissipation(options * option, char *opt);
void optionsEquation(options * option, char *opt);
void optionsFond(options * option, char *opt);
void optionsNombre(options * option, char *opt);
void optionsThread(options * option, char *opt);
void optionsPause(options * option, char *opt);
void optionsSoliton(options * option, char *opt);
void optionsMode(options * option, char *opt);
void optionsDuree(options * option, char *opt);
void optionsAide(void);

int optionsTraitement(options * option, int nb, char *opt[])
	{
	int i=0;
	//fprintf(stderr, "\nNombre d'option : %d\n", nb);
	do
		{
		if(strcmp(opt[i], "fond")==0 && opt[i+1]!=NULL)
			optionsFond(option, opt[i+1]);	// Couleur du fond
		if(strcmp(opt[i], "mode")==0 && opt[i+1]!=NULL)
			optionsMode(option, opt[i+1]);	// Mode -1 : Wait, 1 : Poll
		if(strcmp(opt[i], "pause")==0 && opt[i+1]!=NULL)
			optionsPause(option, opt[i+1]);	// temps de pause en ms
		if(strcmp(opt[i], "duree")==0 && opt[i+1]!=NULL)
			optionsDuree(option, opt[i+1]);	// Nombre d'évolution du système entre les affichages.


		if(strcmp(opt[i], "equation")==0 && opt[i+1]!=NULL)
			optionsEquation(option, opt[i+1]);	// choix de l'équation
		if(strcmp(opt[i], "dt")==0 && opt[i+1]!=NULL)
			optionsDt(option, opt[i+1]);		// discrétisation du temps

		if(strcmp(opt[i], "soliton")==0 && opt[i+1]!=NULL)
			optionsSoliton(option, opt[i+1]);	// déphasage entre les extrémitées


		if(strcmp(opt[i], "moteur")==0 && opt[i+1]!=NULL)
			optionsMoteur(option, opt[i+1]);	// état du moteur

		if(strcmp(opt[i], "frequence")==0 && opt[i+1]!=NULL)
			optionsFrequence(option, opt[i+1]);	// fréquence du générateur

		if(strcmp(opt[i], "dissipation")==0 && opt[i+1]!=NULL)
			optionsDissipation(option, opt[i+1]);	// dissipation
/*
		if(strcmp(opt[i], "th")==0)
			optionsThread(option, opt[i+1]);	// Deux threads
*/
		if(strcmp(opt[i], "aide")==0)
			optionsAide();	// Affiche l'aide.
		if(strcmp(opt[i], "help")==0)
			optionsAide();	// Affiche l'aide.

  		i++;
  		}
		while(i<nb);
	return 0;
	}

    	// Couleur du fond 
void optionsFond(options * option, char *opt)
	{
	int fond = atoi(opt);
	if(fond>0 && fond<255)
		{
		(*option).fond = fond;
		printf("Option fond valide, fond = %d\n", (*option).fond);
		}
	else
		{
		printf("Option fond non valide, fond = %d\n", (*option).fond);
		printf("Option fond : 0 < fond < 255\n");
		}
	return;
	}

		// déphasage entre les extrémitées
void optionsSoliton(options * option, char *opt)
	{
	int dephasage = atoi(opt);
	if(dephasage > -99 && dephasage < 99)
		{
		(*option).dephasage = dephasage * 2 * PI;
		printf("Option soliton valide, dephasage = %d\n", (*option).dephasage);
		}
	else
		{
		printf("Option soliton non valide, dephasage = %d\n", (*option).dephasage);
		printf("Option soliton : -99 < dephasage < 99\n");
		}
	return;
	}

    	// discrétisation du temps 
void optionsDt(options * option, char *opt)
	{
	float dt = atof(opt);
	if(dt>0.0 && dt<DT_MAX)
		{
		(*option).dt = dt;
		printf("Option dt valide, dt = %f\n", (*option).dt);
		}
	else
		{
		printf("Option dt non valide, dt = %f\n", (*option).dt);
		printf("Option dt : 0.0 < dt < %f\n", DT_MAX);
		}
	return;
	}

    	// État du moteur
void optionsMoteur(options * option, char *opt)
	{
	int etat = atoi(opt);
	if(etat>-1 && etat<4)
		{
		(*option).moteur = etat;
		printf("Option moteur valide, moteur = %d\n", (*option).moteur);
		}
	else
		{
		printf("Option moteur non valide, moteur = %d\n", (*option).moteur);
		printf("Option moteur : -1 < fr < 4\n");
		}
	return;
	}

    	// Fréquence du générateur
void optionsFrequence(options * option, char *opt)
	{
	float fr = atof(opt);
	if(fr>0.1 && fr<1111)
		{
		(*option).frequence = fr;
		printf("Option frequence valide, fr = %f\n", (*option).frequence);
		}
	else
		{
		printf("Option frequence non valide, fr = %f\n", (*option).frequence);
		printf("Option frequence : 0.1 < fr < 1111\n");
		}
	return;
	}

    	// Dissipation
void optionsDissipation(options * option, char *opt)
	{
	float dissipation = atof(opt);
	if(dissipation>0.0001 && dissipation<1111)
		{
		(*option).dissipation = dissipation;
		printf("Option dissipation valide, dissipation = %f\n", (*option).dissipation);
		}
	else
		{
		printf("Option dissipation non valide, dissipation = %f\n", (*option).dissipation);
		printf("Option dissipation : 0.0001 < dissipation < 1111\n");
		}
	return;
	}

		// choix de l'équation
void optionsEquation(options * option, char *opt)
	{
	int equation = atoi(opt);
	if(equation > 0 && equation <5)
		{
		(*option).equation = equation;
		printf("Option equation valide, equation = %d\n", (*option).equation);
		}
	else
		{
		printf("Option equation non valide, equation = %d\n", (*option).equation);
		printf("Option equation : 1 < equation < 5\n");
		}
	return;
	}


    	// Nombre d'évolution du système entre les affichages
void optionsDuree(options * option, char *opt)
	{
	int duree = atoi(opt);
	if ( duree > 0 && duree < DUREE_MAX)
		{
		(*option).duree = duree;
		printf("Option duree valide, duree = %d\n", (*option).duree);
		}
	else
		{
		printf("Option duree non valide, duree = %d\n", (*option).duree);
		printf("Option duree : 0.0 < duree < %d\n", DUREE_MAX);
		}
	return;
	}

    	// Temps de pause en ms après affichage graphique
void optionsPause(options * option, char *opt)
	{
	int pause = atoi(opt);
	if(pause>5 || pause<555)
		{
		(*option).pause = pause;
		printf("Option t valide, pause = %d\n", (*option).pause);
		}
	else
		{
		printf("Option t non valide, pause = %d\n", (*option).pause);
		printf("Option t : 5 < pause < 555\n");
		}
	return;
	}

		// Mode  -1 : Wait, 1 : Poll
void optionsMode(options * option, char *opt)
	{
	int mode = atoi(opt);
	if(mode==1 || mode==-1)
		{
		(*option).mode = mode;
		printf("Option mode valide, mode = %d\n", (*option).mode);
		}
	else
		{
		printf("Option mode non valide, mode = %d\n", (*option).mode);
		printf("Option mode : mode = +/- 1\n");
		}
	return;
	}

    	// Un processus ou deux threads
void optionsThread(options * option, char *opt)
	{
	int thread = atoi(opt);
	if(thread==1 || thread==0)
		{
		(*option).thread = thread;
		printf("Option thread valide, thread = %d\n", (*option).thread);
		}
	else
		{
		printf("Option thread non valide, thread = %d\n", (*option).thread);
		printf("Option thread : thread = 0 ou 1\n");
		}
	return;
	}

void optionsAide(void)
	{
	printf("\n\nAide de SiCF, le simulateur de corde vibrante et transformée de Fourier\n\n");

	printf("OPTIONS DE LA LIGNE DE COMMANDE \n\n");

	printf(" fond		0 < fond < 255		couleur du fond de l'affichage (fond noir : 0, fond blanc : 255)\n");
	printf(" soliton	-99 < soliton < 99	déphasage entre les extrémitées\n");
	printf(" dt		0.0 < dt < %4.4f	discrétisation du temps\n", DT_MAX);
	printf(" frequence				fréquence du générateur\n");
	printf(" dissipation				dissipation\n");
	printf(" equation	0 < equation < 5	choix de l'équation\n");
	printf(" pause		5 < pause < 555		pause entre les affichages en ms\n");
	printf(" duree		1 < duree < %d		nombre d'évolution du système entre les affichages\n", DUREE_MAX);
	printf(" mode		= -1 ou 1		mode avec ou sans attente (Mode -1 : Wait, 1 : Poll)\n");
	//printf("	flèches haut, bas, gauche, droite\n\n");

	printf("\nCOMMANDE CLAVIER\n");

	printf("	a, q : augmenter, diminuer le couplage\n");
	printf("	z, s : augmenter, diminuer la masse\n");

	printf("	e, d : diminuer, augmenter la dissipation\n");
	printf("	r, f : retire, forme la dissipation\n");
	printf("	v : forme l'extrémité absorbante\n");

	printf("	t, g : augmenter, diminuer la gravitation\n");
	printf("	y, h : ajoute, enlève un déphasage de 2pi\n");

	printf("	w : conditions aux limites périodique\n");
	printf("	x : extrémités libres\n");
	printf("	c : extrémités fixe\n");
	printf("	b, n : fixe une extrémité, libère l'autre\n");

	printf("	flêche droite : allume, éteint le courant Josephson\n");
	printf("	flêches haut, bas : augmente diminue l'intensité\n");
	printf("	flêche gauche : change le sens du courant\n");

	printf("	u, j : augmenter, diminuer l'amplitude du générateur\n");
	printf("	i : démarre une impulsion\n");
	printf("	o : démarre, éteint le générateur\n");
	printf("	l : démarre le signal carrée\n");
	printf("	p, m : augmenter, diminuer la fréquence\n");

	printf("	F5 : affiche les observables\n");

	printf("	Entrée : change le mode temporel\n");

	printf("	+, - : augmente, diminue la vitesse de la simulation\n");
	printf("	F9, F10, F11, F12 : diminuent, augmentent la vitesse de la simulation\n");

	fprintf(stderr, "\nSortie de SiCF\n");
	exit(EXIT_FAILURE);
	return;
	}

/////////////////////////////////////////////////////////////////////
