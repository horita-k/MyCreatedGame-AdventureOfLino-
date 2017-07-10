
//---------------------------------------------------
/**
 * ��{�����֐�
 * @author SPATZ.
 * @data   2011/12/10 19:09:47	
 */
//---------------------------------------------------
#include <d3dx9.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "Basic.h"

#include "AppLib/Input/InputManager.h"

#ifdef _DEBUG
#include "External/PIC/PIC.h"
#endif // _DEBUG

#ifdef _DEBUG
/*static*/ float Basic::sDbgToolValue[12] = {0};
#endif // _DEBUG

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void ConvertMtx(D3DXMATRIX* pOut, const MATRIX* pIn)
{
	pOut->_11 = pIn->m[0][0];
	pOut->_12 = pIn->m[0][1];
	pOut->_13 = pIn->m[0][2];
	pOut->_14 = pIn->m[0][3];

	pOut->_21 = pIn->m[1][0];
	pOut->_22 = pIn->m[1][1];
	pOut->_23 = pIn->m[1][2];
	pOut->_24 = pIn->m[1][3];

	pOut->_31 = pIn->m[2][0];
	pOut->_32 = pIn->m[2][1];
	pOut->_33 = pIn->m[2][2];
	pOut->_34 = pIn->m[2][3];

	pOut->_41 = pIn->m[3][0];
	pOut->_42 = pIn->m[3][1];
	pOut->_43 = pIn->m[3][2];
	pOut->_44 = pIn->m[3][3];
}

static const char* strLogFileName = "DebugLog.txt";
static FILE* sFp = NULL;

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
/*static*/void
Basic::OpenDebugLog(void)
{
	fopen_s(&sFp, strLogFileName, "a");
	APP_ASSERT_PRINT(sFp != NULL,
					 "Failed fopen() : %s \n", strLogFileName);

   time_t timer;

   /* ���ݎ����̎擾 */
   time(&timer);

   /* ���ݎ����𕶎���ɕϊ����ĕ\�� */
   Basic::WriteDebugLog("********************************\n");
   Basic::WriteDebugLog(ctime(&timer));
   Basic::WriteDebugLog("********************************\n");
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
/*static*/void
Basic::CloseDebugLog(void)
{
	if (sFp != NULL) {
		fclose(sFp);
		sFp = NULL;
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
/*static*/void
Basic::WriteDebugLog(char* str)
{
	APP_ASSERT(sFp != NULL,
			   "sFp is NULL\n");
	APP_ASSERT(str != NULL,
			   "Setting str is NULL\n");
	fputs(str, sFp);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
/*static*/void
Basic::FlushDebugLog(void)
{
	fflush(sFp);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
/*static*/void
Basic::QuickSort(int* pArray, int beginIndex, int endIndex)
{
	int i, j;
	int pivot;   /* ���� */
	int work;
		
	pivot = pArray[ ( beginIndex + endIndex ) / 2 ];  /* �͈͂̒����ɂ���v�f�𐕎��Ƃ��� */
	i = beginIndex;
	j = endIndex;
	
	while( 1 )
	{
		while( pArray[i] < pivot ){ ++i; }  /* �����ȏ�̒l��������܂ŉE�����֐i�߂Ă��� */
		while( pArray[j] > pivot ){ --j; }  /* �����ȉ��̒l��������܂ō������֐i�߂Ă��� */
		if( i >= j ){ break; }  /* ���E����i�߂Ă���i��j���Ԃ������烋�[�v���I���� */
		
		/* �v�f���������� */
		work = pArray[i];
		pArray[i] = pArray[j];
		pArray[j] = work;
		
		/* ����ɔ����āA���ړ_�����炷 */
		i++;
		j--;
	}
	
	/* �����̍����ɗv�f���Q�ȏ゠��΁A���̕����ɂ��ăN�C�b�N�\�[�g���ċA������ */
	if( beginIndex < i - 1 ){ QuickSort( pArray, beginIndex, i - 1 ); }
	/* �����̉E���ɗv�f���Q�ȏ゠��΁A���̕����ɂ��ăN�C�b�N�\�[�g���ċA������ */
	if( j + 1 < endIndex ){ QuickSort( pArray, j + 1, endIndex ); }
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
/*static*/void
Basic::QuickSortEx(int* pArray, int* pArrayIndex, int beginIndex, int endIndex)
{
	int i, j;
	int pivot;   /* ���� */
	int work;
	int work_index;
		
	pivot = pArray[ ( beginIndex + endIndex ) / 2 ];  /* �͈͂̒����ɂ���v�f�𐕎��Ƃ��� */
	i = beginIndex;
	j = endIndex;
	
	while( 1 )
	{
		while( pArray[i] < pivot ){ ++i; }  /* �����ȏ�̒l��������܂ŉE�����֐i�߂Ă��� */
		while( pArray[j] > pivot ){ --j; }  /* �����ȉ��̒l��������܂ō������֐i�߂Ă��� */
		if( i >= j ){ break; }  /* ���E����i�߂Ă���i��j���Ԃ������烋�[�v���I���� */
		
		/* �v�f���������� */
		work = pArray[i];
		work_index = pArrayIndex[i];
		pArray[i] = pArray[j];
		pArray[j] = work;
		pArrayIndex[i] = pArrayIndex[j];
		pArrayIndex[j] = work_index;
		
		/*
		if ((pArray[i] == -33686019) || (pArray[j] == -33686019)) {
			int tmp;
			tmp = 0;
		}
		*/

		/* ����ɔ����āA���ړ_�����炷 */
		i++;
		j--;
	}
	
	/* �����̍����ɗv�f���Q�ȏ゠��΁A���̕����ɂ��ăN�C�b�N�\�[�g���ċA������ */
	if( beginIndex < i - 1 ){ QuickSortEx( pArray, pArrayIndex, beginIndex, i - 1 ); }
	/* �����̉E���ɗv�f���Q�ȏ゠��΁A���̕����ɂ��ăN�C�b�N�\�[�g���ċA������ */
	if( j + 1 < endIndex ){ QuickSortEx( pArray, pArrayIndex, j + 1, endIndex ); }
}

//---------------------------------------------------
/**
   �W���A�T�[�g
 */
//---------------------------------------------------
void
Basic::HoliAssert(char* method, char* str)
{
	int ret = 0;
	char mesg[256];
	memset(mesg,'\0',sizeof(mesg));
	sprintf_s(mesg, sizeof(mesg), "%s \n %s\n", method, str);

	OutputDebugString("====HoliAssert====");
	OutputDebugString("\n");
	OutputDebugString(mesg);

	ret = MessageBox(NULL, mesg, "====HoliAssert====", MB_ABORTRETRYIGNORE|MB_ICONEXCLAMATION);
	if (ret == IDABORT) {
#ifdef _DEBUG
			Basic::FlushDebugLog();
#endif // _DEBUG
		//		abort();
		//		exit(0);
	} else if (ret == IDRETRY) {
		// ����͉�������������Ȃ�z
	} else if (ret == IDIGNORE) {

	}
}

//---------------------------------------------------
/**
   �V�X�e���G���[
 */
//---------------------------------------------------
void
Basic::SystemError(char* method, char* str)
{
	int ret = 0;
	char mesg[256];
	memset(mesg,'\0',sizeof(mesg));
	sprintf_s(mesg, sizeof(mesg), "%s \n %s\n\n", method, str);

	OutputDebugString("==== System Error ====");
	OutputDebugString("\n");
	OutputDebugString(mesg);

	ret = MessageBox(NULL, mesg, "==== System Error ====", MB_OK);

	// �Ӑ}�I�ɋ����I��������
	InputManager::GetInstance()->EnablePressKey(eKEY_ESCAPE);
	
	/* �����I�� */
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
/*static*/void
Basic::SystemMessage(char* header_msg, char* explain_msg)
{
	int ret = 0;

	ret = MessageBox(NULL, explain_msg, header_msg, MB_OK);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
/*static*/bool
Basic::SystemMessageAnswer(char* header_msg, char* explain_msg)
{
	int ret = 0;
	ret = MessageBox(NULL, explain_msg, header_msg, MB_OKCANCEL);

	if (ret == IDOK) {
		return true;
	}
	return false;
}

//---------------------------------------------------
/**
 * �����Ńv���Z�X���N��������
 */
//---------------------------------------------------
/*static*/BOOL
Basic:: CreateProcessSync(char* szFileName)
{
    PROCESS_INFORMATION pi;
    STARTUPINFO si;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    int nRet = CreateProcess(
            NULL, (LPTSTR)szFileName, NULL, NULL, FALSE,
            NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi
            );

    if (nRet == 0) // �N���Ɏ��s�����ꍇ
        return FALSE;
    
    // �N�������v���Z�X���I������܂őҋ@
    while (WaitForSingleObject(pi.hProcess, 0) != WAIT_OBJECT_0) {
        Sleep(100);
    }

    return FALSE;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
/**static*/int
Basic::IndexOf(const char* str, const char keyword)
{
	int length = strlen(str);
	for (int iI = 0; iI < length; iI ++) {
		if (str[iI] == keyword) {
			return iI;
		}
	}
	// �w��̕�����������Ȃ������ꍇ
	return -1;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
/*static*/int
Basic::LastIndexOf(const char* str, const char keyword)
{
	int length = strlen(str);
	for (int iI = length - 1; iI >= 0; iI --) {
		if (str[iI] == keyword) {
			return iI;
		}
	}
	// �w��̕�����������Ȃ������ꍇ
	return -1;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
/*static*/void
Basic::ShowDump(unsigned char* addr, unsigned int size)
{
	PRINT_CONSOLE(" ==== Start Dump ==== \n");
	
	const unsigned int showlen = 8;
	for (unsigned int iI = 0; iI < size; iI += showlen) {
		unsigned char buf[showlen] = {0};
		if (size - iI > showlen) {
			memcpy(&buf[0], &addr[iI], showlen);
		} else {
			memcpy(&buf[0], &addr[iI], (size - iI));
		}
		PRINT_CONSOLE("[0x%x] : %x %x %x %x   %x %x %x %x  \n", (unsigned int)addr + iI, 
					  buf[iI+0],buf[iI+1],buf[iI+2],buf[iI+3],buf[iI+4],buf[iI+5],buf[iI+6],buf[iI+7]);
	}
}

 /**** end of file ****/
