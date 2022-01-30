#ifndef __game_objects_h__
#define __game_objects_h__

#include "glut.h"

enum gameObjectType {
  ballObj = 1,
  platformObj,
  blockObj,
  bonusObj
};

enum bonusType {
  noBonus = 0,
  platformTiny,
  platformBig,
  extraLive,
  superBall,
  multiball,
  points,
  speedUpBall,
  slowDownBall
};

class gameObject {
protected:
  int width, height;
  double xPosition, yPosition;
  double xSpeed, ySpeed, speedAbs;

  int effectMaxTimerInMilliseconds;
public:
  gameObject();

  int GetWidth();
  int GetHeight();
  int GetXPosition();
  int GetYPosition();

  void SetWidth(int newWidth);
  void SetHeight(int newHeight);
  void SetXPosition(int newXPos);
  void SetYPosition(int newYPos);

  void Draw(int scrW, int scrH, float r, float g, float b);
  int CheckCollision(gameObject& object, double& collisionSquare);

  void UpdatePosition();
  virtual void UpdateEffects();
};

class ball : public gameObject {
private:
  double oldSpeed;
  int isSpeedChanged, speedChangeEffectTimer;
  int isSuper, superEffectTimer;

public:
  ball(int ballWidth, int ballHeight, int xPos, int yPos, double ballSpeed, int way);

  int CheckScreenCollision(int x0, int y0, int scrW, int scrH);

  void MakeBallSwitchVertDirection();
  void MakeBallSwitchHorizDirection();

  void MakeSpeedEffect(int effect);
  void MakeSuperBallEffect();

  int IfHaveSpeedEffect();
  int IfHaveSuperBallEffect();
  virtual void UpdateEffects();
};

class platform : public gameObject {
private:
  int isResized, resizedEffectTimer, oldSize;

public:
  platform();
  platform(int platformWidth, int platformHeight, int xPos, int yPos, double platformSpeed);

  void MoveLeft();
  void MoveRight();

  void MakeResizeEffect(int effect);
  void DestroyEffects();
  virtual void UpdateEffects();
};

class block : public gameObject {
private:
  int maxHits, currentHits;
  int bonusType;
public:
  block(int blockWidth, int blockHeight, int blockX, int blockY, int maxHitsCount, int blockBonusType);

  int GetMaxHits();

  void UpdateHits();
  void DestroyBlock();
  int HitsRemains();

  int hasBonus();
};

class bonus : public gameObject {
private:
  int bonusType;
public:
  bonus(int bonusWidth, int bonusHeight, int xPos, int yPos, double newBonusType);

  int GetType();
};

#endif  // __game_objects_h__