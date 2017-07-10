/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �u���[�������_�����O�N���X
 * @author SPATZ.
 * @data   2015/11/23 12:25:41
 */
//---------------------------------------------------
#ifndef __RENDER_BLOOM_H__
#define __RENDER_BLOOM_H__

#include "ModelBase.h"
#include "ModelMap.h"

/*=====================================*
 * class
 *=====================================*/
class RenderBloom
{
 public:

	// �u���[������
	enum eBloomType {
		eBLOOM_TYPE_BRIGHT_DUMMY = 0,
		eBLOOM_TYPE_BRIGHT_LESS,
		eBLOOM_TYPE_BRIGHT_GREATER,
		eBLOOM_TYPE_HSB_RELATIVE,		// HSB ����
		eBLOOM_TYPE_HSB_ABSOLUTE,		// HSB ���

		eBLOOM_TYPE_MAX,
	};
	struct tBloomBrightParam {
		SHORT	mClipParam;				// �N���b�v�p�����[�^
		BYTE	mPixelColorR;
		BYTE	mPixelColorG;
		BYTE	mPixelColorB;
	};
	struct tBloomHSBParam {
		SHORT	mHue;					// �F��
		SHORT	mSaturation;			// �ʓx
		SHORT	mBright;				// �P�x
	};

	struct tChangeBloomWork {
		BYTE	mIsActive;
		WORD	mBlendValueBase;
		WORD	mBlendValueTarget;
		WORD	mGaussValueBase;
		WORD	mGaussValueTarget;

		float	mNowSec;
		float	mMaxSec;
	};

	static void					Create(void);
	static void					Setup(void);
	static void					ClearScreen(void);
	static void					Update(void);
	static void					Destroy(void);

	// �u���[��
	static void					SetEnableSystemBloom(BOOL isEnable);
	static BOOL					GetEnableSystemBloom(void);
	
	static void					SetActiveBloom(BOOL isActive);
	static void					SetBloomType(eBloomType eType);
	static eBloomType			GetBloomType(void);
	static void					SetBloomParam(BYTE blendMode, WORD blendValue, WORD gaussValue);
	static void					GetBloomParam(BYTE& rBlendMode, WORD& rBlendValue, WORD& rGaussValue);
	static tBloomBrightParam&	BloomBrightParam(void);
	static tBloomHSBParam&		BloomHSBParam(void);
	static void					ChangeBloomParam(float setSec, WORD blendValue, WORD gaussValue);

 private:
	static BOOL		mIsActive;

};

#endif // __RENDER_BLOOM_H__

 /**** end of file ****/

