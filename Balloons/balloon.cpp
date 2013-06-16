#include <stdlib.h> // rand()
#include <ctime>
#include <iostream>
#include "balloon.hpp"

using namespace std;

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
  spd = ((float)(rand()%100))/50.0f+0.5f;
}

GLMmodel* Balloon::pmodel = NULL;
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
    glmDraw(pmodel, GLM_SMOOTH | GLM_MATERIAL);
    break;
  case YELLOW:
    glmDraw(pmodelYellow, GLM_SMOOTH | GLM_MATERIAL);
  case GREEN:
    glmDraw(pmodelGreen, GLM_SMOOTH | GLM_MATERIAL);
  case BLUE:
    glmDraw(pmodelBlue, GLM_SMOOTH | GLM_MATERIAL);
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

void Balloon::setModel(GLMmodel* aModel) {
  pmodel = aModel;
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


