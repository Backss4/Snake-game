#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <cctype>
#include <string>
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
	_states oldState = home_screen;
	_states state = home_screen;
	_direction direction = stop;
	CPoint foodPos;
	
	//screens
	void paintLevelBar();
	void paintHomeScreen();
	void paintHelpScreen();
	void paintPauseScreen();
	void paintEndGameScreen();
	
	//handling events
	bool handleGameEvent(int key);
	bool handleHomeScreenEvent(int key);
	bool handlePauseScreenEvent(int key);
public:
  CSnake(CRect r, char _c = ' ') : CFramedWindow(r, _c) {};
  void paint();
  bool handleEvent(int key);
};

#endif
