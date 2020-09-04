#pragma once


class cCamera :
	public cMonoBehavior // ī�޶� ������Ʈ �������� ����
{
public:
	
private:

	XMFLOAT4X4 m_matView; // ��
	XMFLOAT4X4 m_matProjection; // ��������
	XMFLOAT4X4 m_matPerspective; // �������� perspective
	XMFLOAT4X4 m_matOthographic; //  �������� Othographic
	XMFLOAT4X4 m_matVP; // �� ��������

public:

	float m_fov = 60; //ȭ��
	float m_camNear = 0.3f; // z�� ��������
	float m_camFar = 1000; // z�� �ձ���

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
			XMMatrixOrthographicLH(   // �簢�� ���������� ����� Ŭ����
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

