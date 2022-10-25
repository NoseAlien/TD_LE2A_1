#pragma once
#include <vector>
#include <memory>
#include "Vector2.h"
#include "Easing.h"
#include "Sprite.h"

class SceneChange
{
private:
	std::unique_ptr<Sprite> sprite;
	uint32_t tex;
	bool isSceneChange;
	bool isSceneChangeIn;
	bool isSceneChangeOut;
	bool isChange;
	bool isGenerate;

private:
	// イージング関連
	Easing ease;
	Easing bgmEase;

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
