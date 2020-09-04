#include "stdafx.h"
#include "cObject.h"
#include "cGameObject.h"

unsigned int cObject::count = 0; // 오브젝트 확인용 

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