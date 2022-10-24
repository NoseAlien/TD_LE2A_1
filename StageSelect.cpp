#include "StageSelect.h"
#include "GameScene.h"
#include "Input.h"
#include "Player.h"
#include "Stage.h"
#include "Particle.h"
using namespace std;

Model* StageSelect::stageSelectModel = nullptr;
Model* StageSelect::stageTextModel = nullptr;
vector<uint32_t> StageSelect::stageSelectTextures = {};

StageSelect::StageSelect(const int& stageSize) :
	currentStage(0), moveSpeed(0.13), stageSize(stageSize),
	stageSelectSize(0.5)
{
	for (int i = 0; i < Stage::stageNumberTextures.size(); i++)
	{
		stageNumberSprites.push_back(Sprite::Create(Stage::stageNumberTextures[i], { 0,0 }));
		stageNumberSprites.back()->SetAnchorPoint({ 0.5f,0.75f });
		stageNumberSprites.back()->SetSize({ 420,98 });
	}
}
StageSelect::~StageSelect()
{
	for (int i = 0; i < Stage::stageNumberTextures.size(); i++)
	{
		delete stageNumberSprites[i];
	}
}

void StageSelect::Load()
{
	stageSelectModel = Model::CreateFromOBJ("select_screen", false);
	stageTextModel = Model::CreateFromOBJ("select_stage", false);

	for (uint32_t i = 1; i <= 10; i++)
	{
		stageSelectTextures.push_back(
			TextureManager::Load("SpriteTexture/select_screen/select_screen_" + to_string(i) + ".png"));
	}
}
void StageSelect::UnLoad()
{
	delete stageSelectModel;
	delete stageTextModel;
}

static float angle = 0;
void StageSelect::Initialize()
{
	angle = 360 / stageSize;

	for (int i = 0; i < stageSize; i++)
	{
		//stageSelectModel.emplace_back(Model::Create());
		stageSelectTrans.emplace_back(move(make_unique<WorldTransform>()));

		stageSelectTrans.back()->Initialize();
		stageSelectTrans.back()->translation_ = { (float)i * 25,-12,0 };
		//stageSelectTrans.back()->scale_ = { 5,5,1 };
		//stageSelectTrans.back()->scale_ = { 0.75f,0.75f,0.75f };
		stageSelectTrans.back()->scale_ = { stageSelectSize + 0.5f,stageSelectSize,stageSelectSize };
		stageSelectTrans.back()->rotation_ = { 0,DegreeToRad(180),0 };
		stageSelectTrans.back()->UpdateMatrix();

		//stageTextModel.emplace_back(Model::Create());
		stageTextTrans.emplace_back(move(make_unique<WorldTransform>()));

		stageTextTrans.back()->Initialize();
		//stageTextTrans.back()->translation_ = { (float)i * 25,stageSelectTrans.back()->scale_.y,0 };
		stageTextTrans.back()->translation_ = { (float)i * 25,11,0 };
		//stageTextTrans.back()->scale_ = { 1.5,1.5,1.5 };
		stageTextTrans.back()->scale_ = { stageSelectSize + 0.5f,stageSelectSize,stageSelectSize };
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

	currentStage = min(max(currentStage, 0), stageSize - 1);

	for (int i = 0; i < stageSize; i++)
	{
		float moveX = (((i - currentStage) * 25) - stageSelectTrans[i]->translation_.x) * moveSpeed;
		stageSelectTrans[i]->translation_.x += moveX;
		stageTextTrans[i]->translation_.x += moveX;
	}

	for (int i = 0; i < stageSize; i++)
	{
		stageSelectTrans[i]->UpdateMatrix();
		stageTextTrans[i]->UpdateMatrix();
	}

	StageNumberUpdate();

	viewProjection_.UpdateMatrix();
}

void StageSelect::StageNumberUpdate()
{
	for (int i = 0; i < stageNumberSprites.size(); i++)
	{
		Vector2 tempPos = WorldToScreen(stageTextTrans[i]->translation_, viewProjection_);

		stageNumberSprites[i]->SetPosition(tempPos);
	}

}

void StageSelect::Draw()
{
	for (int i = 0; i < stageSize; i++)
	{
		if (i < 10)
		{
			stageSelectModel->Draw(*stageSelectTrans[i], viewProjection_, stageSelectTextures[i]);
		}
		else
		{
			stageSelectModel->Draw(*stageSelectTrans[i], viewProjection_);
		}
		stageTextModel->Draw(*stageTextTrans[i], viewProjection_);
	}
}

void StageSelect::DrawSprite()
{
	for (int i = 0; i < stageNumberSprites.size(); i++)
	{
		stageNumberSprites[i]->Draw2();
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

	for (int i = 0; i < stageSize; i++)
	{
		stageSelectTrans[i]->scale_ = { stageSelectSize + 0.1f,stageSelectSize,stageSelectSize };
		stageTextTrans[i]->scale_ = { stageSelectSize + 0.1f,stageSelectSize,stageSelectSize };

		stageSelectTrans[i]->translation_.x =
			stageSelectTrans[currentStage]->translation_.x -
			(currentStage - i) * 25;
		stageSelectTrans[i]->translation_.y = -12;

		stageTextTrans[i]->translation_.x =
			stageTextTrans[currentStage]->translation_.x -
			(currentStage - i) * 25;
		stageTextTrans[i]->translation_.y = 11;

		stageSelectTrans[i]->UpdateMatrix();
		stageTextTrans[i]->UpdateMatrix();

	}
}
