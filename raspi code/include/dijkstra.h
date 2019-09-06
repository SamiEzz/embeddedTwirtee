/*
 * Dijkstra.h
 *
 *  Created on: Aug 29, 2016
 *	  Author: mf.sassi
 */

#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_
#include "twirtee.h"

#define true 1
#define false 0
#define INT_MAX 2147483647
#define MAX_SPEED 0.3
#define MAX_SPEED_UP 0.5



// --------------------------------------------------

/*
typedef struct Mission {
	int ind[CARTO_NODE_NBR];
	UInt16 size;
} Mission;
*/

static int minDistance(float dist[], Bool8 sptSet[], UInt16 graph_size);
static Float32 get_speed(Arc *a, Cartography *c);

void dijkstra(Cartography *graph, int src, int dest, Path *trajectory);
void initCarto(Cartography * cart, Node *n,int nb_nodes);

#endif /* DIJKSTRA_H_ */
