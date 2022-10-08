#pragma once
#include "Ground.h"
#include "Star.h"
#include "Thorn.h"
#include "Block.h"
#include "Cannon.h"
#include <memory>

enum StageType
{
	BaseStage,
	RaceStage,
	CannonStage,
};

class Stage
{
private:
	std::list<std::unique_ptr<Star>> stars;
	std::list<std::unique_ptr<Thorn>> thorns;
	std::list<std::unique_ptr<Block>> blocks;
	std::list<std::unique_ptr<Cannon>> cannons;
	static uint32_t starTexture;			// 星テクスチャー
	static uint32_t thornTexture;		// トゲテクスチャー

	bool gameClear;
	bool gameOver;

	int stageType;

private:
	void PlayerGenerateStar(const Vector3 pos);

	void PlayerUpdate();
	void FloorUpdate();
	void StarUpdate();
	void ThornUpdate();
	void BlockUpdate();
	void CannonUpdate();

public:
	Stage();
	~Stage();
	static void Load();
	void Init();
	void Update();
	void Draw(const ViewProjection& viewProjection_);

	void GenerateThorn(const Vector3& pos, const Vector3& scale = { 1,1,1 });
	void GenerateBlock(const Vector3& pos, const Vector3& scale = { 1,1,1 });
	void GenerateCannon(const Vector3& pos, const Vector3& rot);
};

