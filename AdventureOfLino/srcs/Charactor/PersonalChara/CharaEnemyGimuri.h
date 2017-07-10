/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �M�����t�@�C��
 * @author SPATZ.
 * @data   2015/12/31 10:04:48
 */
//---------------------------------------------------
#ifndef __CHARA_ENEMY_GIMURI_H__
#define __CHARA_ENEMY_GIMURI_H__

#include "Charactor/CharaEnemy.h"
#include "Object/ObjectHand.h"
#include "DataDefine/MotionDef.h"

#include "AppLib/Graphic/ModelPmd.h"

/*=====================================*
 * class
 *=====================================*/
class CharaEnemyGimuri : public CharaEnemy {
 public:

	enum {
		// 10
		eMT_EXTEND_ATTACK_1 = eMT_STANDARD_MAX,	// �A�b�p�[
		eMT_EXTEND_ATTACK_2,					// ��Z
		eMT_EXTEND_ATTACK_3,					// �p���`
		eMT_EXTEND_ATTACK_4,					// �L�b�N
		eMT_EXTEND_ATTACK_5,					// ���
		eMT_EXTEND_ATTACK_6,					// ����
		
		eMT_EXTEND_MAX,
	};


	/*�֐�*/
	CharaEnemyGimuri();
	~CharaEnemyGimuri()		{}

	void		Setup(const char* enemyName, int maxHitpoint, const char* layoutName);
	void		ProcessDamage(int attackPower, bool isSetMode, VECTOR* pEffPos, VECTOR* pDamageDownVec, bool isDamageDown, eAttackType attackType);
 private:
	/*=====================================*
	 * phaseState
	 *=====================================*/
	/* extend */
	void		phaseEnterStateIdle(void);
	void		phaseEnterStateMove(void);

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

	void		phaseEnterStateWaitAttack(void);

	void		phaseEnterStateDamage(void);
	void		phaseEnterStateRetireEnd(void);

	/* �L�����̍U���X�e�[�^�X�J�ڂ̃`�F�b�N�֐� */
	bool		checkChangeAttackState(int* pAttackCharaState);
	void		phaseStateWaitAttack(void);

	void		processNoMoveAction(void);

	BladeEffect	mBladeEffect2nd;
	EFK_HANDLE	mEfkHandle;
	DWORD		mCannonCount;
};

#endif // __CHARA_ENEMY_GIMURI_H__

/**** end of file ****/

