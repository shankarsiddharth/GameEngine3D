/*
	This is the standard vertex shader

	A vertex shader is responsible for two things:
		* Telling the GPU where the vertex (one of the three in a triangle) should be drawn on screen in a given window
			* The GPU will use this to decide which fragments (i.e. pixels) need to be shaded for a given triangle
		* Providing any data that a corresponding fragment shader will need
			* This data will be interpolated across the triangle and thus vary for each fragment of a triangle that gets shaded
*/

#include <Shaders/shaders.inc>

#if defined( EAE6320_PLATFORM_D3D )

// Entry Point
//============

void main(

	// Input
	//======

	// The "semantics" (the keywords in all caps after the colon) are arbitrary,
	// but must match the C call to CreateInputLayout()

	// These values come from one of the VertexFormats::sVertex_mesh that the vertex buffer was filled with in C code
	in const float3 i_vertexPosition_local : POSITION,
	in const float4 i_color : COLOR,
	// Output
	//=======

	// An SV_POSITION value must always be output from every vertex shader
	// so that the GPU can figure out which fragments need to be shaded
	out float4 o_vertexPosition_projected : SV_POSITION,
	out float4 o_color : COLOR
)
// {
// 	// Transform the local vertex into world space
// 	float4 vertexPosition_world;
// 	{
// 		// This will be done in a future assignment.
// 		// For now, however, local space is treated as if it is the same as world space.
// 		float4 vertexPosition_local = float4( i_vertexPosition_local, 1.0 );
// 		vertexPosition_world = vertexPosition_local;
// 	}
// 	// Calculate the position of this vertex projected onto the display
// 	{
// 		// Transform the vertex from world space into camera space
// 		float4 vertexPosition_camera = mul( g_transform_worldToCamera, vertexPosition_world );
// 		// Project the vertex from camera space into projected space
// 		o_vertexPosition_projected = mul( g_transform_cameraToProjected, vertexPosition_camera );
// 	}
// }

#elif defined( EAE6320_PLATFORM_GL )

// Input
//======

// The locations assigned are arbitrary
// but must match the C calls to glVertexAttribPointer()

// These values come from one of the VertexFormats::sVertex_mesh that the vertex buffer was filled with in C code
layout( location = 0 ) in vec3 i_vertexPosition_local;
layout( location = 1 ) in vec4 i_color;
// Output
//=======

// The vertex shader must always output a position value,
// but unlike HLSL where the value is explicit
// GLSL has an automatically-required variable named "gl_Position"
layout( location = 1 ) out vec4 o_color;

// Entry Point
//============

void main()
// {
// 	// Transform the local vertex into world space
// 	vec4 vertexPosition_world;
// 	{
// 		// This will be done in a future assignment.
// 		// For now, however, local space is treated as if it is the same as world space.
// 		vec4 vertexPosition_local = vec4( i_vertexPosition_local, 1.0 );
// 		vertexPosition_world = vertexPosition_local;
// 	}
// 	// Calculate the position of this vertex projected onto the display
// 	{
// 		// Transform the vertex from world space into camera space
// 		vec4 vertexPosition_camera = g_transform_worldToCamera * vertexPosition_world;
// 		// Project the vertex from camera space into projected space
// 		gl_Position = g_transform_cameraToProjected * vertexPosition_camera;
// 	}
// }

#endif

{
	// Transform the local vertex into world space
	float4 vertexPosition_world;
	{
		// This will be done in a future assignment.
		// For now, however, local space is treated as if it is the same as world space.
		float4 vertexPosition_local = float4( i_vertexPosition_local, 1.0 );
		vertexPosition_world = MatrixMultiplication( g_transform_localToWorld , vertexPosition_local );
	}
	// Calculate the position of this vertex projected onto the display
	{
		// Transform the vertex from world space into camera space
		float4 vertexPosition_camera = MatrixMultiplication( g_transform_worldToCamera, vertexPosition_world );
		// Project the vertex from camera space into projected space
		float4 vertexPosition_projected = MatrixMultiplication( g_transform_cameraToProjected, vertexPosition_camera );
		WriteVertexOutput(vertexPosition_projected, o_vertexPosition_projected)
		WriteVertexColor(i_color, o_color)
	}
}
