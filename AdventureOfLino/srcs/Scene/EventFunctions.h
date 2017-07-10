/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �C�x���g�֘A�W��
 * @author SPATZ
 * @data   2013/08/03 10:40:29
 */
//---------------------------------------------------
#ifndef __EVENT_FUNCTIONS_H__
#define __EVENT_FUNCTIONS_H__

/*=====================================*
 * include
 *=====================================*/
#include "AppLib/Graphic/ModelPmd.h"
#include "DataDefine/ItemDef.h"
#include "Gimmick/GimmickBase.h"

#define RES_LYT_INFO_SELECT_BACK_1		"Selectfile1.jpg"
#define RES_LYT_INFO_SELECT_BACK_2		RES_LYT_INFO_SELECT_BACK_1
#define RES_LYT_INFO_SELECT_BACK_3		"Selectfile3.jpg"
#define RES_LYT_INFO_SELECT_BACK_4		"Selectfile4.jpg"
#define RES_LYT_INFO_SELECT_BACK_5		"Selectfile5.jpg"
#define RES_LYT_INFO_SELECT_BACK_C		RES_LYT_INFO_SELECT_BACK_1

/*=====================================*
 * define
 *=====================================*/
#define EVENT_FLAG(def)		(1<<def)

/*=====================================*
 * �C�x���g�֘A
 *=====================================*/
/* �����ɒǉ������牺�̃e�[�u���ɂ��ǉ�����l�ɁI */

enum {
    eEVENT_00  = 0,
    eEVENT_01,          // 1 
    eEVENT_08,          // 2 ���̍�
    eEVENT_10,          // 3 ���x���j�A�r��
    eEVENT_11,          // 4 ���x���j�A�`
    eEVENT_13,          // 5 �M���X
    eEVENT_14,          // 6 ���p��w
    eEVENT_16,          // 7 �Y�p�̐X
    eEVENT_21,          // 8 ���x���j�A���Ƌ�
    eEVENT_22,          // 9 ���m�̉�
    eEVENT_24,          // 10 �n�����H
    eEVENT_25,          // 11 �n���S
    eEVENT_26,          // 12 �n���
    eEVENT_28,          // 13 �X�̓��A
    eEVENT_29,          // 14 ���x���j�A����E��
    eEVENT_34,          // 15 �����̐X
    eEVENT_35,          // 16 �W���g��
    eEVENT_36,          // 17 �X���g�z�R
    eEVENT_38,          // 18 �N���X�^�����[��
    eEVENT_39,          // 19 �X���g�z�R�E�g���b�R�ꏊ
    eEVENT_40,          // 20 �n���b�p
    eEVENT_41,          // 21 �o�x���̓�
    eEVENT_43,          // 22 �o�x���̓��E�{�X
    eEVENT_44,          // 23 �o�x���̓��E�G���x�[�^
    eEVENT_46,          // 24 �o�x���̓��E�S��
    eEVENT_47,          // 25 ����_���X
    eEVENT_48,          // 26 �o�x���̓��E�O��
    eEVENT_49,          // 27 �n���b�p�E�g���l��
    eEVENT_52,          // 28 �o�x���̓�2
    eEVENT_MAX,          // 
    eEVENT_ERROR = -1,
};

enum {
	eEVENT_00_OPENING = 0,
	
	eEVENT_00_MAX,
};
enum {
	eEVENT_01_GET_MASTER_SWORD = 0,
	eEVENT_01_END_BATTLE_FRES,

	eEVENT_01_MAX,
};
enum {
	eEVENT_08_BATTLE_QUEST_2,
	eEVENT_08_BATTLE_QUEST_2_END,
	eEVENT_08_START_ROOM_BOSS,
	eEVENT_08_END_ROOM_BOSS,
	eEVENT_08_CANNON_JUMP,
	
	eEVENT_08_MAX,
};
enum {
	// 0
	eEVENT_10_CURE_PLAYER,
	eEVENT_10_RIDE_VEHICLE,
	eEVENT_10_MOVE_FOREST,
	eEVENT_10_MOVE_MINE,
	eEVENT_10_GET_MAGIC_TORNADE,
	// 5
	eEVENT_10_RIDE_VEHICLE_LONG,
	eEVENT_10_GET_FAILY,

	eEVENT_10_MAX,
};

enum {
	eEVENT_11_BATTLE_QUEST_2 = 0,
	eEVENT_11_BATTLE_QUEST_2_END,
	eEVENT_11_HINT_STONE,
	eEVENT_11_GOTO_OVERSEA,
	eEVENT_11_BACKTO_OVERSEA,
	eEVENT_11_WAKEUP_TAKORUKA,

	eEVENT_11_MAX,
};

enum {
	eEVENT_13_TAKORUKA,
	eEVENT_13_GOTO_PRISON,

	eEVENT_13_MAX,
};

enum {
	eEVENT_14_WELCOM_COLLEAGE,
	eEVENT_14_WILL_TALK,
	eEVENT_14_ABOUT_SULT_MINE,
	eEVENT_14_WISEMAN,

	eEVENT_14_MAX,
};

enum {
	// 0
	eEVENT_16_WELCOM_FOREST,
	eEVENT_16_USAEMON_COME_1,
	eEVENT_16_USAEMON_COME_2,
	eEVENT_16_RESCUE_USACHU,
	eEVENT_16_SET_STONE_1,
	// 5
	eEVENT_16_SET_STONE_2,
	eEVENT_16_RIVER_IS_LOWER,
	eEVENT_16_RIVER_IS_UPPER,
	eEVENT_16_WARP_CRYSTAL_HALL,
	eEVENT_16_APPEAR_SEASOW,
	// 10
	eEVENT_16_OPEN_WIRE_A,
	eEVENT_16_CLOSE_WIRE_A,
	eEVENT_16_OPEN_WIRE_B,
	eEVENT_16_CLOSE_WIRE_B,
	eEVENT_16_SWORD_TREASURE,
	// 15 
	eEVENT_16_USAEMON_COME_1_B,
	eEVENT_16_RESCUE_USACHU_AFTER,
	eEVENT_16_REVERT_USACHU,
	eEVENT_16_HELP_USACHU,
	eEVENT_16_SET_STONE_3,
	// 20
	eEVENT_16_USAEMON_COME_3,

	eEVENT_16_MAX,
};

enum {
	eEVENT_21_NAVI_EXPLAIN,
	eEVENT_21_BATTLE_QUEST_1,
	eEVENT_21_BATTLE_QUEST_1_END,
	eEVENT_21_MOVE_TO_EARTH,
	eEVENT_21_MOVE_TO_COLLEAGE,
	eEVENT_21_BIN_SHOP,

	eEVENT_21_MAX,
};
enum {
	eEVENT_22_WAKEUP,
	eEVENT_22_GENERAL_OCARINA,
	eEVENT_22_MOVE_TO_HOME_OUT,
	eEVENT_22_MOVE_TO_STAIR_UP,

	eEVENT_22_MAX,
};
enum {
	eEVENT_24_WELCOME_WATERWAY = 0,
	eEVENT_24_DUMMY1,
	eEVENT_24_WATER_IS_LOWER,
	eEVENT_24_WATER_IS_UPPER,
	eEVENT_24_DUMMY2,
	eEVENT_24_CLEAR_BARRIER,
	eEVENT_24_WARP_ENTRANCE,
	eEVENT_24_BLOCK_TO_CASTLE,

	eEVENT_24_MAX,
};
enum {
	eEVENT_25_LEA_TALK = 0,

	eEVENT_25_MAX,
};
enum {
	eEVENT_26_BOSS_APPEAR = 0,
	eEVENT_26_CLEAR_BARRIER,
	eEVENT_26_END_BOSS_BATTLE,

	eEVENT_26_MAX,
};

enum {
	eEVENT_28_BOSS_APPEAR = 0,
	eEVENT_28_END_BOSS_BATTLE,
	eEVENT_28_BTL_TREASURE_A,

	eEVENT_28_MAX,
};

enum {
	// 0
	eEVENT_29_WELCOME_CASTLE = 0,
	eEVENT_29_LION_TREASURE,
	eEVENT_29_BOSS_START,
	eEVENT_29_BOSS_END,
	eEVENT_29_BTL_TREASURE_D,
	// 5
	eEVENT_29_GATE_SECURITY,
	eEVENT_29_BTL_TREASURE_A,
	eEVENT_29_BTL_TREASURE_B,
	eEVENT_29_BTL_TREASURE_C,
	eEVENT_29_BATTLE_ARMER,
	// 10
	eEVENT_29_BIG_KEY,
	eEVENT_29_ABOUT_MARUIFORCE,
	eEVENT_29_MINO_GUARDIAN_OR_MOVE,
	eEVENT_29_APPEAR_WIZARD,
	eEVENT_29_COME_FRES,
	// 15
	eEVENT_29_APPEAR_TRUMP,
	eEVENT_29_BATTLE_QUEST_3,
	eEVENT_29_BATTLE_QUEST_3_END,
	eEVENT_29_MOVE_RICHCITY,
	eEVENT_29_GET_MAGIC_FLAME,
	// 20
	eEVENT_29_MOVE_UPPER_CASTLE,
	eEVENT_29_MOVE_LOWER_CASTLE,
	eEVENT_29_REVIVAL_POPPO,
	eEVENT_29_GO_TO_FRESS_ROOM,

	eEVENT_29_MAX,
};

enum {
	eEVENT_34_MOVE_TO_ANY_WHERE,
	eEVENT_34_PLAYED_SONG_OF_SUN,

	eEVENT_34_MAX,
};

enum {
	eEVENT_35_WELCOME_VILLAGE = 0,
	eEVENT_35_END_BETA_GAME,
	eEVENT_35_GET_SONG_WILDNESS,
	eEVENT_35_GET_SONG_WING,
	eEVENT_35_GET_SONG_SUN,
	eEVENT_35_AFTER_GET_GREEN_CRYSTAL,

	eEVENT_35_MAX,
};

enum {
	// 0
	eEVENT_36_CLEAR_BARRIER_0 = 0,
	eEVENT_36_CLEAR_BARRIER_1,
	eEVENT_36_CLEAR_BARRIER_2,
	eEVENT_36_MOVE_REVERSE_0,

	eEVENT_36_DIRECTION_LIGHT_ON,
	// 5
	eEVENT_36_DIRECTION_LIGHT_OFF,
	eEVENT_36_BOSS_START,
	eEVENT_36_APPEAR_HEARTCASE,
	eEVENT_36_WARP_TREASUREROOM,
	eEVENT_36_WARP_MINE,
	// 10
	eEVENT_36_WELCOME_MINE,
	eEVENT_36_BOSS_END,
	eEVENT_36_DIRECTION_LIGHT_ON_1ST,				// ����̂�
	eEVENT_36_CLEAR_BARRIER_3,
	eEVENT_36_MOVE_REVERSE_1,

	// 15
	eEVENT_36_FALL_ONLY_BOSS,			// �{�X�펞�݂̗̂����C�x���g
	eEVENT_36_ADVICE_TROLLEY,			// �g���b�R�ɂ��ẴA�h�o�C�X
	eEVENT_36_ADVICE_MINE,				// �z�R�ɂ��ẴA�h�o�C�X

	eEVENT_36_MAX,
};

enum {
	eEVENT_38_CRYSTAL_FOREST = 0,
	eEVENT_38_TALK_SENTO,
	eEVENT_38_CRYSTAL_EARTH,
	eEVENT_38_TALK_YODEL,
	eEVENT_38_WARP_OUT_WORLD,

	eEVENT_38_MAX,
};

enum {
	eEVENT_39_GO_TO_OUTSIDE = 0,
	eEVENT_39_GO_TO_INSIDE,

	eEVENT_39_MAX,
};

enum {
	// 0
	eEVENT_40_WELCOME_WILDERNESS = 0,	// �n���b�p����
	eEVENT_40_TORNADE_EVENT,			// �����J�n�C�x���g
	eEVENT_40_BATTLE_TEEDER,			// �Ă��[���[��
	eEVENT_40_BATTLE_USIDERMAN,			// �E�T�C�_�[�}����
	eEVENT_40_FALL_ONLY_BOSS,			// �E�T�C�_�[�}����݂̗̂����C�x���g
	// 5
	eEVENT_40_FAILED_USIDERMAN,			// �E�T�C�_�[�}����E���s
	eEVENT_40_DISAPPER_TORNADE,			// ��������
	eEVENT_40_WAKEUP_BABEL,				// �o�x���̓��o��
	eEVENT_40_MOVE_BABEL,				// �o�x���̓��ړ��C�x���g
	eEVENT_40_BTL_TREASURE_A,			// �o�g����
	// 10
	eEVENT_40_BLOCK_TORNADE,			// �����ɋ߂Â��Ȃ��C�x���g
	eEVENT_40_BLOCK_TO_SULT,			// �X���g�z�R�߂�Ȃ��C�x���g
	eEVENT_40_GET_MAGIC_THUNDER,

	eEVENT_40_MAX,
};

enum {
	// 0
	eEVENT_41_DUMMY_00,
	eEVENT_41_DUMMY_01,
	eEVENT_41_WELCOM_TOWER,				// �������񂷃C�x���g
	eEVENT_41_POWER_ON,					// �d��ON
	eEVENT_41_REVERT_GUARDIAN,			// �K�[�f�B�A����߂�
	// 5
	eEVENT_41_BTL_TREASURE_A,			// �o�g����
	eEVENT_41_BTL_TREASURE_B,			// �o�g����
	eEVENT_41_BTL_TREASURE_C,			// �o�g����
	eEVENT_41_BIG_KEY,

	eEVENT_41_MAX,
};

enum {
	eEVENT_43_BOSS_APPEAR = 0,
	eEVENT_43_END_BOSS_BATTLE,
	eEVENT_43_BTL_TREASURE_A,
	
	eEVENT_43_MAX,
};

enum {
	eEVENT_44_ELEVATOR_UP,
	
	eEVENT_44_MAX,
};

enum {
	eEVENT_46_HELP_SEEK = 0,
	eEVENT_46_END_ENEMY_BATTLE,
	eEVENT_46_WATER_IS_LOWER,
	eEVENT_46_WATER_IS_UPPER,
	
	eEVENT_46_MAX,
};

enum {
	// 0
	eEVENT_47_WELCOME_AIRSHIP = 0,
	eEVENT_47_TALK_SEEK,
	eEVENT_47_STAFF_ROLL,
	eEVENT_47_FALL_AIRSHIP,
	eEVENT_47_EXIT_AIRSHIP,
	// 5
	eEVENT_47_CRASH_AIRSHIP,
	
	eEVENT_47_MAX,
};

enum {
	eEVENT_48_APPEAR_SKYSHIP,			// �����o��
	eEVENT_48_MIDDLE_PLACE,				// ���K�w�C�x���g
	eEVENT_48_APPEAR_TOWER,				// ���o��
	eEVENT_48_GET_SONG_HANDLING,		// �S�擾

	eEVENT_48_MAX,
};

enum {
	eEVENT_49_MOVE_BABEL,

	eEVENT_49_MAX,
};

enum {
	// 0
	eEVENT_52_CLEAR_BARRIER_1,
	eEVENT_52_TRAP_BATTLE_1,
	eEVENT_52_WAKEUP_LIFT_1,
	eEVENT_52_CLEAR_BARRIER_2A,
	eEVENT_52_CLEAR_BARRIER_2B,
	// 5
	eEVENT_52_CLEAR_BARRIER_3A,
	eEVENT_52_CLEAR_BARRIER_3B,
	eEVENT_52_CLEAR_LASER_3F,
	eEVENT_52_CLEAR_BARRIER_3C1,
	eEVENT_52_CLEAR_BARRIER_3C2,
	// 10
	eEVENT_52_TRAP_BATTLE_3,
	eEVENT_52_TRAP_BATTLE_4F_A,
	eEVENT_52_CLEAR_BARRIER_4F_A,
	eEVENT_52_WAKEUP_LIFT_4F_A,
	eEVENT_52_BOOTUP_MAGIC_ARMER,
	// 15
	eEVENT_52_CLEAR_BARRIER_4F_B,
	eEVENT_52_UP_CUBE_BLOCK_4F_A,
	eEVENT_52_DN_CUBE_BLOCK_4F_A,
	eEVENT_52_UP_CUBE_BLOCK_4F_B,
	eEVENT_52_DN_CUBE_BLOCK_4F_B,
	// 20
	eEVENT_52_UP_CUBE_BLOCK_4F_C,
	eEVENT_52_DN_CUBE_BLOCK_4F_C,
	eEVENT_52_UP_CUBE_BLOCK_4F_D,
	eEVENT_52_DN_CUBE_BLOCK_4F_D,
	eEVENT_52_UP_CUBE_BLOCK_4F_E,
	// 25
	eEVENT_52_DN_CUBE_BLOCK_4F_E,
	eEVENT_52_CLEAR_BARRIER_4F_C,

	eEVENT_52_MAX,
};

const BYTE kEVENT_FLAG_NUM		= 32;	// �t���O�̓s����A4byte�ȏ�ɂ͂��Ȃ����ƁI

/*=====================================*
 * �i�r�H�̏��
 *=====================================*/
static const char* strNaviTalkNoMap					= "�}�b�v�͂܂���ɓ���ĂȂ�����g���Ȃ���";
static const char* strNaviTalkNoKey					= "���̕󔠂��J����ɂ͑傫�Ȍ����K�v����I";
static const char* strNaviTalk_WizardProtectStart	= "�i�Ղ̑̂ɃV�[���h����܂ꂽ��I";
static const char* strNaviTalk_BattleUsidermanA		= "�����̌`���ς���Ă�����I";
static const char* strNaviTalk_BattleUsidermanB		= "�z�̑̂���������o���I�`�����X����I";
static const char* strNaviTalk_CheckPoint			= "�`�F�b�N�|�C���g��ŁI";
static const char* strNaviTalk_NoMagicPoint			= "�}�W�b�N�|�C���g������Ȃ���I";
static const char* strNaviTalk_CannotCosplay		= "���̏�ł̓R�X�v���ɒ��ւ���Ȃ���I";
static const char* strNaviTalk_CannotOcarinaEnemy	= "�G������Ԃ͎g���Ȃ���I";
static const char* strNaviTalk_CannotOcarinaGimmick	= "���肵���n�ʂ̏ザ��Ȃ��Ɛ����Ȃ���I";
static const char* strNaviTalk_CannotOcarinaNoSong	= "���m�����t�ł���Ȃ��o���ĂȂ���I";
static const char* strNaviTalk_CannotFlyingMove		= "���̏ꏊ�ł͂ۂ��۔��݂����҂ł��Ȃ���I";
static const char* strNaviTalk_BreakedMagicArmer	= "�}�W�b�N�A�[�}�[����ꂽ�IF4�L�[�ł�蒼����...";
static const char* strNaviTalk_NotFoundTitanis		= "��ꂻ���ȃe�B�^�j�X���߂��ɂ��Ȃ���I";
static const char* strNaviTalk_NotFoundMagicArmer	= "���ꂻ���ȃ}�W�b�N�A�[�}�[���߂��ɂ��Ȃ���I";
static const char* strNaviTalk_NothingToDoSongSun	= "�����Ŏg���Ă������N����Ȃ���I";

// �X�e�[�W3
static const char* strNaviTalk_RedDragonAdviceA		= "���̗�.. ���ȊO�͍U���������Ȃ��̂���..�H";
static const char* strNaviTalk_RedDragonAdviceB		= "����f���Ă�Ԃɖ����������ӂ��������ł����...";
static const char* strNaviTalk_RedDragonAdviceC		= "����f���Ă���Ԃɓz�̓��̏ォ��U������񂾁I";

// �X�e�[�W4
static const char* strNaviTalk_DemonAdviceA			= "�z���������... �|��ŉ󂹂Ȃ����ȁH";
static const char* strNaviTalk_DemonAdviceB			= "���x�̓s���N�̊₪�����I ������|�ő_����I�H";
static const char* strNaviTalk_DemonAdviceC			= "���x�͂Q�����I �S���󂹂�I�H";

/*=====================================*
 * �i�r�H�g�[�N
 *=====================================*/
const char* GetStrNaviTalkMessage(int eventDef, int taskValue);

/*=====================================*
 * �t�B�[���h���̏��
 *=====================================*/
#define GET_FIELD_STAGE_LIST_NUM(array)			(sizeof(array) / sizeof(const int))

struct tFieldWork {
	const int*		mpArray;
	int				mArrayNum;
	const char**	mName;
};

// �t�B�[�h���̗񋓎q
enum {
	eFIELD_ERROR = -1,

	eFIELD_HOME = 0,
	eFIELD_REBERNIAN,
	eFIELD_WILDERNESS,
	eFIELD_WATERWAY,
	eFIELD_PRISM,
	eFIELD_CASTLE_LOW,
	eFIELD_VILLAGE,
	eFIELD_FOREST,
	eFIELD_CRYSTAL_ROOM,
	eFIELD_SULT_MINE,
	eFIELD_SULT_HARAPPA,
	eFIELD_SULT_BABEL,
	eFIELD_AIRSHIP,

	eFIELD_OTHER,

	eFIELD_MAX,
};

/* �e�t�B�[���h�Ɋ��蓖�Ă�X�e�[�W�C���f�b�N�X */
static const int sFieldStageListHome[]			= {22};								// eFIELD_HOME = 0,
static const int sFieldStageListRebernian[]		= {13, 14, 21};                     // eFIELD_REBERNIAN,
static const int sFieldStageListWilderness[]	= {8, 10, 11};                      // eFIELD_WILDERNESS,
static const int sFieldStageListWaterway[]		= {24, 26, 27};                     // eFIELD_WATERWAY,
static const int sFieldStageListPrism[]			= {25};                             // eFIELD_PRISM,
static const int sFieldStageListCastleLow[]		= {12, 29};                         // eFIELD_CASTLE_LOW,
static const int sFieldStageListVillage[]		= {35};                             // eFIELD_VILLAGE,
static const int sFieldStageListForest[]		= {16, 19, 28, 34};                  // eFIELD_FOREST,
static const int sFieldStageListCrystalRoom[]	= {38};                             // eFIELD_CRYSTAL_ROOM,
static const int sFieldStageListSultMine[]		= {36, 37, 39};                     // eFIELD_SULT_MINE,
static const int sFieldStageListHarappa[]		= {40, 48, 49};                     // eFIELD_SULT_HARAPPA,
static const int sFieldStageListBabel[]			= {41, 42, 43, 44, 45, 46, 52};     // eFIELD_SULT_BABEL,
static const int sFieldStageListAirShip[]		= {47};                             // eFIELD_AIRSHIP,
static const int sFieldStageListOther[]			= {0, 14, 15, 20, 23, 30, 31, 	   // eFIELD_OTHER,
	50, 51, 60, 61, 62, 63, 64, 65, 66, 67, 80, 81, 82, 90, 91, 92, 93};

/* �i����� */
static const char* sFieldStageNameHome[4]		= {"�͂��߂���",	"",						"",							NULL                      };	// eFIELD_HOME = 0,
static const char* sFieldStageNameRebernian[4]	= {"�v�����[�O",	"���x���j�A����",		"�`���̎n�܂�",				RES_LYT_INFO_SELECT_BACK_1};    // eFIELD_REBERNIAN,
static const char* sFieldStageNameWilderness[4]	= {"�v�����[�O",	"���x���j�A�r��",		"�`���̎n�܂�",				RES_LYT_INFO_SELECT_BACK_1};    // eFIELD_WILDERNESS,
static const char* sFieldStageNameWaterway[4]	= {"Stage1",		"���x���j�A�n�����H",	"�n�����H�̈���",			RES_LYT_INFO_SELECT_BACK_1};    // eFIELD_WATERWAY,
static const char* sFieldStageNamePrism[4]		= {"Stage2",		"�n���S",				"���𑀂鍕�����m",			RES_LYT_INFO_SELECT_BACK_2};    // eFIELD_PRISM,
static const char* sFieldStageNameCastleLow[4]	= {"Stage2",		"���x���j�A����",		"���𑀂鍕�����m",			RES_LYT_INFO_SELECT_BACK_2};    // eFIELD_CASTLE_LOW,
static const char* sFieldStageNameVillage[4]	= {"Stage3",		"�W���g��",				"�X�ɖ���N���X�^��",		RES_LYT_INFO_SELECT_BACK_3};    // eFIELD_VILLAGE,
static const char* sFieldStageNameForest[4]		= {"Stage3",		"�Y�p�̐X",				"�X�ɖ���N���X�^��",		RES_LYT_INFO_SELECT_BACK_3};    // eFIELD_FOREST,
static const char* sFieldStageNameCrystalRoom[4]= {"Stage3",		"���p��w",				"�X�ɖ���N���X�^��",		RES_LYT_INFO_SELECT_BACK_3};    // eFIELD_CRYSTAL_ROOM,
static const char* sFieldStageNameSultMine[4]	= {"Stage4",		"�X�m���g�z�R",			"�Ñ㕶���̊X�ɑ����z�R",	RES_LYT_INFO_SELECT_BACK_4};    // eFIELD_SULT_MINE,
static const char* sFieldStageNameHarappa[4]	= {"Stage5",		"�Ñ�s�s�n���b�p",		"���mVS�Ñ㕶��",			RES_LYT_INFO_SELECT_BACK_5};    // eFIELD_SULT_HARAPPA,
static const char* sFieldStageNameBabel[4]		= {"Stage5",		"�Ñ�s�s�n���b�p",		"���mVS�Ñ㕶��",			RES_LYT_INFO_SELECT_BACK_5};    // eFIELD_SULT_BABEL,
static const char* sFieldStageNameAirShip[4]	= {"���ŃN���A�I",	"����",				"���x���j�A�ꕐ����",		RES_LYT_INFO_SELECT_BACK_C};    // eFIELD_AIRSHIP,
static const char* sFieldStageNameOther[4]		= {"���܂�",		"Other",				"Other",					NULL                      };    // eFIELD_OTHER,

/* �t�B�[���h�̃e�[�u�� */
static const tFieldWork sFieldWorkTable[eFIELD_MAX] = {
	{sFieldStageListHome,		GET_FIELD_STAGE_LIST_NUM(sFieldStageListHome),			sFieldStageNameHome,},			// eFIELD_HOME = 0,
	{sFieldStageListRebernian,	GET_FIELD_STAGE_LIST_NUM(sFieldStageListRebernian),		sFieldStageNameRebernian,},     // eFIELD_REBERNIAN,
	{sFieldStageListWilderness,	GET_FIELD_STAGE_LIST_NUM(sFieldStageListWilderness),	sFieldStageNameWilderness,},    // eFIELD_WILDERNESS,
	{sFieldStageListWaterway,	GET_FIELD_STAGE_LIST_NUM(sFieldStageListWaterway),		sFieldStageNameWaterway,},      // eFIELD_WATERWAY,
	{sFieldStageListPrism,		GET_FIELD_STAGE_LIST_NUM(sFieldStageListPrism),			sFieldStageNamePrism,},         // eFIELD_PRISM,
	{sFieldStageListCastleLow,	GET_FIELD_STAGE_LIST_NUM(sFieldStageListCastleLow),		sFieldStageNameCastleLow,},     // eFIELD_CASTLE_LOW,
	{sFieldStageListVillage,	GET_FIELD_STAGE_LIST_NUM(sFieldStageListVillage),		sFieldStageNameVillage,},       // eFIELD_VILLAGE,
	{sFieldStageListForest,		GET_FIELD_STAGE_LIST_NUM(sFieldStageListForest),		sFieldStageNameForest,},        // eFIELD_FOREST,
	{sFieldStageListCrystalRoom,GET_FIELD_STAGE_LIST_NUM(sFieldStageListCrystalRoom),	sFieldStageNameCrystalRoom,},  	// eFIELD_COLLEAGE,
	{sFieldStageListSultMine,	GET_FIELD_STAGE_LIST_NUM(sFieldStageListSultMine),		sFieldStageNameSultMine,},      // eFIELD_SULT_MINE,
	{sFieldStageListHarappa,	GET_FIELD_STAGE_LIST_NUM(sFieldStageListHarappa),		sFieldStageNameHarappa,},       // eFIELD_SULT_HARAPPA,
	{sFieldStageListBabel,		GET_FIELD_STAGE_LIST_NUM(sFieldStageListBabel),			sFieldStageNameBabel,},         // eFIELD_SULT_BABEL,
	{sFieldStageListAirShip,	GET_FIELD_STAGE_LIST_NUM(sFieldStageListAirShip),		sFieldStageNameAirShip,},       // eFIELD_AIRSHIP,
	{sFieldStageListOther,		GET_FIELD_STAGE_LIST_NUM(sFieldStageListOther),			sFieldStageNameOther,},			// eFIELD_OTHER,
};

#define kFIELD_INFO_MAP_BIT				(0x80000000)	// �}�b�v�̎w��̃r�b�g
#define kFIELD_INFO_KEY_BIT				(0x40000000)	// �{�X���̎w��r�b�g
#define kFIELD_INFO_START_BIT			(0x20000000)	// �X�e�[�W�J�n�r�b�g

#define kFIELD_INFO_XX_ARENA_MASK		(0x0000000F)	// ������r�b�g�}�X�N (�ő�15���܂Ŏ����\)
#define kFIELD_INFO_XX_ARENA_BATTLE_NUM	(6)

// �e�t�B�[���h�̏��r�b�g

// Stage 08
#define kFIELD_INFO_08_ARENA_1CLEAR_BIT	(0x00000001)	// ������P�N���A�r�b�g
#define kFIELD_INFO_08_ARENA_2CLEAR_BIT	(0x00000002)	// ������Q�N���A�r�b�g
#define kFIELD_INFO_08_ARENA_3CLEAR_BIT	(0x00000003)	// ������R�N���A�r�b�g
#define kFIELD_INFO_08_ARENA_4CLEAR_BIT	(0x00000004)	// ������S�N���A�r�b�g
#define kFIELD_INFO_08_ARENA_5CLEAR_BIT	(0x00000005)	// ������T�N���A�r�b�g

// Stage 21
#define kFIELD_INFO_21_BUY_BIN_BIT		(0x00000010)	// �r���w���r�b�g
#define kFIELD_INFO_21_ARENA_1CLEAR_BIT	(0x00000001)	// ������P�N���A�r�b�g
#define kFIELD_INFO_21_ARENA_2CLEAR_BIT	(0x00000002)	// ������Q�N���A�r�b�g
#define kFIELD_INFO_21_ARENA_3CLEAR_BIT	(0x00000003)	// ������R�N���A�r�b�g
#define kFIELD_INFO_21_ARENA_4CLEAR_BIT	(0x00000004)	// ������S�N���A�r�b�g
#define kFIELD_INFO_21_ARENA_5CLEAR_BIT	(0x00000005)	// ������T�N���A�r�b�g

// Stage 29
#define kFIELD_INFO_29_TRUMP_PUT_K		(0x00000010)	// �g�����v�j�u������
#define kFIELD_INFO_29_TRUMP_PUT_Q		(0x00000020)	// �g�����v�p�u������
#define kFIELD_INFO_29_TRUMP_PUT_J		(0x00000040)	// �g�����v�i�u������
#define kFIELD_INFO_29_ARENA_1CLEAR_BIT	(0x00000001)	// ������P�N���A�r�b�g
#define kFIELD_INFO_29_ARENA_2CLEAR_BIT	(0x00000002)	// ������Q�N���A�r�b�g
#define kFIELD_INFO_29_ARENA_3CLEAR_BIT	(0x00000003)	// ������R�N���A�r�b�g
#define kFIELD_INFO_29_ARENA_4CLEAR_BIT	(0x00000004)	// ������S�N���A�r�b�g
#define kFIELD_INFO_29_ARENA_5CLEAR_BIT	(0x00000005)	// ������T�N���A�r�b�g


// Stage 40
//	�y�K�T�X�̊��͍폜�����ׁA(0x00000001)�̓��U�[�u
#define kFIELD_INFO_40_BAB_HOLE_BREAK_A	(0x00000002)	// �o�x���̓��̉��̌���j�󂵂Ă��邩
#define kFIELD_INFO_40_BAB_HOLE_BREAK_B	(0x00000004)	// �o�x���̓��̏�̌���j�󂵂Ă��邩

// ������̃����N
enum eBattleQuestRank {
	eBATTLE_QUEST_RANK_A = 0,	// �n��\�I
	eBATTLE_QUEST_RANK_B,		// �{��
	eBATTLE_QUEST_RANK_C,		// �S�����

	eBATTLE_QUEST_RANK_MAX,
};

/*=====================================*
 * ���J�֐�
 *=====================================*/	
bool	ExecuteEventFunc(int eventKind, int eventFlag);
BOOL	IsLongEventFlag(int eventKind, int eventFlag);
/* �X�e�[�W�C���f�b�N�X�� eEVENT_??? �ɕϊ�����֐� */
int		ConvertPlaceIndex_TO_EventDefine(int placeIndex);
/* �X�e�[�W�C���f�b�N�X�� eFIELD_??? �ɕϊ�����֐� */
int		ConvertPlaceIndex_TO_FieldInfoDefine(int stageIndex);


/*=====================================*
 * �����֐�
 *=====================================*/
typedef bool (*EventFunc)(void);

bool	eventGeneral_BlockCharactor(int checkStartEventPhase, VECTOR* pOfsPos, float chRot, char* pChName, char* pTalk[4]);
bool	eventGeneral_BlockCharactor(int checkStartEventPhase, int placeIndex, char* pChName, char* pTalk[4]);
bool	eventGeneral_MovePlace(int checkStartEventPhase, int stagePlaceIndex);
bool	eventGeneral_ClearBarrier(int frameIndex, int checkStartEventPhase, SHORT camDeg=-1);
bool	eventGeneral_Common_BattleQuestUpdate(int clearMatchNum);

bool	eventXX_BattleQuest(eBattleQuestRank rank, int clearMatchNum);
bool	eventXX_BattleQuest_End(eBattleQuestRank rank, int clearMatchNum);
bool	eventGeneral_Common_BattleQuestUpdate(eBattleQuestRank rank, int clearMatchNum);

void	eventGeneral_NaviLuminusStart(ModelPmd* pModel);
void	eventGeneral_NaviLuminusUpdate(ModelPmd* pModel);
void	eventGeneral_NaviLuminusBigLight(void);
void	eventGeneral_NaviLuminusEnd(void);
bool	eventGeneral_BattleTreasure(int checkStartEventPhase=0);
bool	eventGeneral_WakeupGimmick(SHORT camDeg=-1);
bool	eventGeneral_AnythingMove(void);
bool	eventGeneral_NotHaveBook(void);					// �قނ�̏��������ċ��Ȃ��P�[�X
bool	eventGeneral_GetMagic(eItemKind itemKind);		// ���@�擾�̃C�x���g
bool	eventGeneral_GetSong(BYTE songType);			// �S�擾�̃C�x���g

bool	eventGeneral_SetObjectStone(int stagePlaceIndex);

bool	eventGeneral_MoveGimmick(int stagePlaceIndex, eGimmickKind gmkKind, VECTOR& rMoveVecRate);

bool	event00_Opening(void);
bool	event01_GetMasterSword(void);
bool	event01_EndBattleFres(void);

bool    event08_BattleQuest2(void);
bool    event08_BattleQuest2_End(void);
bool	event08_StartRoomBoss(void);
bool	event08_EndRoomBoss(void);
bool	event08_CannonJump(void);

bool	event10_CurePlayer(void);
bool	event10_RideVehicle(void);
bool	event10_MoveForest(void);
bool	event10_MoveMine(void);
bool	event10_GetMagicTornade(void);
bool    event10_RideVehicleLong(void);
bool    event10_GetFaily(void);

bool	event11_BattleQuest2(void);
bool	event11_BattleQuest2_End(void);
bool	event11_HintStone(void);
bool    event11_HintStone(void);
bool    event11_GoToOverSea(void);
bool    event11_BackToOverSea(void);
bool    event11_WakeupTakoruka(void);

bool	event13_Takoruka(void);
bool    event13_GoToPrison(void);
bool	event13_GoInRoom(void);
bool	event13_GoOutRoom(void);

bool	event14_WelcomeColleage(void);
bool	event14_WillTalk(void);
bool	event14_AboutSultMine(void);
bool    event14_WiseMan(void);

bool	event16_WelcomeForest(void);
bool	event16_UsaemonCome1(void);
bool	event16_UsaemonCome2(void);
bool	event16_RescueUsachu(void);
bool	event16_SetStone1(void);
bool	event16_SetStone2(void);
bool	event16_RiverIsLower(void);
bool	event16_RiverIsUpper(void);
bool	event16_WarpCrystalHall(void);
bool	event16_AppearSeasow(void);
bool	event16_OpenWireA(void);
bool	event16_CloseWireA(void);
bool	event16_OpenWireB(void);
bool	event16_CloseWireB(void);
bool	event16_SwordTreasure(void);
bool	event16_UsaemonCome1_B(void);
bool	event16_RescueUsachuAfter(void);
bool	event16_RevertUsachu(void);
bool	event16_HelpUsachu(void);
bool	event16_SetStone3(void);
bool    event16_UsaemonCome3(void);

bool	event21_NaviExplain(void);
bool	event21_BattleQuest1(void);
bool	event21_BattleQuest1_End(void);
bool	event21_MoveToEarth(void);
bool	event21_MoveToColleage(void);
bool    event21_BinShop(void);

bool	event22_Wakeup(void);
bool	event22_GeneralOcarina(void);
bool    event22_MoveToHomeOut(void);
bool    event22_MoveToStairUp(void);

bool	event24_WelcomeWaterway(void);
bool	event24_Dummy01(void);
bool	event24_WaterIsLower(void);
bool	event24_WaterIsUpper(void);
bool	event24_Dummy02(void);
bool	event24_ClearBarrier(void);
bool	event24_WarpEntrance(void);
bool	event24_BlockToCastle(void);

bool	event25_LeaTalk(void);

bool	event26_BossAppear(void);
bool	event26_ClearBarrier(void);
bool	event26_EndBossBattle(void);

bool	event28_BossAppear(void);
bool	event28_EndBossBattle(void);
bool	event28_BtlTreasureA(void);

bool	event29_WelcomeCastle(void);
bool	event29_LionTreasure(void);
bool	event29_BossStart(void);
bool	event29_BossEnd(void);
bool	event29_BtlTreasureD(void);
bool	event29_GateSecurity(void);
bool	event29_BtlTreasureA(void);
bool	event29_BtlTreasureB(void);
bool	event29_BtlTreasureC(void);
bool	event29_BattleArmer(void);
bool	event29_BigKey(void);
bool	event29_AboutMaruiforce(void);
bool	event29_MinoGuardianOrMove(void);
bool	event29_AppearWizard(void);
bool	event29_ComeFres(void);
bool	event29_AppearTrump(void);
bool	event29_BattleQuest3(void);
bool	event29_BattleQuest3_End(void);
bool	event29_MoveRichCity(void);
bool	event29_GetMagicFlame(void);
bool    event29_MoveUpperCastle(void);
bool    event29_MoveLowerCastle(void);
bool    event29_RevivalPoppo(void);
bool    event29_GoToFressRoom(void);

bool    event34_MoveToAnyWhere(void);
bool    event34_PlayedSongOfSun(void);

bool	event35_WelcomeVillage(void);
bool	event35_EndBetaGame(void);
bool    event35_GetSongWildness(void);
bool    event35_GetSongWing(void);
bool    event35_GetSongSun(void);
bool    event35_AfterGetGreenCrystal(void);

bool	event36_ClearBarrier00(void);
bool	event36_ClearBarrier01(void);
bool	event36_ClearBarrier02(void);
bool	event36_MoveReverse00(void);
bool	event36_DirectionLightOn(void);
bool	event36_DirectionLightOff(void);
bool	event36_BossStart(void);
bool	event36_AppearHeartCase(void);
bool	event36_WarpTreasureRoom(void);
bool	event36_WarpMine(void);
bool	event36_WelcomeMine(void);
bool	event36_BossEnd(void);
bool	event36_DirectionLightOn1st(void);
bool	event36_ClearBarrier03(void);
bool	event36_MoveReverse01(void);
bool	event36_FallOnlyBoss(void);
bool	event36_AdviceTrolley(void);
bool	event36_AdviceMine(void);

bool	event38_CrystalForest(void);
bool	event38_TalkSento(void);
bool	event38_CrystalEarth(void);
bool	event38_TalkYodel(void);
bool	event38_WarpOutWorld(void);

bool	event39_GoToOutside(void);
bool	event39_GoToInside(void);

bool	event40_WelcomWilderness(void);	
bool	event40_DisapperTornade(void);	
bool	event40_Dummy00(void);			
bool	event40_WakeupBabel(void);		
bool	event40_TornadeEvent(void);		
bool	event40_BattleUsiderman(void);	
bool	event40_BtlTreasureA(void);		
bool	event40_BattleTeeder(void);		
bool	event40_BlockToSult(void);		
bool	event40_FallOnlyBoss(void);		
bool	event40_BlockTornade(void);		
bool	event40_MoveBabel(void);		
bool	event40_FailedUsiderman(void);	
bool	event40_GetMagicThunder(void);	

bool	event48_AppearAirship(void);	
bool	event48_MiddlePlace(void);		
bool    event48_AppearTower(void);
bool    event48_GetSongHandling(void);

bool	event41_Dummy00(void);				// 0
bool	event41_Dummy01(void);				// 1
bool	event41_WelcomTower(void);			// 2
bool	event41_PowerOn(void);				// 3
bool	event41_RevertGuardian(void);		// 4
bool	event41_BtlTreasureA(void);	
bool	event41_BtlTreasureB(void);	
bool	event41_BtlTreasureC(void);	
bool	event41_BigKey(void);

bool	event43_BossStart(void);
bool	event43_EndBossBattle(void);
bool	event43_BtlTreasureA(void);

bool	event44_ElevatorUp(void);

bool	event46_HelpSeek(void);
bool	event46_EndEnemyBattle(void);
bool	event46_WaterIsLower(void);
bool	event46_WaterIsUpper(void);

bool	event47_WelcomeAirship(void);
bool	event47_TalkSeek(void);
bool	event47_StaffRoll(void);
bool	event47_FallAirship(void);
bool	event47_ExitAirship(void);
bool	event47_CrashAirship(void);

bool	event48_Dummy(void);

bool	event49_MoveBabel(void);

bool    event52_ClearBarrier01(void);
bool    event52_TrapBattle01(void);
bool    event52_WakeupLift01(void);
bool    event52_ClearBarrier02A(void);
bool    event52_ClearBarrier02B(void);
bool    event52_ClearBarrier03A(void);
bool    event52_ClearBarrier03B(void);
bool    event52_ClearLaser_3F(void);
bool    event52_ClearBarrier03C1(void);
bool    event52_ClearBarrier03C2(void);
bool    event52_TrapBattle03(void);
bool    event52_TrapBattle_4F_A(void);
bool    event52_ClearBarrier4F_A(void);
bool    event52_WakeupLift_4F_A(void);
bool    event52_BootupMagicArmer(void);
bool    event52_ClearBarrier4F_B(void);
bool    event52_UpCubeBlock_4F_A(void);
bool    event52_DnCubeBlock_4F_A(void);
bool    event52_UpCubeBlock_4F_B(void);
bool    event52_DnCubeBlock_4F_B(void);
bool    event52_UpCubeBlock_4F_C(void);
bool    event52_DnCubeBlock_4F_C(void);
bool    event52_UpCubeBlock_4F_D(void);
bool    event52_DnCubeBlock_4F_D(void);
bool    event52_UpCubeBlock_4F_E(void);
bool    event52_DnCubeBlock_4F_E(void);
bool    event52_ClearBarrier4F_C(void);

int		getEventFunctionEffectHandle(void);

void	DrawFontFunc_BattleUsiderManCounter(void);

/*=====================================*
 * �X�^�b�t���[���֘A
 *=====================================*/
void	startStaffRollEvent(BYTE csvFileIndex);
BOOL	isEndStaffRollEvent(void);
void	updateStaffRollEvent(void);
void	DrawFontFunc_StaffRollString(void);

#endif // __EVENT_FUNCTIONS_H__

 /**** end of file ****/
