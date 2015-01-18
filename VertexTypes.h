////////////////////////////////////////////////////////////////////////////////
// Filename: VertexTypes.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _VERTEX_TYPES_H_
#define _VERTEX_TYPES_H_

//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>

////////////////////////////////////////////////////////////////////////////////
// Structures defining vertex types
////////////////////////////////////////////////////////////////////////////////
struct VertexType
{
	D3DXVECTOR3 position;
    D3DXVECTOR4 color;
};

struct VertexNorm
{
    D3DXVECTOR3 position;
    D3DXVECTOR3 normal;

    VertexNorm(){
    };
    VertexNorm(FLOAT _x, FLOAT _y, FLOAT _z)
    {
        position = D3DXVECTOR3(_x, _y, _z);
    }
    //D3DXVECTOR4 color;
};

struct MaterialBufferType
{
	D3DXVECTOR4 ambientColor;
	D3DXVECTOR4 diffuseColor;
	D3DXVECTOR4 specularColor;
};

#endif