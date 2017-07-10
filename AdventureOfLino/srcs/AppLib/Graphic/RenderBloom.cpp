/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �u���[�������_�����O�N���X
 * @author SPATZ.
 * @data   2015/11/23 12:25:41
 */
//---------------------------------------------------
#include "DxLib.h"

#include "AppLib/Basic/Basic.h"
#include "CameraManager.h"

#include "RenderBloom.h"

#include "DataDefine/ItemDef.h"

/*static*/int RenderBloom::mIsActive;
static int sScrHightBright	= -1;
static int sScrDownScale	= -1;
static int sScrGauss		= -1;
static int sScrWindow		= -1;

// �u���[���֘A 
static BOOL								mEnableSystemBloom;	// �V�X�e�����ł̃u���[���L���t���O
static BOOL								mIsBloomActive;
static RenderBloom::eBloomType			mBloomType;
static BYTE								mBloomBlendMode;
static WORD								mBloomBlendValue;
static WORD								mBloomGaussValue;
static RenderBloom::tBloomBrightParam	mBloomBrightParam;
static RenderBloom::tBloomHSBParam		mBloomHSBParam;
static RenderBloom::tChangeBloomWork	mChangeBloomWork;

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
RenderBloom::Create(void)
{
	mIsActive			= TRUE;
	mEnableSystemBloom	= TRUE;
	mIsBloomActive		= TRUE;

	memset(&mChangeBloomWork, 0, sizeof(mChangeBloomWork));

	mBloomType			= eBLOOM_TYPE_BRIGHT_LESS;
	mBloomBlendMode		= 2;
	mBloomBlendValue	= 255;
	mBloomGaussValue	= 100;
	memset(&mBloomBrightParam, 0, sizeof(mBloomBrightParam));
	mBloomBrightParam.mClipParam = 230;

	memset(&mBloomHSBParam, 0, sizeof(mBloomHSBParam));
	mBloomHSBParam.mHue = 100;
	mBloomHSBParam.mSaturation = 90;
	mBloomHSBParam.mBright = -60;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
RenderBloom::Setup(void)
{
	if (mIsActive == FALSE) return;

	int x, y, depth;
	GetScreenState(&x, &y, &depth);

	sScrHightBright		= MakeScreen(x, y, FALSE);
	sScrDownScale		= MakeScreen((x / 8), (y / 8), FALSE);
	sScrGauss			= MakeScreen((x / 8), (y / 8), FALSE);
	
	SetDrawValidMultiSample(4, 2);
	sScrWindow			= MakeScreen(x, y, FALSE);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
RenderBloom::ClearScreen(void)
{
	if (mEnableSystemBloom) {
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();

		SetDrawScreen( sScrWindow );
		CameraManager::GetInstance()->RevertNearFarClip();
	}

	ClearDrawScreen();
}

//---------------------------------------------------
/**
 * �u���[���֘A����
 */
//---------------------------------------------------
void
RenderBloom::Update(void)
{
	if (mEnableSystemBloom == FALSE) return;
	// �u���[������

	if (mIsBloomActive == FALSE) {
		// �`��Ώۂ𗠉�ʂɂ���
		SetDrawScreen( DX_SCREEN_BACK ) ;
	
		// �ʏ�̕`�挋�ʂ�`�悷��
		DrawGraph( 0, 0, sScrWindow, FALSE );
	}
	// �u���[�������L��
	else {
		tChangeBloomWork* p_change = &mChangeBloomWork;
		if (p_change->mIsActive == TRUE) {
			p_change->mNowSec += (1.0f / 60.0f);
			if (p_change->mNowSec > p_change->mMaxSec) {
				p_change->mIsActive = FALSE;
				mBloomBlendValue = p_change->mBlendValueTarget;
				mBloomGaussValue = p_change->mGaussValueTarget;
			} else {
				float rate = p_change->mNowSec / p_change->mMaxSec;
				mBloomBlendValue = (WORD)((float)(p_change->mBlendValueTarget - p_change->mBlendValueBase) * rate) + p_change->mBlendValueBase;
				mBloomGaussValue = (WORD)((float)(p_change->mGaussValueTarget - p_change->mGaussValueBase) * rate) + p_change->mGaussValueBase;
			}
		}

		if ((mBloomType == eBLOOM_TYPE_BRIGHT_LESS) ||
			(mBloomType == eBLOOM_TYPE_BRIGHT_GREATER)) {

			// �`�挋�ʂ��獂�P�x�����݂̂𔲂��o�����摜�𓾂�
			GraphFilterBlt(sScrWindow, 
						   sScrHightBright, 
						   DX_GRAPH_FILTER_BRIGHT_CLIP, 
						   (mBloomType == eBLOOM_TYPE_BRIGHT_LESS) ? DX_CMP_LESS : DX_CMP_GREATER,
						   mBloomBrightParam.mClipParam,
						   TRUE,					// �N���b�v�����s�N�Z����h��Ԃ���
						   GetColor(mBloomBrightParam.mPixelColorR, mBloomBrightParam.mPixelColorG, mBloomBrightParam.mPixelColorB),
						   255) ;					// �s�N�Z���̃��l
		}
		else if ((mBloomType == eBLOOM_TYPE_HSB_RELATIVE) ||
				 (mBloomType == eBLOOM_TYPE_HSB_ABSOLUTE)) {
			// �F���E�ʓx�E�P�x�t�B���^�[���{��
			GraphFilterBlt(sScrWindow, 
						   sScrHightBright,
						   DX_GRAPH_FILTER_HSB, 
						   (mBloomType == eBLOOM_TYPE_HSB_RELATIVE) ? 0 : 1,	// (0:���� 1:���)
						   mBloomHSBParam.mHue,
						   mBloomHSBParam.mSaturation,
						   mBloomHSBParam.mBright);
		}

		// ���P�x�������W���̂P�ɏk�������摜�𓾂�
		GraphFilterBlt(sScrHightBright, sScrDownScale, DX_GRAPH_FILTER_DOWN_SCALE, 8);

		// �W���̂P�ɏk�������摜���K�E�X�t�B���^�łڂ���
		GraphFilterBlt(sScrDownScale, 
					   sScrGauss, 
					   DX_GRAPH_FILTER_GAUSS, 
					   16,						// �g�p�s�N�Z����
					   mBloomGaussValue);		// �ڂ����p�����[�^

		// �`��Ώۂ𗠉�ʂɂ���
		SetDrawScreen( DX_SCREEN_BACK ) ;
	
		int x, y, depth;
		GetScreenState(&x, &y, &depth);

		// �ʏ�̕`�挋�ʂ�`�悷��
		DrawGraph( 0, 0, sScrWindow, FALSE );

		// �`�惂�[�h���o�C���j�A�t�B���^�����O�ɂ���( �g�債���Ƃ��Ƀh�b�g���ڂ₯��悤�ɂ��� )
		SetDrawMode( DX_DRAWMODE_BILINEAR ) ;

		// �`��u�����h���[�h�����Z�ɂ���
		SetDrawBlendMode(mBloomBlendMode, mBloomBlendValue);

		// ���P�x�������k�����Ăڂ������摜����ʂ����ς��ɂQ��`�悷��( �Q��`�悷��̂͂�薾�邭�݂���悤�ɂ��邽�� )
		DrawExtendGraph( 0, 0, x, y, sScrGauss, FALSE ) ;

		// �`��u�����h���[�h���u�����h�����ɖ߂�
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 255 ) ;

		// �`�惂�[�h���A���X�g�ɖ߂�
		SetDrawMode( DX_DRAWMODE_NEAREST ) ;
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
RenderBloom::Destroy(void)
{
	return;
}

/*=====================================*
 * �u���[��
 *=====================================*/
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
RenderBloom::SetEnableSystemBloom(BOOL isEnable)
{
	mEnableSystemBloom = isEnable; 
	if (isEnable == FALSE) {
		SetDrawScreen( DX_SCREEN_BACK ) ;
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
BOOL
RenderBloom::GetEnableSystemBloom(void)
{
	return mEnableSystemBloom;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
RenderBloom::SetActiveBloom(BOOL isActive)
{
	mIsBloomActive = isActive;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
RenderBloom::SetBloomType(RenderBloom::eBloomType eType)
{
	mBloomType = eType;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
RenderBloom::eBloomType
RenderBloom::GetBloomType(void)
{
	return mBloomType;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
RenderBloom::SetBloomParam(BYTE blendMode, WORD blendValue, WORD gaussValue)
{
	mBloomBlendMode		= blendMode;
	mBloomBlendValue	= blendValue;
	mBloomGaussValue	= gaussValue;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
RenderBloom::GetBloomParam(BYTE& rBlendMode, WORD& rBlendValue, WORD& rGaussValue)
{
	rBlendMode	= mBloomBlendMode;
	rBlendValue = mBloomBlendValue;
	rGaussValue = mBloomGaussValue;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
RenderBloom::tBloomBrightParam&
RenderBloom::BloomBrightParam(void)
{
	return mBloomBrightParam;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
RenderBloom::tBloomHSBParam&
RenderBloom::BloomHSBParam(void)
{
	return mBloomHSBParam;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
RenderBloom::ChangeBloomParam(float setSec, WORD blendValue, WORD gaussValue)
{
	mChangeBloomWork.mIsActive = TRUE;
	mChangeBloomWork.mBlendValueBase	= mBloomBlendValue;
	mChangeBloomWork.mBlendValueTarget	= blendValue;
	mChangeBloomWork.mGaussValueBase	= mBloomGaussValue;
	mChangeBloomWork.mGaussValueTarget	= gaussValue;
	mChangeBloomWork.mNowSec = 0.0f;
	mChangeBloomWork.mMaxSec = setSec;
}

 /**** end of file ****/
