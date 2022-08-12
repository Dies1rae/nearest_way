#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#define MAX_X 20
#define MAX_Y 100
#define FIELDDOT '.'
#define WSTART '@'
#define WEND '!'
#define WTRACK '*'
#define WALL '#'

struct Point {
	Point() : x_(0), y_(0) {};
	Point(int x, int y) : x_(x), y_(y) {};
	int x_;
	int y_;
};

#endif	//OMMON_H
