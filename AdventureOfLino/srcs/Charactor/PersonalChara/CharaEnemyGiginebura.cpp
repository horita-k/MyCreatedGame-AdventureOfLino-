/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �M�M�l�u���t�@�C��
 * @author SPATZ.
 * @data   2012/05/25 00:27:32
 */
//---------------------------------------------------
#include "CharaEnemyGiginebura.h"
#include "Charactor/CharaInfo.h"
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

static const tCapsuleWork sCapsuleWork[] = {
	{"����",	"��",		70.0f,},
	{"�K���P",	"�K����",	60.0f,},
	//	{"��",		"�K���P",	100.0f,},		// ����
	{"���Ђ�",	"�����",	60.0f,},
	{"�E�Ђ�",	"�E���",	60.0f,},
};
static const int kCAPSULE_NUM_GIGINEBURA = sizeof(sCapsuleWork) / sizeof(tCapsuleWork);

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyGiginebura::Setup(const char* enemyName, int maxHitpoint, const char* layoutName)
{
	// �G�x�[�X�N���X��Setup�K�v
	CharaEnemy::Setup(enemyName, maxHitpoint, layoutName);

	mFlinchMax = 190;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
CharaEnemyGiginebura::CheckDamageCollNormal(void* pParam)
{
	tCheckDamageWork* p_work = (tCheckDamageWork *)pParam;

	ShapeCapsule capsule;
	ShapeLine	line;

	line.mStart = p_work->mWeaponBtmPos;
	line.mEnd	= p_work->mWeaponTopPos;

	for (int iI = 0; iI < kCAPSULE_NUM_GIGINEBURA; iI ++) {
		this->GetFramePosFromName(&capsule.mPosA, sCapsuleWork[iI].mPosA);
		this->GetFramePosFromName(&capsule.mPosB, sCapsuleWork[iI].mPosB);
		capsule.mRadius = sCapsuleWork[iI].mRadius;
#ifdef _DEBUG
		GraphicManager::GetInstance()->DbgRenderCapsule(&capsule);
#endif // _DEBUG
		if (CollisionManager::GetInstance()->Check_LineToCapsule(&line, &capsule)) {
			return true;
		}
	}

	ShapeBoard board;
	this->GetFramePosFromName(&board.mPosA, "���Ђ�");
	this->GetFramePosFromName(&board.mPosB, "�����");
	this->GetFramePosFromName(&board.mPosC, "������");
	this->GetFramePosFromName(&board.mPosD, "���r");
#ifdef _DEBUG
		GraphicManager::GetInstance()->DbgRenderBoard(&board);
#endif // _DEBUG
	if (CollisionManager::GetInstance()->Check_LineToBoard(&line, &board)) {
		return true;
	}
	this->GetFramePosFromName(&board.mPosA, "�E�Ђ�");
	this->GetFramePosFromName(&board.mPosB, "�E���");
	this->GetFramePosFromName(&board.mPosC, "�E����");
	this->GetFramePosFromName(&board.mPosD, "�E�r");
#ifdef _DEBUG
		GraphicManager::GetInstance()->DbgRenderBoard(&board);
#endif // _DEBUG
	if (CollisionManager::GetInstance()->Check_LineToBoard(&line, &board)) {
		return true;
	}

	return false;
}

//---------------------------------------------------
/**
 * �L�����̍U���X�e�[�^�X�J�ڂ̃`�F�b�N�֐�
 */
//---------------------------------------------------
bool
CharaEnemyGiginebura::checkChangeAttackState(int* pAttackCharaState)
{
	CharaBase* p_player = (CharaBase *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();
	VECTOR vec = VSub(p_player->Position(), mPosition);
	float dist = VSize(vec);
	
	int rand;
	bool result = false;

	if (dist < mBodySize * 1.5f) {
		float rate = mBodySize / dist;
		rand = GetRand((int)((1.0f - rate) * 30.0f) + 1);

		if (rand > 2) {
			int pattern = GetRand(3);
			switch (pattern) {
			case 0:
				*pAttackCharaState = eCHARA_STATE_ATTACK_1;
				result = true;
				break;
			case 1:
				*pAttackCharaState = eCHARA_STATE_ATTACK_2;
				result = true;
				break;
			default:
				*pAttackCharaState = eCHARA_STATE_ATTACK_3;
				result = true;
				break;
			}
		}
	} else if (dist < mBodySize * 4.0f) {
		rand = GetRand(40);
		if (rand == 0) {
			*pAttackCharaState = eCHARA_STATE_ATTACK_4;
			result = true;
		}
	}
	return result;
}

/*=====================================*
 * extend
 *=====================================*/

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyGiginebura::Update(void)
{
	CharaEnemy::Update();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyGiginebura::phaseEnterStateJumpUp(void)
{
	StartMotionEx(&sMotionParam[eMT_JUMP_START]);

	mIsReserveComboInput = false;
	mIsAttack = false;
	mIsDamage = false;

	// �W�����v�͍��߂ɐݒ�
	mGravity += (kJUMP_GRAVITY * 1.4f);

	mIsLand = false;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyGiginebura::phaseEnterStateJumpEnd(void)
{
	const float size = 40.0f;
	VECTOR effpos;
	effpos = VAdd(mPosition, mMoveVec);

	int eff_handle = EffectManager::GetInstance()->Play(eEF_DUST,
														&effpos,
														&VGet(size, size, size),
														0.0f);
	EffectManager::GetInstance()->Stop(eff_handle, 2.0f);
	SoundManager::GetSndComponent()->PlaySound(eSE_BURST);
	
	//	StartMotionEx(&sMotionParam[eMT_JUMP_END]);
	StartMotion(eMT_JUMP_END, false, 0.75f);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyGiginebura::phaseEnterStateAttack1(void)
{
	SoundManager::GetVcComponent(mCharaName)->PlaySound(eVC_ATTACK_1);
	SoundManager::GetSndComponent()->PlaySound(eSE_SWING_SWORD);
	StartMotion(eMT_GIGINEBURA_ATTACK_1, false, 0.625f);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyGiginebura::phaseStateAttack1(void)
{
	static float sStart = 20.0f;
	static float sEnd = 25.0f;

	bool is_prev_attack;
	is_prev_attack = mIsAttack;
	if ((GetMotionNowTime() >= sStart) &&
		(GetMotionNowTime() <= sEnd)) {
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
		
			this->GetFramePosFromName(&capsuleB.mPosA, "�E�Ђ�");
			this->GetFramePosFromName(&capsuleB.mPosB, "�E���");
			capsuleB.mRadius = 60.0f;
			
			if (CollisionManager::GetInstance()->Check_CapsuleToCapsule(&capsuleA, &capsuleB)) {
				SoundManager::GetSndComponent()->PlaySound(eSE_HIT_SWORD);
				int attack_power = 1;
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

	VECTOR weapon_top_pos;
	if ((is_prev_attack == false) && (mIsAttack == true)) {
		this->GetFramePosFromName(&weapon_top_pos, "�E�Ђ�");
//		mEdgeBillboard.SetStartEdgePos(&weapon_top_pos);
	} else if ((is_prev_attack == true) && (mIsAttack == false)) {
		this->GetFramePosFromName(&weapon_top_pos, "�E�Ђ�");
//		mEdgeBillboard.SetEndEdgePos(&weapon_top_pos);
//		mEdgeBillboard.StartDraw(2.0f, EdgeBillboard::eEDGE_FLIP_REVERSE);
	}

	if (GetIsEndMotion()) {
		ChangeState(eCHARA_STATE_IDLE);
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyGiginebura::phaseEnterStateAttack2(void)
{
	SoundManager::GetVcComponent(mCharaName)->PlaySound(eVC_ATTACK_1);
	SoundManager::GetSndComponent()->PlaySound(eSE_SWING_SWORD);
	StartMotion(eMT_GIGINEBURA_ATTACK_2, false, 0.625f);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyGiginebura::phaseStateAttack2(void)
{
	static float sStart = 20.0f;
	static float sEnd = 25.0f;

	bool is_prev_attack;
	is_prev_attack = mIsAttack;
	if ((GetMotionNowTime() >= sStart) &&
		(GetMotionNowTime() <= sEnd)) {
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
		
			this->GetFramePosFromName(&capsuleB.mPosA, "���Ђ�");
			this->GetFramePosFromName(&capsuleB.mPosB, "�����");
			capsuleB.mRadius = 60.0f;
			
			if (CollisionManager::GetInstance()->Check_CapsuleToCapsule(&capsuleA, &capsuleB)) {
				SoundManager::GetSndComponent()->PlaySound(eSE_HIT_SWORD);
				int attack_power = 1;
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

	VECTOR weapon_top_pos;
	if ((is_prev_attack == false) && (mIsAttack == true)) {
		this->GetFramePosFromName(&weapon_top_pos, "���Ђ�");
//		mEdgeBillboard.SetStartEdgePos(&weapon_top_pos);
	} else if ((is_prev_attack == true) && (mIsAttack == false)) {
		this->GetFramePosFromName(&weapon_top_pos, "���Ђ�");
//		mEdgeBillboard.SetEndEdgePos(&weapon_top_pos);
//		mEdgeBillboard.StartDraw(2.0f, EdgeBillboard::eEDGE_FLIP_REVERSE);
	}

	if (GetIsEndMotion()) {
		ChangeState(eCHARA_STATE_IDLE);
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyGiginebura::phaseEnterStateAttack3(void)
{
	SoundManager::GetVcComponent(mCharaName)->PlaySound(eVC_ATTACK_1);
	SoundManager::GetSndComponent()->PlaySound(eSE_SWING_SWORD);

	StartMotion(eMT_ATTACK_3, false, 0.25f); 
}

//---------------------------------------------------
/**
 * ���[�����O�U��
 */
//---------------------------------------------------
void
CharaEnemyGiginebura::phaseStateAttack3(void)
{
	static float sStart = 0.0f;
	static float sEnd = 25.0f;

	bool is_prev_attack;
	is_prev_attack = mIsAttack;
	if ((GetMotionNowTime() >= sStart) &&
		(GetMotionNowTime() <= sEnd)) {
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
		
			this->GetFramePosFromName(&capsuleB.mPosA, "�K���P");
			this->GetFramePosFromName(&capsuleB.mPosB, "�K����");
			capsuleB.mRadius = 60.0f;
			
			if (CollisionManager::GetInstance()->Check_CapsuleToCapsule(&capsuleA, &capsuleB)) {
				SoundManager::GetSndComponent()->PlaySound(eSE_HIT_SWORD);
				int attack_power = 2;
				VECTOR damage_down_vec;
				damage_down_vec = VSub(p_player->Position(), capsuleB.mPosB);
				damage_down_vec = VNorm(damage_down_vec);
				p_player->ProcessDamage(attack_power, false, NULL, &damage_down_vec, true, eATTACK_TYPE_ENEMY_NORMAL);
			}
		}

	} else {
		mIsAttack = false;
	}

	PRINT_SCREEN(GetColor(255, 255, 0), "NowTime : %.2f\n", GetMotionNowTime());

	if (GetIsEndMotion()) {
		ChangeState(eCHARA_STATE_IDLE);
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyGiginebura::phaseEnterStateAttack4(void)
{
	CharaBase* p_player;
	p_player = (CharaBase *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();

	mJumpMoveVec.mDash = VSub(p_player->Position(), this->mPosition);
	mJumpMoveVec.mDash.y = 0.0f;
	mJumpMoveVec.mDash = VScale(VNorm(mJumpMoveVec.mDash), (kCHARA_RUN_SPEED * 1.5f)); 

	SoundManager::GetSndComponent()->PlaySound(eSE_ROLLING);

	//	StartMotion(eMT_GIGINEBURA_ATTACK_4, false, 0.25f); 
	StartMotion(eMT_GIGINEBURA_ATTACK_4, false, 0.5f); 
}

//---------------------------------------------------
/**
 * �ːi
 */
//---------------------------------------------------
void
CharaEnemyGiginebura::phaseStateAttack4(void)
{
	mMoveVec = mJumpMoveVec.mDash;

	mIsAttack = true;

	/****/
	CharaBase* p_player;
	p_player = (CharaBase *)CollisionManager::GetInstance()->GetAttackCollisionPlayerAddress();

	/* �퓬�s�\�҂Ɗ��ɍU�����ꂽ�҂����� */
	if ((p_player->GetIsRetire() == false) &&
		(p_player->IsDamage() == false)) {

		/*
		CollisionManager::tSphereWork sphereA;
		CollisionManager::tSphereWork sphereB;
		VECTOR vec, outpos;
		float dist;
		sphereA.mPosition = p_player->Position();
		sphereA.mSize = p_player->BodySize();
		sphereB.mPosition = this->mPosition;
		sphereB.mSize = mBodySize;
		*/

		ShapeCapsule capsuleA;
		ShapeCapsule capsuleB;
		capsuleA.mPosA	= p_player->Position();
		capsuleA.mPosB	= p_player->Position();
		capsuleA.mPosB.y += p_player->BodyHeight();
		capsuleA.mRadius= p_player->BodySize();
		
		this->GetFramePosFromName(&capsuleB.mPosA, "����");
		this->GetFramePosFromName(&capsuleB.mPosB, "��");
		capsuleB.mRadius = 70.0f;
		
		//if (CollisionManager::Check_SphereToSphere(&sphereA, &sphereB, &outpos, &dist)) {
		if (CollisionManager::GetInstance()->Check_CapsuleToCapsule(&capsuleA, &capsuleB)) {
			
			SoundManager::GetSndComponent()->PlaySound(eSE_HIT_SWORD);
			int attack_power = 1;
			VECTOR damage_down_vec;
			damage_down_vec = VSub(p_player->Position(), mPosition);
			damage_down_vec = VCross(VGet(0.0f, 1.0f, 0.0f), damage_down_vec);
			damage_down_vec = VNorm(damage_down_vec);
			p_player->ProcessDamage(attack_power, false, NULL, &damage_down_vec, true, eATTACK_TYPE_ENEMY_NORMAL);

		}
	}

	PRINT_SCREEN(GetColor(255, 255, 0), "NowTime : %.2f\n", GetMotionNowTime());

	if (GetIsEndMotion() || mIsLand == false) {
		
		mMoveVec = ZERO_VEC;
		mIsAttack = false;
		mJumpMoveVec.mDash = ZERO_VEC;
		ChangeState(eCHARA_STATE_IDLE);
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
/*
void
CharaEnemyGiginebura::phaseStateWaitAttack(void)
{
	int rand = GetRand(40);

	if (rand > 4) {
		ChangeState(eCHARA_STATE_ATTACK_4);
		return;
	}

	if (rand == 0) {
		ChangeState(eCHARA_STATE_ATTACK_1);
	} else if (rand == 1) {
		ChangeState(eCHARA_STATE_ATTACK_2);
	} else if (rand == 2) {
		ChangeState(eCHARA_STATE_ATTACK_3);
	} else if (rand == 3) {
		ChangeState(eCHARA_STATE_ATTACK_4);
	}
}
*/

#ifdef _DEBUG
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CharaEnemyGiginebura::debugFunc(void)
{
	ShapeCapsule capsuleB;
	for (int iI = 0; iI < kCAPSULE_NUM_GIGINEBURA; iI ++) {
		this->GetFramePosFromName(&capsuleB.mPosA, sCapsuleWork[iI].mPosA);
		this->GetFramePosFromName(&capsuleB.mPosB, sCapsuleWork[iI].mPosB);
		capsuleB.mRadius = sCapsuleWork[iI].mRadius;
		GraphicManager::GetInstance()->DbgRenderCapsule(&capsuleB);
	}
}

#endif // _DEBUG

 /**** end of file ****/
