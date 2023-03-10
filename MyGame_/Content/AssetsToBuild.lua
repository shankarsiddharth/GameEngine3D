--[[
	This file lists every asset that must be built by the AssetBuildSystem
]]

return
{
	shaders =
	{
		{ path = "Shaders/Vertex/standard.shader", arguments = { "vertex" } },
		{ path = "Shaders/Fragment/standard.shader", arguments = { "fragment" } },
		{ path = "Shaders/Fragment/testsample.shader", arguments = { "fragment" } },
		{ path = "Shaders/Vertex/vertexInputLayout_mesh.shader", arguments = { "vertex" } },
	},
	meshes =
	{
		"Meshes/plane.json",
		"Meshes/sphere.json",
		"Meshes/sphere_large.json",
		"Meshes/sonic.json",
		"Meshes/helix.json",
		"Meshes/newMesh.json",
		"Meshes/secondMesh.json",
		"Meshes/thirdMesh.json",
	},
	stories =
	{
		"Stories/test_story.txt",
	},
}
