#include "Block.h"
using namespace std;

Model* Block::blockModel = nullptr;
uint32_t Block::blockBreakTexture = 0;
Audio* Block::audio = nullptr;
uint32_t Block::damageSE = 0;
uint32_t Block::breakSE = 0;

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
	blockBreakTexture = TextureManager::Load("block/block_break.png");
	damageSE = audio->LoadWave("se/floor_damage.wav");
	breakSE = audio->LoadWave("se/block_break.wav");
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
	geneAddRotEase.SetEaseTimer(60);
	geneAddRotEase.SetPowNum(5);

	revival = move(make_unique<RevivalObj>());
}

void Block::GenerateUpdate()
{
	SlowMotion* slowMotion = SlowMotion::GetInstance();
	if (isGenerate == true)
	{
		geneAddScaleEase.Update();
		trans->scale_ = geneAddScaleEase.Out({ 0,0,0 }, { 2,2,2 });
		geneAddRotEase.Update();
		trans->rotation_ = geneAddRotEase.Out({ -8,DegreeToRad(180),0 }, { 0,DegreeToRad(180),0 });
		//trans->rotation_.z = Random::RangeF(-0.15f, 0.15f);
		if (trans->scale_.x >= 2)
		{
			trans->scale_ = { 2,2,2 };
			trans->rotation_ = { 0,DegreeToRad(180),0 };
			isGenerate = false;
			geneAddScaleEase.ReSet();
			geneAddRotEase.ReSet();
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
		if (hp == 1)
		{
			blockModel->Draw(*trans, viewProjection_, blockBreakTexture);
		}
		else if (hp == 2)
		{
			blockModel->Draw(*trans, viewProjection_);
		}
	}
}

void Block::Damage(const int& subhp) {
	hp -= subhp;
	if(hp <= 0)
	{
		audio->PlayWave(breakSE);
	}
	else
	{
		audio->PlayWave(damageSE);
	}
}

