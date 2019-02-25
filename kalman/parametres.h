//======================================================================
//  Auteur : Eric JENN
//  (c) IRT Saint-Exupery
//
//----------------------------------------------------------------------
// Fichier de définition des données utilisées par le filtre de Kalman
//======================================================================

#ifndef PARAMETRES_H_
#define PARAMETRES_H_

#include "mat.h"

// Paramètres relatifs au robot
typedef struct {
    double lwd;					// Diametre de la roue gauche en m
    double rwd;					// Diametre de la roue droite en m
    double width;				// Distance entre les deux roues en m
    double sigma_lw_noise;		// Ecart type vitesse roue gauche
    double sigma_rw_noise;		// Ecart type vitesse roue droite
} T_robot_parameters;


extern const T_robot_parameters K_rp;

//// Affichage des paramètres du robot
//extern void print_robot_parameters();

// ----------------------------------------
// Matrice de covariance du bruit de modele
// ----------------------------------------
extern const double Qq[2][2];
extern const T_mat Q;

// -----------------------------------------------------
// Matrice de covariance des mesures gps et magnetometre
// -----------------------------------------------------
extern const double RGPSd[2][2];
extern const T_mat RGPS;

extern const double RCOMPd[1][1];
extern const T_mat RCOMP;

extern const double RGPSCOMPd[3][3];
extern const T_mat RGPSCOMP;

// --------------------------
// Ecart type sur les mesures
// --------------------------
// Attention, ces données ne sont pas nécessairement strictement
// égales à celles données dans la matrice de covariance utilisée par
// le filtre de Kalman.
typedef struct {
	double sigma_x_noise; 		// Ecart type de la mesure en position selon x par le GPS
    double sigma_y_noise; 		// Ecart type de la mesure en position selon y par le GPS
    double sigma_theta_noise; 	// Ecart type de la mesure cap par le magnetometre
	double sigma_thetal_noise;	// Ecart type de la mesure odometrique roue gauche par le capteur hall
	double sigma_thetar_noise;	// Ecart type de la mesure odometrique roue droite par le capteur hall
} T_measure_noise;

extern const T_measure_noise K_mn;

//// Affiche bruit de mesure
//extern void print_measure_noise();

#endif /* PARAMETRES_H_ */
