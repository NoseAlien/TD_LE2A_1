#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <memory>

class StageSelect
{
private:
	std::vector<Model*> stageSelectModel;
	std::vector<std::unique_ptr<WorldTransform>> stageSelectTrans;
	std::vector<Model*> stageTextModel;
	std::vector<std::unique_ptr<WorldTransform>> stageTextTrans;

	std::unique_ptr<WorldTransform> parentTrans;
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
	inline Vector3 GetSelectPos(const int& number) { return stageSelectTrans[number]->translation_; }
	inline Vector3 GetSelectScale(const int& number) { return stageSelectTrans[number]->scale_; }
	inline Vector3 GetTextPos(const int& number) { return stageTextTrans[number]->translation_; }
	inline Vector3 GetTextScale(const int& number) { return stageTextTrans[number]->scale_; }

	inline void SetSelectPos(const Vector3& pos, const int& number) { stageSelectTrans[number]->translation_ = pos; }
	inline void SetSelectScale(const Vector3& scale, const int& number) { stageSelectTrans[number]->scale_ = scale; }
	inline void SetTextPos(const Vector3& pos, const int& number) { stageTextTrans[number]->translation_ = pos; }
	inline void SetTextScale(const Vector3& scale, const int& number) { stageTextTrans[number]->scale_ = scale; }
};

