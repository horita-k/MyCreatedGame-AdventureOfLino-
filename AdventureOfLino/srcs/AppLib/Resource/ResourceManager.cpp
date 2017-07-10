/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * ���\�[�X�Ǘ��N���X
 * @author SPATZ.
 * @data   2011/11/27 15:30:43
 */
//---------------------------------------------------
#include "ResourceManager.h"

#include "ResourceModel.h"
#include "AppLib/Sound/SoundManager.h"
#include "DataDefine/VoiceDef.h"

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
/*static*/ResourceManager* const
ResourceManager::GetInstance(void)
{
	static ResourceManager sInstance;
	return &sInstance;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
ResourceManager::LoadCharaResource(char* charaName)
{
	// ���f���ǂݍ���
	ResourceModel::GetInstance()->LoadCharaResource(charaName);

	// �T�E���h�ǂݍ���
	SoundManager::GetInstance()->LoadVoiceComponent(charaName, eVC_MAX);
}

 /**** end of file ****/

