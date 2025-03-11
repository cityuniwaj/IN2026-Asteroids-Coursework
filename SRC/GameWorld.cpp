#include "GameUtil.h"
#include "GameObject.h"
#include "GameWorld.h"

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/** Default constructor. */
GameWorld::GameWorld(void) : mWidth(200), mHeight(200)
{
}

/** Destructor. */
GameWorld::~GameWorld(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Update the world. */
void GameWorld::Update(int t)
{
	UpdateObjects(t);
	UpdateCollisions(t);

	// Remove objects flagged for removal
	WeakGameObjectList::iterator it = mGameObjectsToRemove.begin();
	while( it != mGameObjectsToRemove.end() )
	{
		RemoveObject(it->lock());
		it = mGameObjectsToRemove.erase( it );
	}

	// Send update message to listeners
	FireWorldUpdated();
}

/** Render the world by rendering all of its objects. */
void GameWorld::Render(void)
{
	// Update the projection matrix
	glMatrixMode(GL_PROJECTION);
	// Store the current projection matrix
	glPushMatrix();
	// Initialize the projection matrix to the identity matrix
	glLoadIdentity();
	// Set orthographic projection to include the world
	glOrtho(-mWidth/2, mWidth/2, -mHeight/2, mHeight/2, -100, 100);

	// Switch to model mode ready for rendering
	glMatrixMode(GL_MODELVIEW);
	// Initialize the projection matrix to the identity matrix
	glLoadIdentity();
	// Render every object in the world
	for (GameObjectList::iterator it = mGameObjects.begin(); it != mGameObjects.end(); ++it) {
		(*it)->PreRender();
		(*it)->Render();
		(*it)->PostRender();
	}
}

/** Add a game object to the world. */
void GameWorld::AddObject(shared_ptr<GameObject> ptr)
{
	// Add game object
	mGameObjects.push_back(ptr);
	// Add game object to collision map
	mCollisions[ptr] = GameObjectList();
	// Add reference to this world
	ptr->SetWorld(this);
	// Send message to all listeners
	FireObjectAdded(ptr);
}

/** Remove a game object from the world. */
void GameWorld::RemoveObject(GameObject* ptr)
{
	RemoveObject(shared_ptr<GameObject>(ptr));
}

/** Flags an object for removal so it can be removed after all objects have been updated */
void GameWorld::FlagForRemoval(weak_ptr<GameObject> ptr)
{
	// Add it to the list of objects to remove
	mGameObjectsToRemove.push_back(ptr);
}

/** Remove a game object from the world. */
void GameWorld::RemoveObject(shared_ptr<GameObject> ptr)
{
	// Check if we the pointer has already been deleted
	if(ptr.get() == nullptr) return;
	// Remove the game object from the list
	mGameObjects.remove(ptr);
	// Remove game object from collision map
	mCollisions.erase(ptr);
	// Remove reference to this world
	ptr->SetWorld(NULL);
	// Send message to all listeners
	FireObjectRemoved(ptr);
}

/** Inform all listeners of world update. */
void GameWorld::FireWorldUpdated()
{
	// Send update message to all listeners
	for (GameWorldListenerList::iterator it = mListeners.begin(); it != mListeners.end(); ++it) {
		(*it)->OnWorldUpdated(this);
	}
}

/** Inform all listeners of object addition. */
void GameWorld::FireObjectAdded(shared_ptr<GameObject> ptr)
{
	// Send message to all listeners
	for (GameWorldListenerList::iterator it = mListeners.begin(); it != mListeners.end(); ++it) {
		(*it)->OnObjectAdded(this, ptr);
	}
}

/** Inform all listeners of object removal. */
void GameWorld::FireObjectRemoved(shared_ptr<GameObject> ptr)
{
	// Send message to all listeners
	for (GameWorldListenerList::iterator lit = mListeners.begin(); lit != mListeners.end(); ++lit) {
		(*lit)->OnObjectRemoved(this, ptr);
	}
}

/** Get all the collisions for a given object. */
GameObjectList GameWorld::GetCollisions(shared_ptr<GameObject> ptr)
{
	// Find the named game object
	CollisionMap::iterator it = mCollisions.find(ptr);
	// If object does not exist return empty list
	if (it == mCollisions.end()) {
		mCollisions[ptr] = GameObjectList();
		CollisionMap::iterator it2 = mCollisions.find(ptr);
		return it2->second;
	}
	// Otherwise return list from collision map
	return it->second;
}

/** Get all the collisions for a given object. */
GameObjectList GameWorld::GetCollisions(GameObject* optr)
{
	return GetCollisions(shared_ptr<GameObject>(optr));
}

/** Update all objects. */
void GameWorld::UpdateObjects(int t)
{
	// Update every object in the world
	GameObjectList::iterator it = mGameObjects.begin();
	for(GameObjectList::iterator it = mGameObjects.begin(); it != mGameObjects.end(); ++it) 
	{
		(*it)->Update(t);
	}
}

/** Update all collisions. */
void GameWorld::UpdateCollisions(int t)
{
	CollisionMap::iterator it1;
	CollisionMap::iterator it2;

	// Clear collisions
	for (it1 = mCollisions.begin(); it1 != mCollisions.end(); ++it1) {
		GameObjectList &collisions = it1->second;
		collisions.clear();
	}

	// Update collisions
	for (it1 = mCollisions.begin(); it1 != mCollisions.end(); ++it1) {
		shared_ptr<GameObject> object1 = it1->first;
		GameObjectList& collisions1 = it1->second;
		for (it2 = mCollisions.begin(); it2 != mCollisions.end(); ++it2) {
			shared_ptr<GameObject> object2 = it2->first;
			GameObjectList& collisions2 = it2->second;
			if (object2 != object1) {
				if (object1->CollisionTest(object2)) {
					collisions1.push_back(object2);
					collisions2.push_back(object1);
				}
			}
		}
	}


	// Call objects to handle collisions
	it1 = mCollisions.begin();
	while (it1 != mCollisions.end()) {
		// We have to be careful and make a copy of the iterator
		// before calling OnCollision() in case the object removes itself
		it2 = it1++;
		shared_ptr<GameObject> object = it2->first;
		GameObjectList collisions = it2->second;
		if (!collisions.empty()) object->OnCollision(collisions);
	}
}

/** Utility method to wrap positions around the world's edges. */
void GameWorld::WrapXY(GLfloat &x, GLfloat &y)
{
	// Wrap x and y coords that are out of the bounds of the world
	while (x >  mWidth/2)  x -= mWidth; 
	while (y >  mHeight/2) y -= mHeight; 
	while (x < -mWidth/2)  x += mWidth; 
	while (y < -mHeight/2) y += mHeight; 
}
