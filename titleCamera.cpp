#include "main.h"
#include "titleCamera.h"
#include "gameScene.h"
#include "scene.h"
#include "transform3DComponent.h"
#include "boxColiderComponent.h"
#include "player.h"
#include "input.h"
#include "wave.h"
#include "scene.h"
#include "titleScene.h"
#include "audio.h"

void TitleCamera::Init()
{

	AddComponent<Transform3DComponent>();
	GetComponent<Transform3DComponent>()->SetPosition(XMFLOAT3(0.0f, 13.0f, -5.0f));

	m_Target = { 0.0f,1.0f,0.0f };

	XMFLOAT3 pos = GetComponent<Transform3DComponent>()->GetPosition();

	float vx = m_Target.x - pos.x;
	float vy = m_Target.y - pos.y;
	float vz = m_Target.z - pos.z;

	float x2 = vx * vx;
	float y2 = vy * vy;
	float z2 = vz * vz;

	//球座標を求める
	m_Len = sqrt(x2 + y2 + z2);
	m_Angle = atanf(vz / vx);
	m_Fai = atanf(sqrt(x2 + z2) / vy);
	/*if (vx <= -0.001f)
	{
		m_Angle = 3.141592 + m_Angle;
	}*/

	AddComponent<Audio>()->InitMaster();
	GetComponent<Audio>()->Load("asset\\Audio\\waterDrop.wav");
	GetComponent<Audio>()->SetVolume(0.8f);
}

void TitleCamera::Uninit()
{

	for (auto com : ComponentList)
	{
		
		
		com->Uninit();
		delete com;
	
	}

}

void TitleCamera::Update()
{

	//RotateAroundを使うならこれ絶対宣言してね
	
	XMFLOAT3 rotatePosition = m_Target;
	XMFLOAT3 targetPos = GetComponent<Transform>()->GetPosition();

	
	rotatePosition.x = (m_Len * sin(m_Fai) * cos(m_Angle)) + targetPos.x;
	rotatePosition.y = (m_Len * cos(m_Fai)) + targetPos.y;
	rotatePosition.z = (m_Len * sin(m_Fai) * sin(m_Angle)) + targetPos.z;
	

	float Hight = rotatePosition.y;
	float lenghtY = Hight - targetPos.y;
	lenghtY = sqrt(lenghtY * lenghtY);

	//上下させる
	{

		if (m_UpDownFrame >= 100.0f)
		{
			m_ChangeUpDown = !m_ChangeUpDown;
			m_UpDownFrame = 0.0f;

			if (m_ChangeUpDown == false) UpDownSpeed = 0.0f;
			if (m_ChangeUpDown == true)
			{
				/*m_Stop = true;*/
				UpDownSpeed = 8.0f;
				Scene::GetInstance()->GetScene<TitleScene>()->GetGameObject<Wave>()->SetCameraPos(targetPos.x, targetPos.z);
				m_StopFrame = 0.0f;
				GetComponent<Audio>()->Play(false);
			}
		}
		if (m_ChangeUpDown == true)
		{
			targetPos.y += 0.01f * UpDownSpeed;
			UpDownSpeed -= 0.08f;
		}
		if (m_ChangeUpDown == false)
		{
			targetPos.y -= 0.01f * UpDownSpeed;
			UpDownSpeed += 0.08f;
		}

		//前に行き過ぎないようにする
		if (targetPos.z <= 20.0f)
		{
			targetPos.z += 0.02f;
		}
		else
		{
			m_Stop = true;
		}
		m_UpDownFrame++;

	}

	//マウスバージョン
	
	if (Input::GetInstance()->GetKeyTrigger('P'))  OnCursor = !OnCursor;

	if (OnCursor == true)
	{
		float PI = 3.141592;
		m_Radian = PI * 0.01f;
		XMFLOAT2 centerPos = { 760,410 };

		//現在のカーソルのポジション取得
		GetCursorPos(&m_MousePoint);

		m_CursorX = (float)m_MousePoint.x;
		m_CursorY = (float)m_MousePoint.y;

		float addX = 10.0f;
		float addY = 15.0f;

		difference.x = m_CursorX - centerPos.x;
		difference.y = m_CursorY - centerPos.y;

		if (difference.x <= -addX || difference.x >= addX)
		{
			//カーソルを画面の中央に固定
			difference.x = m_CursorX - centerPos.x;
			m_Angle += -difference.x * (m_Radian * m_Sensitivity.x);
			/*GetComponent<Transform3DComponent>()->RotateAround(-difference.x * (m_Radian * m_Sensitivity.x), 0.0f);*/
			SetCursorPos(760, 410);
		}
		if (difference.y <= -addY)
		{
			difference.y = m_CursorY - centerPos.y;

			if (lenghtY <= (m_Len - 0.2f) || rotatePosition.y <= targetPos.y)
			{
				m_Fai += -difference.y * (m_Radian * m_Sensitivity.y);
				/*GetComponent<Transform3DComponent>()->RotateAround(0.0f, -difference.y * (m_Radian * m_Sensitivity.y));*/
			}
			SetCursorPos(760, 410);

		}
		if (difference.y >= addY)
		{
			difference.y = m_CursorY - centerPos.y;

			if (lenghtY <= (m_Len - 0.2f) || rotatePosition.y >= targetPos.y)
			{
				m_Fai += -difference.y * (m_Radian * m_Sensitivity.y);
				/*GetComponent<Transform3DComponent>()->RotateAround(0.0f, -difference.y * (m_Radian * m_Sensitivity.y));*/
			}
			SetCursorPos(760, 410);
		}
	}

	m_Target = rotatePosition;
	GetComponent<Transform>()->SetPosition(targetPos);

}

void TitleCamera::Draw()
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



XMMATRIX TitleCamera::GetViewMatrix()
{
	//ビューマトリクスの設定
	return XMLoadFloat4x4(&m_ViewMatrix);
}
