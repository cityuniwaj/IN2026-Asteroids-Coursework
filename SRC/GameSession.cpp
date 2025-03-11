#include "GameUtil.h"
#include "GameWindow.h"
#include "GameDisplay.h"
#include "GameWorld.h"
#include "GlutSession.h"
#include "GameSession.h"

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/** Construct new game session with given command line arguments. */
GameSession::GameSession(int argc, char *argv[])
{
	mGameWorld = new GameWorld();
	mGameDisplay = new GameDisplay(400, 400);
	mGameWindow = new GameWindow(400, 400, -1, -1, "GameWindow");
	mGameWindow->SetDisplay(mGameDisplay);
	mGameWindow->SetWorld(mGameWorld);
	// Set the window for this session
	GlutSession::GetInstance().SetWindow(mGameWindow);
}

/** Destructor. */
GameSession::~GameSession()
{
	delete mGameWindow;
	delete mGameDisplay;
	delete mGameWorld;
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Start the game. */
void GameSession::Start(void)
{
	// Enable the idle function
	GlutSession::GetInstance().EnableIdleFunction();
	// Start the idle loop to begin the game
	GlutSession::Start();
}

/** Stop the game. */
void GameSession::Stop(void)
{
	GlutSession::Stop();
}

// PROTECTED INSTANCE METHODS /////////////////////////////////////////////////

/** Protected method to set a timer. */
void GameSession::SetTimer(uint msecs, int value)
{
	GlutSession::GetInstance().SetTimer(msecs, this, value);
}
