/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �G�t�F�N�g�m�[�h�N���X
 * @author SPATZ.
 * @data   2014/01/28 23:12:38
 */
//---------------------------------------------------
#ifndef __EFFECT_NODE_H__
#define __EFFECT_NODE_H__

#include <d3dx9.h>
#include "DxLib.h"

#include "../Graphic/EffectType.h"
#include "EffectResource.h"

//const int kEFFECT_NODE_NUM = 16;

/*=====================================*
 * class
 *=====================================*/
class EffectNode 
{
public:

	enum eMatrixType {
		eMATRIX_TYPE_TRS,
		eMATRIX_TYPE_MATRIX,

		eMATRIX_TYPE_MAX,
	};

	// �ݒ肷��Z�}���e�B�N���
	struct tSemanticSetInfo {
		D3DXMATRIX world_mtx;
		D3DXMATRIX view_mtx;
		D3DXMATRIX proj_mtx;
	};

	/* �֐� */
	EffectNode();
	~EffectNode();

	void		Create(void);
	void		Setup(void);
	void		Update(void);
	void		Destroy(void);

	VECTOR&		Position(void)			{ return mPosition; }
	VECTOR&		Rotation(void)			{ return mRotation; }
	VECTOR&		Scale(void)				{ return mScale; }	
	MATRIX&		Matrix(void)			{ return mCalcedMtx; }

	void		SetMatrixType(eMatrixType type)	{ mMatrixType = type; }

	void		Play(EffectResource* pResource, float fadeSec=0.5f);
	void		Stop(float fadeSec=0.5f);
	bool		IsPlay(void)			{ return mIsPlay; }
	void		SetLonevity(const char* pName);

	void		SetAlpha(float fAlpha);
	float		GetAlpha(void);

	void		SetVec3(const char* pName, const VECTOR* pVec3);
	void		SetVec4(const char* pName, const FLOAT4* pVec4);
	void		SetFloat(const char* pName, const float fValue);
	void		SetFloat2(const char* pName, const float* pValue);
	void		SetMatrix(const char* pName, const MATRIX* pMatrix);

	void		GetFloat(const char* pName, float* pValue);
	void		GetVec4(const char* pName, FLOAT4* pVec4);


 private:

	bool		exec(void);
	bool		calc(void);
	typedef		bool (EffectNode::*DRAW_FUNC)(void);
	DRAW_FUNC	mDrawFunc;

	bool		drawFuncNormal(void);
	bool		drawFuncScript(void);
	void		deleteEffect(void);
	void		renderPassModel(int passCnt);
	
	/**** �Z�}���e�B�b�N�֐��錾 ****/	
	/*=====================================*
	 * function
	 *=====================================*/
    void    funcSmt_TIME(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_WORLD(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_VIEW(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_VIEWINVERSE(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_VIEWPROJECTION(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_VIEWPROJECTIONINVERSE(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_PROJECTION(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_PROJECTIONINVERSE(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_WORLDVIEWPROJECTION(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_WORLDVIEWPROJECTIONINVERSE(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_WORLDINVERSETRANSPOSE(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_WORLDVIEW(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_VIEWINVERSETRANSPOSE(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_WORLDINVERSE(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_WORLDVIEWTRANSPOSE(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_WORLDVIEWINVERSE(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_VIEWTRANSPOSE(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_POSITION(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_DIRECTION(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_MODELDATA(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_DIFFUSE(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_AMBIENT(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_EMISSIVE(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_SPECULAR(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_SPECULARPOWER(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_TOONCOLOR(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_EDGECOLOR(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_GROUNDSHADOWCOLOR(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_ELAPSEDTIME(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_CONTROLOBJECT(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_RENDERCOLORTARGET(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_RENDERDEPTHSTENCILTARGET(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_TEXTUREVALUE(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_STANDARDSGLOBAL(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_OFFSCREENRENDERTARGET(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_VIEWPORTPIXELSIZE(int semIndex, tSemanticSetInfo* pInfo);
    void    funcSmt_DUMMY(int semIndex, tSemanticSetInfo* pInfo);

	typedef		void (EffectNode::*SMT_FUNC)(int semIndex, tSemanticSetInfo* pInfo);
	static		SMT_FUNC mSmtFunc[eSMT_MAX];

	/**** �X�N���v�g�֐��錾 ****/	
	/*=====================================*
	 * function
	 *=====================================*/
    void    funcScript_RenderColorTarget(char* pParamString);		    // 0 : �����_�����O�^�[�Q�b�g��ݒ肵�܂��B
    void    funcScript_RenderColorTarget0(char* pParamString);          // 1 : �����_�����O�^�[�Q�b�g��ݒ肵�܂��B
    void    funcScript_RenderColorTarget1(char* pParamString);          // 2 : �����_�����O�^�[�Q�b�g��ݒ肵�܂��B
    void    funcScript_RenderColorTarget2(char* pParamString);          // 3 : �����_�����O�^�[�Q�b�g��ݒ肵�܂��B
    void    funcScript_RenderColorTarget3(char* pParamString);          // 4 : �����_�����O�^�[�Q�b�g��ݒ肵�܂��B
    void    funcScript_RenderDepthStencilTarget(char* pParamString);    // 5 : �[�x�X�e���V���T�[�t�F�C�X��ݒ肵�܂��B
    void    funcScript_ClearSetColor(char* pParamString);               // 6 : �����_�����O�^�[�Q�b�g���N���A����F��ݒ肷��B
    void    funcScript_ClearSetDepth(char* pParamString);               // 7 : �[�x�X�e���V���T�[�t�F�C�X���N���A����Z�l��ݒ肷��B
    void    funcScript_Clear(char* pParamString);                       // 8 : (=Color):�����_�����O�^�[�Q�b�g���N���A���܂��B, (=Depth):�[�x�X�e���V���T�[�t�F�C�X���N���A���܂��B
    void    funcScript_ScriptExternal(char* pParamString);              // 9 : ���̃I�u�W�F�N�g��`�悵�܂��B
    void    funcScript_Pass(char* pParamString);                        // 10 : �w�肵���p�X�����s���܂�
    void    funcScript_LoopByCount(char* pParamString);                 // 11 : �w�肵���񐔂����A�X�N���v�g�̈ꕔ�����[�v���܂��B
    void    funcScript_LoopEnd(char* pParamString);                     // 12 : �w�肵���񐔂����A�X�N���v�g�̈ꕔ�����[�v���܂��B
    void    funcScript_LoopGetIndex(char* pParamString);                // 13 : ���[�v���̃��[�v�J�E���^�̒l���A�w�肵���p�����[�^�ɐݒ肵�܂��B
    void    funcScript_Draw(char* pParamString);                        // 14 : (=Geometory):�I�u�W�F�N�g��`�悵�܂��B (=Buffer):�����_�����O�^�[�Q�b�g�̃X�N���[���ƈ�v����T�C�Y�́A�����`�̃|���S����`�悵�܂��B

	typedef		void (EffectNode::*SCRIPT_FUNC)(char* pParamString);
	
 protected:
	/* �ϐ� */
	VECTOR		mPosition;
	VECTOR		mRotation;
	VECTOR		mScale;
	MATRIX		mCalcedMtx;

	eMatrixType	mMatrixType;

	bool		mIsPlay;

	struct tLifetimeWork {
		BYTE	mIsActive;
		char	mName[32];
		DWORD	mCount;
	} mLifetime;

	struct tAlphaWork {
		float	mAlpha;
	} mAlphaWork;

	struct tFadeWork {
		BYTE	mIsActive;		
		float	mSpeed;
	} mFade;

	ID3DXEffect*	mpEffect;
	EffectResource*	mpResourceRef;
	BYTE			mSemCount;
	D3DXHANDLE		mSemHandle[eSMT_MAX];

	USHORT			mBufferRectWidth;
	USHORT			mBufferRectHeight;
	BYTE			mStencilHandle;

};

void DrawQuad(
			  float x0, float y0, float x1, float y1, 
			  float x2, float y2, float x3, float y3, 
			  float u0, float v0, float u1, float v1, 
			  D3DCOLOR diffuse);

void DrawRect(float x0, float y0, float x1, float y1, 
			  float u0, float v0, float u1, float v1, D3DCOLOR diffuse);

#endif // __EFFECT_NODE_H__

/**** end of file ****/
