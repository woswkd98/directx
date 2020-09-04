#pragma once
#include "cMonoBehavior.h"
class cSoundComponent :
	public cMonoBehavior
{

private:
	float m_fVolume;
	Sound * m_pSound;
public:
	void Awake() override;
	void Update(float deltaSec) override;
	void FixedUpdate() override;
	
	cSoundComponent();
	~cSoundComponent();
};

