//======================================================================
//  Auteur : Eric JENN 
//  (c) IRT Saint-Exupery
//
//----------------------------------------------------------------------  
// Fonction de calcul matriciel élémentaires.
// ATTENTION :	l'implémentation choisie est la plus "évidente" (straightforward)
//				possible. Dans la mesure où on s'intéresse essentiellement à des 
//				matrices, on provilégieles solutions analytiques aux solutions
//				itératives. 
//======================================================================
 
#ifndef MAT_H_
#define MAT_H_

#include "com.h"

// =====================================================================
// TYPES
// =====================================================================

// Type matrice.
//typedef struct {
//	int l;		// Nombre de lignes
//	int c;		// Nombre de colonnes.
//	double (*m)[];
//} T_mat;
 
// =====================================================================
// CONSTANTES
// =====================================================================

#define _MAT_ (double (*)[])
extern const T_mat K_EYE_3_3;

// =====================================================================
// FONCTIONS
// =====================================================================

void mat_mul_scal( T_mat *Ap, double k );

// Copie d'une matrice B=>A
void mat_copy(  T_mat *Ap, const T_mat *Bp );

// Creation d'une matrice
// La zone de données est allouée dans le tas.
void mat_create ( int l, int c, T_mat *Ap);

// Destruction d'une matrice
// A n'utiliser que si la matrice a été créée
void mat_del( T_mat *Ap);

void mat_inv ( const T_mat *Ap, T_mat *Bp );

// Multiplication de deux matrices
// C = A*B
void mat_mul( const T_mat *Ap, const T_mat *Bp, T_mat * Cp );
// C = A*B'
void mat_mul_1( const T_mat *Ap, const T_mat *Bp, T_mat *Cp );
// C = A'*B
void mat_mul_1b( const T_mat *Ap, const T_mat *Bp, T_mat *Cp);
// C = A*B*A'
void mat_mul_2( const T_mat *Ap, const T_mat *Bp, T_mat *Cp );
// C = A'*B*A
void mat_mul_2b( const T_mat *Ap, const T_mat *Bp, T_mat *Cp);

// Transposition
void mat_transp ( const T_mat *Ap, T_mat *Bp);

// Déterminant d'une matrice (cas 1x1, 2x2 et 3x3).
void mat_det ( const T_mat *Ap, double *det );

// Addition de deux matrices
// C = A+B
void mat_add( const T_mat *Ap, const T_mat *Bp, T_mat *Cp);

// Soustraction de deux matrices
// C = A-B
void mat_sub( const T_mat *Ap, const T_mat *Bp, T_mat *Cp);

#endif
