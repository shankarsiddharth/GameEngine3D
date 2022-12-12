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

#endif

{
	// Output solid white
	o_color = float4(
		// RGB (color)
		1.0, 1.0, 1.0,
		// Alpha (opacity)
		1.0 );
		
		o_color.r = 0.0;
		
		float sin_value = sin( g_elapsedSecondCount_simulationTime );
		float cos_value = cos( g_elapsedSecondCount_simulationTime );
		
		//float new_color = clamp(value, 0.0, 1.0);
		
		float new_color_r = ((sin_value + 1.0) / 2.0);
		float new_color_g = ((cos_value + 1.0) / 2.0);
		float new_color_b = (((sin_value * cos_value) + 1.0) / 2.0);
		
		float4 calculatedColor = float4(new_color_r, new_color_g, new_color_b, 1.0);
		float4 combinedColor = calculatedColor * i_color;
		
		//o_color = float4(1.0, 0.0, 0.0, 1.0);
		o_color = combinedColor;
}
