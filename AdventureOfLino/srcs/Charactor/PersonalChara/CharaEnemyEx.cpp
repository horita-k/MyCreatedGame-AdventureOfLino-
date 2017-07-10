/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �G�h���N���X
 * @author SPATZ.
 * @data   2014/01/15 01:59:46
 */
//---------------------------------------------------
#include "CharaEnemyEx.h"
#include "Charactor/CharaInfo.h"
#include "Charactor/CharaPlayer.h"
#include "AppLib/Basic/Basic.h"
#include "AppLib/Graphic/GraphicManager.h"
#include "AppLib/Graphic/CameraManager.h"
#include "AppLib/Effect/EffectManager.h"
#include "AppLib/Input/InputManager.h"
#include "AppLib/Sound/SoundManager.h"

#include "AppLib/Collision/CollisionManager.h"
#include "DataDefine/MotionDef.h"
#include "DataDefine/VoiceDef.h"
#include "DataDefine/EffectDef.h"
#include "Scene/ActionTaskManager.h"
#include "Utility/Utility.h"

/*=====================================*
 * define
 *=====================================*/

#define RESET_SUB_STATE()						\
	{											\
		mStatePhase = 0;						\
		mStateCount = 0;						\
	}

#define NEXT_SUB_STATE()						\
	{											\
		mStatePhase ++;							\
		mStateCount = 0;						\
	}

/*=====================================*
 * �X���C���N���X
 *=====================================*/
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemySlime::Setup(const char* enemyName, int maxHitpoint, const char* layoutName)
{
	// �G�x�[�X�N���X��Setup�K�v
	CharaEnemy::Setup(enemyName, maxHitpoint, layoutName);
	mFlinchMax = 150;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemySlime::phaseEnterStateAttack1(void)
{
	int frame_index = 0;

	SoundManager::GetSndComponent()->PlaySound(eSE_SWING_SWORD);
	StartMotion(eMT_ATTACK_1, false, 0.625f);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemySlime::phaseStateAttack1(void)
{
	bool is_prev_attack;
	is_prev_attack = mIsAttack;

	static const float sStart	= 30.0f;
	static const float sEnd		= 56.0f;

	if (this->EqualPhaseFrame(sStart)) {
		SoundManager::GetVcComponent(mCharaName)->PlaySound(eVC_ATTACK_1);
		SoundManager::GetSndComponent()->PlaySound(eSE_BOYOYON);
	}

	// ���̊֐����� mIsAttack ���ύX�����\��������̂Œ���
	CharaEnemy::ProcessAttackToPlayer(sStart, sEnd, "��", 35.0f, 2, true);

	if (GetIsEndMotion()) {
		ChangeState(eCHARA_STATE_IDLE);
	}
}

//---------------------------------------------------
/**
 * �L�����̍U���X�e�[�^�X�J�ڂ̃`�F�b�N�֐�
 */
//---------------------------------------------------
bool
CharaEnemySlime::checkChangeAttackState(int* pAttackCharaState)
{
	CharaBase* p_player = (CharaBase *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();
	VECTOR vec = VSub(p_player->Position(), mPosition);
	if (VSize(vec) < mBodySize + (mBodySize * (GetRand(200) * 0.010f))) {

		*pAttackCharaState = eCHARA_STATE_ATTACK_1;
		return true;
	}
	return false;
}


/*=====================================*
 * �E�b�h�}���N���X
 *=====================================*/
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyWoodman::phaseEnterStateAttack1(void)
{
	SoundManager::GetVcComponent(mCharaName)->PlaySound(eVC_ATTACK_1);
	StartMotion(eMT_ATTACK_1, false, 0.6f);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyWoodman::phaseStateAttack1(void)
{
	float move_speed = 0.32f * (100 - GetMotionTimePercent());
	mMoveVec.x += -(sin(mRotation.y) * move_speed);
	mMoveVec.z += -(cos(mRotation.y) * move_speed);

	
	static const float sStart	= 4.0f;
	static const float sEnd		= 60.0f;

	// ���̊֐����� mIsAttack ���ύX�����\��������̂Œ���
	CharaEnemy::ProcessAttackToPlayer(sStart, sEnd, "����", 60.0f, 2, true);

	if (this->EqualPhaseFrame(13.0f)) {
		SoundManager::GetSndComponent()->PlaySound(eSE_BOOMERANG_RUN);
	} else if (this->EqualPhaseFrame(35.0f)) {
		SoundManager::GetSndComponent()->PlaySound(eSE_FALL_STONE);
		CameraManager::GetInstance()->StartShakeCamera(0.4f, 2.0f, 60.0f);
	} else if (GetIsEndMotion()) {
		ChangeState(eCHARA_STATE_IDLE);
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyWoodman::phaseStateWaitAttack(void)
{
	ChangeState(mReserveCharaState);
	mReserveCharaState = -1;
}

//---------------------------------------------------
/**
 * �L�����̍U���X�e�[�^�X�J�ڂ̃`�F�b�N�֐�
 */
//---------------------------------------------------
bool
CharaEnemyWoodman::checkChangeAttackState(int* pAttackCharaState)
{
	CharaBase* p_player = (CharaBase *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();
	VECTOR vec = VSub(p_player->Position(), mPosition);
	if (VSize(vec) < (mBodySize * 8.0f) + (mBodySize * (GetRand(200) * 0.05f))) {

		*pAttackCharaState = eCHARA_STATE_ATTACK_1;
		return true;
	}
	return false;
}

/*=====================================*
 * �~�m�^�E���X
 *=====================================*/
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyMinotaurus::Setup(const char* enemyName, int maxHitpoint, const char* layoutName)
{
	// �G�x�[�X�N���X��Setup�K�v
	CharaEnemy::Setup(enemyName, maxHitpoint, layoutName);

	mFlinchMax = 250;

	if (mWeaponModel.GetModelHandle() == -1) {
		mWeaponModel.LinkModel(RES_MDL_AX);
		mWeaponModel.Setup();
	}
	this->AttachModel(&mWeaponModel,
					  ModelPmd::eATTACHED_MODEL_NORMAL,
					  this->GetFrameIndexFromName("�E���"));
	GraphicManager::GetInstance()->EntryDrawModel(&mWeaponModel);
}

//---------------------------------------------------
/**
 * �L�����̍U���X�e�[�^�X�J�ڂ̃`�F�b�N�֐�
 */
//---------------------------------------------------
bool
CharaEnemyMinotaurus::checkChangeAttackState(int* pAttackCharaState)
{
	CharaBase* p_player = (CharaBase *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();
	VECTOR vec = VSub(p_player->Position(), mPosition);
	float dist = VSize(vec);
	
	int rand;
	bool result = false;

	if (dist < mBodySize * 1.5f) {
		rand = GetRand(6);

		if (rand == 0) {
			*pAttackCharaState = eCHARA_STATE_ATTACK_1;
			result = true;
		} else if (rand == 1) {
			*pAttackCharaState = eCHARA_STATE_ATTACK_1;
			result = true;
		} else if (rand == 2) {
			*pAttackCharaState = eCHARA_STATE_ATTACK_1;
			result = true;
		}
	}
	return result;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyMinotaurus::phaseEnterStateAttack1(void)
{
	SoundManager::GetVcComponent(mCharaName)->PlaySound(eVC_ATTACK_1);
	SoundManager::GetSndComponent()->PlaySound(eSE_SWING_SWORD);

	VECTOR weapon_top_pos, weapon_btm_pos;
	this->GetFramePosFromName(&weapon_top_pos, "�E���");
	mWeaponModel.GetFramePosFromName(&weapon_btm_pos, "����");
	mBladeEffect.Play(&weapon_top_pos, &weapon_btm_pos);

	StartMotion(eMT_KNIGHT_ATTACK_1, false, 0.45f);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyMinotaurus::phaseStateAttack1(void)
{
	static float sStart = 15.0f;
	static float sEnd = 21.0f;

	VECTOR weapon_top_pos, weapon_btm_pos;
	mWeaponModel.GetFramePosFromName(&weapon_btm_pos, "����");
	mWeaponModel.GetFramePosFromName(&weapon_top_pos, "����");

	bool is_prev_attack;
	is_prev_attack = mIsAttack;

	if (this->EqualPhaseFrame(sStart)) {
		SoundManager::GetSndComponent()->PlaySound(eSE_HEAVY_SWING);
	}
	if (EqualPhaseFrame(23.0f)) {
		const float size = 30.0f;
		VECTOR effpos;
		effpos = weapon_top_pos;
		effpos.y = mPosition.y;
		int eff_handle = EffectManager::GetInstance()->Play(eEF_DUST,
															&effpos,
															&VGet(size, size, size),
															0.0f);
		EffectManager::GetInstance()->Stop(eff_handle, 2.0f);
		SoundManager::GetSndComponent()->PlaySound(eSE_BURST);
	}

	if ((GetMotionNowTime() >= sStart) &&
		(GetMotionNowTime() <= sEnd)) {
		mIsAttack = true;

		CharaBase* p_player;
		p_player = (CharaBase *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();

		/* �퓬�s�\�҂Ɗ��ɍU�����ꂽ�҂����� */
		if ((p_player->GetIsRetire() == false) &&
			(p_player->IsDamage() == false)) {

			ShapeCapsule capsule;
			ShapeLine line;
			capsule.mRadius = p_player->BodySize() * 0.8f;
			capsule.mPosA = p_player->Position();
			capsule.mPosA.y += capsule.mRadius;
			capsule.mPosB = p_player->Position();
			capsule.mPosB.y += p_player->BodyHeight() - capsule.mRadius;

			mWeaponModel.GetFramePosFromName(&line.mStart, "����");
			mWeaponModel.GetFramePosFromName(&line.mEnd, "����");

			if (CollisionManager::Check_LineToCapsule(&line, &capsule)) {

				SoundManager::GetSndComponent()->PlaySound(eSE_HIT_SWORD);
			
				int attack_power = 4;
				VECTOR damage_down_vec;
				damage_down_vec = VSub(p_player->Position(), mPosition);
				damage_down_vec = VNorm(damage_down_vec);
				p_player->ProcessDamage(attack_power, false, NULL, &damage_down_vec, true, eATTACK_TYPE_ENEMY_NORMAL);
			}
		}

	} else {
		mIsAttack = false;
	}
	PRINT_SCREEN(GetColor(255, 255, 0), "NowTime : %.2f\n", GetMotionNowTime());

	mBladeEffect.Update(&weapon_top_pos, &weapon_btm_pos);

	if (GetIsEndMotion()) {
		mBladeEffect.Stop();
		ChangeState(eCHARA_STATE_IDLE);
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyMinotaurus::phaseEnterStateRetireEnd(void)
{
	GraphicManager::GetInstance()->ReserveRemoveModel(&mWeaponModel);

	mWeaponModel.SetAlpha(0.0f);
	mWeaponModel.DeleteModel();

	CharaEnemy::phaseEnterStateRetireEnd();
}

/*=====================================*
 * �X�R�[�s�I��
 *=====================================*/
/*static*/const char* CharaEnemyScorpion::strAttackTopNode = "����";
/*static*/const char* CharaEnemyScorpion::strAttackBtmNode = "�㕠��";
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyScorpion::phaseEnterStateAttack1(void)
{
	SoundManager::GetVcComponent(mCharaName)->PlaySound(eVC_ATTACK_1);
	SoundManager::GetSndComponent()->PlaySound(eSE_SWING_SWORD);

	StartMotion(eMT_ATTACK_1, false, 0.9f, NULL);

	VECTOR weapon_top_pos, weapon_btm_pos;
	this->GetFramePosFromName(&weapon_top_pos, strAttackTopNode);
	this->GetFramePosFromName(&weapon_btm_pos, strAttackBtmNode);

	mBladeEffect.Play(&weapon_top_pos, &weapon_btm_pos);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyScorpion::phaseStateAttack1(void)
{
	bool is_prev_attack;
	is_prev_attack = mIsAttack;

	static const float sStart	= 28.0f;
	static const float sEnd		= 43.0f;

	// ���̊֐����� mIsAttack ���ύX�����\��������̂Œ���
	CharaEnemy::ProcessAttackToPlayer(sStart, sEnd, (char *)strAttackTopNode, 20.0f, 2, true);

	PRINT_SCREEN(GetColor(255, 255, 0), "NowTime : %.2f\n", GetMotionNowTime());
	
	VECTOR weapon_top_pos, weapon_btm_pos;
	this->GetFramePosFromName(&weapon_top_pos, strAttackTopNode);
	this->GetFramePosFromName(&weapon_btm_pos, strAttackBtmNode);
	mBladeEffect.Update(&weapon_top_pos, &weapon_btm_pos);

	if (GetIsEndMotion()) {
		ChangeState(eCHARA_STATE_IDLE);
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyScorpion::phaseEnterStateRetireEnd(void)
{
	CharaEnemy::phaseEnterStateRetireEnd();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyScorpion::processNoMoveAction(void)
{
	return;		/* nop */
}

/*=====================================*
 * ���{�b�g�E�f��^�C�v
 *=====================================*/
/*static*/const char* CharaEnemyRobotA::strAttackTopNode = "�E�Ђ���";
/*static*/const char* CharaEnemyRobotA::strAttackBtmNode = "�E�Ђ�";
/*static*/const char* CharaEnemyRobotA::strAttackTopNodeB = "���Ђ���";
/*static*/const char* CharaEnemyRobotA::strAttackBtmNodeB = "���Ђ�";
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyRobotA::Setup(const char* enemyName, int maxHitpoint, const char* layoutName)
{
	// �G�x�[�X�N���X��Setup�K�v
	CharaEnemy::Setup(enemyName, maxHitpoint, layoutName);
}

//---------------------------------------------------
/**
 * �L�����̍U���X�e�[�^�X�J�ڂ̃`�F�b�N�֐�
 */
//---------------------------------------------------
bool
CharaEnemyRobotA::checkChangeAttackState(int* pAttackCharaState)
{
	CharaBase* p_player = (CharaBase *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();
	VECTOR vec = VSub(p_player->Position(), mPosition);
	if (VSize(vec) < (mBodySize * 5.0f) + (mBodySize * (GetRand(100) * 0.010f))) {
		if (GetRand(2) == 0) {
			*pAttackCharaState = eCHARA_STATE_ATTACK_2;
		} else {
			*pAttackCharaState = eCHARA_STATE_ATTACK_1;
		}
		return true;
	}
	return false;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyRobotA::phaseEnterStateIdle(void)
{
	CharaEnemy::phaseEnterStateIdle();
	mBladeEffectB.Stop();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyRobotA::phaseEnterStateAttack1(void)
{
	SoundManager::GetSndComponent()->PlaySound(eSE_SWING_SWORD);

	StartMotion(eMT_ATTACK_1, false, 0.4f, NULL);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyRobotA::phaseStateAttack1(void)
{
	float move_speed = 0.0f;
	bool is_prev_attack;
	is_prev_attack = mIsAttack;

	static const float sStart	= 20.0f;
	static const float sEnd		= 25.0f;

	// ���̊֐����� mIsAttack ���ύX�����\��������̂Œ���

	VECTOR weapon_top_pos, weapon_btm_pos;
	this->GetFramePosFromName(&weapon_top_pos, strAttackTopNode);
	this->GetFramePosFromName(&weapon_btm_pos, strAttackBtmNode);

	// �ړ��ʂ̏����@ (�U��������)
	move_speed = 0.04f * (100 - GetMotionTimePercent() );
	mMoveVec.x += (sin(mRotation.y + Utility::DegToRad(180.0f) ) * move_speed);
	mMoveVec.z += (cos(mRotation.y + Utility::DegToRad(180.0f) ) * move_speed);

	CharaBase* p_player = (CharaBase *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();

	if (this->EqualPhaseFrame(2.0f) ) {
		// �e�[���G�t�F�N�g�J�n
		mBladeEffect.Play(&weapon_top_pos, &weapon_btm_pos);
		SoundManager::GetSndComponent()->PlaySound(eSE_HEAVY_SWING);
	} else if (this->BetweenPhaseFrame(sStart, sEnd) ) {

		// �ړ��ʂ̏����A (�U����)
		move_speed = 0.42f * (100 - GetMotionTimePercent() );
		mMoveVec.x += (sin(mRotation.y + Utility::DegToRad(180.0f) ) * move_speed);
		mMoveVec.z += (cos(mRotation.y + Utility::DegToRad(180.0f) ) * move_speed);

		// �e�[���G�t�F�N�g�X�V
		mBladeEffect.Update(&weapon_top_pos, &weapon_btm_pos);

		// �U���̂����蔻��
		if ( !(p_player->GetIsRetire()) && !(p_player->IsDamage()) ) {
			ShapeCapsule capsuleA, capsuleB;
			p_player->GetBodyCapsule(&capsuleA);
			capsuleB.mPosA = weapon_top_pos;
			capsuleB.mPosB = weapon_btm_pos;
			capsuleB.mRadius = 36.0f;
			if (CollisionManager::Check_CapsuleToCapsule(&capsuleA, &capsuleB) ) {
				SoundManager::GetSndComponent()->PlaySound(eSE_HIT_SWORD);
				VECTOR damage_down_vec;
				damage_down_vec = VSub(p_player->Position(), mPosition);
				damage_down_vec = VNorm(damage_down_vec);
				p_player->ProcessDamage(3, false, NULL, &damage_down_vec, true, eATTACK_TYPE_ENEMY_NORMAL);
			}
		}
	}

	PRINT_SCREEN(GetColor(255, 255, 0), "NowTime : %.2f\n", GetMotionNowTime());

	// �I�����菈��
	if (GetIsEndMotion()) {
		ChangeState(eCHARA_STATE_IDLE);
	}
}

//---------------------------------------------------
/**
 * ��]�A�^�b�N
 */
//---------------------------------------------------
void
CharaEnemyRobotA::phaseEnterStateAttack2(void)
{
	SoundManager::GetVcComponent(mCharaName)->PlaySound(eVC_ATTACK_2);

	StartMotion(eMT_ATTACK_2, false, 0.5f, NULL);
}

//---------------------------------------------------
/**
 * ��]�A�^�b�N
 */
//---------------------------------------------------
void
CharaEnemyRobotA::phaseStateAttack2(void)
{
	float move_speed = 0.0f;
	bool is_prev_attack;
	is_prev_attack = mIsAttack;

	static const float sStart	= 36.0f;
	static const float sEnd		= 66.0f;

	VECTOR weapon_top_pos, weapon_btm_pos;
	this->GetFramePosFromName(&weapon_top_pos, strAttackTopNode);
	this->GetFramePosFromName(&weapon_btm_pos, strAttackBtmNode);
	VECTOR weapon_top_posB, weapon_btm_posB;
	this->GetFramePosFromName(&weapon_top_posB, strAttackTopNodeB);
	this->GetFramePosFromName(&weapon_btm_posB, strAttackBtmNodeB);

	CharaBase* p_player = (CharaBase *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();


	if ( (this->EqualPhaseFrame(34.0f)) ||
		 (this->EqualPhaseFrame(42.0f)) ||
		 (this->EqualPhaseFrame(50.0f)) ||
		 (this->EqualPhaseFrame(58.0f)) ) {
		SoundManager::GetSndComponent()->PlaySound(eSE_HEAVY_SWING);
	}

	if (this->EqualPhaseFrame(sStart) ) {
		// �e�[���G�t�F�N�g�J�n
		mBladeEffect.Play(&weapon_top_pos, &weapon_btm_pos);
		mBladeEffectB.Play(&weapon_top_posB, &weapon_btm_posB);

	} else if (this->BetweenPhaseFrame(sStart, sEnd) ) {
		// �e�[���G�t�F�N�g�X�V
		mBladeEffect.Update(&weapon_top_pos, &weapon_btm_pos);
		// �e�[���G�t�F�N�g�X�V
		mBladeEffectB.Update(&weapon_top_posB, &weapon_btm_posB);

		// �U���̂����蔻��
		if ( !(p_player->GetIsRetire()) && !(p_player->IsDamage()) ) {
			bool is_hitA = false;
			bool is_hitB = false;
			ShapeCapsule capsuleA, capsuleB;
			p_player->GetBodyCapsule(&capsuleA);
			capsuleB.mPosA = weapon_top_pos;
			capsuleB.mPosB = weapon_btm_pos;
			capsuleB.mRadius = 20.0f;
			is_hitA = (CollisionManager::Check_CapsuleToCapsule(&capsuleA, &capsuleB) );

			capsuleB.mPosA = weapon_top_posB;
			capsuleB.mPosB = weapon_btm_posB;
			capsuleB.mRadius = 20.0f;
			is_hitB = (CollisionManager::Check_CapsuleToCapsule(&capsuleA, &capsuleB) );

			if ( (is_hitA == true) || (is_hitB == true) ) {
				SoundManager::GetSndComponent()->PlaySound(eSE_HIT_SWORD);
				VECTOR damage_down_vec;
				damage_down_vec = VSub(p_player->Position(), mPosition);
				damage_down_vec = VNorm(damage_down_vec);

				bool is_damage_down = false;
				if (this->BetweenPhaseFrame(50.0f, 66.0f)) {
					is_damage_down = true;
				}
				p_player->ProcessDamage(2, false, NULL, &damage_down_vec, is_damage_down, eATTACK_TYPE_ENEMY_NORMAL);
			}
		}
	}

	PRINT_SCREEN(GetColor(255, 255, 0), "NowTime : %.2f\n", GetMotionNowTime());

	// �I�����菈��
	if (GetIsEndMotion()) {
		ChangeState(eCHARA_STATE_IDLE);
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyRobotA::phaseStateWaitAttack(void)
{
	ChangeState(mReserveCharaState);
	mReserveCharaState = -1;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyRobotA::phaseEnterStateRetireEnd(void)
{
	CharaEnemy::phaseEnterStateRetireEnd();
	mBladeEffectB.Stop();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
CharaEnemyRobotB::CharaEnemyRobotB()
{
	mStatePhase = 0;
	mStateCount = 0;
}

//---------------------------------------------------
/**
 * �e�U��
 */
//---------------------------------------------------
void
CharaEnemyRobotB::phaseEnterStateAttack2(void)
{
	SoundManager::GetVcComponent(mCharaName)->PlaySound(eVC_ATTACK_2);

	mStatePhase = 0;
	mStateCount = 0;

	int rnd_value = GetRand(2);
	StartMotion( (eMT_ATTACK_2) + rnd_value, false, 0.5f, NULL);
}

//---------------------------------------------------
/**
 * �e�U��
 */
//---------------------------------------------------
void
CharaEnemyRobotB::phaseStateAttack2(void)
{
	CharaBase* p_player = (CharaBase *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();

	VECTOR weapon_top_posA, weapon_btm_posA;
	VECTOR weapon_top_posB, weapon_btm_posB;
	this->GetFramePosFromName(&weapon_top_posA, strAttackTopNode);
	this->GetFramePosFromName(&weapon_top_posB, strAttackTopNodeB);
	this->GetFramePosFromName(&weapon_btm_posA, strAttackBtmNode);
	this->GetFramePosFromName(&weapon_btm_posB, strAttackBtmNodeB);

	VECTOR* p_top_pos_tbl[2] = {&weapon_top_posA, &weapon_top_posB};
	VECTOR* p_btm_pos_tbl[2] = {&weapon_btm_posA, &weapon_btm_posB};
	const char* str_top_pos_node[2]= {strAttackTopNode, strAttackTopNodeB};

	switch (mStatePhase) {
	case 0:
		if (GetIsEndMotion() == false) break;
		mStatePhase ++; mStateCount = 0;
		break;

	case 1:
		{
			EFK_HANDLE efk_hdl = NULL;
			VECTOR eff_pos;
			float target_rot;
			VECTOR vecZ, crossX, crossY;

			for (int iI = 0; iI < 2; iI ++) {
				vecZ = VNorm(VSub(*p_top_pos_tbl[iI], *p_btm_pos_tbl[iI]) );
				crossX = VNorm(VCross(vecZ, VGet(0.0f, 1.0f, 0.0f)));
				crossY = VNorm(VCross(vecZ, crossX));

				target_rot = atan2(crossY.x, crossY.z);
				Utility::CorrectAngleFrom000To360(&target_rot);

				this->GetFramePosFromName(&eff_pos, str_top_pos_node[iI]);
				efk_hdl = EffekseerManager::GetInstance()->Play(eEFK_LASER, &eff_pos, 100.0f, 0.0f);

				EffekseerManager::GetInstance()->SetRotationAxis(efk_hdl, &crossY, -target_rot );
			}

			SoundManager::GetSndComponent()->PlaySound(eSE_LASER);

			mStatePhase ++; mStateCount = 0;
		}
		break;
	case 2:

		/* �����蔻�菈�� */
		if (p_player->IsDamage() == false) {

			ShapeCapsule capsuleA, capsuleB;
			float laser_len  = (1800.0f * ( (float)mStateCount / 60.0f) );
			float laser_size = 24.0f;

			p_player->GetBodyCapsule(&capsuleA);

			for (int iI = 0; iI < 2; iI ++) {
				capsuleB.mPosA = *p_top_pos_tbl[iI];
				capsuleB.mPosB = VNorm(VSub(*p_top_pos_tbl[iI], *p_btm_pos_tbl[iI]) );
				capsuleB.mPosB = VAdd( VScale(capsuleB.mPosB, laser_len), *p_top_pos_tbl[iI]);
				capsuleB.mRadius = laser_size;
#ifdef _DEBUG
				GraphicManager::GetInstance()->DbgRenderCapsule(&capsuleB);
#endif // #ifdef _DEBUG
				if (CollisionManager::GetInstance()->Check_CapsuleToCapsule(&capsuleA, &capsuleB)) {
					SoundManager::GetSndComponent()->PlaySound(eSE_HIT_SWORD);
					int attack_power = 2;
					VECTOR damage_down_vec;
					damage_down_vec = VSub(p_player->Position(), capsuleB.mPosA);
					damage_down_vec.y = 0.0f;
					damage_down_vec = VNorm(damage_down_vec);
					p_player->ProcessDamage(attack_power, false, NULL, &damage_down_vec, true, eATTACK_TYPE_ENEMY_NORMAL);
					break;
				}
			}
		}
		if (mStateCount > 60) {
			ChangeState(eCHARA_STATE_IDLE);
		}
		break;

	default:
		break;
	};

	mStateCount ++;

}

//---------------------------------------------------
/**
 * �L�����̍U���X�e�[�^�X�J�ڂ̃`�F�b�N�֐�
 */
//---------------------------------------------------
bool
CharaEnemyRobotB::checkChangeAttackState(int* pAttackCharaState)
{
	CharaBase* p_player = (CharaBase *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();
	VECTOR vec = VSub(p_player->Position(), mPosition);
	if (VSize(vec) < (mBodySize * 15.0f) + (mBodySize * (GetRand(100) * 0.010f))) {
		if (GetRand(2) == 0) {
			*pAttackCharaState = eCHARA_STATE_ATTACK_2;
		}
		else {
			*pAttackCharaState = eCHARA_STATE_ATTACK_2;
		}
		return true;
	}
	return false;
}

/*=====================================*
 * ���y���R�v�^�[
 *=====================================*/
/*static*/const char* CharaEnemyHelicopter::strAttackTopNode = "�U����";
/*static*/const char* CharaEnemyHelicopter::strAttackBtmNode = "�U����";
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
CharaEnemyHelicopter::CharaEnemyHelicopter()
{
	mStatePhase	= 0;
	mStateCount = 0;
	mTmpVec.mEscapePos	= ZERO_VEC;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyHelicopter::Setup(const char* enemyName, int maxHitpoint, const char* layoutName)
{
	// �G�x�[�X�N���X��Setup�K�v
	CharaEnemy::Setup(enemyName, maxHitpoint, layoutName);
	mConstGravitySpeed = 0.0f;	// ���d�ʂɐݒ�
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyHelicopter::phaseStateIdle(void)
{
	CharaBase* p_player = (CharaBase *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();
	if (p_player->GetIsRetire() == false) {
		if (GetRand(10) == 0) {
			VECTOR vec;
			vec = VSub(p_player->Position(), mPosition);

			// �ԍ����̍��W��T��
			float escape_dist	= 500.0f;
			float escape_height	= (100.0f + (50.0f * GetRand(6) ) );

			// �v���C���[�ɑ΂��ĉ�荞�ފp�x
			float escape_rot = p_player->Rotation().y - PHI_F;
			escape_rot += Utility::DegToRad( (15.0f * GetRand(12) ) );
			Utility::CorrectAngleFrom000To360(&escape_rot);

			mTmpVec.mEscapePos = p_player->Position();
			mTmpVec.mEscapePos.x += sin(escape_rot) * escape_dist;
			mTmpVec.mEscapePos.y += escape_height;
			mTmpVec.mEscapePos.z += cos(escape_rot) * escape_dist;

			vec = VSub(mTmpVec.mEscapePos, mPosition);
			mTargetRotY = atan2(vec.x, vec.z) + PHI_F;

			ChangeState(eCHARA_STATE_TURN);
		}
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyHelicopter::phaseStateMove(void)
{
	VECTOR dir_vec;
	dir_vec = VSub(mTmpVec.mEscapePos, mPosition);
	if (VSize(dir_vec) > mMoveSpeed) {
		mMoveVec = VScale(VNorm(dir_vec), mMoveSpeed);
	} else {
		mPosition = mTmpVec.mEscapePos;
		ChangeState(eCHARA_STATE_ATTACK_1);
	}

	for (int iI = (8 - 1); iI > 0; iI --) {
		mMoveHistory[iI] = mMoveHistory[iI - 1];
	}
	mMoveHistory[0] = mPosition;
	// �S�R�ړ����Ă��Ȃ��ꍇ�̏���
	if (VSize(VSub(mMoveHistory[0], mMoveHistory[7])) < mMoveSpeed * 0.1f) {
		ChangeState(eCHARA_STATE_IDLE);
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyHelicopter::phaseEnterStateJumpUp(void)
{
	ChangeState(eCHARA_STATE_IDLE);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyHelicopter::phaseStateJumpUp(void)
{

}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyHelicopter::phaseEnterStateJumpDown(void)
{
	ChangeState(eCHARA_STATE_IDLE);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyHelicopter::phaseStateJumpDown(void)
{

}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyHelicopter::phaseStateAppear(void)
{
	if (((mAppearWaitMax >= mAppearWaitCount) && 
		 (mAppearWaitMax < mAppearWaitCount + (SHORT)kBASIC_FRAME_COUNTER))) {
		
		mPosition.y += 400.0f;

		VECTOR effpos = VAdd(mPosition, mMoveVec);
		int eff_handle = EffectManager::GetInstance()->Play(eEF_BLACKDUST, &effpos, 50.0f, 0.0f);
		EffectManager::GetInstance()->Stop(eff_handle, 2.0f);
		SoundManager::GetSndComponent()->PlaySound(eSE_ENEMY_APPEAR);
		
		mIsLand = false;
		//<! �_���[�W��Ԃ�����
		mIsDamage = false;
		this->SetAlpha(1.0f);
	}

	if (mAppearWaitMax < mAppearWaitCount) {
		ChangeState(eCHARA_STATE_IDLE);
	}
	mAppearWaitCount += (SHORT)kBASIC_FRAME_COUNTER;
}

//---------------------------------------------------
/**
 *
 */
//---------------------------------------------------
void
CharaEnemyHelicopter::phaseEnterStateAttack1(void)
{
	CharaBase* p_player = (CharaBase *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();
	VECTOR vec;
	
	vec = VSub(p_player->Position(), mPosition);
	mTargetRotY = atan2(vec.x, vec.z) + PHI_F;
}

//---------------------------------------------------
/**
 *
 */
//---------------------------------------------------
void
CharaEnemyHelicopter::phaseStateAttack1(void)
{
	float turn_move_speed = 60.0f * kTURN_MOVE_ROT / mBodySize;
	processTurnTargetRotY(turn_move_speed, eCHARA_STATE_ATTACK_2);
}

//---------------------------------------------------
/**
 * �e�U��
 */
//---------------------------------------------------
void
CharaEnemyHelicopter::phaseEnterStateAttack2(void)
{
	SoundManager::GetVcComponent(mCharaName)->PlaySound(eVC_ATTACK_2);
	mStatePhase = 0;
	mStateCount = 0;

	// �e����������x�N�g�������߂�
	CharaBase* p_player = (CharaBase *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();
	mTmpVec.mAimPos = p_player->Position();
	mTmpVec.mAimPos.y += p_player->BodyHeight() * 0.5f;
}

//---------------------------------------------------
/**
 * �e�U��
 */
//---------------------------------------------------
void
CharaEnemyHelicopter::phaseStateAttack2(void)
{
	CharaBase* p_player = (CharaBase *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();

	VECTOR aim_pos, weapon_top_pos;
	this->GetFramePosFromName(&weapon_top_pos, strAttackTopNode);
	aim_pos = mTmpVec.mAimPos;

	EFK_HANDLE efk_hdl = NULL;

	switch (mStatePhase) {
	case 0:
		if (mStateCount > 20) {
			mStatePhase ++; mStateCount = 0;
		}
		break;

	case 1:
		{
			MATRIX mtx, tmp;
			mtx = MGetIdent();
			// �g�k
			tmp = MGetScale(VGet(50.0f, 50.0f, 50.0f) );
			mtx = MMult(mtx, tmp);
			// ��]
			Utility::MTXLookAtMatrix(&tmp, &aim_pos, &weapon_top_pos, (VECTOR *)&UP_VEC);
			mtx = MMult(mtx, tmp);
			// �ړ�
			tmp = MGetTranslate(weapon_top_pos);
			mtx = MMult(mtx, tmp);
			
			efk_hdl = EffekseerManager::GetInstance()->PlayMtx(eEFK_LASER, &mtx);

			EffekseerManager::GetInstance()->SetSpeed(efk_hdl, 0.75f);
			SoundManager::GetSndComponent()->PlaySound(eSE_LASER);

			mStatePhase ++; mStateCount = 0;
		}
		break;
	case 2:

		/* �����蔻�菈�� */
		if (p_player->IsDamage() == false) {

			ShapeCapsule capsuleA, capsuleB;
			float laser_len  = (200.0f * ( (float)mStateCount / 10.0f) );
			float laser_size = 12.0f;

			p_player->GetBodyCapsule(&capsuleA);
			capsuleB.mPosA = weapon_top_pos;
			capsuleB.mPosB = VNorm(VSub(aim_pos, weapon_top_pos) );
			capsuleB.mPosB = VAdd( VScale(capsuleB.mPosB, laser_len), weapon_top_pos);
			capsuleB.mRadius = laser_size;
#ifdef _DEBUG
			GraphicManager::GetInstance()->DbgRenderCapsule(&capsuleB);
#endif // #ifdef _DEBUG
			if (CollisionManager::GetInstance()->Check_CapsuleToCapsule(&capsuleA, &capsuleB)) {
				SoundManager::GetSndComponent()->PlaySound(eSE_HIT_SWORD);
				int attack_power = 2;
				VECTOR damage_down_vec;
				damage_down_vec = VSub(p_player->Position(), capsuleB.mPosA);
				damage_down_vec.y = 0.0f;
				damage_down_vec = VNorm(damage_down_vec);
				p_player->ProcessDamage(attack_power, false, NULL, &damage_down_vec, true, eATTACK_TYPE_ENEMY_NORMAL);
			}
		}
		if (mStateCount > 60) {
			mStatePhase ++; mStateCount = 0;
		}
		break;
		
	case 3:
		if (mStateCount > 60) {
			ChangeState(eCHARA_STATE_IDLE);
		}
		break;

	default:
		break;
	};

	mStateCount ++;

}

//---------------------------------------------------
/**
 * �L�����̍U���X�e�[�^�X�J�ڂ̃`�F�b�N�֐�
 */
//---------------------------------------------------
bool
CharaEnemyHelicopter::checkChangeAttackState(int* pAttackCharaState)
{
	CharaBase* p_player = (CharaBase *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();
	VECTOR vec = VSub(p_player->Position(), mPosition);
	if (VSize(vec) < (mBodySize * 15.0f) + (mBodySize * (GetRand(100) * 0.010f))) {
		if (GetRand(2) == 0) {
			*pAttackCharaState = eCHARA_STATE_ATTACK_1;
			return true;
		}
	}
	return false;
}



/*=====================================*
 * SA-14
 *=====================================*/
/*static*/const char* CharaEnemySA_14::strAttackBtmNode = "�Emg�}�Y��";
/*static*/const char* CharaEnemySA_14::strAttackTopNode = "�Emg�}�Y����";
/*static*/const char* CharaEnemySA_14::strAttackTopNodeLeft = "���n1";
/*static*/const char* CharaEnemySA_14::strAttackBtmNodeLeft = "���d�r";

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
CharaEnemySA_14::CharaEnemySA_14()
{
	for (int iI = 0; iI < kEFFECT_NUM; iI ++) {
		mEfsHandle[iI] = -1;
	}
	memset(mEffTargetVec, 0, sizeof(mEffTargetVec));
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemySA_14::Setup(const char* enemyName, int maxHitpoint, const char* layoutName)
{
	// �G�x�[�X�N���X��Setup�K�v
	CharaEnemy::Setup(enemyName, maxHitpoint, layoutName);
	mFlinchMax = 200;

	int model_handle = -1;
	COLOR_F color;

	for (int iI = 0; iI < kEFFECT_NUM; iI ++) {
		if (mShotModel[iI].GetModelHandle() == -1) {
			mShotModel[iI].LinkModel(RES_MDL_LOCK_CENTER);
			mShotModel[iI].Setup();
		}
		mShotModel[iI].Scale() = ZERO_VEC;

		// �}�e���A���J���[��ύX
		model_handle = mShotModel[iI].GetModelHandle();
		color = GetColorF(0.01f, 0.5f, 1.0f, 1.0f);

		MV1SetMaterialDifColor(model_handle, 0, color);
		MV1SetMaterialSpcColor(model_handle, 0, color);
		MV1SetMaterialAmbColor(model_handle, 0, color);
		MV1SetMaterialEmiColor(model_handle, 0, color);
		MV1SetUseVertDifColor(model_handle, FALSE);
		MV1SetUseVertSpcColor(model_handle, FALSE);

		GraphicManager::GetInstance()->EntryDrawModel(&mShotModel[iI]);
	}
}

//---------------------------------------------------
/**
 * �L�����̍U���X�e�[�^�X�J�ڂ̃`�F�b�N�֐�
 */
//---------------------------------------------------
bool
CharaEnemySA_14::checkChangeAttackState(int* pAttackCharaState)
{
	CharaBase* p_player = (CharaBase *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();
	VECTOR vec = VSub(p_player->Position(), mPosition);

	if (VSize(vec) < (mBodySize * 15.0f) + (mBodySize * (GetRand(100) * 0.010f))) {
		if (GetRand(160) == 0) {
			*pAttackCharaState = eCHARA_STATE_ATTACK_1;
			return true;
		}
	}
	if (VSize(vec) < (mBodySize * 2.0f) + (mBodySize * (GetRand(100) * 0.010f))) {
		*pAttackCharaState = eCHARA_STATE_ATTACK_2;
		return true;
	}


	return false;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemySA_14::phaseEnterStateIdle(void)
{
	CharaEnemy::phaseEnterStateIdle();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemySA_14::phaseStateMove(void)
{
	CharaEnemy::phaseStateMove();

	if (EqualPhaseFrame(4) || EqualPhaseFrame(20)) {
		SoundManager::GetSndComponent()->PlaySound(eSE_FOOT_STEP_ROBOT);

		CameraManager::GetInstance()->StartVibeCamera(0.4f, 2.0f, 10.0f);
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemySA_14::phaseEnterStateAttack1(void)
{
	SoundManager::GetSndComponent()->PlaySound(eSE_CANNON_LASER_CHARGE);

	StartMotion(eMT_ATTACK_1, false, 1.0f, NULL);

	mStatePhase = 0;
	mStateCount = 0;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemySA_14::phaseStateAttack1(void)
{
	CharaBase* p_player = (CharaBase *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();
	VECTOR weapon_btm_pos, weapon_top_pos;
	EFK_HANDLE handle;

	GetFramePosFromName(&weapon_btm_pos, strAttackBtmNode);
	GetFramePosFromName(&weapon_top_pos, strAttackTopNode);

	switch (mStatePhase) {
	case 0:
		SetRotationToTarget(&p_player->Position());
		for (int iI = 0; iI < kEFFECT_NUM; iI ++) {
			mEfsHandle[iI] = EffectManager::GetInstance()->Play(eEF_NAVI,
																&weapon_btm_pos,
																60.0f,
																1.0f);
		}
		SoundManager::GetVcComponent(mCharaName)->PlaySound(eVC_ATTACK_1);
		NEXT_SUB_STATE();

		break;

	case 1:
		for (int iI = 0; iI < kEFFECT_NUM; iI ++) {
			EffectManager::GetInstance()->Position(mEfsHandle[iI]) = weapon_btm_pos;
		}
		if (mStateCount >= 50) {

			float eff_target_len = 43.0f;
			mEffTargetVec[0] = VNorm(VSub(weapon_top_pos, weapon_btm_pos));

			VECTOR crossX;
			crossX = VNorm(VCross(mEffTargetVec[0], UP_VEC));
			
			mEffTargetVec[1] = VAdd(mEffTargetVec[0], crossX);
			mEffTargetVec[2] = VSub(mEffTargetVec[0], crossX);
			
			mEffTargetVec[0] = VScale(mEffTargetVec[0], eff_target_len);
			mEffTargetVec[1] = VScale(mEffTargetVec[1], eff_target_len);
			mEffTargetVec[2] = VScale(mEffTargetVec[2], eff_target_len);

			handle = EffekseerManager::GetInstance()->Play(eEFK_EXPLOSION_BOMB, &weapon_btm_pos, 5.0f, 0.0f);

			for (int iI = 0; iI < kEFFECT_NUM; iI ++) {
				mShotModel[iI].Scale() = VGet(3.0f, 3.0f, 3.0f);
				mShotModel[iI].SetAlpha(0.75f);
			}

			CameraManager::GetInstance()->StartShakeCamera(0.2f, 2.0f, 30.0f);

			SoundManager::GetSndComponent()->PlaySound(eSE_BURST);

			NEXT_SUB_STATE();
		}
		break;

	case 2:
		VECTOR effpos;
		for (int iI = 0; iI < kEFFECT_NUM; iI ++) {
			effpos = EffectManager::GetInstance()->Position(mEfsHandle[iI]);
			effpos = VAdd(effpos, mEffTargetVec[iI]);
			EffectManager::GetInstance()->Position(mEfsHandle[iI]) = effpos;
			mShotModel[iI].Position() = effpos;
			mShotModel[iI].Rotation().z += 0.6f;

			mEffTargetVec[iI] = VScale(mEffTargetVec[iI], 0.99f);
		}		

		if (VSize(mEffTargetVec[0]) < 10.0f) {
			for (int iI = 0; iI < kEFFECT_NUM; iI ++) {
				if (mEfsHandle[iI] != -1) {
					EffectManager::GetInstance()->Stop(mEfsHandle[iI], 0.1f);
					mEfsHandle[iI] = -1;
				}
				mShotModel[iI].Scale() = ZERO_VEC;
			}
			ChangeState(eCHARA_STATE_IDLE);
			return;
		}		

		/* �����蔻�菈�� */
		if (p_player->IsDamage() == false) {
			VECTOR eff_pos;
			float eff_scale;
			ShapeCapsule capsuleA, capsuleB;

			for (int iI = 0; iI < kEFFECT_NUM; iI ++) {
				eff_pos = EffectManager::GetInstance()->Position(mEfsHandle[iI]);
				eff_scale = EffectManager::GetInstance()->Scale(mEfsHandle[iI]).x;
		
				capsuleA.mPosA = p_player->Position();
				capsuleA.mPosB = p_player->Position();
				capsuleA.mPosA.y += p_player->BodySize();
				capsuleA.mPosB.y += (p_player->BodyHeight() - p_player->BodySize());
				capsuleA.mRadius = p_player->BodySize();

				capsuleB = ShapeCapsule(eff_pos, eff_pos, eff_scale);
				capsuleB.mPosB.y += 1.0f;
#ifdef _DEBUG
				GraphicManager::GetInstance()->DbgRenderCapsule(&capsuleA);
				GraphicManager::GetInstance()->DbgRenderCapsule(&capsuleB);
#endif // #ifdef _DEBUG
				if (CollisionManager::GetInstance()->Check_CapsuleToCapsule(&capsuleA, &capsuleB)) {
					SoundManager::GetSndComponent()->PlaySound(eSE_HIT_SWORD);
					int attack_power = 2;
					VECTOR damage_down_vec;
					damage_down_vec = VSub(p_player->Position(), capsuleB.mPosA);
					damage_down_vec.y = 0.0f;
					damage_down_vec = VNorm(damage_down_vec);
					p_player->ProcessDamage(attack_power, false, NULL, &damage_down_vec, true, eATTACK_TYPE_ENEMY_NORMAL);
				}
			}

		}

		break;

	default:
		break;
	};

	mStateCount ++;

}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemySA_14::phaseEnterStateAttack2(void)
{
	SoundManager::GetVcComponent(mCharaName)->PlaySound(eVC_ATTACK_2);
	SoundManager::GetSndComponent()->PlaySound(eSE_SWING_SWORD);
	StartMotion(eMT_GOBURIN_ATTACK_2, false, 0.4f);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemySA_14::phaseStateAttack2(void)
{
	static float sStart = 15.0f;
	static float sEnd = 20.0f;

	VECTOR weapon_btm_pos, weapon_top_pos;
	GetFramePosFromName(&weapon_btm_pos, strAttackTopNodeLeft);
	GetFramePosFromName(&weapon_top_pos, strAttackBtmNodeLeft);

	bool is_prev_attack;

	float move_speed = 0.08f * GetMotionTimePercent();
	mMoveVec.x += -(sin(mRotation.y) * move_speed);
	mMoveVec.z += -(cos(mRotation.y) * move_speed);

	if (EqualPhaseFrame(sStart)) {
		mBladeEffect.Play(&weapon_top_pos, &weapon_btm_pos);
		SoundManager::GetSndComponent()->PlaySound(eSE_HEAVY_SWING);
	}

	is_prev_attack = mIsAttack;
	if (BetweenPhaseFrame(sStart, sEnd)) {

		mBladeEffect.Update(&weapon_top_pos, &weapon_btm_pos);
		
		mIsAttack = true;

		/****/
		CharaBase* p_player;
		p_player = (CharaBase *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();

		/* �퓬�s�\�҂Ɗ��ɍU�����ꂽ�҂����� */
		if ((p_player->GetIsRetire() == false) &&
			(p_player->IsDamage() == false)) {

			ShapeCapsule capsuleA;
			ShapeCapsule capsuleB;
			capsuleA.mPosA	= p_player->Position();
			capsuleA.mPosB	= p_player->Position();
			capsuleA.mPosB.y += p_player->BodyHeight();
			capsuleA.mRadius= p_player->BodySize();
		
			capsuleB.mPosA = weapon_top_pos;
			capsuleB.mPosB = weapon_btm_pos;
			capsuleB.mRadius = 60.0f;
			
			if (CollisionManager::GetInstance()->Check_CapsuleToCapsule(&capsuleA, &capsuleB)) {
				SoundManager::GetSndComponent()->PlaySound(eSE_HIT_SWORD);
				int attack_power = 2;
				VECTOR damage_down_vec;
				damage_down_vec = VSub(p_player->Position(), mPosition);
				damage_down_vec = VNorm(damage_down_vec);
				p_player->ProcessDamage(attack_power, false, NULL, &damage_down_vec, false, eATTACK_TYPE_ENEMY_NORMAL);
			}
		}
	} else {
		mIsAttack = false;
	}

	PRINT_SCREEN(GetColor(255, 255, 0), "NowTime : %.2f\n", GetMotionNowTime());

	if (GetIsEndMotion()) {
		mBladeEffect.Stop();
		ChangeState(eCHARA_STATE_IDLE);
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemySA_14::phaseStateWaitAttack(void)
{
	ChangeState(mReserveCharaState);
	mReserveCharaState = -1;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemySA_14::phaseEnterStateDamage(void)
{
	mBladeEffect.Stop();
	for (int iI = 0; iI < kEFFECT_NUM; iI ++) {
		if (mEfsHandle[iI] != -1) {
			EffectManager::GetInstance()->Stop(mEfsHandle[iI], 0.1f);
			mEfsHandle[iI] = -1;
		}
		mShotModel[iI].Scale() = ZERO_VEC;
	}

	CharaEnemy::phaseEnterStateDamage();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemySA_14::phaseEnterStateDownStart(void)
{
	mBladeEffect.Stop();
	for (int iI = 0; iI < kEFFECT_NUM; iI ++) {
		if (mEfsHandle[iI] != -1) {
			EffectManager::GetInstance()->Stop(mEfsHandle[iI], 0.1f);
			mEfsHandle[iI] = -1;
		}
		mShotModel[iI].Scale() = ZERO_VEC;
	}

	CharaEnemy::phaseEnterStateDownStart();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemySA_14::phaseEnterStateRetireEnd(void)
{
	for (int iI = 0; iI < kEFFECT_NUM; iI ++) {
		GraphicManager::GetInstance()->ReserveRemoveModel(&mShotModel[iI]);
		mShotModel[iI].SetAlpha(0.0f);
		mShotModel[iI].DeleteModel();
	}

	mBladeEffect.Stop();
	for (int iI = 0; iI < kEFFECT_NUM; iI ++) {
		if (mEfsHandle[iI] != -1) {
			EffectManager::GetInstance()->Stop(mEfsHandle[iI], 0.1f);
			mEfsHandle[iI] = -1;
		}
		mShotModel[iI].Scale() = ZERO_VEC;
	}

	CharaEnemy::phaseEnterStateRetireEnd();
}


 /**** end of file ****/


