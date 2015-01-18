////////////////////////////////////////////////////////////////////////////////
// Filename: ArrowModelClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _ARROW_MODELCLASS_H_
#define _ARROW_MODELCLASS_H_

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10math.h>

#include "indexmodelclass.h"

////////////////////////////////////////////////////////////////////////////////
// Class name: ArrowModelClass
////////////////////////////////////////////////////////////////////////////////
class ArrowModelClass : public IndexModelClass
{

public:
    ArrowModelClass();
    ArrowModelClass(const ArrowModelClass&);
    ~ArrowModelClass();

    void Render(ID3D11DeviceContext*);
    MaterialBufferType getMaterial();

private:
    bool InitializeBuffers(ID3D11Device*);
    void RenderBuffers(ID3D11DeviceContext*);

private:
    MaterialBufferType m_Material;
    D3DXMATRIX _staticRotation;
    D3DXMATRIX _staticScale;

};

#endif