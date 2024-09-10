#include "ActionManager.h"
#include <cassert>
#include <algorithm>

void ActionManager::LoadActionData()
{
	std::stringstream popCommands;
	// ファイルを開く
	std::ifstream file;
	file.open("Engine/resources/LevelCSV/ActionTable.csv");
	assert(file.is_open());
	// ファイル内容を文字列に
	popCommands << file.rdbuf();
	// 閉じる
	file.close();

	// 1桁分
	std::string line;
	while (std::getline(popCommands, line))
	{
		// 1桁の文字列を解析しやすく
		std::istringstream line_stream(line);
		// 
		std::string word;
		// 
		std::getline(line_stream, word, ',');
		// '//'はスキップ
		if (word.find("//") == 0) {
			continue;
		}
		if (word.find("DATA") == 0) {
			//
			EnemyActionData data{};
			std::getline(line_stream, word, ',');
			data.direct.x = (float)std::atof(word.c_str());
			std::getline(line_stream, word, ',');
			data.direct.z = (float)std::atof(word.c_str());
			std::getline(line_stream, word, ',');
			data.power = (float)std::atof(word.c_str());
			std::getline(line_stream, word, ',');
			data.time = std::atoi(word.c_str());
			actionContainer_.push_back(data);
		}
	}
	SortByTimeDescending();
}

void ActionManager::SortByTimeDescending()
{
	std::sort(actionContainer_.begin(), actionContainer_.end(),
		[](const EnemyActionData& a, const EnemyActionData& b) {
			return a.time > b.time;
		});
}