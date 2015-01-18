////////////////////////////////////////////////////////////////////////////////
// Filename: d3dclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "d3dclass.h"

D3DClass::D3DClass()
{
	m_swapChain = 0;
	m_device = 0;
	m_deviceContext = 0;
	m_renderTargetView = 0;

	m_depthStencilBuffer = 0;
	m_depthStencilState = 0;
	m_depthStencilView = 0;
	m_rasterState = 0;
}


D3DClass::D3DClass(const D3DClass& other)
{
}


D3DClass::~D3DClass()
{
}

bool D3DClass::Initialize(HWND hwnd, bool fullscreen)
{
	wchar_t wbuffer[128];

	HRESULT result;

	// Fill the swap chain description
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory( &swapChainDesc, sizeof( swapChainDesc ) );

	//Obtain window dimensions from the haddle
	RECT dimensions;
    GetClientRect( hwnd, &dimensions );
	int screenWidth  = dimensions.right - dimensions.left;
    int screenHeight = dimensions.bottom - dimensions.top;

    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = screenWidth;
    swapChainDesc.BufferDesc.Height = screenHeight;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;  //Nelabai
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1; //netechnishkai
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if(fullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Driver types and feature levels
	D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE, D3D_DRIVER_TYPE_SOFTWARE
    };
    unsigned int totalDriverTypes = ARRAYSIZE( driverTypes );

	D3D_FEATURE_LEVEL finalFeatureLevel;
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_1
    };
    unsigned int totalFeatureLevels = ARRAYSIZE( featureLevels );

	// Create D3D Device
    unsigned int driver = 0;
    for( driver = 0; driver < totalDriverTypes; ++driver )
    {
        result = D3D11CreateDeviceAndSwapChain( 0, driverTypes[driver], 0, 0,
                                                featureLevels, totalFeatureLevels,
                                                D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain,
                                                &m_device, &finalFeatureLevel, &m_deviceContext);

        if( SUCCEEDED( result ) )
        {
			switch(driverTypes[driver])
			{
			case D3D_DRIVER_TYPE_HARDWARE:
				swprintf_s( wbuffer, 128, L"Device driver type =\nD3D_DRIVER_TYPE_HARDWARE\0");
				break;
			case D3D_DRIVER_TYPE_WARP:
				swprintf_s( wbuffer, 128, L"Device driver type =\nD3D_DRIVER_TYPE_WARP\0");
				break;
			case D3D_DRIVER_TYPE_REFERENCE:
				swprintf_s( wbuffer, 128, L"Device driver type =\nD3D_DRIVER_TYPE_REFERENCE\0");
				break;
			case D3D_DRIVER_TYPE_SOFTWARE:
				swprintf_s( wbuffer, 128, L"Device driver type =\nD3D_DRIVER_TYPE_SOFTWARE\0");
				break;
			default:
				swprintf_s( wbuffer, 128, L"Unspecified driver type\0");
			}

			switch(finalFeatureLevel)
			{
			case D3D_FEATURE_LEVEL_11_0:
				wcscat_s(wbuffer, 128, L"\nFeature level =\nD3D_FEATURE_LEVEL_11_0\0");
				break;
			case D3D_FEATURE_LEVEL_10_1:
				wcscat_s(wbuffer, 128, L"\nFeature level =\nD3D_FEATURE_LEVEL_10_1\0");
				break;
			case D3D_FEATURE_LEVEL_10_0:
				wcscat_s(wbuffer, 128, L"\nFeature level =\nD3D_FEATURE_LEVEL_10_0\0");
				break;
			default:
				wcscat_s(wbuffer, 128, L"\nOnly feature level of Direct3D 9 is supported\0");
			}

			//MessageBox(hwnd, wbuffer, L"D3DClass::Initialize", MB_OK);
            break;
        }
	/*
	D3D_FEATURE_LEVEL_9_1	= 0x9100,
	D3D_FEATURE_LEVEL_10_0	= 0xa000,
	D3D_FEATURE_LEVEL_10_1	= 0xa100,
	D3D_FEATURE_LEVEL_11_0	= 0xb000

	wsprintf(joined, "%S, %S", a[0], a[1]) 

	wcscpy(result, data[0]); 
	wcscat(result, L", "); 
    wcscat(result, data[n]); 
	*/
    }

 	if(FAILED(result))
	{
		MessageBox(hwnd, L"Failed to create the Direct3D device!", L"D3DClass::Initialize", MB_OK);
		return false;
	}

	// Get the pointer to the back buffer.
	ID3D11Texture2D* backBufferPtr;
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if(FAILED(result))
	{
		MessageBox(hwnd, L"Failed to get the swap chain back buffer!", L"D3DClass::Initialize", MB_OK);
		return false;
	}

	// Create the render target view with the back buffer pointer. (atsiranda pilkas fonas)
	result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
	if(FAILED(result))
	{
		MessageBox(hwnd, L"Failed to create the render target view!", L"D3DClass::Initialize", MB_OK);
		return false;
	}

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;

	// Initialize the description of the depth buffer.
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
	if(FAILED(result))
	{
		MessageBox(hwnd, L"Failed to create the texture for the depth buffer!", L"D3DClass::Initialize", MB_OK);
		return false;
	}

	// Initialize the description of the stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	if(FAILED(result))
	{
		MessageBox(hwnd, L"Failed to create the depth stencil state!", L"D3DClass::Initialize", MB_OK);
		return false;
	}

	// Set the depth stencil state.
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// Initialize the depth stencil view.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	if(FAILED(result))
	{
		MessageBox(hwnd, L"Failed to create the depth stencil view!", L"D3DClass::Initialize", MB_OK);
		return false;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	//m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, NULL);
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	//rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	if(FAILED(result))
	{
		MessageBox(hwnd, L"Failed to create the rasterizer state!", L"D3DClass::Initialize", MB_OK);
		return false;
	}

	// Now set the rasterizer state.
	m_deviceContext->RSSetState(m_rasterState);

	// Setup the viewport for rendering.
	D3D11_VIEWPORT viewport;
    viewport.Width = (float)screenWidth;
    viewport.Height = (float)screenHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

	// Create the viewport.
    m_deviceContext->RSSetViewports(1, &viewport);
   
	return true;
}

void D3DClass::Shutdown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if(m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	if(m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = 0;
	}
	
	if(m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = 0;
	}

	if(m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = 0;
	}

	if(m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = 0;
	}
	if(m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = 0;
	}

	if(m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}

	if(m_device)
	{
		m_device->Release();
		m_device = 0;
	}

	if(m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}

	return;
}


void D3DClass::BeginScene()
{
	// Setup the color to clear the buffer to.
	float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f};

	// Clear the back buffer.
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
    
	// Clear the depth buffer.
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}


void D3DClass::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	m_swapChain->Present(0, 0);

	return;
}


ID3D11Device* D3DClass::GetDevice()
{
	return m_device;
}


ID3D11DeviceContext* D3DClass::GetDeviceContext()
{
	return m_deviceContext;
}
