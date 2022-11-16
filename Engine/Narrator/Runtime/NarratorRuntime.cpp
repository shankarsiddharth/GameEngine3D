// NarratorRuntime.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "Story/Story.h"
#include <iostream>
#include <random>

int main()
{

	Narrator::Runtime::Story::Parse("./TestScripts/story_test.txt");

/*

	Narrator::Runtime::Story story;
	story.FromJSONFile("./TestScripts/story_test.json");

	while (story.canRead())
	{
		while (story.canRead())
		{
			std::string dialogue = story.Read();
			if (!dialogue.empty())
			{
				std::cout << dialogue << std::endl;
			}
		}

		std::vector<std::string> choices = story.GetChoices();
		size_t choiceCount = choices.size();
		if (choiceCount > 0)
		{
			size_t choiceIndex = 0;
			for (const std::string& choiceText : choices)
			{
				std::cout << "Choice Index: " << choiceIndex << "\t" << choiceText << std::endl;
				choiceIndex++;
			}

			std::random_device rd;  //Will be used to obtain a seed for the random number engine
			std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
			std::uniform_int_distribution<> distrib(0, (choiceCount - 1));

			auto selectChoiceIndex = distrib(gen);
			std::cout << "Selecting Choice Index: " << selectChoiceIndex << std::endl;
			story.SelectChoice(selectChoiceIndex);
// 			story.SelectChoice(0);
		}
		else
		{
			//End of Story
			break;
		}
	}

	story.Read();
*/

	return 0;
}
