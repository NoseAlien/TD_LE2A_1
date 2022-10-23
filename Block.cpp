#include "Block.h"
using namespace std;

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
	blockModel = Model::CreateFromOBJ("block", false);
}

void Block::UnLoad()
{
	delete blockModel;
}

void Block::Generate(const Vector3& pos, const Vector3& scale, const bool& haveStar)
{
	hp = maxhp;
	trans->translation_ = pos;
	//trans->scale_ = scale;
	trans->scale_ = { 0,0,0 };
	trans->rotation_ = { 0,DegreeToRad(180),0 };
	trans->UpdateMatrix();
	isDestroy = false;
	isHit = 0;
	this->haveStar = haveStar;

	// ¶¬‚ÌŒ©‚½–ÚŠÖ˜A
	isGenerate = false;
	geneAddScaleEase.SetEaseTimer(60);
	geneAddScaleEase.SetPowNum(5);

	revival = move(make_unique<RevivalObj>());
}

void Block::GenerateUpdate()
{
	SlowMotion* slowMotion = SlowMotion::GetInstance();
	if (isGenerate == true)
	{
		geneAddScaleEase.Update();
		trans->scale_ = geneAddScaleEase.Out({ 0,0,0 }, { 2,2,2 });
		//trans->rotation_.z = Random::RangeF(-0.15f, 0.15f);
		if (trans->scale_.x >= 2)
		{
			trans->scale_ = { 2,2,2 };
			trans->rotation_ = { 0,DegreeToRad(180),0 };
			isGenerate = false;
			geneAddScaleEase.ReSet();
		}
	}
}

void Block::Update()
{
	if (revival->isRevival == true)
	{
		revival->timer++;
		if (revival->timer >= revival->maxTimer)
		{
			isGenerate = true;
			hp = maxhp;
			trans->translation_ = revival->pos;
			trans->scale_ = { 0,0,0 };
			trans->UpdateMatrix();
			isDestroy = false;
			isHit = 0;

			// ¶¬‚ÌŒ©‚½–ÚŠÖ˜A
			//isGenerate = false;
			//geneAddScaleEase.SetEaseTimer(60);
			//geneAddScaleEase.SetPowNum(5);

			revival->timer = 0;
			revival->isRevival = false;
		}
	}

	GenerateUpdate();

	if (isGenerate == false)
	{
		if (hp <= 0)
		{
			hp = 0;
			isDestroy = true;
		}
	}
	trans->UpdateMatrix();
}

void Block::Draw(const ViewProjection& viewProjection_)
{
	if (revival->isRevival == false)
	{
		blockModel->Draw(*trans, viewProjection_);
	}
}

