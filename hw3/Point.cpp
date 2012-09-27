#include <GL/glut.h>
#include <math.h>
#include "Point.h"

const float Point::ACTIVE_RADIUS = 0.05;

// Implements the methods in Point.h 
// The draw technique 
// For active points, this is just a circle to highlight the point. 
// In any event, a small box around the point.  

// Again, this uses old-style OpenGL with glBegin and glEnd for drawing.
// It's just placing vertices in a circle.  
// The point is drawn with a quad, using old-style OpenGL

void Point::draw() {
	if (active) {
		glColor3f(1, 0, 0);
		glBegin(GL_LINE_LOOP);
		float u,v;
		for (float t = 0; t < 2*3.14159; t+= 0.1) {
			u = cos(t)*ACTIVE_RADIUS + x;
			v = sin(t)*ACTIVE_RADIUS + y;
			glVertex3f(u, v, 0);
		}
		glEnd();
	}
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glVertex2f(x-0.01, y-0.01);
	glVertex2f(x+0.01, y-0.01);
	glVertex2f(x+0.01, y+0.01);
	glVertex2f(x-0.01, y+0.01);
	glEnd();

}

Point::Point(float xpos, float ypos)
{
	x = xpos;
	y = ypos;
	active = false;
}

Point::~Point()
{

}
