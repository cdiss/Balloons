#include <stdlib.h> // rand()
#include <ctime>
#include <iostream>
#include "balloon.hpp"

using namespace std;

float* x_prod(float* v1, float* v2);

Balloon::Balloon(BalloonColor aColor) {
  if (aColor == NONE) {
    int newColor_int = rand()%4;
    if (newColor_int == 0) {
      aColor = RED;
    } else if (newColor_int == 1){
        aColor = BLUE;
    } else if (newColor_int == 2){
        aColor = GREEN;
    } else {
        aColor = YELLOW;
    }
  }
  pos[0] = rand()%400 - 200.0f;
  pos[1] = -100.0f;
  pos[2] = rand()%370 - 200.0f;
  color = aColor;
  spd = ((float)(rand()%100))/200.0f+0.5f;
}

GLMmodel* Balloon::pmodelRed = NULL;
GLMmodel* Balloon::pmodelYellow = NULL;
GLMmodel* Balloon::pmodelBlue = NULL;
GLMmodel* Balloon::pmodelGreen = NULL;

void Balloon::randomizeForStart() {
  pos[1] = rand()%150 - 100.0f;
}

void Balloon::draw() {
  glPushMatrix();
  glTranslatef(pos[0], pos[1], pos[2]);
  glScalef(5, 5, 5);
  glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
  switch(color) {
  case RED:
    glmDraw(pmodelRed, GLM_SMOOTH | GLM_MATERIAL);
    break;
  case YELLOW:
    glmDraw(pmodelYellow, GLM_SMOOTH | GLM_MATERIAL);
    break;
  case GREEN:
    glmDraw(pmodelGreen, GLM_SMOOTH | GLM_MATERIAL);
    break;
  case BLUE:
    glmDraw(pmodelBlue, GLM_SMOOTH | GLM_MATERIAL);
    break;
  default:
    break;
  }
  glPopMatrix();
}

void Balloon::recompute() {
  pos[1] += spd;
}

bool Balloon::isOutOfBounds() {
  return (pos[1] > 100.0f);
}

float* Balloon::findCollisionVectorWith(float* point, float* unit_vector) {
  float w[3] = {pos[0] - point[0], pos[1] - point[1], pos[2] - point[2]};
  return x_prod(unit_vector, w);
}

void Balloon::setModelRed(GLMmodel* aModel) {
  pmodelRed = aModel;
}

void Balloon::setModelYellow(GLMmodel* aModel){
    pmodelYellow = aModel;
}

void Balloon::setModelBlue(GLMmodel* aModel){
    pmodelBlue = aModel;
}

void Balloon::setModelGreen(GLMmodel* aModel){
    pmodelGreen = aModel;
}

float* x_prod(float* v1, float* v2) {
  float* return_me = new float[3];
  return_me[0] = v1[1]*v2[2] - v1[2]*v2[1];
  return_me[1] = v1[2]*v2[0] - v1[0]*v2[2];
  return_me[2] = v1[0]*v2[1] - v1[1]*v2[0];
  return return_me;
}

