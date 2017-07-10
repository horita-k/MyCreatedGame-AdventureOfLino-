/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �G�N�V���B�t�@�C��
 * @author SPATZ.
 * @data   2015/12/31 10:04:48
 */
//---------------------------------------------------
#ifndef __CHARA_ENEMY_EXCIVY_H__
#define __CHARA_ENEMY_EXCIVY_H__

#include "Charactor/CharaEnemy.h"
#include "DataDefine/MotionDef.h"

#include "Object/ObjectHand.h"

#include "AppLib/Graphic/ModelPmd.h"
#include "Object/ObjectWeapon.h"

/*=====================================*
 * class
 *=====================================*/
class CharaEnemyExcivy : public CharaEnemy {
 public:
	/*�֐�*/
	CharaEnemyExcivy();
	~CharaEnemyExcivy()		{}

	enum {
		// 10
		eMT_EXTEND_ATTACK_1 = eMT_STANDARD_MAX,	// �\�[�h�t���C��
		eMT_EXTEND_ATTACK_2,					// ���
		eMT_EXTEND_ATTACK_3,					// �Ȃ�����
		eMT_EXTEND_ATTACK_4,					// �K�[�h
		eMT_EXTEND_ATTACK_5,					// �K�[�h�e��

		eMT_EXTEND_ATTACK_6_START,				// �X�}�b�V���a��E�J�n
		eMT_EXTEND_ATTACK_7,					// �o�N�]�a��
		eMT_EXTEND_ATTACK_8,					// �˂�
		eMT_EXTEND_ATTACK_9,					// �a��グ
		
		eMT_EXTEND_MAX,
	};

	void		Setup(const char* enemyName, int maxHitpoint, const char* layoutName);
	void		Update(void);
	void		ProcessDamage(int attackPower, bool isSetMode, VECTOR* pEffPos, VECTOR* pDamageDownVec, bool isDamageDown, eAttackType attackType);

 private:
	/*=====================================*
	 * phaseState
	 *=====================================*/
	/* extend */
	void		phaseEnterStateIdle(void);
	void		phaseEnterStateAttack1(void);
	void		phaseStateAttack1(void);
	void		phaseEnterStateAttack2(void);
	void		phaseStateAttack2(void);
	void		phaseEnterStateAttack3(void);
	void		phaseStateAttack3(void);

	void		phaseEnterStateAttack4(void);
	void		phaseStateAttack4(void);

	void		phaseEnterStateAttack5(void);
	void		phaseStateAttack5(void);

	void		phaseEnterStateAttack6(void);
	void		phaseStateAttack6(void);

	void		phaseEnterStateAttack7(void);
	void		phaseStateAttack7(void);

	void		phaseEnterStateAttack8(void);
	void		phaseStateAttack8(void);
	void		phaseEnterStateAttack9(void);
	void		phaseStateAttack9(void);


	void		phaseEnterStateDamage(void);
	void		phaseEnterStateRetireEnd(void);

	/* �L�����̍U���X�e�[�^�X�J�ڂ̃`�F�b�N�֐� */
	bool		checkChangeAttackState(int* pAttackCharaState);
	void		phaseStateWaitAttack(void);

	void		processNoMoveAction(void);

	bool		checkShieldHit(VECTOR *pAttackDirVec);

	void		changeWeaponMaterial(bool isChange);
	void		getSonicWaveMatrix(MATRIX& rMatrix, float setScale, bool isVertical);

	ModelPmd	mWeaponModel;
	BOOL		mSmashState;

	ObjectWeapon_SonicWave		mSonicWave;

	int			mEfsFlame;
	EFK_HANDLE	mEfkFlame;
	WORD		mHellFlameCnt;		
	VECTOR		mHellFlamePos;

	EFK_HANDLE	mEfkCharge;
	BOOL		mIsSkyDiveAttack;


};

#endif // __CHARA_ENEMY_EXCIVY_H__

/**** end of file ****/

