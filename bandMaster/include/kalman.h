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
void kalman(
    double dt, double wl_mes, double wr_mes, // Vitesse des roues
    T_loc* locp, T_head* headp,              // Mesures capteurs
    T_mat* X_estp,                           // Etat estimé
    T_mat* covp);                            // Matrice de covariance				// Matrice de covariance

#endif

void kalman(double dt, double wl_mes, double wr_mes, // Vitesse des roues
            T_loc* locp, T_head* headp,              // Mesures capteurs
            T_mat* X_estp,                           // Etat estimé
            T_mat* covp);                            // Matrice de covariance