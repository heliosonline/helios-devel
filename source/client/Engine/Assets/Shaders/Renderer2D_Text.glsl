//-----------------------------------------------------------------------------
//  Renderer2D Text Shader (vertex)
//-----------------------------------------------------------------------------
#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;

//layout(std140, binding = 0) uniform Camera
//{
//	mat4 u_ViewProjection;
//};
uniform mat4 u_ViewProjection;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
};

layout(location = 0) out VertexOutput Output;
layout(location = 3) out flat float v_TexIndex;


void main()
{
	Output.Color = a_Color;
	Output.TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;

	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}


//-----------------------------------------------------------------------------
//  Renderer2D Text Shader (fragment)
//-----------------------------------------------------------------------------
#type fragment
#version 450 core

layout(location = 0) out vec4 o_Color;

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
};

layout(location = 0) in VertexOutput Input;
layout(location = 3) in flat float v_TexIndex;

//uniform float pxRange; // set to distance field's pixel range
float pxRange = 2.0; // set to distance field's pixel range

layout(binding = 0) uniform sampler2D u_Textures[32];

vec4 bgColor = vec4(0.0);
int texIndex = int(v_TexIndex);


float median(float r, float g, float b) {
	return max(min(r, g), min(max(r, g), b));
}


float screenPxRange() {
	// TODO: get the correct texture size of the glyph (by uniform? / extra vertex field?)
	vec2 unitRange = vec2(pxRange) / vec2(textureSize(u_Textures[texIndex], 0));
	vec2 screenTexSize = vec2(1.0) / fwidth(Input.TexCoord);
	return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}


void main()
{
	// TODO: add supersampling

	vec3 msd = texture(u_Textures[texIndex], Input.TexCoord).rgb;
	float sd = median(msd.r, msd.g, msd.b);
	float screenPxDistance = screenPxRange() * (sd - 0.5);
	float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);
	if (opacity < 0.05)
		discard;
	o_Color = mix(bgColor, Input.Color, opacity);
}
