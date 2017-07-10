/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �X�e�[�W�ŗL�̏���
 * @author SPATZ.
 * @data   2015/08/26 22:06:24
 */
//---------------------------------------------------
#include <stdio.h>
#include <math.h>
#include <d3dx9math.h>
#include "StageFunc042.h"
#include "AppLib/Effect/EffectManager.h"
#include "AppLib/Sound/SoundManager.h"
#include "AppLib/Graphic/CameraManager.h"
#include "AppLib/Graphic/GraphicManager.h"
#include "AppLib/Resource/ResourceManager.h"
#include "AppLib/Resource/ResourceModel.h"
#include "AppLib/Resource/ResourceLayout.h"
#include "DataDefine/EffectDef.h"
#include "Gimmick/GimmickBase.h"
#include "Gimmick/GimmickExtend.h"
#include "../CommandFunctions.h"
#include "../ExternDefine.h"		//<! Extern �錾�͑S�Ă����ɏW�񂷂�

/*=====================================*
 * static
 *=====================================*/

/*=====================================*
 * public
 *=====================================*/
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
StageFunc042::StageFunc042()
{
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
StageFunc042::~StageFunc042()
{
	destroy();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
StageFunc042::SetupBeforeLoad(void)
{
	// �X�e�[�W�o�b�t�@
	mEffHndAura	= -1;
	mEffHndCrystal	= -1;

	ResourceModel::GetInstance()->LoadGimmickResource(RES_MDL_BARRIER);

	/**** �x�[�X�N���X�֐������N���X������ɃR�[�� ****/
	StageFuncBase::SetupBeforeLoad();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
StageFunc042::SetupAfterLoad(int& rPhase)
{
	int model_handle = sModelMap.GetModelHandle();

	/**** �x�[�X�N���X�֐������N���X�����O�ɃR�[�� ****/
	StageFuncBase::SetupAfterLoad(rPhase);

	sModelXfile[eMD_SKYDOME	].Scale() = ZERO_VEC;

	GraphicManager::GetInstance()->SetEnableDirectionLight(FALSE);
	GraphicManager::GetInstance()->SetEnablePLight(TRUE);
	GraphicManager::GetInstance()->SetPLightPosition(&VGet(0.000f, -252000.000f, 0.000f) );
	GraphicManager::GetInstance()->SetPLightRangeAtten(309000.000f, 0.13f, (float)3E-07, 0);
	GraphicManager::GetInstance()->SetPLightDiffuse(0.419610f, 0.678430f, 0.713730f, 0.000000f);
	GraphicManager::GetInstance()->SetPLightSpecular(0.000000f, 0.000000f, 0.000000f, 0.000000f);
	GraphicManager::GetInstance()->SetPLightAmbient(0.247060f, 0.427450f, 0.247060f, 0.000000f);

	CollisionManager::GetInstance()->EntryEarthCollision(sModelMap.GetModelHandle(), (eBG_TYPE_GIMMICK + 0));
	CollisionManager::GetInstance()->EntryWallCollision(sModelMap.GetModelHandle(), (eBG_TYPE_GIMMICK + 1));
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
StageFunc042::Update(int counter)
{
	/*=====================================*
	 * �R���p�X�̕\��
	 *=====================================*/
	TotalInfo::GetInstance()->UpdateCompass();
}

/*=====================================*
 * private
 *=====================================*/
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
StageFunc042::destroy(void)
{
	return;
}

/**** end of file ****/


