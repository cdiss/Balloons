#ifndef BALLOON_HPP_
#define BALLOON_HPP_

class Balloon {
public:
  Balloon();
  void randomizeForStart();
  void recompute();
  bool isOutOfBounds();

  float pos[3];
  float spd;
};

#endif