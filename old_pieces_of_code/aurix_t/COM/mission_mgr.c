/**
 ******************************************************************************
 * @file		mission_mgr.c
 * @author   [IRT] PHAVORIN Guillaume
 * @brief	process mission_manager
 ******************************************************************************
 */

#include "mission_mgr.h"

#include <math.h>
#include <stdlib.h>
#include "com.h"
#include "mission_mgr.h"
#include "dijkstra.h"


/**
 * @brief  return the path: set of consecutive vertices in Cartography containing those defined by the mission
 * @param  mission[]: the set of vertices that defined the mission
 * @param  mission_size: size of mission (number of sets composing the mission)
 * @param  carto: the Cartography
 * @param  path: the computed path if it exists
 * @param  path_size: pointer to the size of computed path (point to 0 is no path found)
 * @retval None
 */
static void get_mission_path(Mission *mission, Cartography *carto, Path *path) {
	int counter, i;
	int path_index = 1;

	Path local_path;

	if (mission->size > 0)
	{
		path->size = 1; ///< Init the path size of the mission to 1
		path->dest[0] = &(carto->nodes[mission->ind[0]]); ///< Set the first element of path as the source of the mission

		/// find the shortest path of the defined mission by applying recursively Dijkstra between adjacent mission's vertices
		for (counter = 0; counter < mission->size - 1; ++counter)
		{
			dijkstra(carto, mission->ind[counter], mission->ind[counter+1], &local_path);
			if (local_path.size == 0)
			{
				path->size = 0;
				return;
			}
			/// Add the path between the 2 vertices of the mission
			for (i = 1; i < local_path.size; ++i)
			{
				path->dest[path_index] = local_path.dest[i];
				++path_index;
			}
			/* the total path size in incremented by the current local path size minus 1,
			 * corresponding to the source point of the local path which is already
			 * the target point of the previous total path */
			path->size = path->size + (local_path.size - 1);
		}
	}
	else
	{
		path->size = 0;
	}
}

/**
 * @brief mission_ordering AADL thread
 * Right now, the mission_ordering just defines the first mission at initialization
 * @evolution receive some missions and order which one will be executed, consider the cancel of a mission
 */
void com_mission_ordering(Mission *mission0, Mission *o_mission, Bool8 *abort) {
	o_mission->size = mission0->size;
	int i = 0;
	for (i=0; i<o_mission->size; ++i)
	{
		o_mission->ind[i] = mission0->ind[i];
	}
	abort = false;
}

/**
 * @brief path_computing AADL thread
 * Right now, just initialize a pass from the mission given.
 * @evolution consider the canceling of a mission with Abort, also recompute a Path from the new Mission
 * @assert path computed has at least 2 elements <-> mission has at least 2 elements
 */
void com_path_computing(Bool8 abort, Mission *mission, Cartography *map, Path *path_r) {
	get_mission_path(mission, map, path_r);
	path_r->def_max_speed	= map->def_max_speed;
	path_r->def_max_speed_up= map->def_max_speed_up;
}

