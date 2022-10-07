#pragma once
#include "Enemy.h"
#include "Star.h"
#include "Thorn.h"
#include <memory>

class Stage
{
private:
	std::list<std::unique_ptr<Star>> stars;
	std::list<std::unique_ptr<Thorn>> thorns;
	static uint32_t starTexture;			// 星テクスチャー
	static uint32_t thornTexture;		// トゲテクスチャー

	bool gameClear;
	bool gameOver;


private:
	void HitGenerateStar(const Vector3 pos);

	void PlayerUpdate();
	void FloorUpdate();
	void StarUpdate();
	void ThornUpdate();
public:
	Stage();
	~Stage();
	static void Load();
	void Init();
	void Update();
	void Draw(const ViewProjection& viewProjection_);

	void GenerateThorn(const Vector3& pos);
};

