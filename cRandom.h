#pragma once

class cRandom 
{
private:
	mt19937 m_engine;
public:
	void RandInt(int min, int max);
	void RandDouble(double min, double max);

	cRandom();
	~cRandom();
};

