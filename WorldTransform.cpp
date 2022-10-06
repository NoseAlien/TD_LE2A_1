#include "WorldTransform.h"
#include <math.h>

void WorldTransform::UpdateMatrix()
{
	//////�g�k//////
	//�X�P�[�����O�{�����s��ɐݒ肷��
	Matrix4 matScale =
	{scale_.x,0,0,0,
	0,scale_.y,0,0,
	0,0,scale_.z,0,
	0,0,0,1 };

	//////��]//////
	//Z����]�s���錾
	Matrix4 matRotZ =
	{ (float)cos(rotation_.z),(float)sin(rotation_.z),0,0,
	(float)-sin(rotation_.z),(float)cos(rotation_.z),0,0,
	0,0,1,0,
	0,0,0,1 };
	//X����]�s���錾
	Matrix4 matRotX =
	{ 1,0,0,0,
	0,(float)cos(rotation_.x),(float)sin(rotation_.x),0,
	0,(float)-sin(rotation_.x),(float)cos(rotation_.x),0,
	0,0,0,1 };
	//Y����]�s���錾
	Matrix4 matRotY =
	{ (float)cos(rotation_.y),0,(float)-sin(rotation_.y),0,
	0,1,0,0,
	(float)sin(rotation_.y),0,(float)cos(rotation_.y),0,
	0,0,0,1 };

	//�����p��]�s���錾���AZXY�̏��ɍ���
	Matrix4 matRot = matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	//////���s�ړ�//////
	//�ړ��ʂ��s��ɐݒ肷��
	Matrix4 matTrans =
	{ 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	translation_.x,translation_.y,translation_.z,1 };


	//�P�ʍs�����
	matWorld_ =
	{ 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1 };
	//�X�P�[�����O�s����|����
	matWorld_ *= matScale;
	//�����p��]�s����|����
	matWorld_ *= matRot;
	//���s�ړ��s����|����
	matWorld_ *= matTrans;

	matWorldRot_ =
	{ 1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1 }; // �y�����ӏ��z���[���h��]�s��̕ό`�����Z�b�g
	matWorldRot_ *= matRot;        // �y�����ӏ��z���[���h��]�s��ɉ�]�𔽉f

	if (parent_ != nullptr)
	{
		matWorld_ *= parent_->matWorld_;
		matWorldRot_ *= parent_->matWorldRot_;//�y�����ӏ��z�e�s��ɂ���]�����|���Z����
	}

	//�s��̓]��
	TransferMatrix();
}