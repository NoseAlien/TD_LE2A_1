#include "StageSelect.h"
#include "GameScene.h"
#include "Input.h"
using namespace std;

StageSelect::StageSelect() :
	stagesSize(0), currentStage(0), moveSpeed(2),
	isFront(false), isBack(false)
{
	parentTrans = move(make_unique<WorldTransform>());
	parentTrans->Initialize();
	parentTrans->translation_ = { 0,0,20 };
	parentTrans->scale_ = { 1,1,1 };
	parentTrans->rotation_ = { 0,0,0 };
	parentTrans->UpdateMatrix();
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
		float radian = DegreeToRad(i * (360 / this->stagesSize));
		int lenght = 20;

		stageSelectModel.emplace_back(Model::Create());
		stageSelectTrans.emplace_back(move(make_unique<WorldTransform>()));
		stageSelectTrans.back()->Initialize();
		stageSelectTrans.back()->parent_ = parentTrans.get();
		stageSelectTrans.back()->translation_ = { sinf(radian) * lenght,0,-cosf(radian) * lenght };
		stageSelectTrans.back()->scale_ = { 5,5,1 };
		stageSelectTrans.back()->rotation_ = { 0,-radian,0 };
		stageSelectTrans.back()->UpdateMatrix();

		stageTextModel.emplace_back(Model::Create());
		stageTextTrans.emplace_back(move(make_unique<WorldTransform>()));
		stageTextTrans.back()->Initialize();
		stageTextTrans.back()->parent_ = parentTrans.get();
		stageTextTrans.back()->translation_ =
		{
			sinf(radian) * lenght,
			stageSelectTrans.back()->scale_.y,
			-cosf(radian) * lenght
		};
		stageTextTrans.back()->scale_ = { 5.5,1.5,1.5 };
		stageTextTrans.back()->rotation_ = { 0,-radian,0 };
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
		if (isFront == false)
		{
			isFront = true;
			isBack = false;
			currentStage++;
			index++;
			if (currentStage > stagesSize - 1)
			{
				currentStage = 0;
			}
		}
	}
	if (input->TriggerKey(DIK_LEFT))
	{
		if (isBack == false)
		{
			isFront = false;
			isBack = true;
			currentStage--;
			index--;
			if (currentStage < 0)
			{
				currentStage = stagesSize - 1;
			}
		}
	}

	if (isFront == true)
	{
		moveAngle += 5;
		if (moveAngle >= angle * index)
		{
			moveAngle = angle * index;
			isFront = false;
		}
		parentTrans->rotation_.y = DegreeToRad(moveAngle);
	}

	if (isBack == true)
	{
		moveAngle -= 5;
		if (moveAngle <= angle * index)
		{
			moveAngle = angle * index;
			isBack = false;
		}
		parentTrans->rotation_.y = DegreeToRad(moveAngle);
	}

	parentTrans->UpdateMatrix();

	for (int i = 0; i < this->stagesSize; i++)
	{
		stageSelectTrans[i]->UpdateMatrix();
		stageTextTrans[i]->UpdateMatrix();
	}
}

void StageSelect::Draw()
{
	for (int i = 0; i < stagesSize; i++)
	{
		stageSelectModel[i]->Draw(*stageSelectTrans[i], viewProjection_);
		stageTextModel[i]->Draw(*stageTextTrans[i], viewProjection_);
	}
}

void StageSelect::ResetViewPos()
{
	parentTrans->rotation_.y = DegreeToRad(moveAngle);
	viewProjection_.eye = { 0,0,-50 };
	viewProjection_.target = { 0,0,0 };
}
