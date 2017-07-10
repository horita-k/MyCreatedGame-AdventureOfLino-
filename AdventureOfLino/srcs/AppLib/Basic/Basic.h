/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * �x�[�X�@�\
 * @author SPATZ.
 * @data   2011/12/03 14:59:27	
 */
//---------------------------------------------------
#ifndef __BASIC_H__
#define __BASIC_H__

#include <windows.h>
#include <d3dx9.h>
#include "dxlib.h"

/*=====================================*
 * struct �錾
 *=====================================*/
struct D3DXMATRIX;

/*=====================================*
 * �f�o�b�O�֘A
 *=====================================*/
#ifdef _DEBUG /* ------------- Debug -----------------*/
#define PRINT_CONSOLE( str, ... )						\
	{													\
        char cA[256];									\
        sprintf_s( cA, sizeof(cA), str, __VA_ARGS__ );	\
        OutputDebugString( cA );						\
	}

#define PRINT_ASSERT(exp, str, ...)						\
	{													\
		if (!(exp)) {									\
			PRINT_CONSOLE("====HoliAssert====");		\
			char cA[256];								\
			sprintf_s( cA, sizeof(cA), str, __VA_ARGS__ );	\
			OutputDebugString( cA );						\
		}												\
	}
#define PRINT_LOG(str)									\
	{													\
		char mseg[256] = {0};							\
		DETAIL_SPRINTF(mseg, str)						\
		Basic::WriteDebugLog(mseg);						\
	}													
#define PRINT_LOG_PRINT(str, ...)						\
	{													\
		char mseg[256] = {0};							\
		char tmp[256] = {0};							\
		sprintf_s(tmp, sizeof(tmp), str, __VA_ARGS__);	\
		DETAIL_SPRINTF(mseg, tmp);						\
		Basic::WriteDebugLog(mseg);						\
	}													

#define PRINT_TRACE()	PRINT_CONSOLE("%s [%d] \n", __FILE__, __LINE__)

// Int �^�\���}�N��
#define PRINT_CONSOLE_INT(v)					\
	PRINT_CONSOLE("%s = %d\n", #v, v);

// Float �^�\���}�N��
#define PRINT_CONSOLE_FLT(f)					\
	PRINT_CONSOLE("%s = %.2f\n", #f, f);

// �}�g���N�X�\���}�N��
#define PRINT_CONSOLE_MTX(mtx)											\
	PRINT_CONSOLE("%s = \n", #mtx);										\
	PRINT_CONSOLE(" (%2.2f,\t  %2.2f,\t  %2.2f,\t  %2.2f) \n", mtx.m[0][0], mtx.m[0][1], mtx.m[0][2], mtx.m[0][3]); \
	PRINT_CONSOLE(" (%2.2f,\t  %2.2f,\t  %2.2f,\t  %2.2f) \n", mtx.m[1][0], mtx.m[1][1], mtx.m[1][2], mtx.m[1][3]); \
	PRINT_CONSOLE(" (%2.2f,\t  %2.2f,\t  %2.2f,\t  %2.2f) \n", mtx.m[2][0], mtx.m[2][1], mtx.m[2][2], mtx.m[2][3]); \
	PRINT_CONSOLE(" (%2.2f,\t  %2.2f,\t  %2.2f,\t  %2.2f) \n", mtx.m[3][0], mtx.m[3][1], mtx.m[3][2], mtx.m[3][3]); \

// �x�N�g���\���}�N��
#define PRINT_CONSOLE_VEC(vec)											\
	PRINT_CONSOLE("%s = (%2.2f, %2.2f, %2.2f) \n", #vec, vec.x, vec.y, vec.z);

// �������m�ۗp���O
#define ALLOCATE_LOG(ptr)	PRINT_CONSOLE("allocate log, %08x, %5d, %s \n", (DWORD)ptr, __LINE__, __FUNCTION__);

#else // _DEBUG /* ------------- Release -----------------*/
#define PRINT_CONSOLE( str, ... )		// �����
#define PRINT_ASSERT( str, ...)			// �����

#define PRINT_LOG( str )				// �����
#define PRINT_LOG_PRINT( str, ...)		// �����
#define PRINT_TRACE()					// �����
#define PRINT_CONSOLE_MTX(mtx)			// �����
#define ALLOC_LOG(ptr)					// �����

#endif // _DEBUG

#define DETAIL_SPRINTF(out, str)										\
	{																	\
		sprintf_s(out, "%s(L:%d)\n%s \n", __FILE__, __LINE__, str);		\
	}

#if defined(_DEBUG)
#define APP_ASSERT(exp, str)					\
	{											\
		if (!(exp)) {							\
			char mesg[256] = {0};				\
			DETAIL_SPRINTF(mesg, str);			\
			Basic::HoliAssert(#exp, mesg);		\
		}										\
	}

#define APP_ASSERT_PRINT(exp, str, ...)			\
	{											\
		if(!(exp)) {							\
			char mesg[256] = {0};				\
			char tmp[256] = {0};				\
			sprintf_s(tmp, sizeof(tmp), str, __VA_ARGS__);	\
			DETAIL_SPRINTF(mesg, tmp);			\
			Basic::HoliAssert(#exp, mesg);		\
		}										\
	}

#define APP_STATIC_ASSERT(exp, str)				static_assert(exp, str)


#else // #if defined(_DEBUG)
#define APP_ASSERT(exp, str)			// �����
#define APP_ASSERT_PRINT(ext, str, ...)	// �����
#define APP_STATIC_ASSERT(exp, str)		// �����
#endif // #if defined(_DEBUG)

/*=====================================*
 * ���b�Z�[�W��ʕ\���֘A
 *=====================================*/
/* �V�X�e���G���[��ʕ\�� */
#define	SYSTEM_ERROR(exp, str, ...)								\
	{															\
		if(!(exp)) {											\
			char mesg[256] = {0};								\
			char tmp[256] = {0};								\
			sprintf_s(tmp, sizeof(tmp), str, __VA_ARGS__);		\
			DETAIL_SPRINTF(mesg, tmp);							\
			Basic::SystemError(#exp, mesg);						\
			return;												\
		}														\
	}

// �V�X�e�����b�Z�[�W
#define SYSTEM_MESSAGE(header_msg, explain_msg)			Basic::SystemMessage(header_msg, explain_msg)
// �V�X�e�����b�Z�[�W
#define SYSTEM_MESSAGE_ANS(header_msg, explain_msg)		Basic::SystemMessageAnswer(header_msg, explain_msg)

/*=====================================*
 * DirectX�֘A
 *=====================================*/
/* DIRECT3DCREATE9 ���擾����}�N�� */
#define GET_D3DDEV()					((LPDIRECT3DDEVICE9)GetUseDirect3DDevice9())

/* DirectX�֘A�j���}�N�� */
#define SAFE_RELEASE(p)					{ if(p) { (p)->Release(); (p)=NULL; } }
#define SAFE_DELETE(p)					{ if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p)			{ if(p) { delete[] (p);   (p)=NULL; } }

/* �t�@�C���j���}�N�� */
#define SAFE_FILE_CLOSE(p)				{ if(p!=NULL) { fclose(p); fp = NULL; } }

/*=====================================*
 * �^�ϊ��֘A
 *=====================================*/
/* VECTOR ���� FLOAT4 ��ݒ肷��		*/
#define SET_FLOAT4(p, vec)				{p->x = vec.x; p->y = vec.y; p->z = vec.z; p->w = 1.0f;}

void ConvertMtx(D3DXMATRIX* pOut, const MATRIX* pIn);

/* �J���[���� 255 �� 1.0 �̕ϊ��}�N�� */
#define COL_BYTE_2_FLOAT(bytevalue)		((float)bytevalue / 255.0f)
#define COL_FLOAT_2_BYTE(floatvalue)	((BYTE)floatvalue * 255.0f)

/* enum �� bit �ϊ��}�N�� */
#define ENUM_2_BIT(enumV)				(1 << enumV)

#define GET_MIDDLE_POS(posa, posb)		(VScale(VAdd(posa, posb), 0.5f))

/*=====================================*
 * �֗��}�N��
 *=====================================*/
/* ���[�e�[�V��������C���f�b�N�X�̉��Z�E���Z */
#define SELECT_ROTATION_ADD(index, max)	((index + 1 > max - 1) ? 0 : index + 1)
#define SELECT_ROTATION_SUB(index, max)	((index - 1 < 0) ? max - 1 : index - 1)

#define EQUAL_FRAME(chkframe, nowframe, addvalue)	(((chkframe >= nowframe) && (chkframe < nowframe + addvalue)) ? true : false)
#define BETWEEN_FRAME(minframe, maxframe, nowframe)	((((minframe) <= nowframe) && (maxframe > nowframe) ) ? true : false)

/*=====================================*
 * ������֘A
 *=====================================*/
/* �������r�}�N�� */
#define EQUAL_STRING(stra, strb)		(strcmp(stra, strb) == 0)

/*=====================================*
 * �萔��`
 *=====================================*/
#define DXLIB_OK		(0)
#define DXLIB_ERROR		(-1)

/*=====================================*
 * �^��`
 *=====================================*/
typedef char			CHAR;
typedef short			SHORT;
typedef long			LONG;
typedef unsigned char	UCHAR;
typedef unsigned short	USHORT;
typedef unsigned long	ULONG;
typedef	BYTE			Bool;
static const VECTOR		ZERO_VEC = VGet(0.0f, 0.0f, 0.0f);
static const VECTOR		UP_VEC	 = VGet(0.0f, 1.0f, 0.0f);
static const VECTOR		CHARA_DEF_SCALE = VGet(10.0f, 10.0f, 10.0f);

typedef void (*CALLBACK_BASIC)(void *);

/* �f�o�b�O�c�[���g�p���l */
#define DBGTOOLVALUE00		Basic::sDbgToolValue[0]
#define DBGTOOLVALUE01		Basic::sDbgToolValue[1]
#define DBGTOOLVALUE02		Basic::sDbgToolValue[2]
#define DBGTOOLVALUE03		Basic::sDbgToolValue[3]
#define DBGTOOLVALUE04		Basic::sDbgToolValue[4]
#define DBGTOOLVALUE05		Basic::sDbgToolValue[5]
#define DBGTOOLVALUE06		Basic::sDbgToolValue[6]
#define DBGTOOLVALUE07		Basic::sDbgToolValue[7]
#define DBGTOOLVALUE08		Basic::sDbgToolValue[8]
#define DBGTOOLVALUE09		Basic::sDbgToolValue[9]
#define DBGTOOLVALUE10		Basic::sDbgToolValue[10]
#define DBGTOOLVALUE11		Basic::sDbgToolValue[11]

/*=====================================*
 * struct
 *=====================================*/
struct Vec2 {
	int		x;
	int		y;
	Vec2::Vec2(void)
	{
	}

	Vec2::Vec2(int sx, int sy) {
		this->x = sx;
		this->y = sy;
	}

	Vec2 operator=(const Vec2 in) {
		this->x = in.x;
		this->y = in.y;
		return *this;
    }

	Vec2& Vec2::operator+=(const Vec2& in) {
		this->x += in.x;
		this->y += in.y;
		return *this;
	}
};

/*=====================================*
 * class
 *=====================================*/
class Basic 
{
 public:
	static void OpenDebugLog(void);
	static void CloseDebugLog(void);
	static void WriteDebugLog(char* str);
	static void FlushDebugLog(void);

	static void	QuickSort(int* pArray, int beginIndex, int endIndex);
	static void	QuickSortEx(int* pArray, int* pArrayIndex, int beginIndex, int endIndex);

	static void HoliAssert(char* method, char* str);
	static void SystemError(char* method, char* str);
	static void SystemMessage(char* header_msg, char* explain_msg);
	static bool SystemMessageAnswer(char* header_msg, char* explain_msg);

	static BOOL CreateProcessSync(char* szFileName);

	static int	IndexOf(const char* str, const char keyword);
	static int	LastIndexOf(const char* str, const char keyword);

	static void	ShowDump(unsigned char* addr, unsigned int size);

	static void DummyCallback(void *) {}

public:
#ifdef _DEBUG
	static float sDbgToolValue[12];
#endif // _DEBUG

};

#endif // __BASIC_H__

 /**** end of file ****/

