#include "ActionManager.h"
#include <cassert>

void ActionManager::LoadActionData()
{
	// ファイルを開く
	std::ifstream file;
	file.open("Engine/resources/LevelCSV/ActionTable.csv");
	assert(file.is_open());

	// ファイル内容を文字列に
	popCommands_ << file.rdbuf();

	// 閉じる
	file.close();
}

void ActionManager::UpdateActionData(int i)
{
	// 1桁分
	std::string line;

	while (std::getline(popCommands_,line))
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
		if (word.find("WAITTIME") == 0) {
			// 
			std::getline(line_stream, word, ',');

			
		}

	}

}
