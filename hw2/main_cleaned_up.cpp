/*******************************************************
 * Homework 2: OpenGL                                  *
 *-----------------------------------------------------*
 * First, you should fill in problem1(), problem2(),   *
 * and problem3() as instructed in the written part of *
 * the problem set.  Then, express your creativity     *
 * with problem4()!                                    *
 *                                                     *
 * Note: you will only need to add/modify code where   *
 * it says "TODO".                                     *
 *                                                     *
 * The left mouse button rotates, the right mouse      *
 * button zooms, and the keyboard controls which       *
 * problem to display.                                 *
 *                                                     *
 * For Linux/OS X:                                     *
 * To compile your program, just type "make" at the    *
 * command line.  Typing "make clean" will remove all  *
 * computer-generated files.  Run by typing "./hw2"    *
 *                                                     *
 * For Visual Studio:                                  *
 * You can create a project with this main.cpp and     *
 * build and run the executable as you normally would. *
 *******************************************************/

#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "./freeglut-3.2.1/include/GL/freeglut.h"

using namespace std;

bool leftDown = false, rightDown = false;
int lastPos[2];
float cameraPos[4] = {0,1,4,1};
int windowWidth = 640, windowHeight = 480;
double yRot = 0;
int curProblem = 1; // TODO: change this number to try different examples

float specular[] = { 1.0, 1.0, 1.0, 1.0 };
float shininess[] = { 50.0 };

void problem1() {
    // TODO: Your code here!

	const float TEAPOT_SIZE = 0.15;

	// top and bottom
	glPushMatrix(); glTranslatef(0,1.25,0); glRotatef(75,0,0,1); glutSolidTeapot(TEAPOT_SIZE); glPopMatrix();
	glPushMatrix(); glTranslatef(0,-1.25,0); glRotatef(-105,0,0,1); glutSolidTeapot(TEAPOT_SIZE); glPopMatrix();
	
	// right corners
	glPushMatrix(); glTranslatef(0.8,1,0); glRotatef(45,0,0,1); glutSolidTeapot(TEAPOT_SIZE); glPopMatrix();
	glPushMatrix(); glTranslatef(0.8,-1,0); glRotatef(-45,0,0,1); glutSolidTeapot(TEAPOT_SIZE); glPopMatrix();

	// left corners
	glPushMatrix(); glTranslatef(-0.8,1,0); glRotatef(95,0,0,1); glutSolidTeapot(TEAPOT_SIZE); glPopMatrix();
	glPushMatrix(); glTranslatef(-0.8,-1,0); glRotatef(215,0,0,1); glutSolidTeapot(TEAPOT_SIZE); glPopMatrix();

	// right
	glPushMatrix(); glTranslatef(1.25,0.35,0); glRotatef(15,0,0,1); glutSolidTeapot(TEAPOT_SIZE); glPopMatrix();
	glPushMatrix(); glTranslatef(1.25,-0.35,0); glRotatef(-15,0,0,1); glutSolidTeapot(TEAPOT_SIZE); glPopMatrix();
		
	// left	
	glPushMatrix(); glTranslatef(-1.25,0.35,0); glRotatef(150,0,0,1); glutSolidTeapot(TEAPOT_SIZE); glPopMatrix();
	glPushMatrix(); glTranslatef(-1.25,-0.35,0); glRotatef(185,0,0,1); glutSolidTeapot(TEAPOT_SIZE); glPopMatrix();
	
}

void problem2() {
    // TODO: Your code here!
	const float CUBE_SIZE = 0.05;
	float y_start = 1;
	float y_stop = -0.15;
	float z_start = 0;
	float z_thickness = 0.2;
	const float x_step = 5*CUBE_SIZE; // must have an integer scalar
	
	for (float x=-2; x<2; x+=x_step) {
		for (float y=y_start; y>y_stop; y-=CUBE_SIZE) {
			for (float z=z_start; z<z_thickness-z_start; z+=CUBE_SIZE) {
				for (int i=0; i<x_step/CUBE_SIZE; i++)
				{
					glPushMatrix(); 
					glTranslatef(x+i*CUBE_SIZE, y, z); 
					glutSolidCube(CUBE_SIZE); 
					glPopMatrix();

				}
			}
		}
		y_start-=CUBE_SIZE;
	}
	
}

void problem3() {
    // TODO: Your code here!

	const float TEAPOT_SIZE = 0.2;
	for (float i=-3; i<=2; i++) {
		for (float j=-(i+4)/2; j<(i+4)/2; j++) {
			glPushMatrix(); 
			
			glTranslatef(j/2+0.25 + 0.1*j,i/2 + i*0.1,0); 
			glutSolidTeapot(TEAPOT_SIZE); 

			glPopMatrix();

		}
	}
}

struct Color {
	float r, g, b;

	Color(float r, float g, float b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}
};

void setColor(Color color) {
	glColor3f(color.r, color.g, color.b);
}

bool toggle(bool boolean) {
	if (boolean == true) {
		return false;
	}
	else {
		return true;
	}
}

void draw_poker_chip(float diameter, Color chipColor) { // cube size is equivalent to strip size
	
	const float RADIUS = diameter/2;
	const float HEIGHT = diameter/10;
	const float CUBE_SIZE = 0.01;
	
	const Color GREY(0.36471f, 0.31373f, 0.29020f);
	const Color LIGHT_GREY(0.63529f, 0.58431f, 0.56078f);
	
	Color cur_outline_color = GREY;
	bool is_grey = true;

	glPushMatrix();
		// determine x position (relative to center)
		for (float translateX=-(RADIUS*cos(1.309)); translateX<RADIUS*cos(1.309); translateX+=CUBE_SIZE) {
			// determine z position (relative to center)
				float angle = acos(translateX/RADIUS);
				float translateZ = RADIUS * sin(angle);
			
			// determine which color comes next in coin strips (on the sides)
			if (is_grey) {cur_outline_color = GREY;}
			if (!is_grey) {cur_outline_color = LIGHT_GREY;}

			// some tools used to draw coin
			bool is_back_face = true;
			bool is_front_face = false;

			// determine y position (relative to center)
			for (float translateY=(-HEIGHT+CUBE_SIZE)/2; translateY<(HEIGHT-CUBE_SIZE)/2; translateY+=CUBE_SIZE) {
				
				if (translateY + CUBE_SIZE >= (HEIGHT-CUBE_SIZE)/2) {
					is_front_face = true;
				}

				glPushMatrix();
					for (int i=0; i<6; i++) {
					 // draw outline of coin
					 	setColor(cur_outline_color);	

						// ridges
						glPushMatrix();
							glTranslatef(translateX, translateY, translateZ);
							glutSolidCube(CUBE_SIZE);
						glPopMatrix();

						glPushMatrix();
							glTranslatef(translateX, translateY, -translateZ);
							glutSolidCube(CUBE_SIZE);
						glPopMatrix();

					 	 // draw top/bottom surface of coin
						 	bool is_black_rim = true;
							float zStop = sqrt(3) * translateX;
							for (float zCur = translateZ-CUBE_SIZE; zCur > zStop; zCur -= CUBE_SIZE) {
								if (is_front_face) {
									if (is_black_rim) {
										glColor3f(0,0,0);
										is_black_rim = false;
									}
									else {
										setColor(chipColor);

									}

									glPushMatrix();
										glTranslatef(translateX+CUBE_SIZE, translateY-CUBE_SIZE, -zCur);
										glutSolidCube(CUBE_SIZE);
									glPopMatrix();

									glPushMatrix();
										glTranslatef(translateX+CUBE_SIZE, translateY-CUBE_SIZE, zCur);
										glutSolidCube(CUBE_SIZE);
									glPopMatrix();										
								}
								else if (is_back_face) {
									if (is_black_rim) {
										glColor3f(0,0,0);
										is_black_rim = false;
									}
									else {
										setColor(chipColor);

									}

									glPushMatrix();
										glTranslatef(translateX+CUBE_SIZE, translateY+CUBE_SIZE, -zCur);
										glutSolidCube(CUBE_SIZE);
									glPopMatrix();

									glPushMatrix();
										glTranslatef(translateX+CUBE_SIZE, translateY+CUBE_SIZE, zCur);
										glutSolidCube(CUBE_SIZE);
									glPopMatrix();	
								}
							}
						glRotatef(30, 0, 1, 0);
					}
				glPopMatrix();
				if (is_back_face) {
					is_back_face = false;
				}
				if (is_front_face) {
					is_front_face = false;
				}
			}

			is_grey = toggle(is_grey);
		}
		
	
	glPopMatrix();
	return;
}

void problem4() {
    // TODO: Your code here!
	

	glEnable(GL_COLOR_MATERIAL);

	glPushMatrix();		
		const Color RED_CHIP(1.00000f, 0.14118f, 0.25882f);
		const Color BLUE_CHIP(0.02745f, 0.39608f, 0.88235f);
		const Color GREEN_CHIP(0.03529f, 0.67451f, 0.24706f);
		const Color BLACK_CHIP(0.18824f, 0.18039f, 0.18431f);

		float chipDiameter = 0.5;
		float chipHeight = chipDiameter/10;
		
		// Red Stack
		glPushMatrix();
			glTranslatef(-1,0,0);
			draw_poker_chip(chipDiameter, RED_CHIP);
			for (int i=0; i<5; i++) {
				glTranslatef(0,chipHeight,0);
				draw_poker_chip(chipDiameter, RED_CHIP);
			}
		glPopMatrix();

		// Red Solo
		glPushMatrix();
			glTranslatef(-0.68,0.15,-0.04);
			glRotatef(-65, 0,0,1);
			draw_poker_chip(chipDiameter, RED_CHIP);
		glPopMatrix();


		// Black Stack 1
		glPushMatrix();
			glTranslatef(-0.5,0,-0.5);
			draw_poker_chip(chipDiameter,BLACK_CHIP);
			for (int i=0; i<9; i++) {
				glTranslatef(0,chipHeight,0);
				draw_poker_chip(chipDiameter,BLACK_CHIP);
			}
		glPopMatrix();

		// Black Stack 2
		glPushMatrix();
			glTranslatef(-0.2,0,-0.08);
			draw_poker_chip(chipDiameter,BLACK_CHIP);
			for(int i=0; i<4; i++) {
				glTranslatef(0,chipHeight,0);
				draw_poker_chip(chipDiameter,BLACK_CHIP);
			}
		glPopMatrix();

		// Black Solo
		glPushMatrix();
			glTranslatef(-0.35,0.4,-0.15);
			glRotatef(-45,-1,0,1);
			draw_poker_chip(chipDiameter,BLACK_CHIP);
		glPopMatrix();

		// Blue Stack
		glPushMatrix();
			glTranslatef(0.15,0,0.3);
			draw_poker_chip(chipDiameter, BLUE_CHIP);
			for (int i=0; i<3; i++) {
				glTranslatef(0.01,chipHeight,0);
				draw_poker_chip(chipDiameter, BLUE_CHIP);
			}
			for (int i=0;i<4;i++) {
				glTranslatef(-0.005,chipHeight,0);
				draw_poker_chip(chipDiameter,BLUE_CHIP);
			}
		glPopMatrix();

		// Green Stack
		glPushMatrix();
			glTranslatef(-0.4, 0, 0.4);
			draw_poker_chip(chipDiameter, GREEN_CHIP);
			for (int i=0; i<3; i++) {
				glTranslatef(0.01,chipHeight,-0.01);
				draw_poker_chip(chipDiameter, GREEN_CHIP);
			}
		glPopMatrix();

	glPopMatrix();

}

void display() {
	glClearColor(0,0,0,0);

	if (curProblem == 4) {
		glClearColor(0.14510f, 0.16863f, 0.11373f, 0);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glBegin(GL_LINES);
		glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(1,0,0); // x axis
		glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,1,0); // y axis
		glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,1); // z axis
	glEnd(/*GL_LINES*/);

	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glEnable(GL_LIGHT0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0,0,windowWidth,windowHeight);

	float ratio = (float)windowWidth / (float)windowHeight;
	gluPerspective(50, ratio, 1, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2], 0, 0, 0, 0, 1, 0);

	glLightfv(GL_LIGHT0, GL_POSITION, cameraPos);

	glRotatef(yRot,0,1,0);

	if (curProblem == 1) problem1();
	if (curProblem == 2) problem2();
	if (curProblem == 3) problem3();
	if (curProblem == 4) problem4();

	glutSwapBuffers();
}

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) leftDown = (state == GLUT_DOWN);
	else if (button == GLUT_RIGHT_BUTTON) rightDown = (state == GLUT_DOWN);

	lastPos[0] = x;
	lastPos[1] = y;
}

void mouseMoved(int x, int y) {
	if (leftDown) yRot += (x - lastPos[0])*.1;
	if (rightDown) {
		for (int i = 0; i < 3; i++)
			cameraPos[i] *= pow(1.1,(y-lastPos[1])*.1);
	}


	lastPos[0] = x;
	lastPos[1] = y;
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
	curProblem = key-'0';
    if (key == 'q' || key == 'Q' || key == 27){
        exit(0);
    }
	glutPostRedisplay();
}

void reshape(int width, int height) {
	windowWidth = width;
	windowHeight = height;
	glutPostRedisplay();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("HW2");

	glutDisplayFunc(display);
	glutMotionFunc(mouseMoved);
	glutMouseFunc(mouse);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}
