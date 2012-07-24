#include "render.h"

using namespace std;
#define TIMER 33

GLUquadricObj *turntable;

void recomputeFrame(int value);
float mymove;	

void recomputeFrame(int value)
{
	mymove += .01f;
	if(mymove > 360.0f)
		mymove -= 360.0f;
	glutPostRedisplay();

	//	Perform rotating cube animation
	glutTimerFunc(TIMER, recomputeFrame, value);

}

// Default constructor
// used for initializing any user defined variables
Render::Render()
{
	// specify a default location for the camera
	eye[0] = 0.0f;
	eye[1] = 0.0f;
	eye[2] = 40.0f;

	// specify default values to the rotational values in the transformation matrix
	rot[0] = 0.0f;
	rot[1] = 0.0f;
	rot[2] = 0.0f;
	choice=false;
	view = 3;
	turn = 0.0;
	motor_one =0.0;
	moveX = 0.0;
	moveZ = 0.0;
	LEFT = 0;
	MIDDLE = 1;
	RIGHT = 2;
	mButton = -1;
	align = 0;
	factor = 3.14159/180;
	numRows = numCols = 200;
	elevation = numRows/10;
	centroidx = numRows/2.0f;
	centroidz = numCols/2.0f;
	tireRotateAngleIncrement = 0.0;
	turnLeft = false;
	turnRight = false;
	hLightX = 15.0;
	hLightY = 15.0;
	hLightZ = 0.0;
}

Render::~Render()
{


}

void Render::init(void)
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glClearDepth(1.0);

	glEnable (GL_COLOR_MATERIAL);
	glColorMaterial (GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Two different lights are used in this scene
	// first is global ambient light to show the objects
	// LIGHT0: highlights the car
	// LIGHT1: a spotlight functioning as headlight for the car
	
	// specify the ambient lighting in the scene
	GLfloat lmodel_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	glLightModelfv (GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	
	// define the parameters for the ambient light
	GLfloat light0_ambient[] = { 0.1, 0.1, 0.1, 1 };
	GLfloat light0_diffuse[] = { 1.0, 1.0, 1.0, 1 };
	GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);


	// define the parameters for the spotlight
	GLfloat light1_ambient[] = { 1.0, 1.0, 1.0, 1 };
	GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1 };
	GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1 };
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.02);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.05);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);

	
	qobj=gluNewQuadric();
	gluQuadricDrawStyle(qobj,GL_TRUE);
	gluQuadricTexture(qobj,GL_TRUE);
	gluQuadricNormals(qobj,GLU_SMOOTH);
}

void Render::reshape(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	Wx = w;
	Wy = h;
}

void Render::mouseClick(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		mOldX = x;
		mOldY = y;
		switch (button)
		{
			case GLUT_LEFT_BUTTON:
				mButton = LEFT;
				break;
			case GLUT_MIDDLE_BUTTON:
				mButton = MIDDLE;
				break;
			case GLUT_RIGHT_BUTTON:
				mButton = RIGHT;
				break;
		}
	}
	else if (state == GLUT_UP)
	{
		mButton = -1;
	}
}

void Render::mouseMove(int x, int y)
{
	if (mButton == LEFT) 
	{
		rot[0] -= ((mOldY - y) * 180.0f) / 1000.0f;
		rot[1] -= ((mOldX - x) * 180.0f) / 1000.0f;
		clamp(rot[0], rot[1], rot[2]);
	}
	else if (mButton == MIDDLE) 
	{
		eye[2] -= ((mOldY - y) * 180.0f) / 500.0f;
		clamp(rot[0], rot[1], rot[2]);
	}
	else if (mButton == RIGHT)
	{
		eye[0] += ((mOldX - x) * 180.0f) / 1000.0f;
		eye[1] -= ((mOldY - y) * 180.0f) / 1000.0f;
		clamp(rot[0], rot[1], rot[2]);
	}	 
	mOldX = x; 
	mOldY = y;

	// i changed values in the modelview matrix. so
	// update my display and the objects on the screens
	glutPostRedisplay();
}

void Render::keyPos(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'i':
			// specify a default location for the camera
			eye[0] = 0.0f;
			eye[1] = 0.0f;
			eye[2] = 40.0f;

			// specify default values to the rotational values in the transformation matrix
			rot[0] = 0.0f;
			rot[1] = 0.0f;
			rot[2] = 0.0f;

			view = 3;
			turn = 0.0;
			moveX = 0.0;
			moveZ = 0.0;
			align = 0;
			tireRotateAngleIncrement = 0.0;
			turnLeft = false;
			turnRight = false;
			break;

		case 'X':
			hLightX += 1.0;
			break;
		case 'Y':
			hLightY += 1.0;
			break;
		case 'Z':
			hLightZ += 1.0;
			break;

		case 'x':
			hLightX -= 1.0;
			break;
		case 'y':
			hLightY -= 1.0;
			break;
		case 'z':
			hLightZ -= 1.0;
			break;

		case 'q' :
			
			if(motor_one >-30)
				motor_one -= 5.0;
			align = 1;
			//std::cout<<"("<<moveX<<","<<moveY<<","<<moveZ<<")\n";
			break;
		case 'a':
			
			if(motor_one < 30)
				motor_one += 5.0;
			align = -1;
			turnLeft = true;
			//std::cout<<"("<<moveX<<","<<moveY<<","<<moveZ<<")\n";
		
		break;

		case 'w' :
			
			if(motor_two >-30)
				
				motor_two -= 5.0;
				moveX += (30)*cos(motor_two*factor);
				moveY -= (30)*sin(motor_two*factor);
			align = 1;
			std::cout<<"("<<moveX<<","<<moveY<<","<<moveZ<<")\n";
			break;
		case 's':
			
			if(motor_two < 30)
				motor_two += 5.0;
				moveX -= (30)*cos(motor_two*factor);
				moveY += (30)*sin(motor_two*factor);
			align = -1;
			turnLeft = true;
			std::cout<<"("<<moveX<<","<<moveY<<","<<moveZ<<")\n";
		
		break;
		case 'e' :
			
			if(motor_three >-40)
				
				motor_three -= 5.0;
				moveX += (30)*cos(motor_three*factor);
				moveY -= (30)*sin(motor_three*factor);
			align = 1;
			std::cout<<"("<<moveX<<","<<moveY<<","<<moveZ<<")\n";
			break;
		case 'd':
			
			if(motor_three < 30)
				motor_three += 5.0;
				moveX -= (30)*cos(motor_three*factor);
				moveY += (30)*sin(motor_three*factor);
			align = -1;
			turnLeft = true;
			std::cout<<"("<<moveX<<","<<moveY<<","<<moveZ<<")\n";
		
		break;

		case 'r' :
			
			if(motor_four >-15){
				
				motor_four -= 5.0;
				moveX += (30)*cos(motor_four*factor);
				moveY -= (30)*sin(motor_four*factor);
			align = 1;
			}
			else{
				if(motor_five >-20)
				
				motor_five -= 5.0;
				moveX += (30)*cos(motor_four*factor);
				moveY -= (30)*sin(motor_four*factor);
			align = 1;
			std::cout<<"("<<moveX<<","<<moveY<<","<<moveZ<<")\n";
			}
			break;

			case 'f':
			
				if(motor_four < 30){
				motor_four += 5.0;
				moveX -= (30)*cos(motor_four*factor);
				moveY += (30)*sin(motor_four*factor);
			align = -1;
			turnLeft = true;
				}
			else{
				if(motor_five < 20)
				motor_five += 5.0;
				moveX -= (30)*cos(motor_four*factor);
				moveY += (30)*sin(motor_four*factor);
			align = -1;
			turnLeft = true;
			std::cout<<"("<<moveX<<","<<moveY<<","<<moveZ<<")\n";
			}
		break;
		case 27:
			exit (0);

		default:
			break;
	}
//	printf("<hLightX, hLightY, hLightZ> = <%f, %f, %f>\n", hLightX, hLightY, hLightZ);
	glutPostRedisplay();
}


void Render::specialKeyPos(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_UP:
			/*// press the up arrow key to move forward
			
			// first, make the tires rotate forward on their own axle
			tireRotateAngleIncrement += 5.0;
			if(tireRotateAngleIncrement > 360.0f)
				tireRotateAngleIncrement -= 360.0f;

			moveX += (2.5)*cos(turn*factor);
			//moveX =moveX;
			moveZ -= (2.5)*sin(turn*factor);
			//moveZ+=1.0f;
			align = 0;
			std::cout<<"("<<moveX<<","<<moveY<<","<<moveZ<<")\n";*/
			// press the left arrow key to turn left while moving forward
			

		turn -= 5.0;
			moveX = moveX;
			moveY-=(0.5)*sin(turn*factor);
			moveZ=moveZ; 
			if(turn < -360)
				turn += 360;
			align = 1;
			
			break;

		case GLUT_KEY_DOWN:
			// press the down arrow key to move backward
			// first, make the tires rotate forward on their own axle
			/*
			tireRotateAngleIncrement -= 5.0;
			if(tireRotateAngleIncrement < -360.0f)
				tireRotateAngleIncrement += 360.0f;
			moveX -= (2.5)*cos(turn*factor);
			moveZ += (2.5)*sin(turn*factor);
		
			align = 0;
			std::cout<<"("<<moveX<<","<<moveY<<","<<moveZ<<")\n";*/

			// press the right arrow key to turn right while moving forward
			
		turn += 5.0;
			moveX = moveX;
			//moveY-=(0.5)*sin(turn*factor);
			moveZ =moveZ ;
			if(turn > 360)
				turn -= 360;
			align = -1;
			turnLeft = true;
			
		break;


		case GLUT_KEY_LEFT:
			// press the left arrow key to turn left while moving forward
			turn += 5.0;
			moveX = moveX;
			//moveY-=(0.5)*sin(turn*factor);
			moveZ =moveZ ;
			if(turn > 360)
				turn -= 360;
			align = -1;
			turnLeft = true;
			
		break;


		case GLUT_KEY_RIGHT:
			// press the right arrow key to turn right while moving forward
			turn -= 5.0;
			moveX = moveX;
			moveY-=(0.5)*sin(turn*factor);
			moveZ=moveZ; 
			if(turn < -360)
				turn += 360;
			align = 1;
			
		break;
		
		default:
			break;
	}
	glutPostRedisplay();
}

void Render::display(void)
{
	// must clear color and depth buffer for lighting purposes
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Specifies which matrix stack is the target for subsequent matrix operations
	// In this example, the projection matrix is set to perspective projection matrix stack
	glMatrixMode(GL_PROJECTION);
	// all matrix values from previous frames set to identity
	glLoadIdentity();
	
	// perspective projection loaded with new values for Wx and Wy updated
	gluPerspective(60, (GLfloat) Wx/(GLfloat) Wy, 1, 1000000);


	// Applies subsequent matrix operations to the modelview matrix stack.
	glMatrixMode(GL_MODELVIEW);
	// Clears all the previously loaded values in the modelview matrix
	glLoadIdentity();

	glTranslatef(-eye[0], -eye[1], -eye[2]);
	glRotatef(rot[0], 1.0f, 0.0f, 0.0f);
	glRotatef(rot[1], 0.0f, 1.0f, 0.0f);
	glRotatef(rot[2], 0.0f, 0.0f, 1.0f);


	glPushMatrix ();
		glTranslatef(-centroidx, 0, -centroidz);
		drawNavigArea();
	glPopMatrix ();

	glPushMatrix();
		drawObjRobotArm();
	glPopMatrix();

	//recomputeFrame(0);
	glutSwapBuffers ();
	
}


void Render::drawObjRectangle(void)
{
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHT1);
	glPushMatrix();

	
		glTranslatef(moveX,moveY,moveZ);
		glRotatef(turn, 0, 1, 0);
		//glTranslatef(70, 5, 70);
		
		glutSolidCube(10.0);
		glRotatef(mymove,0,1,0);
		glTranslatef(20,0,0);
		glutSolidSphere(2,8,8);
		//glVertex2f(moveX,moveZ);
	glPopMatrix();
	glEnable(GL_LIGHT1);
	glEnable(GL_COLOR_MATERIAL);
}

void Render::drawObjRobotArm(void)
{
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHT1);
	glPushMatrix();

	
		//draw Base
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(90,1,0,0);
		//glutSolidTorus(3,8,15,15);
		glTranslatef(0,0,-5);
		gluCylinder(qobj, 10.0, 10.0, 5.0, 15, 15);
		glRotatef(90,0,1,0);
		glutSolidSphere(10,2,15);
		
		//Draw Cylinder
		glPopMatrix();

		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,30,0);
		glRotatef(90,1,0,0);
		gluCylinder(qobj, 5.0, 5.0, 30.0, 15, 15);
		glPopMatrix();

		glPopMatrix();


		//Draw first joint
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(90,1,0,0);
		glutSolidSphere(5,15,15);
		glPopMatrix();
		//Draw second arm
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,35,0);
		glRotatef(motor_two, 1, 0, 0);

		glRotatef(180,1,0,0);
		glRotatef(70,1,0,0);
		gluCylinder(qobj, 5.0, 5.0, 30.0, 15, 15);
		glPopMatrix();

		
		//draw second joint
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,35,0);
		glRotatef(motor_two, 1, 0, 0);
		glTranslatef(0,32,-11);
		//glRotatef(90,1,0,0);
		//glTranslatef(moveX,0,moveY);
		glutSolidSphere(5,15,15);
		
		glPopMatrix();


		//draw third arm
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two, 1, 0, 0);
		glTranslatef(0,32,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(0,2,-.5);
		glRotatef(180,1,0,0);
		glRotatef(70,1,0,0);
		gluCylinder(qobj, 5.0, 5.0, 30.0, 15, 15);
		glPopMatrix();

		//draw third joint
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two, 1, 0, 0);
		glTranslatef(0,32,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(0,34,-12);
		//glTranslatef(0,99,-25);
		glRotatef(70,1,0,0);
		glutSolidSphere(5,15,15);
		glPopMatrix();

		/////-----------------
		//Thumb Finger
		/////-----------------
		//draw thumb joint
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two,1,0,0);
		glTranslatef(0,33,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(-4.25,37,-16.25);
		glRotatef(90,1,0,0);
		//glTranslatef(moveX,moveY,moveZ);
		glRotatef(10,0,1,0);
		glutSolidSphere(1.5,15,15);
		glPopMatrix();

		//draw thumb-1st digit
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two,1,0,0);
		glTranslatef(0,33,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(-6.75,39,-16.5);
		//glRotatef(180,1,0,0);
		glRotatef(72,1,0,0);
		//glTranslatef(moveX,moveY,moveZ);
		glRotatef(50,0,1,0);
		gluCylinder(qobj, 1.0, 1.0, 3.0, 15, 15);
		glPopMatrix();

		//draw second thumb joint
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two,1,0,0);
		glTranslatef(0,33,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(-6.75,39,-16.5);
		glRotatef(motor_four,0,0,1);
		glRotatef(90,1,0,0);
		//glTranslatef(moveX,moveY,moveZ);
		glRotatef(10,0,1,0);
		glutSolidSphere(1.5,15,15);
		glPopMatrix();

		//draw thumb-2nd digit
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two,1,0,0);
		glTranslatef(0,33,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(-6.75,39,-16.5);
		glRotatef(motor_four,0,0,1);
		glTranslatef(0,3,-1);
		//glRotatef(90,1,0,0);
		//glTranslatef(moveX,moveY,moveZ);
		//glRotatef(turn,1,0,0);
		glRotatef(70,1,0,0);
		
		gluCylinder(qobj, 1.0, 1.0, 4.0, 15, 15);
		
		glPopMatrix();
		

		//draw third thumb joint
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two,1,0,0);
		glTranslatef(0,33,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(-6.75,39,-16.5);
		glRotatef(motor_four,0,0,1);
		glTranslatef(0,3,-1);
		glRotatef(90,1,0,0);
		//glTranslatef(moveX,moveY,moveZ);
		glRotatef(10,0,1,0);
		glutSolidSphere(1.5,15,15);
		glPopMatrix();

		/////-----------------
		/////-----------------


		//draw 2nd finger-thumb joint
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two,1,0,0);
		glTranslatef(0,33,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(-3,37.5,-13.25);
		glRotatef(motor_four,1,0,0);
		glRotatef(90,1,0,0);
		//glTranslatef(moveX,moveY,moveZ);
		glRotatef(10,0,1,0);
		glutSolidSphere(1.5,15,15);
		glPopMatrix();

		

		//draw 2nd finger-1st digit
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two,1,0,0);
		glTranslatef(0,33,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(-3,37.5,-13.25);
		glRotatef(motor_four,1,0,0);
		glTranslatef(-.38,5.5,-1.45);
		//glRotatef(180,1,0,0);
		glRotatef(72,1,0,0);
		//glTranslatef(moveX,moveY,moveZ);
		glRotatef(5,0,1,0);
		gluCylinder(qobj, 1.0, 1.0, 5.0, 15, 15);
		glPopMatrix();

		//draw 2nd finger-2nd joint
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two,1,0,0);
		glTranslatef(0,33,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(-3,37.5,-13.25);
		glRotatef(motor_four,1,0,0);
		glTranslatef(-.38,5.0,-1.45);
		glRotatef(motor_five,1,0,0);
		glRotatef(72,1,0,0);
		//glTranslatef(moveX,moveY,moveZ);
		glRotatef(5,0,1,0);
		glutSolidSphere(1.5,15,15);
		glPopMatrix();
		
		//draw 2nd finger-2nd digit
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two,1,0,0);
		glTranslatef(0,33,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(-3,37.5,-13.25);
		glRotatef(motor_four,1,0,0);
		glTranslatef(-.38,5.0,-1.45);
		glRotatef(motor_five,1,0,0);
		glTranslatef(-.75,4.5,-4);
		//glRotatef(180,1,0,0);
		glRotatef(50,1,0,0);
		//glTranslatef(moveX,moveY,moveZ);
		glRotatef(5,0,1,0);
		gluCylinder(qobj, 1.0, 1.0, 5.0, 15, 15);
		glPopMatrix();

		//draw 2nd finger-3rd joint
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two,1,0,0);
		glTranslatef(0,33,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(-3,37.5,-13.25);
		glRotatef(motor_four,1,0,0);
		glTranslatef(-.38,5.0,-1.45);
		glRotatef(motor_five,1,0,0);
		glTranslatef(-.75,4.5,-4);
		glRotatef(50,1,0,0);
		//glTranslatef(moveX,moveY,moveZ);
		glRotatef(5,0,0,1);
		glutSolidSphere(1.5,15,15);
		glPopMatrix();
		/////-----------------
		/////-----------------
		/////-----------------

		//draw 3rd finger-thumb joint
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two,1,0,0);
		glTranslatef(0,33,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(.75,38,-12);
		glRotatef(motor_four,1,0,0);
		glRotatef(90,1,0,0);
		//glTranslatef(moveX,moveY,moveZ);
		glRotatef(10,0,1,0);
		glutSolidSphere(1.5,15,15);
		glPopMatrix();

		//3rd finger-1st digit
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two,1,0,0);
		glTranslatef(0,33,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(.75,38,-12);
		glRotatef(motor_four,1,0,0);
		glTranslatef(0,6,0);
		//glRotatef(180,1,0,0);
		glRotatef(90,1,0,0);
		//glTranslatef(moveX,moveY,moveZ);
		//glRotatef(10,0,1,0);
		gluCylinder(qobj, 1.0, 1.0, 5.0, 15, 15);
		glPopMatrix();

		//3rd finger-2nd joint
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two,1,0,0);
		glTranslatef(0,33,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(.75,38,-12);
		glRotatef(motor_four,1,0,0);
		glTranslatef(0,6,0);
		glRotatef(90,1,0,0);
		//glTranslatef(moveX,moveY,moveZ);
		//glRotatef(10,0,1,0);
		glutSolidSphere(1.5,15,15);
		glPopMatrix();

		//3rd finger-2nd digit
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two,1,0,0);
		glTranslatef(0,33,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(.75,38,-12);
		glRotatef(motor_four,1,0,0);
		glTranslatef(0,6,0);
		glRotatef(motor_five,1,0,0);
		glTranslatef(0,6,-2);
		//glRotatef(180,1,0,0);
		glRotatef(70,1,0,0);
		//glRotatef(10,1,0,0);
		//glTranslatef(moveX,moveY,moveZ);
		//glRotatef(10,0,1,0);
		gluCylinder(qobj, 1.0, 1.0, 5.0, 15, 15);
		glPopMatrix();

		//3rd finger-3rd joint
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two,1,0,0);
		glTranslatef(0,33,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(.75,38,-12);
		glRotatef(motor_four,1,0,0);
		glTranslatef(0,6,0);
		glRotatef(motor_five,1,0,0);
		glTranslatef(0,5.9,-2);
		glRotatef(70,1,0,0);
		//glRotatef(10,1,0,0);
		//glTranslatef(moveX,moveY,moveZ);
		//glRotatef(10,0,1,0);
		glutSolidSphere(1.5,15,15);
		glPopMatrix();

		/////-----------------
		/////-----------------
		/////-----------------

		//draw 4th finger-thumb joint
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two,1,0,0);
		glTranslatef(0,33,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(3.25,37.5,-14.75);
		glRotatef(motor_four,1,0,0);
		glRotatef(90,1,0,0);
		//glTranslatef(moveX,moveY,moveZ);
		glRotatef(10,0,1,0);
		glutSolidSphere(1.5,15,15);
		glPopMatrix();

		//draw 4th finger-1st digit
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two,1,0,0);
		glTranslatef(0,33,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(3.25,37.5,-14.75);
		glRotatef(motor_four,1,0,0);
		glTranslatef(.25,6,0);
		//glRotatef(180,1,0,0);
		glRotatef(90,1,0,0);
		//glTranslatef(moveX,moveY,moveZ);
		//glRotatef(10,0,1,0);
		gluCylinder(qobj, 1.0, 1.0, 5.0, 15, 15);
		glPopMatrix();

		//draw 4th finger-2nd thumb joint
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two,1,0,0);
		glTranslatef(0,33,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(3.25,37.5,-14.75);
		glRotatef(motor_four,1,0,0);
		glTranslatef(.25,6,0);
		glRotatef(70,1,0,0);
		//glTranslatef(moveX,moveY,moveZ);
		//glRotatef(10,0,1,0);
		glutSolidSphere(1.5,15,15);
		glPopMatrix();

		//draw 4th finger-2nd digit
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two,1,0,0);
		glTranslatef(0,33,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(3.25,37.5,-14.75);
		glRotatef(motor_four,1,0,0);
		glTranslatef(.25,6,0);
		glRotatef(motor_five,1,0,0);
		glTranslatef(0,5,-4);
		//glRotatef(180,1,0,0);
		glRotatef(50,1,0,0);
		//glTranslatef(moveX,moveY,moveZ);
		//glRotatef(10,0,1,0);
		gluCylinder(qobj, 1.0, 1.0, 5.0, 15, 15);
		glPopMatrix();
		
		//draw 4th finger-3rd joint
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two,1,0,0);
		glTranslatef(0,33,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(3.25,37.5,-14.75);
		glRotatef(motor_four,1,0,0);
		glTranslatef(.25,6,0);
		glRotatef(motor_five,1,0,0);
		glTranslatef(0,5,-4);
		glRotatef(70,1,0,0);
		//glTranslatef(moveX,moveY,moveZ);
		//glRotatef(10,0,1,0);
		glutSolidSphere(1.5,15,15);
		glPopMatrix();
		


		/////-----------------
		/////-----------------
		/////-----------------
		//draw 5th finger-thumb joint
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two,1,0,0);
		glTranslatef(0,33,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(2,35,-16);
		glRotatef(-motor_four,0,0,1);
		glRotatef(90,1,0,0);
		//glTranslatef(moveX,moveY,moveZ);
		glRotatef(10,0,1,0);
		glutSolidSphere(1.5,15,15);
		glPopMatrix();

		//draw 5th finger-1st digit
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two,1,0,0);
		glTranslatef(0,33,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(2,35,-16);
		glRotatef(-motor_four,0,0,1);
		glTranslatef(2,2,-1);
		//glRotatef(180,1,0,0);
		glRotatef(72,1,0,0);
		//glTranslatef(moveX,moveY,moveZ);
		glRotatef(-30,0,1,0);
		gluCylinder(qobj, 1.0, 1.0, 3.0, 15, 15);
		glPopMatrix();

		//draw 5th finger-2nd joint
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two,1,0,0);
		glTranslatef(0,33,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(2,35,-16);
		glRotatef(-motor_four,0,0,1);
		glTranslatef(2,2,-1);
		glRotatef(72,1,0,0);
		//glTranslatef(moveX,moveY,moveZ);
		//glRotatef(20,0,1,0);
		glutSolidSphere(1.5,15,15);
		glPopMatrix();

		//draw 5th finger-2nd digit
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two,1,0,0);
		glTranslatef(0,33,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(2,35,-16);
		glRotatef(-motor_four,0,0,1);
		glTranslatef(2,6,-3);
		//glRotatef(180,1,0,0);
		glRotatef(70.0,1,0,0);
		//glTranslatef(moveX,moveY,moveZ);
		//glRotatef(50,0,1,0);
		gluCylinder(qobj, 1.0, 1.0, 3.0, 15, 15);
		glPopMatrix();

		//draw 5th finger-3rd joint
		glPushMatrix();
		glRotatef(turn, 0, 1, 0);
		glRotatef(motor_one, 1, 0, 0);
		glTranslatef(0,33,0);
		glRotatef(motor_two,1,0,0);
		glTranslatef(0,33,-11);
		glRotatef(motor_three,1,0,0);
		glTranslatef(2,35,-16);
		glRotatef(-motor_four,0,0,1);
		glTranslatef(2,6,-3);
		glRotatef(30,0,1,0);
		//glTranslatef(moveX,moveY,moveZ);
		//glRotatef(-20,0,1,0);
		glutSolidSphere(1.5,15,15);
		glPopMatrix();
		/////-----------------
		/////-----------------
		/////-----------------
		//glRotatef(mymove,0,1,0);
		//glTranslatef(20,0,0);
		//glutSolidSphere(2,8,8);
		//glVertex2f(moveX,moveZ);
	glPopMatrix();
	glEnable(GL_LIGHT1);
	glEnable(GL_COLOR_MATERIAL);
}

void Render::drawNavigArea(void)
{
	int i, j;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f (0.4, 0.4, 0.2);

	glBegin(GL_QUADS);
	// Draw floor
	for(j=0; j<numRows; j++)
	{
		for(i=0; i<numCols; i++)
		{
			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f(i, 0, j);

			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f(i, 0, (j+1));

			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f((i+1), 0, (j+1));

			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f((i+1), 0, j);
		}
	}
	glEnd();

	// Left wall
	glColor3f (0.2, 0.2, 0.2);
	glBegin(GL_QUADS);
	for(j=0; j<numRows; j++)
	{
		for(i=0; i<elevation; i++)
		{
			glNormal3f(1, 0, 0);
			glVertex3f(0, i, j);

			glNormal3f(1, 0, 0);
			glVertex3f(0, i+1, j);

			glNormal3f(1, 0, 0);
			glVertex3f(0, i+1, j+1);

			glNormal3f(1, 0, 0);
			glVertex3f(0, i, j+1);
		}
	}
	glEnd();


	glBegin(GL_QUADS);
		glVertex3f(0, 0, 0);

		glVertex3f(200, 0, 0);

		glVertex3f(200, 20, 0);

		glVertex3f(0, 20, 0);
	glEnd();

	// Right wall
	glBegin(GL_QUADS);
		glVertex3f(200, 0, 0);

		glVertex3f(200, 0, 200);

		glVertex3f(200, 20, 200);

		glVertex3f(200, 20, 0);
	glEnd();

}

// this is for clamping the numbers between 0 & 360. used for rotation values in the mouse move function
void Render::clamp(float ang)
{
	if(ang > 360.0f)
		ang -= 360.0f;

}
// this is an overloaded function for clamp - clamp if there are three numbers
void Render::clamp(float v0, float v1, float v2)
{
	if (v0 > 360 || v0 < -360)
		v0 = 0;
	if (v1 > 360 || v1 < -360)
		v1 = 0;
	if (v2 > 360 || v2 < -360)
		v2 = 0;
}

