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
    paintEndGameScreen();
    break;
  }
}

void CSnake::paintLevelBar() {
  gotoyx(geom.topleft.y - 1, geom.topleft.x);
  printl("|LEVEL: %d| |STATE: %d|", level, state);
}

void CSnake::paintHelpScreen() {
  gotoyx(geom.topleft.y + 4, geom.topleft.x + 3);
  printl("h - toggle help screen (this window)");
  gotoyx(geom.topleft.y + 5, geom.topleft.x + 3);
  printl("p - toggle pause/play mode");
  gotoyx(geom.topleft.y + 6, geom.topleft.x + 3);
  printl("r - restart game");
  gotoyx(geom.topleft.y + 7, geom.topleft.x + 3);
  printl("arrows/WASD - move snake (in play mode)");
  gotoyx(geom.topleft.y + 8, geom.topleft.x + 3);
  printl("arrows - move window (in pause mode)");
}

void CSnake::paintHomeScreen() {
  paintHelpScreen();
  gotoyx(geom.topleft.y + geom.size.y / 2,
         geom.topleft.x + geom.size.x / 2 - 11);
  printl("PRESS 'S' TO START GAME");
}

void CSnake::paintPauseScreen() {
  paintHelpScreen();
  gotoyx(geom.topleft.y + geom.size.y / 2,
         geom.topleft.x + geom.size.x / 2 - 5);
  printl("GAME PAUSED");
}

void CSnake::paintEndGameScreen() {
  gotoyx(geom.topleft.y + geom.size.y / 2 - 2,
         geom.topleft.x + geom.size.x / 2 - 4);
  printl("GAME OVER");
  const std::string temp = std::to_string(level);
  const int al = static_cast<int>(floor((7 + temp.length()) / 2));
  gotoyx(geom.topleft.y + geom.size.y / 2 - 1,
         geom.topleft.x + geom.size.x / 2 - al);
  printl("SCORE: %d", level);
  gotoyx(geom.topleft.y + geom.size.y / 2,
         geom.topleft.x + geom.size.x / 2 - 13);
  printl("PRESS 'H' TO GO HOME SCREEN");
  gotoyx(geom.topleft.y + geom.size.y / 2 + 1,
         geom.topleft.x + geom.size.x / 2 - 14);
  printl("OR PRESS 'R' TO RESTART GAME");
}

/*
 * Events handling
 */

bool CSnake::handleEvent(int key) {
  switch (state) {
  case home_screen:
    return handleHomeScreenEvent(key);
  case game_paused:
    return handlePauseScreenEvent(key);
  case in_game:
    return handleGameEvent(key);
  case end_game:
    return handleEndGameEvent(key);
  }
  return false;
}

bool CSnake::handleGameEvent(int key) {
  const int lower_key = tolower(key);
  old_state = in_game;
  if (lower_key == 'p' || lower_key == 'h') {
    state = game_paused;
    return true;
  }
  if (lower_key == 'r') {
    state = home_screen;
    return true;
  }
  if ((key == KEY_UP || lower_key == 'w') && direction != down)
    return true;
  if ((key == KEY_DOWN || lower_key == 's') && direction != up)
    return true;
  if ((key == KEY_LEFT || lower_key == 'a') && direction != right)
    return true;
  if ((key == KEY_RIGHT || lower_key == 'd') && direction != left)
    return true;

  return false;
}

bool CSnake::handleEndGameEvent(int key) {
  const int lower_key = tolower(key);
  old_state = end_game;
  if (lower_key == 'p' || lower_key == 'h') {
    state = home_screen;
    return true;
  }
  if (lower_key == 'r') {
    // startGame();
    return true;
  }

  return CFramedWindow::handleEvent(key);
}

bool CSnake::handleHomeScreenEvent(int key) {
  if (key == 'S' || key == 's') {
    // startGame();
    return true;
  }

  return CFramedWindow::handleEvent(key);
}

bool CSnake::handlePauseScreenEvent(int key) {
  const int lower_key = tolower(key);
  old_state = game_paused;
  if (lower_key == 'p' || lower_key == 'h') {
    // resumeGame();
    return true;
  }

  if (lower_key == 'r') {
    gotoyx(geom.topleft.y + geom.size.y / 2 - 1,
           geom.topleft.x + geom.size.x / 2 - 7);
    printl("GAME RESTARTED");
    // startGame();
    return true;
  }

  return CFramedWindow::handleEvent(key);
}
