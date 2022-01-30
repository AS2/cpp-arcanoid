#include <math.h>
#include "glut.h"
#include "gameObjects.h"

/*******
 * 'gameObject' class methods
 *******/

gameObject::gameObject() {
  width = height = xPosition = yPosition = 0;
}

int gameObject::GetWidth() {
  return width;
}

int gameObject::GetHeight() {
  return height;
}

int gameObject::GetXPosition() {
  return xPosition;
}

int gameObject::GetYPosition() {
  return yPosition;
}

void gameObject::SetWidth(int newWidth) {
  width = newWidth;
}

void gameObject::SetHeight(int newHeight) {
  height = newHeight;
}

void gameObject::SetXPosition(int newXPos) {
  xPosition = newXPos;
}

void gameObject::SetYPosition(int newYPos) {
  yPosition = newYPos;
}

void gameObject::UpdateEffects() {
}

void gameObject::Draw(int scrW, int scrH, float r, float g, float b) {
  glColor3f(r, g, b);
  //glRectd(-0.5, 0.5, 0.5, -0.5);
  glRectd((2.0 * xPosition / scrW) - 1, 1 - (2.0 * yPosition / scrH), (2.0 * (xPosition + width) / scrW) - 1, 1 - (2.0 * (yPosition + height) / scrH));
}

int gameObject::CheckCollision(gameObject& object, double& collisionSquare) {
  double vertCollSquare = -1, horizCollSquare = -1;

  // chech collision from left
  if (xPosition + width >= object.xPosition && xPosition < object.xPosition &&
    ((yPosition >= object.yPosition && yPosition < object.yPosition + object.height) ||
    (yPosition + height >= object.yPosition && yPosition + height < object.yPosition + object.height) ||
    (yPosition < object.yPosition && yPosition + height >= object.yPosition + object.height)))
    vertCollSquare = fabs(fmin(yPosition + height, object.yPosition + object.height) - fmax(yPosition, object.yPosition));
  // chech collision from right
  else if (xPosition <= object.width + object.xPosition && width + xPosition > object.width + object.xPosition &&
    ((yPosition >= object.yPosition && yPosition < object.yPosition + object.height) ||
    (yPosition + height >= object.yPosition && yPosition + height < object.yPosition + object.height) ||
    (yPosition < object.yPosition && yPosition + height >= object.yPosition + object.height)))
    vertCollSquare = fabs(fmin(yPosition + height, object.yPosition + object.height) - fmax(yPosition, object.yPosition));

  // chech collision from up
  if (height + yPosition >= object.yPosition && yPosition < object.yPosition &&
    ((xPosition >= object.xPosition && xPosition < object.xPosition + object.width) ||
    (xPosition + width >= object.xPosition && xPosition + width < object.xPosition + object.width) ||
    (xPosition < object.xPosition && xPosition + width >= object.xPosition + object.width)))
    horizCollSquare = fabs(fmin(xPosition + width, object.xPosition + object.width) - fmax(xPosition, object.xPosition));
  // chech collision from down
  else if (yPosition <= object.height + object.yPosition && yPosition + height > object.height + object.yPosition &&
    ((xPosition >= object.xPosition && xPosition < object.xPosition + object.width) ||
    (xPosition + width >= object.xPosition && xPosition + width < object.xPosition + object.width) ||
    (xPosition < object.xPosition && xPosition + width >= object.xPosition + object.width)))
    horizCollSquare = fabs(fmin(xPosition + width, object.xPosition + object.width) - fmax(xPosition, object.xPosition));

  // no collision
  if (vertCollSquare == -1 && horizCollSquare == -1)
    return 0;
  // horizontal collision
  else if (vertCollSquare == -1 || (vertCollSquare != -1 && horizCollSquare > vertCollSquare)) {
    collisionSquare = horizCollSquare;
    return 1;
  }
  // vertical collision
  else if (horizCollSquare == -1 || (horizCollSquare != -1 && horizCollSquare < vertCollSquare)) {
    collisionSquare = vertCollSquare;
    return 2;
  }
  // corner collision
  else if (horizCollSquare != -1 && vertCollSquare != -1 && horizCollSquare - vertCollSquare < 0.05) {
    collisionSquare = vertCollSquare;
    return 3;
  }
}

void gameObject::UpdatePosition() {
  xPosition += xSpeed * speedAbs;
  yPosition += ySpeed * speedAbs;
}

/*******
 * 'ball' class methods
 *******/

ball::ball(int ballWidth, int ballHeight, int xPos, int yPos, double ballSpeed, int way) {
  width = ballWidth;
  height = ballHeight;
  xPosition = xPos;
  yPosition = yPos;
  xSpeed = (1.0 / sqrt(2)) * (way);
  ySpeed = -xSpeed * (way);
  speedAbs = oldSpeed = ballSpeed;
  isSpeedChanged = speedChangeEffectTimer = isSuper = superEffectTimer = 0;
  effectMaxTimerInMilliseconds = 1000;
}

int ball::CheckScreenCollision(int x0, int y0, int scrW, int scrH) {
  if (xPosition + width >= scrW + x0)
    return 1;
  else if (xPosition < x0)
    return 2;
  else if (yPosition + height >= scrH + y0)
    return 3;
  else if (yPosition < y0)
    return 4;
  else
    return 0;
}

void ball::MakeBallSwitchVertDirection() {
  xSpeed = -xSpeed;
}

void ball::MakeBallSwitchHorizDirection() {
  ySpeed = -ySpeed;
}

void ball::MakeSpeedEffect(int effect) {
  if (isSpeedChanged == 0)
    oldSpeed = speedAbs;

  if (effect == 1)
    speedAbs *= 2;
  else if (effect == -1)
    speedAbs /= 2;

  isSpeedChanged = effect;
  speedChangeEffectTimer = 0;
}

void ball::MakeSuperBallEffect() {
  isSuper = 1;
  superEffectTimer = 0;
}

void ball::UpdateEffects() {
  if (isSpeedChanged != 0 && speedChangeEffectTimer < effectMaxTimerInMilliseconds)
    speedChangeEffectTimer++;
  else if (isSpeedChanged != 0 && speedChangeEffectTimer >= effectMaxTimerInMilliseconds) {
    speedAbs = oldSpeed;
    isSpeedChanged = speedChangeEffectTimer = 0;
  }

  if (isSuper != 0 && superEffectTimer < effectMaxTimerInMilliseconds)
    superEffectTimer++;
  else if (isSuper != 0 && superEffectTimer >= effectMaxTimerInMilliseconds)
    isSuper = superEffectTimer = 0;
}

int ball::IfHaveSpeedEffect() {
  return isSpeedChanged;
}
int ball::IfHaveSuperBallEffect() {
  return isSuper;
}

/*******
 * 'platform' class methods
 *******/

platform::platform() {
  width = height = xSpeed = ySpeed = xPosition = yPosition = isResized = resizedEffectTimer = oldSize = 0;
}

platform::platform(int platformWidth, int platformHeight, int xPos, int yPos, double platformSpeed) {
  width = oldSize = platformWidth;
  height = platformHeight;
  xPosition = xPos;
  yPosition = yPos;
  xSpeed = 2;
  ySpeed = 0;
  speedAbs = platformSpeed;
  isResized = resizedEffectTimer = 0;
  effectMaxTimerInMilliseconds = 1000;
}

void platform::MoveLeft() {
  xPosition += xSpeed * speedAbs;
}

void platform::MoveRight() {
  xPosition -= xSpeed * speedAbs;
}

void platform::MakeResizeEffect(int effect) {
  if (isResized == 0)
    oldSize = width;

  if (effect == -1)
    width /= 2;
  else if (effect == 1)
    width *= 2;

  resizedEffectTimer = 0;
  isResized = 1;
}

void platform::DestroyEffects() {
  if (isResized != 0) {
    width = oldSize;
    isResized = resizedEffectTimer = 0;
  }
}

void platform::UpdateEffects() {
  if (isResized != 0 && resizedEffectTimer < effectMaxTimerInMilliseconds)
    resizedEffectTimer++;
  else if (isResized != 0 && resizedEffectTimer >= effectMaxTimerInMilliseconds)
    DestroyEffects();
}

/*******
 * 'block' class methods
 *******/

block::block(int blockWidth, int blockHeight, int blockX, int blockY, int maxHitsCount, int blockBonusType) {
  width = blockWidth;
  height = blockHeight;
  xPosition = blockX;
  yPosition = blockY;
  xSpeed = ySpeed = currentHits = 0;
  maxHits = maxHitsCount;
  bonusType = blockBonusType;
}

int block::GetMaxHits() {
  return maxHits;
}

void block::UpdateHits() {
  currentHits++;
}

void block::DestroyBlock() {
  currentHits = maxHits;
}

int block::HitsRemains() {
  return maxHits - currentHits;
}

int block::hasBonus() {
  return bonusType;
}

/*******
 * 'bonus' class methods
 *******/

bonus::bonus(int bonusWidth, int bonusHeight, int xPos, int yPos, double newBonusType) {
  width = bonusWidth;
  height = bonusHeight;
  xPosition = xPos;
  yPosition = yPos;
  xSpeed = 0;
  ySpeed = 1;
  speedAbs = 2;
  bonusType = newBonusType;
}

int bonus::GetType() {
  return bonusType;
}
