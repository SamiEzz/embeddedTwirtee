//======================================================================
//  Auteur : Eric JENN 
//  (c) IRT Saint-Exupery
//
//----------------------------------------------------------------------  
// Fonction de calcul matriciel élémentaires.
// ATTENTION : l'implémentation choisie est la plus "évidente" (straightforward)
// 			   possible afin de conserver des marges de progrès...
//======================================================================

#include <stdlib.h>
#include <string.h>
#include "com.h"
#include "mat.h"

// =====================================================================
// CONSTANTES
// =====================================================================

const char* kINCONS_SIZES = "inconsistent sizes";
const char* kSINGULAR = "singular matrix";
const char* kNON_SQUARE = "non square matrix";
const char* kNOT_IMPLEMENTED= "function not implemented yet";
const char* kEIG_ERROR="impossible to calculate eigenvalues/vectors";

// Matrice identité 3x3
const double K_EYE_3_3d[3][3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, { 0.0, 0.0, 1.0} };
const T_mat K_EYE_3_3 = {3,3, _MAT_ K_EYE_3_3d};

// =====================================================================
// TYPES
// =====================================================================

// Aucun

// =====================================================================
// FONCTIONS
// =====================================================================

// --------------------------------------
// Multiplication par un scalaire in situ
// --------------------------------------
void mat_mul_scal( T_mat *Ap, double k ) 
{
	int i, j; 	// i parcourt les colonnes, j parcourt les lignes 
		
	for ( i=0; i<Ap->c; i++)
		for ( j=0; j<Ap->l; j++)
			(*Ap->m)[j*Ap->c+i] *=k;
}

// -------------------
// Copie d'une matrice B=>A
// -------------------
void mat_copy(  T_mat *Ap, const T_mat *Bp ) 
{
	int s;
	if ( ( Bp->l == Ap->l) && ( Bp->c == Ap->c )) 
	{
		s = Ap->l*Ap->c*sizeof(double);
		memcpy(Ap->m,Bp->m,s);
	}
	else
		return;
}

// -------------------------------
// Addition de deux matrices
// -------------------------------
void mat_add( const T_mat *Ap, const T_mat *Bp, T_mat *Cp) 
{
	
	// Vérification de la compatibilité des tailles 
	if ( ( Ap->l == Bp->l )&& 
		 ( Ap->c == Bp->c) && 
		 ( Cp->l == Ap->l) &&
		 ( Cp->c == Ap->c) ) 
	{
		int i,j; 	// i parcourt les colonnes, j parcourt les lignes 

		for ( i=0; i<Cp->c; i++)
			for ( j=0; j<Cp->l; j++)
				(*Cp->m)[j*Cp->c+i] = (*Ap->m)[j*Ap->c+i]+(*Bp->m)[j*Bp->c+i];		
	}
	else
		return;

}

// -------------------------------
// Soustraction de deux matrices
// -------------------------------
void mat_sub( const T_mat *Ap, const T_mat *Bp, T_mat *Cp) 
{
	
	// Vérification de la compatibilité des tailles 
	if ( ( Ap->l == Bp->l )&& 
		 ( Ap->c == Bp->c) && 
		 ( Cp->l == Ap->l) &&
		 ( Cp->c == Ap->c) ) 
	{
		int i,j; 	// i parcourt les colonnes, j parcourt les lignes 
		
		for ( i=0; i<Cp->c; i++)
			for ( j=0; j<Cp->l; j++)
				(*Cp->m)[j*Cp->c+i] = (*Ap->m)[j*Ap->c+i]-(*Bp->m)[j*Bp->c+i];		
	}
	else
		return;

}
 
// -------------------------------
// Multiplication de la forme C=A*B
// -------------------------------

void mat_mul( const T_mat *Ap, const T_mat *Bp, T_mat *Cp) 
{
	
	// Vérification de la compatibilité des tailles 
	if ( ( Ap->c == Bp->l) && 
		 ( Cp->l == Ap->l) &&
		 ( Cp->c == Bp->c) ) 
	{
		int ia,ic,jc; 	// i parcourt les colonnes, j parcourt les lignes 
		
		for ( ic=0; ic<Cp->c; ic++)
			for ( jc=0; jc<Cp->l; jc++)
			{
				(*Cp->m)[jc*Cp->c+ic] = 0.0;
				for ( ia=0; ia<Ap->c; ia++)
						(*Cp->m)[jc*Cp->c+ic] += (*Ap->m)[jc*Ap->c+ia]*(*Bp->m)[ia*Bp->c+ic];
			}		
	}
	else
		return;

}

// -------------------------------
// Multiplication de la forme C=A*B'
// -------------------------------

void mat_mul_1( const T_mat *Ap, const T_mat *Bp, T_mat *Cp) 
{

	// Vérification de la compatibilité des tailles 
	if ( ( Ap->c == Bp->c) && 
		 ( Cp->l == Ap->l) &&
		 ( Cp->c == Bp->l) ) 
	{
		int ia,ic,jc; 	// i parcourt les colonnes, j parcourt les lignes 
		
		for ( ic=0; ic<Cp->c; ic++)
			for ( jc=0; jc<Cp->l; jc++)
			{
				(*Cp->m)[jc*Cp->c+ic] = 0.0;
				for ( ia=0; ia<Ap->c; ia++)
						(*Cp->m)[jc*Cp->c+ic] += (*Ap->m)[jc*Ap->c+ia]*(*Bp->m)[ic*Bp->c+ia];
			}	
	}
	else
		return;
}

// -------------------------------
// Multiplication de la forme C=A'*B
// -------------------------------

void mat_mul_1b( const T_mat *Ap, const T_mat *Bp, T_mat *Cp) 
{
	
	// Vérification de la compatibilité des tailles 
	if ( ( Ap->l == Bp->l) && 
		 ( Cp->l == Ap->c) &&
		 ( Cp->c == Bp->c) ) 
	{
		int ia,ic,jc; 	// i parcourt les colonnes, j parcourt les lignes 
		
		for ( ic=0; ic<Cp->c; ic++)
			for ( jc=0; jc<Cp->l; jc++)
			{
				(*Cp->m)[jc*Cp->c+ic] = 0.0;
				for ( ia=0; ia<Ap->l; ia++)
						(*Cp->m)[jc*Cp->c+ic] += (*Ap->m)[ia*Ap->c+jc]*(*Bp->m)[ia*Bp->c+ic];
			}		
	}
	else
		return;
}

// -------------------------------
// Multiplication de la forme C = A*B*A'
// -------------------------------
void mat_mul_2( const T_mat *Ap, const T_mat *Bp, T_mat *Cp)
{
	T_mat tmp;
	mat_create(Bp->l,Ap->l, &tmp);
	mat_mul_1(Bp,Ap,&tmp);
	mat_mul(Ap,&tmp,Cp); 
	mat_del(&tmp);
}

// -------------------------------
// Multiplication de la forme C = A'*B*A
// -------------------------------
void mat_mul_2b( const T_mat *Ap, const T_mat *Bp, T_mat *Cp)
{
	T_mat tmp;
	mat_create(Bp->l,Ap->c, &tmp);
	mat_mul(Bp,Ap,&tmp);
	mat_mul_1b(Ap,&tmp,Cp); 
	mat_del(&tmp);
}

// -----------------------------------------------
// Créé une matrice de taille (l,c) et l'initialise
// -----------------------------------------------
void mat_create ( int l, int c, T_mat *Ap)
{
	int i, j;	// i parcourt les colonnes, j parcourt les lignes 
	Ap->m = (double (*)[])malloc((l*c)*sizeof(double));
	Ap->l = l;
	Ap->c = c;

	for ( i=0; i< Ap->c; i++)
		for ( j=0; j< Ap->l; j++)
			(*Ap->m)[j*c+i] = 0.0;

}


// --------------------
// Supprime une matrice
// --------------------
void mat_del( T_mat *Ap)
{
	free(Ap->m);
}

// ------------------------------------
// Calcule le déterminant d'une matrice
// ------------------------------------
void mat_det ( const T_mat *Ap, double *det )
{
	// Vérifier que la matrice est bien carrée
	if ( Ap->l == Ap->c )	
	{
		// Traitement des cas simples
		switch (Ap->l) 
		{
			case 1:
				*det = (*Ap->m)[0];
				break;
			case 2: 
				{
					double (*m)[2][2] = (double (*)[2][2]) Ap->m;
					*det = (*m)[0][0]*(*m)[1][1]-(*m)[0][1]*(*m)[1][0];
				}
				break;
			case 3:
				{
					double (*m)[3][3] = (double (*)[3][3]) Ap->m;
					*det = (*m)[0][0]*((*m)[1][1]*(*m)[2][2]-(*m)[2][1]*(*m)[1][2])-
						   (*m)[1][0]*((*m)[0][1]*(*m)[2][2]-(*m)[2][1]*(*m)[0][2])+
						   (*m)[2][0]*((*m)[0][1]*(*m)[1][2]-(*m)[1][1]*(*m)[0][2]);
				}
				break;
			default:
				return;
		}

	}
	else
		return;
} 

// Calcule la matrice des cofacteurs d'une matrice


// --------------------
// Calcule l'inverse d'une matrice
// --------------------
void mat_inv ( const T_mat *Ap, T_mat *Bp ) 
{
	double det;

	// Vérifier que la matrice est bien carrée
	if ( (Ap->l == Ap->c) &&
		 (Bp->l == Bp->c) &&
		 (Bp->l == Ap->l) )
	{
		mat_det ( Ap, &det );
		if ( det == 0.0 )
			return;
		
		switch (Ap->l) 
		{
			case 1:
				(*Bp->m)[0] = 1.0/det;
				break;
			case 2:
				{
					double (*a)[2][2] = (double (*)[2][2]) Ap->m;
					double (*b)[2][2] = (double (*)[2][2]) Bp->m;
					// Calcul de la matrice adjointe (transposée de la matrice des cofacteurs)
					(*b)[0][0] = (*a)[1][1];
					(*b)[0][1] = -(*a)[0][1];
					(*b)[1][0] = -(*a)[1][0];
					(*b)[1][1] = (*a)[0][0];
					// Calcul de la matrice inverse
					mat_mul_scal(Bp,1.0/det);
				}
				break;
			case 3:
				{
					double (*a)[3][3] = (double (*)[3][3]) Ap->m;
					double (*b)[3][3] = (double (*)[3][3]) Bp->m;	
					// Calcul de la matrice adjointe (transposée de la matrice des cofacteurs)
					(*b)[0][0] = (*a)[1][1]*(*a)[2][2]-(*a)[2][1]*(*a)[1][2];
					(*b)[0][1] = -((*a)[0][1]*(*a)[2][2]-(*a)[2][1]*(*a)[0][2]);
					(*b)[0][2] = (*a)[0][1]*(*a)[1][2]-(*a)[1][1]*(*a)[0][2];
					(*b)[1][0] = -((*a)[1][0]*(*a)[2][2]-(*a)[2][0]*(*a)[1][2]);
					(*b)[1][1] = (*a)[0][0]*(*a)[2][2]-(*a)[2][0]*(*a)[0][2];
					(*b)[1][2] = -((*a)[0][0]*(*a)[1][2]-(*a)[1][0]*(*a)[0][2]);
					(*b)[2][0] = (*a)[1][0]*(*a)[2][1]-(*a)[2][0]*(*a)[1][1];
					(*b)[2][1] = -((*a)[0][0]*(*a)[2][1]-(*a)[2][0]*(*a)[0][1]);
					(*b)[2][2] = (*a)[0][0]*(*a)[1][1]-(*a)[1][0]*(*a)[0][1];
					// Calcul de la matrice inverse
					mat_mul_scal(Bp,1.0/det);
				}
				break;
			default:
				return;
		}
	}
	else
		return;
}

// -----------------------------------------------
// Transposition
// -----------------------------------------------
void mat_transp ( const T_mat *Ap, T_mat *Bp)
{
	int i,j;	// i parcourt les colonnes, j parcourt les lignes 
	if ( (Ap->l == Bp->c) && (Ap->c == Bp->l) )	
		for ( i = 0; i< Ap->c; i++)
			for ( j = 0; j< Ap->l; j++)
				(*Bp->m)[i*Bp->c+j] = (*Ap->m)[j*Ap->c+i];
	else
		return;
}
