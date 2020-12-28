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
    paintGameScreen();
    break;
  // end game state
  case end_game:
    paintEndGameScreen();
    break;
  }
}

void CSnake::paintLevelBar() {
  gotoyx(geom.topleft.y - 1, geom.topleft.x);
  printl("|LEVEL: %d| |STATE: %d| |SIZE: %d|", level, state, snake.size());
}

void CSnake::paintGameScreen() {
  for (int y = geom.topleft.y + 1; y < geom.topleft.y + geom.size.y - 1; y++) {
    for (int x = geom.topleft.x + 1; x < geom.topleft.x + geom.size.x - 1;
         x++) {
      auto begin = snake.cbegin();
      gotoyx(geom.topleft.y - 2, geom.topleft.x);
      printl("X Y: %d %d | HEAD X Y: %d %d", x, y, begin->x, begin->y);
      gotoyx(y, x);
      char c = ' ';
      if(foodPos.x == x && foodPos.y == y)
          c= 'o';
      for (auto it = begin; it != snake.cend(); it++) {
        if (it->x == x && it->y == y)
          c = it == begin ? '*' : '+';
      }
      printl("%c", c);
    }
  }
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
    gotoyx(geom.topleft.y + geom.size.y / 2 - 1,
           geom.topleft.x + geom.size.x / 2 - 7);
    printl("GAME RESTARTED");
    state = home_screen;
    return true;
  }
  if (player_input.empty()) {
    if ((key == KEY_UP || lower_key == 'w') && direction != down &&
        direction != up) {
      player_input.push(up);
      return true;
    }
    if ((key == KEY_DOWN || lower_key == 's') && direction != up &&
        direction != down) {
      player_input.push(down);
      return true;
    }
    if ((key == KEY_LEFT || lower_key == 'a') && direction != right &&
        direction != left) {
      player_input.push(left);
      return true;
    }
    if ((key == KEY_RIGHT || lower_key == 'd') && direction != left &&
        direction != right) {
      player_input.push(right);
      return true;
    }
  } else {
    // get last move
    const _direction &last_move = player_input.back();
    if ((key == KEY_UP || lower_key == 'w') && last_move != down &&
        last_move != up) {
      player_input.push(up);
      return true;
    }
    if ((key == KEY_DOWN || lower_key == 's') && last_move != up &&
        last_move != down) {
      player_input.push(down);
      return true;
    }
    if ((key == KEY_LEFT || lower_key == 'a') && last_move != right &&
        last_move != left) {
      player_input.push(left);
      return true;
    }
    if ((key == KEY_RIGHT || lower_key == 'd') && last_move != left &&
        last_move != right) {
      player_input.push(right);
      return true;
    }
  }

  return ticGame();
}

bool CSnake::handleEndGameEvent(int key) {
  const int lower_key = tolower(key);
  old_state = end_game;
  if (lower_key == 'p' || lower_key == 'h') {
    state = home_screen;
    return true;
  }
  if (lower_key == 'r') {
    return startGame();
  }

  return handleWindowMoveEvent(key);
}

bool CSnake::handleHomeScreenEvent(int key) {
  const int lower_key = tolower(key);
  if (lower_key == 's') {
    return startGame();
  }

  return handleWindowMoveEvent(key);
}

bool CSnake::handlePauseScreenEvent(int key) {
  const int lower_key = tolower(key);
  old_state = game_paused;
  if (lower_key == 'p' || lower_key == 'h') {
    return resumeGame();
  }

  if (lower_key == 'r') {
    gotoyx(geom.topleft.y + geom.size.y / 2 - 1,
           geom.topleft.x + geom.size.x / 2 - 7);
    printl("GAME RESTARTED");
    return startGame();
  }

  return handleWindowMoveEvent(key);
}

bool CSnake::handleWindowMoveEvent(int key) {
  if (CFramedWindow::handleEvent(key)) {
    CPoint delta;
    switch (key) {
    case KEY_UP:
      delta = CPoint(0, -1);
      break;
    case KEY_DOWN:
      delta = CPoint(0, 1);
      break;
    case KEY_RIGHT:
      delta = CPoint(1, 0);
      break;
    case KEY_LEFT:
      delta = CPoint(-1, 0);
      break;
    };
    for (auto it = snake.begin(); it != snake.end(); it++) {
      *it += delta;
    }

    return true;
  }

  return false;
}

/*
 * Game logic
 */

bool CSnake::startGame() {
  if (!snake.empty())
    snake.clear();
  for(size_t i = 2; i < 5; i++) {
	 snake.push_front(CPoint(geom.topleft.x + i, geom.topleft.y + 2)); 
  }
  direction = right;
  tic_timer = std::time(nullptr);
  generateFood();
  state = in_game;
  return true;
}

bool CSnake::ticGame() {
  if(std::time(nullptr) - tic_timer < 1)
      return false;
  
  auto head = snake.begin();
  CPoint new_pos = *head;
  //if we have player input get next move and pop
  if (!player_input.empty()) {
    direction = player_input.front();
    player_input.pop();
  }
  switch (direction) {
  case up:
    new_pos += CPoint(0, -1);
    break;
  case down:
    new_pos += CPoint(0, 1);
    break;
  case right:
    new_pos += CPoint(1, 0);
    break;
  case left:
    new_pos += CPoint(-1, 0);
    break;
  };
  
  //we have next pos of head here
  if(new_pos.x == foodPos.x && new_pos.y == foodPos.y) {
	  snake.push_front(new_pos);
	  generateFood();
  } else {
	  std::swap(*head, new_pos);
  }
  
  // check end game
  for (auto it = std::next(head); it != snake.cend(); it++) {
    if (head->x == it->x && head->y == it->y) {
      state = end_game;
      return true;
    }
  }
  
  //move tail
  for (auto it = std::next(head); it != snake.end(); it++) {
    CPoint temp = *it;
    *it = new_pos;
    new_pos = temp;
  }

  tic_timer = std::time(nullptr);
  return true;
}

bool CSnake::resumeGame() {
  state = in_game;
  return true;
}

void CSnake::generateFood() {
	CPoint new_pos(geom.topleft.x + 4, geom.topleft.y + 6);
gen:
	/*new_pos.x = geom.topleft.x + geom.size.x / 2;
	new_pos.y = geom.topleft.y + geom.size.y / 2;*/
	for(auto it = snake.cbegin(); it != snake.cend(); it++) {
		if(it->x == new_pos.x && it->y == new_pos.y) {
		    new_pos.x = geom.topleft.x + geom.size.x / 2;
			new_pos.y = geom.topleft.y + geom.size.y / 2;
		}
	}
	foodPos = new_pos;
}
