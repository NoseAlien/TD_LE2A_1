#include "StageSelect.h"
#include "GameScene.h"
#include "Input.h"
#include "Player.h"
using namespace std;

StageSelect::StageSelect() :
	stagesSize(0), currentStage(0), moveSpeed(0.13)
{
}

StageSelect::~StageSelect()
{
	for (int i = 0; i < stageSelectModel.size(); i++)
	{
		delete stageSelectModel[i];
		delete stageTextModel[i];
	}
}

static float angle = 0;
void StageSelect::Initialize(int stagesSize)
{
	this->stagesSize = stagesSize;
	angle = 360 / this->stagesSize;

	for (int i = 0; i < this->stagesSize; i++)
	{
		stageSelectModel.emplace_back(Model::Create());
		stageSelectTrans.emplace_back(move(make_unique<WorldTransform>()));
		stageSelectTrans.back()->Initialize();
		stageSelectTrans.back()->translation_ = { (float)i * 25,0,0 };
		stageSelectTrans.back()->scale_ = { 5,5,1 };
		stageSelectTrans.back()->UpdateMatrix();

		stageTextModel.emplace_back(Model::Create());
		stageTextTrans.emplace_back(move(make_unique<WorldTransform>()));
		stageTextTrans.back()->Initialize();
		stageTextTrans.back()->translation_ = { (float)i * 25,stageSelectTrans.back()->scale_.y,0 };
		stageTextTrans.back()->scale_ = { 5.5,1.5,1.5 };
		stageTextTrans.back()->UpdateMatrix();
	}
}

float moveAngle = 0;
int index = 0;
void StageSelect::Update()
{
	auto input = Input::GetInstance();

	if (input->TriggerKey(DIK_RIGHT))
	{
		currentStage++;
	}
	if (input->TriggerKey(DIK_LEFT))
	{
		currentStage--;
	}

	currentStage = min(max(currentStage,0), stagesSize - 1);

	for (int i = 0; i < stagesSize; i++)
	{
		float moveX = (((i - currentStage) * 25) - stageSelectTrans[i]->translation_.x) * moveSpeed;
		stageSelectTrans[i]->translation_.x += moveX;
		stageTextTrans[i]->translation_.x += moveX;
	}

	for (int i = 0; i < stagesSize; i++)
	{
		stageSelectTrans[i]->UpdateMatrix();
		stageTextTrans[i]->UpdateMatrix();
	}

	viewProjection_.UpdateMatrix();
}

void StageSelect::Draw()
{
	for (int i = 0; i < stagesSize; i++)
	{
		stageSelectModel[i]->Draw(*stageSelectTrans[i], viewProjection_);
		stageTextModel[i]->Draw(*stageTextTrans[i], viewProjection_);
	}
}

void StageSelect::ResetObjPos()
{
	//for (int i = 0; i < this->stagesSize; i++)
	//{
	//	stageSelectTrans[i]->Initialize();
	//	stageSelectTrans[i]->translation_ = { (float)i * 25,0,0 };
	//	stageSelectTrans[i]->scale_ = { 5,5,1 };

	//	stageTextTrans[i]->Initialize();
	//	stageTextTrans[i]->translation_ = { (float)i * 25,stageSelectTrans.back()->scale_.y,0 };
	//	stageTextTrans[i]->scale_ = { 5.5,1.5,1.5 };
	//}

	for (int i = 0; i < stagesSize; i++)
	{
		stageSelectTrans[i]->translation_.x =
			stageSelectTrans[currentStage]->translation_.x -
			(currentStage - i) * 25;
		stageTextTrans[i]->translation_.x =
			stageTextTrans[currentStage]->translation_.x -
			(currentStage - i) * 25;

		stageSelectTrans[i]->UpdateMatrix();
		stageTextTrans[i]->UpdateMatrix();

	}
}
