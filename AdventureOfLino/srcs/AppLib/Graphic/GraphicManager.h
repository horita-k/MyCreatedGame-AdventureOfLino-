/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �O���t�B�b�N�����}�l�[�W���N���X
 * @author SPATZ.
 * @data   2011/11/20 23:20:14
 */
//---------------------------------------------------
#ifndef __GRAPHIC_MANAGER_H__
#define __GRAPHIC_MANAGER_H__

#include <vector>
#include "DxLib.h"
#include "ModelBase.h"
#include "ModelMap.h"
#include "LayoutBase.h"
#include "BillboardBase.h"
#include "Shadow.h"
#include "Shape.h"

#ifdef _DEBUG
#define PRINT_SCREEN(color, str, ...)								\
	{																\
		char cB[128] = {0};											\
		sprintf_s(cB, sizeof(cB), str, __VA_ARGS__);				\
		GraphicManager::GetInstance()->DbgRenderString(cB, color);		\
}
#else
#define PRINT_SCREEN(color, str, ... ) // �����
#endif // _DEBUG

// �F��`
#define COLOR_RED						GetColor(255, 0, 0)
#define COLOR_GREEN						GetColor(0, 255, 0)
#define COLOR_BLUE						GetColor(0, 0, 255)
#define COLOR_WHITE						GetColor(255, 255, 255)
#define COLOR_BLACK						GetColor(0, 0, 0)
#define COLOR_YELLOW					GetColor(255, 255, 0)

// �����`��֐�
#define RENDER_LINE(pos, tar, color)	GraphicManager::GetInstance()->DbgRenderLine(pos, tar, color)

static const int kDRAW_STRING_LEN = 64;

typedef		void (*FUNC_DRAW_FONT)(void);

enum ePLightNo {
	ePLIGHT_NO_00 = 0,	// �|�C���g���C�g�@
	eSLIGHT_NO_00,		// �X�|�b�g���C�g�@

	ePLIGHT_NO_01,		// �|�C���g���C�g�A

	
	ePLIGHT_NO_MAX,
};

/*=====================================*
 * class
 *=====================================*/
class GraphicManager
{
 public:
	static const int kMESSAGE_LINE_NUM = 4;

	enum eStringType {
		eSTRING_TYPE_DEBUG = 0,
		eSTRING_TYPE_NAME,
		eSTRING_TYPE_MESSAGE_0,
		eSTRING_TYPE_MESSAGE_1,
		eSTRING_TYPE_MESSAGE_2,
		eSTRING_TYPE_MESSAGE_3,

		eSTRING_TYPE_MAX,
	};

	struct tRenderStringWork {
		char	mString[kDRAW_STRING_LEN];
		int		mColor;
		eStringType		mStringType;
	};
	
	enum eFogPattern {
		eFOG_PATTERN_MIST = 0,
		eFOG_PATTERN_DARK,
		eFOG_PATTERN_WATER,
		eFOG_PATTERN_PURPLE,

		eFOG_PATTERN_MAX,
	};

	struct tFogParam {
		BOOL mIsEnable;
		BOOL mIsActive;
		int mColR;
		int mColG;
		int mColB;
		float mNear;
		float mFar;
	};

	// �t�H���g���
	enum eFontType {
		eFONT_TYPE_DEFAULT = 0,
		eFONT_TYPE_ENDING,

		eFONT_TYPE_MAX,
	};

	// �X�N���[�����
	enum eScreenType {
		eSCREEN_TYPE_FULL = 0,
		eSCREEN_TYPE_HALF,

		eSCREEN_TYPE_MAX,
	};

	// ���C�A�E�g�v���C�I���e�B
	enum eSystemLayoutPriority {
		eSYS_LAYOUT_PRIORITY_FIRST,				// �Œ�
		eSYS_LAYOUT_PRIORITY_FADE_BEFORE,		// �t�F�[�h��艺
		eSYS_LAYOUT_PRIORITY_FADE_AFTER,		// �t�F�[�h����
		eSYS_LAYOUT_PRIORITY_LAST,				// �ō�

		eSYS_LAYOUT_PRIORITY_MAX,
	};

	static GraphicManager* const GetInstance(void);

	void		Create(void);
	void		Setup(void);
	void		Update(void);
	
	void		PostUpdate(void);

	void		Destroy(void);

	void		Refresh(void);
	
	// �t�H���g�f�[�^�̐���
	void		CreateFontData(int fontSize);
	void		CreateEndingFontData(void);
	void		DeleteEndingFontData(void);

	void		SetFontType(GraphicManager::eFontType fontType) ;

	// ���f��
	void		EntryDrawModel(ModelBase* pModelBase);
	void		InsertDrawModel(ModelBase* pModelBase, ModelBase* pTargetModelBase);
	void		RemoveDrawModel(ModelBase* pModelBase);
	void		RemoveDrawModelAll(void);
	void		ReserveEntryModel(ModelBase* pModel);
	void		ReserveRemoveModel(ModelBase* pModel);
	BOOL		IsUpdateDrawModelVector(void)	{ return mIsUpdateDrawModelVector; }

	// ���f��(�}�b�v���f���`��O)
	void		EntryDrawPreUpdateModel(ModelBase* pModelBase);
	void		RemoveDrawPreUpdateModel(ModelBase* pModelBase);
	void		RemoveDrawPreUpdateModelAll(void);

	// �}�b�v
	void		EntryDrawMapModel(ModelMap* pModelMap);
	void		RemoveDrawMapModel(ModelMap* pModelMap);
	void		RemoveDrawMapModelAll(void);

	void		EntryCursorModel(ModelBase* pModelBase);
	BYTE		IsEntryCursorModel(void);

	void		EntryAfterDrawModel(ModelBase* pModelBase);

	// ���C�A�E�g
	void		EntryDrawLayout(LayoutBase* pLayoutBase);
	void		InsertDrawLayout(LayoutBase* pLayoutBase, LayoutBase* pTargetLayoutBase);
	void		RemoveDrawLayout(LayoutBase* pLayoutBase);
	void		RemoveDrawLayoutAll(void);
	bool		CheckEntryDrawLayout(LayoutBase* pLayoutBase);

	// �V�X�e�����C�A�E�g

	void		EntrySystemLayout(LayoutBase* pLayoutBase, eSystemLayoutPriority lytPrio=eSYS_LAYOUT_PRIORITY_LAST);
	void		RemoveSystemLayout(LayoutBase* pLayoutBase);
	void		RemoveSystemLayoutAll(void);
	bool		CheckEntrySystemLayout(LayoutBase* pLayoutBase);
	void		AssignFadeboardSystemLayout(LayoutBase* pLayoutBase);

	// �r���{�[�h
	void		EntryDrawBillboard(BillboardBase* pBillboardBase);
	void		RemoveDrawBillboard(BillboardBase* pBillboardBase);
	void		ReserveRemoveBillboard(BillboardBase* pBillboardBase);

	// �V���h�E
	void		EntryDrawShadow(Shadow* pShadow);
	void		RemoveDrawShadow(Shadow* pShadow);
	void		RemoveDrawShadowAll(void);
	bool		CheckEntryShadow(Shadow* pShadow);

	// �����E���`�`��
	void		RenderMessage(char* const CharaName, char* const Message[kMESSAGE_LINE_NUM], int setColor=0);
	void		RenderString(eStringType type, char* setString, int setColor=-1);
	void		RenderLine(ShapeLine& rLine);
	// �f�o�b�O�p�`��֐�
#ifdef _DEBUG
	void		DbgRenderString(char *String ,int Color);
	//	void		DbgRenderLine(VECTOR* pStart, VECTOR* pEnd, int Color);
	void		DbgRenderLine(ShapeLine* pLine);
	void		DbgRenderLine(VECTOR* pStart, VECTOR* pEnd);
	void		DbgRenderLine(VECTOR* pStart, VECTOR* pEnd, int color);
	void		DbgRenderCapsule(ShapeCapsule* pCapsule);
	void		DbgRenderBox(ShapeBox* pBox);
	void		DbgRenderBoard(ShapeBoard* pBoard);

	void		DbgSetRenderString(bool isRender);
	void		DbgSetRenderGraph(bool isRender);
#endif // _DEBUG

	float		GetLayoutScaleRateX(void)		{ return mLayoutScaleRateX; }
	float		GetLayoutScaleRateY(void)		{ return mLayoutScaleRateY; }

	// �A�X�y�N�g��ɂ��␳�l (Y��1.0�ɌŒ肵���AX�̒l���i�[)
	float		GetAspectRate(void)				{ return mAspectRate; }
	bool		IsWideScreen(void)				{ return (mAspectRate == 0.75f) ? true : false; }
	
	// �t�H�O
	void		EnableFogPattern(eFogPattern pattern);
	void		EnableFogPattern(int setR, int setG, int setB, float setNear, float setFar);
	void		DisableFogPattern(void);
	void		PauseFog(void);
	void		ResumeFog(void);
	void		BackupFogParam(void)			{ mBackupFogParam = mFogParam; }
	void		RevertFogParam(float setSec);
	void		ChangeFogParam(float setSec, tFogParam* pFogParam);

	// ���C�g
	VECTOR&		LightDirection(void)			{ return mLightDirection; }

	void		SetEnableDirectionLight(bool isEnable);
	bool		GetEnableDirectionLight(void);
	void		SetDefaultDirectionLight(void);

	/* �|�C���g���C�g */
	void		SetEnablePLight(bool isEnable, ePLightNo lightIndex=ePLIGHT_NO_00);
	void		DisablePLightAll(void);
	bool		GetEnablePLight(ePLightNo lightIndex=ePLIGHT_NO_00);

	void		SetPLightPosition(VECTOR* pPosition, ePLightNo lightIndex=ePLIGHT_NO_00);
	void		SetPLightRangeAtten(float range, float atten0, float atten1, float atten2, ePLightNo lightIndex=ePLIGHT_NO_00);
	void		SetPLightDiffuse(float r, float g, float b, float a, ePLightNo lightIndex=ePLIGHT_NO_00);
	void		SetPLightSpecular(float r, float g, float b, float a, ePLightNo lightIndex=ePLIGHT_NO_00);
	void		SetPLightAmbient(float r, float g, float b, float a, ePLightNo lightIndex=ePLIGHT_NO_00);

	void		SetPLightDirection(VECTOR* pDir, ePLightNo lightIndex=ePLIGHT_NO_00);
	void		SetPLightAngle(float outAngle, float inAngle, ePLightNo lightIndex=ePLIGHT_NO_00);


	void		GetPLightPosition(VECTOR* pPosition, ePLightNo lightIndex=ePLIGHT_NO_00);
	void		GetPLightRangeAtten(float* range, float* atten0, float* atten1, float* atten2, ePLightNo lightIndex=ePLIGHT_NO_00);
	void		GetPLightDiffuse(COLOR_F* pColor, ePLightNo lightIndex=ePLIGHT_NO_00);
	void		GetPLightSpecular(COLOR_F* pColor, ePLightNo lightIndex=ePLIGHT_NO_00);
	void		GetPLightAmbient(COLOR_F* pColor, ePLightNo lightIndex=ePLIGHT_NO_00);

	// �I���W�i���V�F�[�_�[
	void		SetEnableOriginalShader(bool isEnable);
	bool		GetEnableOriginalShader(void);
	void		StartOriginalShader(void);
	void		EndOriginalShader(void);

	// ��~���[�h
	void		SetStopModeUpdate(bool isStop);
	bool		GetStopModeUpdate(void);

	// �ėp�R�[���o�b�N�֐�
	void		EntryDrawFontFunc(FUNC_DRAW_FONT func);
	void		EntryLockonFontFunc(FUNC_DRAW_FONT func);

	void DrawFontString( int x , int y , char *String , int Color, int size);
	void DrawFontStringCenter( int x , int y , char *String , int Color, int size);

private:
	GraphicManager();
	~GraphicManager();

	void		updateBloomProcess(void);

	void		testBlurInit(void);
	void		testBlurPreUpdate(void);
	void		testBlurUpdate(void);
	void		testBlurDestroy(void);

#ifdef _DEBUG
	void		dumpDrawModelVector(void);
#endif // _DEBUG

	struct tShapeVector
	{
		std::vector<ShapeLine>			mLine;
		std::vector<ShapeCapsule>		mCapsule;
		std::vector<ShapeBox>			mBox;
		std::vector<ShapeBoard>			mBoard;
	};

	// ���f���֘A
	std::vector<ModelBase *>			mDrawModelVector;		// �`�惂�f��
	std::vector<ModelBase *>			mDrawPreUpdateModelVector;	// �`�惂�f��
	std::vector<ModelMap *>				mDrawModelMapVector;	// �`�惂�f��
	ModelBase*							mpCursorModel;
	ModelBase*							mpAfterEffectModel;		// �G�t�F�N�g�`���ɕ`�悳��郂�f��
	BYTE								mIsUpdateDrawModelVector;// mDrawModelVector ���X�V�����ǂ���

	// ���C�A�E�g�֘A
	std::vector<LayoutBase *>			mDrawLayoutVector;		// �`�惌�C�A�E�g
	std::vector<LayoutBase *>			mSystemLayoutVector;	// �V�X�e���p���C�A�E�g
	std::vector<Shadow *>				mDrawShadowVector;		// �`��V���h�E
	std::vector<tRenderStringWork>		mDrawStringVector;		// �`�敶��
	std::vector<ShapeLine>				mShapeLineVector;		// �`��`��(�{�g�p)
#ifdef _DEBUG
	std::vector<tShapeVector>			mDbgShapeVector;		// �`��`��(�f�o�b�O�p)
#endif // _DEBUG
	LayoutBase*							mpFadeLayout;

	float								mLayoutScaleRateX;
	float								mLayoutScaleRateY;
	float								mAspectRate;

	std::vector<ModelBase *>			sReserveEntryModel;
	std::vector<ModelBase *>			sReserveRemoveModel;


	bool								mIsStopModeUpdate;

	// �t�H�O�֘A
	tFogParam							mFogParam;
	tFogParam							mBackupFogParam;

	// ���C�g�֘A
	VECTOR								mLightDirection;
	int									mOtherLightHandle[ePLIGHT_NO_MAX];

	// DxLib�V�F�[�_�[
	int									mVShaderHandle;
	int									mPShaderHandle;
	BYTE								mIsEnableOriginalShader;

	struct tFongChangeWork {
		bool			mIsActive;
		tFogParam		mDiffParam;
		float			mNowSec;
		float			mMaxSec;
	} mFogChangeWork;

};

#endif // __MODEL_PMD_H__

 /**** end of file ****/
