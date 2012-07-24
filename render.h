#ifndef _RENDER_H
#define _RENDER_H

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "GLUT/glut.h"

class Render
{
public:
	
	//constructor definition
	Render();
	//destructor definition
	~Render();

	// These functions are defined public so they can be called by the main function in main.cpp
	void display(void);
	void init(void);
	void reshape(int w, int h);
	void mouseClick(int button, int state, int x, int y);
	void mouseMove(int x, int y);
	void keyPos(unsigned char key, int x, int y);
	void specialKeyPos(int key, int x, int y);


private:
	// These functions are private and cannot be called anywhere outside of render class
	void draw(void);
	void drawNavigArea(void);
	void drawObjRectangle(void);
	void drawObjRobotArm(void);
	void clamp(float v0, float v1, float v2);
	void clamp(float ang);

	// Variables used in the member functions of render class
	float eye[3];
	float rot[3];
	int Wx, Wy;
	int LEFT, MIDDLE, RIGHT;
	int mButton;
	int mOldX, mOldY;
	int view;
	bool choice;
	float turn;
	float moveX,moveY, moveZ;
	int align;
	float factor;

	//motors
	int motor_one;
	int motor_two;
	int motor_three;
	int motor_four;
	int motor_five;


	// driving area variables
	int numRows;	// # rows (height) of quads for the floor
	int numCols;	// # columns (width) of quads for the floor
	int elevation;
	float centroidx, centroidz;

	float tireRotateAngleIncrement;
	float turnLeft, turnRight;
	float hLightX, hLightY, hLightZ;

	GLUquadricObj *qobj;


};

#endif
