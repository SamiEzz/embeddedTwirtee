/*
 * Dijkstra.h
 *
 *  Created on: Aug 29, 2016
 *	  Author: mf.sassi
 */

#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#define true 1
#define false 0
#define INT_MAX 2147483647

/* first test: 2 vertices */
static int minDistance(float dist[], Bool8 sptSet[], UInt16 graph_size);
static Float32 get_speed(Arc *a, Cartography *c);

void dijkstra(Cartography *graph, int src, int dest, Path *trajectory);
#endif /* DIJKSTRA_H_ */