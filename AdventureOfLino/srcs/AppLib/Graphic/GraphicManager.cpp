/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �O���t�B�b�N�����}�l�[�W���N���X
 * @author SPATZ.
 * @data   2011/11/20 23:25:23
 */
//---------------------------------------------------
#include "AppLib/Basic/Basic.h"
#include "GraphicManager.h"
#include "RenderBloom.h"
#include "../Effect/EffectManager.h"
#include "EffekseerManager.h"
#include "CameraManager.h"
#include "ShadowMap.h"

#include "AppLib/Resource/ResourceLayout.h"

#ifdef _DEBUG
#include "AppLib/Input/InputManager.h"
#include "AppLib/Basic/Counter.h"
#endif // _DEBUG

/*=====================================*
 * macro
 *=====================================*/
 /* �O���b�h�\����L���ɂ��� */
#undef _VISIBLE_GRID

/* �u���[���L�� */
#define _ENABLE_BLOOM

/*=====================================*
 * �v���g�^�C�v
 *=====================================*/
void dummyDrawFunc(void);

/*=====================================*
 * static
 *=====================================*/
#ifdef _DEBUG
static bool sIsRenderDbgString	= true;
static bool sIsRenderDbgGraph	= false;
#endif // _DEBUG

/* ���荞�݊֐� */
static FUNC_DRAW_FONT spDrawFontFunc   = dummyDrawFunc;
static FUNC_DRAW_FONT spLockonFontFunc = dummyDrawFunc;

// �t�H���g�n���h��
static int sFontHandle[GraphicManager::eFONT_TYPE_MAX] = {-1, -1};
static BYTE sNowFontType = GraphicManager::eFONT_TYPE_DEFAULT;


//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
/*static*/GraphicManager * const
GraphicManager::GetInstance(void)
{
	static GraphicManager sInstance;
	return &sInstance;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
GraphicManager::GraphicManager()
{
	// initialize
	mDrawModelVector.clear();
	mDrawPreUpdateModelVector.clear();
	mDrawModelMapVector.clear();
	mpCursorModel = NULL;
	mpAfterEffectModel = NULL;
	mIsUpdateDrawModelVector = FALSE;
	mDrawLayoutVector.clear();
	mSystemLayoutVector.clear();
	mDrawStringVector.clear();
	mShapeLineVector.clear();
#ifdef _DEBUG
	mDbgShapeVector.clear();
#endif // _DEBUG
	mDrawShadowVector.clear();

	mLayoutScaleRateX = 0.0f;
	mLayoutScaleRateY = 0.0f;
	mAspectRate = 0.0f;

	sReserveEntryModel.clear();
	sReserveRemoveModel.clear();

	memset(&mFogParam, 0, sizeof(mFogParam));
	mFogParam.mFar = 50000.0f;

	memset(&mFogChangeWork, 0, sizeof(mFogChangeWork));

	mLightDirection = ZERO_VEC;
	for (int iI = 0; iI < ePLIGHT_NO_MAX; iI ++) {
		mOtherLightHandle[iI] = -1;
	}

	// DxLib�V�F�[�_�[
	mVShaderHandle = -1;
	mPShaderHandle = -1;
	mIsEnableOriginalShader = FALSE;

	// �u���[���֘A�̏�����
	RenderBloom::Create();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
GraphicManager::~GraphicManager()
{
	// ���C�g�n���h����S�č폜����
	DeleteLightHandleAll();
	for (int iI = 0; iI < ePLIGHT_NO_MAX; iI ++) {
		mOtherLightHandle[iI] = -1;
	}

	// �쐬�����t�H���g�f�[�^���폜����

	for (int iI = 0; iI < eFONT_TYPE_MAX; iI ++) {
		if (sFontHandle[iI] != -1) {
			DeleteFontToHandle(sFontHandle[iI]);
			sFontHandle[iI] = -1;
		}
	}

	// DxLib�V�F�[�_�[�̔j��
	if (mVShaderHandle != -1) {
		DeleteShader(mVShaderHandle);
		mVShaderHandle = -1;
	}
	if (mPShaderHandle != -1) {
		DeleteShader(mPShaderHandle);
		mPShaderHandle = -1;
	}

	// �V���h�E�}�b�v�̏I��
	ShadowMap::Destroy();

	// �u���[���֘A�̏I��
	RenderBloom::Destroy();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::Create(void)
{
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::Setup(void)
{
	// ���C�g�ݒ�
	ePLightNo light_no;

	// ���C�g�n���h�����쐬����
	light_no = ePLIGHT_NO_00;
	mOtherLightHandle[light_no] = CreatePointLightHandle(VGet( 0.0f, 0.0f, 0.0f ), 0.0f, 0.0f, 0.0f, 0.0f);
	SetLightEnableHandle(mOtherLightHandle[light_no], FALSE);

	light_no = ePLIGHT_NO_01;
	mOtherLightHandle[light_no] = CreatePointLightHandle(VGet( 0.0f, 0.0f, 0.0f ), 0.0f, 0.0f, 0.0f, 0.0f);
	SetLightEnableHandle(mOtherLightHandle[light_no], FALSE);

	light_no = eSLIGHT_NO_00;
	mOtherLightHandle[light_no] = CreateSpotLightHandle(ZERO_VEC,
														VGet(0.0f, 0.0f, 1.0f),
														DX_PI_F/2.0f, DX_PI_F/4.0f,	0.0f,
														0.0f, 0.0f, 0.0f);
	SetLightEnableHandle(mOtherLightHandle[light_no], FALSE);

	// �f�t�H���g�I�t�����A�ꉞ�ݒ肵�Ă���
	SetPLightPosition(&VGet(0.0f, 1000.0f, 0.0f));
	SetPLightRangeAtten(5000.0f,
						0.1f,
						0.0f, 
						0.0f );
	SetPLightDiffuse(0.5f, 0.5f, 0.5f, 0.0f);
	SetPLightSpecular(0.0f, 0.0f, 0.0f, 0.0f);
	SetPLightAmbient(0.25f, 0.25f, 0.25f, 0.0f);

	// �f�B���N�V�������C�g���f�t�H���g�l�ɐݒ�
	SetDefaultDirectionLight();

	mLightDirection = VNorm(VGet(1.0f, -2.0f, 1.0f));
	SetLightDirection(mLightDirection);

	/* �d�͐ݒ� */
	MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_LOADCALC);
	MV1SetLoadModelPhysicsWorldGravity( -10.0f );

	int x, y, depth;
	GetScreenState(&x, &y, &depth);

	/* ��ʔ䂩�烌�C�A�E�g�̔䗦�����߂� */
	float screen_rate;
	screen_rate = (float)y / (float)x;
	if (screen_rate == 0.5625f) {
		/* 16 : 9 */
		mLayoutScaleRateX = (float)x / 640.0f;
		mLayoutScaleRateY = (float)y / 360.0f;

		mAspectRate = (360.0f / 480.0f);		// Y����1�ɂ���l�ɂ��킹��
	} else if (screen_rate == 0.8f) {
		/* 5 : 4 */
		mLayoutScaleRateX = (float)x / 640.0f;
		mLayoutScaleRateY = (float)y / 512.0f;

		mAspectRate = (512.0f / 480.0f);		// Y����1�ɂ���l�ɂ��킹��
	} else {
		/* 4 : 3 */
		mLayoutScaleRateX = (float)x / 640.0f;
		mLayoutScaleRateY = (float)y / 480.0f;

		mAspectRate = (480.0f / 480.0f);		// Y����1�ɂ���l�ɂ��킹��
	}

	mLayoutScaleRateX *= mAspectRate;
	mLayoutScaleRateY *= mAspectRate;

	// DxLib�̃V�F�[�_�[���g�p
	mVShaderHandle = LoadVertexShader("object/DxLibShader/PointLightShaderVS.vso");
	mPShaderHandle = LoadPixelShader("object/DxLibShader/PointLightShaderPS.pso");
	
	// �V���h�E�}�b�v�̐���
	ShadowMap::Setup();

	// �u���[���֘A�̃Z�b�g�A�b�v
	RenderBloom::Setup();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::CreateFontData(int fontSize)
{
#define FONT_NAME		("HG�ۺ޼��M-PRO")

	const int font_thin = 5;

	// �t�H���g�𐶐�����
	if (sFontHandle[eFONT_TYPE_DEFAULT] == -1) {
		sFontHandle[eFONT_TYPE_DEFAULT] = CreateFontToHandle(FONT_NAME,						// �쐬����t�H���g��( NULL �ɂ���ƃf�t�H���g�̃t�H���g )
															 fontSize,						// �t�H���g�̃T�C�Y( ���悻�h�b�g�� -1:�f�t�H���g�̃T�C�Y )
															 font_thin,						// �t�H���g�̑���( 0 �`�@9 -1:�f�t�H���g�̑��� )
															 DX_FONTTYPE_ANTIALIASING_4X4);	// �t�H���g�̃^�C�v
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::CreateEndingFontData(void)
{
#define ENDING_FONT_NAME		("SimSun")

	const int font_thin = 5;
	int font_size = GetFontSizeToHandle( sFontHandle[eFONT_TYPE_DEFAULT] ) / 2;

	// �t�H���g�𐶐�����
	if (sFontHandle[eFONT_TYPE_ENDING] == -1) {
		sFontHandle[eFONT_TYPE_ENDING] = CreateFontToHandle(ENDING_FONT_NAME,						// �쐬����t�H���g��( NULL �ɂ���ƃf�t�H���g�̃t�H���g )
															font_size,						// �t�H���g�̃T�C�Y( ���悻�h�b�g�� -1:�f�t�H���g�̃T�C�Y )
															font_thin,						// �t�H���g�̑���( 0 �`�@9 -1:�f�t�H���g�̑��� )
															DX_FONTTYPE_ANTIALIASING_4X4);	// �t�H���g�̃^�C�v
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::DeleteEndingFontData(void)
{
	if (sFontHandle[eFONT_TYPE_ENDING] != -1) {
		DeleteFontToHandle(sFontHandle[eFONT_TYPE_ENDING]);
		sFontHandle[eFONT_TYPE_ENDING] = -1;
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::SetFontType(GraphicManager::eFontType fontType) 
{
	sNowFontType = fontType;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::Update(void)
{
	// �V���h�E�}�b�v�̍X�V
	ShadowMap::Update();

	// �V���h�E�}�b�v�`��J�n
	ShadowMap::RenderBegin();

	/**** �}�b�v�`��O�̃��f���`�� ****/
	{
		std::vector<ModelBase *>::iterator ite;
		if (mIsStopModeUpdate == false) {
			for (ite = mDrawPreUpdateModelVector.begin(); ite != mDrawPreUpdateModelVector.end(); ite ++) {
				(*ite)->PreUpdate();
			}
		} else {
			for (ite = mDrawPreUpdateModelVector.begin(); ite != mDrawPreUpdateModelVector.end(); ite ++) {
				(*ite)->PreDraw();
			}
		}
	}

	// �V���h�E�}�b�v�`��I��
	ShadowMap::RenderEnd();

	// �V���h�E�}�b�v�`��J�n
	ShadowMap::RenderBegin();

	/**** �}�b�v�`�� ****/
	{
		std::vector<ModelMap *>::iterator ite;
		for (ite = mDrawModelMapVector.begin(); ite != mDrawModelMapVector.end(); ite ++) {
			(*ite)->Update();

		}
	}
	// �V���h�E�}�b�v�`��I��
	ShadowMap::RenderEnd();


	/**** ���f���`�� ****/
	mIsUpdateDrawModelVector = TRUE;
	{
		std::vector<ModelBase *>::iterator ite;
		if (mIsStopModeUpdate == false) {
			for (ite = mDrawModelVector.begin(); ite != mDrawModelVector.end(); ite ++) {
				(*ite)->Update();
			}
		} else {
			for (ite = mDrawModelVector.begin(); ite != mDrawModelVector.end(); ite ++) {
				(*ite)->Draw();
			}
		}
	}
	mIsUpdateDrawModelVector = FALSE;


	// �V���h�E�}�b�v�`��I��
	ShadowMap::RenderBegin();

	/**** �}�b�v(�A���t�@����)�`�� ****/
	{
		std::vector<ModelMap *>::iterator ite;
		for (ite = mDrawModelMapVector.begin(); ite != mDrawModelMapVector.end(); ite ++) {
			(*ite)->RenderAlphaFrame();
		}
	}
	// �V���h�E�}�b�v�`��I��
	ShadowMap::RenderEnd();


	/**** �e�`�� ****/
	// shadow
	if (ShadowMap::GetShadowMapType() == ShadowMap::eSHADOWMAP_TYPE_NONE) {
		std::vector<Shadow *>::iterator ite;
		for (ite = mDrawShadowVector.begin(); ite != mDrawShadowVector.end(); ite ++) {
			(*ite)->Update();
		}
	} 
	else {
		std::vector<Shadow *>::iterator ite;
		for (ite = mDrawShadowVector.begin(); ite != mDrawShadowVector.end(); ite ++) {
			(*ite)->UpdateNotRender();
		}
	}

	/*** �J�[�\�����f�� (navi) �`�� ****/
	if (mpCursorModel != NULL) {
		mpCursorModel->Update();
	}

	// Bloom���L���̏ꍇ�݈̂ȉ��̏����������Ȃ�
	if (RenderBloom::GetEnableSystemBloom() == TRUE) {

		/**** �G�t�F�N�g�`�� ****/
		SetFogEnable(FALSE);

		// �`��s��𒼂��ׂ̃_�~�[�`��A�Ȃ񂶂Ⴛ���
		VERTEX3D Vertex[ 3 ] ;
		int graph_handle = ResourceLayout::GetInstance()->GetHandle("Shadow.tga");
		DrawPolygon3D( Vertex, 1, graph_handle, TRUE ) ;

		// effect
		EffectManager::GetInstance()->Update();
		SetFogEnable(mFogParam.mIsEnable & mFogParam.mIsActive);

		/*** �G�t�F�N�g�`���ɕ`�悳��郂�f�� (����Ȃ�������) ****/
		if (mpAfterEffectModel != NULL) {
			mpAfterEffectModel->Update();
		}

		/**** Effekseer �`�� ****/
		SetFogEnable(FALSE);

		if (mIsStopModeUpdate == false) {
			EffekseerManager::GetInstance()->Update(CameraManager::GetInstance()->Position(),
													CameraManager::GetInstance()->Target(),
													CameraManager::GetInstance()->Up());
		}
		else {
			EffekseerManager::GetInstance()->Draw(CameraManager::GetInstance()->Position(),
													CameraManager::GetInstance()->Target(),
													CameraManager::GetInstance()->Up());
		}

	}


	// �V���h�E�}�b�v�쐬�J�n
	{
		SetFogEnable(FALSE);
		ShadowMap::CreateShadowMap(mDrawModelVector);
		// �t�H�O�������ɖ߂�
		SetFogEnable(mFogParam.mIsEnable & mFogParam.mIsActive);
	}


	/**** �t�H�O�̕ω����� ****/
	if (mFogChangeWork.mIsActive == true) {
		mFogParam.mColR += mFogChangeWork.mDiffParam.mColR;
		mFogParam.mColG += mFogChangeWork.mDiffParam.mColG;
		mFogParam.mColB += mFogChangeWork.mDiffParam.mColB;
		mFogParam.mNear += mFogChangeWork.mDiffParam.mNear;
		mFogParam.mFar  += mFogChangeWork.mDiffParam.mFar;

		if (mFogParam.mNear > mFogParam.mFar) {
			mFogParam.mNear = mFogParam.mFar - 0.1f;
		}
		mFogChangeWork.mNowSec += (1.0f / 60.0f);
		if (mFogChangeWork.mMaxSec <= mFogChangeWork.mNowSec) {
			mFogChangeWork.mIsActive = false;

			mFogParam.mIsEnable = mFogChangeWork.mDiffParam.mIsEnable;
			mFogParam.mIsActive = mFogChangeWork.mDiffParam.mIsActive;
		} else {
			// �t�H�O���X�V����
			SetFogColor(mFogParam.mColR, mFogParam.mColG, mFogParam.mColB);
			SetFogStartEnd(mFogParam.mNear, mFogParam.mFar);
		}
	}

	// �t�H�O�������ɖ߂�
	SetFogEnable(mFogParam.mIsEnable & mFogParam.mIsActive);

	/**** Bloom�֘A�X�V ****/
	RenderBloom::Update();

	// �o�C���j�A�@�ɐݒ�
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	/**** ���C�A�E�g�`�� ****/
	{
		std::vector<LayoutBase *>::iterator ite;
		for (ite = mDrawLayoutVector.begin(); ite != mDrawLayoutVector.end(); ite ++) {
			(*ite)->Update();
		}
	}

	/**** �V�X�e�����C�A�E�g�`�� ****/
	{
		std::vector<LayoutBase *>::iterator ite;
		for (ite = mSystemLayoutVector.begin(); ite != mSystemLayoutVector.end(); ite ++)
		{
			(*ite)->Update();
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	SetDrawBright(255, 255, 255);

	/**** ���`�̕`�� ****/
	{
		std::vector<ShapeLine>::iterator ite;
		for (ite = mShapeLineVector.begin(); ite != mShapeLineVector.end(); ite ++) {
			(*ite).Draw();
		}
	}

#ifdef _DEBUG
	{
		std::vector<tShapeVector>::iterator ite;
		for (ite = mDbgShapeVector.begin(); ite != mDbgShapeVector.end(); ite ++) {
			{
				std::vector<ShapeCapsule>::iterator sub;
				for (sub = (*ite).mCapsule.begin(); sub != (*ite).mCapsule.end(); sub ++) {
					(*sub).Draw();
				}
			}
			{
				std::vector<ShapeLine>::iterator sub;
				for (sub = (*ite).mLine.begin(); sub != (*ite).mLine.end(); sub ++) {
					(*sub).Draw();
				}
			}
			{
				std::vector<ShapeBox>::iterator sub;
				for (sub = (*ite).mBox.begin(); sub != (*ite).mBox.end(); sub ++) {
					(*sub).Draw();
				}
			}
			{
				std::vector<ShapeBoard>::iterator sub;
				for (sub = (*ite).mBoard.begin(); sub != (*ite).mBoard.end(); sub ++) {
					(*sub).Draw();
				}
			}
		}
	}
	// �O���b�h��\������
#ifdef _VISIBLE_GRID
	{
		int width_length = 32;
		int height_length = 32;
		int x, y, depth;
		int color = 0;
		GetScreenState(&x, &y, &depth);
		for (int iI = 0; iI < x; iI += width_length) {
			if (iI % 256 == 0) {
				color = GetColor(255, 0, 0);
			} else {
				color = GetColor(255, 255, 255);
			}
			DrawLine(iI , 0 , iI , y , color);
		}
		for (int iJ = 0; iJ < y; iJ += height_length) {
			if (iJ % 256 == 0) {
				color = GetColor(255, 0, 0);
			} else {
				color = GetColor(255, 255, 255);
			}
			DrawLine(0 , iJ , x , iJ , color);
		}
	}
#endif _VISIBLE_GRID
#endif // _DEBUG

	/**** �t�H���g�`�� ****/
	{
		int cnt = 0;
		int font_size = 24;
		std::vector<tRenderStringWork>::iterator ite;
		for (ite = mDrawStringVector.begin(); ite != mDrawStringVector.end(); ite ++) {
			switch ((*ite).mStringType) {
			case eSTRING_TYPE_DEBUG:
				this->DrawFontString(0, (cnt++ * font_size), (*ite).mString, (*ite).mColor, font_size);
				break;

			case eSTRING_TYPE_NAME:
				this->DrawFontStringCenter(132, 304, (*ite).mString, (*ite).mColor, font_size);
				break;
			default:
				this->DrawFontStringCenter(320, (290 + ((int)(*ite).mStringType * (font_size + 4))), (*ite).mString, (*ite).mColor, font_size);
				break;
			} 
		}
		/* ���R�ɕ�����`�悷�邽�߂̃n���h�� */
		spDrawFontFunc();

		/* ���b�N�I����p������`��n���h�� */
		spLockonFontFunc();
	}

	/* �ǉ��\��̃��f����j������ */
	if (sReserveEntryModel.empty() == false) {
		std::vector<ModelBase *>::iterator ite;
		for (ite = sReserveEntryModel.begin(); ite != sReserveEntryModel.end(); ite ++) {
			GraphicManager::GetInstance()->EntryDrawModel(*ite);
		}
		sReserveEntryModel.clear();	// �S�č폜
	}
	/* �폜�\��̃��f����j������ */
	if (sReserveRemoveModel.empty() == false) {
		std::vector<ModelBase *>::iterator ite;
		for (ite = sReserveRemoveModel.begin(); ite != sReserveRemoveModel.end(); ite ++) {
			GraphicManager::GetInstance()->RemoveDrawModel(*ite);
		}
		sReserveRemoveModel.clear();	// �S�č폜
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::PostUpdate(void)
{
	
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::Destroy(void)
{

}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::Refresh(void)
{
	mDrawStringVector.clear();
	mShapeLineVector.clear();
#ifdef _DEBUG
	mDbgShapeVector.clear();
#endif // _DEBUG
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::EntryDrawModel(ModelBase* pModelBase)
{
	std::vector<ModelBase *>::iterator ite;
	for (ite = mDrawModelVector.begin(); ite != mDrawModelVector.end(); ite ++) {
		if (*ite == pModelBase) {
			return;
		}
	}
#ifdef _DEBUG
	APP_ASSERT( (mIsUpdateDrawModelVector == FALSE),
				"mDrawModelVector is Update Now! \n");

#endif // _DEBUG
	mDrawModelVector.push_back(pModelBase);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::InsertDrawModel(ModelBase* pModelBase, ModelBase* pTargetModelBase)
{
#ifdef _DEBUG
	APP_ASSERT( (mIsUpdateDrawModelVector == FALSE),
				"mDrawModelVector is Update Now! \n");
#endif // _DEBUG

	std::vector<ModelBase *>::iterator ite;
	// ���ɃG���g���[����Ă��Ȃ����`�F�b�N
	for (ite = mDrawModelVector.begin(); ite != mDrawModelVector.end(); ite ++) {
		if (*ite == pModelBase) {
			return;
		}
	}

	for (ite = mDrawModelVector.begin(); ite != mDrawModelVector.end(); ite ++) {
		if (*ite == pTargetModelBase) {
			mDrawModelVector.insert(ite, pModelBase);
			return;
		}
	}

	// ���� pTargetModelBase ���}������Ă��Ȃ��ꍇ�͍Ō���� push_back() ����
	mDrawModelVector.push_back(pModelBase);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::RemoveDrawModel(ModelBase* pModelBase)
{
#ifdef _DEBUG
	APP_ASSERT( (mIsUpdateDrawModelVector == FALSE),
				"mDrawModelVector is Update Now! \n");
#endif // _DEBUG
	if (mDrawModelVector.size() == 0) return;

	std::vector<ModelBase *>::iterator ite;
	for (ite = mDrawModelVector.begin(); ite != mDrawModelVector.end(); ite ++) {
		if (*ite == pModelBase) {
			mDrawModelVector.erase(ite);
			break;
		}
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::RemoveDrawModelAll(void)
{
#ifdef _DEBUG
	APP_ASSERT( (mIsUpdateDrawModelVector == FALSE),
				"mDrawModelVector is Update Now! \n");
#endif // _DEBUG

	mDrawModelVector.clear();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::ReserveEntryModel(ModelBase* pModel)
{
	std::vector<ModelBase *>::iterator ite;
	for (ite = mDrawModelVector.begin(); ite != mDrawModelVector.end(); ite ++) {
		if (*ite == pModel) {
			return;
		}
	}

	sReserveEntryModel.push_back(pModel);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::ReserveRemoveModel(ModelBase* pModel)
{
	std::vector<ModelBase *>::iterator ite;
	for (ite = mDrawModelVector.begin(); ite != mDrawModelVector.end(); ite ++) {
		if (*ite == pModel) {
			sReserveRemoveModel.push_back(pModel);
			break;
		}
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::EntryDrawPreUpdateModel(ModelBase* pModelBase)
{
	std::vector<ModelBase *>::iterator ite;
	for (ite = mDrawPreUpdateModelVector.begin(); ite != mDrawPreUpdateModelVector.end(); ite ++) {
		if (*ite == pModelBase) {
			return;
		}
	}
	mDrawPreUpdateModelVector.push_back(pModelBase);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::RemoveDrawPreUpdateModel(ModelBase* pModelBase)
{
	std::vector<ModelBase *>::iterator ite;
	for (ite = mDrawPreUpdateModelVector.begin(); ite != mDrawPreUpdateModelVector.end(); ite ++) {
		if (*ite == pModelBase) {
			mDrawPreUpdateModelVector.erase(ite);
			break;
		}
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::RemoveDrawPreUpdateModelAll(void)
{
	mDrawPreUpdateModelVector.clear();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::EntryCursorModel(ModelBase* pModelBase)
{
	mpCursorModel = pModelBase;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
BYTE
GraphicManager::IsEntryCursorModel(void)
{
	return (mpCursorModel == NULL) ? FALSE : TRUE;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::EntryAfterDrawModel(ModelBase* pModelBase)
{
	mpAfterEffectModel = pModelBase;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::EntryDrawMapModel(ModelMap* pModelMap)
{
	std::vector<ModelMap *>::iterator ite;
	for (ite = mDrawModelMapVector.begin(); ite != mDrawModelMapVector.end(); ite ++) {
		if (*ite == pModelMap) {
			return;
		}
	}
	mDrawModelMapVector.push_back(pModelMap);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::RemoveDrawMapModel(ModelMap* pModelMap)
{
	std::vector<ModelMap *>::iterator ite;
	for (ite = mDrawModelMapVector.begin(); ite != mDrawModelMapVector.end(); ite ++) {
		if (*ite == pModelMap) {
			mDrawModelMapVector.erase(ite);
			break;
		}
	}
}


//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::RemoveDrawMapModelAll(void)
{
	mDrawModelMapVector.clear();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::EntryDrawLayout(LayoutBase* pLayoutBase)
{
	mDrawLayoutVector.push_back(pLayoutBase);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::InsertDrawLayout(LayoutBase* pLayoutBase, LayoutBase* pTargetLayoutBase)
{
	std::vector<LayoutBase *>::iterator ite;
	// ���ɃG���g���[����Ă��Ȃ����`�F�b�N
	for (ite = mDrawLayoutVector.begin(); ite != mDrawLayoutVector.end(); ite ++) {
		if (*ite == pLayoutBase) {
			return;
		}
	}

	// pTargetLayoutBase ��T�������A���̑O�� insert() ���s��
	for (ite = mDrawLayoutVector.begin(); ite != mDrawLayoutVector.end(); ite ++) {
		if (*ite == pTargetLayoutBase) {
			mDrawLayoutVector.insert(ite, pLayoutBase);
			return;
		}
	}

	// ���� pTargetLayoutBase ���}������Ă��Ȃ��ꍇ�͍Ō���� push_back() ����
	mDrawLayoutVector.push_back(pLayoutBase);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::RemoveDrawLayout(LayoutBase* pLayoutBase)
{
	std::vector<LayoutBase *>::iterator ite;
	for (ite = mDrawLayoutVector.begin(); ite != mDrawLayoutVector.end(); ite ++) {
		if (*ite == pLayoutBase) {
			mDrawLayoutVector.erase(ite);
			break;
		}
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::RemoveDrawLayoutAll(void)
{
	mDrawLayoutVector.clear();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
GraphicManager::CheckEntryDrawLayout(LayoutBase* pLayoutBase)
{
	std::vector<LayoutBase *>::iterator ite;
	for (ite = mDrawLayoutVector.begin(); ite != mDrawLayoutVector.end(); ite ++) {
		if (*ite == pLayoutBase) {
			return true;
		}
	}
	return false;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::EntrySystemLayout(LayoutBase* pLayoutBase, eSystemLayoutPriority lytPrio/*=eSYS_LAYOUT_PRIORITY_LAST*/)
{
	std::vector<LayoutBase *>::iterator ite;

	switch (lytPrio) {
	case eSYS_LAYOUT_PRIORITY_FIRST:
		// �Œ�
		ite = mSystemLayoutVector.begin();
		mSystemLayoutVector.insert(ite, pLayoutBase);
		break;

	case eSYS_LAYOUT_PRIORITY_FADE_BEFORE:
		// �t�F�[�h��艺
		for (ite = mSystemLayoutVector.begin(); ite != mSystemLayoutVector.end(); ite ++) {
			if (*ite == mpFadeLayout) {
				mSystemLayoutVector.insert(ite, pLayoutBase);
				break;
			}
		}
		break;

	case eSYS_LAYOUT_PRIORITY_FADE_AFTER:
		// �t�F�[�h����
		for (ite = mSystemLayoutVector.begin(); ite != mSystemLayoutVector.end(); ite ++) {
			if (*ite == mpFadeLayout) {
				ite ++;
				mSystemLayoutVector.insert(ite, pLayoutBase);
				break;
			}
		}
		break;

	case eSYS_LAYOUT_PRIORITY_LAST:
		// �ō�
		mSystemLayoutVector.push_back(pLayoutBase);
		break;

	default:
		APP_ASSERT(NULL, "Invalid case \n");
		break;
	};
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::RemoveSystemLayout(LayoutBase* pLayoutBase)
{
	if (mSystemLayoutVector.size() == 0) return;

	std::vector<LayoutBase *>::iterator ite;
	for (ite = mSystemLayoutVector.begin(); ite != mSystemLayoutVector.end(); ite ++) {
		if (*ite == pLayoutBase) {
			mSystemLayoutVector.erase(ite);
			break;
		}
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::RemoveSystemLayoutAll(void)
{
	mSystemLayoutVector.clear();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
GraphicManager::CheckEntrySystemLayout(LayoutBase* pLayoutBase)
{
	std::vector<LayoutBase *>::iterator ite;
	for (ite = mSystemLayoutVector.begin(); ite != mSystemLayoutVector.end(); ite ++) {
		if (*ite == pLayoutBase) {
			return true;
		}
	}
	return false;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::AssignFadeboardSystemLayout(LayoutBase* pLayoutBase)
{
	mpFadeLayout = pLayoutBase;
}

#if 0
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::EntryHitModel(ModelBase* pModelBase)
{
	mHitModelVector.push_back(pModelBase);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::RemoveHitModel(ModelBase* pModelBase)
{
	std::vector<ModelBase *>::iterator ite;
	for (ite = mHitModelVector.begin(); ite != mHitModelVector.end(); ite ++) {
		if (*ite == pModelBase) {
			mHitModelVector.erase(ite);
			break;
		}
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::RemoveHitModelAll(void)
{
	mHitModelVector.clear();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
int
GraphicManager::GetHitModelNum(void)
{
	return mHitModelVector.size();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
ModelBase*
GraphicManager::GetHitModelBaseAddr(int modelIndex)
{
	return mHitModelVector.at(modelIndex);
}
#endif /* 0 */

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::EntryDrawShadow(Shadow* pShadow)
{
	mDrawShadowVector.push_back(pShadow);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::RemoveDrawShadow(Shadow* pShadow)
{
	if (mDrawShadowVector.size() == 0) return;

	std::vector<Shadow *>::iterator ite;
	for (ite = mDrawShadowVector.begin(); ite != mDrawShadowVector.end(); ite ++) {
		if (*ite == pShadow) {
			mDrawShadowVector.erase(ite);
			break;
		}
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::RemoveDrawShadowAll(void)
{
	mDrawShadowVector.clear();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
GraphicManager::CheckEntryShadow(Shadow* pShadow)
{
	std::vector<Shadow *>::iterator ite;
	for (ite = mDrawShadowVector.begin(); ite != mDrawShadowVector.end(); ite ++) {
		if (*ite == pShadow) {
			return true;
		}
	}
	return false;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::RenderMessage(char* const CharaName, char* const Message[GraphicManager::kMESSAGE_LINE_NUM], int setColor)
{
	int str_len;
	tRenderStringWork work;
	int color = setColor;

	str_len = strlen(CharaName);
	// �g�p�ł���ő啶����𒴂��Ă���ꍇ
	if (str_len > kDRAW_STRING_LEN - 1) {
		return; // �g�p�s��
	}
	strcpy_s(work.mString, sizeof(work.mString), CharaName);
	work.mString[str_len] = '\0';
	work.mColor = color;
	work.mStringType = eSTRING_TYPE_NAME;

	mDrawStringVector.push_back(work);

	for (int iI = 0; iI < kMESSAGE_LINE_NUM; iI ++) {
		str_len = strlen(Message[iI]);
		// �g�p�ł���ő啶����𒴂��Ă���ꍇ
		if (str_len > kDRAW_STRING_LEN - 1) {
			return; // �g�p�s��
		}
		strcpy_s(work.mString, sizeof(work.mString), Message[iI]);
		work.mString[str_len] = '\0';
		work.mColor = color;
		work.mStringType = eStringType(eSTRING_TYPE_MESSAGE_0 + iI);
		
		mDrawStringVector.push_back(work);
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::RenderString(eStringType type, char* setString, int setColor)
{
	int str_len;
	tRenderStringWork work;

	int color;
	color = setColor;

	str_len = strlen(setString);
	// �g�p�ł���ő啶����𒴂��Ă���ꍇ
	if (str_len > kDRAW_STRING_LEN - 1) {
		return; // �g�p�s��
	}
	strcpy_s(work.mString, sizeof(work.mString), setString);
	work.mString[str_len] = '\0';
	work.mColor = color;
	work.mStringType = type;
		
	mDrawStringVector.push_back(work);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::RenderLine(ShapeLine& rLine)
{
	mShapeLineVector.push_back(rLine);
}

#ifdef _DEBUG
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::DbgRenderString(char *String ,int Color)
{
	if (sIsRenderDbgString == false) return;

	int str_len = strlen(String);
	// �g�p�ł���ő啶����𒴂��Ă���ꍇ
	if (str_len > kDRAW_STRING_LEN - 1) {
		return; // �g�p�s��
	}

	tRenderStringWork work;
	strcpy_s(work.mString, sizeof(work.mString), String);
	work.mString[str_len] = '\0';
	work.mColor = Color;
	work.mStringType = eSTRING_TYPE_DEBUG;

	mDrawStringVector.push_back(work);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::DbgRenderLine(ShapeLine* pLine)
{
	if (sIsRenderDbgGraph == false) return;

	tShapeVector shape;
	shape.mLine.push_back(*pLine);
	mDbgShapeVector.push_back(shape);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::DbgRenderLine(VECTOR* pStart, VECTOR* pEnd)
{
	if (sIsRenderDbgGraph == false) return;

	tShapeVector shape;
	ShapeLine line;
	line.mStart = *pStart;
	line.mEnd	= *pEnd;
	line.mColor = COLOR_YELLOW;

	shape.mLine.push_back(line);
	mDbgShapeVector.push_back(shape);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::DbgRenderLine(VECTOR* pStart, VECTOR* pEnd, int color)
{
	if (sIsRenderDbgGraph == false) return;

	tShapeVector shape;
	ShapeLine line;
	line.mStart = *pStart;
	line.mEnd	= *pEnd;
	line.mColor = color;

	shape.mLine.push_back(line);
	mDbgShapeVector.push_back(shape);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::DbgRenderCapsule(ShapeCapsule* pCapsule)
{
	if (sIsRenderDbgGraph == false) return;

	tShapeVector shape;
	shape.mCapsule.push_back(*pCapsule);
	mDbgShapeVector.push_back(shape);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::DbgRenderBox(ShapeBox* pBox)
{
	if (sIsRenderDbgGraph == false) return;

	tShapeVector shape;
	shape.mBox.push_back(*pBox);
	mDbgShapeVector.push_back(shape);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::DbgRenderBoard(ShapeBoard* pBoard)
{
	if (sIsRenderDbgGraph == false) return;

	tShapeVector shape;
	shape.mBoard.push_back(*pBoard);
	mDbgShapeVector.push_back(shape);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::DbgSetRenderString(bool isRender)
{
	sIsRenderDbgString = isRender;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::DbgSetRenderGraph(bool isRender)
{
	sIsRenderDbgGraph = isRender;
}
#endif // _DEBUG

//---------------------------------------------------
/**
   
 */
//---------------------------------------------------
void
GraphicManager::EnableFogPattern(int setR, int setG, int setB, float setNear, float setFar)
{
	mFogParam.mIsEnable = TRUE;
	mFogParam.mIsActive = TRUE;
	mFogParam.mColR = setR;
	mFogParam.mColG = setG;
	mFogParam.mColB = setB;
	mFogParam.mNear = setNear;
	mFogParam.mFar = setFar;

	SetFogColor(mFogParam.mColR, mFogParam.mColG, mFogParam.mColB);
	SetFogStartEnd(mFogParam.mNear, mFogParam.mFar);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::DisableFogPattern(void)
{
	mFogParam.mIsEnable = FALSE;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::PauseFog(void)
{
	mFogParam.mIsActive = FALSE;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::ResumeFog(void)
{
	mFogParam.mIsActive = TRUE;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::RevertFogParam(float setSec)
{ 
	ChangeFogParam(setSec, &mBackupFogParam);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::ChangeFogParam(float setSec, tFogParam *pFogParam)
{
	if (setSec == 0.0f) return;

	float max_frame = (60.0f * setSec);
	float move_rate = 1.0f / max_frame;

	if (mFogParam.mIsActive == FALSE) {
		if (pFogParam->mIsActive == FALSE) {
			/* ���� �� ���� */
			return;		// �I��

		} else if (pFogParam->mIsActive == TRUE) {
			/* ���� �� �L�� */
			// �t�H�O�������̏ꍇ��Near�̒l��Far�Ɠ����ɐݒ肷��
			mFogParam.mNear = mFogParam.mFar;

			mFogParam.mIsEnable = TRUE;
			mFogParam.mIsActive = TRUE;
		}
	} else if (mFogParam.mIsActive == TRUE)	{
		if (pFogParam->mIsActive == FALSE) {
			/* �L�� �� ���� */
			pFogParam->mNear = pFogParam->mFar;
		}
	}

	mFogChangeWork.mIsActive = true;
	mFogChangeWork.mDiffParam.mIsEnable = pFogParam->mIsEnable;
	mFogChangeWork.mDiffParam.mIsActive = pFogParam->mIsActive;
	mFogChangeWork.mDiffParam.mColR = (int)((float)(pFogParam->mColR - mFogParam.mColR) * move_rate);
	mFogChangeWork.mDiffParam.mColG = (int)((float)(pFogParam->mColG - mFogParam.mColG) * move_rate);
	mFogChangeWork.mDiffParam.mColB = (int)((float)(pFogParam->mColB - mFogParam.mColB) * move_rate);
	mFogChangeWork.mDiffParam.mNear = ((pFogParam->mNear - mFogParam.mNear) * move_rate);
	mFogChangeWork.mDiffParam.mFar = ((pFogParam->mFar - mFogParam.mFar) * move_rate);
	mFogChangeWork.mNowSec		= 0.0f;
	mFogChangeWork.mMaxSec		= setSec;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::SetEnableDirectionLight(bool isEnable)
{
	// �ʏ탉�C�g��L���ɂ���
	SetLightEnable((isEnable == true ? TRUE : FALSE));
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
GraphicManager::GetEnableDirectionLight(void)
{
	return (GetLightEnable() == TRUE ? true : false);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::SetDefaultDirectionLight(void)
{
	int ret;
	static float col_d = 0.8f;
	static float col_s = 0.1f;
	static float col_a = 0.5f;

	COLOR_F col;
	col = GetColorF(col_d, col_d, col_d, col_d);
	ret = SetLightDifColor(col);
	col = GetColorF(col_s, col_s, col_s, col_s);
	ret = SetLightSpcColor(col);
	col = GetColorF(col_a, col_a, col_a, col_a);
	ret = SetLightAmbColor(col);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::SetEnablePLight(bool isEnable, ePLightNo lightIndex/*=ePLIGHT_NO_00*/)
{
	// �|�C���g���C�g�𖳌��ɂ���
	SetLightEnableHandle(mOtherLightHandle[lightIndex], (isEnable == true ? TRUE : FALSE));
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::DisablePLightAll(void)
{
	for (int iI = 0; iI < ePLIGHT_NO_MAX; iI ++) {
		// �|�C���g���C�g�𖳌��ɂ���
		SetLightEnableHandle(mOtherLightHandle[iI], FALSE);
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
GraphicManager::GetEnablePLight(ePLightNo lightIndex/*=ePLIGHT_NO_00*/)
{
	return (GetLightEnableHandle(mOtherLightHandle[lightIndex]) == TRUE ? true : false);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::SetPLightPosition(VECTOR* pPosition, ePLightNo lightIndex/*=ePLIGHT_NO_00*/)
{
	SetLightPositionHandle(mOtherLightHandle[lightIndex], *pPosition);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::SetPLightRangeAtten(float range, float atten0, float atten1, float atten2, ePLightNo lightIndex/*=ePLIGHT_NO_00*/)
{
	SetLightRangeAttenHandle(mOtherLightHandle[lightIndex], range, atten0, atten1, atten2);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::SetPLightDiffuse(float r, float g, float b, float a, ePLightNo lightIndex/*=ePLIGHT_NO_00*/)
{
	SetLightDifColorHandle(mOtherLightHandle[lightIndex], GetColorF(r, g, b, a));
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::SetPLightSpecular(float r, float g, float b, float a, ePLightNo lightIndex/*=ePLIGHT_NO_00*/)
{
	SetLightSpcColorHandle(mOtherLightHandle[lightIndex], GetColorF(r, g, b, a));
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::SetPLightAmbient(float r, float g, float b, float a, ePLightNo lightIndex/*=ePLIGHT_NO_00*/)
{
	SetLightAmbColorHandle(mOtherLightHandle[lightIndex], GetColorF(r, g, b, a));
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::SetPLightDirection(VECTOR* pDir, ePLightNo lightIndex/*=ePLIGHT_NO_00*/)
{
	SetLightDirectionHandle(mOtherLightHandle[lightIndex], *pDir);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::SetPLightAngle(float outAngle, float inAngle, ePLightNo lightIndex/*=ePLIGHT_NO_00*/)
{
	SetLightAngleHandle(mOtherLightHandle[lightIndex], outAngle, inAngle);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::GetPLightPosition(VECTOR* pPosition, ePLightNo lightIndex/*=ePLIGHT_NO_00*/)
{
	*pPosition = GetLightPositionHandle(mOtherLightHandle[lightIndex]);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::GetPLightRangeAtten(float* range, float* atten0, float* atten1, float* atten2, ePLightNo lightIndex/*=ePLIGHT_NO_00*/)
{
	GetLightRangeAttenHandle(mOtherLightHandle[lightIndex], range, atten0, atten1, atten2);	
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::GetPLightDiffuse(COLOR_F* pColor, ePLightNo lightIndex/*=ePLIGHT_NO_00*/)
{
	*pColor = GetLightDifColorHandle(mOtherLightHandle[lightIndex]);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::GetPLightSpecular(COLOR_F* pColor, ePLightNo lightIndex/*=ePLIGHT_NO_00*/)
{
	*pColor = GetLightSpcColorHandle(mOtherLightHandle[lightIndex]);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::GetPLightAmbient(COLOR_F* pColor, ePLightNo lightIndex/*=ePLIGHT_NO_00*/)
{
	*pColor = GetLightAmbColorHandle(mOtherLightHandle[lightIndex]);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::SetEnableOriginalShader(bool isEnable)
{
	mIsEnableOriginalShader = (isEnable == true ? TRUE : FALSE);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
GraphicManager::GetEnableOriginalShader(void)
{
	return (mIsEnableOriginalShader == TRUE ? true : false);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::StartOriginalShader(void)
{
	MV1SetUseOrigShader(mIsEnableOriginalShader);

	SetUseVertexShader(mVShaderHandle);
	SetUsePixelShader(mPShaderHandle);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::EndOriginalShader(void)
{
	MV1SetUseOrigShader(FALSE);

	SetUseVertexShader(-1);
	SetUsePixelShader(-1);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::SetStopModeUpdate(bool isStop)
{
	if (isStop) {
		EffectManager::SetEffectUpdateSpeed(0);
	}
	else {
		EffectManager::SetEffectUpdateSpeed(1);
	}
	mIsStopModeUpdate = isStop; 
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
GraphicManager::GetStopModeUpdate(void)
{
	return mIsStopModeUpdate; 
}

//---------------------------------------------------
/**
 * �t�H���g�̎��R�`��p�֐�
 */
//---------------------------------------------------
void
GraphicManager::EntryDrawFontFunc(FUNC_DRAW_FONT func)
{
	if (func == NULL) {
		spDrawFontFunc = dummyDrawFunc;
	} else {
		spDrawFontFunc = func;
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::EntryLockonFontFunc(FUNC_DRAW_FONT func)
{
	if (func == NULL) {
		spLockonFontFunc = dummyDrawFunc;
	} else {
		spLockonFontFunc = func;
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::DrawFontString(int x ,int y ,char *String ,int Color, int size)
{
	int string_len = strlen(String);

	x = (int)(x * mLayoutScaleRateX);
	y = (int)(y * mLayoutScaleRateY);

	DrawStringToHandle(x, y, String , Color, sFontHandle[sNowFontType]);
}

//---------------------------------------------------
/**
 * X���ɃZ���^�����O���ĕ�����\��
 */
//---------------------------------------------------
void
GraphicManager::DrawFontStringCenter(int x ,int y ,char *String ,int Color, int size)
{
	static const float sFontSizeTable[GraphicManager::eFONT_TYPE_MAX] = {5.5f, 3.2f};

	int string_len = strlen(String);
	float scale_rate_x = mLayoutScaleRateX / mAspectRate;
	float center_pos_x = (float)x * scale_rate_x;
	center_pos_x -= 16.0f;

	x = (int)(center_pos_x - (sFontSizeTable[sNowFontType] * scale_rate_x * (float)string_len));
	y = (int)(y * mLayoutScaleRateY);

	DrawStringToHandle(x, y, String , Color, sFontHandle[sNowFontType]);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void dummyDrawFunc(void)
{

}

/*=====================================*
 * Blur �e�X�g
 *=====================================*/
#if 0
//---------------------------------------------------
/**
 * ��ʁi�o�b�N�o�b�t�@�j�̏���
 */
//---------------------------------------------------
void GraphicsClear(D3DCOLOR color) 
{
	GET_D3DDEV()->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, color, 1, 0 );
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void SetRenderTarget(
	LPDIRECT3DTEXTURE9 texture, 
	LPDIRECT3DSURFACE9 depth_stencil
) {
	LPDIRECT3DSURFACE9 rt;
	if (SUCCEEDED(texture->GetSurfaceLevel(0, &rt))) {
		GET_D3DDEV()->SetRenderTarget(0, rt);
		rt->Release();
		GET_D3DDEV()->SetDepthStencilSurface(depth_stencil);
	}
}

//---------------------------------------------------
/**
 * �����_�����O�^�[�Q�b�g�̏�����
 */
//---------------------------------------------------
void ResetRenderTarget() 
{
	GET_D3DDEV()->SetRenderTarget(0, EffectManager::GetInstance()->GetOrgRenderTarget() );
	GET_D3DDEV()->SetDepthStencilSurface(EffectManager::GetInstance()->GetOrgDepthStencil() );
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
// �e�N�X�`���̕`��
void DrawTexture(
	LPDIRECT3DBASETEXTURE9 texture, 
	float x0, float y0, float x1, float y1, 
	float x2, float y2, float x3, float y3, 
	float u0, float v0, float u1, float v1, 
	D3DCOLOR diffuse
) {
	// �e�N�X�`���������@�̐ݒ�
	GET_D3DDEV()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	GET_D3DDEV()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	GET_D3DDEV()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	GET_D3DDEV()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	GET_D3DDEV()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	GET_D3DDEV()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// �e�N�X�`���̐ݒ�
	GET_D3DDEV()->SetTexture(0, texture);
	
	// �l�p�`�̕`��
	DrawQuad(x0, y0, x1, y1, x2, y2, x3, y3, u0, v0, u1, v1, diffuse);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void DrawTexture(
		LPDIRECT3DBASETEXTURE9 texture, 
		float x0, float y0, float x1, float y1, 
		float u0, float v0, float u1, float v1, 
		D3DCOLOR diffuse
) {
	DrawTexture(
				texture, x0, y0, x1, y0, x0, y1, 
				x1, y1, u0, v0, u1, v1, diffuse);
}


/*=====================================*
 * �u���[�e�X�g�֘A
 *=====================================*/
static LPDIRECT3DTEXTURE9* WorkTexture = NULL;
static LPDIRECT3DSURFACE9* WorkDepthStencil = NULL;
static int Target = 0;
static D3DCOLOR BGColor;
float ObjYaw, EnvYaw, Alpha, Roll, Scale;
int Width, Height;

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::testBlurInit(void)
{
	BGColor = D3DCOLOR_XRGB(128, 128, 128);

	Target = 0;
	ObjYaw = 0;
	EnvYaw = 0;
	Alpha = 1.5f;
	
	Roll = 1.02f;
	Scale = 1.1f;

	// �N���C�A���g�̈�̃T�C�Y���擾
	int depth;
	GetScreenState(&Width, &Height, &depth);

	WorkTexture			= new LPDIRECT3DTEXTURE9[4];
	WorkDepthStencil	= new LPDIRECT3DSURFACE9[4];

	for (int i=0, n=4; i<n; i++) {
		GET_D3DDEV()->CreateTexture(Width, Height, 1, D3DUSAGE_RENDERTARGET, 
									D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &WorkTexture[i], NULL);
		GET_D3DDEV()->CreateDepthStencilSurface
			(Width, Height, D3DFMT_D16, 
			 D3DMULTISAMPLE_NONE, 0, TRUE, &WorkDepthStencil[i], NULL);
	}
}

static BYTE sFlg = FALSE;
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::testBlurPreUpdate(void)
{
	if (sFlg == FALSE) return;

	// �e�N�X�`���O�Ƀ��b�V����`��
	SetRenderTarget(WorkTexture[Target], WorkDepthStencil[Target]);
	GraphicsClear(BGColor);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::testBlurUpdate(void)
{
	if (sFlg == FALSE) return;

	float w = (float)Width;
	float h = (float)Height;

	// �e�N�X�`���P���e�N�X�`���O��ɕ`��
	float 
		r=Roll*D3DX_PI*2, c=cos(r), s=sin(r), 
		dx=((-w*c+h*s)*Scale+w)/2, 
		dy=((-w*s-h*c)*Scale+h)/2;
	DrawTexture(
		WorkTexture[1-Target], 
		dx, dy, w-1-dy, dx, dy, h-1-dx, w-1-dx, h-1-dy, 
		0, 0, 1, 1, D3DCOLOR_ARGB((int)(255*Alpha), 255, 255, 255));

	// �e�N�X�`���O����ʂɕ`��
	ResetRenderTarget();
	GET_D3DDEV()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	DrawTexture(WorkTexture[Target], 0, 0, w, h, 0, 0, 1, 1, D3DXCOLOR(1, 1, 1, 1));
	GET_D3DDEV()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
GraphicManager::testBlurDestroy(void)
{
	for (int iI = 0; iI < 4; iI ++ ){
		SAFE_RELEASE(WorkTexture[iI]);
		SAFE_RELEASE(WorkDepthStencil[iI]);
	}

	delete[] WorkTexture;
	WorkTexture = NULL;

	delete[] WorkDepthStencil;
	WorkDepthStencil = NULL;

}
#endif // 0

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
#ifdef _DEBUG
void
GraphicManager::dumpDrawModelVector(void)
{
	int cnt = 0;

	PRINT_CONSOLE(" * GraphicManager::dumpDrawModelVector() : Start * \n");

	std::vector<ModelBase *>::iterator ite;
	for (ite = mDrawModelVector.begin(); ite != mDrawModelVector.end(); ite ++) {

		PRINT_CONSOLE("* mDrawModelVector[%d] : 0x%08x \n", cnt++, (DWORD)*ite);

	}

	PRINT_CONSOLE(" * GraphicManager::dumpDrawModelVector() : End * \n");

}
#endif // _DEBUG



/**** end of file ****/

