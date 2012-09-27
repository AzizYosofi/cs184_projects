/*****************************************************************************/
/* This is the program skeleton for homework 2 in CS 184 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/


#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <GL/glut.h>
#include "shaders.h"
#include "Transform.h"
#include <FreeImage.h>
#include "UCB/grader.h"

using namespace std ;

// Main variables in the program.
#define MAINPROGRAM
#include "variables.h"
#include "readfile.h" // prototypes for readfile.cpp
void display(void) ;  // prototype for display function.
void transformLight(float degrees, int direction);
void startObjTransform(void);
void stopObjTransform(void);
void scaleObj(int direction, int type);
void translateObj(int direction);
void rotateObj(int direction);

Grader grader;
bool allowGrader = false;

// Uses the Projection matrices (technically deprecated) to set perspective
// We could also do this in a more modern fashion with glm.
void reshape(int width, int height){
	w = width;
	h = height;
    mat4 mv ; // just like for lookat
    
	glMatrixMode(GL_PROJECTION);
    float aspect = w / (float) h, zNear = 0.1, zFar = 99.0 ;
    // I am changing the projection stuff to be consistent with lookat
    if (useGlu) mv = glm::perspective(fovy,aspect,zNear,zFar) ;
    else {
        mv = Transform::perspective(fovy,aspect,zNear,zFar) ;
        mv = glm::transpose(mv) ; // accounting for row major
    }
    glLoadMatrixf(&mv[0][0]) ;
    
	glViewport(0, 0, w, h);
}

void saveScreenshot(string fname) {
	int pix = w * h;
	BYTE pixels[3*pix];
	glReadBuffer(GL_FRONT);
	glReadPixels(0,0,w,h,GL_BGR,GL_UNSIGNED_BYTE, pixels);
    
	FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
	
	std::cout << "Saving screenshot: " << fname << "\n";
    
	FreeImage_Save(FIF_PNG, img, fname.c_str(), 0);
    std::cout << "done saving\n";
}


void printHelp() {
    std::cout << "\npress 'h' to print this message again.\n"
    << "press '+' or '-' to change the amount of rotation that\noccurs with each arrow press.\n"
    << "press 'i' to run image grader test cases\n"
    << "press 'g' to switch between using glm::lookAt and glm::Perspective or your own LookAt.\n"
    << "press 'r' to reset the transformations.\n"
    << "press 'v' 't' 's' to do view [default], translate, scale.\n"
    << "press [0-9] to choose a light and use arrow keys to rotate\n"
    << "press 'o' to begin single object transformations\n"
    << "press 'w' to stop single object transformations\n"
    << "press ESC to quit.\n" ;
}


void keyboard(unsigned char key, int x, int y) {
	switch(key) {
        case 'w':
            stopObjTransform();
            break;
        case 'o':
            stopObjTransform();
            startObjTransform();
            break;
        case '+':
            amount++;
            std::cout << "amount set to " << amount << "\n" ;
            break;
        case '-':
            amount--;
            std::cout << "amount set to " << amount << "\n" ;
            break;
        case 'i':
            if(allowGrader) {
                std::cout << "Running tests...\n";
                grader.runTests();
                std::cout << "Done! [ESC to quit]\n";
            } else {
                std::cout << "Error: no input file specified for grader\n";
            }
            break;
        case 'g':
            useGlu = !useGlu;
            reshape(w,h) ;
            std::cout << "Using glm::LookAt and glm::Perspective set to: " << (useGlu ? " true " : " false ") << "\n" ;
            break;
        case 'h':
            printHelp();
            break;
        case 'x':
            std::cout << "printing image...\n";
            fileName << "extra_credit.obj.screenshot_" << screenshotCount << ".png";
            saveScreenshot(fileName.str());
            fileName.str("");
            screenshotCount++;
            break;
        case 27:  // Escape to quit
            exit(0) ;
            break ;
        case 'r': // reset eye and up vectors, scale and translate.
            for(int i = 0; i < 4*numLights; i++) lightposn[i] = lightposnReset[i];
            for(int k = 0; k < numobjects; k++){
                object * obj = &(objects[k]);
                obj -> transform = obj -> transformOriginal;
            }
            eye = eyeinit ;
            up = upinit ;
            sx = sy = 1.0 ;
            tx = ty = 0.0 ;
            break ;
        case 'v':
            transop = view ;
            std::cout << "Operation is set to View\n" ;
            break ;
        case 't':
            transop = translate ;
            std::cout << "Operation is set to Translate\n" ;
            break ;
        case 's':
            transop = scale ;
            std::cout << "Operation is set to Scale\n" ;
            break ;
        case '0':
            transop = lightop;
            activeLight = 0;
            std::cout << "Operation is set to light 0 movement\n";
            break;
        case '1':
            transop = lightop;
            activeLight = 1;
            std::cout << "Operation is set to light 1 movement\n";
            break;
        case '2':
            transop = lightop;
            activeLight = 2;
            std::cout << "Operation is set to light 2 movement\n";
            break;
        case '3':
            transop = lightop;
            activeLight = 3;
            std::cout << "Operation is set to light 3 movement\n";
            break;
        case '4':
            transop = lightop;
            activeLight = 4;
            std::cout << "Operation is set to light 4 movement\n";
            break;
        case '5':
            transop = lightop;
            activeLight = 5;
            std::cout << "Operation is set to light 5 movement\n";
            break;
        case '6':
            transop = lightop;
            activeLight = 6;
            std::cout << "Operation is set to light 6 movement\n";
            break;
        case '7':
            transop = lightop;
            activeLight = 7;
            std::cout << "Operation is set to light 7 movement\n";
            break;
        case '8':
            transop = lightop;
            activeLight = 8;
            std::cout << "Operation is set to light 8 movement\n";
            break;
        case '9':
            transop = lightop;
            activeLight = 9;
            std::cout << "Operation is set to light 9 movement\n";
            break;
    }
	glutPostRedisplay();
}

//  You will need to enter code for the arrow keys
//  When an arrow key is pressed, it will call your transform functions

void specialKey(int key, int x, int y) {
	switch(key) {
        case 100: //left
            if(transformingObj){
                if (transop == view) rotateObj(tleft);
                else if (transop == scale) scaleObj(xDir, scaleDown);
                else if (transop == translate) translateObj(tleft);
                else if (transop == lightop) transformLight(-amount, leftDir);
            }else{
                if (transop == view) Transform::left(amount, eye,  up);
                else if (transop == scale) sx -= amount * 0.01 ;
                else if (transop == translate) tx -= amount * 0.01 ;
                else if (transop == lightop) transformLight(-amount, leftDir);
            }
            break;
        case 101: //up
            if(transformingObj){
                if (transop == view) rotateObj(tup);
                else if (transop == scale) scaleObj(yDir, scaleUp);
                else if (transop == translate) translateObj(tup);
                else if (transop == lightop) transformLight(-amount, leftDir);
            }else{
                if (transop == view) Transform::up(amount,  eye,  up);
                else if (transop == scale) sy += amount * 0.01 ;
                else if (transop == translate) ty += amount * 0.01 ;
                else if (transop == lightop) transformLight(-amount, upDir);
            }
            break;
        case 102: //right
            if(transformingObj){
                if (transop == view) rotateObj(tright);
                else if (transop == scale) scaleObj(xDir, scaleUp) ;
                else if (transop == translate) translateObj(tright);
                else if (transop == lightop) transformLight(-amount, leftDir);
            }else{
                if (transop == view) Transform::left(-amount, eye,  up);
                else if (transop == scale) sx += amount * 0.01 ;
                else if (transop == translate) tx += amount * 0.01 ;
                else if (transop == lightop) transformLight(amount, leftDir);
            }
            break;
        case 103: //down
            if(transformingObj){
                if (transop == view) rotateObj(tdown);
                else if (transop == scale) scaleObj(yDir, scaleDown);
                else if (transop == translate) translateObj(tdown);
                else if (transop == lightop) transformLight(-amount, leftDir);
            }else{
                if (transop == view) Transform::up(-amount,  eye,  up);
                else if (transop == scale) sy -= amount * 0.01 ;
                else if (transop == translate) ty -= amount * 0.01 ;
                else if (transop == lightop) transformLight(amount, upDir);
            }
            break;
	}
	glutPostRedisplay();
}

/* Rotates light set in variables */
void transformLight(float degrees, int lightTDir){
    vec4 lightVec = glm::vec4(lightposn[activeLight*4 + 0], lightposn[activeLight*4 + 1], lightposn[activeLight*4 + 2], lightposn[activeLight*4 + 3]);
    mat4 rotMat = mat4(1.0);
    
    if(lightTDir == upDir){
        vec3 orth = glm::normalize(glm::cross(vec3(0,1,0), vec3(0,0,1)));
        rotMat = glm::mat4(Transform::rotate(degrees, orth));
    }else if(lightTDir == leftDir){
        rotMat = glm::mat4(Transform::rotate(degrees, vec3(0,1,0)));
    }
    
    lightVec = lightVec * rotMat;
    lightposn[activeLight*4 + 0] = lightVec[0];
    lightposn[activeLight*4 + 1] = lightVec[1];
    lightposn[activeLight*4 + 2] = lightVec[2];
    lightposn[activeLight*4 + 3] = lightVec[3];
}

/* Stops the current object transformation process and sets object values to original */
void stopObjTransform(){
    if(objIndex > -1 && transformingObj){
        object * obj = &(objects[objIndex]);
        
        obj -> diffuse[0] = diffuseBackup[0];
        obj -> diffuse[1] = diffuseBackup[1];
        obj -> diffuse[2] = diffuseBackup[2];
        obj -> diffuse[3] = diffuseBackup[3];
    
        obj -> shininess = shininessBackup;
    }
    
    transformingObj = false;
}

/* Sets necessary values to start single object transformations */
void startObjTransform(){
    objIndex = (objIndex + 1) % numobjects;
    transformingObj = true;
    
    if(objIndex > -1){
        object * obj = &(objects[objIndex]);
    
        diffuseBackup[0] = obj -> diffuse[0];
        diffuseBackup[1] = obj -> diffuse[1];
        diffuseBackup[2] = obj -> diffuse[2];
        diffuseBackup[3] = obj -> diffuse[3];
    
        obj -> diffuse[0] = 0.99;
        obj -> diffuse[1] = 0.99;
        obj -> diffuse[2] = 0.99;
        obj -> diffuse[3] = 0.99;
    
        shininessBackup = obj -> shininess;
        obj -> shininess = 0.5;
    }
}

/* Scales object specified by objIndex */
void scaleObj(int direction, int type){
    object * obj = &(objects[objIndex]);
    
    mat4 scaleMat = mat4(1.0);
    float scaleAmount = 0.01*amount;
    
    if(type == scaleDown){
        scaleAmount = -scaleAmount;
    }
    
    float scaleFactor = 1 + scaleAmount;
    
    if(direction == xDir){
        scaleMat = Transform::scale(1,scaleFactor,1);
    }else if(direction == yDir){
        scaleMat = Transform::scale(scaleFactor,1,1);
    }
    
    obj -> transform = (obj -> transform * scaleMat);
}

/* Translates object specified by objIndex */
void translateObj(int direction){
    object * obj = &(objects[objIndex]);
    
    mat4 tMat = mat4(1.0);
    
    if(direction == tup){
        tMat = Transform::translate(0,amount*0.01,0);
    }else if(direction == tdown){
        tMat = Transform::translate(0,-amount*0.01,0);
    }else if(direction == tleft){
        tMat = Transform::translate(-amount*0.01,0,0);
    }else if(direction == tright){
        tMat = Transform::translate(amount*0.01,0,0);
    }
    
    tMat = glm::transpose(tMat);
    
    obj -> transform = obj -> transform * tMat;
}

/* Rotates obj specified by objIndex */
void rotateObj(int direction){
    object * obj = &(objects[objIndex]);
    
    mat4 rotMat = mat4(1.0);
    
    if(direction == tup){
        vec3 orth = glm::normalize(glm::cross(up, eye));
        rotMat = mat4(Transform::rotate(amount, orth));
    }else if(direction == tdown){
        vec3 orth = glm::normalize(glm::cross(up, eye));
        rotMat = mat4(Transform::rotate(-amount, orth));
    }else if(direction == tleft){
        rotMat = mat4(Transform::rotate(-amount, up));
    }else if(direction == tright){
        rotMat = mat4(Transform::rotate(amount, up));
    }
    
    rotMat = glm::transpose(rotMat);
    obj -> transform = rotMat * obj -> transform;
    
}

void init() {
    // Initialize shaders
    vertexshader = initshaders(GL_VERTEX_SHADER, "shaders/light.vert.glsl") ;
    fragmentshader = initshaders(GL_FRAGMENT_SHADER, "shaders/light.frag.glsl") ;
    shaderprogram = initprogram(vertexshader, fragmentshader) ;
    enablelighting = glGetUniformLocation(shaderprogram,"enablelighting") ;
    lightpos = glGetUniformLocation(shaderprogram,"lightposn") ;
    lightcol = glGetUniformLocation(shaderprogram,"lightcolor") ;
    numusedcol = glGetUniformLocation(shaderprogram,"numused") ;
    ambientcol = glGetUniformLocation(shaderprogram,"ambient") ;
    diffusecol = glGetUniformLocation(shaderprogram,"diffuse") ;
    specularcol = glGetUniformLocation(shaderprogram,"specular") ;
    emissioncol = glGetUniformLocation(shaderprogram,"emission") ;
    shininesscol = glGetUniformLocation(shaderprogram,"shininess") ;
    
    //Init operations for object movement
    transformingObj = false;
    shininessBackup = 0;
    objIndex = -1;
    diffuseBackup[0] = 0.99;
    diffuseBackup[1] = 0.99;
    diffuseBackup[2] = 0.99;
    diffuseBackup[3] = 1;
    
    scaleDown = 0;
    scaleUp = 1;
    xDir = 0;
    yDir = 1;
    zDir = 2;
    
    tup = 0;
    tleft = 1;
    tright = 2;
    tdown = 3;
    
    screenshotCount = 0;
}

int main(int argc, char* argv[]) {
    
	if (argc < 2) {
		cerr << "Usage: transforms scenefile [grader input (optional)]\n";
		exit(-1);
	}
    
  	FreeImage_Initialise();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("HW2: Scene Viewer");
	init();
    readfile(argv[1]) ;
	glutDisplayFunc(display);
	glutSpecialFunc(specialKey);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutReshapeWindow(w, h);
    
	if (argc > 2) {
		allowGrader = true;
		stringstream tcid;
		tcid << argv[1] << "." << argv[2];
		grader.init(tcid.str());
		grader.loadCommands(argv[2]);
		grader.bindDisplayFunc(display);
		grader.bindSpecialFunc(specialKey);
		grader.bindKeyboardFunc(keyboard);
		grader.bindScreenshotFunc(saveScreenshot);
	}
    
	printHelp();
	glutMainLoop();
	FreeImage_DeInitialise();
	return 0;
}
