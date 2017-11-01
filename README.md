# SiCF
Simulateur de corde vibrante et transformée de Fourier

Ce logiciel est régi par la licence CeCILL

SiCF

NOM
		SiCP - Simulateur de corde vibrante et spectre de fréquence

SYNOPSIS
		./SiCF [ OPTION VALEUR ]...

OPTION
	fond
		Couleur du fond	(noir 0 < fond < 255 blanc)

	soliton
		Déphasage entre les extrémitées / 2PI	( -99 < dephasage < 99 ),
		nombre de solitons initiales

	dt
		Discrétisation du temps	( 0.0 < dt < 0.003 )

	frequence
		Fréquence du générateur initiale

	dissipation
		Frottement visqueux initiale

	equation
		Choix de l'équation	( 0 < equation < 5 )
			1 : pendule, 2 : linéarisation, 3 : corde, 4 : dioptre

	pause
		Temps de pause entre les affichages en ms	( 5 < pause < 555 )

	duree
		Nombre d'évolution du système entre les affichages.

	mode
		Mode attente ou non		( -1 : Wait, 1 : Poll )

AUTEUR
		Écrit par Stephan Runigo

SIGNALER DES BOGUES
		runigo@free.fr

UTILISATION

Le terminal dans lequel est lancé l'exécutable affiche des informations
lors de l'exécution, la fenêtre graphique doit être active pour piloter
le simulateur.


Entrée : change le mode temporelle

+, - : augmente, diminue la vitesse de la simulation


a, q : augmenter, diminuer le couplage
z, s : augmenter, diminuer la masse

e, d : diminuer, augmenter la dissipation
r, f : retire, forme la dissipation
v : forme l'extremité absorbante

t, g : augmenter, diminuer la gravitation
y, h : ajoute, enlève un déphasage de 2 pi

w : conditions aux limites périodique
x : extrémités libres
c : extrémités fixe
b, n : fixe une extrémité, libère l'autre

u, j : augmenter, diminuer l'amplitude du générateur
i : démarre une impulsion
o : démarre, éteint le générateur
l : démarre le signal carrée
p, m : augmenter, diminuer la fréquence

F5 : affiche les énergies
F6 : affiche les paramètres physiques

flêche droite : allume, éteint le courant Josephson
flêches haut, bas : augmente, diminue l'intensité
flêche gauche : change le sens du courant

F1 : Pendule
F2 : Harmonique
F3 : Corde
F4 : Dioptre


LICENCE

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

