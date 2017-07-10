/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * ���[�V�����t�@�C���̒�`	
 * @author SPATZ.
 * @data   2011/11/27 11:43:56
 */
//---------------------------------------------------
#ifndef __MOTION_DEF_H__
#define __MOTION_DEF_H__

#include "../AppLib/Graphic/ModelType.h"

/*=====================================*
 * ���m�̃��[�V����
 *=====================================*/
enum {
	eMT_IDLE = 0,						// 0	�A�C�h��
	eMT_WALK,							// 1	����
	eMT_RUN,							// 2	����
	eMT_JUMP_START,						// 3	�W�����v�J�n
	eMT_JUMP_DOING,						// 4	�W�����v��
	eMT_JUMP_END,						// 5	�W�����v�I��
	eMT_DAMAGE,							// 6	�_���[�W
	eMT_DOWN_START,						// 7	�|�ꂱ�݊J�n
	eMT_DOWN_DOING,						// 8	�|�ꂱ�ݒ�
	eMT_DOWN_END,						// 9	�|�ꂱ�ݏI��
	eMT_STANDARD_MAX,
	eMT_ATTACK_1 = eMT_STANDARD_MAX,	// 10	�U���P
	eMT_ATTACK_2,						// 11	�U���Q
	eMT_ATTACK_3,						// 12	�U���R
	eMT_ATTACK_ROLL,					// 13	��]�U��
	eMT_ATTACK_THRUST,					// 14	�˂��U��
	eMT_HOOK_SHOT,						// 15	�t�b�N�V���b�g
	eMT_HOOK_MOVE,						// 16	�t�b�N�V���b�g�ړ���
	eMT_TAKE_TREASURE,					// 17	�󔠎擾
	eMT_AWAY_RIGHT,						// 18	����E
	eMT_AWAY_LEFT,						// 19	�����
	eMT_AWAY_BACK,						// 20	������
	eMT_RUN_FAST,						// 21	��������
	eMT_GREETING,						// 22	���A(���U��)
	eMT_IDLE_NOWEAPON,					// 23	���햢�������A�C�h��
	eMT_ATTACK_4,						// 24	�U���S
	eMT_PEACE,							// 25	�s�[�X
	eMT_PUT_SWORD,						// 26	�������܂�
	eMT_SLEEP,							// 27	�Q�Ă���
	eMT_BATOKIN_EVENT,					// 28	�o�g�[�L���C�x���g
	eMT_KITCHEN_EVENT,					// 29	���m�̉Ƃ̃C�x���g
	eMT_KICK_TREASURE,					// 30	�󔠋�
	eMT_021_00_EVENT,					// 31	�C�x���g���[�V����
	eMT_BOOMERANG,						// 32	�u�[������
	eMT_AWAY_FRONT,						// 33	���O
	eMT_ATTACK_JUMP,					// 34	�W�����v�U��
	eMT_026_00_EVENT,					// 35	�C�x���g���[�V����
	eMT_KICK_TREASURE_SHORT,			// 36	�󔠋�(�V���[�g��)
	eMT_IDLE_SLEEP,						// 37	������
	eMT_WEEKLY,							// 38	�m��
	eMT_DUMMY_39,						// 39	�_�~�[���[�V����
	eMT_SMILE_00,						// 40	���m�̏Ί�O
	eMT_SMILE_01,						// 41	���m�̏Ί�P
	eMT_SMILE_02,						// 42	���m�̏Ί�Q
	eMT_SMILE_03,						// 43	���m�̏Ί�R
	eMT_SMILE_04,						// 44	���m�̏Ί�S
	eMT_SMILE_05,						// 45	���m�̏Ί�T
	eMT_SMILE_06,						// 46	���m�̏Ί�U
	eMT_SMILE_07,						// 47	���m�̏Ί�V
	eMT_SMILE_08,						// 48	���m�̏Ί�W
	eMT_SMILE_09,						// 49	���m�̏Ί�X
	eMT_029_00_EVENT,					// 50	�C�x���g���[�V����
	eMT_029_01_EVENT,					// 51	�C�x���g���[�V����
	eMT_029_04_EVENT,					// 52	�C�x���g���[�V����
	eMT_SHIELD_GUARD,					// 53	���K�[�h
	eMT_SMILE_QUESTION,					// 54	���m�̏Ί�@�^��
	eMT_014_01_EVENT,					// 55	�C�x���g���[�V����
	eMT_RAISE_IDLE,						// 56	���������グ�����̃A�C�h��
	eMT_RAISE_WALK,						// 57	���������グ�����̕���
	eMT_RAISE_THROW,					// 58	���𓊂���
	eMT_RAISE_PUT,						// 59	����u��
	eMT_RAISE_SWORD,					// 60	�����f����
	eMT_TAKEBACK_ARCHERY,				// 61	�|������
	eMT_CHARGE_ARCHERY,					// 62	�|�̗��ߎ�
	eMT_SHOOT_ARCHERY,					// 63	�|�����
	eMT_WORRY,							// 64	���������
	eMT_SMILE_YEAH,						// 65	���m�̏Ί�@�C�G�[�C�I

	eMT_ATTACK_SMASH_START,				// 66	���юa��J�n
	eMT_ATTACK_SMASH_DOING,				// 67	���юa�蒆
	eMT_ATTACK_SMASH_END,				// 68	���юa��I��
	
	eMT_LOOK_ARROUND,					// 69	���������
	eMT_DOWN_HIP,						// 70	�K��������
	eMT_CLING_LEFT_HAND,				// 71	����ł���
	eMT_ATTACK_SLAPPING,				// 72	�r���^
	eMT_DAMAGE_SLAPPING,				// 73	�r���^
	eMT_SMILE_SHY,						// 74	���m�̏Ί�E�Ƃ�
	eMT_ATTACK_REAR,					// 75	�w�ʎa��
	eMT_ATTACK_ROLLING_KICK,			// 76	�񂵏R��
	eMT_RIDE_VEHICLE,					// 77	��蕨�ɏ���Ă�
	eMT_RIDE_VEHICLE_RUNNING,			// 78	��蕨�ɏ���Ă�(�ړ���)
	
	eMT_SHIELD_PIYOPIYO,				// 79	�s���s��
	eMT_SHIELD_PIYOPIYO_2ND,			// 80	�s���s��
	eMT_GUTS_POSE,						// 81	�K�b�c�|�[�Y

	eMT_MAGIC_ARIA_START,				// 82	���@�r���J�n
	eMT_MAGIC_ARIA_DOING,				// 83	���@�r����
	eMT_OCARINA_STANCE,					// 84	�I�J���i�E�\����
	eMT_SHIELD_GUARD_DOING,				// 85	���K�[�h�͂���
	eMT_COPY_ROD,						// 86   �R�s�[���b�h
	eMT_OCARINA_PLAY_START,				// 87   �I�J���i�E���t�J�n
	eMT_OCARINA_PLAY_DOING,				// 88   �I�J���i�E���t��
	eMT_SLEEP_ENDING,					// 89	�Q�Ă���A�G���f�B���O�p

	eMT_MAX,							// 90
};

enum {
	eMT_NAVI_STAGE22_START = 3,
	eMT_NAVI_026_00_EVENT,
	eMT_NAVI_024_00_WELCOM_WATERWAY,
	eMT_NAVI_029_00_WELCOM_CASTLE,

	eMT_NAVI_MAX,
};

enum {
	eMT_WIZARD_029_00_WELCOM_CASTLE = 13,
	eMT_WIZARD_029_01_APPEAR_WIZARD = 14,
	eMT_WIZARD_029_04_COME_FRES = 15,
};

enum {
	eMT_CLOCK_029_00_WELCOM_CASTLE = 2,
};

// �t���X�x���N�E�퓬�p
enum {
	// 10
	eMT_FRESBELK_ATTACK_1 = eMT_STANDARD_MAX,
	eMT_FRESBELK_ATTACK_2,
	eMT_FRESBELK_ATTACK_3,
	eMT_FRESBELK_ATTACK_4,
	eMT_FRESBELK_ATTACK_5,

	eMT_FRESBELK_MAX,
};
// �t���X�x���N�E�C�x���g�p
enum {
	// 10
	eMT_FRES_ATTACK_1 = eMT_STANDARD_MAX,
	/*
	eMT_FRES_ATTACK_2,
	eMT_FRES_ATTACK_3,
	eMT_FRES_ATTACK_ROLL,
	eMT_FRES_ATTACK_JUMP,
	*/
	// 15
	eMT_FRES_EVENT00,
	eMT_FRES_TALK,
	eMT_FRES_029_04_COME_FRES = 15,
	eMT_FRES_IDLE_NOWEAPON,
	eMT_FRES_CROSS_ARM,

	eMT_FRES_MAX,
};

enum {
	// 10
	eMT_GIGINEBURA_ATTACK_1 = eMT_STANDARD_MAX,
	eMT_GIGINEBURA_ATTACK_2,
	eMT_GIGINEBURA_ATTACK_3,
	eMT_GIGINEBURA_ATTACK_4,

	eMT_GIGINEBURA_MAX,
};

enum {
	// 10
	eMT_GOBURIN_ATTACK_1 = eMT_STANDARD_MAX,
	eMT_GOBURIN_ATTACK_2,
	eMT_GOBURIN_ATTACK_3,
	eMT_GOBURIN_ATTACK_4,

	eMT_GOBURIN_MAX,
};

enum {
	// 10
	eMT_STRATEGIST_ATTACK_1 = eMT_STANDARD_MAX,
	eMT_STRATEGIST_ATTACK_2,
	eMT_STRATEGIST_ATTACK_3,
	eMT_STRATEGIST_ATTACK_4,

	eMT_STRATEGIST_MAX,
};

enum {
	// 10
	eMT_KNIGHT_ATTACK_1 = eMT_STANDARD_MAX,
	eMT_KNIGHT_ATTACK_2,
	eMT_KNIGHT_ATTACK_3,
	eMT_KNIGHT_ATTACK_4,

	eMT_KNIGHT_MAX,
};

enum {
	// 10
	eMT_MONK_ATTACK_1 = eMT_STANDARD_MAX,
	eMT_MONK_ATTACK_2,
	eMT_MONK_ATTACK_3,
	eMT_MONK_AWAY,

	eMT_MONK_MAX,
};

enum {
	// 10
	eMT_DRAGON_ATTACK_1 = eMT_STANDARD_MAX,
	eMT_DRAGON_ATTACK_2,
	eMT_DRAGON_ATTACK_3,
	eMT_DRAGON_ATTACK_4,

	eMT_DRAGON_MAX,
};

enum {
	// 10
	eMT_ANHEL_ATTACK_1 = eMT_STANDARD_MAX,
	eMT_ANHEL_ATTACK_2,
	eMT_ANHEL_ATTACK_3,
	eMT_ANHEL_ATTACK_4,

	eMT_ANHEL_MAX,
};

enum {
	// 10
	eMT_HATOPHON_ATTACK_1 = eMT_STANDARD_MAX,
	eMT_HATOPHON_ATTACK_2,
	eMT_HATOPHON_ATTACK_3,
	eMT_HATOPHON_ATTACK_4,
	eMT_HATOPHON_026_000_EVENT,

	eMT_HATOPHON_MAX,
};

enum {
	eMT_TRESURE_IDLE = 0,
	eMT_TRESURE_EVENT,
	eMT_TRESURE_EVENT_SHORT,

	eMT_TRESURE_MAX, 
};

enum {
	eMT_LEA_IDLE = 0,
	eMT_LEA_XXX, 
	eMT_LEA_PRISON_TALK,

	eMT_LEA_MAX,
};

enum {
	eMT_ELDA_IDLE = 0,
	eMT_ELDA_TALK,
	eMT_025_00_EVENT_OLD,
	eMT_ELDA_CHAIRING,
};

enum {
	eMT_CELCIANA_IDLE = 0,
	eMT_CELCIANA_014_000_EVENT,

	eMT_CELCIANA_MAX,
};

// �e�B�[�K�[��p�U�����[�V����
enum {
	// 10
	eMT_TEEDER_ATTACK_1 = eMT_STANDARD_MAX,
	eMT_TEEDER_ATTACK_2,
	eMT_TEEDER_ATTACK_3,
	eMT_TEEDER_ATTACK_4,

	eMT_TEEDER_MAX,
};

enum {
	// 10
	eMT_REDDRAGON_ATTACK_1 = eMT_STANDARD_MAX,
	eMT_REDDRAGON_ATTACK_2,
	eMT_REDDRAGON_ATTACK_3,
	eMT_REDDRAGON_ATTACK_4,
	eMT_REDDRAGON_ATTACK_5,		// ���ꗎ���郂�[�V����
	eMT_REDDRAGON_ATTACK_6,	
	eMT_REDDRAGON_DOWN_START,	// �_�E�����[�V����

	eMT_REDDRAGON_MAX,
};

enum {
	// 10
	eMT_DEMON_ATTACK_1 = eMT_STANDARD_MAX,
	eMT_DEMON_ATTACK_2,
	eMT_DEMON_ATTACK_3,
	eMT_DEMON_ATTACK_4,
	eMT_DEMON_ATTACK_5,
	// 15
	eMT_DEMON_UP_ARM,
	eMT_DEMON_INDICATE_FINGER,

	eMT_DEMON_MAX,
};

enum {
	eMT_RAFIA_ATTACK_1 = eMT_STANDARD_MAX,
	eMT_RAFIA_HIP_DOWN,
	eMT_RAFIA_ATTACK_SLAPPING,
	eMT_RAFIA_DAMAGE_SLAPPING,

	eMT_RAFIA_MAX,
};

static const tMotionParam sMotionParam[eMT_MAX] = {
	{eMT_IDLE,				true,	0.5f,	NULL,},
	{eMT_WALK,				true,	0.6f,	NULL,},						
	{eMT_RUN,				true,	1.0f,	NULL,},						
	{eMT_JUMP_START,		false,	0.5f,	&sMotionParam[eMT_JUMP_DOING],},
	{eMT_JUMP_DOING,		true,	0.5f,	NULL,},						
	{eMT_JUMP_END,			false,	0.75f,	NULL,},
	{eMT_DAMAGE,			false,	0.75f,	&sMotionParam[eMT_IDLE],},
	{eMT_DOWN_START,		false,	0.75f,	NULL,},
	{eMT_DOWN_DOING,		false,	0.75f,	NULL,},
	{eMT_DOWN_END,			false,	0.75f,	NULL,},
	{eMT_ATTACK_1,			false,	0.6f,	NULL,},
	{eMT_ATTACK_2,			false,	0.5f,	NULL,},
	{eMT_ATTACK_3,			false,	0.65f,	NULL,},
	{eMT_ATTACK_ROLL,		false,	0.75f,	NULL,},
	{eMT_ATTACK_THRUST,		false,	0.75f,	NULL,},
	{eMT_HOOK_SHOT,			true,	0.75f,	NULL,},
	{eMT_HOOK_MOVE,			false,	1.00f,	NULL,},
	{eMT_TAKE_TREASURE,		false,	1.00f,	NULL,},
	{eMT_AWAY_RIGHT,		false,	0.75f,	NULL,},
	{eMT_AWAY_LEFT,			false,	0.75f,	NULL,},
	{eMT_AWAY_BACK,			false,	0.75f,	NULL,},
	{eMT_RUN_FAST,			true,	0.75f,	NULL,},
	{eMT_GREETING,			false,	0.75f,	&sMotionParam[eMT_IDLE],},
	{eMT_IDLE_NOWEAPON,		true,	0.5f,	NULL,},
	{eMT_ATTACK_4,			false,	0.55f,	NULL,},
	{eMT_PEACE,				true,	0.25f,	NULL,},
	{eMT_PUT_SWORD,			false,	0.375f,	&sMotionParam[eMT_PEACE],},
	{eMT_SLEEP,				false,	0.50f,	NULL,},
	{eMT_BATOKIN_EVENT,		false,	0.50f,	NULL,},
	{eMT_KITCHEN_EVENT,		false,	0.50f,	NULL,},
	{eMT_KICK_TREASURE,		false,	0.50f,	NULL,},
	{eMT_021_00_EVENT,		false,	0.50f,	NULL,},
	{eMT_BOOMERANG,			false,	0.50f,	NULL,},
	{eMT_AWAY_FRONT,		false,	3.00f,	NULL,},
	{eMT_ATTACK_JUMP,		false,	0.75f,	NULL,},

	// 35
	{eMT_026_00_EVENT,		false,	0.00f,	NULL,},	//	eMT_026_00_EVENT,
	{eMT_KICK_TREASURE,		false,	0.00f,	NULL,},	//	eMT_KICK_TREASURE_SHORT,
	{eMT_IDLE_SLEEP,		false,	0.00f,	NULL,},	//	eMT_IDLE_SLEEP,
	{eMT_WEEKLY,			true,	0.2f,	NULL,}, //	eMT_WEEKLY
	{eMT_DUMMY_39,			false,	0.00f,	NULL,},	//	eMT_DUMMY_39,
	// 40
	{eMT_SMILE_00,			false,	0.50f,	NULL,},	//	eMT_SMILE_00,
	{eMT_SMILE_01,			false,	0.10f,	NULL,},	//	eMT_SMILE_01,
	{eMT_SMILE_02,			false,	0.10f,	NULL,},	//	eMT_SMILE_02,
	{eMT_SMILE_03,			false,	0.10f,	NULL,},	//	eMT_SMILE_03,
	{eMT_SMILE_04,			false,	0.10f,	NULL,},	//	eMT_SMILE_04,
	{eMT_SMILE_05,			false,	0.10f,	NULL,},	//	eMT_SMILE_05,
	{eMT_SMILE_06,			false,	0.50f,	NULL,},	//	eMT_SMILE_06,
	{eMT_SMILE_07,			false,	0.50f,	NULL,},	//	eMT_SMILE_07,
	{eMT_SMILE_08,			false,	0.50f,	NULL,},	//	eMT_SMILE_08,
	{eMT_SMILE_09,			false,	0.50f,	NULL,},	//	eMT_SMILE_09,

	{eMT_029_00_EVENT,		false,	0.50f,	NULL,},	//	eMT_029_00_EVENT,					// 50	�C�x���g���[�V����
	{eMT_029_01_EVENT,		false,	0.50f,	NULL,},	//	eMT_029_01_EVENT,					// 51	�C�x���g���[�V����
	{eMT_029_04_EVENT,		false,	0.50f,	NULL,},	//	eMT_029_04_EVENT,					// 52	�C�x���g���[�V����
	{eMT_SHIELD_GUARD,		false,	0.50f,	NULL,},	//	eMT_SHIELD_GUARD,					// 53	���K�[�h
	{eMT_SMILE_QUESTION,	false,	0.50f,	NULL,},	//	eMT_SMILE_QUESTION,					// 54	���m�̏Ί�@�^��
	{eMT_014_01_EVENT,		false,	0.50f,	NULL,},	//	eMT_014_01_EVENT,					// 55	�C�x���g���[�V����
	{eMT_RAISE_IDLE,		true,	0.50f,	NULL,},	//	eMT_RAISE_IDLE,						// 56	���������グ�����̃A�C�h��
	{eMT_RAISE_WALK,		true,	0.50f,	NULL,},	//	eMT_RAISE_WALK,						// 57	���������グ�����̕���
	{eMT_RAISE_THROW,		false,	1.00f,	NULL,},	//	eMT_RAISE_THROW,					// 58	���𓊂���
	{eMT_RAISE_PUT,			true,	0.50f,	NULL,},	//	eMT_RAISE_PUT,						// 59	����u��

};

#endif // __MOTION_DEF_H__

 /**** end of file ****/
