
// PiCipherToolDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CPiCipherToolDlg 对话框
class CPiCipherToolDlg : public CDialogEx
{
// 构造
public:
	CPiCipherToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PICIPHERTOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox comboCtrl_CipherType;
	CString comboVal_CihperType;
	UINT editVal_ADLength;
	CString editbrowseVal_FilePath;
	CButton btn_Encryption;
	CButton btn_Decryption;
	afx_msg void OnBnClickedButtonEnc();
	afx_msg void OnBnClickedButtonDec();
	int ae16_encryption(char* MessageFileName, word32 ADLength, char* CipherFileName, word16* PMN, word16* SMN, word16* K, word16* Authen);
	int ae16_decryption(char* CipherFileName, word32 ADLength, char* MessageFileName, word16* PMN, word16* SMN, word16* K, word16* Authen);
	int ae32_encryption(char* MessageFileName, word32 ADLength, char* CipherFileName, word32* PMN, word32* SMN, word32* K, word32* Authen);
	int ae32_decryption(char* CipherFileName, word32 ADLength, char* MessageFileName, word32* PMN, word32* SMN, word32* K, word32* Authen);
	int ae64_128_encryption(char* MessageFileName, word32 ADLength, char* CipherFileName, word64* PMN, word64* SMN, word64* K, word64* Authen);
	int ae64_128_decryption(char* CipherFileName, word32 ADLength, char* MessageFileName, word64* PMN, word64* SMN, word64* K, word64* Authen);
	int ae64_256_encryption(char* MessageFileName, word32 ADLength, char* CipherFileName, word64* PMN, word64* SMN, word64* K, word64* Authen);
	int ae64_256_decryption(char* CipherFileName, word32 ADLength, char* MessageFileName, word64* PMN, word64* SMN, word64* K, word64* Authen);
	CString editVal_PMN;
	CEdit editCtrl_SMN;
	CString editVal_SMN;
	CString editVal_Key;
	CButton btn_Demo1;
	afx_msg void OnBnClickedButtonDemo1();
	afx_msg void OnBnClickedButtonDemo2();
	void cstring2wordArray(CString src, word16* dst);
	void cstring2wordArray(CString src, word32* dst);
	void cstring2wordArray(CString src, word64* dst);
	CEdit editCtrl_Authen;
	CString editVal_Authen;
	CButton btn_Empty;
	afx_msg void OnBnClickedButtonEmpty();
	void HexString_to_WordArray(CString src, word16* dst);
	BYTE Hex2Byte(BYTE value);
	void HexString_to_WordArray(CString src, word32* dst);
	void HexString_to_WordArray(CString src, word64* dst);
	CEdit editCtrl_DecSMN;
	CString editVal_DecSMN;
};
