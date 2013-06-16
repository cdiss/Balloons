#ifndef _RENDER_H
#define _RENDER_H

#include <time.h> //for timer
#include <sstream> //for timer, to convert double to string
#include <stdlib.h>
#include <math.h>
#include <iostream>
#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include "glut.h"		//GLUT and OpenGL functions
#endif

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
  void specialKeyStroke(int key, int x, int y);
	
private:
	// These functions are private and cannot be called anywhere outside of render class
	void drawTextureBox(void);
	void clamp(float v0, float v1, float v2);

	// Variables used in the member functions of render class
	float eye[3];
	float rot[3];
	int Wx, Wy;
	int LEFT, MIDDLE, RIGHT;
	int mButton;
	int mOldX, mOldY;
	
	GLUquadricObj *qobj;
	void drawCeiling(void);
	void drawObjBalloon(void);
  void drawGunSights(void);
  void drawTimer(void);
  void drawScore(void);
};

#endif
