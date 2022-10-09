#include "StageSelect.h"
#include "GameScene.h"
#include "Input.h"
using namespace std;

StageSelect::StageSelect() :
	stagesSize(0), currentStage(0), moveSpeed(2),
	isFront(false), isBack(false)

{
}

StageSelect::~StageSelect()
{
	for (int i = 0; i < stageSelectModel.size(); i++)
	{
		delete stageSelectModel[i];
	}
}

void StageSelect::Initialize(int stagesSize)
{
	this->stagesSize = stagesSize;
	for (int i = 0; i < this->stagesSize; i++)
	{
		stageSelectModel.emplace_back(Model::Create());
		stageSelectTrans.emplace_back(move(make_unique<WorldTransform>()));
		stageSelectTrans.back()->Initialize();
		stageSelectTrans.back()->translation_ = { (float)i * 25,0,0 };
		stageSelectTrans.back()->scale_ = { 5,5,5 };
		stageSelectTrans.back()->rotation_ = { 0,0,DegreeToRad(45) };
		stageSelectTrans.back()->UpdateMatrix();
	}
}

void StageSelect::Update()
{
	auto input = Input::GetInstance();

	if (input->TriggerKey(DIK_RIGHT) && currentStage < stagesSize - 1)
	{
		if (isFront == false)
		{
			isFront = true;
			isBack = false;
			currentStage++;
		}
	}
	if (input->TriggerKey(DIK_LEFT) && currentStage > 0)
	{
		if (isBack == false)
		{
			isFront = false;
			isBack = true;
			currentStage--;
		}
	}

	if (isFront == true)
	{
		viewProjection_.eye.x += moveSpeed;
		viewProjection_.target.x += moveSpeed;
		if (viewProjection_.eye.x >= stageSelectTrans[currentStage]->translation_.x)
		{
			viewProjection_.eye.x = stageSelectTrans[currentStage]->translation_.x;
			isFront = false;
		}
	}

	if (isBack == true)
	{
		viewProjection_.eye.x -= moveSpeed;
		viewProjection_.target.x -= moveSpeed;
		if (viewProjection_.eye.x <= stageSelectTrans[currentStage]->translation_.x)
		{
			viewProjection_.eye.x = stageSelectTrans[currentStage]->translation_.x;
			isBack = false;
		}
	}

	viewProjection_.UpdateMatrix();
}

void StageSelect::Draw()
{
	for (int i = 0; i < stagesSize; i++)
	{
		auto tempTrans = stageSelectTrans[i].get();
		stageSelectModel[i]->Draw(*tempTrans, viewProjection_);
	}
}

void StageSelect::ResetViewPos()
{
	viewProjection_.eye.x = currentStage * 25;
	viewProjection_.target.x = currentStage * 25;
}
