Projects:

StoryNarrator
 - Static Lib
StoryBuilder
 - Exe => Converts the Document to JSON format
StoryNarratorCommandLine
 - Exe => cmd utility
===========================================
Text File to Create the Story with some syntax

===========================================
Tokens / Special Characters:
----------------------------
Sections/Knots: === section_name ===
Choices: * <choice_text>
Redirections/Diverts: -> <divert_name>
EndOfTheDocument:	->END 
					->DONE

===========================================
Parsing:
----------------------------
Line by Line:

	->End
	is different from 
	->
	End
----------------------------

Directed Graph (Cyclic / Acyclic)
Nodes & Links

Links:
	UniIn
	UniOut
	MultiIn
	MultiOut

Nodes:
StartNode
EndNode
KnotNode
DivertNode
DecisionNode
ChoiceNode
DialogueNode

=============================================
Compiling:
----------------------------
Converts to JSON format

=============================================
Runtime:
----------------------------
Load A StoryJSON / StoryAsset

 - canRead()
 - Read()
 - GetChoices()
 - SelectChoice()
 
 - End of the Documents
	cannot read & choice count is 0

=============================================
Command Line Utility:
----------------------------
 - Parse the Document
 - Converts to JSON
 - PlayMode

=============================================
Things to consider

No UI
Cycles -> GraphCycles

