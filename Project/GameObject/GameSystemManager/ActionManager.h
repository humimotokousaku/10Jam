#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <stdint.h>
#include <vector>
#include "Vector3.h"

class ActionManager
{
public:
	// アクションのコンテナ
	struct EnemyActionData {
		Vector3 direct;
		float power;
		int32_t time;
	};
	void LoadActionData();
	std::vector<EnemyActionData>* GetContainer() { return &actionContainer_; }
	// 
	std::vector<EnemyActionData> actionContainer_;

	// 降順ソート
	void SortByTimeDescending();
};
