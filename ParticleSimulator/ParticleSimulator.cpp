// ParticleSimulator.cpp : 定義主控台應用程式的進入點。
//


#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <ctime>

#include "Object.h"
#include "Particle.h"
#include "Source.h"
#include "Field.h"
#include "VectorCal.h"

#include "gl/glee.h"
#include "gl/shared/gltools.h"	// OpenGL toolkit
#include "gl/shared/glframe.h"   // OpenGL Frame class

#pragma comment(lib, "GLee.lib" )
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glaux.lib")
#pragma comment(lib,"glut32.lib")

#define  TYPE_ROT 1
#define  TYPE_TRANS 2
#define	PI 3.14159265358979323846f

int transType = 0;
float v0[3], v1[3];
static GLint iRenderMode = 1;
GLfloat scale = 1.0;
float mo[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
float m1[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
float project[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

const float filedZero[3] = { 0, 0, 0 };
float veloVec[3] = { 1, 0, 0 };
float sourcePosi[3] = { -100, 0, 0 };
float fieldPos1[3] = { 0, 0, 0 };
float fieldPos2[3] = { 100, 0, 0 };
float fieldPos3[3] = { 100, 0, 0 };
float fieldPos4[3] = { 100, 0, 0 };

float flatfield[3] = { 0, -0.0075f, 0 };

int timer = 0;
int fieldTimer = 0;
int parNum = 25000;
bool field1b = true;
bool field2b = false;
bool field3b = false;
bool field4b = false;

bool KEYF3 = false;
bool VeloColor = true;
Field spField[5];
Source s1(parNum, 0.5, veloVec, sourcePosi);

void hsvToRgb(float h, float* rgb){
	float r, g, b;
	float s = 1.0f;
	float v = 1.0f;

	h *= -0.67*2;
	h += 0.67;

	if (h < 0){ h = 0; }
	if (h > 0.67){ h = 0.67; }

	int i = int(h * 6);
	double f = h * 6 - i;
	double p = v * (1 - s);
	double q = v * (1 - f * s);
	double t = v * (1 - (1 - f) * s);

	switch (i % 6){
	case 0: r = v, g = t, b = p; break;
	case 1: r = q, g = v, b = p; break;
	case 2: r = p, g = v, b = t; break;
	case 3: r = p, g = q, b = v; break;
	case 4: r = t, g = p, b = v; break;
	case 5: r = v, g = p, b = q; break;
	}

	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}

void ChangeSize(int w, int h)
{
	GLfloat nRange = 1000;

	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);

	// Reset projection matrix stack
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Establish clipping volume (left, right, bottom, top, near, far)
	if (w <= h)
		glOrtho(-nRange*scale, nRange*scale, -nRange*h / w*scale, nRange*h / w*scale, -nRange, nRange);
	else
		glOrtho(-nRange*w / h*scale, nRange*w / h*scale, -nRange*scale, nRange*scale, -nRange, nRange);

	glGetFloatv(GL_PROJECTION_MATRIX, project);
	// Reset Model view matrix stack
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mousebutton(int button, int state, int x, int y)
{
	//middle button down, model rotate
	if (button == GLUT_RIGHT_BUTTON)
	{
		transType = TYPE_ROT;
		vassign(v0, 3.5*x / glutGet(GLUT_WINDOW_HEIGHT) - 1, -3.5*y / glutGet(GLUT_WINDOW_WIDTH) + 1, 1);
		normalize(v0);
	}
	else if (button == GLUT_LEFT_BUTTON)//left button down, model translate
	{
		transType = TYPE_TRANS;
		vassign(v0, x, y, 1);
	}
	else if ((button == 3) || (button == 4))  // Wheel reports as button 3(scroll up) and button 4(scroll down) // wheel up, zoom in; wheel down, zoom out
	{
		GLfloat scale2 = 1.05f;
		GLfloat scale3 = 1.00f;
		if (button == 3)
		{
			scale /= scale2;
			scale3 = scale2;
		}
		else
		{
			scale *= scale2;
			scale3 = 1 / scale2;
		}
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glScalef(scale3, scale3, 1);
		glMultMatrixf(project);
		glGetFloatv(GL_PROJECTION_MATRIX, project);

		glutPostRedisplay();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
};

void mousemove(int x, int y)
{
	if (transType == TYPE_ROT)
	{
		float axis[3], angle;

		vassign(v1, 3.5*x / glutGet(GLUT_WINDOW_HEIGHT) - 1, -3.5*y / glutGet(GLUT_WINDOW_WIDTH) + 1, 1);
		normalize(v1);
		if (dot(v0, v1)>.999999)
			return;
		cross(axis, v0, v1);
		normalize(axis);
		angle = acosf(clamp(dot(v0, v1), -1, 1));
		vassign(v0, v1);

		glPushMatrix();
		glLoadIdentity();
		glRotatef(angle * 180 / PI, axis[0], axis[1], axis[2]);
		switch (iRenderMode)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			glMultMatrixf(m1);
			glGetFloatv(GL_MODELVIEW_MATRIX, m1);
			break;
		default:
			glMultMatrixf(mo);
			glGetFloatv(GL_MODELVIEW_MATRIX, mo);
			break;
		}

		glPopMatrix();
	}
	else if (transType == TYPE_TRANS)
	{
		glPushMatrix();
		glLoadIdentity();
		//glTranslatef((x - v0[0])*(2 * 200 * tan(22.5*PI / 180)) / y, (y - v0[1])*(2 * 200 * tan(22.5*PI / 180)) / y, 0.0f);
		glTranslatef((x - v0[0])*0.5f, (y - v0[1])*-0.5f, 0.0f);
		glMultMatrixf(mo);
		glGetFloatv(GL_MODELVIEW_MATRIX, mo);

		glPopMatrix();
		vassign(v0, x, y, 1);
	}
	glutPostRedisplay();
};

void SetupRC()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	srand(time(0));

	//***particle and field***//
	std::cout.precision(10);
	std::cout << std::fixed;
	
	spField[0].Set(fieldPos1, fieldPos1);
	spField[0].SetAccer(50);

	spField[1].Set(fieldPos2, fieldPos1);
	spField[1].SetAccer(400);

	spField[2].Set(fieldPos3, fieldPos1);
	spField[2].SetAccer(0.0f);

	spField[3].Set(fieldPos4, fieldPos1);
	spField[4].SetAccer(0.0f);

	spField[4].SetAccer(fieldPos1);
	//***particle and field***//

}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (KEYF3)
	{
		fieldPos1[0] = 150 * sin(fieldTimer * 4 / 180.0*PI);
		fieldPos1[1] = 150 * cos(fieldTimer * 4 / 180.0*PI);
		fieldPos1[2] = 200;

		fieldPos2[0] = -150 * sin(fieldTimer * 4 / 180.0*PI);
		fieldPos2[1] = -150 * cos(fieldTimer * 4 / 180.0*PI);
		fieldPos2[2] = 200;

		spField[0].Set(fieldPos1, filedZero);
		spField[1].Set(fieldPos2, filedZero);
	}



	//***particle and field***//
	s1.gravityReset();
	s1.gravityFlat(spField[4]);
	s1.gravityByGravityField(spField[0], field1b);
	s1.gravityByGravityField(spField[1], field2b);
	s1.gravityByGravityField(spField[2], field3b);
	s1.gravityByGravityField(spField[3], field4b);
	s1.Launch(timer);
	//***particle and field***//
	timer++;
	glPushMatrix();
	glMultMatrixf(mo);
	glMultMatrixf(m1);

	glColor3f(1,1, 1);
	glPointSize(5);
	glBegin(GL_POINTS);
	verplot(fieldPos1);
	verplot(fieldPos2);
	verplot(fieldPos3);
	verplot(fieldPos4);
	glEnd();


	glPointSize(2);
	float a[3];
	float RGB[3];
	for (int k = 0; k < parNum; k++)
	{
		float h = 0;
		if (VeloColor)
		{
			h = s1.GetAbsParVelo(k) / 6.0f;
		}
		else{
			h = s1.GetAbsParAcce(k) * 10.0f;
		}
		hsvToRgb(h, RGB);
		glColor3f(RGB[0], RGB[1], RGB[2]);
		
		s1.GetParPosi(k, a);
		
		glBegin(GL_POINTS);
		verplot(a);
		glEnd();
	}


	
	glPopMatrix();

	glutSwapBuffers();
}

void SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_F1)
	{
		KEYF3 = false;
		sourcePosi[0] = -100;
		sourcePosi[1] = 0;
		sourcePosi[2] = 0;

		field1b = true;
		field2b = false;
		field3b = false;
		field4b = false;

		fieldPos1[0] = 0;
		fieldPos1[1] = 0;
		fieldPos1[2] = 0;

		fieldPos2[0] = 100;
		fieldPos2[1] = 0;
		fieldPos2[2] = 0;

		fieldPos3[0] = 0;
		fieldPos3[1] = 0;
		fieldPos3[2] = 0;

		fieldPos4[0] = 0;
		fieldPos4[1] = 0;
		fieldPos4[2] = 0;

		s1.Set(sourcePosi, filedZero);
		s1.SetVeloNMag(0.5, veloVec);

		spField[0].Set(fieldPos1, filedZero);
		spField[0].SetAccer(50);

		spField[1].Set(fieldPos2, filedZero);
		spField[1].SetAccer(400);

		spField[2].Set(filedZero, filedZero);
		spField[2].SetAccer(filedZero);
		spField[2].SetAccer(0.0f);

		spField[3].Set(filedZero, filedZero);
		spField[3].SetAccer(filedZero);
		spField[3].SetAccer(0.0f);
		
		spField[4].SetAccer(filedZero);
	}

	if (key == GLUT_KEY_F2){
		KEYF3 = false;
		float kk[3] = { 0, 1, 0 };

		sourcePosi[0] = 0;
		sourcePosi[1] = 50;
		sourcePosi[2] = 0;

		field1b = false;
		field2b = false;
		field3b = false;
		field4b = false;

		fieldPos1[0] = -200;
		fieldPos1[1] = 0;
		fieldPos1[2] = 0;

		fieldPos2[0] = 200;
		fieldPos2[1] = 0;
		fieldPos2[2] = 0;

		fieldPos3[0] = 0;
		fieldPos3[1] = 0;
		fieldPos3[2] = 200;

		fieldPos4[0] = 0;
		fieldPos4[1] = 0;
		fieldPos4[2] = -200;

		s1.Set(sourcePosi, filedZero);
		s1.SetVeloNMag(2, kk);

		spField[0].Set(fieldPos1, filedZero);
		spField[0].SetAccer(390);

		spField[1].Set(fieldPos2, filedZero);
		spField[1].SetAccer(390);

		spField[2].Set(fieldPos3, filedZero);
		spField[2].SetAccer(390);

		spField[3].Set(fieldPos4, filedZero);
		spField[3].SetAccer(390);

		spField[4].SetAccer(flatfield);
	}
		

	if (key == GLUT_KEY_F3){
		KEYF3 = TRUE;
		float kk[3] = { 0, 0, -1 };

		sourcePosi[0] = 0;
		sourcePosi[1] = 0;
		sourcePosi[2] = 0;

		field1b = false;
		field2b = false;
		field3b = false;
		field4b = false;

		fieldPos1[0] = 150 * sin(fieldTimer * 4 / 180.0*PI);
		fieldPos1[1] = 150 * cos(fieldTimer * 4 / 180.0*PI);
		fieldPos1[2] = 200;

		fieldPos2[0] = -150 * sin(fieldTimer * 4 / 180.0*PI);
		fieldPos2[1] = -150 * cos(fieldTimer * 4 / 180.0*PI);
		fieldPos2[2] = 200;

		fieldPos3[0] = 0;
		fieldPos3[1] = 150;
		fieldPos3[2] = 0;

		fieldPos4[0] = 0;
		fieldPos4[1] = -150;
		fieldPos4[2] = 0;

		s1.Set(sourcePosi, filedZero);
		s1.SetVeloNMag(2, kk);

		spField[0].Set(fieldPos1, filedZero);
		spField[0].SetAccer(350);

		spField[1].Set(fieldPos2, filedZero);
		spField[1].SetAccer(350);

		spField[2].Set(fieldPos3, filedZero);
		spField[2].SetAccer(50);

		spField[3].Set(fieldPos4, filedZero);
		spField[3].SetAccer(50);

		spField[4].SetAccer(filedZero);
	}
		

	if (key == GLUT_KEY_F4)
	{
		s1.AllReset();
	}
	
	if (key == GLUT_KEY_F5)
	{
		VeloColor = !VeloColor;
	}

	// Refresh the Window
	glutPostRedisplay();
}

void empty(int){ 
	glutPostRedisplay();
	glutTimerFunc(10, empty, 0);
	fieldTimer = (fieldTimer+1)%90;
}

int _tmain(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Particle Simulator");
	glutReshapeFunc(ChangeSize);

	glutMouseFunc(mousebutton);
	glutMotionFunc(mousemove);

	glutSpecialFunc(SpecialKeys);
	SetupRC();
	glutDisplayFunc(RenderScene);
	glutTimerFunc(10,empty, 0);
	glutMainLoop();

	return 0;
}

