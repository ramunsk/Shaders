////////////////////////////////////////////////////////////////////////////////
// Filename: trivial_shaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LIGHTING_SHADERCLASS_H_
#define _LIGHTING_SHADERCLASS_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
using namespace std;

#include "VertexTypes.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: LightingShaderClass
////////////////////////////////////////////////////////////////////////////////
class LightingShaderClass
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct CameraBufferType
	{
		D3DXVECTOR3 cameraPos;
		float padding;
	};

public:
	LightingShaderClass();
	LightingShaderClass(const LightingShaderClass&);
	~LightingShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, D3DXMATRIX&, D3DXMATRIX&, D3DXMATRIX&, D3DXVECTOR3, MaterialBufferType);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX&, D3DXMATRIX&, D3DXMATRIX&, D3DXVECTOR3, MaterialBufferType);
	void RenderShader(ID3D11DeviceContext*);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_cameraBuffer;
	ID3D11Buffer* m_lightBuffer;
};

#endif