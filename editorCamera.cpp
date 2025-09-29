#include "main.h"
#include "editorCamera.h"
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
#include "mapCreateScene.h"

void EditorCamera::Init()
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
	Tag = "Editor";

}

void EditorCamera::Uninit()
{

	for (auto com : ComponentList)
	{
		
		
		com->Uninit();
		delete com;
	
	}

}

void EditorCamera::Update()
{

	if (OnCursor == true) return;
	//RotateAroundを使うならこれ絶対宣言してね
	
	XMFLOAT3 rotatePosition = m_Target;
	XMFLOAT3 targetPos = GetComponent<Transform>()->GetPosition();

	
	rotatePosition.x = (m_Len * sin(m_Fai) * cos(m_Angle)) + targetPos.x;
	rotatePosition.y = (m_Len * cos(m_Fai)) + targetPos.y;
	rotatePosition.z = (m_Len * sin(m_Fai) * sin(m_Angle)) + targetPos.z;
	

	float Hight = rotatePosition.y;
	float lenghtY = Hight - targetPos.y;
	lenghtY = sqrt(lenghtY * lenghtY);


	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		//XMFLOAT3 worldPos = GetMouseWorldPosition();

		if (doOnce == false)
		{
			//現在のカーソルのポジション取得
			GetCursorPos(&m_MousePoint);

			m_CursorX = (float)m_MousePoint.x;
			m_CursorY = (float)m_MousePoint.y;
			m_OldCursorX = (float)m_MousePoint.x;
			m_OldCursorY = (float)m_MousePoint.y;
			doOnce = true;
		}
		float PI = 3.141592;
		m_Radian = PI * 0.01f;
		//XMFLOAT2 centerPos = { 760,410 };

		//現在のカーソルのポジション取得
		GetCursorPos(&m_MousePoint);

		m_CursorX = (float)m_MousePoint.x;
		m_CursorY = (float)m_MousePoint.y;

		float addX = 10.0f;
		float addY = 15.0f;

		difference.x = m_CursorX - m_OldCursorX;
		difference.y = m_CursorY - m_OldCursorY;

		
		m_Angle += -difference.x * (m_Radian * m_Sensitivity.x);
		GetComponent<Transform3DComponent>()->RotateAround(-difference.x * (m_Radian * m_Sensitivity.x), 0.0f);

		

		if (difference.y >= 1.0f)
		{
			m_Fai += -difference.y * (m_Radian * m_Sensitivity.y);
			GetComponent<Transform3DComponent>()->RotateAround(0.0f, -difference.y * (m_Radian * m_Sensitivity.y));
		}
		else if (difference.y <= -1.0f)
		{
			m_Fai += -difference.y * (m_Radian * m_Sensitivity.y);
			GetComponent<Transform3DComponent>()->RotateAround(0.0f, -difference.y * (m_Radian * m_Sensitivity.y));
		}
		
	}
	else
	{
		doOnce = false;
	}

	m_Target = rotatePosition;

	GetComponent<Transform>()->SetPosition(targetPos);

	Transform* trans = GetComponent<Transform>();

	XMFLOAT3 TransPos = trans->GetPosition();
	XMFLOAT3 TransRot = trans->GetRotation();

	XMFLOAT3 FowardVector = { rotatePosition.x - targetPos.x,rotatePosition.y - targetPos.y ,rotatePosition.z - targetPos.z };
	FowardVector = Normalize(FowardVector);
	FowardVector = { FowardVector.x * cameraSpeed,FowardVector.y * cameraSpeed,FowardVector.z * cameraSpeed, };

	float PI = 3.141592;
	float posRot = PI;

	float wheel = GetMouseWheelDelta();

	FowardVector = { FowardVector.x * wheel,FowardVector.y * wheel ,FowardVector.z * wheel };
	TransPos = { TransPos.x + FowardVector.x,TransPos.y + FowardVector.y ,TransPos.z + FowardVector.z };
	posRot = PI;
	
	if (GetAsyncKeyState(VK_MBUTTON) & 0x8000)//マウスホイールを動かしながら
	{
		//現在のカーソルのポジション取得
		GetCursorPos(&m_MousePoint);

		m_CursorX = (float)m_MousePoint.x;
		m_CursorY = (float)m_MousePoint.y;

		float addX = 10.0f;
		float addY = 15.0f;

		difference.x = m_CursorX - m_OldCursorX;
		difference.y = m_CursorY - m_OldCursorY;

		XMFLOAT3 rotatePos = m_Target;//見ているポジション
		float fai = 0.0f;
		float angle = 0.0f;

		rotatePos.x = (m_Len * sin(m_Fai) * cos(m_Angle)) + targetPos.x;
		rotatePos.y = (m_Len * cos(m_Fai)) + targetPos.y;
		rotatePos.z = (m_Len * sin(m_Fai) * sin(m_Angle)) + targetPos.z;


		if (difference.y >= 1.0f)//↑
		{
			fai = m_Fai + (PI / 4.0f);
			rotatePos.x = (m_Len * sin(fai) * cos(m_Angle)) + targetPos.x;
			rotatePos.y = (m_Len * cos(fai)) + targetPos.y;
			rotatePos.z = (m_Len * sin(fai) * sin(m_Angle)) + targetPos.z;


			XMFLOAT3 TopVector = { rotatePos.x - targetPos.x,rotatePos.y - targetPos.y ,rotatePos.z - targetPos.z };
			TopVector = Normalize(TopVector);

			//TopVector = {TopVector.x * }

			TransPos = Add(TransPos, TopVector);

		}
		if (difference.y <= -1.0f)//↓
		{

		}


		//カメラを上に移動させたい
	}


	m_OldCursorX = m_CursorX;
	m_OldCursorY = m_CursorY;

	trans->SetPosition(TransPos);
	trans->SetRotation(TransRot);

	

}

void EditorCamera::Draw()
{
	

	//DrawImGUI();

	XMFLOAT3 pos = GetComponent<Transform3DComponent>()->GetPosition();

	//ビューマトリクスの設定
	XMFLOAT3 up{ 0.0f,1.0f,0.0f };
	XMMATRIX vierMatrix = XMMatrixLookAtLH(XMLoadFloat3(&pos),XMLoadFloat3(&m_Target),XMLoadFloat3(&up));

	Renderer::SetViewMatrix(vierMatrix);
	m_ViewMatrix = vierMatrix;

	//プロジェクションマトリクス設定
	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(1.0f,(float)SCREEN_WIDTH/SCREEN_HEIGHT,1.0f,1000.0f);
	Renderer::SetProjectionMatrix(projectionMatrix);
	m_ProjectionMatrix = projectionMatrix;


}

void EditorCamera::DrawImGui()
{
	
	ImguiManager* imgui = Scene::GetInstance()->GetScene<GameScene>()->GetImguiManager();

	static  float speed = 1.0f;

	ImGui::SliderFloat("speed", &speed, 0.1f, 1.0f);
	cameraSpeed = speed;

	static float xy[2] = { 0.1f,0.1f};
	ImGui::SliderFloat2("sensitivity", xy, 0.1f, 3.0f);
	m_Sensitivity.x = xy[0];
	m_Sensitivity.y = xy[1];

	ImGui::Text("wheel %.2f", GetMouseWheelDelta());

	ImGui::Text("FPS = %.1f", imgui->FrameRate);
}



XMMATRIX EditorCamera::GetViewMatrix()
{
	//ビューマトリクスの設定
	return m_ViewMatrix;
}

XMFLOAT3 EditorCamera::GetMouseWorldPosition()
{

	// マウスのスクリーン座標を取得
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	ScreenToClient(GetActiveWindow(), &cursorPos);

	float mouseX = static_cast<float>(cursorPos.x);
	float mouseY = static_cast<float>(cursorPos.y);

	//スクリーン座標を「正規化デバイス座標 (NDC)」に変換
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	Renderer::GetDeviceContext()->RSGetViewports(&numViewports, &viewport);

	float ndcX = (mouseX / viewport.Width) * 2.0f - 1.0f;
	float ndcY = 1.0f - (mouseY / viewport.Height) * 2.0f; // Y軸は上下逆

	//逆射影行列（View * Projection の逆行列）を作成
	XMMATRIX viewMatrix = m_ViewMatrix;
	XMMATRIX projMatrix = m_ProjectionMatrix;
	XMMATRIX invVPMatrix = XMMatrixInverse(nullptr, viewMatrix * projMatrix);

	//近点 (Near) と遠点 (Far) の座標を計算（レイを作成）
	XMVECTOR nearPoint = XMVector3TransformCoord(XMVectorSet(ndcX, ndcY, 0.0f, 1.0f), invVPMatrix);
	XMVECTOR farPoint = XMVector3TransformCoord(XMVectorSet(ndcX, ndcY, 1.0f, 1.0f), invVPMatrix);

	//カメラ位置からレイを発射（方向ベクトルを計算）
	XMVECTOR rayDir = XMVector3Normalize(farPoint - nearPoint);
	XMFLOAT3 rayOrigin, rayDirection;
	XMStoreFloat3(&rayOrigin, nearPoint);
	XMStoreFloat3(&rayDirection, rayDir);

	//レイと地面の交点を計算
	float planeHeight = 0.0f; // フィールドの地面の高さ
	float t = (planeHeight - rayOrigin.y) / rayDirection.y; // レイの t を求める

	XMFLOAT3 worldPos = {
		rayOrigin.x + t * rayDirection.x,
		planeHeight,
		rayOrigin.z + t * rayDirection.z
	};

	return worldPos;
}

XMFLOAT3 EditorCamera::GetMouseRayDirection()
{
	//マウスのスクリーン座標を取得
	//POINT cursorPos;
	//GetCursorPos(&cursorPos);
	//ScreenToClient(GetActiveWindow(), &cursorPos);

	//float mouseX = static_cast<float>(cursorPos.x);
	//float mouseY = static_cast<float>(cursorPos.y);

	////スクリーン座標を「正規化デバイス座標 (NDC)」に変換
	//D3D11_VIEWPORT viewport;
	//UINT numViewports = 1;
	//Renderer::GetDeviceContext()->RSGetViewports(&numViewports, &viewport);

	MapCreateScene* scene = Scene::GetInstance()->GetScene<MapCreateScene>();

	float ndcX = scene->GetNdcX();
	float ndcY = scene->GetNdcY(); // Y軸は逆

	// `View * Projection` の逆行列を作成
	XMMATRIX viewMatrix = m_ViewMatrix;
	XMMATRIX projMatrix = m_ProjectionMatrix;
	XMMATRIX invVPMatrix = XMMatrixInverse(nullptr, viewMatrix * projMatrix);

	// 近点 (Near) と遠点 (Far) の座標を計算
	XMVECTOR nearPoint = XMVector3TransformCoord(XMVectorSet(ndcX, ndcY, 0.0f, 1.0f), invVPMatrix);
	XMVECTOR farPoint = XMVector3TransformCoord(XMVectorSet(ndcX, ndcY, 1.0f, 1.0f), invVPMatrix);

	// レイの方向を計算
	XMVECTOR rayDir = XMVector3Normalize(farPoint - nearPoint);

	XMFLOAT3 rayOrigin, rayDirection;
	XMStoreFloat3(&rayOrigin, nearPoint);
	XMStoreFloat3(&rayDirection, rayDir);

	return rayDirection;
}