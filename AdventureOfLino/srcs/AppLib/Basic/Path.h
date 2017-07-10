/* -*- mode: c++; coding: shift_jis-dos; tab-width: 4; -*- */
//---------------------------------------------------
/**
 * Path.h
 * �p�X�����N���X
 * @author SPATZ.
 * @data   2011/04/02 16:35:28	
 */
//---------------------------------------------------
#ifndef __PATH_H__
#define __PATH_H__

/*=====================================*
 * include
 *=====================================*/

/*=====================================*
 * class
 *=====================================*/
class Path
{
	/* �֐� */
 public:
	Path();

	static bool SetupRootDirectory(const char* pFindDirectory);

	/* �֗��֐� */
	static void	ConvertFullFilename(char* outName, size_t outNameSize, const char* fileName);
	static void	ConvertLocalFilename(char* outName, size_t outNameSize, const char* fileName);
	static void	GetDirectoryName(char* outName, size_t outNameSize, const char* fileName);
	static void GetOnlyFileName(char* outName, size_t outNameSize, const char* fileName);

	static int	GetFileNum(const char* filePath);

	/* �ϐ� */
	static char mRootDirName[256];

};

#endif // __PATH_H__

/**** end of file ****/


