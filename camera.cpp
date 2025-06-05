#include "main.h"
#include "camera.h"
#include "gameScene.h"
#include "scene.h"
#include "transform3DComponent.h"
#include "boxColiderComponent.h"
#include "player.h"

void Camera::Init()
{

	AddComponent<Transform3DComponent>();
	GetComponent<Transform3DComponent>()->SetPosition(XMFLOAT3(0.0f, 5.0f, -5.0f));

	m_Target = { 0.0f,0.0f,1.0f };

}

void Camera::Uninit()
{

	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}

}

void Camera::Update()
{

}

void Camera::Draw()
{
	

	XMFLOAT3 pos = GetComponent<Transform3DComponent>()->GetPosition();

	//ビューマトリクスの設定
	XMFLOAT3 up{ 0.0f,1.0f,0.0f };
	XMMATRIX vierMatrix = XMMatrixLookAtLH(XMLoadFloat3(&pos),XMLoadFloat3(&m_Target),XMLoadFloat3(&up));

	Renderer::SetViewMatrix(vierMatrix);
	XMStoreFloat4x4(&m_ViewMatrix,vierMatrix);

	//プロジェクションマトリクス設定
	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(1.0f,(float)SCREEN_WIDTH/SCREEN_HEIGHT,1.0f,1000.0f);
	Renderer::SetProjectionMatrix(projectionMatrix);


}



XMMATRIX Camera::GetViewMatrix()
{
	//ビューマトリクスの設定
	return XMLoadFloat4x4(&m_ViewMatrix);
}
