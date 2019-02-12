//======================================================================
//  Auteur : Eric JENN 
//  (c) IRT Saint-Exupery
//
//----------------------------------------------------------------------  
// Filtre de Kalman
//======================================================================

#ifndef KALMAN_H_
#define KALMAN_H_

#include "mat.h"

// Filtrage de Kalman.
// IN:
// - dt : intervalle d'échantillonnage
// - wl_cons, wr_cons : vitesses angulaire des roues mesurées
// - X_estp : état estimé
// - covp : matrice de covariance
// OUT: 
// - locp->val, headp->val : valeur mise à jour (élimination des "outliers")
// - X_estp : état estimé
// - covp : matrice de covariance


typedef struct kalman_param {
	double dt,wl_mes,wr_mes;
	T_loc *locp;
	T_head *headp;
	T_mat *X_estp;
	T_mat *Pp;
};
void *kalman(void * k_in);

#endif


