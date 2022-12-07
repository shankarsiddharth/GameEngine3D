// Includes
//=========

#include "cFinalGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include <Engine/Logging/Logging.h>
#include <Engine/StoryNarrator/Includes.h>
#include <External/imgui/Includes.h>
#include <random>
#include <string>

// Inherited Implementation
//=========================

void eae6320::cFinalGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime,
	const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	m_BGAudio.SubmitAudioSource();
}

void eae6320::cFinalGame::UpdateSimulationBasedOnInput()
{
	/*if (UserInput::IsKeyPressed(eae6320::UserInput::KeyCodes::Enter))
	{
		ReadStory();
	}*/

	if (!m_BGAudio.IsPlaying())
	{
		m_BGAudio.Play();
	}

	/*if (UserInput::IsKeyPressed(eae6320::UserInput::KeyCodes::Space))
	{
		
	}*/
	
	if (m_enableScrollTrack)
	{
		m_enableScrollTrack = false;
	}
}

void eae6320::cFinalGame::ReadStory()
{
	if (story.canRead())
	{
		std::string dialogue = story.Read();
		if (dialogue.rfind("#", 0) == 0) 
		{ 
			//Skip lines that start with #
		}
		else
		{
			story_string.append("\n");
			story_string.append(dialogue);
		}
		
	}
	else
	{
		std::vector<std::string> choices = story.GetChoices();
		size_t choiceCount = choices.size();
		if (choiceCount > 0)
		{
			if (!m_displayChoices && !m_choiceSelected)
			{
				m_displayChoices = true;
			}

			if (!m_displayChoices && m_choiceSelected)
			{
				story_string.append("\n-----------------------------------------------------\n");
				story.SelectChoice((std::uint32_t)(m_selectedChoice));
				m_choiceSelected = false;
				ReadStory();
				m_enableScrollTrack = true;
			}
		}
		else
		{
			//End of Story
		}
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
	auto result = eae6320::Results::Success;

	RegisterOnImGuiRenderUI(std::bind(&cFinalGame::RenderUI, this));

	result = InitializeStory();

	result = InitializeAudio();

	result = InitializeUI();

	return result;
}

eae6320::cResult eae6320::cFinalGame::CleanUp()
{
	return Results::Success;
}

eae6320::cResult eae6320::cFinalGame::InitializeUI()
{
	auto result = eae6320::Results::Success;

	ImGuiIO& io = ImGui::GetIO();
	m_latoFont = io.Fonts->AddFontFromFileTTF("data/fonts/Lato-Light.ttf", 24.0f);

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;

	return result;
}

eae6320::cResult eae6320::cFinalGame::InitializeStory()
{
	auto result = eae6320::Results::Success;

	story.FromJSONFile("data/stories/vikram_vedha.storyasset");

	/*
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
	*/

	return result;
}

eae6320::cResult eae6320::cFinalGame::InitializeAudio()
{
	auto result = eae6320::Results::Success;
	
	m_BGAudio.CreateAudioData("data/audios/bg.mp3", "bg_audio", 150, true);

	return result;
}

void eae6320::cFinalGame::GetDefaultInitialResolution(uint16_t& o_width, uint16_t& o_height) const
{
	o_width = 900;
	o_height = 900;
}

void eae6320::cFinalGame::RenderUI()
{
	ImGui::PushFont(m_latoFont);
	
	{
		ImGui::Begin("Vikram Vedha", NULL, window_flags);

		if (m_showStartStory)
		{
			if (ImGui::Button("Start Story"))
			{
				m_showStartStory = false;
				ReadStory();				
			}
		}
		else
		{
			ReadStory();
		}
		
		ImGui::TextWrapped("%s", story_string.c_str());
		if (m_enableScrollTrack)
		{
			ImGui::SetScrollHereY(1.0f);
		}
		ImGui::Text("    ");
		ImGui::End();
	}	
	{
		if (m_displayChoices)
		{
			ImGui::Begin("Choices", NULL, window_flags);

			std::vector<std::string> choices = story.GetChoices();
			size_t choiceCount = choices.size();

			for (size_t choiceIndex = 0; choiceIndex < choiceCount; choiceIndex++)
			{
				const std::string& choiceText = choices[choiceIndex];
				if (ImGui::Button(choiceText.c_str()))
				{
					m_enableScrollTrack = true;
					m_selectedChoice = choiceIndex;
					m_choiceSelected = true;
					m_displayChoices = false;
					ReadStory();
				}
			}

			ImGui::End();
		}
	}
	ImGui::PopFont();
}
