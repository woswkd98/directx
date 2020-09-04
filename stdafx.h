// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>

extern HWND			g_hWnd;		//������ �ڵ� ( �ڽ��� ���α׷����� �����ִ� ������ ��ȣ )
extern HINSTANCE	g_hInst;	//���α׷� �ν��Ͻ� �ڵ� ( OS �� �ο��� ���α׷� ��ȣ )


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
using namespace std;

#include <vector>
#include <list>
#include <map>
#include <string>
#include <random>
#include <set>
#include <functional>
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"d3d11.lib")


//#pragma comment(lib,"directx9/Lib/x86/d3dx9.lib")
//#pragma comment(lib,"directx9/Lib/x86d3d9.lib")
#define SAFE_DELETE(p)		{ if(p){ delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARR(p)	{ if(p){ delete[] (p); (p) = NULL; } }
#define SAFE_RELEASE(p)		{ if(p){ (p)->Release(); (p) = NULL; } }
//#define DEGREE_TO_RADIAN    0.0175f

// ����
#include "fmod\inc\fmod.hpp"				//fmod.hpp �߰�
#pragma comment(lib, "fmod/lib/fmodex_vc.lib")	//lib��ũ
using namespace FMOD; // �̸����� ���
// d3dx11 ���� shaderResourceView������ ����
// ����� d3d11���� tga ���� ���� �ȵȴ� 
#include "DirectXTex\DirectXTex.h"
#pragma comment(lib,"DirectXTex.lib")
#include "x����\D3DX11.h"
#include "x����\D3DX11async.h"
#pragma comment(lib,"x����/d3dx11.lib")
#pragma comment(lib,"x����/d3dx11d.lib")

#include "cLogMgr.h"
#include <d3d11_1.h>
#include <d3d11.h>
//#include "directx9\Include\d3dx9.h"


#include <wrl.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <dxgiformat.h>
#include <directxcolors.h>

using namespace DirectX;
using namespace Microsoft::WRL;

// fbxsdk ��� �޽ø� �ε��ϱ� ���� ���
#include "fbxsdk\include\fbxsdk.h"


#define GRAVITY_ACCEL XMFLOAT3(0, -9.81f, 0)

struct CB_VS_PER_OBJECT
{
	XMMATRIX m_matWVP;
	XMMATRIX m_matWorld;

	CB_VS_PER_OBJECT() { ZeroMemory(this, sizeof(CB_VS_PER_OBJECT)); }
};

struct CB_PS_PER_OBJECT
{
	XMVECTOR m_vEyePosition;
	float m_fSpecExp;
	float m_fSpecIntensity;
	float pad[3];

	CB_PS_PER_OBJECT() { ZeroMemory(this, sizeof(CB_PS_PER_OBJECT)); }
};


struct ST_VERTEX
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 UV;
	XMFLOAT3 tangent;
	XMFLOAT3 binormal;

	ST_VERTEX()
	{
		ZeroMemory(this, sizeof(XMFLOAT3));
	}
};

#define SHAPE_BOX 0
#define SHAPE_SPHERE 1
#define SHAPE_CAPSULE 2
#define SHAPE_LINE 3
#define SHAPE_PLANE 4

#define EPSILON 0.001f // �ε��Ҽ��� �ٻ񰪰��

typedef function<void()> Event; //


#include "cTimeManager.h"
#include "cDeviceManager.h"
#include "cKeyManager.h"
#include "cGBuffer.h"

#include "CommonFunction.h"

#include "cObject.h"

#include "cGameObject.h"

#include "cLightManager.h"

#include "cMonoBehavior.h"

#include "cCollider.h"
#include "cMaterial.h"

#include "cCamera.h"
#include "cCameraManager.h"
#include "cMesh_Node.h"

#include "cPhysicsComponent.h"
#include "cScene.h"
#include "cFBXFileLoader.h"
#include "cPool.h"

#include "cRigidbodyManager.h"
#include "cSceneManager.h"
#include "cGizmoManager.h"
#include "cSoundManager.h"


// script
#include "cCubeMove.h"


