/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * ItemDef.cpp
 * @author SPATZ.
 * @data   2014/05/17 15:23:46
 */
//---------------------------------------------------
#include "Windows.h"
#include "ItemDef.h"
#include "AppLib/Basic/Basic.h"

/*=====================================*
 * �A�C�e�����e�[�u�� (csv�t�@�C������ړ�)
 *=====================================*/
static const tItemDetailInfo sItemDetailInfoTable[eITEM_KIND_MAX] = {
	// �t�@�C����				:���C�A�E�g��					:�\����							:�l�i	:����1												: ����2											:����3
	{RES_MDL_MASTERSWORD,		RES_LYT_ITEM_RING_MASTERSWORD,	"�}�X�c�@�[�\�[�h",				0,		"�o�g���G���A�Łu�ʏ�U���{�^���v�ōU�����邼",		"�`���̌����܂����Ƃ̒���",						"����Ȃ�ă��b�L�[�I��",					},
	{RES_MDL_LONGHOOK,			RES_LYT_ITEM_RING_LONGHOOK,		"�t�b�N�V���[�g",				400,	"������؂�ւ��āu�A�C�e���g�p�{�^���v�Ŏg�p",		"�����ȕǂɂЂ�������",						"�����̓G�ɍU���ł��邼�I",					},
	{RES_MDL_MAP,				RES_LYT_ITEM_RING_NONE,			"�}�b�v",						100,	"�u�}�b�v�\���{�^���v�Œn�}������邼�I",			"������������ꏊ��",							"�ړ��\�ȏꏊ�������邼",					},
	{RES_MDL_MONEY_5,			RES_LYT_ITEM_RING_NONE,			"5�m���s�[",					5,		"���̍��̂�����",									"�A�C�e���𔃂�����",							"F4�L�[�g�p���ɕK�v�ɂȂ邼",   			},
	{RES_MDL_HEART,				RES_LYT_HEART_FULL,				"�n�[�g",  						10,		"HP�����̏��1�񕜂����邼�I",						"",												"",	                            			},
	{RES_MDL_PIKOHUM,			RES_LYT_ITEM_RING_PIKO_HUM,		"�s�R�n��",						20,		"������؂�ւ��āu�A�C�e���g�p�{�^���v�Ŏg�p",		"����ɐ▭�Ȃ����݂�",						"�����邼��",                 			},
	{"",						RES_LYT_ITEM_RING_NONE,			"�Ȃ�",							0,		"������؂�ւ��āu�A�C�e���g�p�{�^���v�Ŏg�p",		"�����Ȃ�",										"",                             			},
	{RES_MDL_BOOMERANG,			RES_LYT_ITEM_RING_BOOMERANG,	"�u�[�������O",					30,		"������؂�ւ��āu�A�C�e���g�p�{�^���v�Ŏg�p",		"�������̓G���U��������",						"�X�C�b�`��ς�����ł��邼",   			},
	{RES_MDL_HEART_CASE,		RES_LYT_ITEM_RING_NONE,			"�n�[�g�̌���",					0,		"�ő�HP���P�������邼�I",							"",												"",	                            			},
	{RES_MDL_BOSSKEY,			RES_LYT_ITEM_RING_NONE,			"�傫�Ȍ�",						0,		"�傫�ȕ󔠂�{�X�ɑ�������",						"�����J�����Ƃ��ł��邼�I",						"",	                            			},
	{RES_MDL_PLATE_COMMON,		RES_LYT_ITEM_RING_PORTION,		"�|�[�c����",					30,		"�I����ʂőI�����Ďg�p",							"HP���R�񕜂��邼�I",							"��x�g�p����ƂȂ��Ȃ��",     			},
	{RES_MDL_MONEY_10,			RES_LYT_ITEM_RING_NONE,			"10�m���s�[",					10,		"���̍��̂�����",									"�A�C�e���𔃂�����",							"F4�L�[�g�p���ɕK�v�ɂȂ邼",   			},
	{RES_MDL_MONEY_50,			RES_LYT_ITEM_RING_NONE,			"50�m���s�[",					50,		"���̍��̂�����",									"�A�C�e���𔃂�����",							"F4�L�[�g�p���ɕK�v�ɂȂ邼",   			},
	{RES_MDL_MONEY_100,			RES_LYT_ITEM_RING_NONE,			"100�m���s�[",					100,	"���̍��̂�����",									"�A�C�e���𔃂�����",							"F4�L�[�g�p���ɕK�v�ɂȂ邼",				},
	{RES_MDL_MONEY_1,/*��*/		RES_LYT_ITEM_RING_NONE,			"200�m���s�[",					200,	"���̍��̂�����",									"�A�C�e���𔃂�����",							"F4�L�[�g�p���ɕK�v�ɂȂ邼",				},
	{RES_MDL_MONEY_1,/*��*/		RES_LYT_ITEM_RING_NONE,			"400�m���s�[",					400,	"���̍��̂�����",									"�A�C�e���𔃂�����",							"F4�L�[�g�p���ɕK�v�ɂȂ邼",				},
	{RES_MDL_PLATE_COMMON,		RES_LYT_ICON_MAP_LINO_FACE,		"���m�̏Ί炻�̂P",				999,	"���x���j�A�X���ɉB����Ă���U�̂����̂P��",	"�u�O�L�[�v�������ƕ\��ς�邼�I",			"",											},
	{RES_MDL_PLATE_COMMON,		RES_LYT_ICON_MAP_LINO_FACE,		"���m�̏Ί炻�̂Q",				999,	"���x���j�A�X���ɉB����Ă���U�̂����̂P��",	"�u�P�L�[�v�������ƕ\��ς�邼�I",			"",											},
	{RES_MDL_PLATE_COMMON,		RES_LYT_ICON_MAP_LINO_FACE,		"���m�̏Ί炻�̂R",				999,	"���x���j�A�X���ɉB����Ă���U�̂����̂P��",	"�u�Q�L�[�v�������ƕ\��ς�邼�I",			"",											},
	{RES_MDL_PLATE_COMMON,		RES_LYT_ICON_MAP_LINO_FACE,		"���m�̏Ί炻�̂S",				999,	"���x���j�A�X���ɉB����Ă���U�̂����̂P��",	"�u�R�L�[�v�������ƕ\��ς�邼�I",			"",											},
	{RES_MDL_PLATE_COMMON,		RES_LYT_ICON_MAP_LINO_FACE,		"���m�̏Ί炻�̂T",				999,	"���x���j�A�X���ɉB����Ă���U�̂����̂P��",	"�u�S�L�[�v�������ƕ\��ς�邼�I",			"",											},
	{RES_MDL_PLATE_COMMON,		RES_LYT_ICON_MAP_LINO_FACE,		"���m�̏Ί炻�̂U",				999,	"���x���j�A�X���ɉB����Ă���U�̂����̂P��",	"�u�T�L�[�v�������ƕ\��ς�邼�I",			"",											},
	{RES_MDL_PLATE_COMMON,		RES_LYT_ICON_MAP_LINO_FACE,		"���m�̗x��"    ,				999,	"�u�U�L�[�v�������ƃ��m���x�肾�����I",				"",                         					"",                             			},
	{RES_MDL_LIB_SHIELD,		RES_LYT_ITEM_RING_SHIELD,		"���x���j�A�̏�",				30,		"����{�^���������Ƃŏ����\���邼�I",	            "�ʏ�U�����K�[�h������A",						"�J�~�i������𒵂˕Ԃ����I",				},
	{RES_MDL_CARD_COMMON,		RES_LYT_CARD_K,					"�g�����v�EK",					30,		"���x���j�A�g�����v��KING�̃J�[�h��",				"",												"",							    			},
	{RES_MDL_CARD_COMMON,		RES_LYT_CARD_Q,					"�g�����v�EQ",					30,		"���x���j�A�g�����v��QUEEN�̃J�[�h��",				"",												"",											},
	{RES_MDL_CARD_COMMON,		RES_LYT_CARD_J,					"�g�����v�EJ",					30,		"���x���j�A�g�����v��JACK�̃J�[�h��",				"",												"",							    			},
	{RES_MDL_CRYSTAL,			RES_LYT_ITEM_RING_NONE,			"�y�̃N���X�^��",				0,		"�}���C�t�H�[�X�̗͂𕕈󂷂�",						"�d�v�ȗ͂����N���X�^����",					"�c��̃N���X�^���͂��ƂQ�I",				},
	// ��bit����32�𒴂��Ă��̑�֎�i�Ƃ��ăM�U�[���̖�؂��R�����g�A�E�g�I
	{RES_MDL_BOMB,				RES_LYT_ITEM_RING_BOMB,			"�o�N�_�\",						40,		"������؂�ւ��āu�A�C�e���g�p�{�^���v�Ŏg�p",		"�߂��̓G�ɑ�_���[�W��^������",				"����󂵂���ł��邼(�ő�10���Ă�)",		},
	{RES_MDL_CRYSTAL,			RES_LYT_ITEM_RING_NONE,			"�X�̃N���X�^��",				0,		"�}���C�t�H�[�X�̗͂𕕈󂷂�",						"�d�v�ȗ͂����N���X�^����",					"�c��̃N���X�^���͂��ƂR�I",				},
	{RES_MDL_ARCHERY,			RES_LYT_ITEM_RING_ARCHERY,		"������݁���",					0,		"������؂�ւ��āu�A�C�e���g�p�{�^���v�Ŏg�p",		"�����̓G��I��_�������ł��邼",	    		"�X�N���[���ŋ����̒�����(�ő�50�{���Ă�)",	},
	{RES_MDL_ANGEL_WING,		RES_LYT_ITEM_RING_WING,			"�y�K�T�X�̊�",					0,		"�󒆂ɂ���ԂɃW�����v�{�^���Ŏg�p�\",           "������y�K�T�X�̗����͂��Ăӂ��蒵�ׂ�I", 	"���m����}�W�V�g�I",						},
	{RES_MDL_PLATE_COMMON,		RES_LYT_COSPLAY_PRONAMA,		"�v���������̃R�X�v��",		400,	"�R�X�v�����j���[����R�X�v���ł��邼�I",           "",                                     		"",											},
	// 32bit�ȏ�
	{RES_MDL_MONEY_1,			RES_LYT_ITEM_RING_NONE,			"1�m���s�[",					1,		"���̍��̂�����",									"�A�C�e���𔃂�����",							"F4�L�[�g�p���ɕK�v�ɂȂ邼",   			},
	{RES_MDL_MONEY_20,			RES_LYT_ITEM_RING_NONE,			"20�m���s�[",					20,		"���̍��̂�����",									"�A�C�e���𔃂�����",							"F4�L�[�g�p���ɕK�v�ɂȂ邼",   			},
	{RES_MDL_PLATE_COMMON,		RES_LYT_PLATE_ARROW,			"�|��̖�",						10,		"�|��̖�P�O�{��",									"",					                    		"",   										},
	{RES_MDL_PLATE_COMMON,		RES_LYT_PLATE_BOMB,				"���e�̒e",						20,		"���e�̒e�R��",									"",					                    		"",   										},
	{RES_MDL_BIN,				RES_LYT_ITEM_RING_BIN_EMPTY,	"�󂫃r��",						100,	"�󂫃r����",									    "�r���̒��ɖ����ꂽ��",               		"�d����߂܂�����ł��邼",         		},
	{RES_MDL_BIN,				RES_LYT_ITEM_RING_BIN_MAGENTA,	"�}�[���^�̖�",					120,	"�}�[���^�F�̖�",									"HP���P�O�񕜂��邼�I",                   		"(�w������ɂ͋󂫃r�����K�v)",				},
	{RES_MDL_BIN,				RES_LYT_ITEM_RING_BIN_CYAN,		"�V�A���̖�",					180,	"�V�A���F�̖�",									"MP���S�񕜁A�s���������邼�I",            		"(�w������ɂ͋󂫃r�����K�v)",				},
	{RES_MDL_BIN,				RES_LYT_ITEM_RING_BIN_YELLOW,	"�C�G���[�̖�",					300,	"�C�G���[�F�̖�",									"HP�AMP�Ƃ��ɑS�񕜂��邼�I",              		"(�w������ɂ͋󂫃r�����K�v)",				},
	{RES_MDL_BIN,				RES_LYT_ITEM_RING_BIN_FIARY,	"�d���e�B���e",					1,		"�d���̃e�B���e��",									"HP���S�񕜂��邼�I",                   		"(�w������ɂ͋󂫃r�����K�v)",				},
	{RES_MDL_PLATE_COMMON,		RES_LYT_COSPLAY_DABAH,			"���΂����m�����̃R�X�v��",	0,		"�R�X�v�����j���[����R�X�v���ł��邼�I",           "",                                     		"",											},
	{RES_MDL_PLATE_COMMON,		RES_LYT_RING_WALLET_A,			"���킢���T�C�t",				0,		"������600�m���s�[�܂œ���鎖���ł��邼�I",       "�J�G���̂��܂����ŃJ���C�C�I",   				"",											},
	{RES_MDL_PLATE_COMMON,		RES_LYT_RING_WALLET_B,			"���������T�C�t",				320,	"������1100�m���s�[�܂œ���鎖���ł��邼�I",       "�r�o�`�s�y�u�����h�̃_�T���T�C�t��", 			"�_�T���E�E�E�_�T�����邼�I",				},
	{RES_MDL_PLATE_COMMON,		RES_LYT_RING_WALLET_C,			"���Ԃ��T�C�t",					680,	"������9999�m���s�[�܂œ���鎖���ł��邼�I",       "���x���j�A�̋M�����g���a���T�C�t��",   		"�a���˂��A�I�^�N�S���a�����I",				},
	{RES_MDL_PLATE_COMMON,		RES_LYT_RING_SEAL_KEY,			"�����̌�",					0,		"���m���c�������炸���Ǝ�ɂ����Ă��錮��",         "���̌����̓��m���g���m��Ȃ�����",				"�����̗l�ɐ̂���厖�Ɏ����Ă���",		},
	{RES_MDL_PLATE_COMMON,		RES_LYT_COSPLAY_TDA_MIKU,		"�~�N�̃R�X�v��",				0,		"�R�X�v�����j���[����R�X�v���ł��邼�I",           "",                                     		"",											},
	{RES_MDL_PLATE_COMMON,		RES_LYT_COSPLAY_LAPAN,			"��ς�̃R�X�v��",				0,		"�R�X�v�����j���[����R�X�v���ł��邼�I",           "",                                     		"",											},
	{RES_MDL_PLATE_COMMON,		RES_LYT_COSPLAY_FRES,			"�t���X�x���N�̃R�X�v��",		0,		"�R�X�v�����j���[����R�X�v���ł��邼�I",           "",                                     		"",											},
	{RES_MDL_PLATE_COMMON,		RES_LYT_COSPLAY_MASK,			"�}�X�N�E�U�E�t���̃R�X�v��",	0,		"�R�X�v�����j���[����R�X�v���ł��邼�I",           "",                                     		"",											},
	{RES_MDL_PLATE_COMMON,		RES_LYT_RING_MAGIC_THUNDER,		"���C�g�j���O�̖��@",			0,		"���j���[����I������Ǝg�p�\",                   "���m�̑O���ɋ��͂ȃC�J�d�`������I", 		"MP���傫������̂����ӂ��I",				},
	{RES_MDL_PLATE_COMMON,		RES_LYT_RING_MAGIC_FLAME,		"�t���C���̖��@",				0,		"���j���[����I������Ǝg�p�\",                   "���m�̎��ӂɎܔM�̉�������I",       		"MP������̂Ɖr���Ɏ��Ԃ������邼�I",		},
	{RES_MDL_PLATE_COMMON,		RES_LYT_RING_MAGIC_TORNADE,		"�g���l�[�h�̖��@",				0,		"���j���[����I������Ǝg�p�\",                   "���m�̎��ӂɉs�������𔭐������邼�I", 		"MP��������x���邩�璍�ӂ��K�v���I",		},
	{RES_MDL_OCARINA,			RES_LYT_RING_OCARINA,			"�v�Ђł̃I�J�\�i",				0,		"������؂�ւ��āu�A�C�e���g�p�{�^���v�Ŏg�p",     "�I�J���i�ŉ��F��t�ł�",               		"�l�X�Ȍ��ʂ������炷���I",					},
	{RES_MDL_COPY_ROD,			RES_LYT_RING_COPY_ROD,			"�C�J�d�`�̃��b�h",				666,	"������؂�ւ��āu�A�C�e���g�p�{�^���v�Ŏg�p",     "���b�h����C�J�d�`������I",         		"MP������̂Ŏg�������ɒ��ӂ��I",			},
	{RES_MDL_PLATE_COMMON,		RES_LYT_COSPLAY_HATOPHONE,		"�n�g�t�H���̃R�X�v��",			200,	"�R�X�v�����j���[����R�X�v���ł��邼�I",           "",                                     		"",											},
	{RES_MDL_PLATE_COMMON,		RES_LYT_COSPLAY_BAT,			"��������R�X�v��",				100,	"�R�X�v�����j���[����R�X�v���ł��邼�I",           "",                                     		"",											},
	{RES_MDL_PLATE_COMMON,		RES_LYT_COSPLAY_CHILNO,			"�`���m�̃R�X�v��",				400,	"�R�X�v�����j���[���炠�����̃R�X�v���ł��邼�I",   "",                                     		"",											},
	{RES_MDL_PLATE_COMMON,		RES_LYT_COSPLAY_HONG_KONG,		"���`�f��P�̃R�X�v��",			0,		"�R�X�v�����j���[����R�X�v���ł��邼�I",           "",                                     		"",											},
	{RES_MDL_PLATE_COMMON,		RES_LYT_COSPLAY_LOANA,			"���A�i�̃R�X�v��",				0,		"�R�X�v�����j���[����R�X�v���ł��邼�I",           "",                                     		"",											},
	{RES_MDL_PLATE_COMMON,		RES_LYT_COSPLAY_YUKARI,			"�����䂩��̃R�X�v��",			400,	"�R�X�v�����j���[����R�X�v���ł��邼�I",           "",                                     		"",											},
	{RES_MDL_PLATE_COMMON,		RES_LYT_COSPLAY_NAKANO,			"��{�J�m�̃R�X�v��",			400,	"�R�X�v�����j���[����R�X�v���ł��邼�I",           "",                                     		"",											},
	{RES_MDL_HEART_PIECE,		RES_LYT_ITEM_RING_NONE,			"�n�[�g�̃p�[�c",				0,		"�S�W�߂�ƍő�HP���P�������邼�I",				"",												"",	                            			},
	{RES_MDL_PLATE_COMMON,		RES_LYT_RING_TROPHY,			"���x���j�A�ꕐ����̃g���t�B",	0,		"���x���j�A�ꕐ����̑S�����𐧂����҂�����",     "�����Ƃ��������鉩���̃g���t�B���I",   		"���O���i���o�[�P��!!",						},
	{RES_MDL_PLATE_COMMON,		RES_LYT_RING_NAVI_LIGHT,		"�i�r�H���C�g",					0,		"�i�r�H�̓��ӋZ���I",       						"�Èłɓ���ƃi�r�H����������ӂ���Ƃ炷���I", "",											},
	{RES_MDL_BOOK_CLOSE,		RES_LYT_RING_BOOK,				"�قނ�̏�",					0,		"�Ñ�̖����g����",         						"�u�قނقތ���v���A����œǂ߂邼�I",   		"",											},
	{RES_MDL_PLATE_COMMON,		RES_LYT_RING_FLAME_RING,		"���̎w��",						300,	"���̃A�C�e���������Ă����",       				"���n�̃_���[�W�𔼌����邼�I",   				"",											},
	{RES_MDL_PLATE_COMMON,		RES_LYT_RING_CHICK_PIERCE,		"�q���R�̃s�A�X",				560,	"���̃A�C�e���������Ă����",       				"�s���s���Q�[�W�̌����3/4�ɂ��邼�I",   		"",											},
	{RES_MDL_PLATE_COMMON,		RES_LYT_RING_FAIRY_BRACELET,	"�d���̘r��",					480,	"���̃A�C�e���������Ă����",       				"����MP�𔼌����邼�I",   						"",											},
};

/*=====================================*
 * 
 *=====================================*/
static const eItemKind sKeyitemLinkTable[eKEYITEM_MAX] = {
	eITEM_KIND_MASTERSWORD,
	eITEM_KIND_SEAL_KEY,
	eITEM_KIND_WALLET_A,
	eITEM_KIND_WALLET_B,
	eITEM_KIND_WALLET_C,
	eITEM_KIND_LIB_SHIELD,
	eITEM_KIND_ANGEL_WING,
	eITEM_KIND_TROPHY,
	eITEM_KIND_BOOK,
	eITEM_KIND_FLAME_RING,
	eITEM_KIND_CHICK_PIERCE,
	eITEM_KIND_FAIRY_BRACELET,

	eITEM_KIND_NAVI_LIGHT,
	eITEM_KIND_CARD_K,
	eITEM_KIND_CARD_Q,
	eITEM_KIND_CARD_J,

	eITEM_KIND_SMILE_0,
	eITEM_KIND_SMILE_1,
	eITEM_KIND_SMILE_2,
	eITEM_KIND_SMILE_3,
	eITEM_KIND_SMILE_4,
	eITEM_KIND_SMILE_5,
	eITEM_KIND_DANCE,
};

/*=====================================*
 *
 *=====================================*/
static const char* strSongName[eSONG_TYPE_MAX] = {
	"���z�̉S",			// eSONG_TYPE_SUN,		// ���z�̉S
	"�����̉S",			// eSONG_TYPE_WILDNESS,	// �����̉S
	"���̉S",			// eSONG_TYPE_WING,		// ���̉S
	"����̉S",			// eSONG_TYPE_HANDLING,	// ����̉S	
};

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
const tItemDetailInfo * GetItemDetailInfo(BYTE itemKind)
{
	return &sItemDetailInfoTable[itemKind];
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
eItemKind GetKeyitemItemKind(BYTE keyitemKind)
{
	APP_ASSERT_PRINT( (keyitemKind < eKEYITEM_MAX),
					  "Invalid keyitemKind : %d \n", keyitemKind);
	return sKeyitemLinkTable[keyitemKind];
}

//---------------------------------------------------
/**
 * �v���[�g�����擾����
 */
//---------------------------------------------------
char* GetItemPlateName(BYTE itemKind)
{
	char* str_plate_name = NULL;

	switch (itemKind) {
	case eITEM_KIND_SMILE_0:
	case eITEM_KIND_SMILE_1:
	case eITEM_KIND_SMILE_2:
	case eITEM_KIND_SMILE_3:
	case eITEM_KIND_SMILE_4:
	case eITEM_KIND_SMILE_5:
	case eITEM_KIND_DANCE:
		str_plate_name = (char *)RES_LYT_ICON_MAP_LINO_FACE;
		break;

	case eITEM_KIND_PORTION:
		str_plate_name = (char *)RES_LYT_ITEM_RING_PORTION;
		break;
	case eITEM_KIND_COSPLAY_FRES:
		str_plate_name = RES_LYT_COSPLAY_FRES;
		break;
	case eITEM_KIND_COSPLAY_DABAH:
		str_plate_name = RES_LYT_COSPLAY_DABAH;
		break;
	case eITEM_KIND_COSPLAY_SUNOPHONE:
		str_plate_name = RES_LYT_COSPLAY_HATOPHONE;
		break;
	case eITEM_KIND_COSPLAY_MASK:
		str_plate_name = RES_LYT_COSPLAY_MASK;
		break;
	case eITEM_KIND_COSPLAY_BAT:
		str_plate_name = RES_LYT_COSPLAY_BAT;
		break;
	case eITEM_KIND_COSPLAY_TDA_MIKU:
		str_plate_name = RES_LYT_COSPLAY_TDA_MIKU;
		break;
	case eITEM_KIND_COSPLAY_PRONAMA:
		str_plate_name = RES_LYT_COSPLAY_PRONAMA;
		break;
	case eITEM_KIND_COSPLAY_LAPAN:
		str_plate_name = RES_LYT_COSPLAY_LAPAN;
		break;
	case eITEM_KIND_COSPLAY_CHILNO:
		str_plate_name = RES_LYT_COSPLAY_CHILNO;
		break;
	case eITEM_KIND_COSPLAY_HONGKONG:
		str_plate_name = RES_LYT_COSPLAY_HONG_KONG;
		break;
	case eITEM_KIND_COSPLAY_LOANA:
		str_plate_name = RES_LYT_COSPLAY_LOANA;
		break;
	case eITEM_KIND_COSPLAY_YUKARI:
		str_plate_name = RES_LYT_COSPLAY_YUKARI;
		break;
	case eITEM_KIND_COSPLAY_NAKANO:
		str_plate_name = RES_LYT_COSPLAY_NAKANO;
		break;


	case eITEM_KIND_PLATE_ARROW:
		str_plate_name = RES_LYT_PLATE_ARROW;
		break;
	case eITEM_KIND_PLATE_BOMB:
		str_plate_name = RES_LYT_PLATE_BOMB;
		break;

	case eITEM_KIND_WALLET_A:
		str_plate_name = (char *)RES_LYT_RING_WALLET_A;
		break;
	case eITEM_KIND_WALLET_B:
		str_plate_name = (char *)RES_LYT_RING_WALLET_B;
		break;
	case eITEM_KIND_WALLET_C:
		str_plate_name = (char *)RES_LYT_RING_WALLET_C;
		break;
	case eITEM_KIND_SEAL_KEY:
		str_plate_name = (char *)RES_LYT_RING_SEAL_KEY;
		break;

	case eITEM_KIND_MAGIC_THUNDER:
		str_plate_name  = (char *)RES_LYT_RING_MAGIC_THUNDER;
		break;
	case eITEM_KIND_MAGIC_FLAME:
		str_plate_name  = (char *)RES_LYT_RING_MAGIC_FLAME;
		break;
	case eITEM_KIND_MAGIC_TORNADE:
		str_plate_name  = (char *)RES_LYT_RING_MAGIC_TORNADE;
		break;

	case eITEM_KIND_FLAME_RING:
		str_plate_name  = (char *)RES_LYT_RING_FLAME_RING;
		break;
	case eITEM_KIND_CHICK_PIERCE:
		str_plate_name  = (char *)RES_LYT_RING_CHICK_PIERCE;
		break;
	case eITEM_KIND_FAIRY_BRACELET:
		str_plate_name  = (char *)RES_LYT_RING_FAIRY_BRACELET;
		break;

	default:
		break;
	};

	return str_plate_name;
}

//---------------------------------------------------
/**
 * �J�[�h�����擾����
 */
//---------------------------------------------------
char* GetItemCardName(BYTE itemKind)
{
	char* str_card_name = NULL;
	
	switch (itemKind) {
		
	case eITEM_KIND_CARD_K:
		str_card_name = (char *)RES_LYT_CARD_K;
		break;
	case eITEM_KIND_CARD_Q:
		str_card_name = (char *)RES_LYT_CARD_Q;
		break;
	case eITEM_KIND_CARD_J:
		str_card_name = (char *)RES_LYT_CARD_J;
		break;
		
	default:
		break;
	};
	
	return str_card_name;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
char* GetSongName(BYTE songType)
{
	return (char *)strSongName[songType];
}

/**** end of file ****/
