//======================================================================
//  Auteur : Eric JENN
//  (c) IRT Saint-Exupery
//
//----------------------------------------------------------------------
// Fichier de d�finition des donn�es utilis�es par le filtre de Kalman
//======================================================================

#include "../include/com.h"
#include "../include/mat.h"
#include "../include/parametres.h"



// Param�tres relatifs au robot

const T_robot_parameters K_rp = {
	0.07, 	// Diametre de la roue gauche en m
 	0.07, 	// Diametre de la roue droite en m
	0.3,	// Distance entre les deux roues en m
  	1.745,	// Ecart type vitesse roue gauche
	1.745	// Ecart type vitesse roue droite
};


/* ---------------------------------
   Parametres du filtrage de Kalman
  ---------------------------------
 On voit ici que la matrice R devrait contenir sur sa diagonale les memes
 valeurs que les bruits definis sur la mesure. Mais nous cherchons ici a
 mettre en evidence la distinction entre les parametres du filtrage et les
 parametres de la simulation qui sont ici pour simuler la realite physique.
 Il n'est donc pas pertinent de mettre les meme variables mais plutot de
 definir des valeurs "manuellement" comme cela sera fait pour le filtrage
 reel.

    % Covariance du bruit de modele (eric)
    % -----------------------------
    % (Nota: dans l'article de Sasiadeck [1], la matrice Q d�pend du temps.
    % Une formulation de la matrice de covariance est donn�e dans l'article
    % de C. Ming Wang [2], sous une forme discr�tis�e (la matrice est mise
    % � jour tous les delta t).
    % Une autre formulation est donn�e dans [3]. Dans cette formulation, la
    % matrice est exprim�e explicitement ("closed-form") pour diff�rentes
    % "portions" de chemin, droits ou courbe. Le code matlab est donn� en
    % annexe de l'article.
    % Une autre forme explicite est donn�e dans [4] (avec l'auteur de [3]).
    % Les auteurs montrent le caract�re incorrect du mod�le traditionnel.
    % On trouve aussi
    % Q=[ \sigma r_r Vr, \sigma rl V ; \sigma lr V, \sigma ll Vl]
    % avec V = (Vl+Vr)/2
    % d'o� Qk = Q \Delta_t

*/

// ----------------------------------------
// Matrice de covariance du bruit de modele
// ----------------------------------------
const double Qd[2][2] =
		{ { 0.01, 0 },
		  { 0, 0.01 } };

const T_mat Q = {2,2, _MAT_ Qd };


const T_measure_noise K_mn = {
	0.01, 						// Ecart type de la mesure en position selon x
    0.01, 						// Ecart type de la mesure en position selon y
    0.087 						// Ecart type de la mesure cap
};

// -----------------------------------------------------
// Matrice de covariance des mesures gps et magnetometre
// -----------------------------------------------------
const double RGPSd[2][2] =
			{ {	0.01*0.01, 	0.0			},
			  { 0.0,		0.01*0.01	} };

const T_mat RGPS = { 2,2, _MAT_ RGPSd };

const double RCOMPd[1][1] =
		{{	0.087*0.087 }};

const T_mat RCOMP = { 1,1, _MAT_ RCOMPd };


const double RGPSCOMPd[3][3] =
			{ {	0.01*0.01, 	0.0,		0.0 },
			  { 0.0,		0.01*0.01,	0.0	},
			  { 0.0,		0.0,		0.087*0.087 }} ;

const T_mat RGPSCOMP = {3,3, _MAT_ RGPSCOMPd };

// --------------------------
// Ecart type sur les mesures
// --------------------------
// Attention, ces donn�es ne sont pas n�cessairement strictement
// �gales � celles donn�es dans la matrice de covariance utilis�e par
// le filtre de Kalman.

