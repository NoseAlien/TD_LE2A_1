#include "Star.h"
#include "Random.h"
#include "Stage.h"
#include "SlowMotion.h"
#include "Player.h"
using namespace MathUtility;
using namespace std;

Model* Star::starModel = nullptr;
unique_ptr<Model> Star::powerUpModel = nullptr;

Star::Star() :
	gravity(2), collisionRadius(1),
	isCanHit(false), maxCanHitTimer(10), stageType(BaseStage),
	isChangeColor(false), changeColorTimer(0), changeColorMaxTimer(210),
	isSucked(false), suckedMaxTimer(180), suckedTimer(0)
{
	grainMoveEffect = move(make_unique<GrainMoveEffect>());
	trans = new WorldTransform();
	trans->Initialize();
	suckedCurve = move(make_unique<BezierCurve>(60));
}

Star::~Star()
{
	//grainMoveEffect->Clear();
	delete trans;
}

void Star::Load()
{
	starModel = Model::CreateFromOBJ("star", true);
	powerUpModel.reset(Model::CreateFromOBJ("player_kotubu", true));
}

void Star::UnLoad()
{
	delete starModel;
}

void Star::Generate(const Vector3& pos, const Vector3& dirVec, const int& generateType)
{
	// アニメーション関連
	animeIndex = 0;
	fream = 0;
	maxFream = 5;

	//------
	trans->translation_ = pos;
	//trans->scale_ = { 1.5,1.5,1.5 };
	trans->scale_ = { 0,0,0 };
	trans->rotation_ = { 0,DegreeToRad(180),0 };
	trans->UpdateMatrix();

	if (generateType != 0)
	{
		gravity = 1;
	}
	else
	{
		gravity = 0;
	}
	collisionRadius = trans->scale_.x;
	isNotGravity = false;
	isAngleShake = false;
	this->dirVec = dirVec;
	this->generateType = generateType;

	isDestroy = false;
	isGround = false;

	isAttack = 0;

	// 生成の見た目関連
	isGenerate = false;
	geneAddScaleEase.SetEaseTimer(60);
	geneAddScaleEase.SetPowNum(5);

	revival = move(make_unique<RevivalObj1>());

}

void Star::GenerateUpdate()
{
	SlowMotion* slowMotion = SlowMotion::GetInstance();
	if (isGenerate == true)
	{
		geneAddScaleEase.Update();
		trans->scale_ = geneAddScaleEase.Out({ 0,0,0 }, { 1.5f,1.5f,1.5f });
		trans->rotation_.z = Random::RangeF(-0.15f, 0.15f);
		//if (geneAddScaleEase.GetisEnd() == true)
		//{
		//	trans->scale_ = { 1.5f,1.5f,1.5f };
		//	trans->rotation_ = { 0,DegreeToRad(180),0 };
		//	isGenerate = false;
		//	geneAddScaleEase.ReSet();
		//}
		if (trans->scale_.x >= 1.4f)
		{
			trans->scale_ = { 1.5f,1.5f,1.5f };
			trans->rotation_ = { 0,DegreeToRad(180),0 };
			isGenerate = false;
			geneAddScaleEase.ReSet();
		}
	}
}

void Star::Update()
{
	if (revival->isRevival == true)
	{
		revival->timer++;
		if (revival->timer >= revival->maxTimer)
		{
			isGenerate = true;

			isDestroy = false;

			// アニメーション関連
			animeIndex = 0;
			fream = 0;
			maxFream = 5;

			//------
			trans->translation_ = revival->pos;
			trans->scale_ = { 0,0,0 };
			trans->rotation_ = { 0,DegreeToRad(180),0 };
			trans->UpdateMatrix();

			if (generateType != 0)
			{
				gravity = 1;
			}
			else
			{
				gravity = 0;
			}
			collisionRadius = trans->scale_.x;
			isNotGravity = false;
			isAngleShake = false;
			isDestroy = false;
			isGround = false;
			isAttack = 0;

			changeColorTimer = 0;
			isChangeColor = false;

			suckedTimer = 0;
			isSucked = false;
			suckedCurve->ReSet();

			revival->timer = 0;
			revival->isRevival = false;

		}
	}

	SlowMotion* slowMotion = SlowMotion::GetInstance();

	GenerateUpdate();

	if (isGenerate == false)
	{
		if (isAttack == false)
		{
			if (generateType == 0)
			{
				gravity -= 0.05 * slowMotion->GetSlowExrate();
				if (gravity <= -1) gravity = -1;
				speed -= 0.1 * slowMotion->GetSlowExrate();
				if (speed <= 0)
				{
					speed = 0;
				}
				trans->translation_.x += speed * dirVec.Normalized().x * slowMotion->GetSlowExrate();
				trans->translation_.y += gravity * slowMotion->GetSlowExrate();
			}
			else if (generateType == 1 || generateType == 2)
			{
				gravity -= 0.05 * slowMotion->GetSlowExrate();
				if (gravity <= -1) gravity = -1;
				trans->translation_.x += speed * dirVec.Normalized().x * slowMotion->GetSlowExrate();
				trans->translation_.y += gravity * slowMotion->GetSlowExrate();
			}

			// 攻撃可能かどうか
			canHitTimer += 1 * slowMotion->GetSlowExrate();
			if (canHitTimer >= maxCanHitTimer)
			{
				canHitTimer = maxCanHitTimer;
				isCanHit = true;
			}

			//trans->rotation_ = { 0,0,trans->rotation_.z };

			if (isAngleShake == true)
			{
				trans->scale_ = trans->scale_ = { 1.5,1.5,1.5 };
				trans->scale_.z *= cos(clock() * 0.05) * 0.15 + 1;
				trans->scale_.x *= cos(clock() * 0.05) * 0.15 + 1;
				trans->scale_.y *= sin(clock() * 0.05) * 0.15 + 1;
			}
			else
			{
				trans->rotation_.z = 0;

				int sign = 1;
				if (dirVec.x < 0)
				{
					sign = -1;
				}

				if (generateType == 0)
				{
					trans->scale_ = { 1.5f / (1 + speed),1.5f / (1 + speed),1.5 };
					//trans->rotation_.z += sign * 0.05 * slowMotion->GetSlowExrate();
				}
				else if (generateType == 1 || generateType == 2)
				{
					trans->scale_ = trans->scale_ = { 1.5,1.5,1.5 };
					//trans->rotation_.z += sign * 0.05 * slowMotion->GetSlowExrate();
				}
				trans->scale_.z *= cos(clock() * 0.005) * 0.1 + 1;
				trans->scale_.x *= cos(clock() * 0.005) * 0.1 + 1;
				trans->scale_.y *= sin(clock() * 0.005) * 0.1 + 1;
			}
		}


		AttackUpdate();

		SuckedUpdate();
	}
	trans->UpdateMatrix();
}

void Star::AttackUpdate()
{
	if (isAttack == 1)
	{
		if (gravity > 0)
		{
			isGround = false;
		}

		SlowMotion* slowMotion = SlowMotion::GetInstance();

		gravity -= 0.05 * slowMotion->GetSlowExrate();
		if (gravity <= -1) gravity = -1;
		trans->translation_.x += speed * dirVec.Normalized().x * slowMotion->GetSlowExrate();
		trans->translation_.y += gravity * slowMotion->GetSlowExrate();
	}
}

void Star::SuckedUpdate()
{
	if (isAttack == false)
	{
		changeColorTimer++;
		if (changeColorTimer >= changeColorMaxTimer)
		{
			changeColorTimer = changeColorMaxTimer;
			isChangeColor = true;
		}

		if (isChangeColor == true)
		{
			suckedTimer++;
			if (suckedTimer >= suckedMaxTimer)
			{
				suckedTimer = suckedMaxTimer;
				if (isSucked == false)
				{
					Vector3 targetPos = ground->GetMouthPos();

					Vector3 p1 =
					{
						trans->translation_.x - 10,
						(targetPos.y - trans->translation_.y) / 3,
						-10,
					};

					Vector3 p2 =
					{
						(targetPos.x - trans->translation_.x) / 2,
						(targetPos.y - trans->translation_.y) / 2,
						-15,
					};

					Vector3 p3 =
					{
						targetPos.x,
						targetPos.y,
						-10,
					};

					suckedCurve->AddPoint(trans->translation_);
					suckedCurve->AddPoint(p1);
					suckedCurve->AddPoint(p2);
					suckedCurve->AddPoint(p3);
					suckedCurve->AddPoint(targetPos);
					isSucked = true;
				}
			}
		}

		if (isSucked == true)
		{
			suckedCurve->SetBackPoint(ground->GetMouthPos());
			suckedCurve->Update();
			trans->translation_ = suckedCurve->InterPolation(BezierCurve::InterPolationType::EaseIn);

			if (suckedCurve->GetisEnd() == true && isDestroy == false &&
				revival->isRevival == false)
			{
				ground->AddHP(2);
				isDestroy = true;
			}
		}
	}
}

void Star::Draw(const ViewProjection& viewProjection_)
{
	if (revival->isRevival == false)
	{
		fream++;
		if (fream > maxFream)
		{
			animeIndex++;
			if (animeIndex > 8)
			{
				animeIndex = 0;
			}
			fream = 0;
		}

		if (isChangeColor == false)
		{
			starModel->Draw(*trans, viewProjection_, Player::playerTexAnime[animeIndex]);
		}
		else
		{
			powerUpModel->Draw(*trans, viewProjection_);
		}
	}
}

void Star::DrawEffectBack()
{
	grainMoveEffect->Draw();
}

void Star::UpdateEffect()
{
	if (isGround == false)
	{
		grainMoveEffect->Generate(trans->translation_, 1);
	}
	grainMoveEffect->Update();
}

