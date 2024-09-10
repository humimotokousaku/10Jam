#pragma once
#include <string>
#include <sstream>
#include <fstream>

class ActionManager
{
public:
	std::stringstream popCommands_;
	void Initialize();
	void LoadActionData();
	void UpdateActionData(int i);


};
