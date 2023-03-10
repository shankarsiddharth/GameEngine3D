/*
	This is the standard fragment shader

	A fragment shader is responsible for telling the GPU what color a specific fragment should be
*/

#include <Shaders/shaders.inc>

#if defined( EAE6320_PLATFORM_D3D )

// Entry Point
//============

void main(

	// Input
	//======

	in const float4 i_fragmentPosition : SV_POSITION,
	in const float4 i_color : COLOR,
	// Output
	//=======

	// Whatever color value is output from the fragment shader
	// will determine the color of the corresponding pixel on the screen
	out float4 o_color : SV_TARGET

)
// {
// 	// Output solid white
// 	o_color = float4(
// 		// RGB (color)
// 		1.0, 1.0, 1.0,
// 		// Alpha (opacity)
// 		1.0 );
// }

#elif defined( EAE6320_PLATFORM_GL )

// Input
//=======

layout( location = 1 ) in vec4 i_color;

// Output
//=======

// Whatever color value is output from the fragment shader
// will determine the color of the corresponding pixel on the screen
out vec4 o_color;

// Entry Point
//============

void main()
// {
// 	// Output solid white
// 	o_color = vec4(
// 		// RGB (color)
// 		1.0, 1.0, 1.0,
// 		// Alpha (opacity)
// 		1.0 );
// }

#endif

{
	// Output solid white
	//o_color = float4(
		// RGB (color)
	//	1.0, 1.0, 1.0,
		// Alpha (opacity)
	//	1.0 );
	WriteVertexColor(i_color, o_color)
}
