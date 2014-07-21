// constant buffer
cbuffer MatrixBuffer : register(b0)
{
	matrix WVP : packoffset(c0)
};

// input, output structure
struct VS_INPUT
{
    float4 position : POSITION;
	flaot3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    flaot3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
VS_OUTPUT VS_Main(VS_INPUT input)
{
    VS_OUTPUT output;

    // Calculate the position.
    output.position = mul(input.position, WVP);
    
    // Calcualte the normal.
	output.normal = mul(input.normal, (float3x3)WVP);

	// texture coord
	output.texcoord = input.texcoord;
    
    return output;
}