//-----------------------------------------------------------------------------
//  Renderer2D Text Shader (vertex)
//-----------------------------------------------------------------------------
#type vertex
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


//-----------------------------------------------------------------------------
//  Renderer2D Text Shader (fragment)
//-----------------------------------------------------------------------------
#type fragment
#version 450 core

struct VertexOutput
{
	vec4  Color;
	vec2  TexCoord;
	vec2  TexSize;
	float TexIndex;
};
layout(location = 0) in VertexOutput Input;

layout(binding = 0) uniform sampler2D u_Textures[32];

layout(location = 0) out vec4 o_Color;


//uniform float u_pxRange; // set to distance field's pixel range
float u_pxRange = 8;

float pxRange = u_pxRange * 0.6;
// ^^ this should be unchanged but then the glyps have no smoth edges
//
// better results for small fonts -> nearer we get to half of distance field's pixel range
// but the glyps are more transparent then
// still not optimal!!!


float median(float r, float g, float b) {
	return max(min(r, g), min(max(r, g), b));
}


float screenPxRange() {
	vec2 unitRange = vec2(pxRange) / Input.TexSize;
	vec2 screenTexSize = vec2(1.0) / fwidth(Input.TexCoord);
	return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}


void main()
{
	// TODO: add supersampling for small fonts ???
	//       but how ???

	vec3 msd = texture(u_Textures[int(Input.TexIndex)], Input.TexCoord).rgb;
	float sd = median(msd.r, msd.g, msd.b);
	float screenPxDistance = screenPxRange() * (sd - 0.5);
	float opacity = clamp(screenPxDistance + 0.5, 0.0, 1.0);

	if (opacity < 0.0)
		discard;

//	vec4 bgColor = vec4(0.0);
//	vec4 fgColor = Input.Color;

	/* DEBUG  screenPxRange() must never be lower than 1. If it is lower than 2,   */
	/* DEBUG  there is a high probability that the anti-aliasing will fail         */
	//	if (screenPxRange() < 2.0)
	//		bgColor = vec4(1.0, 1.0, 0.0, 1.0);
	//	if (screenPxRange() <= 1.0)
	//		bgColor = vec4(1.0, 0.0, 0.0, 1.0);

//	o_Color = mix(bgColor, fgColor, opacity);
	o_Color = Input.Color;
	o_Color.a = opacity;
}
