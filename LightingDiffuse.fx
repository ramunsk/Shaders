/*
    Lighting Shader
*/

cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};
/*
cbuffer CameraBuffer
{
    float3 cameraPos;
};
*/
cbuffer MaterialBuffer
{
	float4 ambientMaterColor;
	float4 diffuseMaterColor;
    float4 specularMaterColor;
};

struct VS_Input
{
    float4 pos  : POSITION;
    float3 norm : NORMAL;
};

struct PS_Input
{
    float4 pos  : SV_POSITION;
	float1 difterm : TEXCOORD1;
};


PS_Input VS_Main( VS_Input vertex )
{
    PS_Input vsOut = ( PS_Input )0;
    float4 worldPos = mul( vertex.pos, worldMatrix );
    vsOut.pos = mul( worldPos, viewMatrix );
    vsOut.pos = mul( vsOut.pos, projectionMatrix );

	// Transform the normal for light computations
    float3 norm = mul( vertex.norm, (float3x3)worldMatrix );
    norm = normalize( norm );

	// Prepare lighting vector for interpolation per pixel
    float3 lightPos = float3( 0.0f, 50.0f, -30.0f );
    float3 lightVec = normalize( lightPos - worldPos );
  
	// Compute diffusive term (interpolation per vertex)
    vsOut.difterm = clamp( dot( norm, lightVec ), 0.0f, 1.0f );

    return vsOut;
}


float4 PS_Main( PS_Input frag ) : SV_TARGET
{
    float3 lightColor = float3( 0.7f, 0.7f, 0.7f );

	// Final lighting formula
	//float3 finalColor = diffuseMaterColor * lightColor * frag.difterm;
	float3 finalColor = ambientMaterColor + diffuseMaterColor * lightColor * frag.difterm;

    return float4( finalColor, 1.0f );
} 