#include "render.h"
#include <vector>
#include "balloon.hpp"
#include "SOIL.h"

#define TIMER 33

using namespace std;

  const float LASER_SIGHT_LENGTH = 600.0f;
  const float CEILING_HEIGHT = 135.0f;
  const float CEILING_MIN_X = -200.0f;
  const float CEILING_MAX_X = 200.0f;
  const float CEILING_MIN_Z = -200.0f;
  const float CEILING_MAX_Z = 170.0f;
  const float GUN_POS[] = {0.0f, -50.0f, 350.0f};
  const float ACCURACY_TOLERANCE = 15.0f;

void recomputeFrame(int value);
float mag(float* v);

std::vector<Balloon*> balloons;
float gunYawDegrees = 0.0f;
float gunPitchDegrees = 30.0f;

int balloonCollisionIndex = -1;

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
	GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	// set up positioned lights
	GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos0[] = {400.0f, 200.0f, 300.0f, 1.0f}; //Positioned at (4, 0, 8)
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	/*//Add directed light
    GLfloat lightColor1[] = {0.3f, 0.1f, 0.1f, 1.0f}; //Color (0.5, 0.2, 0.2)
    GLfloat lightPos1[] = {-1.0f, 0.5f, 0.5f, 0.0f};  //Coming from the direction (-1, 0.5, 0.5)
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
     */
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
  GLMmodel* pmodelRed = NULL;
  GLMmodel* pmodelYellow = NULL;
  GLMmodel* pmodelBlue = NULL;
  GLMmodel* pmodelGreen = NULL;
    
  if (!pmodelRed) {
      pmodelRed = glmReadOBJ("data/balloonRed.obj");
      if (!pmodelRed) {
			  exit(0);
		  }
      glmUnitize(pmodelRed);
      glmFacetNormals(pmodelRed);
      glmVertexNormals(pmodelRed, 90.0);
  }
	Balloon::setModelRed(pmodelRed);
    
    if (!pmodelYellow) {
        pmodelYellow = glmReadOBJ("data/balloonYellow.obj");
        if (!pmodelYellow) {
            exit(0);
        }
        glmUnitize(pmodelYellow);
        glmFacetNormals(pmodelYellow);
        glmVertexNormals(pmodelYellow, 90.0);
    }
	Balloon::setModelYellow(pmodelYellow);
    
    if (!pmodelBlue) {
        pmodelBlue = glmReadOBJ("data/balloonBlue.obj");
        if (!pmodelBlue) {
            exit(0);
        }
        glmUnitize(pmodelBlue);
        glmFacetNormals(pmodelBlue);
        glmVertexNormals(pmodelBlue, 90.0);
    }
	Balloon::setModelBlue(pmodelBlue);
    
    if (!pmodelGreen) {
        pmodelGreen = glmReadOBJ("data/balloonGreen.obj");
        if (!pmodelGreen) {
            exit(0);
        }
        glmUnitize(pmodelGreen);
        glmFacetNormals(pmodelGreen);
        glmVertexNormals(pmodelGreen, 90.0);
    }
	Balloon::setModelGreen(pmodelGreen);
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
        if (balloonCollisionIndex >= 0) {
          delete balloons.at(balloonCollisionIndex);
          balloons.erase(balloons.begin()+balloonCollisionIndex);
          balloons.push_back(new Balloon());
        }
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
	/*
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
  */

  float angle = atan(((float)y-mOldY)/((float)x-mOldX));
  if (x-mOldX > 0.0f) {  // move was somewhat to the right
    if (angle < -M_PI/3) {  // move down only
      gunPitchDegrees -= 0.5f;
    } else if (angle < -M_PI/6) {  // move both down and right
      gunPitchDegrees -= 0.5f;
      gunYawDegrees -= 0.5f;
    } else if (angle < M_PI/6) {  // move right only
      gunYawDegrees -= 0.5f;
    } else if (angle < M_PI/3) {  // move both up and right
      gunPitchDegrees += 0.5f;
      gunYawDegrees -= 0.5f;
    } else {  // move up only
      gunPitchDegrees += 0.5f;
    }
  } else {  // move was somewhat to the left
    if (angle < -M_PI/3) {  // move up only
      gunPitchDegrees += 0.5f;
    } else if (angle < -M_PI/6) {  // move both up and left
      gunPitchDegrees += 0.5f;
      gunYawDegrees += 0.5f;
    } else if (angle < M_PI/6) {  // move left only
      gunYawDegrees += 0.5f;
    } else if (angle < M_PI/3) {  // move both down and left
      gunPitchDegrees -= 0.5f;
      gunYawDegrees += 0.5f;
    } else {  // move down only
      gunPitchDegrees -= 0.5f;
    }
  }
  if(gunYawDegrees > 80.0f) gunYawDegrees = 80.0f;
  if(gunYawDegrees < -80.0f) gunYawDegrees = -80.0f;
  if(gunPitchDegrees > 80.0f) gunPitchDegrees = 80.0f;
  if(gunPitchDegrees < -10.0f) gunPitchDegrees = -10.0f;
  
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

void Render::specialKeyStroke(int key, int x, int y)
{
  switch(key)
  {
  case GLUT_KEY_LEFT:
    gunYawDegrees += 2.0f;
    if(gunYawDegrees > 80.0f) gunYawDegrees = 80.0f;
    break;
  case GLUT_KEY_RIGHT:
    gunYawDegrees -= 2.0f;
    if(gunYawDegrees < -80.0f) gunYawDegrees = -80.0f;
    break;
  case GLUT_KEY_UP:
    gunPitchDegrees += 2.0f;
    if(gunPitchDegrees > 80.0f) gunPitchDegrees = 80.0f;
    break;
  case GLUT_KEY_DOWN:
    gunPitchDegrees -= 2.0f;
    if(gunPitchDegrees < -10.0f) gunPitchDegrees = -10.0f;
    break;
  default:
    break;
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
	gluPerspective(55, (GLfloat) Wx/(GLfloat) Wy, 1.0, 10000.0);
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

    /* load an image file directly as a new OpenGL texture */
    GLuint tex_2d = SOIL_load_OGL_texture
	(
     "pow.png",
     SOIL_LOAD_AUTO,
     SOIL_CREATE_NEW_ID,
     SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
     );
	
    /* check for an error during the load process */
    if( 0 == tex_2d )
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
    }
    
	//WHY?
  glDisable(GL_COLOR_MATERIAL);

	// draw balloons
  for(int i=0; i<10; i++) {
    balloons.at(i)->draw();
  }
	
  // ALSO WHY? 
  glEnable(GL_COLOR_MATERIAL);

  drawGunSights();

  // this allows opengl to wait for the draw buffer to be ready in the background for the next frame
	// therefore, while the current buffer is being drawn in the current frame, a buffer is set ready to draw on frame+1
	// this call is effective only when GL_DOUBLE is enabled in glutInitDisplayMode in the main function
	// It is recommended to use glutSwapBuffers and double buffering always
	glutSwapBuffers();
}

void Render::drawCeiling(void) 
{
  glPushMatrix();
  float ambientCeiling[] = {0.2f, 0.2f, 0.2f, 1.0f};
  float diffuseCeiling[] = {0.4f, 0.4f, 0.4f, 1.0f};
  float specularCeiling[] = {0.4f, 0.4f, 0.4f, 1.0f};
  float emissionCeiling[] = {0.0f, 0.0f, 0.0f, 1.0f};
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambientCeiling);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseCeiling);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specularCeiling);
  glMaterialfv(GL_FRONT, GL_EMISSION, emissionCeiling);
  glBegin(GL_POLYGON);
    glVertex3f(CEILING_MIN_X, CEILING_HEIGHT, CEILING_MIN_Z);
    glVertex3f(CEILING_MIN_X, CEILING_HEIGHT, CEILING_MAX_Z);
    glVertex3f(CEILING_MAX_X, CEILING_HEIGHT, CEILING_MAX_Z);
    glVertex3f(CEILING_MAX_X, CEILING_HEIGHT, CEILING_MIN_Z);
  glEnd();
  glPopMatrix();
}

void Render::drawGunSights(void)
{
  float brightRed[] = {1.0f, 0.0f, 0.0f, 1.0f};
  glMaterialfv(GL_FRONT, GL_AMBIENT, brightRed);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, brightRed);
  glMaterialfv(GL_FRONT, GL_SPECULAR, brightRed);
  glMaterialfv(GL_FRONT, GL_EMISSION, brightRed);
  glLineWidth(1.0f);

  glPushMatrix();
    glTranslatef(GUN_POS[0], GUN_POS[1], GUN_POS[2]);
    glutSolidSphere(5.0, 50, 50);  // the gun for now (programmer art haha)
    glPushMatrix();
      glTranslatef(-2.0f, 0.0f, 0.0f);
      glRotatef(gunYawDegrees, 0.0f, 1.0f, 0.0f);
      glRotatef(gunPitchDegrees, 1.0f, 0.0f, 0.0f);
      glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, -LASER_SIGHT_LENGTH);
      glEnd();
    glPopMatrix();
    glPushMatrix();
      glTranslatef(2.0f, 0.0f, 0.0f);
      glRotatef(gunYawDegrees, 0.0f, 1.0f, 0.0f);
      glRotatef(gunPitchDegrees, 1.0f, 0.0f, 0.0f);
      glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, -LASER_SIGHT_LENGTH);
      glEnd();
    glPopMatrix();
  glPopMatrix();
  
  // In the same coordinate system as the one that the ceiling is drawn in
  float laser_endpoint[3];
  laser_endpoint[0] = GUN_POS[0] - LASER_SIGHT_LENGTH * sin(gunYawDegrees/180.0*M_PI) * cos(gunPitchDegrees/180.0*M_PI);
  laser_endpoint[1] = GUN_POS[1] + LASER_SIGHT_LENGTH * sin(gunPitchDegrees/180.0*M_PI);
  laser_endpoint[2] = GUN_POS[2] - LASER_SIGHT_LENGTH * cos(gunYawDegrees/180.0*M_PI) * cos(gunPitchDegrees/180.0*M_PI);
  //system("CLS");
  //cout << "laser_endpoint: (" << laser_endpoint[0] << ", " << laser_endpoint[1] << ", " << laser_endpoint[2] << ") " << endl;

  // calculation for dots on balloon
  float laser_unit_vector[3];
  laser_unit_vector[0] = (laser_endpoint[0]-GUN_POS[0])/LASER_SIGHT_LENGTH;
  laser_unit_vector[1] = (laser_endpoint[1]-GUN_POS[1])/LASER_SIGHT_LENGTH;
  laser_unit_vector[2] = (laser_endpoint[2]-GUN_POS[2])/LASER_SIGHT_LENGTH;
  bool found = false;
  for (unsigned int i=0; i<balloons.size(); i++) {
    float* collision_vec = balloons[i]->findCollisionVectorWith((float*)GUN_POS, laser_unit_vector);
    //cout << "collision_vec: (" << collision_vec[0] << ", " << collision_vec[1] << ", " << collision_vec[2] << ") " << endl;
    //cout << "mag: " << mag(collision_vec) << endl;
    if(mag(collision_vec) < ACCURACY_TOLERANCE) {
      glPushMatrix();
      glTranslatef(balloons[i]->pos[0], balloons[i]->pos[1], balloons[i]->pos[2]);
      glTranslatef(collision_vec[1], -collision_vec[0], collision_vec[2]+10.0f);
      balloonCollisionIndex = i;
      found = true;
      //cout << "Drawing dot on balloon, at (" << balloons[i]->pos[0] + collision_vec[0] << ", " << balloons[i]->pos[1] + collision_vec[1] << ", " << balloons[i]->pos[2] + collision_vec[2] << ") " << endl;
      glutSolidSphere(7.0, 15, 15);
      glPopMatrix();
    }
    delete[] collision_vec;
  }
  if (!found) balloonCollisionIndex = -1;

  // calculation for ceiling dots
  if(gunPitchDegrees != 0.0f) {   // avoid division by 0
    float laser_intersect_ceiling_percent = (CEILING_HEIGHT-GUN_POS[1])/(laser_endpoint[1]-GUN_POS[1]);  // the percent of the way along the laser sight that the ceiling is hit
    //cout << "laser_intersect_ceiling_percent: " << laser_intersect_ceiling_percent << endl;
    if (laser_intersect_ceiling_percent > 0.0f && laser_intersect_ceiling_percent < 1.0f) {
      float laser_intersect_ceiling[3] = {GUN_POS[0] + laser_intersect_ceiling_percent*(laser_endpoint[0]-GUN_POS[0]), 
                                            GUN_POS[1] + laser_intersect_ceiling_percent*(laser_endpoint[1]-GUN_POS[1]), 
                                            GUN_POS[2] + laser_intersect_ceiling_percent*(laser_endpoint[2]-GUN_POS[2]) };
      //cout << "laser_intersect_ceiling: (" << laser_intersect_ceiling[0] << ", " << laser_intersect_ceiling[1] << ", " << laser_intersect_ceiling[2] << ") " << endl;
      if (laser_intersect_ceiling[0] > CEILING_MIN_X && laser_intersect_ceiling[0] < CEILING_MAX_X &&
          laser_intersect_ceiling[2] > CEILING_MIN_Z && laser_intersect_ceiling[2] < CEILING_MAX_Z ) {
            // draw dot at intersection with ceiling
            glPushMatrix();
            glTranslatef(laser_intersect_ceiling[0], laser_intersect_ceiling[1], laser_intersect_ceiling[2]);
            //cout << "Drawing dots" << endl;
            glutSolidSphere(2.0, 15, 15);
            glPopMatrix();
      } else {
        //cout << "Not drawing dots" << endl;
      }
    } else {
      //cout << "Not calculating ceiling intersection" << endl;
    }
  }
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

float mag(float* v) {
  return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}
