#ifndef __SNAKE_H__
#define __SNAKE_H__

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
	
	CPoint foodPos;
	size_t score = 0;
	size_t level = 0;
	_states oldState = home_screen;
	_states state = home_screen;
	
	//zarzadzanie stanem gry
	bool handleGameEvent(int key);
	bool handleHomeScreenEvent(int key);
	
	void paintLevelBar();
	void paintHomeScreen();
	void paintHelpScreen();
	void paintPauseScreen();
	void paintEndGameScreen();
public:
  CSnake(CRect r, char _c = ' ') : CFramedWindow(r, _c) {};
  void paint();
  bool handleEvent(int key);
};

#endif
