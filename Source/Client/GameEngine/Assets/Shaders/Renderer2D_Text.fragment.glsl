//-----------------------------------------------------------------------------
//  Renderer2D Text Shader (fragment)
//-----------------------------------------------------------------------------
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
	if (opacity < 0.1)
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
	o_Color = vec4(Input.Color.rgb, opacity);
}
