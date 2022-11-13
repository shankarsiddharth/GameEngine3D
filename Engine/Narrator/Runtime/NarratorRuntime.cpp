// NarratorRuntime.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "Story/Story.h"



int main()
{
/*

	Narrator::Runtime::Story story = Narrator::Runtime::Story::Parse("./TestScripts/story.txt");

*/
	Narrator::Runtime::Story story;
	story.FromJSONFile("./TestScripts/story_test.json");
}
