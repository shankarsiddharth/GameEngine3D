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
		{ path = "Audios/bg.mp3"},
		{ path = "Audios/select_choice.wav"},
	},
	fonts =
	{
		{ path = "Fonts/Lato-Light.ttf"},
	},
}
