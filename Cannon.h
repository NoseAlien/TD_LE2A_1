#pragma once
#include "WorldTransform.h"
#include "Model.h"

class Cannon
{
private:
	WorldTransform* trans = nullptr;
	static Model* cannonModel;		// ƒ‚ƒfƒ‹
	uint32_t blockTexture;

	int shotTimer;
	int maxShotTimer;
	bool isShot;

	bool isAddScale;
	Vector3 addScale;
	int addScaleTimer;
	int maxAddScaleTimer;
	int isReverseAddScale;

	Vector3 dirVec;

public:
	Cannon();
	~Cannon();
	static void Load();
	static void UnLoad();
	void Generate(const Vector3& pos, const Vector3& rot, const float& dirVecAngle);
	void Update();
	void Draw(const ViewProjection& viewProjection_);

	inline void SetisShot(const bool& isShot) { this->isShot = isShot; }
	inline void SetisAddScale(const bool& isAddScale) { this->isAddScale = isAddScale; }

	inline bool GetisShot() { return isShot; }
	inline bool GetisAddScale() { return isAddScale; }
	inline bool GetisReverseAddScale() { return isReverseAddScale; }
	inline Vector3 GetDirVec() { return dirVec; }
	inline Vector3 GetPos() { return trans->translation_; }

};

