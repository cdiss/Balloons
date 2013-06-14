#include "render.h"
#include <vector>
#include "balloon.hpp"

#define TIMER 33

using namespace std;

void recomputeFrame(int value);
std::vector<Balloon*> balloons;

void recomputeFrame(int value)
{
  for (unsigned int i=0; i<balloons.size(); i++) {
    balloons.at(i)->recompute(); 
	  if (balloons.at(i)->isOutOfBounds()) {
      delete balloons.at(i);
      balloons.erase(balloons.begin()+i);
      balloons.push_back(new Balloon()); // for the moment, immediately replace the balloon that went off the top
    }
  }
  glutPostRedisplay();
	glutTimerFunc(TIMER, recomputeFrame, value);
}

// Default constructor
// used for initializing any user defined variables
Render::Render()
{
	// specify a default location for the camera
	eye[0] = 0.0f;
	eye[1] = 0.0f;
	eye[2] = 450.0f;

	// specify default values to the rotational values in the transformation matrix
	rot[0] = 0.0f;
	rot[1] = 0.0f;
	rot[2] = 0.0f;

	LEFT = 0;
	MIDDLE = 1;
	RIGHT = 2;
	mButton = -1;
}

Render::~Render()
{


}

void Render::init(void)
{
	// clear the background to the color values specified in the parentheses
	glClearColor (0.0, 0.0, 0.0, 0.0);

	// this is a 3D scene. so, clear the depth
	glClearDepth(1.0);

	// I need depth. enable depth
	glEnable(GL_DEPTH_TEST);

	glShadeModel (GL_SMOOTH);


	// Initialize lighting
	glEnable (GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);


	// Setup Ambient lighting parameters
	GLfloat ambientColor[] = { 0.1f, 0.1f, 0.1f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	// set up positioned lights
	GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos0[] = {4.0f, 0.0f, 0.0f, 1.0f}; //Position
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	 //Add directed light
    GLfloat lightColor1[] = {0.5f, 0.2f, 0.2f, 1.0f}; //Color (0.5, 0.2, 0.2)
    //Coming from the direction (-1, 0.5, 0.5)
    GLfloat lightPos1[] = {-1.0f, 0.5f, 0.5f, 0.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricTexture(qobj, GL_TRUE);
	gluQuadricNormals(qobj, GLU_SMOOTH);

	recomputeFrame(0);

  // Place 10 initial balloons
  for(int i=0; i<10; i++) {
    Balloon* newballoon = new Balloon();
    newballoon->randomizeForStart();
    balloons.push_back(newballoon);
  }
	
	// Read an obj file and load it, but not displayed yet
  GLMmodel* pmodel = NULL;
  if (!pmodel) {
      pmodel = glmReadOBJ("data/balloon.obj");
      if (!pmodel) { 
			  cout << "POOOOOOP" << endl;
			  exit(0);
		  }
      glmUnitize(pmodel);
      glmFacetNormals(pmodel);
      glmVertexNormals(pmodel, 90.0);
  }
	Balloon::setModel(pmodel);

}

void Render::reshape(int w, int h)
{
	glViewport(0,0,(GLsizei) w, (GLsizei) h);
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
			default:
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

}

void Render::keyPos(unsigned char key, int x, int y)
{
	switch (key)
	{
		default:
			break;
	    case 27:	// this is for the 'Esc' key on the keyboard, quit the program when the esc key is pressed
			exit (0);
	}
}



void Render::display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Specifies which matrix stack is the target for subsequent matrix operations
	// In this example, the projection matrix is set to perspective projection matrix stack
	glMatrixMode(GL_PROJECTION);
	// all matrix values from previous frames set to identity
	glLoadIdentity();
	
	// perspective projection loaded with new values for Wx and Wy updated
	gluPerspective(45, (GLfloat) Wx/(GLfloat) Wy, 1.0, 10000.0);
	// use glOrtho for a 3D orthogonal projection
	//glOrtho(-100, 100, -100, 100, -100, 100);

	// Applies subsequent matrix operations to the modelview matrix stack.
	glMatrixMode(GL_MODELVIEW);
	// Clears all the previously loaded values in the modelview matrix
	glLoadIdentity();
	// Now, add a bunch of transformation operations
	glTranslatef(-eye[0], -eye[1], -eye[2]);
    glRotatef(rot[0], 1.0f, 0.0f, 0.0f);
    glRotatef(rot[1], 0.0f, 1.0f, 0.0f);
    glRotatef(rot[2], 0.0f, 0.0f, 1.0f);
  
  drawCeiling();

  glDisable(GL_COLOR_MATERIAL);
	
  for(int i=0; i<10; i++) {
    balloons.at(i)->draw();
  }

	
  glEnable(GL_COLOR_MATERIAL);
	
  // this allows opengl to wait for the draw buffer to be ready in the background for the next frame
	// therefore, while the current buffer is being drawn in the current frame, a buffer is set ready to draw on frame+1
	// this call is effective only when GL_DOUBLE is enabled in glutInitDisplayMode in the main function
	// It is recommended to use glutSwapBuffers and double buffering always
	glutSwapBuffers();
}


//void Render::drawObjBalloon(void)
//{
//	glPushMatrix();
//		glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
//    glmDraw(pmodel, GLM_SMOOTH | GLM_MATERIAL);
//	glPopMatrix();
//}

void Render::drawCeiling(void) 
{
  glPushMatrix();
  float ambientCeiling[] = {0.2f, 0.2f, 0.2f, 1.0f};
  float diffuseCeiling[] = {0.4f, 0.4f, 0.4f, 1.0f};
  float specularCeiling[] = {0.4f, 0.4f, 0.4f, 1.0f};
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambientCeiling);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseCeiling);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specularCeiling);
  glBegin(GL_POLYGON);
    glVertex3f(-200.0f, 107.0f, -200.0f);
    glVertex3f(-200.0f, 107.0f, 170.0f);
    glVertex3f(200.0f, 107.0f, 170.0f);
    glVertex3f(200.0f, 107.0f, -200.0f);
  glEnd();
  glPopMatrix();
}

// this is for clamping the numbers between 0 & 360. used for rotation values in the mouse move function
void Render::clamp(float v0, float v1, float v2)
{
	if (v0 > 360 || v0 < -360)
		v0 = 0;
	if (v1 > 360 || v1 < -360)
		v1 = 0;
	if (v2 > 360 || v2 < -360)
		v2 = 0;
}

