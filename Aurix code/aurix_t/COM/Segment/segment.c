/****{ IRT }******************************************************************
	@file		segment.c													**
	@author		RIPOLL Guillaume											**
	@brief		Geometric functions used in COM/tracking					**
	Tab_size	4															**
******************************************************************************
*/
#include <math.h>
#include <stdlib.h>
#include "com.h"
#include "segment.h"
#define EPSILON 0.00001f ///< imprecision margin
#define PI_DIV2 1.57079632679

/**
	@brief	Generate a Segment from 2 points A, B
	@param	IN Point A
	@param	IN Point B
	@return the generated Segment
*/
Segment segment(Point A, Point B) {
	Segment s;
	s.p1 = A;
	s.p2 = B;
	return s;
}

/**
	@assert Epsilon must be positive
	@return test in [limit1, limit2] OR [limit2, limit1], +- epsilon
*/
Bool8 inInterval(float test, float limit1, float limit2, float epsilon) {
	return limit1 <= limit2 ?
			(test >= (limit1-epsilon) && test <= (limit2+epsilon)) :
			(test >= (limit2-epsilon) && test <= (limit1+epsilon)) ;
}

/**
	@brief	Test (s1 intersection s2) != EMPTY
	considering every geometric cases,
	but no side effect on float consideration
	@param	IN Segment s1
	@param	IN Segment s2
	@return result of test (s1 intersection s2) != EMPTY
*/
Bool8 intersects(Segment AB, Segment CD) {
	Segment_Flags status;
	Point	A = AB.p1,
			B = AB.p2,
			C = CD.p1,
			D = CD.p2;
	float r, s, num, denum;
	status.f_null = (
		((B.x-A.x) == 0.0f && (B.y-A.y) == 0.0f) ||
		((D.x-C.x) == 0.0f && (D.y-C.y) == 0.0f) );
	num		= (A.y-C.y)*(D.x-C.x) - (A.x-C.x)*(D.y-C.y);
	denum	= (B.x-A.x)*(D.y-C.y) - (B.y-A.y)*(D.x-C.x);

	// [TODO] (ejn) To be corrected!

	/* if the denominator is null, then segments are either colinear (ie parallel) or coincident */
	if (denum == 0)
//	if (abs(denum) <= EPSILON)
	{
		status.f_parallel = true;
		/* if the numerator is null, as the denominator is also null, then segments are coincident */
		if (num == 0)
//		if (abs(num) <= EPSILON)
		{
			status.f_coincident = true;
			/* as segments are coincident, there is an intersection only if:
			 * C is in [A, B] or
			 * D is in [A, B] or
			 * if both C and D are outside [A, B] then that means there is an intersection only if [A, B] is in [C, D] */
			return (( inInterval(C.x, A.x, B.x, EPSILON) && inInterval(C.y, A.y, B.y, EPSILON) ) ||
					( inInterval(D.x, A.x, B.x, EPSILON) && inInterval(D.y, A.y, B.y, EPSILON) ) ||
					( inInterval(A.x, C.x, D.x, EPSILON) && inInterval(A.y, C.y, D.y, EPSILON) ) );
		}
		else
		{
			status.f_coincident = false;
			/* segments are strictly parallel so there is no intersection */
			return false;
		}
	}
	else
	{
		status.f_parallel = false;
		status.f_coincident = false;

		/* segment intersection is checked using lines defined in terms of first degree Bézier parameters */
		r = num / denum;
		num = (A.y-C.y)*(B.x-A.x) - (A.x-C.x)*(B.y-A.y);
		s = num / denum;
		return (0<= r&&r <=1) && (0<= s&& s<=1);
//		return (inInterval(r, 0, 1, EPSILON) && inInterval(s, 0, 1, EPSILON));
	}
//	status.f_parallel = (!denum);
//	status.f_colinear = status.f_parallel && (!num);
//
//	if(!status.f_parallel)
//	{
//		r = num / denum;
//		num = (A.y-C.y)*(B.x-A.x) - (A.x-C.x)*(B.y-A.y);
//		s = num / denum;
//		return (0<= r&&r <=1) && (0<= s&& s<=1);
//	}
//
//	return	status.f_colinear && (
//			( inInterval(C.x, A.x, B.x, EPSILON) && inInterval(C.y, A.y, B.y, EPSILON) ) ||
//			( inInterval(D.x, A.x, B.x, EPSILON) && inInterval(D.y, A.y, B.y, EPSILON) ) ||
//			( inInterval(A.x, C.x, D.x, EPSILON) && inInterval(A.y, C.y, D.y, EPSILON) ) );
}

/**
	@brief	get the intersection between 2 segments
	considering every geometric cases,
	but no side effect on float consideration
	@param	IN	Segment AB
	@param	IN	Segment CD
	@param	OUT	Point intersection
	@param	OUT	Segment_Flags status (tag success, parallel, colinear & null_vector cases)
*/
void get_intersection(Segment AB, Segment CD, Point *inter, Segment_Flags *status) {
	Point	A = AB.p1,
			B = AB.p2,
			C = CD.p1,
			D = CD.p2;
	float r, s, num, denum;
	status->f_null = (
		((B.x-A.x) == 0.0f && (B.y-A.y) == 0.0f) ||
		((D.x-C.x) == 0.0f && (D.y-C.y) == 0.0f) );
	if(status->f_null) {
		status->success =	(A.x == C.x)&&
							(A.y == C.y);
		if(status->success) {
			inter->x = A.x;
			inter->y = A.y;
		}
	} else {
		num		= (A.y-C.y)*(D.x-C.x) - (A.x-C.x)*(D.y-C.y);
		denum	= (B.x-A.x)*(D.y-C.y) - (B.y-A.y)*(D.x-C.x);
		// [TODO] (ejn) Correct the next lines! Change the way the status is returned!
		if (denum == 0)
//		if (abs(denum) <= EPSILON)
		{
			status->f_parallel = true;
			if (num == 0)
//			if (abs(num) <= EPSILON)
			{
				status->f_coincident = true;
				if( inInterval(C.x, A.x, B.x, EPSILON) && inInterval(C.y, A.y, B.y, EPSILON) ) {
					status->success = true;
					inter->x = C.x; //< at least C solution, but possible overlap
					inter->y = C.y;
				} else if ( inInterval(D.x, A.x, B.x, EPSILON) && inInterval(D.y, A.y, B.y, EPSILON) ) {
					status->success = true;
					inter->x = D.x; //< at least D solution, but possible overlap
					inter->y = D.y;
				} else if ( inInterval(A.x, C.x, D.x, EPSILON) && inInterval(A.y, C.y, D.y, EPSILON) ) {
					status->success = true;
					inter->x = A.x; //< at least A solution, but possible overlap
					inter->y = A.y;
				} else status->success = false;
			}
			else
			{
				status->f_coincident = false;
				/* segments are strictly parallel so there is no intersection */
				status->success = false;
			}
		}
		else
		{
			status->f_parallel = false;
			status->f_coincident = false;

			r = num / denum;
			num = (A.y-C.y)*(B.x-A.x) - (A.x-C.x)*(B.y-A.y);
			s = num / denum;
			status->success = (0<= r&&r <=1) && (0<= s&& s<=1);
			inter->x = A.x + r*(B.x-A.x);
			inter->y = A.y + r*(B.y-A.y);
		}
//		status->f_parallel = (!denum);
//		status->f_colinear = status->f_parallel && (!num);
//		if(!status->f_parallel){
//			r = num / denum;
//			num = (A.y-C.y)*(B.x-A.x) - (A.x-C.x)*(B.y-A.y);
//			s = num / denum;
//			status->success = (0<= r&&r <=1) && (0<= s&& s<=1);
//			inter->x = A.x + r*(B.x-A.x);
//			inter->y = A.y + r*(B.y-A.y);
//		}else if(status->f_colinear) {
//			if( inInterval(C.x, A.x, B.x, EPSILON) && inInterval(C.y, A.y, B.y, EPSILON) ) {
//				status->success = true;
//				inter->x = C.x; //< at least C solution, but possible overlap
//				inter->y = C.y;
//			} else if ( inInterval(D.x, A.x, B.x, EPSILON) && inInterval(D.y, A.y, B.y, EPSILON) ) {
//				status->success = true;
//				inter->x = D.x; //< at least D solution, but possible overlap
//				inter->y = D.y;
//			} else if ( inInterval(A.x, C.x, D.x, EPSILON) && inInterval(A.y, C.y, D.y, EPSILON) ) {
//				status->success = true;
//				inter->x = A.x; //< at least A solution, but possible overlap
//				inter->y = A.y;
//			} else status->success = false;
//		} else status->success = false;
	}
}

/**
	@brief test if segment(ref, p) intersects any left_segment or right_segment
	from segment(side[min_i], side[min_i+1]) to segment(l_side[max_i-1], l_side[max_i])
	@return segment(ref, p) intersects left or right side in given interval
*/
Bool8 test_intersection_point(Point ref, Point p, int min_i, int max_i, Point l_side[MAX_DELTA_SEGMENTS], Point r_side[MAX_DELTA_SEGMENTS]) {
	Segment ref_to_p, side;
	ref_to_p = segment(ref, p);
	int i = 0;
	for(i = min_i; i<=max_i-2; ++i) {
		side = segment(l_side[i], l_side[i+1]);
		if(intersects(ref_to_p, side))
			return true;
		side = segment(r_side[i], r_side[i+1]);
		if(intersects(ref_to_p, side))
			return true;
	}
	return false;
}

Bool8 test_intersection_pos(Point ref, Position target, int min_i, int max_i, Point l_side[MAX_DELTA_SEGMENTS], Point r_side[MAX_DELTA_SEGMENTS]) {
	Point p;
	p.x = target.x;
	p.y = target.y;
	return test_intersection_point(ref, p, min_i, max_i, l_side, r_side);
}

/**	@assert (n1 != n2)
	@brief 
		Computes the left-side and right-side poses located at a distance d from n1 and with an
		angle at 90 degrees of [n1,n2], directed away from n1
	@param IN  Node n1
	@param IN  Node n2
	@param IN  Float32 d
	@param OUT Point *left  with angle=(theta + PI_DIV2) and POINTDIST=d from n1
	@param OUT Point *right with angle=(theta - PI_DIV2) and POINTDIST=d from n1
*/
void side_lr_points(Node n1, Node n2, Float32 d, Point *left, Point *right) {
	Float32 theta = acos((n2.x-n1.x) / POINTDIST(n1, n2));
	if(n1.y > n2.y)
		theta = - theta;
	left->x	 = n1.x + d*cos(theta+PI_DIV2);
	right->x = n1.x + d*cos(theta-PI_DIV2);
	left->y	 = n1.y + d*sin(theta+PI_DIV2);
	right->y = n1.y + d*sin(theta-PI_DIV2);
}

/**
	@brief
		Computes the left-side and right-side poses located at a distance d from n1 and with an
		angle at 90 degrees of the bissector of [n1,n],[n,n2], directed away from n1.
*/
void lr_points(Node n1, Node n, Node n2, Float32 d, Point *left, Point *right) {
	Float32 theta_1 = (n1.y > n.y) ? - acos((n.x-n1.x) / POINTDIST(n1, n)) : acos((n.x-n1.x) / POINTDIST(n1, n)),
			theta_2 = (n.y > n2.y) ? - acos((n2.x-n.x) / POINTDIST(n, n2)) : acos((n2.x-n.x) / POINTDIST(n, n2)),
			theta = ( theta_1 + theta_2 )*0.5f;
	left->x	 = n.x + d*cos(theta+PI_DIV2);
	right->x = n.x + d*cos(theta-PI_DIV2);
	left->y	 = n.y + d*sin(theta+PI_DIV2);
	right->y = n.y + d*sin(theta-PI_DIV2);
}

/**
	@assert 0 <= dist <= |AB|
	@return point C at distance d from A,
	where C is in AB 
*/
Position segPoint_point_node(Point A, Node B, Float32 dist) {
	Float32 AB = POINTDIST(A, B);
	Float32 theta = (A.y > B.y) ? -acos((B.x-A.x) / AB) : acos((B.x-A.x) / AB);
	Position C;
	C.x = A.x + dist*cos(theta);
	C.y = A.y + dist*sin(theta);
	C.theta = theta;
	return C;
}

/* tests if B is at left of A considering A.theta */
Bool8 is_collision_right(Position A, Node B) {
	Float32 theta_AB = acos((B.x-A.x) / POINTDIST(A, B));
	if(A.y > B.y)
		theta_AB = -theta_AB;
	theta_AB = theta_AB + A.theta;
	return (cos(theta_AB) > 0);
}

/**
	@brief convert a node to a point
	@param a node n
	@return the point conversion from n
*/
Point node_to_point(Node n) {
	Point p;
	p.x = n.x;
	p.y = n.y;
	return p;
}

/**
	@brief from 2 nodes,
	@return the angle value between segment(n1, n2) & Ref(0xy), in RADIAN
*/
Float32 compute_theta(Node n1, Node n2) {
	return (n1.y > n2.y  ? -acos((n2.x-n1.x) / POINTDIST(n1, n2)) : acos((n2.x-n1.x) / POINTDIST(n1, n2)) );
}
