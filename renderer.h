#pragma once





struct VERTEX_3D
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT4 Diffuse;
	XMFLOAT2 TexCoord;
};



struct MATERIAL
{
	XMFLOAT4	Ambient;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Specular;
	XMFLOAT4	Emission;
	float		Shininess;
	BOOL		TextureEnable;
	float		Dummy[2];
};



struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];
	XMFLOAT4	Direction;
	XMFLOAT4	Diffuse;
	XMFLOAT4	Ambient;
	//XMFLOAT4 SkyColor; //天球色
	//XMFLOAT4 GroundColor; //地面色
	//XMFLOAT4 GroundNormal; //地面の法線

	//XMFLOAT4 Position; //光源の座標
	//XMFLOAT4 PointLightParam; //光の範囲


};



class Renderer
{
private:

	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device* m_Device;
	static ID3D11DeviceContext* m_DeviceContext;
	static IDXGISwapChain* m_SwapChain;
	static ID3D11RenderTargetView* m_RenderTargetView;
	static ID3D11DepthStencilView* m_DepthStencilView;

	static ID3D11Buffer* m_WorldBuffer;
	static ID3D11Buffer* m_ViewBuffer;
	static ID3D11Buffer* m_ProjectionBuffer;
	static ID3D11Buffer* m_ViewProjectionBuffer;
	static ID3D11Buffer* m_MaterialBuffer;
	static ID3D11Buffer* m_LightBuffer;
	static ID3D11Buffer* m_CameraBuffer;
	//ビュー射影行列の定数バッファ
	static ID3D11Buffer* m_MatrixBuffer;
	


	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;
	static ID3D11DepthStencilState* m_StencilWrite;
	static ID3D11DepthStencilState* m_StencilRead;

	static ID3D11BlendState* m_BlendState;
	static ID3D11BlendState* m_BlendStateAdd;	//加算合成
	static ID3D11BlendState* m_BlendStateATC;
	static ID3D11BlendState* m_BlendStateMask;

	static ID3D11RasterizerState* m_RasterizerStateCullBack;
	static ID3D11RasterizerState* m_RasterizerStateCullNone;

	static ID3D11Texture2D* m_GameRenderTargetTexture;
	static ID3D11Buffer* m_SwapChainBuffer;
	static ID3D11RenderTargetView* m_SwapChainRenderTargetView;
	static ID3D11Texture2D* m_RenderTargetTexture;
	static ID3D11ShaderResourceView* m_TextureSRV;

public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void Update();
	static void End();

	static void SetDepthEnable(bool Enable);
	static void SetATCEnable(bool Enable);
	static void SetStencilEnable(bool Enable);
	static void SetBlendAddEnable(bool Enable);
	static void SetBlendMaskEnable(bool Enable);
	static void SetCullEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(XMMATRIX WorldMatrix);
	static void SetViewMatrix(XMMATRIX ViewMatrix);
	static void SetProjectionMatrix(XMMATRIX ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT Light);
	static void SetCameraPosision(XMFLOAT3 CameraPosition);

	static ID3D11Device* GetDevice(void) { return m_Device; }
	static ID3D11DeviceContext* GetDeviceContext(void) { return m_DeviceContext; }
	static ID3D11RenderTargetView* GetRenderTagetView(void) { return m_RenderTargetView; }
	static IDXGISwapChain* GetSwapChain(void) { return m_SwapChain; }
	static ID3D11ShaderResourceView* GetTextureSRV() { return m_TextureSRV; }

	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);
	static void CreateComputeShader(ID3D11ComputeShader** ComputeShader, const char* FileName);


};
