#include "SceneChange.h"
#include "GameScene.h"
#include "MathUtility.h"
using namespace std;

void SceneChange::Load()
{
	tex = TextureManager::Load("black1x1.png");
}

void SceneChange::Initialize()
{
	isSceneChange = false;
	isChange = false;
	isGenerate = false;
	isSceneChangeIn = false;
	isSceneChangeOut = false;
	frame = 10;
	index = 0;
	addDightIndex = 0;
	blocks.clear();
}

void SceneChange::Update()
{
	if (isSceneChange == true)
	{
		if (isGenerate == true)
		{
			for (int y = 0; y < 9; y++)
			{
				for (int x = 0; x < 16; x++)
				{
					blocks.push_back(move(make_unique<SceneChangeBlock>(tex)));
					blocks.back()->pos = { (float)(60 + x * 120), (float)(60 + y * 120) };
					blocks.back()->exrate = 0;
					blocks.back()->angle = 0;
				}
			}
			isGenerate = false;
			isSceneChangeIn = true;
		}

		if (isSceneChangeIn == true)
		{
			if (blocks.size() > 0)
			{
				if (addDightIndex < 24)
				{
					index = 0;
					for (int y = 0; y < 9; y++)
					{
						for (int x = 0; x < 16; x++)
						{
							if (y + x == addDightIndex)
							{
								blocks[index]->isIn = true;
							}
							index++;
						}
					}
					addDightIndex++;
				}
				else if (addDightIndex == 24)
				{
					int count = 0;

					for (int i = 0; i < blocks.size(); i++)
					{
						if (blocks[i]->isIn == false)
						{
							count++;
						}
					}
					if (count >= blocks.size() - 1)
					{
						isSceneChangeIn = false;
						isSceneChangeOut = true;
						isChange = true;
						index = 0;
						addDightIndex = 0;
					}
				}
			}
		}
		if (isSceneChangeOut == true)
		{
			if (blocks.size() > 0)
			{
				if (addDightIndex < 24)
				{
					index = 0;
					for (int y = 0; y < 9; y++)
					{
						for (int x = 0; x < 16; x++)
						{
							if (y + x == addDightIndex)
							{
								blocks[index]->isOut = true;
							}
							index++;
						}
					}
					addDightIndex++;
				}
				else if (addDightIndex == 24)
				{
					int count = 0;

					for (int i = 0; i < blocks.size(); i++)
					{
						if (blocks[i]->isIn == false)
						{
							count++;
						}
					}
					if (count >= blocks.size() - 1)
					{
						Initialize();
					}
				}
			}
		}


		for (int i = 0; i < blocks.size(); i++)
		{
			blocks[i]->Update(frame);
		}
	}

}
void SceneChange::Draw()
{
	for (int i = 0; i < blocks.size(); i++)
	{
		blocks[i]->Draw();
		//DrawRotaGraph(blocks[i]->pos.x, blocks[i]->pos.y, blocks[i]->exrate, Radian(blocks[i]->angle), image, true);
	}
}

void SceneChange::StartSceneChange()
{
	if (isSceneChange == false)
	{
		Initialize();
		isSceneChange = true;
		isGenerate = true;
	}
}

SceneChange* SceneChange::GetInstance()
{
	static SceneChange* sceneChange = new SceneChange;
	return sceneChange;
}

void SceneChange::DestroyInstance()
{
	delete SceneChange::GetInstance();
}
SceneChange* sceneChange = SceneChange::GetInstance();

// ƒuƒƒbƒN
SceneChangeBlock::SceneChangeBlock(const uint32_t& tex) : exrate(0), angle(0)
{
	sprite = Sprite::Create(tex, pos);
}
SceneChangeBlock::~SceneChangeBlock()
{
	delete sprite;
}
void SceneChangeBlock::Update(float frame)
{
	if (isIn)
	{
		exrate += 1.0f / frame;
		if (exrate >= 1) exrate = 1;
		angle += 90.0f / frame;
		if (angle >= 90) angle = 90;

		if (exrate >= 1 && angle >= 90)
		{
			isIn = false;
		}
	}

	if (isOut)
	{
		exrate -= 1.0f / frame;
		if (exrate <= 0) exrate = 0;
		angle += 90.0f / frame;
		if (angle >= 180) angle = 180;

		if (exrate <= 0 && angle <= 180)
		{
			isOut = false;
		}
	}

	sprite->SetPosition(pos);
	sprite->SetRotation(DegreeToRad(angle));
	sprite->SetSize({ exrate * 120,exrate * 120 });
	sprite->SetAnchorPoint({ 0.5f, 0.5f });
}
void SceneChangeBlock::Draw()
{
	sprite->Draw();
}

