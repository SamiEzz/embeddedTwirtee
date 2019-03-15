/**
******************************************************************************
* @file		guidance_mgr.c
* @author   [IRT] RIPOLL Guillaume, JENN Eric
* @brief	C code from the AADL process guidance_manager
* [TODO] (ejn) Code to be completely reviewed and simplified:
		- Coding rules to be applied.
		- Code to be commented.
		- Function descriptions to be clarified.
		- Variable names to be chosen appropriately.
		- etc.
[History]
	- (2017) Initial version (gr)
	- (08/2018) Slight corrections, documentation, cleanup (ejn)
******************************************************************************
*/
#include <math.h>
#include <stdlib.h>
#include "../include/com.h"
#include "../include/guidance_mgr.h"
#include "../include/segment.h"


/**
 * @param IN Node *n1
 * @param IN Node  *n2
 * @return TRUE if node n1 and n2 are exactly at the same location.
 [TODO] (ejn) To be checked. n1 and n2 should be "equal" iff they point to the same element!
*/
static Bool8 node_eq(Node *n1, Node *n2) {
	return(n1->x == n2->x && n1->y == n2->y);
}

/**
	@brief Return the speed and accel constraint for the p_ind-th element of path p.
	[TODO] (ejn) Replace "speed_up" by "accel" !!!
	[TODO] (ejn) Replace "arcs" by "edges" !!!
*/
static void get_speed_constraints(Path *p, int p_ind, Float32 *speed, Float32 *speed_up) {
	int i = 0;
	int i_path = p_ind < (p->size-1) ? p_ind : (p_ind-1);
	Node start = *(p->dest[i_path]);
	*speed = -1.0f;
	*speed_up = -1.0f;
	// Traverse all edges
	for(i = 0; i<start.nb_a; ++i)
	{
		if( node_eq(start.arcs[i].dest, p->dest[i_path+1]) )
		{
			*speed = start.arcs[i].max_speed;
			*speed_up = start.arcs[i].max_speed_up;
			break;
		}
	}
	// If no explicit speed constraints are given on the edges, use the default values.
	// [TODO] (ejn) Suppress this and simply initialize speed and speed_up to the default values!
	if(*speed == -1.0f)
		*speed = p->def_max_speed;
	if(*speed_up == -1.0f)
		*speed_up = p->def_max_speed_up;
}

/**
	@abstractions acceleration & deceleration time, collisions, end_point
	@brief considering a point H in segment (Path[i_path-1]..Path[i_path]) corresponding to the current position,
	the def_speed_constraints setted in path and specific speed constraints that may be setted in arc nodes (path.dest[node_id].arcs[arc_in_path].speed)
	@return the max_range a setpoint may reach,
	@param IN	Point	H
	@param IN	Path	p
	@param IN	p_ind	index which starts the path
*/
static Float32 get_max_range(Point H, Path *p, int p_ind) {
	Float32 s,
			s_up,
			dist,
			cum_dist = 0.0f,	// Cumulated distance
			dt = DELTA_TIME;
	int i = 0;

	// Traverse all segments in the path, from the p_ind-th segment and
	// accumulate the distance.
	for(i=p_ind; i<=(p->size-2); ++i)
	{
		// Compute the distance traversed during dt for the segment.
		get_speed_constraints(p, i, &s, &s_up);
		dist = s*dt;

		// Have we
		if( dist <= POINTDIST(H, (*(p->dest[i+1]))) )
		{
			// We are unable to go to the next node, so the loop is complete.
			cum_dist = cum_dist + dist;
			return cum_dist;
		}
		else
		{
			// We are able to consume the distance to the next node, so: do it!
			dist = POINTDIST(H,(*(p->dest[i+1])));
			cum_dist = cum_dist + dist;
			// It took dist/s units of time to cover that distance, so compute the time left.
			dt = dt - (dist / s);
			// As there is some time left, go to the next node.
		}
	}
	get_speed_constraints(p, p->size-1, &s, &s_up);
	return s*DELTA_TIME;
}

/*	@brief Considering a random position P,
	a segment AB == segment(Path with prev_index)
	@return when exists the H point in AB which verify PH perpendicular to AB
	if it does not exist, return the nearest Point (A or B) to P
*/
static Point projection(Position p, Path *path, int prev_index) {
	Float32 U, U_div, minX, minY, maxX, maxY;
	Point A, B, H;
	if(prev_index == 0)
	{
		A.x = path->dest[0]->x;
		A.y = path->dest[0]->y;
		B.x = path->dest[1]->x;
		B.y = path->dest[1]->y;
	} else
	{
		A.x = path->dest[prev_index-1]->x;
		A.y = path->dest[prev_index-1]->y;
		B.x = path->dest[prev_index]->x;
		B.y = path->dest[prev_index]->y;
	}

	if(A.x==B.x && A.y==B.y)
		return A;

	U = ((p.x-A.x)*(B.x-A.x))+((p.y-A.y)*(B.y-A.y));
	U_div = ((B.x-A.x)*(B.x-A.x))+((B.y-A.y)*(B.y-A.y));
	U = U / U_div;
	H.x = A.x + (U*(B.x-A.x));
	H.y = A.y + (U*(B.y-A.y));

	if(A.x <= B.x)
	{
		minX = A.x;
		maxX = B.x;
	}
	else
	{
		minX = B.x;
		maxX = A.x;
	}

	if(A.y <= B.y)
	{
		minY = A.y;
		maxY = B.y;
	}
	else
	{
		minY = B.y;
		maxY = A.y;
	}

	// Check if H is actually on the segment
	if(	(minX <= H.x && H.x <= maxX) &&
		(minY <= H.y && H.y <= maxY)  )
	{
		// YES!
		return H;
	}
	else
	{
		// NO: H is not on a segment, so return the closest point *** on the segment ***.
		if (POINTDIST(A, H) <= POINTDIST(B, H))
			return A;
		else
			return B;
	}
}

/**
 * @brief Returns TRUE if a node must be "flied-by".
*/
static Bool8 is_mandatory(Node n) {
	return( n.nt == TARGET || n.nt == STATION);
}

/**
 * @brief Returns TRUE if the distance between pose p and node n is less than EPSILON on both axes.
*/
static Bool8 eq_pos(Position p, Node n, Float32 epsilon) {
	Float32 diff_x = p.x - n.x,
			diff_y = p.y - n.y;
	return (diff_x < epsilon && diff_x > -epsilon)
		&& (diff_y < epsilon && diff_y > -epsilon);
}

/**
 * @brief Computes the pose of the robot at the end of segment (n_1,n_2).
   The angle is relative to 0x.
*/
static Position get_pose_at_last_node(Node n_1, Node n_2) {
	Position p;
	p.x = n_2.x;
	p.y = n_2.y;
	p.theta = (n_1.y > n_2.y) ? -acos((n_2.x-n_1.x) / POINTDIST(n_1, n_2)) : acos((n_2.x-n_1.x) / POINTDIST(n_1, n_2));
	return p;
}

/**
	@brief	GetSetPoint algorithm,
			aim : generate the setpoint which verifies following rules :
			1. The setpoint must be the farthest in the PATH
			2. If the robot rotates to more than 90� he'll have a NULL shifting
			3. The gap between robot travel and PATH must never exceed D
			4. Some points are mandatory and the robot must not deviate at all from those points
	@param D > 0		: allowed distance between PATH and ROBOT
	@param MAX_RANGE 	: distance to the setpoint in the best case.
	@param path_r must have at least 2 elements
	[TODO] (ejn) This algorithm seems a bit complicated. It seems that the constraint about distance d
	could be handled in a simpler way than computing a left and right path.
	[TODO] (ejn) This function has a nasty side effect!!!! (on prev_pos, prev_index; l_side). See if it could be avoided! 
		It could be avoided by performing the complete calculation each and every cycle. This would be less efficient, 
		but much simpler to analyse in terms of WCET...
	[TODO] (ejn) Replace the returned value (0,1,...) by an enumeration!
	[TODO] (ejn) Provide a sensible documentation fort this function (e.g.: the returned value is not documented).
*/
static int get_setpoint(Position pos_r, Path *path_r, Position *pos_sp)
{
	// State variables
	static Bool8	is_init 	= true;
	static Position	prev_pos;
	static int		prev_index	= 0, ///< last followed index
					i_side		= 0;

	// l_side and r_side contain the left and right "borders" of the path.
	static Point	l_side[MAX_CARTO_NODES],
					r_side[MAX_CARTO_NODES];

	int		ind					= prev_index,
			side_start;
	Bool8	flag_range			= false,	///< max range reached
			flag_collision		= false;	///< intersection reached
	Node	npos;
	Point	pos,
			H;					///< Closest point to the robot on the path.
	float	distance	 		= 0.0f,
			max_range ;

	// Compute the point of the path that is the closest to the robot.
	H = projection(pos_r, path_r, prev_index);

	// Compute the maximal distance that could be covered by the robot considering the speed constraints.
	// The *** actual *** distance may be shorter in order to prevent the robot to take "shortcuts" from its
	// current location to the target. (Consider for instance a path with a small curvature radius.
	max_range = get_max_range(H, path_r, prev_index);

	// Set the index of the left and right side pathes.
	if ( prev_index == 0 )
	{
		side_start = 0;
	}
	else
	{
		side_start = prev_index - 1;
	}

	// Are we in the last segment of the path and the last node can be reached during this cycle?
	// [TODO] (ejn) Is the second condition necessary?
	if((prev_index==path_r->size-1) && eq_pos(prev_pos, *(path_r->dest[path_r->size-1]), max_range))
	{
		// YES: the target is the last node.
		*pos_sp = get_pose_at_last_node(*(path_r->dest[path_r->size-2]), *(path_r->dest[path_r->size-1]));
		// We can return.
		return 0;
	}

	// NO, we are not at the last segment of the path.

	// Move the target setpoint as far as we can, until either the complete range is covered OR the segment
	// (H, target setpoint) intersects the left or right sides (which means that the cross-track
	// constraint is not satisfied.
	// Note that we may also get out of the loop because the
	while( (!flag_range) && (!flag_collision) )
	{
		npos = *(path_r->dest[ind]);
		pos.x = npos.x;
		pos.y = npos.y;

		// Compute the distance from H to the next point.
		distance = POINTDIST(H, *(path_r->dest[ind]));

		// Have we covered the maximal range?
		flag_range = (distance > max_range);
		if(!flag_range)
		{	//	NO, we have not covered the maximal range.
			// Build the left and right side paths.
			while(i_side <= ind)
			{
				// Distinguish between the case where
				// - there is a unique segment (i.e., 2 nodes)
				// - there are two segments (i.e., 3 nodes)
				if(i_side > path_r->size-2)
				{
					// Compute the next side point when there is only a unique segment to be covered.
					side_lr_points(	*(path_r->dest[i_side]), *(path_r->dest[i_side-1]),
									_D_,
									r_side+i_side, l_side+i_side);
				}
				else
				// [TODO] (ejn) This case doesn't seem specific. To be removed?
				if(i_side == 0)
				{
					//
					side_lr_points(	*(path_r->dest[0]), *(path_r->dest[1]),
									_D_,
									l_side, r_side);
				}
				else
				{
					// Compute the next side point when there are more than one segment to be covered.
					lr_points(*(path_r->dest[i_side-1]), *(path_r->dest[i_side]), *(path_r->dest[i_side+1]),
									_D_,
									l_side+i_side, r_side+i_side);
				}
				++i_side;
			}

			// Now that the side paths have been built, check if (H, target) intersects the left or right path.

			if(i_side > 0)
			{
				// Check if there is an intersection with the "side" path, starting from "side_start" up to i_side.
				flag_collision = test_intersection_point(H, pos, side_start, i_side, l_side, r_side);
			}

			// Is there an intersection?
			if(! flag_collision)
			{
				// NO, (H, target) does not intersect the left or right path.

				// If the node a "fly-by" node and we are not already at that point, the next target is that node.
				// [TODO] (ejn) Beware of those "==" when dealing with floating points... To be checked.
				if((is_mandatory(npos) &&! (H.x==npos.x && H.y==npos.y)) || ind == (path_r->size-1) )
				{
					// YES: it is a fly-by node.
					Point start;
					// [TODO] (ejn) replace by "if (ind > 0) ... "
					if(ind)
					{
						start.x = path_r->dest[ind-1]->x;
						start.y = path_r->dest[ind-1]->y;
					} else
					{
						start.x = H.x;
						start.y = H.y;
					}

					// Set the next target (which is a "fly-by" node.
					pos_sp->x = npos.x;
					pos_sp->y = npos.y;
					pos_sp->theta = acos((npos.x-start.x) / POINTDIST(start, npos));
					pos_sp->theta = (start.y > npos.y) ? -pos_sp->theta : pos_sp->theta;
					prev_index = ind;

					if(is_init)
					{
						prev_pos = *pos_sp;
						is_init = false;
						return 1;
					}
					else
					if ( (ind==path_r->size-1) || (!(pos_sp->x == prev_pos.x && pos_sp->y == prev_pos.y && pos_sp->theta == prev_pos.theta)) )
					{
						prev_pos = *pos_sp;
						return 2;
					}
				}
				// Move to the next node
				++ind;
			}
		}
	}

	// We have moved the target point as far as we could.
	// Have we covered the maximal range?
	if(flag_range)
	{
		// YES, we have covered the maximal range.
		// So the target is at max_range from H, on the current segment.
		*pos_sp = segPoint_point_node(H, *(path_r->dest[ind]), max_range);

		// Check if the segment from H to the target intersects the left or right side.
		flag_collision = test_intersection_pos(H, *pos_sp, side_start, i_side, l_side, r_side);

		// If there an intersection?
		if(!flag_collision)
		{
			// NO: the target is OK, we update the target angle.
			// [TODO] (ejn) The test is useless, to be suppressed.
			if ( ind > 0 )
				pos_sp->theta = compute_theta(*(path_r->dest[ind-1]), *(path_r->dest[ind]));
			else
				pos_sp->theta = compute_theta(*(path_r->dest[0]), *(path_r->dest[1]));

			prev_index = ind;
			is_init = false;

			prev_pos = *pos_sp;

			// Computation complete.
			return 3;
		}
	}

	// Is there an intersection?
	if(flag_collision)
	{
		// YES, there is a collision. We have to find the farthest point that does not intersect
		Position pos_finds_side;
		Point ip; ///< intersection point
		Point *side;

		pos_finds_side.x = H.x;
		pos_finds_side.y = H.y;
		pos_finds_side.theta = acos((path_r->dest[ind]->x-H.x) / POINTDIST(H, *(path_r->dest[ind])));

		if(H.y > path_r->dest[ind]->y)
			pos_finds_side.theta = -pos_finds_side.theta;

		// Determine which side segment (left or right) intersects
		// [TODO] (ejn) Change function name to "is_at_right_side_of")
		if ( is_collision_right(pos_finds_side, *(path_r->dest[ind])) )
		{
			// YES
			side = r_side;
		}
		else
		{
			side = l_side;
		}

		Segment_Flags status;

		// Now that we know the side, compute the farthest point, which is located at the intersection of
		// [H,I] and the next path segment.
		// [TODO] (ejn) This is an approximation: the target could be farther than that.
		if(ind > 0)
			get_intersection(segment(H, side[ind-1]), segment(node_to_point(*(path_r->dest[ind-1])), node_to_point(*(path_r->dest[ind]))), &ip, &status);
		else
			get_intersection(segment(H, side[0]), segment(node_to_point(*(path_r->dest[0])), node_to_point(*(path_r->dest[ind]))), &ip, &status);

		// Do [H,I] and the next path segment intersect?
		if(status.success || !status.f_parallel)
		{
			// YES they intersect.
			// The new target is the intersection point.
			pos_sp->x = ip.x;
			pos_sp->y = ip.y;
			// Update the target angle.
			pos_sp->theta = ind > 0 ? compute_theta(*(path_r->dest[ind-1]), *(path_r->dest[ind])) : compute_theta(*(path_r->dest[0]), *(path_r->dest[1]));
			prev_index = ind;
			is_init = false;
			prev_pos = *pos_sp;
			return 4;
		}

		// [TODO] (ejn) Check what happens if [H,I] and the next path segment do not intersect.
		// Is it even possible?
	}
	return -1;
}

/*

*/
static void clamp_v_to_sp(Position pos, Position pos_sp, float *v)
{
	float dist = POINTDIST(pos, pos_sp);
	if(*v > dist*CALL_PER_SEC) {
		*v = dist*CALL_PER_SEC;
	}
}
//------------------------------------------------------------------------------------------------------------------------------------




//------------------------------------------------------------------------------------------------------------------------------------
void com_tracking(Position pos, Position pos_sp, T_mat *Q, Speed *speed_sp)
{
	static Bool8 is_init = true;
	static Position prev_setpoint;
	static float vr, wr;
	Position error;

	if(is_init) {
		prev_setpoint = pos;
		is_init = false;
	}
	/// Call kanayama algo
	get_error_vector(pos, pos_sp, &error);
	get_ref_speed(pos_sp, prev_setpoint, &vr, &wr);
	clamp_v_to_sp(pos, pos_sp, &vr);
	kanayama(vr, wr, error, &speed_sp->v, &speed_sp->w);
	prev_setpoint = pos_sp;
}

void com_generation(Position pos, Path *path_r, Position *pos_sp)
{
	get_setpoint(pos, path_r, pos_sp);
}

