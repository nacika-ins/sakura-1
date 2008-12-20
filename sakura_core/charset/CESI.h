#pragma once

/*!	@file
	@brief �����R�[�h�̔��蒲�����鎞�Ɏg��������

	@author Sakura-Editor collaborators
	@date 2006/12/10 �V�K�쐬
	@date 2007/10/26 �N���X�̐����ύX (���F�����R�[�h�������ێ��N���X)
	@date 2008/19/17 �N���X�̐����ύX�i���F�����R�[�h�𒲍����鎞�Ɏg���C���^�[�t�F�[�X�N���X�j
*/
/*
	Copyright (C) 2006
	Copyright (C) 2007

	This software is provided 'as-is', without any express or implied
	warranty. In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

		1. The origin of this software must not be misrepresented;
		   you must not claim that you wrote the original software.
		   If you use this software in a product, an acknowledgment
		   in the product documentation would be appreciated but is
		   not required.

		2. Altered source versions must be plainly marked as such,
		   and must not be misrepresented as being the original software.

		3. This notice may not be removed or altered from any source
		   distribution.
*/

class CESI;
class CMemory;

#include "global.h"

#include "mem/CNativeT.h"


struct tagEncodingInfo {
	ECodeType eCodeID;  // �����R�[�h���ʔԍ�
	//int nEval1;    // �]���l�P
	//int nEval2;    // �]���l�Q
	int nSpecific;
	int nPoints;
};
typedef struct tagEncodingInfo  MBCODE_INFO, WCCODE_INFO;

/*
	���@�]���l�̎g�����@��

	SJIS, JIS, EUCJP, UTF-8, UTF-7 �̏ꍇ�F

	typedef�� MBCODE_INFO
	�]���l�P �� ���L�o�C�g��
	�]���l�Q �� �|�C���g���i���L�o�C�g�� �| �s���o�C�g���j

	UTF-16 UTF-16BE �̏ꍇ�F

	typedef�� WCCODE_INFO
	�]���l�P �� ���C�h�����̉��s�̌�
	�]���l�Q �� �s���o�C�g��
*/

static const DWORD ESI_NOINFORMATION		= 0;
static const DWORD ESI_MBC_DETECTED			= 1;
static const DWORD ESI_MBC_HANKATACOUNTED	= 2;
static const DWORD ESI_WC_DETECTED			= 4;
static const DWORD ESI_NODETECTED			= 8;
static const DWORD ESI_MBC_CESU8DETECTED	= 16;

// ���C�h�����̂Q��ނ�����̂̊i�[�ʒu
enum EStoreID4WCInfo {
	ESI_WCIDX_UTF16LE,
	ESI_WCIDX_UTF16BE,
	ESI_WCIDX_MAX,
};
// BOM �^�C�v
enum EBOMType {
	ESI_BOMTYPE_UNKNOWN = -1,
	ESI_BOMTYPE_LE =0,
	ESI_BOMTYPE_BE =1,
};


/*!
	�����R�[�h�𒲍����鎞�ɐ�������i�[�N���X
*/

class CESI {
public:

	virtual ~CESI() { ; }
	CESI() {
		m_dwStatus = ESI_NOINFORMATION;
		m_nTargetDataLen = -1;
	}
	// �������ʂ̏����i�[
	bool SetInformation( const char*, const int, ECodeType eFavoriteCodeType );

protected:

	// �Y�����Ɏg����D�揇�ʕ\���쐬
	void InitPriorityTable( const ECodeType );
	// ECodeType �Ŏw�肳�ꂽ�����R�[�h���D�悳���悤�ɂ���
	void MakePriorityTable( const ECodeType );

	//	**** �S��
	// �}���`�o�C�g�n��UNICODE�n�Ƃł��ꂼ����̊i�[�悪�Ⴄ�B
	// �ȉ��̊֐��ŋz������
	int GetIndexById( const ECodeType ) const; //!< �����R�[�hID ������i�[��C���f�b�N�X�𐶐�

	// �f�[�^�Z�b�^/�Q�b�^�[
	void SetEvaluation( const ECodeType, const int, const int );
	void GetEvaluation( const ECodeType, int *, int * ) const;

	// �D�悳��Ă��镶���R�[�h���擾
	ECodeType GetFavoriteCode( void ) const;

	// �����ΏۂƂȂ����f�[�^�̒����i8bit �P�ʁj
	int m_nTargetDataLen;

	// ���茋�ʂ��i�[�������
	unsigned int m_dwStatus;

public:

	// m_dwStatus �̃Z�b�^�[�^�Q�b�^�[
	void SetStatus( DWORD inf ){ m_dwStatus |= inf; }
	DWORD GetStatus(  ) const { return m_dwStatus; }

	// m_nTargetDataLen �̃Z�b�^�[�^�Q�b�^�[
	void SetDataLen( const int n ){ m_nTargetDataLen = n; }
	int GetDataLen( void ) const { return m_nTargetDataLen; }

protected:
	/*
		������̕����R�[�h�������W����
	*/
	void ScanMBCode( const char *, const int );
	void ScanUnicode( const char *, const int );

	void GetEncodingInfo_sjis( const char *, const int );
	void GetEncodingInfo_jis( const char *, const int );
	void GetEncodingInfo_eucjp( const char *, const int );
	void GetEncodingInfo_utf8( const char *, const int );
	void GetEncodingInfo_utf7( const char *, const int );
	void GetEncodingInfo_uni( const char *, const int );

	bool _CheckUtf16Eol( const char* pS, const bool bbig_endian );
	inline bool _CheckUtf16EolLE( const char* p ){ return _CheckUtf16Eol( p, false ); }
	inline bool _CheckUtf16EolBE( const char* p ){ return _CheckUtf16Eol( p, true ); }

public:
	//
	//	**** �}���`�o�C�g����֌W�̕ϐ����̑�
	//
	static const int NUM_OF_MBCODE = (CODE_CODEMAX - 2);
	MBCODE_INFO m_aMbcInfo[NUM_OF_MBCODE];   //!< SJIS, JIS, EUCJP, UTF8, UTF7 ���i�D��x�ɏ]���Ċi�[�����j
	MBCODE_INFO* m_apMbcInfo[NUM_OF_MBCODE]; //!< �]�����Ƀ\�[�g���ꂽ SJIS, JIS, EUCJP, UTF8, UTF7 �̏��
	int m_aMbcPriority[CODE_CODEMAX];        //!< ���s���Ɏg�p�����D�揇�ʕ\
	int m_nMbcSjisHankata;                   //!< SJIS ���p�J�^�J�i�̃o�C�g��
	bool m_bMbcCesu8Enabled;                 //!< UTF-8 �̌����ɂ����āACESU-8 �����o���ꂽ���ǂ���

	/*
		// �f�t�H���g�̗D�揇��
		const int gm_pMbDefaultPriority[] =
		{
			0,			//CODE_SJIS
			1,			//CODE_JIS
			2,			//CODE_EUC
			INT_MAX,	//CODE_UNICODE
			3,			//CODE_UTF8
			4,			//CODE_UTF7
			INT_MAX,	//CODE_UNICODEBE
		};
	*/

	// �}���`�o�C�g�n�̑{�����ʂ��A�|�C���g���傫�����Ƀ\�[�g�B
	// �\�[�g�������ʂ́Am_apMbcInfo �Ɋi�[
	void SortMBCInfo( void );

public:
	//
	// 	**** UTF-16 ����֌W�̕ϐ����̑�
	//
	WCCODE_INFO m_aWcInfo[ESI_WCIDX_MAX];  //!< UTF-16 LE/BE ���
	EBOMType m_eWcBomType;          //!< m_pWcInfo ���琄������� BOM �̎��

	EBOMType GetBOMType(void) const { return m_eWcBomType; }

protected:
	// BOM�̎�ނ𐄑�
	EBOMType GuessUtf16Bom( void ) const;


#ifdef _DEBUG
public:
	static void GetDebugInfo( const char*, const int, CNativeT* );
#endif
};