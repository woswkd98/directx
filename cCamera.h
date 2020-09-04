#pragma once


class cCamera :
	public cMonoBehavior // 카메라도 컴포넌트 형식으로 관리
{
public:
	
private:

	XMFLOAT4X4 m_matView; // 뷰
	XMFLOAT4X4 m_matProjection; // 프로젝션
	XMFLOAT4X4 m_matPerspective; // 프로젝션 perspective
	XMFLOAT4X4 m_matOthographic; //  프로젝션 Othographic
	XMFLOAT4X4 m_matVP; // 뷰 프로젝션

public:

	float m_fov = 60; //화각
	float m_camNear = 0.3f; // z의 가까운길이
	float m_camFar = 1000; // z의 먼길이

	float m_sizeX = 0;
	float m_sizeY = 0;

	float m_startX = 0;
	float m_startY = 0;
	

	float gridNum = 0;

public:

	void SetUpCamSize(float sizeX , float sizeY , float startX , float startY )
	{
		m_sizeX = sizeX;
		m_sizeY = sizeY;

		m_startX = startX;
		m_startY = startY;
	}

	void Awake() override;
	void Update(float deltaSce) override;

	void SetUpUICam(float depth, float sizeX, float sizeY)
	{
		m_sizeX = sizeX;
		m_sizeY = sizeY;
	
		XMStoreFloat4x4(
			&m_matOthographic,
			XMMatrixOrthographicLH(   // 사각형 프로젝션을 만드는 클래스
				m_sizeX ,
				m_sizeY ,
				0.1,
				depth
			));

		m_matProjection = m_matOthographic;
	}

	XMFLOAT3 GetEyePos() { return this->GetGameObject()->GetTransform()->GetPositionRef(); }
	XMFLOAT4X4 GetViewProjection() { return m_matVP; }
	XMFLOAT4X4 GetViewMatrix() { return m_matView; }
	XMFLOAT4X4 GetProjection() { return m_matProjection; }
	cCamera() {
	
	}
	~cCamera();
};

