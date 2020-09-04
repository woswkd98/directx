#include "stdafx.h"
#include "cGizmo.h"
#include "cGizmoManager.h"
extern cGizmoManager g_GizmoManager;


cGizmo::cGizmo()
{
	g_GizmoManager.InsertGizmo(this);
}


cGizmo::~cGizmo()
{
	g_GizmoManager.DeleteGizmo(this);
}
