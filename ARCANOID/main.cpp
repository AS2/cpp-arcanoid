#include "glut.h"
#include "game.h"
#include <windows.h>

#define width 1280
#define height 720

screen screenGame(width, height);
int isPause = 0;

void DisplayFunc() {
  glClearColor(0.0, 0.0, 0.0, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  if (!isPause)
    screenGame.UpdateObjects();
  screenGame.DrawObjects();
  
  glFinish();
  glutSwapBuffers();

  Sleep(1);
  glutPostRedisplay();
}

void KeyboardFunc(unsigned char key, int x, int y) {
  if (key == 27)
    exit(0);
  else if (key == 'p')
    isPause = !isPause;
}

void SpecKeyboardFunc(int key, int x, int y) {
  switch (key) {
  case GLUT_KEY_LEFT:
    screenGame.MovePlatform(-1);
    break;
  case GLUT_KEY_RIGHT:
    screenGame.MovePlatform(1);
    break;
  }
}

int main(int argc, char* argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB);

  glutInitWindowPosition(100, 100);
  glutInitWindowSize(width, height);
  glutCreateWindow("AR-CA-NO-ID!");

  screenGame.InitGame(0);

  glutDisplayFunc(DisplayFunc);
  glutKeyboardFunc(KeyboardFunc);
  glutSpecialFunc(SpecKeyboardFunc);

  glutMainLoop();

  return 0;
}
