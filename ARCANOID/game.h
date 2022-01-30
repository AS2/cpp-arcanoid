#ifndef __game_h__
#define __game_h__

#include <list>
#include <string>
#include "gameObjects.h"

class screen {
private:
  int screenWidth, screenHeight, gameZoneWidth, gameZoneHeight, infoHeight;

  int ballSpeed, platformWidth, lvlNo, maxLevels;

  int lifes, totalPoints;
  std::list<ball> balls;
  platform plat;
  std::list<bonus> bonuses;

  int totalBlocks, destroyedBlocks, blocksRows, blocksInRow;
  std::list<block> blocks;

  void BuildLevel(int levelNumber);
  void MakeStartPosition();

  // draw text functions
  void BeginText();
  void EndText();
  void DrawInfo();

  // check collisions
  void CheckCollisionBallsWithBlocks();
  void CheckCollisionBallsWithScreen();
  void CheckCollisionBallsWithPlatform();
  void CheckCollisionBonusesWithPlatform();
public:
  screen(int width, int height);

  void InitGame(int CurrentLevel);

  void DrawObjects();
  void UpdateObjects();
  
  void MovePlatform(int direction);
};

#endif  // __game_h__