/*
 * Dijkstra.c
 *
 *  Created on: Aug 29, 2016
 *  Modified on: Jun 05, 2018
 *	  Authors: mf.sassi, gph
 */

#include <math.h>
#include "../include/com.h"
#include "../include/dijkstra.h"


/* A utility function to find the vertex with minimum distance value, from
 * the set of vertices not yet included in shortest path tree
 */
/*
 * graph_size is used to limit the search space to the actual size of the graph
 */
static int minDistance(float dist[], Bool8 sptSet[], UInt16 graph_size)
{
	// Initialize min value
	float min = (float) INT_MAX;
	int min_index;
	int v;

	for (v = 0; v < graph_size; ++v)
	{
		if (sptSet[v] == false && dist[v] <= min)
		{
			min = dist[v];
			min_index = v;
		}
	}
	return min_index;
}

/**
 * @brief An arc MAY have a speed constraint. When it's not,
 * the default speed constraint is defined by the Cartography.
 * @param IN Arc
 * @param IN Cartography
 * @return the actual speed constraint given an Arc
*/
static Float32 get_speed(Arc *a, Cartography *c)
{
	return a->max_speed == -1 ? c->def_max_speed : a->max_speed;
}

float ndistance(Node * n1,Node * n2){
	return(sqrt((n1->x-n2->x)*(n1->x-n2->x)+(n1->y-n2->y)*(n1->y-n2->y)));
}


/* Funtion that implements Dijkstra's single source shortest path algorithm
 * for a graph represented using adjacency matrix representation.
 * trajectory contains the sequence of vertices between source and other sources: first element of each row is the path size
 */
void dijkstra(Cartography *graph, int src, int dest, Path *trajectory)
{
	Bool8 sptSet[graph->nb_nodes];	// sptSet[i] will true if vertex i is included in path tree or shortest distance from src to i is finalized
	int count,
		pt_index,
		node_index,
		i, u, v, v_index;
	float dist[graph->nb_nodes];
	float dist_uv = 0.0;
	int predecessor[graph->nb_nodes];
	UInt16 size[graph->nb_nodes];
	UInt16 graph_size;

	graph_size = graph->nb_nodes;

	// Initialize all distances as INFINITE and stpSet[] as false
	for (i = 0; i < graph_size; ++i)
	{
		dist[i] = INT_MAX;
		sptSet[i] = false;
		predecessor[i] = -1;
		size[i] = 0;
	}

	// Distance of source vertex from itself is always 0
	dist[src] = 0;
	size[src] = 1;

	 // Find the shortest path for all vertices
	for (count = 0; count < graph_size; ++count)
	{
		// Pick the minimum distance vertex from the set of vertices not
		// yet processed. u is always equal to src in first iteration.
		u = minDistance(dist, sptSet, graph_size);

		// Mark the picked vertex as processed
		sptSet[u] = true;

		// Update dist value of the adjacent vertices of the picked vertex.
		for (v = 0; v < (graph->nodes+u)->nb_a; ++v)
		{
			//dist_uv = POINTDIST((graph->nodes+u), *(graph->nodes+u)->arcs[v].dest) / get_speed(&(graph->nodes+u)->arcs[v], graph);
			Node * n1,*n2;
			n1 = (graph->nodes+u);
			n2 = (graph->nodes+u)->arcs[v].dest;
			dist_uv = ndistance(n1,n2)/get_speed(&(graph->nodes+u)->arcs[v], graph);
			/* the corresponding index in graph->nodes table is retrieved using pointer arithmetic */
			v_index = (int) (((graph->nodes+u)->arcs[v].dest) - graph->nodes);
			/* Update dist[v] only if is not in sptSet
			 * and total weight of path from src to v through u
			 * is smaller than current value of dist[v] */
			if (!sptSet[v_index] && (dist[u] != INT_MAX) && ((dist[u]+dist_uv) < dist[v_index]))
			{
				dist[v_index] = dist[u] + dist_uv;
				predecessor[v_index] = u;
				size[v_index] = size[u] + 1;
			}
		}
	}

	if (dist[dest] == (float) INT_MAX)
	{
		trajectory->size = 0;
	}
	else
	{
		trajectory->size = size[dest];
		node_index = dest;
		pt_index = trajectory->size - 1;
		while (node_index != src)
		{
			trajectory->dest[pt_index] = (graph->nodes+node_index);
			--pt_index;
			node_index = predecessor[node_index];
			if (node_index == -1) //case of a BUG -> no path assumed
			{
				trajectory->size = 0;
				return;
			}
		}
		trajectory->dest[0] = (graph->nodes+src);
	}
}
