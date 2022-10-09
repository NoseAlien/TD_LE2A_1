#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <memory>

class StageSelect
{
private:
	std::vector<Model*> stageSelectModel;
	std::vector<std::unique_ptr<WorldTransform>> stageSelectTrans;
	int stagesSize;
	int currentStage;
	bool isFront;
	bool isBack;
	float moveSpeed;

public:
	StageSelect();
	~StageSelect();
	void Initialize(int stagesSize);
	void Update();
	void Draw();
	void ResetViewPos();

	inline int GetCurrentStage() { return currentStage; }
};

