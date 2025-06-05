#pragma once
#include "transform.h"
#include "renderer.h"
#include "modelRenderer.h"


class Transform3DComponent : public Transform
{

private:

	XMFLOAT3 m_OldPosition = { 0.0f,0.0f,0.0f };

	XMMATRIX m_rot = {};

	ModelRenderer* m_Model = nullptr;

	const char* m_ModelData = "‚È‚µ";

	float m_Rot = 0.0f; //Œü‚«

public:

	Transform3DComponent() = default;
	Transform3DComponent(GameObject* obj) : Transform(obj) {}
	Transform3DComponent(GameObject* obj,const char* date) : Transform(obj) , m_ModelData(date){}

	void AddModelData(const char* date) { 
		m_ModelData = date; 
		m_Model->Load(m_ModelData);

	}
	void ChangeModelData(const char* date)
	{
		m_Model->Uninit();
		m_ModelData = date;
		m_Model->Load(m_ModelData);
	}

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	XMFLOAT3 GetOldPosition() const{ return m_OldPosition; }
	float GetRot() const{ return m_Rot; }


	void SetOldPosition(const XMFLOAT3 pos) { m_OldPosition = pos; }
	void SetRot(const float r) { m_Rot = r; }

	XMMATRIX GetRotMatrix() const
	{
		return m_rot;
	}

};

