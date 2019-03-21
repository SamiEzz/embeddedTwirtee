//======================================================================
//  Auteur : Eric JENN
//  (c) IRT Saint-Exupery
//
//----------------------------------------------------------------------
// Filtre de Kalman
//======================================================================
//#include <math.h>
#include "../include/kalman.h"
#include "../include/parametres.h"
#include <math.h>

// ----------------------------------------------------------------------
// Fonction d'élimination des outliers
// ----------------------------------------------------------------------
// Cette fonction retourne OK si le vecteur de mesure retourné par les capteurs est cohérent
// avec la valeur de l'état courant et de la matrice de covariance.
// On applique un critère basé sur la loi du Khi 2 d'ordre 3, p=0.95.
// Attention : le calcul de la distance de Mahalanobis repose sur le calcul de l'inverse
// de la matrice de covariance. Il faut donc que celle-ci soit inversible.
// TODO: traiter le cas où la matrice est singulière.
// IN:
// - Xp:
// - locp : mesure de la position (x,y)
// - headp : mesure du cap
// - Pp : matrice de covariance
// OUT:
// - locp->val
// - headp->val
// TODO: traiter la validité des mesures séparément.
void check_mes(const T_mat* Xp, const T_mat* Pp, T_loc* locp, T_head* headp) {
    double TMP1d[3][3];
    T_mat TMP1 = {3, 3, _MAT_ TMP1d};
    double Xd[3][1];
    T_mat X = {3, 1, _MAT_ Xd};
    double XMd[3][1] = {{locp->x}, {locp->y}, {headp->theta}};
    T_mat XM = {3, 1, _MAT_ XMd};
    double TMP2d[1];
    T_mat TMP2 = {1, 1, _MAT_ TMP2d};

    // X = etat - mesure;
    mat_sub(Xp, &XM, &X);

    // calcul de la distance de Mahalanobis : (X'*P^-1*X)^(1/2)
    mat_inv(Pp, &TMP1);
    mat_mul_2b(&X, &TMP1, &TMP2);
    double d_mahalanobis = sqrt(TMP2d[0]);

    const double k_square = 7.82; // loi du Khi 2 d'ordre 3 a 95%
    locp->val &= (d_mahalanobis <= k_square);
    headp->val &= (d_mahalanobis <= k_square);
}

// ----------------------------------------------------------------------
// Filtrage de Kalman
// ----------------------------------------------------------------------
// IN:
// - dt : intervalle d'échantillonnage
// - wl_cons, wr_cons : vitesses angulaire des roues mesurées
// - X_estp : état estimé
// - covp : matrice de covariance
// OUT:
// - locp->val, headp->val : valeur mise à jour (élimination des "outliers")
// - X_estp : état estimé
// - covp : matrice de covariance

void kalman(double dt, double wl_mes, double wr_mes, // Vitesse des roues mesurées
            T_loc* locp, T_head* headp,              // Mesures capteurs
            T_mat* X_estp,                           // Etat estimé
            T_mat* Pp)                               // matrice de covariance
{

    // --------------------
    //  Phase de prediction
    // --------------------

    // Construction du vecteur de commande U
    double vl_mes = wl_mes * K_rp.lwd / 2.0;
    double vr_mes = wr_mes * K_rp.rwd / 2.0;

    double Matvitd[2][2] = {{-0.5, -0.5}, {1.0 / K_rp.width, -1.0 / K_rp.width}};

    double v_cons = Matvitd[0][0] * vl_mes + Matvitd[0][1] * vr_mes;
    double w_cons = Matvitd[1][0] * vl_mes + Matvitd[1][1] * vr_mes;

    double Ud[2] = {v_cons, w_cons};
    T_mat U = {2, 1, _MAT_ Ud};

    double theta_est = (*X_estp->m)[2];

    // Prédiction de l'état
    // X_pred = X_est + G*U
    double Gd[3][2] = {{-dt * sin(theta_est + Ud[1] * dt / 2.0), 0.0},
                       {dt * cos(theta_est + Ud[1] * dt / 2.0), 0.0},
                       {0.0, dt}};
    T_mat G = {3, 2, _MAT_ Gd};

    double TMP1d[3][1];
    T_mat TMP1 = {3, 1, _MAT_ TMP1d};
    double X_predd[3];
    T_mat X_pred = {3, 1, _MAT_ X_predd};

    mat_mul(&G, &U, &TMP1);
    mat_add(X_estp, &TMP1, &X_pred);

    // Prediction de la covariance
    // Calcul du jacobien df/dX
    double JFd[3][3] = {{1.0, 0.0, -Ud[0] * dt * cos(theta_est + Ud[1] * dt / 2.0)},
                        {0.0, 1.0, -Ud[0] * dt * sin(theta_est + Ud[1] * dt / 2.0)},
                        {0.0, 0.0, 1.0}};

    T_mat JF = {3, 3, _MAT_ JFd};

    // Calcul du jacobien df/dU
    double JQd[3][2] = {{-dt * sin(theta_est + Ud[1] * dt / 2.0),
                         -0.5 * Ud[0] * dt * dt * cos(theta_est + Ud[1] * dt / 2.0)},
                        {dt * cos(theta_est + Ud[1] * dt / 2.0),
                         -0.5 * Ud[0] * dt * dt * sin(theta_est + Ud[1] * dt / 2.0)},
                        {0.0, 1.0}};

    T_mat JQ = {3, 2, _MAT_ JQd};

    // Calcul de la prédiction de l'erreur : P(k+1 | k)
    // P(k+1 | k) = JF(k)P(k)JF(k)' + JQ(k)Q(k)JQ(k)')
    // ou P = JF * P * JF' + JQ * Q * JQ';
    double TMP2d[3][3];
    T_mat TMP2 = {3, 3, _MAT_ TMP2d};
    double TMP3d[3][3];
    T_mat TMP3 = {3, 3, _MAT_ TMP3d};

    mat_mul_2(&JF, Pp, &TMP2);
    mat_mul_2(&JQ, &Q, &TMP3);
    mat_add(&TMP2, &TMP3, Pp);

    //  --------------------------------------------------
    //  Phase de mise a jour de l'estimation
    //  --------------------------------------------------

    // Vérification de la cohérence des mesures
    // L'indicateur de validité est mis à jour directement.
    
    //check_mes(X_estp, Pp, locp, headp);

    if (headp->val && !locp->val) {
        //  --------------------------------------------------
        //  Traitement de la mesure de cap
        //  --------------------------------------------------
        // H est la matrice de passage de l'espace d'état vers l'espace de mesure
        double Hd[1][3] = {{0.0, 0.0, 1.0}};
        T_mat H = {1, 3, _MAT_ Hd};

        // Calcul de ZP, la mesure prédite
        // zp= H*X_pred;
        double ZPd[1][1];
        T_mat ZP = {1, 1, _MAT_ ZPd};
        mat_mul(&H, &X_pred, &ZP);

        // Calcul de Z, la mesure effective
        double Zd[1][1] = {{headp->theta}};
        T_mat Z = {1, 1, _MAT_ Zd};

        // Calcul de l'innovation : I = Z - ZP
        double Id[1][1];
        T_mat I = {1, 1, _MAT_ Id};
        mat_sub(&Z, &ZP, &I);

        // Calcul de S
        // On ne prend en compte que l'erreur sur la position
        // S = H*P*H' + RCOMP;
        double Sd[1][1];
        T_mat S = {1, 1, _MAT_ Sd};
        mat_mul_2(&H, Pp, &S);
        mat_add(&S, &RCOMP, &S);

        // Calcul du gain de Kalman
        // On rappelle que S est semi-définie positive.
        // K = P*H'*pinv(S);
        double Kd[3][1];
        T_mat K = {3, 1, _MAT_ Kd};
        double SId[1][1];
        T_mat SI = {1, 1, _MAT_ SId};
        double HTd[3][1];
        T_mat HT = {3, 1, _MAT_ HTd};
        double TMP4d[3][1];
        T_mat TMP4 = {3, 1, _MAT_ TMP4d};

        mat_transp(&H, &HT);
        mat_inv(&S, &SI);
        mat_mul(&HT, &SI, &TMP4);
        mat_mul(Pp, &TMP4, &K);

        // Mise a jour de la covariance
        // P = ( eye(3) - K*H )*P;
        double TMP5d[3][3];
        T_mat TMP5 = {3, 3, _MAT_ TMP5d};
        double TMP6d[3][3];
        T_mat TMP6 = {3, 3, _MAT_ TMP6d};
        double TMP7d[3][3];
        T_mat TMP7 = {3, 3, _MAT_ TMP7d};

        mat_mul(&K, &H, &TMP5);
        mat_sub(&K_EYE_3_3, &TMP5, &TMP6);
        mat_mul(&TMP6, Pp, &TMP7);
        mat_copy(Pp, &TMP7);

        double TMP8d[3][1];
        T_mat TMP8 = {3, 1, _MAT_ TMP8d};

        // Mise à jour de l'etat
        // X_est = X_est + K*I;
        mat_mul(&K, &I, &TMP8);
        mat_add(X_estp, &TMP8, X_estp);

    } else if (locp->val && !headp->val) {
        //  --------------------------------------------------
        // Traitement de la mesure de position
        //  --------------------------------------------------
        // H est la matrice de passage de l'espace d'état vers l'espace de mesure
        double Hd[2][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}};
        T_mat H = {2, 3, _MAT_ Hd};

        // Calcul de ZP, la mesure prédite
        double ZPd[2][1];
        T_mat ZP = {2, 1, _MAT_ ZPd};

        //  zp= H * X_pred;
        mat_mul(&H, &X_pred, &ZP);

        // Calcul de Z, la mesure effective
        double Zd[2][1] = {{locp->x}, {locp->y}};
        T_mat Z = {2, 1, _MAT_ Zd};

        // Calcul de l'innovation : I = Z - ZP
        double Id[2][1];
        T_mat I = {2, 1, _MAT_ Id};
        mat_sub(&Z, &ZP, &I);

        // Calcul de S
        // On ne prend en compte que l'erreur sur la position
        // S = H*P*H' + RGPS;
        double Sd[2][2];
        T_mat S = {2, 2, _MAT_ Sd};
        mat_mul_2(&H, Pp, &S);
        mat_add(&S, &RGPS, &S);

        // Calcul du gain de Kalman
        // On rappelle que S est semi-définie positive.
        // K = P*H'*pinv(S);
        double Kd[3][2];
        T_mat K = {3, 2, _MAT_ Kd};

        double SId[2][2];
        T_mat SI = {2, 2, _MAT_ SId};

        double HTd[3][2];
        T_mat HT = {3, 2, _MAT_ HTd};

        double TMP4d[3][2];
        T_mat TMP4 = {3, 2, _MAT_ TMP4d};

        mat_transp(&H, &HT);
        mat_inv(&S, &SI);
        mat_mul(&HT, &SI, &TMP4);
        mat_mul(Pp, &TMP4, &K);

        // Mise a jour de la covariance
        // P = ( eye(3) - K*H)*P;
        double TMP5d[3][3];
        T_mat TMP5 = {3, 3, _MAT_ TMP5d};
        double TMP6d[3][3];
        T_mat TMP6 = {3, 3, _MAT_ TMP6d};
        double TMP7d[3][3];
        T_mat TMP7 = {3, 3, _MAT_ TMP7d};

        mat_mul(&K, &H, &TMP5);
        mat_sub(&K_EYE_3_3, &TMP5, &TMP6);
        mat_mul(&TMP6, Pp, &TMP7);
        mat_copy(Pp, &TMP7);

        double TMP8d[3][1];
        T_mat TMP8 = {3, 1, _MAT_ TMP8d};
        // Mise à jour de l'etat
        // X_est = X_est + K*I;
        mat_mul(&K, &I, &TMP8);
        mat_add(X_estp, &TMP8, X_estp);

    } else if (locp->val && headp->val) { // Les deux mesures sont valides
        // H est la matrice de passage de l'espace d'état vers l'espace de mesure
        double Hd[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};

        T_mat H = {3, 3, _MAT_ Hd};

        // Calcul de ZP, la mesure prédite
        double ZPd[3][1];
        T_mat ZP = {3, 1, _MAT_ ZPd};

        //  zp= H* X_pred;
        mat_mul(&H, &X_pred, &ZP);

        // Calcul de Z, la mesure effective
        double Zd[3][1] = {{locp->x}, {locp->y}, {headp->theta}};
        T_mat Z = {3, 1, _MAT_ Zd};

        // Calcul de l'innovation : I = Z - ZP
        double Id[3][1];
        T_mat I = {3, 1, _MAT_ Id};
        mat_sub(&Z, &ZP, &I);

        // Calcul de S
        // On ne prend en compte que l'erreur sur la position
        // S = H*P*H' + RGPSCOMP;
        double Sd[3][3];
        T_mat S = {3, 3, _MAT_ Sd};
        mat_mul_2(&H, Pp, &S);
        mat_add(&S, &RGPSCOMP, &S);

        // Calcul du gain de Kalman
        // On rappelle que S est semi-définie positive.
        // K = P*H'*pinv(S);
        double Kd[3][3];
        T_mat K = {3, 3, _MAT_ Kd};

        double SId[3][3];
        T_mat SI = {3, 3, _MAT_ SId};

        double HTd[3][3];
        T_mat HT = {3, 3, _MAT_ HTd};

        double TMP4d[3][3];
        T_mat TMP4 = {3, 3, _MAT_ TMP4d};

        mat_transp(&H, &HT);
        mat_inv(&S, &SI);

        double XXd[3][3];
        T_mat XX = {3, 3, _MAT_ XXd};
        mat_mul(&S, &SI, &XX);
        // mat_print(&XX);

        mat_mul(&HT, &SI, &TMP4);
        mat_mul(Pp, &TMP4, &K);

        // Mise a jour de la covariance
        // P = ( eye(3) - K*H)*P;
        double TMP5d[3][3];
        T_mat TMP5 = {3, 3, _MAT_ TMP5d};
        double TMP6d[3][3];
        T_mat TMP6 = {3, 3, _MAT_ TMP6d};
        double TMP7d[3][3];
        T_mat TMP7 = {3, 3, _MAT_ TMP7d};

        mat_mul(&K, &H, &TMP5);
        mat_sub(&K_EYE_3_3, &TMP5, &TMP6);
        mat_mul(&TMP6, Pp, &TMP7);
        mat_copy(Pp, &TMP7);

        double TMP8d[3][1];
        T_mat TMP8 = {3, 1, _MAT_ TMP8d};
        // Mise à jour de l'etat
        // X_est = X_est + K*I;
        mat_mul(&K, &I, &TMP8);
        mat_add(X_estp, &TMP8, X_estp);
    } else { // if ( !locp->val && !headp->val ) {
        // La position estimée est la position prédite (pas de correction)
        mat_copy(X_estp, &X_pred);
    }
}
