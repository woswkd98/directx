#pragma once
#include "cSingletonBase.h"
class cSoundCompoenet;
class cSoundManager :
	public cSingletonBase <cSoundManager>
{
private:
	System* m_pSystem;			
	Sound** m_pSound;				
	Channel** m_pChannel;			

	map<string, Sound*> m_mapSounds;
public:
	void Init();

	void InsertSound(string soundName)
	{
		map<string, Sound*>::iterator it = m_mapSounds.find(soundName);

		if (it != m_mapSounds.end())
		{
			return;
		}
		
		//m_mapSounds.insert(make_pair(soundName, m_pSystem->createSound(soundName, )))



	
	}


	cSoundManager();
	~cSoundManager();
};

