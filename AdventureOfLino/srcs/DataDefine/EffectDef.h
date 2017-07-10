/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �G�t�F�N�g�t�@�C���̒�`
 * @author SPATZ.
 * @data   2012/04/29 20:13:58
 */
//---------------------------------------------------
#ifndef __EFFECT_DEF_H__
#define __EFFECT_DEF_H__

//<!!!! �����̓C�x���g�ł��g����\��������̂Ō�������ւ��Ȃ����� !!!!!
enum {
	// 0
	eEF_FLAME = 0,
	eEF_HIT,
	eEF_THUNDER,
	eEF_AURA,
	eEF_WIND,
	// 5
	eEF_DUST,
	eEF_MY_TAIL,
	eEF_BLACKDUST,
	eEF_WATERDUST,
	eEF_LIGHTNING_EX,
	// 10
	eEF_LIGHTNING,
	eEF_NAVI,
	eEF_ENERGY_BALL,
	eEF_PROTECT,
	eEF_WINDFIRE,
	// 15
	eEF_ENERGY_BALL_R,
	eEF_ENERGY_BALL_G,
	eEF_ENERGY_BALL_B,
	eEF_MARUI,
	eEF_FLAME_BOMB,
	// 20
	eEF_CHARGE,
	eEF_FOREST,
	eEF_ARROW,
	eEF_GREEN,
	//	eEF_BLUR,
	// 25
	eEF_REALFIRE,
	eEF_TORNADE,
	eEF_WHITE_TAIL,	
	eEF_SKY,

	// ���̂Q�͍Ō�
	eEF_DRAG_DROP,
	//	eEF_BLADE,

	eEF_MAX,
};

enum {
	// 0
	eEFK_MAGIC_SQUARE = 0,
	eEFK_SHIELD,
	eEFK_MAGIC_BALL,
	eEFK_SPREAD,
	eEFK_DISAPPER,
	// 5
	eEFK_PROTECT_START,
	eEFK_HEAL,
	eEFK_BREAK_BARRIER,
	eEFK_EXPLOSION_BOMB,
	eEFK_BREAK_JAR,
	// 10
	eEFK_BOSS_DEATH,
	eEFK_SHOCK,			// �傫�ȏՌ�
	eEFK_PIYOPIYO,		// �҂�҂�
	eEFK_SHOCK_THIN,	// �����Ռ�
	eEFK_BREAK_LOCK,	// ��̔j��
	// 15
	eEFK_TELEKINESIS,	// �O����
	eEFK_GETKEYITEM,	// �L�[�A�C�e������
	eEFK_LASER,			// ���[�U�[
	eEFK_CANNON_LASER_SHOOT,	// ���僌�[�U�[
	eEFK_SONICK_WAVE,	// �\�j�b�N�E�F�[�u
	// 20
	eEFK_WIND,			// ���؂�
	eEFK_THRUST,		// �˂�
	eEFK_PORTION,		// �|�[�V����
	eEFK_CANNON_LASER_CHARGE,		// �`���[�W
	eEFK_AURA_FLAME,	// ��
	// 25
	eEFK_HELL_FLAME,	// ��
	eEFK_REAL_FLAME,	// ��
	eEFK_STAGE_CLEAR,	// �X�e�[�W�N���A
	eEFK_LIGHTNING_STRIKE,		// ��
	eEFK_SENPU_AREA,	// ����
	// 30
	eEFK_MAGMA,			// �}�O�}�G�t�F�N�g
	eEFK_HIT_LIGHTNING,	// ���q�b�g
	eEFK_LIGHT_PILLAR,	// ���̒�

#ifdef _DEBUG
	eEFK_DRAG_DROP_SAMPLE, // �h���b�O���h���b�v�p�e�X�g
#endif // _DEBUG

	eEFK_MAX,
};


#endif // __EFFECT_DEF_H__

 /**** end of file ****/
