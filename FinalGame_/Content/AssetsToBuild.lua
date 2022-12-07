--[[
	This file lists every asset that must be built by the AssetBuildSystem
]]

return
{
	shaders =
	{
		{ path = "Shaders/Vertex/standard.shader", arguments = { "vertex" } },
		{ path = "Shaders/Fragment/standard.shader", arguments = { "fragment" } },

		{ path = "Shaders/Vertex/vertexInputLayout_mesh.shader", arguments = { "vertex" } },
	},
	stories =
	{
		"Stories/Vikram_Vedha.txt",
	},
	audios =
	{
		{ path = "Audios/BG.mp3"},
		{ path = "Audios/Test.mp3"},
	},
	fonts =
	{
		{ path = "Fonts/Lato-Light.ttf"},
	},
}
