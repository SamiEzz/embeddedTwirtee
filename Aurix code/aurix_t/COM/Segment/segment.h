/****{ IRT }******************************************************************
	@file		segment.h													**
	@author		RIPOLL Guillaume											**
	@brief		Segment functions prototypes, used in COM/tracking			**
	Tab_size	4															**
******************************************************************************
*/

#ifndef SEGMENT_H_
#define SEGMENT_H_

typedef struct {
	Bool8 success;
	Bool8 f_parallel;
	Bool8 f_coincident;
	Bool8 f_null;
} Segment_Flags;

/**
	@brief	Generate a Segment from 2 points A, B
	@param	IN Point A
	@param	IN Point B
	@return the generated Segment
*/
Segment segment(Point A, Point B);

/**
	@assert Epsilon must be positive
	@return test in [limit1, limit2] OR [limit2, limit1], +- epsilon
*/
Bool8 inInterval(float test, float limit1, float limit2, float epsilon);

/**
	@brief	Test (s1 intersection s2) != EMPTY
	considering every geometric cases,
	but no side effect on float consideration
	@param	IN Segment s1
	@param	IN Segment s2
	@return result of test (s1 intersection s2) != EMPTY
*/
Bool8 intersects(Segment s1, Segment s2);

/* @brief test C intersects AB */
Bool8 point_intersects(Segment AB, Point C);

/**
	@brief	inter <- intersection(s1, s2)
	considering every geometric cases,
	but no side effect on float consideration
	@param	IN	Segment AB
	@param	IN	Segment CD
	@param	OUT	Point intersection
	@param	OUT	Segment_Flags status (tag success, parallel, colinear & null_vector cases)
*/
void get_intersection(Segment AB, Segment CD, Point *inter, Segment_Flags *status);

/**
	@brief test if segment(ref, p) intersects any left_segment or right_segment
	from segment(side[min_i], side[min_i+1]) to segment(l_side[max_i-1], l_side[max_i])
	@return segment(ref, p) intersects left or right side in given interval
*/
Bool8 test_intersection_point(Point ref, Point         p, int min_i, int max_i, Point l_side[MAX_DELTA_SEGMENTS], Point r_side[MAX_DELTA_SEGMENTS]);

/**
	@brief convert target Position to Point & call test_intersection_point
*/
Bool8 test_intersection_pos  (Point ref, Position target, int min_i, int max_i, Point l_side[MAX_DELTA_SEGMENTS], Point r_side[MAX_DELTA_SEGMENTS]);

/*	@assert (n1 != n2)
	@brief 
		1. Compute the theta a rover should have
		to travel [n1 -> n2] whith a NULL angular speed
		2. Considering this theta and the given distance,
		compute the left & right points from n1
	@param IN  Node n1, main node
	@param IN  Node n2, mandatory to know theta
	@param IN  Float32 d distance bewteen [left..n1] & [n1..right]
	@param OUT Point *left  with angle=(theta + PI_DIV2) and distance=d from n1
	@param OUT Point *right with angle=(theta - PI_DIV2) and distance=d from n1
*/
void side_lr_points(Node n1, Node n2, Float32 d, Point *left, Point *right);
void lr_points(Node n0, Node n, Node n2, Float32 d, Point *left, Point *right);

/**
	@assert 0 <= D <= |AB|
	@return the point C at distance D from A,
	where C is in AB 
*/
Position segPoint_point_node(Point A, Node B, Float32 D);

/**	Heuristic function
	Considering positions A, B
	Let A be the referential
	Considering there is at most 1 intersection in the PATH from A to B
	this functions defines if the intersection would be on the Right
*/
Bool8 is_collision_right(Position A, Node B);

/**
	@brief convert a node to a point
	@param a node n
	@return the point conversion from n
*/
Point node_to_point(Node n);

/**
	@brief from 2 nodes,
	@return the angle value between segment(n1, n2) & Ref(0xy), in RADIAN
*/
Float32 compute_theta(Node n1, Node n2);

#endif ///< SEGMENT_H_
