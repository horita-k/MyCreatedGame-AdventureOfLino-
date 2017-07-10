/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �M�M�l�u���t�@�C��
 * @author SPATZ.
 * @data   2011/11/27 11:15:20
 */
//---------------------------------------------------
#ifndef __CHARA_ENEMY_HATOPHON_H__
#define __CHARA_ENEMY_HATOPHON_H__

#include "Charactor/CharaEnemy.h"
#include "AppLib/Graphic/EffekseerManager.h"

/*=====================================*
 * class
 *=====================================*/
class CharaEnemyHatophon : public CharaEnemy {
 public:

	/*�֐�*/
	CharaEnemyHatophon();
	~CharaEnemyHatophon()		{}

	void		Setup(const char* enemyName, int maxHitpoint, const char* layoutName);
	void		Update(void);

	/* �_���[�W�����蔻��`�F�b�N�֐� */
	bool		CheckDamageCollNormal(void* pParam);

 private:
	/*=====================================*
	 * phaseState
	 *=====================================*/
	/* extend */
	void		phaseEnterStateJumpDown(void);
	void		phaseEnterStateJumpUp(void);

	void		phaseEnterStateJumpEndStandard(void);
	void		phaseEnterStateJumpEndDarts(void);
	void		phaseStateJumpEndStandard(void);
	void		phaseStateJumpEndDarts(void);

	void		phaseStateJumpUp(void);

	void		phaseEnterStateAttack1(void);
	void		phaseStateAttack1(void);
	void		phaseEnterStateAttack2(void);
	void		phaseStateAttack2(void);
	void		phaseEnterStateAttack3(void);
	void		phaseStateAttack3(void);
	void		phaseEnterStateAttack4(void);
	void		phaseStateAttack4(void);

	//	void		phaseEnterStateRetireStart(void);
	//	void		phaseStateRetireStart(void);
	void		phaseEnterStateRetireDoing(void);
	void		phaseStateRetireDoing(void);
	void		phaseEnterStateRetireEnd(void);
	void		phaseStateRetireEnd(void);


	//	void		phaseStateWaitAttack(void);

	/* �L�����̍U���X�e�[�^�X�J�ڂ̃`�F�b�N�֐� */
	bool		checkChangeAttackState(int* pAttackCharaState);
	void		processLandingFunc(void)		{}

#ifdef _DEBUG
	/* debug func */
	void		debugFunc(void);
#endif // _DEBUG

	/* �ϐ� */
	int			mStateCount;
	int			mStatePhase;
	int			mEffIndex;
	EFK_HANDLE	mEfkHandle;

};

#endif // __CHARA_ENEMY_H__

/**** end of file ****/

