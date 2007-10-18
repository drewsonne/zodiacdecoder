// Vertex shader input structure
struct VS_INPUT
{
    float4 Position   : POSITION;
    float4 Color    : 	COLOR0;
	float2 Texture1		:	TEXCOORD0;
};


// Vertex shader output structure
struct VS_OUTPUT
{
    float4 Position   : POSITION;
    float4 Color    : 	COLOR0;
	float2 Texture1		:	TEXCOORD0;
};


// Global variables
float4x4 WorldViewProj;
float4 globalColor;

// Name: Simple Vertex Shader
// Type: Vertex shader
// Desc: Vertex transformation and texture coord pass-through
//
VS_OUTPUT main( in VS_INPUT In )
{
    VS_OUTPUT Out;                      //create an output vertex

    Out.Position = mul(In.Position,
                       WorldViewProj);  //apply vertex transformation
    Out.Color  = In.Color;          //copy original texcoords
	Out.Color = globalColor;
	Out.Texture1 = In.Texture1;

    return Out;                         //return output vertex
}