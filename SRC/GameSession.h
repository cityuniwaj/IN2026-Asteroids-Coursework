#ifndef __GAMESESSION_H__
#define __GAMESESSION_H__

#include "ITimerListener.h"

class GameWorld;
class GameDisplay;
class GameWindow;

class GameSession : public ITimerListener
{
public:
	GameSession(int argc, char* argv[]);
	~GameSession(void);

	virtual void OnTimer(int value) {}

	virtual void Start(void);
	virtual void Stop(void);

protected:
	GameWorld* mGameWorld;
	GameDisplay* mGameDisplay;
	GameWindow* mGameWindow;

	void SetTimer(uint msecs, int value);
};

#endif