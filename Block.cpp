#include "Block.h"

Model* Block::blockModel = nullptr;

Block::Block() :
	maxhp(2)
{
	trans = new WorldTransform();
	trans->Initialize();
}

Block::~Block()
{
	delete trans;
}

void Block::Load()
{
	blockModel = Model::CreateFromOBJ("block", true);
}

void Block::UnLoad()
{
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

void Block::Draw(const ViewProjection& viewProjection_)
{
	blockModel->Draw(*trans, viewProjection_);
}
