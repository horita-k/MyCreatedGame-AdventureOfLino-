/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �G�h���N���X
 * @author SPATZ.
 * @data   2014/01/15 01:59:46
 */
//---------------------------------------------------
#ifndef __CHARA_ENEMY_EX_H__
#define __CHARA_ENEMY_EX_H__

#include "Charactor/CharaEnemy.h"

/*=====================================*
 * �X���C���N���X
 *=====================================*/
class CharaEnemySlime : public CharaEnemy {
 public:
	/*�֐�*/
 private:
	/*=====================================*
	 * phaseState
	 *=====================================*/
	void		Setup(const char* enemyName, int maxHitpoint, const char* layoutName);

	void		phaseEnterStateAttack1(void);
	void		phaseStateAttack1(void);
	/* �L�����̍U���X�e�[�^�X�J�ڂ̃`�F�b�N�֐� */
	bool		checkChangeAttackState(int* pAttackCharaState);
};

/*=====================================*
 * �E�b�h�}���N���X
 *=====================================*/
class CharaEnemyWoodman : public CharaEnemy {
public:
	/* �֐� */
	void		phaseEnterStateAttack1(void);
	void		phaseStateAttack1(void);
	void		phaseStateWaitAttack(void);

	bool		checkChangeAttackState(int* pAttackCharaState);
};

/*=====================================*
 * �~�m�^�E���X
 *=====================================*/
class CharaEnemyMinotaurus : public CharaEnemy {
 public:
	void		Setup(const char* enemyName, int maxHitpoint, const char* layoutName);
 private:
	void		phaseEnterStateAttack1(void);
	void		phaseStateAttack1(void);
	void		phaseEnterStateRetireEnd(void);

	bool		checkChangeAttackState(int* pAttackCharaState);

	ModelPmd	mWeaponModel;
};

/*=====================================*
 * �X�R�[�s�I��
 *=====================================*/
class CharaEnemyScorpion : public CharaEnemy {
 public:
 private:
	static const char* strAttackTopNode;
	static const char* strAttackBtmNode;

	void		phaseEnterStateAttack1(void);
	void		phaseStateAttack1(void);
	void		phaseEnterStateRetireEnd(void);

	void		processNoMoveAction(void);
};

/*=====================================*
 * ���{�b�g�E�f��^�C�v
 *=====================================*/
class CharaEnemyRobotA : public CharaEnemy {
 public:
	void		Setup(const char* enemyName, int maxHitpoint, const char* layoutName);
	void		ProcessDamage(int attackPower, bool isSetMode, VECTOR* pEffPos, VECTOR* pDamageDownVec, bool isDamageDown, eAttackType attackType) {
		if ((attackType == eATTACK_TYPE_COPY_ROD) || (attackType == eATTACK_TYPE_THUNDER)) {
			attackPower *= 2;
		}
		CharaEnemy::ProcessDamage(attackPower, isSetMode, pEffPos, pDamageDownVec, isDamageDown, attackType);
	}
	
protected:
	static const char* strAttackTopNode;
	static const char* strAttackBtmNode;
	static const char* strAttackTopNodeB;
	static const char* strAttackBtmNodeB;

private:
	virtual void		phaseEnterStateIdle(void);
	virtual void		phaseEnterStateAttack1(void);
	virtual void		phaseStateAttack1(void);
	virtual void		phaseEnterStateAttack2(void);
	virtual void		phaseStateAttack2(void);
	virtual void		phaseStateWaitAttack(void);
	virtual void		phaseEnterStateRetireEnd(void);

	virtual bool		checkChangeAttackState(int* pAttackCharaState);

	BladeEffect	mBladeEffectB;
};

/*=====================================*
 * ���{�b�g�E�e�^�C�v
 *=====================================*/
class CharaEnemyRobotB : public CharaEnemyRobotA {

public:
	CharaEnemyRobotB();

private:

	void		phaseEnterStateAttack2(void);
	void		phaseStateAttack2(void);

	bool		checkChangeAttackState(int* pAttackCharaState);

	BYTE		mStatePhase;
	WORD		mStateCount;
};

/*=====================================*
 * ���{�b�g�E�A�[�}�[�^�C�v
 *=====================================*/
class CharaEnemySA_14 : public CharaEnemy {
 public:
	CharaEnemySA_14();
	void		Setup(const char* enemyName, int maxHitpoint, const char* layoutName);
	void		ProcessDamage(int attackPower, bool isSetMode, VECTOR* pEffPos, VECTOR* pDamageDownVec, bool isDamageDown, eAttackType attackType) {
		if ((attackType == eATTACK_TYPE_COPY_ROD) || (attackType == eATTACK_TYPE_THUNDER)) {
			attackPower *= 2;
		}
		CharaEnemy::ProcessDamage(attackPower, isSetMode, pEffPos, pDamageDownVec, isDamageDown, attackType);
	}
protected:
	static const BYTE  kEFFECT_NUM	  = 3;
	static const char* strAttackTopNode;
	static const char* strAttackBtmNode;

	static const char* strAttackTopNodeLeft;
	static const char* strAttackBtmNodeLeft;

private:
	virtual void		phaseEnterStateIdle(void);

	virtual void		phaseStateMove(void);

	virtual void		phaseEnterStateAttack1(void);
	virtual void		phaseStateAttack1(void);

	virtual void		phaseEnterStateAttack2(void);
	virtual void		phaseStateAttack2(void);

	virtual void		phaseStateWaitAttack(void);

	virtual void		phaseEnterStateDamage(void);
	virtual void		phaseEnterStateDownStart(void);
	virtual void		phaseEnterStateRetireEnd(void);

	virtual bool		checkChangeAttackState(int* pAttackCharaState);

	BYTE		mStatePhase;
	WORD		mStateCount;
	int			mEfsHandle[kEFFECT_NUM];
	VECTOR		mEffTargetVec[kEFFECT_NUM];
	ModelBase	mShotModel[kEFFECT_NUM];
};

/*=====================================*
 * ���y���R�v�^�[
 *=====================================*/
class CharaEnemyHelicopter : public CharaEnemy {
public:
	static const char* strAttackTopNode;
	static const char* strAttackBtmNode;

	CharaEnemyHelicopter();

	void		Setup(const char* enemyName, int maxHitpoint, const char* layoutName);
	void		ProcessDamage(int attackPower, bool isSetMode, VECTOR* pEffPos, VECTOR* pDamageDownVec, bool isDamageDown, eAttackType attackType) {
		if ((attackType == eATTACK_TYPE_COPY_ROD) || (attackType == eATTACK_TYPE_THUNDER)) {
			attackPower *= 2;
		}
		CharaEnemy::ProcessDamage(attackPower, isSetMode, pEffPos, pDamageDownVec, isDamageDown, attackType);
	}

	void		phaseStateIdle(void);
	void		phaseStateMove(void);

	void		phaseEnterStateJumpUp(void);
	void		phaseStateJumpUp(void);
	void		phaseEnterStateJumpDown(void);
	void		phaseStateJumpDown(void);
	void		phaseStateAppear(void);
	void		phaseEnterStateAttack1(void);
	void		phaseStateAttack1(void);
	void		phaseEnterStateAttack2(void);
	void		phaseStateAttack2(void);

	bool		checkChangeAttackState(int* pAttackCharaState);

private:
	BYTE		mStatePhase;
	WORD		mStateCount;

	union {
		VECTOR		mEscapePos;
		VECTOR		mAimPos;
	} mTmpVec;

};


#endif // __CHARA_ENEMY_EX_H__

/**** end of file ****/

