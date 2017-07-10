/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * csvaccesser.cpp
 * CSV�t�@�C���A�N�Z�X�N���X
 * @author SPATZ.
 * @data   2011/05/15 00:59:40
 */
//---------------------------------------------------
//#include <list>

#include "dxlib.h"

#include "AppLib/Basic/Basic.h"
#include "CsvAccesser.h"

/*=====================================*
 * define
 *=====================================*/
//---------------------------------------------------
/**
   �R���X�g���N�^
*/
//---------------------------------------------------
CsvAccesser::CsvAccesser()
{
	// ������
	mFileHandle = NULL;
	mFileSize = 0;
	mpBuffer = NULL;

	mLoadPhase		= eLOAD_PHASE_NONE;
}

//---------------------------------------------------
/**
   �f�X�g���N�^
*/
//---------------------------------------------------
CsvAccesser::~CsvAccesser()
{
	DeleteFile();
}

//---------------------------------------------------
/**
   ����
*/
//---------------------------------------------------
void
CsvAccesser::Create(void)
{

}

/*=====================================*
 * ���\�[�X�ǂݍ��݊֘A
 *=====================================*/
//---------------------------------------------------
/**
   �����Ń��\�[�X�ǂݍ���
*/
//---------------------------------------------------
bool
CsvAccesser::LoadFile(const char* fileName)
{
	// ��x�j�����s��
	DeleteFile();

	// �t�@�C�����̃|�C���^���擾
	if (loadResource(fileName) == false) {
		return false;
	}

	setupRowList();

	changeDot2Null();

	mLoadPhase = eLOAD_PHASE_FINISH;

	return true;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CsvAccesser::DeleteFile(void)
{
	// �t�@�C���ǂݍ��݃o�b�t�@�̔j��
	if (mpBuffer != NULL) {
		delete[] mpBuffer;
		mpBuffer = NULL;
	}

	// �ǂݍ��݊��������̂Ńt�@�C�������
	if (mFileHandle != -1) {
		FileRead_close(mFileHandle);
		mFileHandle = -1;
	}

	// �S�v�f���폜
	mRowList.clear();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CsvAccesser::GetLastWriteTime(const char* fileName, FILETIME* fileTime)
{
	WIN32_FIND_DATA find_data;
	HANDLE hFile;
	hFile = FindFirstFile(fileName, &find_data);
	APP_ASSERT_PRINT((hFile != INVALID_HANDLE_VALUE),
					 "Failed FindFirstFile(%s) \n", fileName);
	FindClose(hFile);
	FileTimeToLocalFileTime(&find_data.ftLastWriteTime, fileTime);
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
char*
CsvAccesser::GetString(DWORD row, DWORD column)
{
	APP_ASSERT_PRINT(row < GetRowNum(),
					 "Select row is OverFlow[%d / %d]\n", row, GetRowNum());
	APP_ASSERT_PRINT(column < GetColumnNum(),
					 "Select column is OverFlow[%d / %d]\n", column, GetColumnNum());
	
	tStringWork str_work;
	// �w�肳�ꂽ row, column ����f�[�^�𒊏o
	str_work = mRowList.at(row).at(column);

	return str_work.mpStr;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
float 
CsvAccesser::GetFloat(DWORD row, DWORD column)
{
	return (float)atof(GetString(row, column));
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
int 
CsvAccesser::GetInt(DWORD row, DWORD column)
{
	return atoi(GetString(row, column));
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
bool
CsvAccesser::GetBool(DWORD row, DWORD column)
{
	const char* tmp = GetString(row, column);
	if (strcmp(tmp, "true") == 0 || 
		strcmp(tmp, "TRUE") == 0 ||
		strcmp(tmp, "True") == 0) {
		return true;
	}
	return false;
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
VECTOR
CsvAccesser::GetVector(DWORD row, DWORD column)
{
	VECTOR out = ZERO_VEC;

	char* str = GetString(row, column);

	const char* search_word = " ";
	
	char* next;
	char buf[16];
	DWORD size;

	next = strstr(str, search_word);
	size = (DWORD)next - (DWORD)str;
	memcpy(buf, str, size);
	buf[size] = '\0';
	out.x = (float)atof(buf);

	str = next + 1;

	next = strstr(str, search_word);
	size = (DWORD)next - (DWORD)str;
	memcpy(buf, str, size);
	buf[size] = '\0';
	out.y = (float)atof(buf);

	str = next + 1;

	next = strstr(str, search_word);
	if (next == NULL) {
		out.z = (float)atof(str);
	} else {
		size = (DWORD)next - (DWORD)str;
		memcpy(buf, str, size);
		buf[size] = '\0';
		out.z = (float)atof(buf);
	} 

	return out;
}

//---------------------------------------------------
/**
   
 */
//---------------------------------------------------
DWORD
CsvAccesser::GetRowNum(void)
{
	return mRowList.size();
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
DWORD
CsvAccesser::GetColumnNum(void)
{
	return mRowList.at(0).size();
}
	
/*=====================================*
  private method
  *=====================================*/
//---------------------------------------------------
/**
   ���\�[�X�ǂݍ���
*/
//---------------------------------------------------
bool
CsvAccesser::loadResource(const char* fileName)
{
	BOOL result = FALSE;
	
	char fullname[256] = {0};
	// �t�@�C�����J��

	mFileHandle = FileRead_open(fileName);

	// �t�@�C���̓ǂݍ��݂Ɏ��s�����ꍇ�̓G���[���^�[��
	if (mFileHandle == 0) {
		PRINT_CONSOLE("Failed FileRead_open %s\n", fullname);
		return false;
	}

	PRINT_CONSOLE("====CsvAccesser::loadResource() mFileHandle : %d \n====");

	// �t�@�C���T�C�Y���擾
	mFileSize = (DWORD)FileRead_size(fileName);
	APP_ASSERT_PRINT(mFileSize != -1,
					 "Failed FileRead_size %s\n", fullname);
	PRINT_CONSOLE("Open CsvAccesser file %s, [%d]byte\n", fileName, mFileSize);

	// �t�@�C���ǂݍ��ݗp�o�b�t�@�̃��������m��
	mpBuffer = new char[mFileSize];
	APP_ASSERT_PRINT(mpBuffer != NULL,
					 "Failed Alloc FileBuffer, size %d\n", mFileSize);
	
	DWORD readed_byte = 0;
	// �t�@�C�������ǂݍ���
	FileRead_read((void *)mpBuffer, mFileSize, mFileHandle);
	// �ǂݍ��݊��������̂Ńt�@�C�������
	if (mFileHandle != -1) {
		FileRead_close(mFileHandle);
		mFileHandle = -1;
	}

	return true;
}
	
//---------------------------------------------------
/**
   ���\�[�X�j��
*/
//---------------------------------------------------
/*static*/void
CsvAccesser::deleteResource(void)
{
	return;
}

//---------------------------------------------------
/**
   
 */
//---------------------------------------------------
void
CsvAccesser::setupRowList(void)
{
	char* buf = (char *)mpBuffer;

	// �s�Ɨ�̐��𒲂ׂ�
	DWORD last_index = 0;
	
	std::vector<tStringWork> tmp_list;
	tStringWork str_work;

	for (DWORD iI = 0; iI < mFileSize; iI ++) {

		if (buf[iI] == ',') {
			str_work.mLength	= iI - last_index;
			str_work.mpStr		= &buf[last_index];
			tmp_list.push_back(str_work);

			last_index = iI + 1;
		}

		if (buf[iI] == '\r' && buf[iI + 1] == '\n') {
			
			str_work.mLength	= iI - last_index;
			str_work.mpStr		= &buf[last_index];
			tmp_list.push_back(str_work);

			mRowList.push_back(tmp_list);
			tmp_list.clear();

			last_index = iI + 2;
		}
	}
}

//---------------------------------------------------
/**
	
 */
//---------------------------------------------------
void
CsvAccesser::changeDot2Null(void)
{
	char* buf = (char *)mpBuffer;

	for (DWORD iI = 0; iI < mFileSize; iI ++) {
		if (buf[iI] == ',') {
			buf[iI] = 0;
		}
	}
}

/**** end of file ****/




