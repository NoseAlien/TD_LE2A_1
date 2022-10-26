#include "Cannon.h"
#include <math.h>

Model* Cannon::cannonModel = nullptr;

Cannon::Cannon() : maxShotTimer(100)
{
	trans = new WorldTransform();
	trans->Initialize();
}

Cannon::~Cannon()
{
	delete trans;
}

void Cannon::Load()
{
	cannonModel = Model::CreateFromOBJ("battery", true);
}

void Cannon::UnLoad()
{
	delete cannonModel;
}

void Cannon::Generate(const Vector3& pos, const Vector3& rot, const float& dirVecAngle)
{
	trans->translation_ = pos;
	//trans->scale_ = { 5,2,2 };
	trans->scale_ = { 0.5,0.5,0.5 };
	trans->rotation_ = rot;
	trans->UpdateMatrix();
	shotTimer = 0;
	isAddScale = false;
	addScaleTimer = 0;
	maxAddScaleTimer = 5;
	isReverseAddScale = 0;
	dirVec = { cosf(DegreeToRad(dirVecAngle)),sinf(DegreeToRad(dirVecAngle)),0 };
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
			//trans->scale_.x -= 0.4;
			//trans->scale_.y += 0.1;
			//trans->scale_.z += 0.1;
			trans->scale_.x -= 0.4 / 10;
			trans->scale_.y += 0.1 / 4;
			trans->scale_.z += 0.1 / 4;

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
			//trans->scale_.x += 0.4;
			//trans->scale_.y -= 0.1;
			//trans->scale_.z -= 0.1;
			trans->scale_.x += 0.4 / 10;
			trans->scale_.y -= 0.1 / 4;
			trans->scale_.z -= 0.1 / 4;
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
			//trans->scale_.x -= 0.4;
			//trans->scale_.y += 0.1;
			//trans->scale_.z += 0.1;
			trans->scale_.x -= 0.4 / 10;
			trans->scale_.y += 0.1 / 4;
			trans->scale_.z += 0.1 / 4;
			addScaleTimer++;
			if (addScaleTimer >= maxAddScaleTimer)
			{
				addScaleTimer = 0;
				//maxAddScaleTimer = maxAddScaleTimer;
				isReverseAddScale = 0;
				isAddScale = false;
				//trans->scale_ = { 5,2,2 };
				trans->scale_ = { 0.5,0.5,0.5 };
			}
		}
	}

	trans->UpdateMatrix();
}

void Cannon::Draw(const ViewProjection& viewProjection_)
{
	cannonModel->Draw(*trans, viewProjection_);
}
