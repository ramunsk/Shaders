////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Transform = 0;
	//m_Sphere = 0;
	m_Cube = 0;
	m_PlaneModel = 0;
	m_LightingShader = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(HWND hwnd)
{
	bool result;

	// Create the Direct3D object.
	m_D3D = new D3DClass();
	if(!m_D3D)
	{
		MessageBox(hwnd, L"Could not create the D3DClass object!", L"Error", MB_OK);
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(hwnd, FULL_SCREEN);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D!", L"Error", MB_OK);
		return false;
	}

	// Create the Sphere object.
/*	m_Sphere = new SphereModelClass();
	if(!m_Sphere)
	{
		MessageBox(hwnd, L"Could not create the Sphere object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the Sphere object.
	result = m_Sphere->Initialize(m_D3D->GetDevice());
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the Sphere object.", L"Error", MB_OK);
		return false;
	}
*/
	// Create the cube8 object.
	m_Cube = new CubeModelClass();
	if(!m_Cube)
	{
		MessageBox(hwnd, L"Could not create the cube8 object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the cube object.
	result = m_Cube->Initialize(m_D3D->GetDevice());
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the cube8 object.", L"Error", MB_OK);
		return false;
	}

	// Create the Plane field object.
	m_PlaneModel = new PlaneModelClass();
	if(!m_PlaneModel)
	{
		MessageBox(hwnd, L"Could not create the Plane field object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the model object.
	result = m_PlaneModel->Initialize(m_D3D->GetDevice());
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the Plane field object.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Transform = new TransformationClass();
	if(!m_Transform)
	{
		MessageBox(hwnd, L"Could not create the camera object.", L"Error", MB_OK);
		return false;
	}

	m_Transform->Initialize(hwnd);
	// Set the initial position of the camera.
	m_Transform->SetCameraPosition(D3DXVECTOR3(0.0f, 2.0f, -10.0f));
	m_Transform->SetCameraTarget(D3DXVECTOR3(0.0f, 2.0f, 0.0f));
	m_Transform->SetCameraUPV(D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	// Create the trivial shader object.
	m_LightingShader = new LightingShaderClass();
	if(!m_LightingShader)
	{
		MessageBox(hwnd, L"Could not create the color shader object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the trivial shader object.
	result = m_LightingShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the trivial shader object.
	if(m_LightingShader)
	{
		m_LightingShader->Shutdown();
		delete m_LightingShader;
		m_LightingShader = 0;
	}

	// Release the model objects.
/*	if(m_Sphere)
	{
		m_Sphere->Shutdown();
		delete m_Sphere;
		m_Sphere = 0;
	}
*/
	if(m_Cube)
	{
		m_Cube->Shutdown();
		delete m_Cube;
		m_Cube = 0;
	}

	if(m_PlaneModel)
	{
		m_PlaneModel->Shutdown();
		delete m_PlaneModel;
		m_PlaneModel = 0;
	}

	// Release the camera object.
	if(m_Transform)
	{
		delete m_Transform;
		m_Transform = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}

bool GraphicsClass::Frame()
{
	if(!Render())
	{
		return false;
	}
	else
		return true;
}


bool GraphicsClass::Render()
{
	bool result;

	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene();

	// Generate the view matrix based on the camera's position.
	m_Transform->Render();
	// Get the view and projection matrices from the transformation object.
	viewMatrix = m_Transform->GetViewMatrix();
	projectionMatrix = m_Transform->GetProjectionMatrix();

	// Get the world matrix of the cube8.
	worldMatrix = m_Cube->GetModelWorldMatrix();
	// Render the model using the trivial shader.
	result = m_LightingShader->Render(m_D3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Transform->GetCameraPosition(), m_Cube->getMaterial());
	if(!result)
	{
		return false;
	}
	m_Cube->Render(m_D3D->GetDeviceContext());

	// Get the world matrix of the Sphere.
/*	worldMatrix = m_Sphere->GetModelWorldMatrix();
	// Render the model using the trivial shader.
	result = m_LightingShader->Render(m_D3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Transform->GetCameraPosition(), m_Sphere->getMaterial());
	if(!result)
	{
		return false;
	}
	m_Sphere->Render(m_D3D->GetDeviceContext());
*/
	// Put the Plane model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	worldMatrix = m_PlaneModel->GetModelWorldMatrix();
	result = m_LightingShader->Render(m_D3D->GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, m_Transform->GetCameraPosition(), m_PlaneModel->getMaterial());
	if(!result)
	{
		return false;
	}
	m_PlaneModel->Render(m_D3D->GetDeviceContext());

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}