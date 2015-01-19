////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "transformationclass.h"
#include "sphere_modelclass.h"
#include "ArrowModelClass.h"
#include "plane_modelclass.h"
#include "lighting_shaderclass.h"


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();	

private:
	D3DClass* m_D3D;
	TransformationClass* m_Transform;
	SphereModelClass* m_Sphere;
	IndexModelClass* m_Model;
	PlaneModelClass* m_PlaneModel;
	LightingShaderClass* m_LightingShader;
};

#endif