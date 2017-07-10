/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �{�C�X�t�@�C���̒�`	
 * @author SPATZ.
 * @data   2012/02/05 16:47:50
 */
//---------------------------------------------------
#ifndef __VOICE_DEF_H__
#define __VOICE_DEF_H__

enum {
	eVC_ATTACK_1,
	eVC_JUMP,
	eVC_DAMAGE,
	eVC_DOWN,
	eVC_ATTACK_2,

	eVC_ATTACK_3,
	eVC_FALL_DOWN,
	eVC_ATTACK_4,

	eVC_MAX,
};                      

enum {
	eVC_NAVY_HEY = 0,
	eVC_NAVY_LISTN,

	eVC_NAVY_MAX,
};

enum {
	// 0
	eSE_LANDING,			// ���n��
	eSE_SWING_SWORD,		// ����U�鉹
	eSE_HIT_SWORD,			// �U���̃q�b�g��
	eSE_ERASE_MODEL,		// �G����
	eSE_HOOK_START,			// �t�b�N�V���b�g�\��
	// 5
	eSE_HOOK_SHOTTING,		// �t�b�N�V���b�g����
	eSE_REPELED_WALL,		// �t�b�N�V���b�g���ǂɓ����鉹
	eSE_HOOK_END,			// �t�b�N�V���b�g���߂��Ă�����
	eSE_FOOT_STEP_A,		// ����A
	eSE_FOOT_STEP_B,		// ����B
	// 10
	eSE_MESSAGE_IN,			// ���b�Z�[�W�J�n
	eSE_MESSAGE_OUT,		// ���b�Z�[�W�I��
	eSE_TREASURE,			// ����
	eSE_EXPLOSION,			// �����H
	eSE_ENCOUNT,			// �G���J�E���g
	// 15
	eSE_CHARGE,				// ����
	eSE_ROLLING,			// ��]�U��
	eSE_OPEN_WING,			// �����L����
	eSE_SKY_AWAY,			// ��ɔ��ł���
	eSE_BURST,				// ����
	// 20
	eSE_STAGE_CLEAR,		// �X�e�[�W�N���A
	eSE_AWAY,				// �����
	eSE_PUT_SWORD,			// �������߂鉹
	eSE_BOYOYON,			// �ڂ���
	eSE_PRESS_KEY,			// �����L�[���������Ƃ�
	// 25
	eSE_SELECT_KEY,			// �����L�[���������Ƃ�
	eSE_CHANGE_PAGE,		// �y�[�W���߂������Ƃ�
	eSE_CANCEL,				// �L�����Z����
	eSE_GETITEM,			// ���܂���V���[�g�o�[�W����
	eSE_NG,
	// 30
	eSE_WARP,				// ���[�v��
	eSE_COMICAL,			// �R�~�J���ȉ��i�L�c�[�l�̑����Ƃ�)
	eSE_DECIDE_KEY,			// ����̃V�X�e����
	eSE_ITEM_APPEAR,		// �A�C�e���o��
	eSE_FALLDOWN,			// ������
	// 35
	eSE_LANDING_WATER,		// ���̒��ł̒��n��
	eSE_FOOT_STEP_WATER_A,	// ���̒��ł̑���A
	eSE_FOOT_STEP_WATER_B,	// ���̒��ł̑���B
	eSE_ENEMY_APPEAR,		// �G�o��
	eSE_CLEAR_BATTLEBOX,	// �o�g���{�b�N�X�폜
	// 40
	eSE_SOLVE_PUZZLE,		// �������
	eSE_BOOMERANG_RUN,		// �u�[�����������艹
	eSE_MONSTER_VOICE,		// �����̗Y������
	eSE_GET_MONEY,			// ���擾
	eSE_HEAVY_SWING,		// �d��������
	// 45
	eSE_DANGER,				// �댯
	eSE_MAGIC,				// ���@
	eSE_THUNDER,			// ��
	eSE_SHIELD,				// �V�[���h
	eSE_DARK,				// ��
	// 50
	eSE_SPREAD,				// �y��
	eSE_HORROR,				// �z���[
	eSE_FLAME_ARIA,			// ���E�r��
	eSE_FLAME_SHOOT,		// ���E���o
	eSE_REFLECT,			// ���@���˕Ԃ�
	// 55
	eSE_BREAK_BARRIER,		// �o���A�j��
	eSE_BREAK_JAR,			// ��j��
	eSE_HIT_HEAVY,			// �d���Ō���
	eSE_FOOT_STEP_GRASS_A,	// ���̏�ł̑���A
	eSE_FOOT_STEP_GRASS_B,	// ���̏�ł̑���B
	// 60
	eSE_LANDING_GRASS,		// ���̏�ł̒��n��
	eSE_FOOT_STEP_SAND_A,	// ���̏�ł̑���A
	eSE_FOOT_STEP_SAND_B,	// ���̏�ł̑���B
	eSE_LANDING_SAND,		// ���̏�ł̒��n��
	eSE_FOOT_STEP_SOIL_A,	// �y�̏�ł̑���A
	// 65
	eSE_FOOT_STEP_SOIL_B,	// �y�̏�ł̑���B
	eSE_LANDING_SOIL,		// �y�̏�ł̒��n��
	eSE_FALL_STONE,			// �΂𗎂Ƃ�����
	eSE_FALL_FOREST_SND_A,	// �X�̉��`
	eSE_FALL_FOREST_SND_B,	// �X�̉��a
	// 70
	eSE_HOLY,				// ���Ȃ銴���̉�
	eSE_GET_KEYITEM,		// �L�[�A�C�e���擾��
	eSE_HIT_SWORD_2,		// �a���Q
	eSE_PIYOPIYO,			// �҂�҂�
	eSE_HIT_SWORD_3,		// �a���R
	// 75
	eSE_PULL_ARCHERY,		// �|������
	eSE_SHOOT_ARCHERY,		// �|�����
	eSE_HIT_ARROW,			// �|��������
	eSE_CAT,				// �L
	eSE_TROLLEY,			// �g���b�R
	// 80
	eSE_MOTOR_START,		// ���[�^�[�E��]�n��
	eSE_MOTOR_RUNNING,		// ���[�^�[�E��]��
	eSE_WINGS,				// �H�΂���
	eSE_TORNADE,			// ����
	eSE_LASER,				// ���[�U�[
	// 85
	eSE_CANNON_LASER_CHARGE,// ���僌�[�U�[�E����
	eSE_CANNON_LASER,		// ���僌�[�U�[
	eSE_CANNON_LASER_DOING,	// ���僌�[�U�[�E���o��
	eSE_MACHINE_BOOTUP,		// �@�B�u�[�g�A�b�v
	eSE_SNAPPING,			// �r���^
	// 90
	eSE_BBA,				// ���邳�� BBA
	eSE_JET_AIRSHIP,		// ���D�̉�
	eSE_FOOT_STEP_ROBOT,	// ���{�b�g�̑���
	eSE_ELEVATOR,			// �G���x�[�^�[��
	eSE_DRAGON_FLYING,		// ���̔�s
	// 95
	eSE_BRAKE,				// �u���[�L��
	eSE_OCARINA_WILEDNESS,	// �I�J���i���E�����̉S
	eSE_OCARINA_WING,		// �I�J���i���E���̉S
	eSE_OCARINA_HANDLING,	// �I�J���i���E����̉S
	eSE_OCARINA_SUN,		// �I�J���i���E���z�̉S
	
	eSE_MAX,
};

/*=====================================*
 * BGM
 *=====================================*/
/**** �L�����̃e�[�} ****/
// �^�C�g��
#define BGM_TITLE			("WindOfBorghese.mp3")				// �{���Q�[�[�̕�

// ���m�̃e�[�}
#define BGM_LINO_THEMA		("HillOfSolarClock.ogg")			// �����v�̋u
// �G���_�̃e�[�}
#define BGM_ELDA_THEMA		("TraditionalHill.ogg")				// �`���̋u

/**** �X�̋� ****/
#define BGM_LIBELUNIA		("SmallTown.ogg")					// �����Ȓ�
#define BGM_CASTLE			("Strategy_loop.ogg")				// PerituneMaterial_Strategy_loop
#define BGM_CASTLE_2		("Kishidan.ogg")					// �R�m�c
#define BGM_COLLEGE			("DoorOfMagicIsOpenedNight.ogg")	// ���@�̔��͖�J��
#define BGM_HARBOR			("Resort.ogg")						// PerituneMaterial_Resort
#define BGM_VILLAGE			("SimpleVillage.ogg")				// �f�p�ȑ�

/**** �t�B�[���h ****/
#define BGM_FIELD			("OverWorld5.ogg")					// PerituneMaterial_OverWorld5_loop
#define BGM_RIDE_VEHICLE	("RideVehicle.ogg")					// ���ɗh����
#define BGM_DEPARTURE		("Departure.ogg")					// �o���i�����났�l�j
#define BGM_JAPAN			("ohka_japan.ogg")					// ohka_japan.ogg

/**** �_���W���� ****/
#define BGM_DUNGEON_A		("02_Dungeon/Hishou.ogg")			// �e��
#define BGM_DUNGEON_C		("RaceOfSand.ogg")					// ���̖���
#define BGM_DUNGEON_3		("02_Dungeon/EdgeAndCorridor.ogg")	// 
#define BGM_DUNGEON_D		("GoesToWarFire.ogg")				// ��΂֕���
#define BGM_DUNGEON_E		("Deep_Woods_loop.ogg")				// PerituneMaterial_Deep_Woods_loop
#define BGM_DUNGEON_F		("02_Dungeon/FreezingWorld.ogg")	// �X���̐��E
#define BGM_DUNGEON_F_2		("02_Dungeon/loop_122.ogg")			// loop_122
#define BGM_DUNGEON_G		("The_Night_Industrial.ogg")		// The_Night_Industrial
#define BGM_DUNGEON_Z		("EternalSamsara.ogg")				// �i�v�̗։�

/**** �{�X�� ****/
#define BGM_BOSS_A			("Hodou.ogg")						// �e��
#define BGM_BOSS_B			("A-pe-kamuy.ogg")					// a-pe-kamuy_mixv2
#define BGM_BOSS_C			("DigitalWorld.ogg")				// DigitalWorld
#define BGM_BOSS_W			("FlameOfJudge.ogg")				// �ق��̉�
#define BGM_BOSS_D			("refrectable.mp3")					// refectable

/**** �X�g�[���[ ****/
#define BGM_FORCE_PLACE		("02_Dungeon/TownOfThe Mirage.ogg")	// 凋C�O�̊X
#define BGM_CRYSTAL_PLACE	("02_Dungeon/YugenNoMai.ogg")		// �H���̕�
#define BGM_SKYSHIP			("Amakakeru.ogg")					// �V�삯��
#define BGM_SERIUS			("Keisou1.ogg")						// �����T
#define BGM_DUNGEON_B		("02_Dungeon/Sougenwoyuku.ogg")		// �������䂭
#define BGM_ENDING			("StaffRoll.ogg")					// ������n

/**** ���̑� ****/
#define BGM_SKYLAND			("GreenWind.ogg")
#define BGM_SHIP			("UnderLake.ogg")
#define BGM_ZELDA			("Zelda.ogg")
#define BGM_TOWN			("City24.ogg")
#define BGM_DUMMY			("")

#endif // __VOICE_DEF_H__

 /**** end of file ****/
