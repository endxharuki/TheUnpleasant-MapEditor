#include "main.h"
#include "gameScene.h"
#include "scene.h"
#include "transform3DComponent.h"
#include "boxColiderComponent.h"
#include "player.h"
#include "playerCamera.h"
#include "input.h"
#include "circle2DColider.h"


XMFLOAT2 difference = { 0,0 };
/*
Unityの３Dのカメラ制御をのプログラムを見ろ
*/

//三次元の回転座標変換 公式
/*

座標A = 動かす前の元の座標
座標B = 動かした後の座標
β = 動かした角度またはラジアン

x軸回転
Bx = Ax * 1 + Ay *  0 + Az * 0,
By = Ax * 0 + Ay * cosβ + Az * sinβ
Bz = Ax * 0 + Ay * -sinβ + Az * cosβ
y軸回転
Bx = Ax * cosβ + Ay * 0 + Az * sinβ,
By = Ax *   0   + Ay * 1 + Az * 0
Bz = Ax * -sinβ + Ay * 0 + Az * cosβ
z軸回転
Bx = Ax * cosβ + Ay * sinβ + Az * 0,
By = Ax * -sinβ + Ay * cosβ + Az * 0
Bz = Ax * 0 + Ay * 0 + Az * 1

*/

//球座標から直交座標に
/*
 r = sqrt(x*x + y*y + z*z)
θ = atan(y / x)
Φ = atan(sqrt(x*x + y*y) / z)
*/
//公式と変える
/*
 r = sqrt(x*x + y*y + z*z)
θ = atan(z / x)
Φ = atan(sqrt(x*x + z*z) / y)
*/
//直交座標から球座標に
/*
x = r * sin(Φ) * cos(θ)
y = r * sin(Φ) * sin(θ)
z = r * cos(Φ)
*/
//公式と変える
/*
x = r * sin(Φ) * cos(θ)
y = r * cos(Φ)
z = r * sin(Φ) * sin(θ)
*/

void PlayerCamera::Init()
{


	AddComponent<Transform3DComponent>();

	GetComponent<Transform3DComponent>()->SetPosition(XMFLOAT3(0.0f, 3.0f, 3.5f));

	AddComponent<Circle2DColider>()->OnImGui();
	/*m_Position = XMFLOAT3(0.0f, 20.0f, -30.0f);*/
	m_Target = { 0.0f,0.0f,0.0f };

	GetComponent<Circle2DColider>()->SetRadius(200.0f);

	//float vx = GetComponent<Transform3DComponent>()->GetPosition().x - m_Target.x;
	//float vz = GetComponent<Transform3DComponent>()->GetPosition().z - m_Target.z;
	//float vy = GetComponent<Transform3DComponent>()->GetPosition().y - m_Target.y;

	//float x2 = vx * vx;
	//float y2 = vy * vy;
	//float z2 = vz * vz;

	////球座標を求める
	//m_Len = sqrt(x2 + y2 + z2);
	//m_Angle = atan(vz / vx);
	//m_Fai = atan(sqrt(x2 + z2) / vy);

	

}

void PlayerCamera::Uninit()
{

	for (auto com : ComponentList)
	{
		com->Uninit();
		delete com;
	}

}

void PlayerCamera::Update()
{
	XMFLOAT3 cameraPos = GetComponent<Transform3DComponent>()->GetPosition();
	XMFLOAT3 cameraOldPos = GetComponent<Transform3DComponent>()->GetOldPosition();
	XMFLOAT3 cameraRot = GetComponent<Transform3DComponent>()->GetRotation();
	XMFLOAT3 playerPos = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetPosition();
	GetComponent<Transform3DComponent>()->SetOldPosition(cameraPos);

	m_Target = {playerPos.x + AddTargetPosition.x,playerPos.y + AddTargetPosition.y ,playerPos.z + AddTargetPosition.z };
	m_Target.y += 1.0f;

	for (auto com : ComponentList)
	{
		com->Update();
	}

	CameraControl();
	

}

void PlayerCamera::Draw()
{
	for (auto com : ComponentList)
	{
		com->Draw();
	}
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

	GetComponent<Circle2DColider>()->SetViewMatrix(vierMatrix);
	GetComponent<Circle2DColider>()->SetProjectionMatrix(projectionMatrix);
	//DrawImGui();

	

}


void PlayerCamera::CameraControl()
{

	//プレイヤー
	XMFLOAT3 playerPos = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetPosition();
	//XMFLOAT3 playerOldPos = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetOldPosition();
	//XMFLOAT3 difference = XMFLOAT3(playerPos.x - playerOldPos.x, playerPos.y - playerOldPos.y, playerPos.z - playerOldPos.z);//プレイヤーがどのぐらい動いたかの差分
	Player* player = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>();
	//カメラ
	XMFLOAT3 cameraPos = GetComponent<Transform3DComponent>()->GetPosition();
	XMFLOAT3 cameraOldPos = GetComponent<Transform3DComponent>()->GetOldPosition();
	XMFLOAT3 cameraRot = GetComponent<Transform3DComponent>()->GetRotation();
	float posRot = GetComponent<Transform3DComponent>()->GetRot();
	float PI = 3.14159265359;
	float Radian = PI * 0.01f;

	//RotateAroundを使うならこれ絶対宣言してね
	GetComponent<Transform3DComponent>()->RotateAroundInit(playerPos);
	if (doOnce == false)
	{
		 m_Len = GetComponent<Transform>()->GetLength();
		doOnce = true;
	}

	float Hight = GetComponent<Transform3DComponent>()->GetRotatePosition(playerPos).y;
	float lenghtY = Hight - playerPos.y;
	lenghtY = sqrt(lenghtY * lenghtY);

	XMFLOAT3 vec = { 0.707f,0.0f,0.707f };

	
	//キーボードバージョン
	{
		if ((GetKeyState('J') & 0x8000))
		{
			GetComponent<Transform3DComponent>()->RotateAround(Radian, 0.0f);
		}
		if ((GetKeyState('L') & 0x8000)) {

			GetComponent<Transform3DComponent>()->RotateAround(-Radian, 0.0f);
		}
		if (Input::GetInstance()->GetKeyTrigger('I'))
		{
			//上の上限をつける
			if (lenghtY <= (GetComponent<Transform3DComponent>()->GetLength() - 0.2f) || cameraPos.y <= playerPos.y)
			{
				if (cameraPos.y > playerPos.y) GetComponent<Transform3DComponent>()->RotateAround(0.0f, -Radian);
				else GetComponent<Transform3DComponent>()->RotateAround(0.0f, -Radian);
			}
		}
		if (Input::GetInstance()->GetKeyTrigger('K'))
		{
			//下の上限をつける
			if (lenghtY <= (GetComponent<Transform3DComponent>()->GetLength() - 0.2f) || cameraPos.y >= playerPos.y)
			{
				if (cameraPos.y > playerPos.y) GetComponent<Transform3DComponent>()->RotateAround(0.0f, Radian);
				else GetComponent<Transform3DComponent>()->RotateAround(0.0f, Radian);

			}
		}
	}
	
	//マウスバージョン
	if (Input::GetInstance()->GetKeyTrigger(VK_RETURN))
	{
		OnCursor = !OnCursor;
	}
	/*if ((GetKeyState('P') & 0x8000))  OnCursor = !OnCursor;*/

	if (OnCursor == true)
	{
		XMFLOAT2 centerPos = { 760,410 };
		
		//現在のカーソルのポジション取得
		GetCursorPos(&m_MousePoint);

		m_CursorX = (float)m_MousePoint.x;
		m_CursorY = (float)m_MousePoint.y;

		float addX = 10.0f;
		float addY = 20.0f;

		difference.x = m_CursorX - centerPos.x;
		difference.y = m_CursorY - centerPos.y;

		
		//カーソルが横に動いた場合
		if (difference.x <= -addX || difference.x >= addX)
		{
			//カーソルを画面の中央に固定
			difference.x = m_CursorX - centerPos.x;
			GetComponent<Transform3DComponent>()->RotateAround(-difference.x * (Radian * m_Sensitivity.x), 0.0f);
			SetCursorPos(760, 410);
		}
		//カーソルが下に動いた場合
		if (difference.y <= -addY)
		{
			difference.y = m_CursorY - centerPos.y;

			if (lenghtY <= (GetComponent<Transform3DComponent>()->GetLength() - 0.2f) || cameraPos.y >= playerPos.y)
			{
				GetComponent<Transform3DComponent>()->RotateAround(0.0f, -difference.y * (Radian * m_Sensitivity.y));
			}
			SetCursorPos(760, 410);

		}
		//カーソルが上に動いた場合
		if (difference.y >= addY)
		{
			difference.y = m_CursorY - centerPos.y;

			if (lenghtY <= (GetComponent<Transform3DComponent>()->GetLength() - 0.2f) || cameraPos.y <= playerPos.y)
			{
				GetComponent<Transform3DComponent>()->RotateAround(0.0f, -difference.y * (Radian * m_Sensitivity.y));

			}
			SetCursorPos(760, 410);
		}
	}

	cameraPos = GetComponent<Transform3DComponent>()->GetRotatePosition(playerPos);
	GetComponent<Transform3DComponent>()->SetPosition(cameraPos);

	//エイム処理
	//マウスの左を押しているか、プレイヤーが魔法を使える状態か
	if (GetAsyncKeyState(VK_RBUTTON))
	{
	
		XMFLOAT3 right = GetComponent<Transform>()->GetRight();
		AddTargetPosition = { right.x * -1.5f,right.y * -1.5f ,right.z * -1.5f };

		GetComponent<Circle2DColider>()->CircleDrawState(true);
		GetComponent<Circle2DColider>()->CircleUpdateState(true);

		float len = GetComponent<Transform>()->GetLength();
		if (len > 3.5f)
		{
			GetComponent<Transform>()->SetLength(len - 0.2f);
		}
		XMFLOAT3 rot = player->GetComponent<Transform>()->GetRotation();
		rot = XMFLOAT3(rot.x, PI + cameraRot.y + 0.5f, rot.z);
		player->GetComponent<Transform>()->SetRotation(rot);

		AssistCamera();
	}
	else
	{
		AddTargetPosition = { 0.0f,0.0f,0.0f};

		GetComponent<Circle2DColider>()->CircleDrawState(false);
		GetComponent<Circle2DColider>()->CircleUpdateState(false);
		float len = GetComponent<Transform>()->GetLength();
		if (len < m_Len)
		{
			GetComponent<Transform>()->SetLength(len + 0.2f);
		}
		
	}
	
	if (Input::GetInstance()->GetKeyPress('U'))
	{
		float len = GetComponent<Transform>()->GetLength();
		GetComponent<Transform>()->SetLength(len + 0.01f);
	}
	if (Input::GetInstance()->GetKeyPress('N'))
	{
		float len = GetComponent<Transform>()->GetLength();
		GetComponent<Transform>()->SetLength(len - 0.01f);
	}


	

	if (cameraPos.y < playerPos.y)
	{
		cameraPos.y = playerPos.y - 0.1f;
	}

	GetComponent<Transform3DComponent>()->SetPosition(cameraPos);
	/*GetComponent<Transform3DComponent>()->SetRotation(cameraRot);*/
	GetComponent<Transform3DComponent>()->SetRot(posRot);

}

void PlayerCamera::AssistCamera()
{
	XMFLOAT2 centerPos = GetComponent<Circle2DColider>()->GetPosition();
	XMFLOAT2 targetPos = GetComponent<Circle2DColider>()->GetTargetPosition();
	float PI = 3.14159265359;
	float Radian = PI * 0.01f;

	if (GetComponent<Circle2DColider>()->GetIsInObject() == true) 
	{
		if (centerPos.x + 2.0f < targetPos.x)
		{
			GetComponent<Transform3DComponent>()->RotateAround(-5 * (Radian * m_Sensitivity.x), 0.0f);
		}
		else if (centerPos.x - 2.0f > targetPos.x)
		{
			GetComponent<Transform3DComponent>()->RotateAround(5 * (Radian * m_Sensitivity.x), 0.0f);

		}

		if (centerPos.y + 2.0f < targetPos.y)
		{
			GetComponent<Transform3DComponent>()->RotateAround(0.0f, -5 * (Radian * m_Sensitivity.y));

		}
		else if (centerPos.y - 2.0f > targetPos.y)
		{
			GetComponent<Transform3DComponent>()->RotateAround(0.0f, 5 * (Radian * m_Sensitivity.y));

		}

	}

}

XMMATRIX PlayerCamera::GetViewMatrix() const
{
	//ビューマトリクスの設定
	return XMLoadFloat4x4(&m_ViewMatrix);
}

void PlayerCamera::DrawImGui()
{
	XMFLOAT3 playerPos = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetPosition();
	XMFLOAT3 cameraPos = GetComponent<Transform3DComponent>()->GetPosition();

	XMFLOAT3 vel = {m_Target.x - cameraPos.x,m_Target.x - cameraPos.x ,m_Target.x - cameraPos.x };
	vel = GetComponent<GameObjectComponent>()->Normalize(vel);

	float cameraPosTop = cameraPos.y + playerPos.y;
	float cameraPosBottom = playerPos.y - (cameraPos.y - playerPos.y);

	float lenghtY = cameraPos.y - playerPos.y;
	
	lenghtY = sqrt(lenghtY * lenghtY);


	{

		ImGui::Begin("PlayerCamera");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

		ImGui::Text("cameraPosTop = %f", cameraPosTop);
		ImGui::Text("cameraPosBttom = %f", cameraPosBottom);
		ImGui::Text("playerpos = %f", playerPos);

		ImGui::Text("cameraPos.y = %f", cameraPos.y);

		ImGui::Text("cursorpoint = %.1f , %.1f", m_CursorX, m_CursorY);
		/*ImGui::Text("MousePos = %d",)*/

		ImGui::SliderFloat("Xsensi", &m_Sensitivity.x, 0.001f, 0.05f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::SliderFloat("Ysensi", &m_Sensitivity.y, 0.001f, 0.05f);            // Edit 1 float using a slider from 0.0f to 1.0f

		ImGui::Text("cameraRight x %.1f, y %.1f, z %.1f", vel.x, vel.y, vel.z);

		ImGui::Text("cursorVec = %.1f , %.1f", difference.x, difference.y);

		ImGui::Text("Yのながさ = %f", lenghtY);

		ImGui::Text("Length = %f", GetComponent<Transform3DComponent>()->GetLength());

		ImGui::End();

	}

}

bool PlayerCamera::CehckView(XMFLOAT3)
{
	XMFLOAT3 playerPos = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform3DComponent>()->GetPosition();

	XMFLOAT3 cameraPos = GetComponent<Transform3DComponent>()->GetPosition();


	XMFLOAT3 up(0.0f, 1.0f, 0.0f);
	XMMATRIX viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&cameraPos), XMLoadFloat3(&playerPos), XMLoadFloat3(&up));

	XMMATRIX projectionMatrix;
	projectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	XMMATRIX vp, invvp;
	XMVECTOR det;

	vp = viewMatrix * projectionMatrix;
	invvp - XMMatrixInverse(&det, vp);

	XMFLOAT3 vpos[4];
	XMFLOAT3 wpos[4];


	vpos[0] = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	vpos[1] = XMFLOAT3(1.0f, 1.0f, 1.0f);
	vpos[2] = XMFLOAT3(-1.0f, -1.0f, 1.0f);
	vpos[3] = XMFLOAT3(1.0f, -1.0f, 1.0f);

	XMVECTOR vposv[4];
	XMVECTOR wposv[4];

	vposv[0] = XMLoadFloat3(&vpos[0]);
	vposv[1] = XMLoadFloat3(&vpos[1]);
	vposv[2] = XMLoadFloat3(&vpos[2]);
	vposv[3] = XMLoadFloat3(&vpos[3]);

	wposv[0] = XMVector3TransformCoord(vposv[0], invvp);
	wposv[1] = XMVector3TransformCoord(vposv[1], invvp);
	wposv[2] = XMVector3TransformCoord(vposv[2], invvp);
	wposv[3] = XMVector3TransformCoord(vposv[3], invvp);


	XMStoreFloat3(&wpos[0], wposv[0]);
	XMStoreFloat3(&wpos[1], wposv[1]);
	XMStoreFloat3(&wpos[2], wposv[2]);
	XMStoreFloat3(&wpos[3], wposv[3]);


	return false;
}
