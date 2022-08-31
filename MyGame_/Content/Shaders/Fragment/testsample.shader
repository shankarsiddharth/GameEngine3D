/*
	This is the standard fragment shader

	A fragment shader is responsible for telling the GPU what color a specific fragment should be
*/

#include <Shaders/shaders.inc>

#if defined( EAE6320_PLATFORM_D3D )

// Constant Buffers
//=================

cbuffer g_constantBuffer_frame : register( b0 )
{
	float4x4 g_transform_worldToCamera;
	float4x4 g_transform_cameraToProjected;

	float g_elapsedSecondCount_systemTime;
	float g_elapsedSecondCount_simulationTime;
	// For float4 alignment
	float2 g_padding;
};

// Entry Point
//============

void main(

	// Input
	//======

	in const float4 i_fragmentPosition : SV_POSITION,

	// Output
	//=======

	// Whatever color value is output from the fragment shader
	// will determine the color of the corresponding pixel on the screen
	out float4 o_color : SV_TARGET

)
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
		float new_color_b = ((sin_value + 1.0) / 2.0);
		
		o_color = float4(new_color_r, new_color_g, new_color_b, 1.0);
}

#elif defined( EAE6320_PLATFORM_GL )

// Constant Buffers
//=================

layout( std140, binding = 0 ) uniform g_constantBuffer_frame
{
	mat4 g_transform_worldToCamera;
	mat4 g_transform_cameraToProjected;

	float g_elapsedSecondCount_systemTime;
	float g_elapsedSecondCount_simulationTime;
	// For vec4 alignment
	vec2 g_padding;
};

// Output
//=======

// Whatever color value is output from the fragment shader
// will determine the color of the corresponding pixel on the screen
out vec4 o_color;

// Entry Point
//============

void main()
{
	// Output solid white
	o_color = vec4(
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
	float new_color_b = ((sin_value + 1.0) / 2.0);
	
	o_color = vec4(new_color_r, new_color_g, new_color_b, 1.0);
}

#endif
