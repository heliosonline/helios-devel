//-----------------------------------------------------------------------------
//  Renderer2D Text Shader (vertex)
//-----------------------------------------------------------------------------
#version 450 core

layout(location = 0) in vec3  a_Position;
layout(location = 1) in vec4  a_Color;
layout(location = 2) in vec2  a_TexCoord;
layout(location = 3) in vec2  a_TexSize;
layout(location = 4) in float a_TexIndex;

//layout(std140, binding = 0) uniform Camera
//{
//	mat4 u_ViewProjection;
//};
uniform mat4 u_ViewProjection;

struct VertexOutput
{
	vec4  Color;
	vec2  TexCoord;
	vec2  TexSize;
	float TexIndex;
};
layout(location = 0) out VertexOutput Output;


void main()
{
	Output.Color    = a_Color;
	Output.TexCoord = a_TexCoord;
	Output.TexSize  = a_TexSize;
	Output.TexIndex = a_TexIndex;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}
