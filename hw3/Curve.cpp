#include "Curve.h"

// Implementation of the Curve class in Curve.h
// Note that the draw functions is virtual, and you will implement 
// suitable extensions in the bezier and bspline classes.
// The other functions just perform management on the point list, and 
// you can likely use those in the skeleton code.


// This is a simple curve drawer, that just connects the dots for various 
// points (using Pvector iterators).  Draws a curve with line segments. 
// The final step draws the points themselves, in addition to the lines.

// This does use old-style glBegin... glEnd commands for drawing the lines. 
// We also use old-style opengl commands for glColor3f and glLineWidth.  
// If you are enterprising, you may want to try re-doing this with buffer objs
// The point drawing routines similarly use older OpenGL commands.

void Curve::connectTheDots() {
	Pvector::iterator it, next;
	if (points.size() > 1) {
		for (it = points.begin(); it != points.end()-1; it++) {
			next = it+1;
			glColor3f(0, 1, 0);
			glLineWidth(8.0) ;
			glBegin( GL_LINES );
			glVertex2f(it->x, it->y);
			glVertex2f(next->x, next->y);
			glEnd();
		}
	}
	for (it = points.begin(); it != points.end(); it++) {
		it->draw();
	}
}


// Curve drawing routine.  For the base curve class, it simply calls connect 
// the dots.  However, derived classes like Bezier and Bspline can/will take 
// an additional argument corresponding loosely to level of detail or how 
// many subdivisions/segments to use.  

void Curve::draw(int lod) {
	if (!points.empty()) {
		connectTheDots();
	}
}


// Add a point to the curve.  
// To avoid misclicks, new points are added only if there is no current 
// active point, to avoid ambiguity.  
void Curve::addPoint(float x, float y) {
	if (!isactive) {
		points.push_back(Point(x, y));
	}
}


// Deletes the current active point (hence requires one exists)
// Then resets that there is no active point
void Curve::deleteActivePoint() {
	if (isactive) {
		points.erase(activePoint);
		isactive = false ;
	}
}

// Moves the active point if non-null.
void Curve::moveActivePoint(float dx, float dy) {
	if (isactive) {
		activePoint->x += dx;
		activePoint->y += dy;
	}
}

// Update the active point, when you click on a new one.  
// The first part checks to make sure the click is outside a given 
// radius [i.e. not the same point], and if so, finds the closest point.
// Note that the closest point must be within the radius to be accepted.
void Curve::updateActivePoint(float x, float y) {
	float dx, dy, dist2;
	float radius = Point::ACTIVE_RADIUS*Point::ACTIVE_RADIUS;
	dist2 = 1;
	if (isactive) {
		dx = activePoint->x - x;
		dy = activePoint->y - y;
		dist2 = dx*dx + dy*dy;
	}
	if (dist2 > radius) {
		if (isactive) activePoint->active = false;
		isactive = false ; 
		for (Pvector::iterator it = points.begin(); it != points.end(); it++) {
			dx = it->x - x;
			dy = it->y - y;
			dist2 = dx*dx + dy*dy;
			if (dist2 < radius) {
				activePoint = it;
				it->active = true;
				isactive = true ; 
				break;
			}
		}
	}
}

// Simple auxiliary routine to draw a line between two points.
// Can be called from Bezier/B-Spline curves
// Again, this uses old-style OpenGL
void Curve::drawLine(float x1, float y1, float x2, float y2) {
	glColor3f(0,0,1);
	glBegin( GL_LINES );
	glVertex2f( x1, y1 );
	glVertex2f( x2, y2 );
	glEnd();
}

Curve::Curve()
{
	isactive = false ;
}

Curve::~Curve()
{

}
