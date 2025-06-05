#include "main.h"
#include "renderer.h"
#include "texturePool.h"

void TexturePool::Init()
{
	/*ID3D11ShaderResourceView* Texture;*/

	/*for (auto iter = m_TextureList.begin(); iter != m_TextureList.end(); iter++)
	{

		ID3D11ShaderResourceView* Texture{};

		CreateShaderResourceView(Renderer::GetDevice(),iter->second.second.GetImages(), iter->second.second.GetImageCount(), iter->second.first, &Texture);
		assert(Texture);

		m_TexturePool.emplace(Texture, iter->first);
	}*/

	
}

void TexturePool::LoadTextrue()
{

	//タイトル用
	AddTextrue(L"asset\\Texture\\title_4.png","TitleBack");
	AddTextrue(L"asset\\Texture\\push.png", "PushSpace");
	AddTextrue(L"asset\\Texture\\gametitle_1.png", "TitleName");
	AddTextrue(L"asset\\Texture\\fade_white.png", "Fade");
	AddTextrue(L"asset\\Texture\\star.png", "Star");
	AddTextrue(L"asset\\Texture\\Suimen2.jpg", "Suimen");
	AddTextrue(L"asset\\Texture\\Ground.jpg", "Ground");

	//ゲームシーン用
	AddTextrue(L"asset\\Texture\\number.png", "Number");
	AddTextrue(L"asset\\Texture\\explosion.png", "Explosion");
	AddTextrue(L"asset\\Texture\\jimen.jpg", "Jimen");
	AddTextrue(L"asset\\Texture\\tree.png", "Tree");
	AddTextrue(L"asset\\Texture\\SlashFirst.png", "slashFirst");
	AddTextrue(L"asset\\Texture\\SlashSecound.png", "slashSecound");
	AddTextrue(L"asset\\Texture\\sky.jpg", "Sky");
	AddTextrue(L"asset\\Texture\\magicFrame.png", "MagicFrame");
	AddTextrue(L"asset\\Texture\\Ice2.png", "Ice");
	AddTextrue(L"asset\\Texture\\BlackGround.jpg", "BlackGround");
	AddTextrue(L"asset\\Texture\\Hp.png", "Hp");
	AddTextrue(L"asset\\Texture\\HpFlame.png", "HpFlame");
	AddTextrue(L"asset\\Texture\\StaminaFlame.png", "StaminaFlame");
	AddTextrue(L"asset\\Texture\\Stamina.png", "Stamina");



}

void TexturePool::UnLoadTexture()
{


	for (std::pair<std::string, ID3D11ShaderResourceView*> tex : m_TexturePool)
	{
		tex.second->Release();
	}

	m_TexturePool.clear();
}

void TexturePool::AddTextrue(const wchar_t* TextureData, std::string name)
{
	
	/*std::pair<TexMetadata, ScratchImage> Tex;*/
	ID3D11ShaderResourceView* Texture{};

	TexMetadata metadata;
	ScratchImage image;
	ScratchImage mipChain;

	LoadFromWICFile(TextureData, WIC_FLAGS_NONE, &metadata, image);
	GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(),
		TEX_FILTER_DEFAULT,0,mipChain);
	

	CreateShaderResourceView(Renderer::GetDevice(),mipChain.GetImages(), mipChain.GetImageCount(), mipChain.GetMetadata(), &Texture);
	assert(Texture);

	m_TexturePool.emplace(name, Texture);
	/*Tex = std::make_pair(metadata, image);*/

	/*m_TextureList.emplace(name, Tex);*/

}

ID3D11ShaderResourceView* TexturePool::GetTexture(std::string name)
{
	if (m_TexturePool.count(name))
	{
		auto iter = m_TexturePool.find(name);
		return iter->second;
	}
	else
	{
		return nullptr;
	}

	/*for (auto iter = m_TexturePool.begin(); iter != m_TexturePool.end(); iter++)
	{
		if (iter->first == name)
		{
			return iter->second;
		}
	}

	return nullptr;*/
}
