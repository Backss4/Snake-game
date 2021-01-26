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
  printl("|LEVEL: %d|", level);
}

void CSnake::paintGameScreen() {
  for (int y = geom.topleft.y + 1; y < geom.topleft.y + geom.size.y - 1; y++) {
    for (int x = geom.topleft.x + 1; x < geom.topleft.x + geom.size.x - 1;
         x++) {
      auto begin = snake.cbegin();
      gotoyx(y, x);
      char c = ' ';
      if (foodPos.x == x && foodPos.y == y)
        c = 'o';
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

  const _direction dir = player_input.empty() ? direction : player_input.back();
  if ((key == KEY_UP || lower_key == 'w') && dir != down) {
    player_input.push(up);
  } else if ((key == KEY_DOWN || lower_key == 's') && dir != up) {
    player_input.push(down);
  } else if ((key == KEY_LEFT || lower_key == 'a') && dir != right) {
    player_input.push(left);
  } else if ((key == KEY_RIGHT || lower_key == 'd') && dir != left) {
    player_input.push(right);
  }

  return ticGame();
}

bool CSnake::handleEndGameEvent(int key) {
  const int lower_key = tolower(key);
  if (lower_key == 'p' || lower_key == 'h') {
    state = home_screen;
    return true;
  }
  if (lower_key == 'r') {
    return startGame();
  }

  return CFramedWindow::handleEvent(key);
}

bool CSnake::handleHomeScreenEvent(int key) {
  const int lower_key = tolower(key);
  if (lower_key == 's') {
    return startGame();
  }

  return CFramedWindow::handleEvent(key);
}

bool CSnake::handlePauseScreenEvent(int key) {
  const int lower_key = tolower(key);
  if (lower_key == 'p' || lower_key == 'h') {
    return resumeGame();
  }

  if (lower_key == 'r') {
    gotoyx(geom.topleft.y + geom.size.y / 2 - 1,
           geom.topleft.x + geom.size.x / 2 - 7);
    printl("GAME RESTARTED");
    return startGame();
  }

  return CFramedWindow::handleEvent(key);
}

void CSnake::move(const CPoint &delta) {
  CFramedWindow::move(delta);
  for (auto it = snake.begin(); it != snake.end(); it++) {
    *it += delta;
  }
  foodPos += delta;
}

/*
 * Game logic
 */

bool CSnake::startGame() {
  std::queue<_direction>().swap(player_input);
  if (!snake.empty())
    snake.clear();
  for (size_t i = 2; i < 5; i++) {
    snake.push_front(CPoint(geom.topleft.x + i, geom.topleft.y + 2));
  }
  direction = right;
  generateFood();
  level = 1;
  foodForNextLevel = 3;
  foodCount = 0;
  tic_timer = std::chrono::high_resolution_clock::now();
  state = in_game;
  return true;
}

bool CSnake::ticGame() {
  auto t2 = std::chrono::high_resolution_clock::now();
  auto elapsed_time =
      std::chrono::duration_cast<std::chrono::milliseconds>(t2 - tic_timer)
          .count();
  auto x = std::chrono::milliseconds(310).count();
  auto y = std::chrono::milliseconds(
               multiplier * level >= 300 ? 300 : multiplier * level)
               .count();
  bool boost =
      !player_input.empty() && player_input.front() == direction ? true : false;
  if (elapsed_time < x - y && !boost)
    return false;

  auto head = snake.begin();
  CPoint new_pos = *head;
  // if we have player input get next move and pop
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
  }

  // check border
  if (geom.topleft.x + 1 > new_pos.x ||
      geom.topleft.x + geom.size.x - 2 < new_pos.x ||
      geom.topleft.y + 1 > new_pos.y ||
      geom.topleft.y + geom.size.y - 2 < new_pos.y) {
    switch (direction) {
    case up:
      new_pos.y = geom.topleft.y + geom.size.y - 2;
      break;
    case down:
      new_pos.y = geom.topleft.y + 1;
      break;
    case right:
      new_pos.x = geom.topleft.x + 1;
      break;
    case left:
      new_pos.x = geom.topleft.x + geom.size.x - 2;
      break;
    }
  }

  // we have next pos of head here
  if (new_pos == foodPos) {
    snake.push_front(new_pos);
    generateFood();
    if (++foodCount == foodForNextLevel) {
      level++;
      foodCount = 0;
      if (level % 2 == 0)
        foodForNextLevel++;
    }
    return ticGame();
  } else {
    std::swap(*head, new_pos);
  }

  // move tail and check colision with tail
  for (auto it = std::next(head); it != snake.end(); it++) {
    std::swap(*it, new_pos);
    if (*head == *it) {
      state = end_game;
      return true;
    }
  }

  tic_timer = std::chrono::high_resolution_clock::now();
  return true;
}

bool CSnake::resumeGame() {
  state = in_game;
  return true;
}

void CSnake::generateFood() {
  CPoint new_pos;
gen:
  new_pos.x = std::uniform_int_distribution<int>{
      geom.topleft.x + 1, geom.topleft.x + geom.size.x - 2}(generator);
  new_pos.y = std::uniform_int_distribution<int>{
      geom.topleft.y + 1, geom.topleft.y + geom.size.y - 2}(generator);
  for (auto it = snake.cbegin(); it != snake.cend(); it++) {
    if (new_pos == *it)
      goto gen;
  }
  foodPos = new_pos;
}
