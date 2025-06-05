#include "main.h"
#include "player.h"
#include "transform2DComponent.h"
#include "bullet.h"
#include "scene.h"
#include "gameScene.h"
#include "particleSpowner.h"
#include "camera.h"
#include "transform3DComponent.h"
#include "texturePool.h"
#include "titleCamera.h"

//�^�~�ɍ炭��Ղ̍������
void ParticleSpowner::Init()
{

	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-3.0f, 3.0f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(3.0f, 3.0f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-3.0f, -3.0f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(3.0f, -3.0f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	//���_�o�b�t�@����
	D3D11_BUFFER_DESC bd{};
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	m_3DTexture = TexturePool::GetInstance()->GetTexture("Star");


	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	m_Position = XMFLOAT3(5.0f, 1.0f, 5.0f);
	m_Size = XMFLOAT3(0.5f, 0.5f, 0.5f);

}

void ParticleSpowner::Uninit()
{
	

	m_VertexBuffer->Release();
	m_3DTexture->Release();

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void ParticleSpowner::Update()
{
	//�p�[�e�B�N������
	for (int i = 0; i < PARTICLE_MAX;i++) {

		if (m_Particle[i].Enable == false)
		{
			m_Particle[i].Enable = true;
			m_Particle[i].Life = 60;
			m_Particle[i].pos = m_Position;

			m_Particle[i].vel.x = (rand() % 50 - 50) / 200.0f;
			m_Particle[i].vel.y = 0.0f;
			m_Particle[i].vel.z = (rand() % 50 - 50) / 200.0f;

			break;
		}

	}
	//�p�[�e�B�N���̈ړ�
	for (int i = 0; i < PARTICLE_MAX;i++)
	{
		if (m_Particle[i].Enable == true)
		{
			//�d��
			m_Particle[i].vel.y += -0.01f;

			m_Particle[i].pos.x += m_Particle[i].vel.x;
			m_Particle[i].pos.y += m_Particle[i].vel.y;
			m_Particle[i].pos.z += m_Particle[i].vel.z;

			//���C�t
			m_Particle[i].Life--;
			if (m_Particle[i].Life == 0)
			{
				m_Particle[i].Enable = false;
			}

		}
	}

	DeleteRecest();

}

void ParticleSpowner::Draw()
{

	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	XMMATRIX view = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<TitleCamera>()->GetViewMatrix();

	//�r���[�̋t�s��
	XMMATRIX invView;
	invView = XMMatrixInverse(nullptr, view);//�t�s��
	invView.r[3].m128_f32[0] = 0.0f;
	invView.r[3].m128_f32[1] = 0.0f;
	invView.r[3].m128_f32[2] = 0.0f;

	//���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);


	//�}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_3DTexture);

	//�v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	/*�|���S���`��*/
	/*Renderer::GetDeviceContext()->Draw(4, 0);*/

	Renderer::SetDepthEnable(false);

	for (int i = 0; i < PARTICLE_MAX;i++) {

		if (m_Particle[i].Enable == true) {
			//���[���h�}�g���N�X�ݒ�
			XMMATRIX world, scale, rot, trans;
			scale = XMMatrixScaling(m_Size.x, m_Size.y, m_Size.z);
			trans = XMMatrixTranslation(m_Particle[i].pos.x, m_Particle[i].pos.y, m_Particle[i].pos.z);
			world = scale * invView * trans;
			Renderer::SetWorldMatrix(world);

			//�|���S���`��
			Renderer::GetDeviceContext()->Draw(4, 0);
		}
	}
	Renderer::SetDepthEnable(true);

}
