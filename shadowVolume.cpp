#include "main.h"
#include "shadowVolume.h"
#include "modelRenderer.h"
#include "scene.h"
#include "manager.h"
#include "player.h"
#include "gameScene.h"

void ShadowVolume::Init()
{
	m_Component = new ModelRenderer(this);
	((ModelRenderer*)m_Component)->Load("asset\\Model\\sphere_smooth.obj");

	m_Size = { 0.5f,0.5f,0.5f };

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"shader\\vertexLightingVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader,
		"shader\\vertexLightingPS.cso");

	//�e�N�X�`���ǂݍ���
	TexMetadata metadata;
	ScratchImage image;
	LoadFromWICFile(L"asset\\Texture\\sky.jpg", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_EnvTexture);
	assert(m_EnvTexture);


}

void ShadowVolume::Uninit()
{
	m_Component->Uninit();
	delete m_Component;

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void ShadowVolume::Update()
{
	XMFLOAT3 playerPos = Scene::GetInstance()->GetScene<GameScene>()->GetGameObject<Player>()->GetComponent<Transform>()->GetPosition();

	m_Position = playerPos;


	//SetRotX(GetRot().x + 0.01f);
	//SetRotZ(GetRot().z + 0.01f);
}

void ShadowVolume::Draw()
{
	//���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	//�V�F�[�_�[�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//���[���h�}�g���N�X�ݒ�
	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Size.x, m_Size.y, m_Size.z);
	rot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(world);

	//�e�N�X�`���ݒ�
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_EnvTexture);

	//�J���[�o�b�t�@�}�X�N�L��
	Renderer::SetBlendMaskEnable(true);

	//�X�e���V�������L��
	Renderer::SetStencilEnable(true);

	//�J�����O�𖳌�
	Renderer::SetCullEnable(false);

	m_Component->Draw();

	//�J���[�o�b�t�@�}�X�N����
	Renderer::SetBlendMaskEnable(false);

	//�X�e���V����������
	Renderer::SetDepthEnable(true);

	//�J�����O�L��
	Renderer::SetCullEnable(true);
}
