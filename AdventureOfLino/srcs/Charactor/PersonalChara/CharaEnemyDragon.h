/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �M�M�l�u���t�@�C��
 * @author SPATZ.
 * @data   2011/11/27 11:15:20
 */
//---------------------------------------------------
#ifndef __CHARA_ENEMY_DRAGON_H__
#define __CHARA_ENEMY_DRAGON_H__

#include "Charactor/CharaEnemy.h"

/*=====================================*
 * class
 *=====================================*/
class CharaEnemyDragon : public CharaEnemy {
 public:

	/*�֐�*/
	CharaEnemyDragon()		{}
	~CharaEnemyDragon()		{}

	void		Setup(const char* enemyName, int maxHitpoint, const char* layoutName);
	void		Update(void);

	/* �_���[�W�����蔻��`�F�b�N�֐� */
	//	bool		CheckDamageCollNormal(void* pParam);
	bool		CheckDamageCollNormal(tCheckDamageWork* pParam);

 private:
	/*=====================================*
	 * phaseState
	 *=====================================*/
	/* extend */

	void		phaseEnterStateJumpUp(void);
	void		phaseEnterStateJumpEnd(void);

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

	//	void		phaseStateWaitAttack(void);

	/* �L�����̍U���X�e�[�^�X�J�ڂ̃`�F�b�N�֐� */
	bool		checkChangeAttackState(int* pAttackCharaState);

#ifdef _DEBUG
	/* debug func */
	void		debugFunc(void);
#endif // _DEBUG
};

#endif // __CHARA_ENEMY_H__

/**** end of file ****/

