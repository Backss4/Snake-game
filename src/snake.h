#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <cctype>
#include <string>
#include <queue>
#include <list>
#include <cmath>
#include <random>
#include <iterator>
#include <chrono>
#include "winsys.h"
#include "cpoint.h"
#include "screen.h"

class CSnake:public CFramedWindow
{	
	enum _states {
		home_screen = 0,
		game_paused,
		in_game,
		end_game,
	};
	enum _direction {
		left = 0,
		right,
		up,
		down,
	};
	
	
	const char multiplier = 10;
	size_t foodForNextLevel = 3;
	size_t foodCount = 0;
	size_t level = 1;
	_states state = home_screen;
	_direction direction = right;
	CPoint foodPos;
	
	//random
	std::mt19937 generator{std::random_device{}()};
	
	//snake nodes
	std::list<CPoint> snake;
	
	//player inputs
	std::queue<_direction> player_input;
	
	//timer for ticGame
	std::chrono::high_resolution_clock::time_point tic_timer;
	
	//screens
	void paintLevelBar();
	void paintGameScreen();
	void paintHomeScreen();
	void paintHelpScreen();
	void paintPauseScreen();
	void paintEndGameScreen();
	
	//handling events
	bool handleGameEvent(int key);
	bool handleEndGameEvent(int key);
	bool handleHomeScreenEvent(int key);
	bool handlePauseScreenEvent(int key);
	
	//game logic
	bool startGame();
	bool ticGame();
	bool resumeGame();
	void generateFood();
public:
  CSnake(CRect r, char _c = ' ') : CFramedWindow(r, _c) {};
  void paint();
  bool handleEvent(int key);
  void move (const CPoint & delta);
};

#endif
