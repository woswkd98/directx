#include "stdafx.h"
#include "cObject.h"
#include "cGameObject.h"

unsigned int cObject::count = 0; // ������Ʈ Ȯ�ο� 

void cObject::ObjectsDelete()
{

}


cObject::cObject() : m_bActive(false) {

}


cObject::~cObject()
{
	

}
void cObject::SetActive(bool active)
{
	this->m_bActive = active;
}