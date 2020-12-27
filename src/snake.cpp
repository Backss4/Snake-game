#include "snake.h"

/*
 * Painting screens
 */ 

void CSnake::paint() {
  CFramedWindow::paint();
  paintLevelBar();
  switch (state) {
  // home screen state - first state after running program
  case home_screen:
    paintHomeScreen();
    break;
  // pause state
  case game_paused:
    paintPauseScreen();
    break;
  case in_game:
    break;
  // end game state
  case end_game:
    break;
  }
}

void CSnake::paintLevelBar() {
  gotoyx(geom.topleft.y - 1, geom.topleft.x);
  printl("|LEVEL: %d|", level + 1);
}

void CSnake::paintHelpScreen() {
  gotoyx(geom.topleft.y + 4, geom.topleft.x + 3);
  printl("h - help");
  gotoyx(geom.topleft.y + 5, geom.topleft.x + 3);
  printl("p - toggle pause/play mode");
  gotoyx(geom.topleft.y + 6, geom.topleft.x + 3);
  printl("r - restart game");
  gotoyx(geom.topleft.y + 7, geom.topleft.x + 3);
  printl("arrows/WASD - move snake (in play mode)");
  gotoyx(geom.topleft.y + 8, geom.topleft.x + 17);
  printl("arrows - move window (in pause mode)");
}

void CSnake::paintHomeScreen() { 
	paintHelpScreen(); 
	gotoyx(geom.topleft.y + geom.size.y / 2,
         geom.topleft.x + geom.size.x / 2 - 11);
    printl("PRESS 'S' TO START GAME");
}

void CSnake::paintPauseScreen() {
  gotoyx(geom.topleft.y + geom.size.y / 2,
         geom.topleft.x + geom.size.x / 2 - 5);
  printl("GAME PAUSED");
}

void CSnake::paintEndGameScreen() {
  gotoyx(geom.topleft.y + 1, geom.topleft.x + 1);
  printl("GAME OVER");
  gotoyx(geom.topleft.y + 2, geom.topleft.x + 1);
  printl("SCORE: ", score);
}

/*
 * Events handling
 */

bool CSnake::handleEvent(int key) {
  switch (state) {
  case home_screen:
    return handleHomeScreenEvent(key);
  case game_paused:
    return CFramedWindow::handleEvent(key);
  case in_game:
    return handleGameEvent(key);
  }
  return false;
}

bool CSnake::handleGameEvent(int key) { return false; }

bool CSnake::handleHomeScreenEvent(int key) { return false; }
