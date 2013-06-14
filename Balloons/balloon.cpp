#include "balloon.hpp"
#include <cstdlib> // rand()
#include <ctime>
#include <iostream>
using namespace std;

Balloon::Balloon() {
  pos[0] = rand()%400 - 200.0f;
  pos[1] = -100.0f;
  pos[2] = rand()%370 - 200.0f;
  spd = ((float)(rand()%100))/100.0f+1.0f;
}

void Balloon::randomizeForStart() {
  pos[1] = rand()%150 - 100.0f;
}

void Balloon::recompute() {
  pos[1] += spd;
}

bool Balloon::isOutOfBounds() {
  return (pos[1] > 100.0f);
}
