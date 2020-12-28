#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <cctype>
#include <string>
#include <queue>
#include <list>
#include <cmath>
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
		stop = 0,
		left,
		right,
		up,
		down,
	};
	
	size_t level = 1;
	_states old_state = home_screen;
	_states state = home_screen;
	_direction direction = stop;
	CPoint foodPos;
	
	//snake nodes
	list<CPoint> snake;
	
	//player inputs
	queue<pair<CPoint, _direction>> player_input;
	
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
	bool handleWindowMoveEvent(int key);
	
	//game logic
	bool startGame();
	bool ticGame();
	bool resumeGame();
public:
  CSnake(CRect r, char _c = ' ') : CFramedWindow(r, _c) {};
  void paint();
  bool handleEvent(int key);
};

#endif
