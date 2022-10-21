#pragma once
#include <vector>
#include <memory>
#include "Vector2.h"
#include "Easing.h"
#include "Sprite.h"

struct SceneChangeBlock
{
	Sprite* sprite = nullptr;

	Vector2 pos;
	float exrate;
	float angle;

	bool isIn = false;
	bool isOut = false;
public:

	SceneChangeBlock(const uint32_t& tex);
	~SceneChangeBlock();
	void Update(float frame);
	void Draw();
};

class SceneChange
{
private:
	std::vector<std::unique_ptr<SceneChangeBlock>> blocks;
	std::unique_ptr<Sprite> sprite;
	uint32_t tex;
	bool isSceneChange;
	bool isSceneChangeIn;
	bool isSceneChangeOut;
	bool isChange;
	bool isGenerate;
	float frame;
	int index;
	int addDightIndex;

private:
	// イージング関連
	Easing ease;

	//Easing easeIn;
	//Easing easeOut;

public:
	void Load();
	void Initialize();
	void Update();
	void Draw();

	void StartSceneChange();

	inline bool GetisChange() { return isChange; }
	inline void SetisChange(bool isChange) { this->isChange = isChange; }
	inline bool GetisSceneChangeNow() { return isSceneChange; }

public:
	static SceneChange* GetInstance();
	static void DestroyInstance();
private:
	SceneChange() {}
	~SceneChange() {}

	SceneChange(const SceneChange&) = delete;
	SceneChange& operator=(const SceneChange&) = delete;
	SceneChange(const SceneChange&&) = delete;
	SceneChange& operator=(const SceneChange&&) = delete;
};

extern SceneChange* sceneChange;
