/*
	This file should be #included by all shaders
*/

// Version Information
//====================

#if defined( GE3D_PLATFORM_GL )

	// GLSL shaders require the version to be #defined before anything else in the shader
	#version 420

#endif

#if defined( GE3D_PLATFORM_D3D )

	#define DeclareConstantBuffer( i_name, i_id ) cbuffer i_name : register( b##i_id )
	#define DeclareDrawCallConstantBuffer( i_name, i_id ) cbuffer i_name : register( b##i_id )
	#define mat4 float4x4
	#define vec2 float2 
	#define vec3 float3
	#define vec4 float4
	#define MatrixMultiplication( i_matrix_a, i_matrix_b ) mul(i_matrix_a, i_matrix_b)
	#define WriteVertexOutput(i_matrix, out_matrix) out_matrix = i_matrix;
	#define WriteVertexColor(i_color, out_color) out_color = i_color;

#elif defined( GE3D_PLATFORM_GL )

	#define DeclareConstantBuffer( i_name, i_id ) layout( std140, binding = i_id ) uniform i_name
	#define DeclareDrawCallConstantBuffer( i_name, i_id ) layout( std140, binding = i_id ) uniform i_name
	#define float4x4 mat4
	#define float2 vec2
	#define float3 vec3
	#define float4 vec4
	#define MatrixMultiplication( i_matrix_a, i_matrix_b ) i_matrix_a * i_matrix_b
	#define WriteVertexOutput(i_matrix, out_matrix) gl_Position = i_matrix;
	#define WriteVertexColor(i_color, out_color) out_color = i_color;

#endif

// Constant Buffers
//=================

DeclareConstantBuffer( g_constantBuffer_frame, 0 )
{
	float4x4 g_transform_worldToCamera;
	float4x4 g_transform_cameraToProjected;

	float g_elapsedSecondCount_systemTime;
	float g_elapsedSecondCount_simulationTime;
	// For float4 alignment
	float2 g_padding;
};

DeclareDrawCallConstantBuffer( g_constantBuffer_drawCall, 2 )
{
	float4x4 g_transform_localToWorld;
};
