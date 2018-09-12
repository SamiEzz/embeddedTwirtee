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

typedef struct Cartography{
	Float32 def_max_speed;
	Float32 def_max_speed_up;
	Int16 nb_arcs;
	Int16 nb_nodes;
	Node nodes[MAX_CARTO_NODES];
} Cartography;

typedef struct Path {
	Float32 def_max_speed;
	Float32 def_max_speed_up;
	UInt16 size;
	Node *dest[MAX_CARTO_NODES];
} Path;
void dijkstra(Cartography *graph, int src, int dest, Path *trajectory);

#endif /* DIJKSTRA_H_ */