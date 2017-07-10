/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * EnemyDef.cpp
 * @author SPATZ.
 * @data   2014/01/14 22:47:05	
 */
//---------------------------------------------------
#include "EnemyDef.h"
#include "CharaDef.h"

#include "AppLib/Resource/ResourceModel.h"

// �G���Ȃ�
#define ENEMY_NONE		((eEnemyKind)-1)

/*=====================================*
 * �G�p�����[�^�e�[�u��
 *=====================================*/
static const tEnemyWork sEnemyWorkTbl[eENEMY_KIND_MAX] = {
	{"Killer",           12.0f,               90.0f,                 200.0f,        kCHARA_WALK_SPEED*2.0f,      4,       eOBJECT_KIND_MONEY_1,    40,      "�L���[�}�V�[��",      },
	{NAME_Fighter,       12.0f,               45.0f,                 150.0f,        kCHARA_WALK_SPEED,           3,       eOBJECT_KIND_MONEY_1,    100,     "�t�@�C�^�[",          },
	{NAME_Fresbelk,      12.0f,               90.0f,                 200.0f,        kCHARA_WALK_SPEED*2.0f,      92,      eOBJECT_KIND_HEART,      40,      "�t���X�x���N",        },
	{"Giginebura",       14.0f,               520.0f,                360.0f,        kCHARA_WALK_SPEED*2.0f,      50,      eOBJECT_KIND_HEART,      40,      "�M�M�l�u��",          },
	{"Ray",              12.0f,               90.0f,                 260.0f,        kCHARA_WALK_SPEED*2.0f,      30,      eOBJECT_KIND_HEART,      40,      "���C",                },
	{"01/Dragon",        DRAGON_SCALE,        400.0f,                180.0f,        kCHARA_WALK_SPEED*2.0f,      70,      eOBJECT_KIND_HEART,      100,     "�g���P��",            },
	{"01/Gargoyle",      12.0f,               100.0f,                260.0f,        kCHARA_WALK_SPEED*2.0f,      7,       eOBJECT_KIND_HEART,      40,      "�K�[�S�C��",          },
	{"Anhel",            12.0f,               300.0f,                660.0f,        kCHARA_WALK_SPEED*2.0f,      50,      eOBJECT_KIND_HEART,      40,      "�A���w��",            },
	{NAME_Hatophon,      14.0f,               110.0f,                360.0f,        kCHARA_WALK_SPEED*2.0f,      36,      eOBJECT_KIND_HEART_CASE, 100,     "�n�g�t�H��",          },
	{NAME_Hatophon,      14.0f,               110.0f,                360.0f,        kCHARA_WALK_SPEED*2.0f,      36,      eOBJECT_KIND_HEART,      60,      "�n�g�t�H��",          },
	{"01/Dewri",         12.0f,               90.0f,                 200.0f,        kCHARA_WALK_SPEED*2.0f,      4,       eOBJECT_KIND_MONEY_1,    40,      "�Ƃ�",                },
	{"01/Goblin",        GOBLIN_SCALE,        90.0f,                 200.0f,        kCHARA_WALK_SPEED*1.0f,      8,       eOBJECT_KIND_HEART,      40,      "�S�u����",            },
	{"01/Bat",           12.0f,               90.0f,                 200.0f,        kCHARA_WALK_SPEED*2.5f,      2,       eOBJECT_KIND_MONEY_1,    40,      "�������肾���ӂ�",    },
	{"01/Minotaur",      12.0f,               90.0f,                 200.0f,        kCHARA_WALK_SPEED*2.0f,      4,       eOBJECT_KIND_HEART,      40,      "�~�m�^�E���X",        },
	{"01/Bear",          12.0f,               90.0f,                 200.0f,        kCHARA_WALK_SPEED*2.0f,      4,       eOBJECT_KIND_MONEY_1,    40,      "�N�}��",              },
	{"01/Malon",         12.0f,               90.0f,                 260.0f,        kCHARA_WALK_SPEED*1.5f,      55,      eOBJECT_KIND_HEART,      100,     "�}����",              },
	{"01/Knight",        12.0f,               90.0f,                 260.0f,        kCHARA_WALK_SPEED*1.0f,      8,       eOBJECT_KIND_HEART,      40,      "�R�m",                },
	{"01/Dragon",        DRAGON_SCALE*5.0f,   400.0f*5.0f,           180.0f*5.0f,   kCHARA_WALK_SPEED*10.0f,     400,     eOBJECT_KIND_HEART,      100,     "����g���P��",        },
	{"01/Goblin",        GOBLIN_SCALE*5.0f,   90.0f*5.0f,            200.0f*5.0f,   kCHARA_WALK_SPEED*5.0f,      300,     eOBJECT_KIND_HEART,      100,     "����S�u����",        },
	{NAME_DarkWizard,    12.0f,               90.0f,                 260.0f,        kCHARA_WALK_SPEED,           38,      eOBJECT_KIND_HEART_CASE, 0,       "�ł̎i��",            },
	{"01/Magician",      12.0f,               90.0f,                 320.0f,        kCHARA_WALK_SPEED*1.0f,      4,       eOBJECT_KIND_MONEY_5,    40,      "���@�g��",            },
	{"01/Armer",         10.0f,               90.0f,                 260.0f,        kCHARA_WALK_SPEED*1.0f,      5,       eOBJECT_KIND_MONEY_20,   40,      "�����Z",              },
	{NAME_Oak,           12.0f,               90.0f,                 160.0f,        kCHARA_WALK_SPEED*1.0f,      5,       eOBJECT_KIND_MONEY_10,   40,      "�I�[�N",              },
	{"01/Woodman",       12.0f,               90.0f,                 200.0f,        kCHARA_WALK_SPEED*2.0f,      4,       eOBJECT_KIND_MONEY_10,   40,      "�E�b�h�}��",          },
	{"01/Woodman",       10.0f*5.0f,          90.0f*5.0f,            200.0f*5.0f,   kCHARA_WALK_SPEED*2.0f,      20,      eOBJECT_KIND_MONEY_10,   40,      "�r�b�O�E�b�h�}��",    },
	{"01/Slime",         10.0f,               90.0f,                 50.0f,         kCHARA_WALK_SPEED*1.0f,      6,       eOBJECT_KIND_HEART,      40,      "���`�X���C��",        },
	{"01/Rabbit",        12.0f,               90.0f,                 200.0f,        kCHARA_WALK_SPEED*1.0f,      4,       eOBJECT_KIND_HEART,      40,      "������",              },
	{"01/RedDrag",       12.0f,               800.0f,                200.0f*5.0f,   kCHARA_WALK_SPEED*3.0f,      65,      eOBJECT_KIND_HEART_CASE, 100,     "�ԗ�",                },
	{"01/Minotaurus",    12.0f,               90.0f,                 200.0f,        kCHARA_WALK_SPEED*2.0f,      8,       eOBJECT_KIND_HEART,      40,      "�~�m�^�E���X",        },
	{"01/Demon",         475.0f,              1800.0f,               12500.0f,      kCHARA_WALK_SPEED*2.0f,      10,      eOBJECT_KIND_HEART_CASE, 100,     "�r���̃f�[����",      },
	{"01/Scorpion",      92.0f,               68.0f,                 52.0f,         kCHARA_WALK_SPEED*2.0f,      6,       eOBJECT_KIND_MONEY_10,   60,      "���u�T�\��",          },
	{"01/RobotA",        12.0f,               80.0f,                 200.0f,        kCHARA_WALK_SPEED*1.4f,      8,       eOBJECT_KIND_MONEY_10,   40,      "N-YAN���{",           },
	{"01/RobotB",        12.0f,               80.0f,                 200.0f,        kCHARA_WALK_SPEED*1.0f,      4,       eOBJECT_KIND_HEART,      80,      "N-YAN���{BLUE",       },
	{"01/RobotC",        12.0f,               80.0f,                 200.0f,        kCHARA_WALK_SPEED*1.4f,      8,       eOBJECT_KIND_HEART,      40,      "-",                   },
	{"01/Teeger",        TEEGER_SCALE,        TEEGER_BODY_SIZE,      570.0f,        kCHARA_WALK_SPEED*2.0f,      68,      eOBJECT_KIND_HEART,      40,      "�Ă��`���`�����",    },
	{"01/Helicopter",    12.0f,               90.0f,                 200.0f,        kCHARA_WALK_SPEED*2.0f,      2,       eOBJECT_KIND_HEART,      0,       "��@���{",            },
	{NAME_MekaDragon,    12.0f,               90.0f,                 200.0f,        kCHARA_WALK_SPEED*3.0f,      56,      eOBJECT_KIND_HEART_CASE, 100,     "�A���j���[�g",        },
	{NAME_SA_14,         12.0f,               180.0f,                400.0f,        kCHARA_WALK_SPEED*1.0f,      20,      eOBJECT_KIND_MONEY_50,   40,      "SA-14",               },
	{"01/Teeger",        TEEGER_SCALE*0.35f,  TEEGER_BODY_SIZE*0.35f,570.0f*0.35f,  kCHARA_WALK_SPEED*1.0f,      8,       eOBJECT_KIND_MONEY_20,   40,      "�~�j�Ă��`���`",      },
	{NAME_MinoArmer,     12.0f,               220.0f,                300.0f,        kCHARA_WALK_SPEED*1.0f,      28,      eOBJECT_KIND_MONEY_100,  100,     "�~�m�^�E���X�E�L���O",},
	{NAME_Spider,        12.0f,               220.0f,                300.0f,        kCHARA_WALK_SPEED*1.0f,      28,      eOBJECT_KIND_MONEY_100,  100,     "�X�p�C�_�[",          },
	{NAME_HongKong,      12.0f,               90.0f,                 200.0f,        kCHARA_WALK_SPEED*1.5f,      95,      eOBJECT_KIND_HEART,      100,     "���`�f��P",           },
	{NAME_Loana,         12.0f,               90.0f,                 200.0f,        kCHARA_WALK_SPEED*1.5f,      66,      eOBJECT_KIND_HEART,      100,     "���A�i",              },
	{NAME_Oak,           12.0f,               90.0f,                 160.0f,        kCHARA_WALK_SPEED*1.0f,      8,       eOBJECT_KIND_MONEY_10,   40,      "�_�C�i�}�C�g�E�I�[�N",},
	{NAME_Usae,          12.0f,               80.0f,                 200.0f,        kCHARA_WALK_SPEED*1.5f,      56,      eOBJECT_KIND_HEART,      100,     "����������",          },
	{NAME_Usaida,        12.0f,               80.0f,                 200.0f,        kCHARA_WALK_SPEED*1.5f,      22,      eOBJECT_KIND_HEART,      100,     "�E�T�C�_�[�}�b",      },
	{NAME_Gimuri,        12.0f,               60.0f,                 150.0f,        kCHARA_WALK_SPEED*1.5f,      88,      eOBJECT_KIND_HEART,      100,     "�M����",              },
	{NAME_Excivy,        12.0f,               120.0f,                500.0f,        kCHARA_WALK_SPEED*2.0f,      102,     eOBJECT_KIND_HEART,      40,      "�G�N�V���B",          },

};

static const eEnemyKind sEnemyArrayENEMY_TBL_IDX_NONE[][kENEMY_ONCE_MAX] = {
	{ENEMY_NONE, ENEMY_NONE, ENEMY_NONE, ENEMY_NONE, ENEMY_NONE,}, 
};
static const eEnemyKind sEnemyArrayENEMY_TBL_IDX_000[][kENEMY_ONCE_MAX] = {
	{ENEMY_NONE, ENEMY_NONE, ENEMY_NONE, ENEMY_NONE, ENEMY_NONE,}, 
};
static const eEnemyKind sEnemyArrayENEMY_TBL_IDX_001[][kENEMY_ONCE_MAX] = {
	{ENEMY_NONE, ENEMY_NONE, ENEMY_NONE, ENEMY_NONE, ENEMY_NONE,}, 
};
static const eEnemyKind sEnemyArrayENEMY_TBL_IDX_002[][kENEMY_ONCE_MAX] = {
	{ENEMY_NONE, ENEMY_NONE, ENEMY_NONE, ENEMY_NONE, ENEMY_NONE,}, 
};
static const eEnemyKind sEnemyArrayENEMY_TBL_IDX_003[][kENEMY_ONCE_MAX] = {
	{ENEMY_NONE, ENEMY_NONE, ENEMY_NONE, ENEMY_NONE, ENEMY_NONE,}, 
};
static const eEnemyKind sEnemyArrayENEMY_TBL_IDX_004[][kENEMY_ONCE_MAX] = {
	{ENEMY_NONE, ENEMY_NONE, ENEMY_NONE, ENEMY_NONE, ENEMY_NONE,}, 
};
static const eEnemyKind sEnemyArrayENEMY_TBL_IDX_005[][kENEMY_ONCE_MAX] = {
	{ENEMY_NONE, ENEMY_NONE, ENEMY_NONE, ENEMY_NONE, ENEMY_NONE,}, 
};
static const eEnemyKind sEnemyArrayENEMY_TBL_IDX_010[][kENEMY_ONCE_MAX] = {
	// 0
	{eENEMY_KIND_GOBULIN,	eENEMY_KIND_FIGHTER,eENEMY_KIND_BAT,		ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_GOBULIN,	eENEMY_KIND_GOBULIN,ENEMY_NONE,				ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_GOBULIN,	eENEMY_KIND_GOBULIN,eENEMY_KIND_BAT,		ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_FIGHTER,	eENEMY_KIND_FIGHTER,eENEMY_KIND_FIGHTER,	ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_DRAGON,	eENEMY_KIND_DRAGON,	eENEMY_KIND_DRAGON,		ENEMY_NONE, ENEMY_NONE,},
	// 5
	{eENEMY_KIND_FIGHTER,	eENEMY_KIND_FIGHTER,eENEMY_KIND_FIGHTER,	ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_DRAGON,	eENEMY_KIND_DRAGON,	eENEMY_KIND_DRAGON,		ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_GOBULIN,	eENEMY_KIND_FIGHTER,eENEMY_KIND_BAT,		ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_GOBULIN,	eENEMY_KIND_GOBULIN,ENEMY_NONE,			ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_GOBULIN,	eENEMY_KIND_GOBULIN,eENEMY_KIND_BAT,		ENEMY_NONE, ENEMY_NONE,},
	// 10
	{eENEMY_KIND_GOBULIN,	eENEMY_KIND_GOBULIN,eENEMY_KIND_BAT,		ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_FIGHTER,	eENEMY_KIND_FIGHTER,eENEMY_KIND_FIGHTER,	ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_DRAGON,	eENEMY_KIND_DRAGON,	eENEMY_KIND_DRAGON,		ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_GOBULIN,	eENEMY_KIND_FIGHTER,eENEMY_KIND_BAT,		ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_GOBULIN,	eENEMY_KIND_GOBULIN,ENEMY_NONE,				ENEMY_NONE, ENEMY_NONE,},
	// 15
	{eENEMY_KIND_FIGHTER,	eENEMY_KIND_FIGHTER,eENEMY_KIND_FIGHTER,	ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_DRAGON,	eENEMY_KIND_DRAGON,	eENEMY_KIND_DRAGON,		ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_GOBULIN,	eENEMY_KIND_FIGHTER,eENEMY_KIND_BAT,		ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_GOBULIN,	eENEMY_KIND_GOBULIN,ENEMY_NONE,			ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_GOBULIN,	eENEMY_KIND_GOBULIN,eENEMY_KIND_BAT,		ENEMY_NONE, ENEMY_NONE,},
	// 20
	{eENEMY_KIND_GOBULIN,	eENEMY_KIND_GOBULIN,eENEMY_KIND_BAT,		ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_FIGHTER,	eENEMY_KIND_FIGHTER,eENEMY_KIND_FIGHTER,	ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_DRAGON,	eENEMY_KIND_DRAGON,	eENEMY_KIND_DRAGON,		ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_GOBULIN,	eENEMY_KIND_FIGHTER,eENEMY_KIND_BAT,		ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_GOBULIN,	eENEMY_KIND_GOBULIN,ENEMY_NONE,			ENEMY_NONE, ENEMY_NONE,},
	// 25
	{eENEMY_KIND_FIGHTER,	eENEMY_KIND_FIGHTER,eENEMY_KIND_FIGHTER,	ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_DRAGON,	eENEMY_KIND_DRAGON,	eENEMY_KIND_DRAGON,		ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_GOBULIN,	eENEMY_KIND_FIGHTER,eENEMY_KIND_BAT,		ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_GOBULIN,	eENEMY_KIND_GOBULIN,ENEMY_NONE,			ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_GOBULIN,	eENEMY_KIND_GOBULIN,eENEMY_KIND_BAT,		ENEMY_NONE, ENEMY_NONE,},
	// 30
	{eENEMY_KIND_GOBULIN,	eENEMY_KIND_GOBULIN,eENEMY_KIND_BAT,		ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_FIGHTER,	eENEMY_KIND_FIGHTER,eENEMY_KIND_FIGHTER,	ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_DRAGON,	eENEMY_KIND_DRAGON,	eENEMY_KIND_DRAGON,		ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_GOBULIN,	eENEMY_KIND_FIGHTER,eENEMY_KIND_BAT,		ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_GOBULIN,	eENEMY_KIND_GOBULIN,ENEMY_NONE,			ENEMY_NONE, ENEMY_NONE,},
	// 35
	{eENEMY_KIND_GOBULIN,	eENEMY_KIND_GOBULIN,eENEMY_KIND_BAT,		ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_DRAGON_B,	ENEMY_NONE, ENEMY_NONE,				ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_GOBULIN_B,	ENEMY_NONE, ENEMY_NONE,				ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_GOBULIN_B,	eENEMY_KIND_GOBULIN, eENEMY_KIND_GOBULIN,	ENEMY_NONE, ENEMY_NONE,},
};
static const eEnemyKind sEnemyArrayENEMY_TBL_IDX_KARUN_A[][kENEMY_ONCE_MAX] = {
	{ENEMY_NONE, ENEMY_NONE, ENEMY_NONE, ENEMY_NONE, ENEMY_NONE,}, 
};
static const eEnemyKind sEnemyArrayENEMY_TBL_IDX_KARUN_BOSS_A[][kENEMY_ONCE_MAX] = {
	{ENEMY_NONE, ENEMY_NONE, ENEMY_NONE, ENEMY_NONE, ENEMY_NONE,}, 
};

static const int sEnemyArrayENEMY_TBL_IDX_016[][kENEMY_ONCE_MAX] = {
	// 0
	{eENEMY_KIND_GOBULIN,		-1,			-1,		-1, -1,},
	{eENEMY_KIND_WOODMAN,		-1,			-1,		-1, -1,},
	{eENEMY_KIND_MINOTAURUS,	-1,			-1,		-1, -1,},
	{eENEMY_KIND_SLIME,			eENEMY_KIND_SLIME,		-1,		-1, -1,},
	{eENEMY_KIND_GOBULIN,		-1,			-1,		-1, -1,},
	// 5
	{eENEMY_KIND_SLIME,			-1,	-1,	-1, -1,},
	{eENEMY_KIND_WOODMAN,		eENEMY_KIND_WOODMAN, -1, -1, -1,},
	{eENEMY_KIND_BAT,			eENEMY_KIND_BAT,		eENEMY_KIND_FIGHTER, -1, -1,},
	{eENEMY_KIND_WOODMAN,		eENEMY_KIND_WOODMAN, eENEMY_KIND_WOODMAN, eENEMY_KIND_WOODMAN, -1,},
	{eENEMY_KIND_GOBULIN,		eENEMY_KIND_WOODMAN, -1, -1, -1,},
	// 10
	{eENEMY_KIND_GOBULIN,		-1,	-1,	-1, -1,},
	{eENEMY_KIND_SLIME,			eENEMY_KIND_WOODMAN, -1, -1, -1,},
	{eENEMY_KIND_FIGHTER,		eENEMY_KIND_BAT, -1, -1, -1,},
	{eENEMY_KIND_GOBULIN,		eENEMY_KIND_SLIME, -1, -1, -1,},
};

static const eEnemyKind sEnemyArrayENEMY_TBL_IDX_024[][kENEMY_ONCE_MAX] = {
	{eENEMY_KIND_FIGHTER,	eENEMY_KIND_FIGHTER,ENEMY_NONE,			ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_FIGHTER,	eENEMY_KIND_BAT,	eENEMY_KIND_BAT,		ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_GOBULIN,	eENEMY_KIND_FIGHTER,eENEMY_KIND_BAT,		ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_GOBULIN,	ENEMY_NONE,		ENEMY_NONE,			ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_GOBULIN,	eENEMY_KIND_GOBULIN,eENEMY_KIND_BAT,		ENEMY_NONE, ENEMY_NONE,},
};
static const eEnemyKind sEnemyArrayENEMY_TBL_IDX_ARENA[][kENEMY_ONCE_MAX] = {
	{eENEMY_KIND_KNIGHT, eENEMY_KIND_GOBULIN, eENEMY_KIND_MALON, eENEMY_KIND_HATOPHON_Z, eENEMY_KIND_DRAGON, },
	{eENEMY_KIND_FRESBELK, ENEMY_NONE, ENEMY_NONE, ENEMY_NONE, ENEMY_NONE},
};

static const eEnemyKind sEnemyArrayENEMY_TBL_IDX_026[][kENEMY_ONCE_MAX] = {
	{eENEMY_KIND_HATOPHON,	ENEMY_NONE,		ENEMY_NONE,			ENEMY_NONE, ENEMY_NONE,}, 
};
static const eEnemyKind sEnemyArrayENEMY_TBL_IDX_028[][kENEMY_ONCE_MAX] = {
	{eENEMY_KIND_RED_DRAGON, ENEMY_NONE, ENEMY_NONE, ENEMY_NONE, ENEMY_NONE,}, 
};

// ���x���j�A��
static const int sEnemyArrayENEMY_TBL_IDX_029[][kENEMY_ONCE_MAX] = {
	// 0
	{eENEMY_KIND_KNIGHT,	-1,	-1,		-1, -1,},
	{eENEMY_KIND_MAGICIAN,	eENEMY_KIND_MAGICIAN,	-1,		-1, -1,},
	{eENEMY_KIND_KNIGHT,	eENEMY_KIND_KNIGHT,		-1,		-1, -1,},
	{eENEMY_KIND_ARMER,		eENEMY_KIND_ARMER,		eENEMY_KIND_ARMER,	eENEMY_KIND_ARMER, -1,},
	{eENEMY_KIND_MAGICIAN,	eENEMY_KIND_MAGICIAN,	eENEMY_KIND_KNIGHT, eENEMY_KIND_KNIGHT, -1,},
	// 5
	{eENEMY_KIND_OAK,		-1, -1, -1, -1,},
	{eENEMY_KIND_ARMER,		eENEMY_KIND_ARMER,		eENEMY_KIND_ARMER,	-1, -1,},
	{eENEMY_KIND_KNIGHT,	eENEMY_KIND_OAK,		-1, -1, -1,},
	{eENEMY_KIND_KNIGHT,	eENEMY_KIND_KNIGHT,		-1,-1, -1,},
	{eENEMY_KIND_WIZARD,	-1,	-1,	-1, -1,},
	// 10
	{eENEMY_KIND_ARMER,		eENEMY_KIND_ARMER,		eENEMY_KIND_KNIGHT, -1, -1,},
};

static const eEnemyKind sEnemyArrayENEMY_TBL_IDX_036[][kENEMY_ONCE_MAX] = {
	{eENEMY_KIND_MINOTAURUS, eENEMY_KIND_MINOTAURUS, ENEMY_NONE,		ENEMY_NONE,			ENEMY_NONE,}, 
	{eENEMY_KIND_WOODMAN,	eENEMY_KIND_SLIME,		eENEMY_KIND_SLIME,	ENEMY_NONE,			ENEMY_NONE,}, 
	{eENEMY_KIND_BAT,		eENEMY_KIND_BAT,		eENEMY_KIND_GOBULIN,	ENEMY_NONE,			ENEMY_NONE,}, 
	{eENEMY_KIND_KNIGHT,	eENEMY_KIND_MAGICIAN,	eENEMY_KIND_MAGICIAN,	ENEMY_NONE,			ENEMY_NONE,}, 
};

static const eEnemyKind sEnemyArrayENEMY_TBL_IDX_039[][kENEMY_ONCE_MAX] = {
	{eENEMY_KIND_MINOTAURUS, eENEMY_KIND_GOBULIN,	ENEMY_NONE,		ENEMY_NONE,			ENEMY_NONE,}, 
	{eENEMY_KIND_WOODMAN,	eENEMY_KIND_WOODMAN,	eENEMY_KIND_WOODMAN,	ENEMY_NONE,			ENEMY_NONE,}, 
	{eENEMY_KIND_MAGICIAN,	eENEMY_KIND_MAGICIAN,	eENEMY_KIND_SLIME,	ENEMY_NONE,			ENEMY_NONE,}, 
};

static const eEnemyKind sEnemyArrayENEMY_TBL_IDX_040[][kENEMY_ONCE_MAX] = {
	{eENEMY_KIND_TEEGER,	ENEMY_NONE,				ENEMY_NONE,			ENEMY_NONE,			ENEMY_NONE,}, 
	{eENEMY_KIND_ROBOT_B,	ENEMY_NONE,				ENEMY_NONE,			ENEMY_NONE,			ENEMY_NONE,}, 
	{eENEMY_KIND_ROBOT_A,	ENEMY_NONE,				ENEMY_NONE,			ENEMY_NONE,			ENEMY_NONE,}, 
	{eENEMY_KIND_SCORPION,	ENEMY_NONE,				ENEMY_NONE,			ENEMY_NONE,			ENEMY_NONE,}, 
	// ���̏�
	{eENEMY_KIND_SA_14,		ENEMY_NONE,				ENEMY_NONE,			ENEMY_NONE,			ENEMY_NONE,}, 
	{eENEMY_KIND_ROBOT_A,	eENEMY_KIND_ROBOT_A,	ENEMY_NONE,			ENEMY_NONE,			ENEMY_NONE,}, 
	{eENEMY_KIND_HELICOPTER,eENEMY_KIND_ROBOT_A,	eENEMY_KIND_HELICOPTER, 	ENEMY_NONE,			ENEMY_NONE,}, 
	{eENEMY_KIND_HELICOPTER,eENEMY_KIND_HELICOPTER,	ENEMY_NONE,			ENEMY_NONE,			ENEMY_NONE,}, 
};

static const eEnemyKind sEnemyArrayENEMY_TBL_IDX_041[][kENEMY_ONCE_MAX] = {
	{eENEMY_KIND_HELICOPTER,	eENEMY_KIND_HELICOPTER,	eENEMY_KIND_HELICOPTER,	eENEMY_KIND_HELICOPTER,			ENEMY_NONE,}, 
	{eENEMY_KIND_SA_14,			eENEMY_KIND_ROBOT_A,	eENEMY_KIND_ROBOT_A,	ENEMY_NONE,			ENEMY_NONE,}, 
	{eENEMY_KIND_TEEGER_M,	eENEMY_KIND_TEEGER_M,	ENEMY_NONE,				ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_SA_14,		eENEMY_KIND_TEEGER_M,		eENEMY_KIND_ROBOT_B,				ENEMY_NONE,			ENEMY_NONE,}, 
	{eENEMY_KIND_SA_14,		eENEMY_KIND_SA_14,			eENEMY_KIND_SA_14,		eENEMY_KIND_SA_14,	eENEMY_KIND_SA_14,},
	

	{eENEMY_KIND_ROBOT_A,	eENEMY_KIND_ROBOT_B,	eENEMY_KIND_ROBOT_B,	ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_SA_14,		eENEMY_KIND_SA_14,		ENEMY_NONE,				ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_TEEGER_M,	eENEMY_KIND_TEEGER_M,	ENEMY_NONE,				ENEMY_NONE, ENEMY_NONE,},
};

static const eEnemyKind sEnemyArrayENEMY_TBL_IDX_042[][kENEMY_ONCE_MAX] = {
	{eENEMY_KIND_ROBOT_A,	eENEMY_KIND_ROBOT_B,	eENEMY_KIND_ROBOT_B,	ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_SA_14,		eENEMY_KIND_SA_14,		ENEMY_NONE,				ENEMY_NONE, ENEMY_NONE,},
	{eENEMY_KIND_TEEGER_M,	eENEMY_KIND_TEEGER_M,	ENEMY_NONE,				ENEMY_NONE, ENEMY_NONE,},
};

static const eEnemyKind sEnemyArrayENEMY_TBL_IDX_046[][kENEMY_ONCE_MAX] = {
	{eENEMY_KIND_TEEGER_M,	eENEMY_KIND_TEEGER_M,	ENEMY_NONE,				ENEMY_NONE, ENEMY_NONE,},
};

static const int sEnemyArrayENEMY_TBL_IDX_TEST[][kENEMY_ONCE_MAX] = {
	{eENEMY_KIND_FRESBELK, -1,-1,-1,-1,},
	{eENEMY_KIND_USAEMON, eENEMY_KIND_USIDERMAN,-1,-1,-1,},
	{eENEMY_KIND_EXCIVY, -1,-1,-1,-1,},
	{eENEMY_KIND_HONGKONG, -1,-1,-1,-1,},
	{eENEMY_KIND_HONGKONG, -1,-1,-1,-1,},
	{eENEMY_KIND_LOANA, -1,-1,-1,-1,},
	{eENEMY_KIND_FRESBELK, -1,-1,-1,-1,},
	{eENEMY_KIND_GIMURI, -1,-1,-1,-1,},
	{eENEMY_KIND_USAEMON, eENEMY_KIND_USIDERMAN,-1,-1,-1,},
	{-1, -1,-1,-1,-1,},
	{-1, -1,-1,-1,-1,},
};

static const tEnemyTableInfo sEnemyArrayTable[eENE_TBL_IDX_MAX] = {
	{(eEnemyKind *)sEnemyArrayENEMY_TBL_IDX_NONE,				ENEMY_ARRAY_NUM(sEnemyArrayENEMY_TBL_IDX_NONE),},			// 	eENE_TBL_IDX_NONE
	{(eEnemyKind *)sEnemyArrayENEMY_TBL_IDX_000,				ENEMY_ARRAY_NUM(sEnemyArrayENEMY_TBL_IDX_000),},			// 	eENE_TBL_IDX_000
	{(eEnemyKind *)sEnemyArrayENEMY_TBL_IDX_001,				ENEMY_ARRAY_NUM(sEnemyArrayENEMY_TBL_IDX_001),},			// 	eENE_TBL_IDX_001
	{(eEnemyKind *)sEnemyArrayENEMY_TBL_IDX_002,				ENEMY_ARRAY_NUM(sEnemyArrayENEMY_TBL_IDX_002),},			// 	eENE_TBL_IDX_002
	{(eEnemyKind *)sEnemyArrayENEMY_TBL_IDX_003,				ENEMY_ARRAY_NUM(sEnemyArrayENEMY_TBL_IDX_003),},			// 	eENE_TBL_IDX_003
	{(eEnemyKind *)sEnemyArrayENEMY_TBL_IDX_004,				ENEMY_ARRAY_NUM(sEnemyArrayENEMY_TBL_IDX_004),},			// 	eENE_TBL_IDX_004
	{(eEnemyKind *)sEnemyArrayENEMY_TBL_IDX_005,				ENEMY_ARRAY_NUM(sEnemyArrayENEMY_TBL_IDX_005),},			// 	eENE_TBL_IDX_005
	{(eEnemyKind *)sEnemyArrayENEMY_TBL_IDX_010,				ENEMY_ARRAY_NUM(sEnemyArrayENEMY_TBL_IDX_010),},			// 	eENE_TBL_IDX_010
	{(eEnemyKind *)sEnemyArrayENEMY_TBL_IDX_KARUN_A,			ENEMY_ARRAY_NUM(sEnemyArrayENEMY_TBL_IDX_KARUN_A),},		// 	eENE_TBL_KARUN_A
	{(eEnemyKind *)sEnemyArrayENEMY_TBL_IDX_KARUN_BOSS_A,		ENEMY_ARRAY_NUM(sEnemyArrayENEMY_TBL_IDX_KARUN_BOSS_A),},	// 	eENE_TBL_KARUN_BOSS_A
	{(eEnemyKind *)sEnemyArrayENEMY_TBL_IDX_016,				ENEMY_ARRAY_NUM(sEnemyArrayENEMY_TBL_IDX_016),},			// 	eENE_TBL_IDX_016
	{(eEnemyKind *)sEnemyArrayENEMY_TBL_IDX_024,				ENEMY_ARRAY_NUM(sEnemyArrayENEMY_TBL_IDX_024),},			// 	eENE_TBL_IDX_024
	{(eEnemyKind *)sEnemyArrayENEMY_TBL_IDX_026,				ENEMY_ARRAY_NUM(sEnemyArrayENEMY_TBL_IDX_026),},			// 	eENE_TBL_IDX_026
	{(eEnemyKind *)sEnemyArrayENEMY_TBL_IDX_ARENA,				ENEMY_ARRAY_NUM(sEnemyArrayENEMY_TBL_IDX_ARENA),},			// 	eENE_TBL_IDX_ARENA
	{(eEnemyKind *)sEnemyArrayENEMY_TBL_IDX_028,				ENEMY_ARRAY_NUM(sEnemyArrayENEMY_TBL_IDX_028),},			// 	eENE_TBL_IDX_028
	{(eEnemyKind *)sEnemyArrayENEMY_TBL_IDX_029,				ENEMY_ARRAY_NUM(sEnemyArrayENEMY_TBL_IDX_029),},			// 	eENE_TBL_IDX_029
	{(eEnemyKind *)sEnemyArrayENEMY_TBL_IDX_036,				ENEMY_ARRAY_NUM(sEnemyArrayENEMY_TBL_IDX_036),},			// 	eENE_TBL_IDX_036
	{(eEnemyKind *)sEnemyArrayENEMY_TBL_IDX_039,				ENEMY_ARRAY_NUM(sEnemyArrayENEMY_TBL_IDX_039),},			// 	eENE_TBL_IDX_039
	{(eEnemyKind *)sEnemyArrayENEMY_TBL_IDX_040,				ENEMY_ARRAY_NUM(sEnemyArrayENEMY_TBL_IDX_040),},			// 	eENE_TBL_IDX_040
	{(eEnemyKind *)sEnemyArrayENEMY_TBL_IDX_041,				ENEMY_ARRAY_NUM(sEnemyArrayENEMY_TBL_IDX_041),},			// 	eENE_TBL_IDX_041
	{(eEnemyKind *)sEnemyArrayENEMY_TBL_IDX_042,				ENEMY_ARRAY_NUM(sEnemyArrayENEMY_TBL_IDX_042),},			// 	eENE_TBL_IDX_042
	{(eEnemyKind *)sEnemyArrayENEMY_TBL_IDX_046,				ENEMY_ARRAY_NUM(sEnemyArrayENEMY_TBL_IDX_046),},			// 	eENE_TBL_IDX_046
	{(eEnemyKind *)sEnemyArrayENEMY_TBL_IDX_TEST,				ENEMY_ARRAY_NUM(sEnemyArrayENEMY_TBL_IDX_TEST),},			// 	eENE_TBLIDX_TEST
};

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
const tEnemyWork* GetEnemyWorkTable(int enemyKind)
{
	return &sEnemyWorkTbl[enemyKind]; 
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
const tEnemyTableInfo* GetEnemyTableInfo(int tableIndex)
{
	return &sEnemyArrayTable[tableIndex];
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void LoadEnemyAttachmentModel(eEnemyKind kind)
{
	ResourceModel* p_res_mdl = ResourceModel::GetInstance();

	switch (kind) {
	case eENEMY_KIND_KNIGHT:
		p_res_mdl->LoadItemResource(RES_MDL_SWORD);
		break;

	case eENEMY_KIND_MINOTAURUS:
		p_res_mdl->LoadItemResource(RES_MDL_AX);
		break;

	case eENEMY_KIND_MAGICIAN:
		p_res_mdl->LoadItemResource(RES_MDL_STAFF);
		break;

	case eENEMY_KIND_OAK:
		p_res_mdl->LoadItemResource(RES_MDL_SHIELD);
		break;

	default:
		break;
	};
}

 /**** end of file ****/
