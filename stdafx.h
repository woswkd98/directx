// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>

extern HWND			g_hWnd;		//윈도우 핸들 ( 자신의 프로그램에서 돌고있는 윈도우 번호 )
extern HINSTANCE	g_hInst;	//프로그램 인스턴스 핸들 ( OS 가 부여한 프로그램 번호 )


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
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

// 사운드
#include "fmod\inc\fmod.hpp"				//fmod.hpp 추가
#pragma comment(lib, "fmod/lib/fmodex_vc.lib")	//lib링크
using namespace FMOD; // 이름공간 사용
// d3dx11 파일 shaderResourceView때문에 쓴다
// 참고로 d3d11에선 tga 파일 지원 안된다 
#include "DirectXTex\DirectXTex.h"
#pragma comment(lib,"DirectXTex.lib")
#include "x파일\D3DX11.h"
#include "x파일\D3DX11async.h"
#pragma comment(lib,"x파일/d3dx11.lib")
#pragma comment(lib,"x파일/d3dx11d.lib")

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

// fbxsdk 헤더 메시를 로딩하기 위한 헤더
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

#define EPSILON 0.001f // 부동소수점 근삿값계산

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


