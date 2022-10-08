#include "Cannon.h"
#include <math.h>

Cannon::Cannon() : maxShotTimer(240)
{
	trans = new WorldTransform();
	trans->Initialize();
	blockModel = Model::Create();
}

Cannon::~Cannon()
{
	delete trans;
	delete blockModel;
}

void Cannon::Generate(const Vector3& pos, const Vector3& rot)
{
	trans->translation_ = pos;
	trans->scale_ = { 5,2,2 };
	trans->rotation_ = rot;
	trans->UpdateMatrix();
	shotTimer = 0;
	isAddScale = false;
	addScaleTimer = 0;
	maxAddScaleTimer = 5;
	isReverseAddScale = 0;
	dirVec = { cosf(rot.z),sinf(rot.z),0 };
}

void Cannon::Update()
{
	if (isAddScale == false)//isShot == false)
	{
		shotTimer++;
		if (shotTimer >= maxShotTimer)
		{
			shotTimer = 0;
			isAddScale = true;
		}
	}

	if (isAddScale == true)
	{
		if (isReverseAddScale == 0)
		{
			trans->scale_.x -= 0.4;
			trans->scale_.y += 0.1;
			trans->scale_.z += 0.1;
			addScaleTimer++;
			if (addScaleTimer >= maxAddScaleTimer)
			{
				addScaleTimer = 0;
				maxAddScaleTimer = maxAddScaleTimer * 2;
				isReverseAddScale = 1;
				isShot = true;
			}
		}
		else if (isReverseAddScale == 1)
		{
			trans->scale_.x += 0.4;
			trans->scale_.y -= 0.1;
			trans->scale_.z -= 0.1;
			addScaleTimer++;
			if (addScaleTimer >= maxAddScaleTimer)
			{
				addScaleTimer = 0;
				maxAddScaleTimer = maxAddScaleTimer / 2;
				isReverseAddScale = 2;
			}
		}
		else if (isReverseAddScale == 2)
		{
			trans->scale_.x -= 0.4;
			trans->scale_.y += 0.1;
			trans->scale_.z += 0.1;
			addScaleTimer++;
			if (addScaleTimer >= maxAddScaleTimer)
			{
				addScaleTimer = 0;
				//maxAddScaleTimer = maxAddScaleTimer;
				isReverseAddScale = 0;
				isAddScale = false;
				trans->scale_ = { 5,2,2 };
			}
		}
	}

	trans->UpdateMatrix();
}

void Cannon::Draw(const ViewProjection& viewProjection_, const uint32_t& starTexture)
{
	blockModel->Draw(*trans, viewProjection_, starTexture);
}
