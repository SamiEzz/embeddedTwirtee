/**
 * @file localization.c
 * @author Guillaume PHAVORIN
 * @brief Regroupe les fonctions du process Localization de l'unit� COM
 */
#include "../include/localization.h"

#include "../include/com.h"
#include "../include/kalman.h"
#include "../include/mat.h"
#include "../include/parametres.h"
#include <math.h>
#define PI_DIV_2 1.57079632679
#define PI_DIV_4 0.78539816339
/*
extern double v, w;

// Cette fonction calcule les vitesses de rotation des roues (omega left, omega right) en
// fonction des consignes de vitesse tangentielle et de vitesse de rotation.
// IN:
// - v_cons: vitesse tangentielle de consigne (m/s)
// - w_cons: vitesse de rotation de consigne (rad/s)
// OUT:
// - wl_cons: vitesse de rotation roue gauche (rad/s)
// - wr_cons: vitesse de rotation roue droite (rad/s)
static void compute_command(double v_cons, double w_cons, double *wl_cons, double *wr_cons ) {
        *wl_cons = (-v_cons+K_rp.width/2.0*w_cons)*2.0/K_rp.rwd;
        *wr_cons = (-v_cons-K_rp.width/2.0*w_cons)*2.0/K_rp.lwd;
}
*/

/** @brief If in the general referential, a linear speed 1 with a null angular speed
        from a position (x=0, y=0, T=0 rad) would imply a position (x=1, y=0, T=0 rad),
        kalman would imply (x=0, y=-1, T=0 rad).
        Axis change and that's why this interface is needed : change only for Kalman call time the
   referential.
*/

static void kalman_interface(T_head compass, T_loc gps, T_odo odometry, Position* pos) {
    static double estimation_pos[3][1];
    static T_mat estimation = {3, 1, _MAT_ estimation_pos};
    static double q_err_arr[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
    static T_mat q_err = {3, 3, _MAT_ q_err_arr};
    static Bool8 is_init = true;
    static int count = 0;

    if (is_init) {
        pos->x = gps.x;
        pos->y = gps.y;
        pos->theta = compass.theta;
        estimation_pos[0][0] = -pos->y;
        estimation_pos[1][0] = pos->x;
        estimation_pos[2][0] = pos->theta;
        is_init = false;
    }

    /** Adapt devices referential to Kalman - devices aren't output parameters, so these changes are
     * local */
    Float32 last_x = gps.x;
    gps.x = -gps.y;
    gps.y = last_x;

    // gps.val = true;
    compass.val &= gps.val; // false; ///< bug with Kalman (when Compass valid but not GPS, )

    kalman(KALMAN_SAMPLE_PERIOD, odometry.l_speed, odometry.r_speed, &gps, &compass, &estimation,
           &q_err);
    pos->x = (*estimation.m)[1];
    pos->y = -(*estimation.m)[0];
    pos->theta = (*estimation.m)[2];

    ++count;
}

/**
 * @param gps Variable containing the GPS tag measurements
 * @param compass Variable containing the value measured by the compass
 */
void com_localize(T_head compass, T_loc gps, T_odo odometry, Position* pos, T_mat* Q) {
    /// Call Kalman Filter
    kalman_interface(compass, gps, odometry, pos);
}
