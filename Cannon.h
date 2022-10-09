#pragma once
#include "WorldTransform.h"
#include "Model.h"

class Cannon
{
private:
	WorldTransform* trans = nullptr;
	Model* blockModel = nullptr;		// ���f��
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
	void Generate(const Vector3& pos, const Vector3& rot);
	void Update();
	void Draw(const ViewProjection& viewProjection_, const uint32_t& starTexture);

	inline void SetisShot(const bool& isShot) { this->isShot = isShot; }
	inline bool GetisShot() { return isShot; }
	inline bool GetisReverseAddScale() { return isReverseAddScale; }
	inline Vector3 GetDirVec() { return dirVec; }
	inline Vector3 GetPos() { return trans->translation_; }

};
