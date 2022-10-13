#include "Block.h"

Block::Block() :
	maxhp(10)
{
	trans = new WorldTransform();
	trans->Initialize();
	blockModel = Model::Create();
}

Block::~Block()
{
	delete trans;
	delete blockModel;
}

void Block::Generate(const Vector3& pos, const Vector3& scale, const bool& haveStar)
{
	hp = maxhp;
	trans->translation_ = pos;
	trans->scale_ = scale;
	trans->UpdateMatrix();
	isDestroy = false;
	isHit = 0;
	this->haveStar = haveStar;
}

void Block::Update()
{
	if (hp <= 0)
	{
		hp = 0;
		isDestroy = true;
	}

	trans->UpdateMatrix();
}

void Block::Draw(const ViewProjection& viewProjection_, const uint32_t& starTexture)
{
	blockModel->Draw(*trans, viewProjection_, starTexture);
}
