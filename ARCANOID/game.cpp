#include <iostream>
#include <fstream>

#include <string>
#include <sstream>
#include <iterator>

#include "glut.h"
#include "game.h"

const char* levels[] = {
  "levels/lvl0.txt",
  "levels/lvl1.txt",
  "levels/lvl2.txt",
  //"levels/lvl3.txt"
};

screen::screen(int width, int height) {
  // init params
  infoHeight = 23;
  gameZoneHeight = height - infoHeight;
  screenWidth = gameZoneWidth = width;
  screenHeight = height;
  totalPoints = 0;
  lifes = 3;
  ballSpeed = 5;
  platformWidth = 500;
  lvlNo = 0;
  maxLevels = sizeof(levels) / sizeof(levels[0]);
  plat = platform(platformWidth, 20, screenWidth / 2 - platformWidth / 2, screenHeight - 21, 8);
}

static void GetLevelParams(std::string str, int& w, int& h) {
  w = h = 0;
  int strPos;
  for (strPos = 0; str[strPos] != ' '; strPos++)
    w = w * 10 + (str[strPos] - '0');
  for (strPos++; str[strPos] != 0; strPos++)
    h = h * 10 + (str[strPos] - '0');
}

void screen::MakeStartPosition() {
  while (!balls.empty())
    balls.pop_front();
  balls.push_back(ball(20, 20, screenWidth / 2 - 10, screenHeight - 42, ballSpeed * (1 + 0.5 * (double)lvlNo / maxLevels), 1));

  plat.DestroyEffects();
  plat.SetWidth(platformWidth / (1 + 0.7 * (double)lvlNo / maxLevels));
  plat.SetXPosition(screenWidth / 2 - platformWidth / (1 + 0.7 * (double)lvlNo / maxLevels) / 2);
}

void screen::BuildLevel(int levelNumber) {
  // init blocks from 'level' directory
  std::ifstream level(levels[levelNumber]);
  std::string oneStringFromFile;
  getline(level, oneStringFromFile);

  GetLevelParams(oneStringFromFile, blocksInRow, blocksRows);

  totalBlocks = destroyedBlocks = 0;
  int blockWidth = screenWidth / blocksInRow - 2, blockHeight = (int)(0.35 * screenHeight / blocksRows - 2);
  int blocksStartW = (screenWidth - blockWidth * blocksInRow) / 4;
  for (int j = 0; j < blocksRows; j++) {
    getline(level, oneStringFromFile);
    for (int i = 0; i < blocksInRow; i++) {
      switch (oneStringFromFile[i]) {
      case '*':
        totalBlocks--;
        break;
      case '1':
        blocks.push_back(block(blockWidth, blockHeight, blocksStartW + (blockWidth + 2) * i + 1, infoHeight + (blockHeight + 2) * j + 1, 1, noBonus));
        break;
      case '2':
        blocks.push_back(block(blockWidth, blockHeight, blocksStartW + (blockWidth + 2) * i + 1, infoHeight + (blockHeight + 2) * j + 1, 2, noBonus));
        break;
      case '3':
        blocks.push_back(block(blockWidth, blockHeight, blocksStartW + (blockWidth + 2) * i + 1, infoHeight + (blockHeight + 2) * j + 1, 3, noBonus));
        break;
      case 'T':
        blocks.push_back(block(blockWidth, blockHeight, blocksStartW + (blockWidth + 2) * i + 1, infoHeight + (blockHeight + 2) * j + 1, 3, platformTiny));
        break;
      case 'B':
        blocks.push_back(block(blockWidth, blockHeight, blocksStartW + (blockWidth + 2) * i + 1, infoHeight + (blockHeight + 2) * j + 1, 3, platformBig));
        break;
      case 'L':
        blocks.push_back(block(blockWidth, blockHeight, blocksStartW + (blockWidth + 2) * i + 1, infoHeight + (blockHeight + 2) * j + 1, 3, extraLive));
        break;
      case 'H':
        blocks.push_back(block(blockWidth, blockHeight, blocksStartW + (blockWidth + 2) * i + 1, infoHeight + (blockHeight + 2) * j + 1, 3, superBall));
        break;
      case 'M':
        blocks.push_back(block(blockWidth, blockHeight, blocksStartW + (blockWidth + 2) * i + 1, infoHeight + (blockHeight + 2) * j + 1, 3, multiball));
        break;
      case 'P':
        blocks.push_back(block(blockWidth, blockHeight, blocksStartW + (blockWidth + 2) * i + 1, infoHeight + (blockHeight + 2) * j + 1, 3, points));
        break;
      case 'F':
        blocks.push_back(block(blockWidth, blockHeight, blocksStartW + (blockWidth + 2) * i + 1, infoHeight + (blockHeight + 2) * j + 1, 3, speedUpBall));
        break;
      case 'S':
        blocks.push_back(block(blockWidth, blockHeight, blocksStartW + (blockWidth + 2) * i + 1, infoHeight + (blockHeight + 2) * j + 1, 3, slowDownBall));
        break;
      }
      totalBlocks++;
    }
  }
}

void screen::InitGame(int CurrentLevel) {
  // init objects (except blocks)
  lvlNo = CurrentLevel;
  MakeStartPosition();
  BuildLevel(CurrentLevel);
}

void screen::BeginText() {
  // Save the current projection matrix
  glPushMatrix();

  // Make the current matrix the identity matrix
  glLoadIdentity();

  // Set the projection (to 2D orthographic)
  gluOrtho2D(0, screenWidth, 0, screenHeight);
}

void screen::EndText() {
  // Restore the original projection matrix
  glPopMatrix();
}

void screen::DrawInfo() {
  glColor3f(0.8, 0.8, 0.8);
  glRectd(-1, 1, 1, 1.0 - (double)infoHeight * 2 / screenHeight);

  BeginText();

  std::stringstream infoStringStream;
  infoStringStream << "Level: " << std::scientific << lvlNo + 1 << "  Points: " << std::scientific << totalPoints << "  Lifes: " << std::scientific << lifes;
  

  std::string str = infoStringStream.str();

  glColor3f(0, 0, 0);
  glRasterPos2f(10, screenHeight - 18);
  
  for (int i = 0; str[i] != '\0'; i++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);

  EndText();
}

void screen::DrawObjects() {
  DrawInfo();

  // draw balls
  for (auto &ball : balls) {
    if (ball.IfHaveSuperBallEffect() == 1)
      ball.Draw(screenWidth, screenHeight, 1, 0.5, 0);
    else
      ball.Draw(screenWidth, screenHeight, 1, 1, 1);
  }

  // draw bonuses
  for (auto &bonus : bonuses)
    switch (bonus.GetType()) {
    case platformTiny:
      bonus.Draw(screenWidth, screenHeight, 0.5, 0.5, 1);
      break;
    case platformBig:
      bonus.Draw(screenWidth, screenHeight, 1, 0.5, 0.5);
      break;
    case extraLive:
      bonus.Draw(screenWidth, screenHeight, 0.75, 0, 0);
      break;
    case superBall:
      bonus.Draw(screenWidth, screenHeight, 1, 0.5, 0);
      break;
    case multiball:
      bonus.Draw(screenWidth, screenHeight, 0.5, 0.5, 0.5);
      break;
    case points:
      bonus.Draw(screenWidth, screenHeight, 0.65, 0.65, 0);
      break;
    case speedUpBall:
      bonus.Draw(screenWidth, screenHeight, 0.5, 0.2, 0.2);
      break;
    case slowDownBall:
      bonus.Draw(screenWidth, screenHeight, 0.2, 0.2, 0.5);
      break;
    }

  // draw platform
  plat.Draw(screenWidth, screenHeight, 1, 1, 1);

  // draw blocks
  int hitsRemain;
  for (auto &block : blocks) {
    hitsRemain = block.HitsRemains();
    switch (hitsRemain) {
    case 1:
      block.Draw(screenWidth, screenHeight, 1, 0, 0);
      break;
    case 2:
      block.Draw(screenWidth, screenHeight, 1, 1, 0);
      break;
    case 3:
      block.Draw(screenWidth, screenHeight, 0, 1, 0);
      break;
    }
  }
}

void screen::CheckCollisionBallsWithBlocks() {
  // check ball collision with blocks
  int collisionCheckRes, vertCol, horizCol, wasUpper, wasLefter;
  double vertSquareCol, horizSquareCol, square;

  int needToDelete;
  std::list<block>::iterator blocksIter = blocks.begin(), blockToDelete;
  std::list<std::list<block>::iterator> blocksToDelete;

  for (auto &ball : balls) {
    blocksIter = blocks.begin();

    needToDelete = 0;
    vertCol = horizCol = vertSquareCol = horizSquareCol = 0;
    while (blocksIter != blocks.end()) {
      collisionCheckRes = ball.CheckCollision((*blocksIter), square);

      if (collisionCheckRes == 1) {
        horizCol++;
        horizSquareCol += square;
      }
      else if (collisionCheckRes == 2) {
        vertCol++;
        vertSquareCol += square;
      }
      else if (collisionCheckRes == 3) {
        horizCol++;
        vertCol++;
        vertSquareCol += square;
        horizSquareCol += square;
      }

      if (collisionCheckRes != 0) {
        if (ball.IfHaveSuperBallEffect() != 1)
          (*blocksIter).UpdateHits();
        else
          (*blocksIter).DestroyBlock();
        if ((*blocksIter).HitsRemains() == 0) {
          blocksToDelete.push_back(blocksIter);
          needToDelete = 1;
          totalPoints += 100 * (*blocksIter).GetMaxHits();
        }
      }

      (*blocksIter++);
    }

    if (ball.IfHaveSuperBallEffect() != 1) {
      if (vertSquareCol < horizSquareCol) {
        ball.MakeBallSwitchHorizDirection();
      }
      else if (vertSquareCol > horizSquareCol)
        ball.MakeBallSwitchVertDirection();
      else if (vertSquareCol != 0) {
        ball.MakeBallSwitchHorizDirection();
        ball.MakeBallSwitchVertDirection();
      }
    }
    if (needToDelete == 1) {
      while (!blocksToDelete.empty()) {
        blockToDelete = blocksToDelete.front();
        blocksToDelete.pop_front();
        destroyedBlocks++;

        // create bonus if block had it
        if ((*blockToDelete).hasBonus())
          bonuses.push_back(bonus(20, 20, (*blockToDelete).GetXPosition() + (*blockToDelete).GetWidth() / 2 - 10, (*blockToDelete).GetYPosition() + (*blockToDelete).GetHeight() / 2 - 10, (*blockToDelete).hasBonus()));

        blocks.erase(blockToDelete);
      }
    }
  }
}

void screen::CheckCollisionBallsWithScreen() {
  std::list<ball>::iterator ballsIter = balls.begin();
  int collisionCheckRes;

  std::list<std::list<ball>::iterator> ballsToDelete;
  while (ballsIter != balls.end()) {
    collisionCheckRes = (*ballsIter).CheckScreenCollision(0, infoHeight, gameZoneWidth, gameZoneHeight);
    if (collisionCheckRes == 1) {
      (*ballsIter).MakeBallSwitchVertDirection();
      (*ballsIter).SetXPosition(screenWidth - 20 - 1);
    }
    else if (collisionCheckRes == 2) {
      (*ballsIter).MakeBallSwitchVertDirection();
      (*ballsIter).SetXPosition(1);
    }
    else if (collisionCheckRes == 4) {
      (*ballsIter).MakeBallSwitchHorizDirection();
      (*ballsIter).SetYPosition(infoHeight + 1);
    }

    if ((*ballsIter).GetYPosition() >= screenHeight)
      ballsToDelete.push_back(ballsIter);

    (*ballsIter++);
  }
  for (auto& iter : ballsToDelete)
    balls.erase(iter);
}

void screen::CheckCollisionBallsWithPlatform() {
  int collisionCheckRes;
  double square;

  for (auto &ball : balls) {
    collisionCheckRes = ball.CheckCollision(plat, square);
    if (collisionCheckRes != 0) {
      ball.SetYPosition(screenHeight - 42);
      ball.MakeBallSwitchHorizDirection();
    }
  }
}

void screen::CheckCollisionBonusesWithPlatform() {
  std::list<bonus>::iterator bonusesIter = bonuses.begin();
  std::list<ball>::iterator ballsIter;
  int collisionCheckRes;
  double square;

  std::list<std::list<bonus>::iterator> bonusesToDelete;
  while (bonusesIter != bonuses.end()) {
    collisionCheckRes = (*bonusesIter).CheckCollision(plat, square);

    if (collisionCheckRes == 1) {
      switch ((*bonusesIter).GetType()) {
      case platformTiny:
        plat.MakeResizeEffect(-1);
        break;
      case platformBig:
        plat.MakeResizeEffect(1);
        break;
      case extraLive:
        lifes++;
        break;
      case superBall:
        ballsIter = balls.begin();
        while (ballsIter != balls.end())
          (*ballsIter++).MakeSuperBallEffect();
        break;
      case multiball:
        balls.push_back(ball(20, 20, (*bonusesIter).GetXPosition() - 11, screenHeight - 42, ballSpeed * (1 + 0.5 * (double)lvlNo / maxLevels), -1));
        balls.push_back(ball(20, 20, (*bonusesIter).GetXPosition() + 11, screenHeight - 42, ballSpeed * (1 + 0.5 * (double)lvlNo / maxLevels), 1));
        break;
      case points:
        totalPoints += 1000;
        break;
      case speedUpBall:
        ballsIter = balls.begin();
        while (ballsIter != balls.end())
          (*ballsIter++).MakeSpeedEffect(1);
        break;
      case slowDownBall:
        ballsIter = balls.begin();
        while (ballsIter != balls.end())
          (*ballsIter++).MakeSpeedEffect(-1);
        break;
      }
      bonusesToDelete.push_back(bonusesIter);
    }

    if ((*bonusesIter).GetYPosition() > screenHeight)
      bonusesToDelete.push_back(bonusesIter);

    (*bonusesIter++);
  }

  for (auto& iter : bonusesToDelete)
    bonuses.erase(iter);
}

void screen::UpdateObjects() {
  // update ball possition and effects
  for (auto& ball : balls) {
    ball.UpdatePosition();
    ball.UpdateEffects();
  }

  // update bonuses possition
  for (auto &bonus : bonuses)
    bonus.UpdatePosition();

  // update platform effects
  plat.UpdateEffects();

  CheckCollisionBallsWithBlocks();
  CheckCollisionBallsWithPlatform();
  CheckCollisionBallsWithScreen();
  CheckCollisionBonusesWithPlatform();

  // check game conditions
  if (lifes == 0) {
    std::cout << "You lose..." << std::endl;
    exit(0);
  }
  if (balls.empty()) {
    MakeStartPosition();
    lifes--;
  }

  if (destroyedBlocks == totalBlocks && lvlNo != maxLevels - 1) {
    lvlNo++;
    InitGame(lvlNo);
    lifes++;
  }
  else if (destroyedBlocks == totalBlocks && lvlNo == maxLevels - 1) {
    std::cout << "CONGRATULATIONS!" << std::endl;
    exit(0);
  }
}

void screen::MovePlatform(int direction) {
  if (direction == 1)
    plat.MoveLeft();
  else if (direction == -1)
    plat.MoveRight();
}
