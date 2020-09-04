#include "stdafx.h"
#include "cRandom.h"


void cRandom::RandInt(int min, int max)
{
	uniform_int_distribution< > distribution(min, max); // uniform_int_distribution �ּ� �ִ밪�� ������ ����

	distribution(m_engine);
}

void cRandom::RandDouble(double min, double max)
{
	uniform_real< > distribution(min, max); // uniform_int_distribution �ּ� �ִ밪�� ������ �Ǽ� 8 ����Ʈ ������

	distribution(m_engine);	
}

cRandom::cRandom()
{
	m_engine = mt19937(static_cast<UINT>(time(NULL)));
}


cRandom::~cRandom()
{
}
