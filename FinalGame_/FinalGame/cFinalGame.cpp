// Includes
//=========

#include "cFinalGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/Logging/Logging.h>
#include <Engine/StoryNarrator/Includes.h>
#include <External/imgui/Includes.h>
#include <random>

// Inherited Implementation
//=========================

void eae6320::cFinalGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime,
	const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{

}

void eae6320::cFinalGame::UpdateSimulationBasedOnInput()
{
	if (UserInput::IsKeyPressed(eae6320::UserInput::KeyCodes::Space))
	{
		show_demo_window = false;
	}
	else
	{
		show_demo_window = true;
	}
}

void eae6320::cFinalGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{

}

void eae6320::cFinalGame::UpdateBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{

}

// Run
//----

void eae6320::cFinalGame::UpdateBasedOnInput()
{
	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{
		// Exit the application
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}
}

// Initialize / Clean Up
//----------------------

eae6320::cResult eae6320::cFinalGame::Initialize()
{
	RegisterOnImGuiRenderUI(std::bind(&cFinalGame::RenderUI, this));

	InitializeStory();

	return Results::Success;
}

eae6320::cResult eae6320::cFinalGame::CleanUp()
{
	return Results::Success;
}

eae6320::cResult eae6320::cFinalGame::InitializeStory()
{
	auto result = eae6320::Results::Success;

	Narrator::Runtime::Story story;
	story.FromJSONFile("data/stories/test_story.storyasset");

	while (story.canRead())
	{
		while (story.canRead())
		{
			std::string dialogue = story.Read();
			if (!dialogue.empty())
			{
				story_string.append("\n");
				story_string.append(dialogue);
				eae6320::Logging::OutputMessage(dialogue.c_str());
				// 				std::cout << dialogue << std::endl;
			}
		}

		std::vector<std::string> choices = story.GetChoices();
		size_t choiceCount = choices.size();
		if (choiceCount > 0)
		{
			size_t choiceIndex = 0;
			for (const std::string& choiceText : choices)
			{
				const std::string message = "Choice Index: " + std::to_string(choiceIndex) + std::string("\t") + choiceText;
				// 				std::cout << message << std::endl;
				story_string.append("\n");
				story_string.append(message);
				eae6320::Logging::OutputMessage(message.c_str());
				choiceIndex++;
			}

			std::random_device rd;  //Will be used to obtain a seed for the random number engine
			std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
			std::uniform_int_distribution<size_t> distrib(0, (choiceCount - 1));

			auto selectChoiceIndex = distrib(gen);
			const std::string message = "Selecting Choice Index: " + std::to_string(selectChoiceIndex);
			// 			std::cout << message << std::endl;
			story_string.append("\n");
			story_string.append(message);
			eae6320::Logging::OutputMessage(message.c_str());
			story.SelectChoice((std::uint32_t)(selectChoiceIndex));
		}
		else
		{
			//End of Story
			break;
		}
	}

	return result;
}

void eae6320::cFinalGame::GetDefaultInitialResolution(uint16_t& o_width, uint16_t& o_height) const
{
	o_width = 900;
	o_height = 900;
}

void eae6320::cFinalGame::RenderUI()
{
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);
		ImGui::Text("%s", story_string.c_str());

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}
