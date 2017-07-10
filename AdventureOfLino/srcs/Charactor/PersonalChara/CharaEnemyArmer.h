/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �M�M�l�u���t�@�C��
 * @author SPATZ.
 * @data   2011/11/27 11:15:20
 */
//---------------------------------------------------
#ifndef __CHARA_ENEMY_ARMER_H__
#define __CHARA_ENEMY_ARMER_H__

#include "Charactor/CharaEnemy.h"

/*=====================================*
 * class
 *=====================================*/
class CharaEnemyArmer : public CharaEnemy {
 public:

	/*�֐�*/
	CharaEnemyArmer()		{}
	~CharaEnemyArmer();

	void		Setup(const char* enemyName, int maxHitpoint, const char* layoutName);

 private:

	void		ProcessDamage(int attackPower, bool isSetMode, VECTOR* pEffPos, VECTOR* pDamageDownVec, bool isDamageDown, eAttackType attackType);

	/*=====================================*
	 * phaseState
	 *=====================================*/
	/* extend */

	void		phaseEnterStateIdle(void);
	void		phaseStateIdle(void);
	void		phaseEnterStateMove(void);
	void		phaseStateMove(void);
	void		phaseEnterStateJumpUp(void);
	void		phaseStateJumpUp(void);
	void		phaseEnterStateJumpDown(void);
	void		phaseStateJumpDown(void);
	void		phaseEnterStateJumpEnd(void);
	void		phaseEnterStateAttack1(void);
	void		phaseStateAttack1(void);
	void		phaseEnterStateRetireEnd(void);

	/* �L�����̍U���X�e�[�^�X�J�ڂ̃`�F�b�N�֐� */
	bool		checkChangeAttackState(int* pAttackCharaState);
	void		processLandingFunc(void)		{}

	void		updateAttackCollisionLocal(int attackPower);

	/* �ϐ� */
	int			mStateCount;
	Shadow		mAnotherShadow;
};

#endif // __CHARA_ENEMY_ARMER_H__

/**** end of file ****/

