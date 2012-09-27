/***********************************************************************/
/***** The Skeleton Code for the Curves Assignment for HW 4 in CS 184 **/
/***** Originally written by Aner Ben-Artzi several years ago      *****/
/***** Modified in Dec 2009 by Ravi Ramamoorthi for more comments, *****/
/***** and consistency with modern C++ rules                         ***/
/***** Modified in Dec 2011 by Ravi Ramamoorthi to be consistent    ****/

#include <GL/glut.h>
#include "shaders.h"
#include "WorkingScene.h"


WorkingScene scene; // the scene information 
GLuint vertexshader, fragmentshader, shaderprogram; // shaders

// Basic display and main setup routines.  
// Display just does basic OpenGL stuff to display 2D curve
// The main routine just sets up a basic display function


void display() {	
	//First, the ModelViewProjection Matrix is setup
	//this determines how the scene will be viewed
	float m[] = {	2.0, 0, 0, 0,
		0, 2.0, 0, 0,
		0, 0, 2.0, 0,
		-1.0, -1.0, 0, 1.0};
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(m); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Second, the framebuffer is filled with the objects in the scene
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (scene.theOnlyCurve) {
		scene.theOnlyCurve->draw(scene.levelOfDetail);
	}
	glutSwapBuffers();
}

// The initialization function for curves does almost nothing. 
// It sets up pass-through (no-op) shaders.
// Since we are drawing in 2D, the shaders don't really matter much.  

void init() {
	vertexshader = initshaders(GL_VERTEX_SHADER, "shaders/nop.vert.glsl");
	fragmentshader = initshaders(GL_FRAGMENT_SHADER, "shaders/nop.frag.glsl");
	shaderprogram = initprogram(vertexshader, fragmentshader); 
}

int main(int argc, char* argv[]) {
	glutInit( &argc, argv );
	glutInitWindowSize( scene.width, scene.height );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutCreateWindow( "HW4: Curves" );
	init(); 
	glutDisplayFunc( display );
	glutKeyboardFunc( scene.keyboard );
	glutMouseFunc( scene.mouse );
	glutReshapeFunc( scene.reshape );
	glutPassiveMotionFunc( scene.passiveMotion );
	glutMotionFunc( scene.drag );	
	glutMainLoop();
	return 0;
}
