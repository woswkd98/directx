#pragma once

class cPool : public cMonoBehavior
{
	UINT index;
	int m_size;
	vector<cGameObject*> m_data;
public:

	void Init(int size, cGameObject * data)
	{
		m_size = size;
		index = 0;
		m_data.resize(size);

		for (int i = 0; i < size; ++i)
		{
			char buffer[4];
			m_data[i] = CopyGameObject(data);
			m_data[i]->SetName(m_data[i]->GetName() + itoa(i, buffer, 4));
			m_data[i]->SetActive(false);

			this->GetGameObject()->GetTransform()->InsertChild(m_data[i]->GetTransform()); // 임의로 자식에 넣는다
		}
	}

	cGameObject * GetNewData()
	{
		 if (index > m_size) index = 0;
		 if (m_data[index]->GetActive()) return nullptr; // 에러 메세지 띄움
		 
		 return m_data[index++];
	}

	void Release()
	{
		for (int i = 0; i < m_data.size(); ++i)
		{
			DestroyGameObject(m_data[i]);
		}
		m_data.clear();
	}

	cPool() { }
	
	~cPool() { Release(); }
};

