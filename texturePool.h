#pragma once
#include <map>
#include <string>
#include "renderer.h"
#include <iostream>
#include <tuple>

class TexturePool
{
private:
	

	TexMetadata metadata;
	ScratchImage image;

	TexturePool() = default;
	~TexturePool() = default;
	
	std::map<std::string, ID3D11ShaderResourceView*> m_TexturePool = {};
	std::map <std::string, std::pair<TexMetadata, ScratchImage>> m_TextureList = {};

	/*ID3D11ShaderResourceView* m_3DTexture{};
	const wchar_t* TextureData = L"‚È‚µ";*/

public:

	static TexturePool* GetInstance()
	{
		static TexturePool* pool = new TexturePool();
		return pool;
	}

	void Init();

	void LoadTextrue();
	void UnLoadTexture();

	void AddTextrue(const wchar_t* TextureData,std::string name);
	ID3D11ShaderResourceView* GetTexture(std::string name);
	


};

