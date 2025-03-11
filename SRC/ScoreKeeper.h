#ifndef __SCOREKEEPER_H__
#define __SCOREKEEPER_H__

#include "GameUtil.h"

#include "GameObject.h"
#include "GameObjectType.h"
#include "IScoreListener.h"
#include "IGameWorldListener.h"

class ScoreKeeper : public IGameWorldListener
{
public:
	ScoreKeeper() { mScore = 0; }
	virtual ~ScoreKeeper() {}

	void OnWorldUpdated(GameWorld* world) {}
	void OnObjectAdded(GameWorld* world, shared_ptr<GameObject> object) {}

	void OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object)
	{
		if (object->GetType() == GameObjectType("Asteroid")) {
 			mScore += 10;
			FireScoreChanged();
		}
	}

	void AddListener(shared_ptr<IScoreListener> listener)
	{
		mListeners.push_back(listener);
	}

	void FireScoreChanged()
	{
		// Send message to all listeners
		for (ScoreListenerList::iterator lit = mListeners.begin(); lit != mListeners.end(); ++lit) {
			(*lit)->OnScoreChanged(mScore);
		}
	}

private:
	int mScore;

	typedef std::list< shared_ptr<IScoreListener> > ScoreListenerList;

	ScoreListenerList mListeners;
};

#endif
