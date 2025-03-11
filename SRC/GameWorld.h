#ifndef __GAMEWORLD_H__
#define __GAMEWORLD_H__

#include "GameUtil.h"
#include "IGameWorldListener.h"

class GameObject;

// Define a type of list to hold game objects
typedef list< shared_ptr< GameObject > > GameObjectList;
typedef list< weak_ptr< GameObject > > WeakGameObjectList;

// Define a type of map to hold lists of collisions
typedef map< shared_ptr<GameObject>, GameObjectList > CollisionMap;

class GameWorld
{
public:
	GameWorld(void);
	~GameWorld(void);

	void Update(int t);
	void Render(void);

	void AddObject( shared_ptr<GameObject> ptr );
	void RemoveObject( shared_ptr<GameObject> ptr );
	void RemoveObject( GameObject* ptr );
	// shared_ptr<GameObject> GetGameObject( string name );

	void FlagForRemoval( GameObject* ptr );
	void FlagForRemoval( weak_ptr<GameObject> ptr );

	GameObjectList GetCollisions( shared_ptr<GameObject> ptr );
	GameObjectList GetCollisions( GameObject* optr );

	void AddListener( IGameWorldListener* lptr) { mListeners.push_back(lptr); }
	void RemoveListener( IGameWorldListener* lptr) { mListeners.remove(lptr); }

	void FireWorldUpdated();
	void FireObjectAdded( shared_ptr<GameObject> ptr );
	void FireObjectRemoved( shared_ptr<GameObject> ptr );

	void SetWidth(int w) { mWidth = w; }
	int GetWidth() { return mWidth; }

	void SetHeight(int h) { mHeight = h; }
	int GetHeight() { return mHeight; }

	void WrapXY(float &x, float &y);

protected:
	void UpdateObjects(int t);
	void UpdateCollisions(int t);

	// Create a map of named game objects
	GameObjectList mGameObjects;
	// Create a map of colliding game objects
	CollisionMap mCollisions;

	// Objects to remove when the update has completed
	WeakGameObjectList mGameObjectsToRemove;

	// Define a type of list to hold game world listeners
	typedef list< IGameWorldListener* > GameWorldListenerList;
	// Create a list of game world listeners
	GameWorldListenerList mListeners;

	// The width of the world
	int mWidth;
	// The height of the world
	int mHeight;
};

#endif