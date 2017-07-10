/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
#include <math.h>
#include "AppLib/Basic/Basic.h"
#include "AppLib/Graphic/GraphicManager.h"
#include "AppLib/Graphic/ModelMap.h"
#include "AppLib/Collision/CollisionManager.h"
#include "AppLib/Resource/ResourceLayout.h"
#include "Shadow.h"
//#include "Charactor/CharaBase.h"
#include "AppLib/Graphic/ModelCollision.h"
#include "DataDefine/BGDef.h"

#ifdef _DEBUG
#include "AppLib/Basic/Counter.h"
#endif // _DEBUG

/*=====================================*
 * define
 *=====================================*/
/* ���_�f�[�^�ŕω��������������Z�b�g */
#define INIT_SHADOW_VERTEX(vtx)								\
	{														\
		vtx[0].dif = GetColorU8( 255,255,255,255 ) ;		\
		vtx[0].spc = GetColorU8( 0,0,0,0 ) ;				\
		vtx[0].su = 0.0f ;									\
		vtx[0].sv = 0.0f ;									\
		vtx[1] = vtx[0] ;									\
		vtx[2] = vtx[0] ;									\
	} 

static const float kRIPPLE_CREATE_FRAME	= 15.0f;		// �g��𐶐��ł���Ԋu
static const float kRIPPLE_PLAY_FRAME	= 45.0f;		// �g��̍Đ�����(frame)
static const float kRIPPLE_ALPHA_MAX	= 0.35f;		// �g��̃A���t�@(�Z��)�̍ő�l

/*=====================================*

 *=====================================*/
/*static*/
Shadow::UPDATE_FUNC
Shadow::mUpdateFunc[Shadow::eUPDATE_TYPE_MAX] = {
	&Shadow::updateFuncAlways,
	&Shadow::updateFuncOnce,
};

// �V���h�E�}�b�v�L���t���O
/*static*/BYTE Shadow::sEnableShadowMap = FALSE;

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
Shadow::Shadow()
{
	initialize();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
Shadow::~Shadow()
{
	if (mpRippleWork != NULL) {
		delete[] mpRippleWork;
		mpRippleWork = NULL;
	}

	return;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
Shadow::Setup(ModelCollision* pChara, eUpdateType updateType)
{
	mpChara		= pChara;
	mUpdateType	= updateType;

	
	if (mUpdateType == eUPDATE_TYPE_ALWAYS) {
		if (mpRippleWork == NULL) {
			mpRippleWork = new tRippleWork[kRIPPLE_NUM];
			APP_ASSERT_PRINT((mpRippleWork != NULL),
							 "Failed new tRippleWork[%d] \n", kRIPPLE_NUM);
			memset(mpRippleWork, 0, (sizeof(tRippleWork) * kRIPPLE_NUM) );
		}
	}
	else if (mUpdateType == eUPDATE_TYPE_ONCE) {
		/**** �e�����̏��� ****/

	} //<! (mUpdateType == eUPDATE_TYPE_ONCE) {

	// �����œo�^
	if (GraphicManager::GetInstance()->CheckEntryShadow(this) == false) {
		GraphicManager::GetInstance()->EntryDrawShadow(this);
	}

}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
Shadow::EnableShadow(void)
{
	if (GraphicManager::GetInstance()->CheckEntryShadow(this) == false) {
		GraphicManager::GetInstance()->EntryDrawShadow(this);
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
Shadow::DisableShadow(void)
{
	mIsRenderShadow = false;
	GraphicManager::GetInstance()->RemoveDrawShadow(this);
}

//---------------------------------------------------
/**
 * �e�̈ʒu���ړ������� (���W�ړ�)
 */
//---------------------------------------------------
void
Shadow::SetShadowVector(VECTOR moveVec)
{
	tRippleWork* p_work = &mOnceDrawWork;

	for (int iI = 0; iI < kBACKUP_POLYGON_NUM; iI ++) {
		for (int iJ = 0; iJ < 3; iJ ++ ) {
			p_work->mVector[iI][iJ] = VSub(p_work->mVector[iI][iJ], moveVec);
		}
	}
}

//---------------------------------------------------
/**
   
 */
//---------------------------------------------------
void
Shadow::SetupOnceDrawShadow(void)
{
	MV1_COLL_RESULT_POLY_DIM HitResDim = {0};
	MV1_COLL_RESULT_POLY *HitRes ;
	VERTEX3D Vertex[3];
		
	VECTOR chara_pos = mpChara->Position(); // �����l�Ƃ��� mPosition ��ݒ�
	mpChara->GetFramePosFromName(&chara_pos, "�Z���^�[");
	chara_pos.y = mpChara->Position().y;
		
	ShapeCapsule capsule;
	static float sCapsuleHeght = 6.0f;
	// �R���W�����p�̃J�v�Z���p��
	capsule.mRadius = mpChara->BodySize() * 0.8f;
	capsule.mPosA = chara_pos;
	capsule.mPosA.y += capsule.mRadius-sCapsuleHeght;
	capsule.mPosB = capsule.mPosA;
	capsule.mPosB.y += sCapsuleHeght;

	int model_handle = -1;

	//<! NPC�̉e���O�Ԃ̃��f���݂̂̕`��Ƃ���I�i�f���j
	model_handle = CollisionManager::GetInstance()->GetEarthModelHandle(0);
	HitResDim = MV1CollCheck_Capsule(model_handle, eBG_TYPE_EARTH, capsule.mPosA, capsule.mPosB, capsule.mRadius);
	// ���_�f�[�^�ŕω��������������Z�b�g
	INIT_SHADOW_VERTEX(Vertex);
	// ���̒����ɑ��݂���|���S���̐������J��Ԃ�
	HitRes = HitResDim.Dim ;
		
	tRippleWork* p_work = &mOnceDrawWork;
	p_work->mVtxNum = HitResDim.HitNum;
		
	for(int iI = 0 ; iI < HitResDim.HitNum ; iI ++, HitRes ++ ) {
		if (iI >= kBACKUP_POLYGON_NUM) break;
			
		p_work->mVector[iI][0] = HitRes->Position[0];
		p_work->mVector[iI][1] = HitRes->Position[1];
		p_work->mVector[iI][2] = HitRes->Position[2];
		p_work->mNormal[iI] = HitRes->Normal;
	}
	// ���o�����n�ʃ|���S�����̌�n��
	if (HitResDim.HitNum > 0) {
		MV1CollResultPolyDimTerminate( HitResDim );
	}
		
	if (p_work->mVtxNum != 0) {
		p_work->mCharaOldPos = VAdd(mpChara->Position(), mpChara->MoveVec());
		p_work->mCount = (int)kRIPPLE_PLAY_FRAME;
		p_work->mSpeed = 0;
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
Shadow::Update(void)
{
	(this->*mUpdateFunc[mUpdateType])();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
Shadow::UpdateNotRender(void)
{
	if (mUpdateType == eUPDATE_TYPE_ALWAYS) {
		updateFuncAlways();
	}
}

//---------------------------------------------------
/**
 * ����e��`�悷�����
 */
//---------------------------------------------------
void
Shadow::updateFuncAlways(void)
{
	mIsRenderShadow = false;
	mNormalVec = ZERO_VEC;

	MV1_COLL_RESULT_POLY_DIM HitResDim = {0};
	MV1_COLL_RESULT_POLY *HitRes ;
	VERTEX3D Vertex[3] ;

	VECTOR chara_pos = mpChara->Position(); // �����l�Ƃ��� mPosition ��ݒ�
	mpChara->GetFramePosFromName(&chara_pos, "�Z���^�[");
	chara_pos.y = mpChara->Position().y;

	int graph_handle = -1;
	float shadow_size = mpChara->BodySize() * 1.0f; /* �e�̑傫���͈ꗥ����傫������ */
	/*=====================================*
	 *��������L�����̓����蔻��
	 *=====================================*/
	ShapeCapsule capsule;
	static float sCapsuleHeght = 6.0f;
	// �R���W�����p�̃J�v�Z���p��
	capsule.mRadius = mpChara->BodySize() * 0.8f;
	capsule.mPosA = chara_pos;
	capsule.mPosA.y += capsule.mRadius-sCapsuleHeght;
	capsule.mPosB = capsule.mPosA;
	capsule.mPosB.y += sCapsuleHeght;
	capsule.mPosB.y += mShadowHeightOffset;

	// ���C�e�B���O�𖳌��ɂ���
	SetUseLighting( FALSE ) ;

	// �y�o�b�t�@��L���ɂ���
	SetUseZBuffer3D( TRUE ) ;

	// �e�N�X�`���A�h���X���[�h�� CLAMP �ɂ���( �e�N�X�`���̒[����͒[�̃h�b�g�����X���� )
	SetTextureAddressMode( DX_TEXADDRESS_CLAMP ) ;

	/*=====================================*
	 * �n�ʂ̉e�̏����@* eBG_TYPE_EARTH 
	 *=====================================*/
	graph_handle = ResourceLayout::GetInstance()->GetHandle("Shadow.tga");

	int vertex_num = 0;
	int model_handle = -1;
	int frame_index = -1;
	for (int iI = 0; iI < CollisionManager::GetInstance()->GetEarthModelNum(); iI ++) {

		model_handle = CollisionManager::GetInstance()->GetEarthModelHandle(iI);
		frame_index = CollisionManager::GetInstance()->GetEarthModelFrameIndex(iI);

		HitResDim = MV1CollCheck_Capsule(model_handle, frame_index, capsule.mPosA, capsule.mPosB, capsule.mRadius);
		// ���_�f�[�^�ŕω��������������Z�b�g
		INIT_SHADOW_VERTEX(Vertex);
		// ���̒����ɑ��݂���|���S���̐������J��Ԃ�
		HitRes = HitResDim.Dim ;

		/*=====================================*
		 * �V���h�E�}�b�v�����̏ꍇ
		 *=====================================*/
		if (sEnableShadowMap == FALSE) {
			for(int iJ = 0 ; iJ < HitResDim.HitNum ; iJ ++, HitRes ++ ) {
				updateShadowVertexInfo(&Vertex[0], HitRes, shadow_size, &chara_pos);
				// �e�|���S����`��
				DrawPolygon3D( Vertex, 1, graph_handle, TRUE ) ;

				vertex_num ++;
				mNormalVec = VAdd(mNormalVec, HitRes->Normal);
				mIsRenderShadow = true;
			}
		/*=====================================*
		 * �V���h�E�}�b�v�L��̏ꍇ
		 * ���e�̕`�悾���s�킸�A�����蔻��p�̃f�[�^�݂̂��擾����
		 *=====================================*/
		}
		else { // sEnableShadowMap == TRUE
			for(int iJ = 0 ; iJ < HitResDim.HitNum ; iJ ++, HitRes ++ ) {
				updateShadowVertexInfo(&Vertex[0], HitRes, shadow_size, &chara_pos);

				vertex_num ++;
				mNormalVec = VAdd(mNormalVec, HitRes->Normal);
				mIsRenderShadow = true;
			}

		}

		// ���o�����n�ʃ|���S�����̌�n��
		if (HitResDim.HitNum > 0) {
			MV1CollResultPolyDimTerminate( HitResDim ) ;
		}
	}

	mNormalVec = VNorm(VScale(mNormalVec, (1.0f / vertex_num)));

	/*=====================================*
	 * ���ʂ̏����@* eBG_TYPE_WATER
	 *=====================================*/
	/**** ���ʃf�[�^�����̏��� ****/
	model_handle = CollisionManager::GetInstance()->GetEarthModelHandle(0);

	mRippleCreateCount -= 1.0f;
	if (mRippleCreateCount <= 0.0f) {
		mRippleCreateCount = 0.0f;
		float speed = VSize(mpChara->MoveVec());
		if (speed > 1.0f) {

			HitResDim = MV1CollCheck_Capsule(model_handle, eBG_TYPE_WATER, capsule.mPosA, capsule.mPosB, capsule.mRadius);
			// ���_�f�[�^�ŕω��������������Z�b�g
			INIT_SHADOW_VERTEX(Vertex);
			// ���̒����ɑ��݂���|���S���̐������J��Ԃ�
			HitRes = HitResDim.Dim ;

			tRippleWork* p_work = &mpRippleWork[mRippleIndex];
			p_work->mVtxNum = HitResDim.HitNum;

			for(int iI = 0 ; iI < HitResDim.HitNum ; iI ++, HitRes ++ ) {
				if (iI >= kBACKUP_POLYGON_NUM) break;

				p_work->mVector[iI][0] = HitRes->Position[0];
				p_work->mVector[iI][1] = HitRes->Position[1];
				p_work->mVector[iI][2] = HitRes->Position[2];
				p_work->mNormal[iI] = HitRes->Normal;
			}
			// ���o�����n�ʃ|���S�����̌�n��
			if (HitResDim.HitNum > 0) {
				MV1CollResultPolyDimTerminate( HitResDim );
			}

			if (p_work->mVtxNum != 0) {
				p_work->mCharaOldPos = VAdd(mpChara->Position(), mpChara->MoveVec());
				p_work->mCount = (int)kRIPPLE_PLAY_FRAME;
				p_work->mSpeed = speed;
				mRippleIndex = (mRippleIndex + 1) % kRIPPLE_NUM;
				mRippleCreateCount = kRIPPLE_CREATE_FRAME;
			}
		}
	}

	CollisionManager* p_instance = NULL;
	p_instance = CollisionManager::GetInstance();

	/**** ���ʕ`��̏��� ****/
	graph_handle = ResourceLayout::GetInstance()->GetHandle("Ripple.tga");
	tRippleWork* p_work = NULL;
	for (int iI = 0; iI < kRIPPLE_NUM; iI ++) {
		p_work = &mpRippleWork[iI];
		if (p_work->mCount > 0) {

			float rate		= ((kRIPPLE_PLAY_FRAME - (float)p_work->mCount) / kRIPPLE_PLAY_FRAME);
			rate = sqrt(rate);
			BYTE alpha		= BYTE( (kRIPPLE_ALPHA_MAX * sin(rate * PHI_F) ) * 255.0f);
			float setsize	= shadow_size + (rate * (p_work->mSpeed * 8.0f));

			for (int iJ = 0; iJ < p_work->mVtxNum; iJ ++) {

				// �|���S���̍��W�͒n�ʃ|���S���̍��W
				Vertex[0].pos = p_work->mVector[iJ][0];
				Vertex[1].pos = p_work->mVector[iJ][1];
				Vertex[2].pos = p_work->mVector[iJ][2];
				
				// ���ʂ̍������擾���A�����グ�ďd�Ȃ�Ȃ��悤�ɂ���
				ModelMap* p_map = (ModelMap *)p_instance->GetEarthModel();

				float wave_ofs_pos = (p_map->GetWaveHeight() + 0.25f);
				VECTOR SlideVec;
				SlideVec = VScale( p_work->mNormal[iJ], (wave_ofs_pos * ALL_SCALE) ) ;
				Vertex[0].pos = VAdd( Vertex[0].pos, SlideVec ) ;
				Vertex[1].pos = VAdd( Vertex[1].pos, SlideVec ) ;
				Vertex[2].pos = VAdd( Vertex[2].pos, SlideVec ) ;

				Vertex[0].dif.a = alpha;
				Vertex[1].dif.a = alpha;
				Vertex[2].dif.a = alpha;

				Vertex[0].u = ( p_work->mVector[iJ][0].x - p_work->mCharaOldPos.x ) / ( setsize * 2.0f ) + 0.5f ;
				Vertex[0].v = ( p_work->mVector[iJ][0].z - p_work->mCharaOldPos.z ) / ( setsize * 2.0f ) + 0.5f ;
				Vertex[1].u = ( p_work->mVector[iJ][1].x - p_work->mCharaOldPos.x ) / ( setsize * 2.0f ) + 0.5f ;
				Vertex[1].v = ( p_work->mVector[iJ][1].z - p_work->mCharaOldPos.z ) / ( setsize * 2.0f ) + 0.5f ;
				Vertex[2].u = ( p_work->mVector[iJ][2].x - p_work->mCharaOldPos.x ) / ( setsize * 2.0f ) + 0.5f ;
				Vertex[2].v = ( p_work->mVector[iJ][2].z - p_work->mCharaOldPos.z ) / ( setsize * 2.0f ) + 0.5f ;

				DrawPolygon3D(&Vertex[0], 1, graph_handle, TRUE);
			}
			p_work->mCount --;
		}
	}

	// ���C�e�B���O��L���ɂ���
	SetUseLighting( TRUE ) ;

	// �y�o�b�t�@�𖳌��ɂ���
	SetUseZBuffer3D( FALSE ) ;
}

//---------------------------------------------------
/**
   ��x�����`�����
 */
//---------------------------------------------------
void
Shadow::updateFuncOnce(void)
{
	VECTOR chara_pos = mpChara->Position(); // �����l�Ƃ��� mPosition ��ݒ�
	mpChara->GetFramePosFromName(&chara_pos, "�Z���^�[");
	chara_pos.y = mpChara->Position().y;
	float shadow_height = mpChara->BodyHeight();

	int graph_handle = -1;

	// ���C�e�B���O�𖳌��ɂ���
	SetUseLighting( FALSE ) ;
	// �y�o�b�t�@��L���ɂ���
	SetUseZBuffer3D( TRUE ) ;
	// �e�N�X�`���A�h���X���[�h�� CLAMP �ɂ���( �e�N�X�`���̒[����͒[�̃h�b�g�����X���� )
	SetTextureAddressMode( DX_TEXADDRESS_CLAMP ) ;

	graph_handle = ResourceLayout::GetInstance()->GetHandle("Shadow.tga");

	tRippleWork* p_work = &mOnceDrawWork;
	VERTEX3D Vertex[3] ;
	float shadow_size = mpChara->BodySize() * 1.0f; /* �e�̑傫���͈ꗥ����傫������ */

	INIT_SHADOW_VERTEX(Vertex);

	for (int iJ = 0; iJ < p_work->mVtxNum; iJ ++) {
		// �|���S���̍��W�͒n�ʃ|���S���̍��W
		Vertex[0].pos = p_work->mVector[iJ][0];
		Vertex[1].pos = p_work->mVector[iJ][1];
		Vertex[2].pos = p_work->mVector[iJ][2];
				
		// ���ʂ̍������擾���A�����グ�ďd�Ȃ�Ȃ��悤�ɂ���

		VECTOR SlideVec;
		SlideVec = VScale( p_work->mNormal[iJ], (0.6f * ALL_SCALE) ) ;
		Vertex[0].pos = VAdd( Vertex[0].pos, SlideVec ) ;
		Vertex[1].pos = VAdd( Vertex[1].pos, SlideVec ) ;
		Vertex[2].pos = VAdd( Vertex[2].pos, SlideVec ) ;

		Vertex[0].dif.a = 0;
		Vertex[1].dif.a = 0;
		Vertex[2].dif.a = 0;
		if( p_work->mVector[iJ][0].y > chara_pos.y - shadow_height )
			Vertex[ 0 ].dif.a = BYTE(128 * ( 1.0f - fabs( p_work->mVector[iJ][0].y - chara_pos.y ) / shadow_height ));

		if( p_work->mVector[iJ][1].y > chara_pos.y - shadow_height )
			Vertex[ 1 ].dif.a = BYTE(128 * ( 1.0f - fabs( p_work->mVector[iJ][1].y - chara_pos.y ) / shadow_height ));

		if( p_work->mVector[iJ][2].y > chara_pos.y - shadow_height )
			Vertex[ 2 ].dif.a = BYTE(128 * ( 1.0f - fabs( p_work->mVector[iJ][2].y - chara_pos.y ) / shadow_height ));

		// �t�u�l�͒n�ʃ|���S���ƃv���C���[�̑��΍��W���犄��o��
		Vertex[ 0 ].u = ( p_work->mVector[iJ][ 0 ].x - chara_pos.x ) / ( shadow_size * 2.0f ) + 0.5f ;
		Vertex[ 0 ].v = ( p_work->mVector[iJ][ 0 ].z - chara_pos.z ) / ( shadow_size * 2.0f ) + 0.5f ;
		Vertex[ 1 ].u = ( p_work->mVector[iJ][ 1 ].x - chara_pos.x ) / ( shadow_size * 2.0f ) + 0.5f ;
		Vertex[ 1 ].v = ( p_work->mVector[iJ][ 1 ].z - chara_pos.z ) / ( shadow_size * 2.0f ) + 0.5f ;
		Vertex[ 2 ].u = ( p_work->mVector[iJ][ 2 ].x - chara_pos.x ) / ( shadow_size * 2.0f ) + 0.5f ;
		Vertex[ 2 ].v = ( p_work->mVector[iJ][ 2 ].z - chara_pos.z ) / ( shadow_size * 2.0f ) + 0.5f ;

		DrawPolygon3D(&Vertex[0], 1, graph_handle, TRUE);
	}

	// ���C�e�B���O��L���ɂ���
	SetUseLighting( TRUE ) ;
	// �y�o�b�t�@�𖳌��ɂ���
	SetUseZBuffer3D( FALSE ) ;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
Shadow::initialize(void)
{
	mpChara = NULL;
	memset(&mNormalVec, 0, sizeof(mNormalVec));

	mShadowHeightOffset = 0.0f;

	mRippleCreateCount = 0.0f;
	mpRippleWork = NULL;
	mRippleIndex = 0;

	mUpdateType = (eUpdateType)0;

	memset(&mOnceDrawWork, 0, sizeof(mOnceDrawWork));
}

//---------------------------------------------------
/**
 * Vertex �\���̂̍X�V
 */
//---------------------------------------------------
void
Shadow::updateShadowVertexInfo(VERTEX3D* pVertex, MV1_COLL_RESULT_POLY* pHitRes, float shadowSize, VECTOR* pCharaPos)
{
	VECTOR SlideVec;
	VECTOR chara_pos = *pCharaPos;
	chara_pos.y += mShadowHeightOffset;
	float shadow_height = mpChara->BodyHeight();

	// �|���S���̍��W�͒n�ʃ|���S���̍��W
	pVertex[ 0 ].pos = pHitRes->Position[ 0 ] ;
	pVertex[ 1 ].pos = pHitRes->Position[ 1 ] ;
	pVertex[ 2 ].pos = pHitRes->Position[ 2 ] ;

	// ������Ǝ����グ�ďd�Ȃ�Ȃ��悤�ɂ���
	//	SlideVec = VScale( pHitRes->Normal, (0.5f * ALL_SCALE) ) ;
	SlideVec = VScale( pHitRes->Normal, (0.6f * ALL_SCALE) ) ;
	pVertex[ 0 ].pos = VAdd( pVertex[ 0 ].pos, SlideVec ) ;
	pVertex[ 1 ].pos = VAdd( pVertex[ 1 ].pos, SlideVec ) ;
	pVertex[ 2 ].pos = VAdd( pVertex[ 2 ].pos, SlideVec ) ;

	// �|���S���̕s�����x��ݒ肷��
	pVertex[ 0 ].dif.a = 0 ;
	pVertex[ 1 ].dif.a = 0 ;
	pVertex[ 2 ].dif.a = 0 ;
	if( pHitRes->Position[ 0 ].y > chara_pos.y - shadow_height )
		pVertex[ 0 ].dif.a = BYTE(128 * ( 1.0f - fabs( pHitRes->Position[ 0 ].y - chara_pos.y ) / shadow_height ));

	if( pHitRes->Position[ 1 ].y > chara_pos.y - shadow_height )
		pVertex[ 1 ].dif.a = BYTE(128 * ( 1.0f - fabs( pHitRes->Position[ 1 ].y - chara_pos.y ) / shadow_height ));

	if( pHitRes->Position[ 2 ].y > chara_pos.y - shadow_height )
		pVertex[ 2 ].dif.a = BYTE(128 * ( 1.0f - fabs( pHitRes->Position[ 2 ].y - chara_pos.y ) / shadow_height ));

	// �t�u�l�͒n�ʃ|���S���ƃv���C���[�̑��΍��W���犄��o��
	pVertex[ 0 ].u = ( pHitRes->Position[ 0 ].x - chara_pos.x ) / ( shadowSize * 2.0f ) + 0.5f ;
	pVertex[ 0 ].v = ( pHitRes->Position[ 0 ].z - chara_pos.z ) / ( shadowSize * 2.0f ) + 0.5f ;
	pVertex[ 1 ].u = ( pHitRes->Position[ 1 ].x - chara_pos.x ) / ( shadowSize * 2.0f ) + 0.5f ;
	pVertex[ 1 ].v = ( pHitRes->Position[ 1 ].z - chara_pos.z ) / ( shadowSize * 2.0f ) + 0.5f ;
	pVertex[ 2 ].u = ( pHitRes->Position[ 2 ].x - chara_pos.x ) / ( shadowSize * 2.0f ) + 0.5f ;
	pVertex[ 2 ].v = ( pHitRes->Position[ 2 ].z - chara_pos.z ) / ( shadowSize * 2.0f ) + 0.5f ;
}

 /**** end of file ****/
