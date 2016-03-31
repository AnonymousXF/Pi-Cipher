
// PiCipherToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PiCipherTool.h"
#include "PiCipherToolDlg.h"
#include "afxdialogex.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include "Pi_16.h"
#include "Pi_32.h"
#include "Pi_64.h"
using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPiCipherToolDlg 对话框



CPiCipherToolDlg::CPiCipherToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PICIPHERTOOL_DIALOG, pParent)
	, comboVal_CihperType(_T(""))
	, editVal_ADLength(0)
	, editbrowseVal_FilePath(_T(""))
	, editVal_PMN(_T(""))
	, editVal_SMN(_T(""))
	, editVal_Key(_T(""))
	, editVal_Authen(_T(""))
	, editVal_DecSMN(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPiCipherToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CIPHER_TYPE, comboCtrl_CipherType);
	DDX_CBString(pDX, IDC_COMBO_CIPHER_TYPE, comboVal_CihperType);
	DDX_Text(pDX, IDC_EDIT_AD_LENGTH, editVal_ADLength);
	DDX_Text(pDX, IDC_MFCEDITBROWSE_FILE_PATH, editbrowseVal_FilePath);
	DDX_Control(pDX, IDC_BUTTON_ENC, btn_Encryption);
	DDX_Control(pDX, IDC_BUTTON_DEC, btn_Decryption);
	DDX_Text(pDX, IDC_EDIT_PMN, editVal_PMN);
	DDX_Control(pDX, IDC_EDIT_SMN, editCtrl_SMN);
	DDX_Text(pDX, IDC_EDIT_SMN, editVal_SMN);
	DDX_Text(pDX, IDC_EDIT_KEY, editVal_Key);
	DDX_Control(pDX, IDC_BUTTON_DEMO1, btn_Demo1);
	DDX_Control(pDX, IDC_EDIT_AUTHEN, editCtrl_Authen);
	DDX_Text(pDX, IDC_EDIT_AUTHEN, editVal_Authen);
	DDX_Control(pDX, IDC_BUTTON_EMPTY, btn_Empty);
	DDX_Control(pDX, IDC_EDIT_DEC_SMN, editCtrl_DecSMN);
	DDX_Text(pDX, IDC_EDIT_DEC_SMN, editVal_DecSMN);
}

BEGIN_MESSAGE_MAP(CPiCipherToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ENC, &CPiCipherToolDlg::OnBnClickedButtonEnc)
	ON_BN_CLICKED(IDC_BUTTON_DEC, &CPiCipherToolDlg::OnBnClickedButtonDec)
	ON_BN_CLICKED(IDC_BUTTON_DEMO1, &CPiCipherToolDlg::OnBnClickedButtonDemo1)
	ON_BN_CLICKED(IDC_BUTTON_DEMO2, &CPiCipherToolDlg::OnBnClickedButtonDemo2)
	ON_BN_CLICKED(IDC_BUTTON_EMPTY, &CPiCipherToolDlg::OnBnClickedButtonEmpty)
END_MESSAGE_MAP()


// CPiCipherToolDlg 消息处理程序

BOOL CPiCipherToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	comboCtrl_CipherType.AddString(_T("π16-Cipher096"));
	comboCtrl_CipherType.AddString(_T("π32-Cipher128"));
	comboCtrl_CipherType.AddString(_T("π64-Cipher128"));
	comboCtrl_CipherType.AddString(_T("π64-Cipher256"));

	editCtrl_Authen.SetReadOnly();
	editCtrl_DecSMN.SetReadOnly();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPiCipherToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPiCipherToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPiCipherToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPiCipherToolDlg::OnBnClickedButtonEnc()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	editVal_Authen = _T("");
	editbrowseVal_FilePath.Replace('\\', '/');
	string temp = CT2A(editbrowseVal_FilePath);
	char* m = const_cast<char*>(temp.data());
	char* c = "cipher.txt";

	//π16-Cipher096
	if (comboVal_CihperType == _T("π16-Cipher096")) {
		if (editVal_PMN.GetLength() != 8) {
			MessageBox(_T("算法要求PMN长度为4字节，当前长度不符合要求!"));
			return;
		}
		if (editVal_SMN.GetLength() != 32) {
			MessageBox(_T("算法要求SMN长度为16字节，当前长度不符合要求!"));
			return;
		}
		if (editVal_Key.GetLength() != 24) {
			MessageBox(_T("算法要求Key长度为12字节，当前长度不符合要求!"));
			return;
		}
		word16 K[6], PMN[2], SMN[8], Authen[8];
		//cstring2wordArray(editVal_PMN, PMN);
		//cstring2wordArray(editVal_SMN, SMN);
		//cstring2wordArray(editVal_Key, K);
		HexString_to_WordArray(editVal_PMN, PMN);
		HexString_to_WordArray(editVal_SMN, SMN);
		HexString_to_WordArray(editVal_Key, K);

		int state = ae16_encryption(m, editVal_ADLength, c, PMN, SMN, K, Authen);
		for (int i = 0; i < 8; i++)
			editVal_Authen.AppendFormat(_T("%04x"), Authen[i]);

		switch (state) {
		case MSG_FILE_OPEN_FAILED:MessageBox(_T("消息/明文文件打开失败！")); break;
		case CIPHER_FILE_OPEN_FAILED:MessageBox(_T("密文文件创建失败！")); break;
		case AD_LENGTH_NOT_ENOUGH:MessageBox(_T("关联数据长度超出文件长度！")); break;
		case FILE_OPEN_FAILED:MessageBox(_T("临时文件创建失败！")); break;
		case ENCRYPT_SUCCEED:MessageBox(_T("Encryption Succeed！")); break;
		}
		UpdateData(FALSE);
	}
	//π32-Cipher128
	if (comboVal_CihperType == _T("π32-Cipher128")) {
		if (editVal_PMN.GetLength() != 32) {
			MessageBox(_T("算法要求PMN长度为16字节，当前长度不符合要求!"));
			return;
		}
		if (editVal_SMN.GetLength() != 64) {
			MessageBox(_T("算法要求SMN长度为32字节，当前长度不符合要求!"));
			return;
		}
		if (editVal_Key.GetLength() != 32) {
			MessageBox(_T("算法要求Key长度为16字节，当前长度不符合要求!"));
			return;
		}
		word32 K[4], PMN[4], SMN[8], Authen[8];
		//cstring2wordArray(editVal_PMN, PMN);
		//cstring2wordArray(editVal_SMN, SMN);
		//cstring2wordArray(editVal_Key, K);
		HexString_to_WordArray(editVal_PMN, PMN);
		HexString_to_WordArray(editVal_SMN, SMN);
		HexString_to_WordArray(editVal_Key, K);

		int state = ae32_encryption(m, editVal_ADLength, c, PMN, SMN, K, Authen);
		for (int i = 0; i < 8; i++)
			editVal_Authen.AppendFormat(_T("%08x"), Authen[i]);

		switch (state) {
		case MSG_FILE_OPEN_FAILED:MessageBox(_T("消息/明文文件打开失败！")); break;
		case CIPHER_FILE_OPEN_FAILED:MessageBox(_T("密文文件创建失败！")); break;
		case AD_LENGTH_NOT_ENOUGH:MessageBox(_T("关联数据长度超出文件长度！")); break;
		case FILE_OPEN_FAILED:MessageBox(_T("临时文件创建失败！")); break;
		case ENCRYPT_SUCCEED:MessageBox(_T("Encryption Succeed！")); break;
		}
		UpdateData(FALSE);
	}
	//π64-Cipher128
	if (comboVal_CihperType == _T("π64-Cipher128")) {
		if (editVal_PMN.GetLength() != 32) {
			MessageBox(_T("算法要求PMN长度为16字节，当前长度不符合要求!"));
			return;
		}
		if (editVal_SMN.GetLength() != 128) {
			MessageBox(_T("算法要求SMN长度为64字节，当前长度不符合要求!"));
			return;
		}
		if (editVal_Key.GetLength() != 32) {
			MessageBox(_T("算法要求Key长度为16字节，当前长度不符合要求!"));
			return;
		}
		word64 K[2], PMN[2], SMN[8],Authen[8];
		//cstring2wordArray(editVal_PMN, PMN);
		//cstring2wordArray(editVal_SMN, SMN);
		//cstring2wordArray(editVal_Key, K);
		HexString_to_WordArray(editVal_PMN, PMN);
		HexString_to_WordArray(editVal_SMN, SMN);
		HexString_to_WordArray(editVal_Key, K);

		int state = ae64_128_encryption(m, editVal_ADLength, c, PMN, SMN, K, Authen);
		for (int i = 0; i < 8; i++)
			editVal_Authen.AppendFormat(_T("%I64x"), Authen[i]);

		switch (state) {
		case MSG_FILE_OPEN_FAILED:MessageBox(_T("消息/明文文件打开失败！")); break;
		case CIPHER_FILE_OPEN_FAILED:MessageBox(_T("密文文件创建失败！")); break;
		case AD_LENGTH_NOT_ENOUGH:MessageBox(_T("关联数据长度超出文件长度！")); break;
		case FILE_OPEN_FAILED:MessageBox(_T("临时文件创建失败！")); break;
		case ENCRYPT_SUCCEED:MessageBox(_T("Encryption Succeed！")); break;
		}
		UpdateData(FALSE);
	}
	//π64-Cipher256
	if (comboVal_CihperType == _T("π64-Cipher256")) {
		if (editVal_PMN.GetLength() != 32) {
			MessageBox(_T("算法要求PMN长度为16字节，当前长度不符合要求!"));
			return;
		}
		if (editVal_SMN.GetLength() != 128) {
			MessageBox(_T("算法要求SMN长度为64字节，当前长度不符合要求!"));
			return;
		}
		if (editVal_Key.GetLength() != 64) {
			MessageBox(_T("算法要求Key长度为32字节，当前长度不符合要求!"));
			return;
		}
		word64 K[4], PMN[2], SMN[8], Authen[8];
		//cstring2wordArray(editVal_PMN, PMN);
		//cstring2wordArray(editVal_SMN, SMN);
		//cstring2wordArray(editVal_Key, K);
		HexString_to_WordArray(editVal_PMN, PMN);
		HexString_to_WordArray(editVal_SMN, SMN);
		HexString_to_WordArray(editVal_Key, K);

		int state = ae64_256_encryption(m, editVal_ADLength, c, PMN, SMN, K, Authen);
		for (int i = 0; i < 8; i++)
			editVal_Authen.AppendFormat(_T("%I64x"), Authen[i]);

		switch (state) {
		case MSG_FILE_OPEN_FAILED:MessageBox(_T("消息/明文文件打开失败！")); break;
		case CIPHER_FILE_OPEN_FAILED:MessageBox(_T("密文文件创建失败！")); break;
		case AD_LENGTH_NOT_ENOUGH:MessageBox(_T("关联数据长度超出文件长度！")); break;
		case FILE_OPEN_FAILED:MessageBox(_T("临时文件创建失败！")); break;
		case ENCRYPT_SUCCEED:MessageBox(_T("Encryption Succeed！")); break;
		}
		UpdateData(FALSE);
	}
}


void CPiCipherToolDlg::OnBnClickedButtonDec()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	editVal_Authen = _T("");
	editVal_DecSMN = _T("");
	editbrowseVal_FilePath.Replace('\\', '/');
	string temp = CT2A(editbrowseVal_FilePath);
	char* c = const_cast<char*>(temp.data());
	char* n = "new.txt";

	//π16-Cipher096
	if (comboVal_CihperType == _T("π16-Cipher096")) {
		if (editVal_PMN.GetLength() != 8) {
			MessageBox(_T("算法要求PMN长度为4字节，当前长度不符合要求!"));
			return;
		}
		if (editVal_Key.GetLength() != 24) {
			MessageBox(_T("算法要求Key长度为12字节，当前长度不符合要求!"));
			return;
		}
		word16 K[6], PMN[2], SMN[8], Authen[8];
		//cstring2wordArray(editVal_PMN, PMN);
		//cstring2wordArray(editVal_Key, K);
		HexString_to_WordArray(editVal_PMN, PMN);
		HexString_to_WordArray(editVal_Key, K);

		int state = ae16_decryption(c, editVal_ADLength, n, PMN, SMN, K, Authen);
		if (state != AUTHEN_FAILED) {
			for (int i = 0; i < 8; i++) {
				editVal_Authen.AppendFormat(_T("%04x"), Authen[i]);
				editVal_DecSMN.AppendFormat(_T("%04x"), SMN[i]);
			}
		}

		switch (state) {
		case MSG_FILE_OPEN_FAILED:MessageBox(_T("消息/明文文件创建失败！")); break;
		case CIPHER_FILE_OPEN_FAILED:MessageBox(_T("密文文件打开失败！")); break;
		case FILE_OPEN_FAILED:MessageBox(_T("临时文件创建失败！")); break;
		case AUTHEN_FAILED:MessageBox(_T("认证摘要验证错误，解密失败！")); break;
		case DECRYPT_SUCCEED:MessageBox(_T("Decryption Succeed!")); break;
		}
		UpdateData(FALSE);
	}
	//π32-Cipher128
	if (comboVal_CihperType == _T("π32-Cipher128")) {
		if (editVal_PMN.GetLength() != 32) {
			MessageBox(_T("算法要求PMN长度为16字节，当前长度不符合要求!"));
			return;
		}
		if (editVal_Key.GetLength() != 32) {
			MessageBox(_T("算法要求Key长度为16字节，当前长度不符合要求!"));
			return;
		}
		word32 K[4], PMN[4], SMN[8], Authen[8];
		//cstring2wordArray(editVal_PMN, PMN);
		//cstring2wordArray(editVal_Key, K);
		HexString_to_WordArray(editVal_PMN, PMN);
		HexString_to_WordArray(editVal_Key, K);

		int state = ae32_decryption(c, editVal_ADLength, n, PMN, SMN, K, Authen);
		if (state != AUTHEN_FAILED) {
			for (int i = 0; i < 8; i++) {
				editVal_Authen.AppendFormat(_T("%08x"), Authen[i]);
				editVal_DecSMN.AppendFormat(_T("%08x"), SMN[i]);
			}
		}

		switch (state) {
		case MSG_FILE_OPEN_FAILED:MessageBox(_T("消息/明文文件创建失败！")); break;
		case CIPHER_FILE_OPEN_FAILED:MessageBox(_T("密文文件打开失败！")); break;
		case FILE_OPEN_FAILED:MessageBox(_T("临时文件创建失败！")); break;
		case AUTHEN_FAILED:MessageBox(_T("认证摘要验证错误，解密失败！")); break;
		case DECRYPT_SUCCEED:MessageBox(_T("Decryption Succeed!")); break;
		}
		UpdateData(FALSE);
	}
	//π64-Cipher128
	if (comboVal_CihperType == _T("π64-Cipher128")) {
		if (editVal_PMN.GetLength() != 32) {
			MessageBox(_T("算法要求PMN长度为16字节，当前长度不符合要求!"));
			return;
		}
		if (editVal_Key.GetLength() != 32) {
			MessageBox(_T("算法要求Key长度为16字节，当前长度不符合要求!"));
			return;
		}
		word64 K[2], PMN[2], SMN[8], Authen[8];
		//cstring2wordArray(editVal_PMN, PMN);
		//cstring2wordArray(editVal_Key, K);
		HexString_to_WordArray(editVal_PMN, PMN);
		HexString_to_WordArray(editVal_Key, K);

		int state = ae64_128_decryption(c, editVal_ADLength, n, PMN, SMN, K, Authen);
		if (state != AUTHEN_FAILED) {
			for (int i = 0; i < 8; i++) {
				editVal_Authen.AppendFormat(_T("%I64x"), Authen[i]);
				editVal_DecSMN.AppendFormat(_T("%I64x"), SMN[i]);
			}
				
		}

		switch (state) {
		case MSG_FILE_OPEN_FAILED:MessageBox(_T("消息/明文文件创建失败！")); break;
		case CIPHER_FILE_OPEN_FAILED:MessageBox(_T("密文文件打开失败！")); break;
		case FILE_OPEN_FAILED:MessageBox(_T("临时文件创建失败！")); break;
		case AUTHEN_FAILED:MessageBox(_T("认证摘要验证错误，解密失败！")); break;
		case DECRYPT_SUCCEED:MessageBox(_T("Decryption Succeed!")); break;
		}
		UpdateData(FALSE);
	}
	//π64-Cipher256
	if (comboVal_CihperType == _T("π64-Cipher256")) {
		if (editVal_PMN.GetLength() != 32) {
			MessageBox(_T("算法要求PMN长度为16字节，当前长度不符合要求!"));
			return;
		}
		if (editVal_Key.GetLength() != 64) {
			MessageBox(_T("算法要求Key长度为32字节，当前长度不符合要求!"));
			return;
		}
		word64 K[4], PMN[2], SMN[8],Authen[8];
		//cstring2wordArray(editVal_PMN, PMN);
		//cstring2wordArray(editVal_Key, K);
		HexString_to_WordArray(editVal_PMN, PMN);
		HexString_to_WordArray(editVal_Key, K);

		int state = ae64_256_decryption(c, editVal_ADLength, n, PMN, SMN, K, Authen);
		if (state != AUTHEN_FAILED) {
			for (int i = 0; i < 8; i++) {
				editVal_Authen.AppendFormat(_T("%I64x"), Authen[i]);
				editVal_DecSMN.AppendFormat(_T("%I64x"), SMN[i]);
			}
		}

		switch (state) {
		case MSG_FILE_OPEN_FAILED:MessageBox(_T("消息/明文文件创建失败！")); break;
		case CIPHER_FILE_OPEN_FAILED:MessageBox(_T("密文文件打开失败！")); break;
		case FILE_OPEN_FAILED:MessageBox(_T("临时文件创建失败！")); break;
		case AUTHEN_FAILED:MessageBox(_T("认证摘要验证错误，解密失败！")); break;
		case DECRYPT_SUCCEED:MessageBox(_T("Decryption Succeed!")); break;
		}
		UpdateData(FALSE);
	}
}


/***********    认证加密(16bit字长)    ***********
消息M和关联数据AD均采用文件方式输入
PMN:word16[2], SMN:word16[8], K:word16[6]
*************************************************/
int CPiCipherToolDlg::ae16_encryption(char* MessageFileName, word32 ADLength, char* CipherFileName, word16* PMN, word16* SMN, word16* K, word16* Authen)
{
	word64 ctr;
	word16 CIS[16] = { 0 };
	//初始化
	ctr = initialization_16(K, PMN, CIS);
	//消息、关联数据填充
	word32 MessageLength = 0;
	word8 ByteOne = 0x01;
	word8 ByteZero = 0x00;
	FILE *MessageFile, *MessageTemp, *ADTemp, *CipherFile;
	if ((MessageFile = fopen(MessageFileName, "rb+")) == NULL)
		return MSG_FILE_OPEN_FAILED;
	if ((MessageTemp = fopen("MessageTemp.txt", "wb+")) == NULL)
		return FILE_OPEN_FAILED;
	if ((ADTemp = fopen("ADTemp.txt", "wb+")) == NULL)
		return FILE_OPEN_FAILED;
	if ((CipherFile = fopen(CipherFileName, "wb+")) == NULL)
		return CIPHER_FILE_OPEN_FAILED;
	word8 byte;
	for (word32 i = 0; i < ADLength; i++) {
		if (fread(&byte, sizeof(byte), 1, MessageFile) != 1) {
			_fcloseall();
			remove("MessageTemp.txt");
			remove("ADTemp.txt");
			remove(CipherFileName);
			return AD_LENGTH_NOT_ENOUGH;
		}
		fwrite(&byte, sizeof(byte), 1, ADTemp);
		fwrite(&byte, sizeof(byte), 1, CipherFile);
	}
	fseek(MessageFile, 0, SEEK_END);
	MessageLength = ftell(MessageFile) - ADLength;
	fseek(MessageFile, ADLength, SEEK_SET);
	while (fread(&byte, sizeof(byte), 1, MessageFile) == 1)
		fwrite(&byte, sizeof(byte), 1, MessageTemp);

	if (MessageLength % 16 == 0) {
		fwrite(&ByteOne, sizeof(ByteOne), 1, MessageTemp);
		for (int i = 1; i <= 15; i++)
			fwrite(&ByteZero, sizeof(ByteZero), 1, MessageTemp);
	}
	else {
		fwrite(&ByteOne, sizeof(ByteOne), 1, MessageTemp);
		for (unsigned int i = 1; i <= 16 - (MessageLength % 16) - 1; i++)
			fwrite(&ByteZero, sizeof(ByteZero), 1, MessageTemp);
	}
	if (ADLength % 16 == 0) {
		fwrite(&ByteOne, sizeof(ByteOne), 1, ADTemp);
		for (int i = 1; i <= 15; i++)
			fwrite(&ByteZero, sizeof(ByteZero), 1, ADTemp);
	}
	else {
		fwrite(&ByteOne, sizeof(ByteOne), 1, ADTemp);
		for (unsigned int i = 1; i <= 16 - (ADLength % 16) - 1; i++)
			fwrite(&ByteZero, sizeof(ByteZero), 1, ADTemp);
	}
	fseek(MessageTemp, 0, SEEK_SET);
	fseek(ADTemp, 0, SEEK_SET);
	//处理关联数据
	word16 ADi[8], ti[8];
	word16 T_1[8] = { 0 };
	int a = 0;
	while (fread(&ADi, sizeof(word16), 8, ADTemp) == 8) {
		a++;
		processADBlock_16(CIS, ADi, ctr, a, ti);
		for (int i = 0; i < 8; i++)
			T_1[i] = (T_1[i] + ti[i]) & mod_16;
	}
	//更新CIS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			CIS[8 * i + j] ^= T_1[4 * i + j];
		}
	}
	piFunction_16(CIS, CIS);
	//处理SMN
	word16 C0[8], T_2[8];
	processSMN4Encryption_16(CIS, SMN, ctr, a, C0, T_1, T_2, CIS);
	fwrite(&C0, sizeof(word16), 8, CipherFile);
	//加密消息
	word16 T[8];
	word16 Mi[8], Ci[8];
	int m = 0;
	for (int i = 0; i < 8; i++)
		T[i] = T_2[i];
	while (fread(&Mi, sizeof(word16), 8, MessageTemp) == 8) {
		m++;
		processMessageBlock_16(CIS, ctr, a, m, Ci, Mi, ti);
		fwrite(&Ci, sizeof(word16), 8, CipherFile);
		for (int i = 0; i < 8; i++)
			T[i] = (T[i] + ti[i]) & mod_16;
	}
	fwrite(&T, sizeof(word16), 8, CipherFile);
	for (int i = 0; i < 8; i++)
		Authen[i] = T[i];
	fclose(MessageTemp);
	fclose(ADTemp);
	remove("ADTemp.txt");
	remove("MessageTemp.txt");
	fclose(CipherFile);
	return ENCRYPT_SUCCEED;
}


/***********    解密验证(16bit字长)    ***********
密文C和关联数据AD均采用文件方式输入
PMN:word16[2], K:word16[6]
*************************************************/
int CPiCipherToolDlg::ae16_decryption(char* CipherFileName, word32 ADLength, char* MessageFileName, word16* PMN, word16* SMN, word16* K, word16* Authen)
{
	word64 ctr;
	word16 CIS[16] = { 0 };
	//初始化
	ctr = initialization_16(K, PMN, CIS);
	//关联数据填充
	word8 ByteOne = 0x01;
	word8 ByteZero = 0x00;
	FILE *CipherFile, *ADTemp, *MessageFile;
	if ((CipherFile = fopen(CipherFileName, "rb+")) == NULL)
		return CIPHER_FILE_OPEN_FAILED;
	if ((ADTemp = fopen("ADTemp.txt", "wb+")) == NULL)
		return FILE_OPEN_FAILED;
	if ((MessageFile = fopen(MessageFileName, "wb+")) == NULL)
		return MSG_FILE_OPEN_FAILED;

	word8 byte;
	for (word32 i = 0; i < ADLength; i++) {
		fread(&byte, sizeof(byte), 1, CipherFile);
		fwrite(&byte, sizeof(byte), 1, ADTemp);
		fwrite(&byte, sizeof(byte), 1, MessageFile);
	}
	if (ADLength % 16 == 0) {
		fwrite(&ByteOne, sizeof(ByteOne), 1, ADTemp);
		for (int i = 1; i <= 15; i++)
			fwrite(&ByteZero, sizeof(ByteZero), 1, ADTemp);
	}
	else {
		fwrite(&ByteOne, sizeof(ByteOne), 1, ADTemp);
		for (unsigned int i = 1; i <= 16 - (ADLength % 16) - 1; i++)
			fwrite(&ByteZero, sizeof(ByteZero), 1, ADTemp);
	}
	fseek(ADTemp, 0, SEEK_SET);
	//处理关联数据
	word16 ADi[8], ti[8];
	word16 T_1[8] = { 0 };
	int a = 0;
	while (fread(&ADi, sizeof(word16), 8, ADTemp) == 8) {
		a++;
		processADBlock_16(CIS, ADi, ctr, a, ti);
		for (int i = 0; i < 8; i++)
			T_1[i] = (T_1[i] + ti[i]) & mod_16;
	}
	//更新CIS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			CIS[8 * i + j] ^= T_1[4 * i + j];
		}
	}
	piFunction_16(CIS, CIS);
	fclose(ADTemp);
	remove("ADTemp.txt");
	//处理SMN
	word16 C0[8], T_2[8];
	word32 CipherLength = 0;

	fseek(CipherFile, 0, SEEK_END);
	CipherLength = ftell(CipherFile) - ADLength;
	fseek(CipherFile, ADLength, SEEK_SET);

	fread(&C0, sizeof(word16), 8, CipherFile);
	processSMN4Decryption_16(CIS, C0, ctr, a, SMN, T_1, T_2, CIS);
	//解密密文
	word16 T[8], T_in_Cipher[8];
	word16 Mi[8], Ci[8];
	int m = 1;
	for (int i = 0; i < 8; i++)
		T[i] = T_2[i];

	while (fread(&Ci, sizeof(word16), 8, CipherFile) == 8) {
		processCipherBlock_16(CIS, ctr, a, m, Ci, Mi, ti);
		fwrite(&Mi, sizeof(word16), 8, MessageFile);
		for (int i = 0; i < 8; i++)
			T[i] = (T[i] + ti[i]) & mod_16;
		if (m == ((CipherLength / 16) - 2))
			break;
		else
			m++;
	}
	fread(&T_in_Cipher, sizeof(word16), 8, CipherFile);
	for (int i = 0; i < 8; i++) {
		if (T[i] != T_in_Cipher[i]) {
			fclose(CipherFile);
			fclose(MessageFile);
			remove(MessageFileName);
			return AUTHEN_FAILED;
		}
		else
			Authen[i] = T[i];
	}
	fclose(CipherFile);
	//将填充内容改为全部都为0x00
	fseek(MessageFile, -16L, SEEK_END);
	word8 padding[16];
	if ((fread(&padding, sizeof(word8), 16, MessageFile)) == 16) {
		for (int i = 15; i >= 0; i--) {
			if (padding[i] == 0x01) {
				padding[i] = 0x00;
				break;
			}
		}
		fseek(MessageFile, -16L, SEEK_END);
	}
	fwrite(&padding, sizeof(word8), 16, MessageFile);
	fclose(MessageFile);
	return DECRYPT_SUCCEED;
}


/***********    认证加密(32bit字长)    ***********
消息M和关联数据AD均采用文件方式输入
PMN:word32[4], SMN:word32[8], K:word32[4]
*************************************************/
int CPiCipherToolDlg::ae32_encryption(char* MessageFileName, word32 ADLength, char* CipherFileName, word32* PMN, word32* SMN, word32* K, word32* Authen)
{
	word64 ctr;
	word32 CIS[16] = { 0 };
	//初始化
	ctr = initialization_32(K, PMN, CIS);
	//消息、关联数据填充
	word32 MessageLength = 0;
	word8 ByteOne = 0x01;
	word8 ByteZero = 0x00;
	FILE *MessageFile, *MessageTemp, *ADTemp, *CipherFile;
	if ((MessageFile = fopen(MessageFileName, "rb+")) == NULL)
		return MSG_FILE_OPEN_FAILED;
	if ((MessageTemp = fopen("MessageTemp.txt", "wb+")) == NULL)
		return FILE_OPEN_FAILED;
	if ((ADTemp = fopen("ADTemp.txt", "wb+")) == NULL)
		return FILE_OPEN_FAILED;
	if ((CipherFile = fopen(CipherFileName, "wb+")) == NULL)
		return CIPHER_FILE_OPEN_FAILED;
	word8 byte;
	for (word32 i = 0; i < ADLength; i++) {
		if (fread(&byte, sizeof(byte), 1, MessageFile) != 1) {
			_fcloseall();
			remove("MessageTemp.txt");
			remove("ADTemp.txt");
			remove(CipherFileName);
			return AD_LENGTH_NOT_ENOUGH;
		}
		fwrite(&byte, sizeof(byte), 1, ADTemp);
		fwrite(&byte, sizeof(byte), 1, CipherFile);
	}
	fseek(MessageFile, 0, SEEK_END);
	MessageLength = ftell(MessageFile) - ADLength;
	fseek(MessageFile, ADLength, SEEK_SET);
	while (fread(&byte, sizeof(byte), 1, MessageFile) == 1)
		fwrite(&byte, sizeof(byte), 1, MessageTemp);

	if (MessageLength % 32 == 0) {
		fwrite(&ByteOne, sizeof(ByteOne), 1, MessageTemp);
		for (int i = 1; i <= 31; i++)
			fwrite(&ByteZero, sizeof(ByteZero), 1, MessageTemp);
	}
	else {
		fwrite(&ByteOne, sizeof(ByteOne), 1, MessageTemp);
		for (unsigned int i = 1; i <= 32 - (MessageLength % 32) - 1; i++)
			fwrite(&ByteZero, sizeof(ByteZero), 1, MessageTemp);
	}
	if (ADLength % 32 == 0) {
		fwrite(&ByteOne, sizeof(ByteOne), 1, ADTemp);
		for (int i = 1; i <= 31; i++)
			fwrite(&ByteZero, sizeof(ByteZero), 1, ADTemp);
	}
	else {
		fwrite(&ByteOne, sizeof(ByteOne), 1, ADTemp);
		for (unsigned int i = 1; i <= 32 - (ADLength % 32) - 1; i++)
			fwrite(&ByteZero, sizeof(ByteZero), 1, ADTemp);
	}
	fseek(MessageTemp, 0, SEEK_SET);
	fseek(ADTemp, 0, SEEK_SET);
	//处理关联数据
	word32 ADi[8], ti[8];
	word32 T_1[8] = { 0 };
	int a = 0;
	while (fread(&ADi, sizeof(word32), 8, ADTemp) == 8) {
		a++;
		processADBlock_32(CIS, ADi, ctr, a, ti);
		for (int i = 0; i < 8; i++)
			T_1[i] = (T_1[i] + ti[i]) & mod_32;
	}
	//更新CIS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			CIS[8 * i + j] ^= T_1[4 * i + j];
		}
	}
	piFunction_32(CIS, CIS);
	//处理SMN
	word32 C0[8], T_2[8];
	processSMN4Encryption_32(CIS, SMN, ctr, a, C0, T_1, T_2, CIS);
	fwrite(&C0, sizeof(word32), 8, CipherFile);
	//加密消息
	word32 T[8];
	word32 Mi[8], Ci[8];
	int m = 0;
	for (int i = 0; i < 8; i++)
		T[i] = T_2[i];
	while (fread(&Mi, sizeof(word32), 8, MessageTemp) == 8) {
		m++;
		processMessageBlock_32(CIS, ctr, a, m, Ci, Mi, ti);
		fwrite(&Ci, sizeof(word32), 8, CipherFile);
		for (int i = 0; i < 8; i++)
			T[i] = (T[i] + ti[i]) & mod_32;
	}
	fwrite(&T, sizeof(word32), 8, CipherFile);
	for (int i = 0; i < 8; i++)
		Authen[i] = T[i];
	fclose(MessageTemp);
	fclose(ADTemp);
	remove("ADTemp.txt");
	remove("MessageTemp.txt");
	fclose(CipherFile);
	return ENCRYPT_SUCCEED;
}


/***********    解密验证(32bit字长)    ***********
密文C和关联数据AD均采用文件方式输入
PMN:word32[4], K:word32[4]
*************************************************/
int CPiCipherToolDlg::ae32_decryption(char* CipherFileName, word32 ADLength, char* MessageFileName, word32* PMN, word32* SMN, word32* K, word32* Authen)
{
	word64 ctr;
	word32 CIS[16] = { 0 };
	//初始化
	ctr = initialization_32(K, PMN, CIS);
	//关联数据填充
	word8 ByteOne = 0x01;
	word8 ByteZero = 0x00;
	FILE *CipherFile, *ADTemp, *MessageFile;
	if ((CipherFile = fopen(CipherFileName, "rb+")) == NULL)
		return CIPHER_FILE_OPEN_FAILED;
	if ((ADTemp = fopen("ADTemp.txt", "wb+")) == NULL)
		return FILE_OPEN_FAILED;
	if ((MessageFile = fopen(MessageFileName, "wb+")) == NULL)
		return MSG_FILE_OPEN_FAILED;

	word8 byte;
	for (word32 i = 0; i < ADLength; i++) {
		fread(&byte, sizeof(byte), 1, CipherFile);
		fwrite(&byte, sizeof(byte), 1, ADTemp);
		fwrite(&byte, sizeof(byte), 1, MessageFile);
	}
	if (ADLength % 32 == 0) {
		fwrite(&ByteOne, sizeof(ByteOne), 1, ADTemp);
		for (int i = 1; i <= 31; i++)
			fwrite(&ByteZero, sizeof(ByteZero), 1, ADTemp);
	}
	else {
		fwrite(&ByteOne, sizeof(ByteOne), 1, ADTemp);
		for (unsigned int i = 1; i <= 32 - (ADLength % 32) - 1; i++)
			fwrite(&ByteZero, sizeof(ByteZero), 1, ADTemp);
	}
	fseek(ADTemp, 0, SEEK_SET);
	//处理关联数据
	word32 ADi[8], ti[8];
	word32 T_1[8] = { 0 };
	int a = 0;
	while (fread(&ADi, sizeof(word32), 8, ADTemp) == 8) {
		a++;
		processADBlock_32(CIS, ADi, ctr, a, ti);
		for (int i = 0; i < 8; i++)
			T_1[i] = (T_1[i] + ti[i]) & mod_32;
	}
	//更新CIS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			CIS[8 * i + j] ^= T_1[4 * i + j];
		}
	}
	piFunction_32(CIS, CIS);
	fclose(ADTemp);
	remove("ADTemp.txt");
	//处理SMN
	word32 C0[8], T_2[8];
	word32 CipherLength = 0;

	fseek(CipherFile, 0, SEEK_END);
	CipherLength = ftell(CipherFile) - ADLength;
	fseek(CipherFile, ADLength, SEEK_SET);

	fread(&C0, sizeof(word32), 8, CipherFile);
	processSMN4Decryption_32(CIS, C0, ctr, a, SMN, T_1, T_2, CIS);
	//解密密文
	word32 T[8], T_in_Cipher[8];
	word32 Mi[8], Ci[8];
	int m = 1;
	for (int i = 0; i < 8; i++)
		T[i] = T_2[i];

	while (fread(&Ci, sizeof(word32), 8, CipherFile) == 8) {
		processCipherBlock_32(CIS, ctr, a, m, Ci, Mi, ti);
		fwrite(&Mi, sizeof(word32), 8, MessageFile);
		for (int i = 0; i < 8; i++)
			T[i] = (T[i] + ti[i]) & mod_32;
		if (m == ((CipherLength / 32) - 2))
			break;
		else
			m++;
	}
	fread(&T_in_Cipher, sizeof(word32), 8, CipherFile);
	for (int i = 0; i < 8; i++) {
		if (T[i] != T_in_Cipher[i]) {
			fclose(CipherFile);
			fclose(MessageFile);
			remove(MessageFileName);
			return AUTHEN_FAILED;
		}
		else
			Authen[i] = T[i];
	}
	fclose(CipherFile);
	//将填充内容改为全部都为0x00
	fseek(MessageFile, -32L, SEEK_END);
	word8 padding[32];
	if ((fread(&padding, sizeof(word8), 32, MessageFile)) == 32) {
		for (int i = 31; i >= 0; i--) {
			if (padding[i] == 0x01) {
				padding[i] = 0x00;
				break;
			}
		}
		fseek(MessageFile, -32L, SEEK_END);
	}
	fwrite(&padding, sizeof(word8), 32, MessageFile);
	fclose(MessageFile);
	return DECRYPT_SUCCEED;
}


/***********    认证加密(64bit字长 128bit密钥)    ***********
消息M和关联数据AD均采用文件方式输入
PMN:word64[2], SMN:word64[8], K:word64[2]
************************************************************/
int CPiCipherToolDlg::ae64_128_encryption(char* MessageFileName, word32 ADLength, char* CipherFileName, word64* PMN, word64* SMN, word64* K, word64* Authen)
{
	word64 ctr;
	word64 CIS[16] = { 0 };
	//初始化
	ctr = initialization_64_128(K, PMN, CIS);
	//消息、关联数据填充
	word32 MessageLength = 0;
	word8 ByteOne = 0x01;
	word8 ByteZero = 0x00;
	FILE *MessageFile, *MessageTemp, *ADTemp, *CipherFile;
	if ((MessageFile = fopen(MessageFileName, "rb+")) == NULL)
		return MSG_FILE_OPEN_FAILED;
	if ((MessageTemp = fopen("MessageTemp.txt", "wb+")) == NULL)
		return FILE_OPEN_FAILED;
	if ((ADTemp = fopen("ADTemp.txt", "wb+")) == NULL)
		return FILE_OPEN_FAILED;
	if ((CipherFile = fopen(CipherFileName, "wb+")) == NULL)
		return CIPHER_FILE_OPEN_FAILED;
	word8 byte;
	for (word32 i = 0; i < ADLength; i++) {
		if (fread(&byte, sizeof(byte), 1, MessageFile) != 1) {
			_fcloseall();
			remove("MessageTemp.txt");
			remove("ADTemp.txt");
			remove(CipherFileName);
			return AD_LENGTH_NOT_ENOUGH;
		}
		fwrite(&byte, sizeof(byte), 1, ADTemp);
		fwrite(&byte, sizeof(byte), 1, CipherFile);
	}
	fseek(MessageFile, 0, SEEK_END);
	MessageLength = ftell(MessageFile) - ADLength;
	fseek(MessageFile, ADLength, SEEK_SET);
	while (fread(&byte, sizeof(byte), 1, MessageFile) == 1)
		fwrite(&byte, sizeof(byte), 1, MessageTemp);

	if (MessageLength % 64 == 0) {
		fwrite(&ByteOne, sizeof(ByteOne), 1, MessageTemp);
		for (int i = 1; i <= 63; i++)
			fwrite(&ByteZero, sizeof(ByteZero), 1, MessageTemp);
	}
	else {
		fwrite(&ByteOne, sizeof(ByteOne), 1, MessageTemp);
		for (unsigned int i = 1; i <= 64 - (MessageLength % 64) - 1; i++)
			fwrite(&ByteZero, sizeof(ByteZero), 1, MessageTemp);
	}
	if (ADLength % 64 == 0) {
		fwrite(&ByteOne, sizeof(ByteOne), 1, ADTemp);
		for (int i = 1; i <= 63; i++)
			fwrite(&ByteZero, sizeof(ByteZero), 1, ADTemp);
	}
	else {
		fwrite(&ByteOne, sizeof(ByteOne), 1, ADTemp);
		for (unsigned int i = 1; i <= 64 - (ADLength % 64) - 1; i++)
			fwrite(&ByteZero, sizeof(ByteZero), 1, ADTemp);
	}
	fseek(MessageTemp, 0, SEEK_SET);
	fseek(ADTemp, 0, SEEK_SET);
	//处理关联数据
	word64 ADi[8], ti[8];
	word64 T_1[8] = { 0 };
	int a = 0;
	while (fread(&ADi, sizeof(word64), 8, ADTemp) == 8) {
		a++;
		processADBlock_64(CIS, ADi, ctr, a, ti);
		for (int i = 0; i < 8; i++)
			T_1[i] = (T_1[i] + ti[i]) & mod;
	}
	//更新CIS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			CIS[8 * i + j] ^= T_1[4 * i + j];
		}
	}
	piFunction_64(CIS, CIS);
	//处理SMN
	word64 C0[8], T_2[8];
	processSMN4Encryption_64(CIS, SMN, ctr, a, C0, T_1, T_2, CIS);
	fwrite(&C0, sizeof(word64), 8, CipherFile);
	//加密消息
	word64 T[8];
	word64 Mi[8], Ci[8];
	int m = 0;
	for (int i = 0; i < 8; i++)
		T[i] = T_2[i];
	while (fread(&Mi, sizeof(word64), 8, MessageTemp) == 8) {
		m++;
		processMessageBlock_64(CIS, ctr, a, m, Ci, Mi, ti);
		fwrite(&Ci, sizeof(word64), 8, CipherFile);
		for (int i = 0; i < 8; i++)
			T[i] = (T[i] + ti[i]) & mod;
	}
	fwrite(&T, sizeof(word64), 8, CipherFile);
	for (int i = 0; i < 8; i++)
		Authen[i] = T[i];
	fclose(MessageTemp);
	fclose(ADTemp);
	remove("ADTemp.txt");
	remove("MessageTemp.txt");
	fclose(CipherFile);
	return ENCRYPT_SUCCEED;
}


/***********    解密验证(64bit字长 128bit密钥)    ***********
密文C和关联数据AD均采用文件方式输入
PMN:word64[2], K:word64[2]
************************************************************/
int CPiCipherToolDlg::ae64_128_decryption(char* CipherFileName, word32 ADLength, char* MessageFileName, word64* PMN, word64* SMN, word64* K, word64* Authen)
{
	word64 ctr;
	word64 CIS[16] = { 0 };
	//初始化
	ctr = initialization_64_128(K, PMN, CIS);
	//关联数据填充
	word8 ByteOne = 0x01;
	word8 ByteZero = 0x00;
	FILE *CipherFile, *ADTemp, *MessageFile;
	if ((CipherFile = fopen(CipherFileName, "rb+")) == NULL)
		return CIPHER_FILE_OPEN_FAILED;
	if ((ADTemp = fopen("ADTemp.txt", "wb+")) == NULL)
		return FILE_OPEN_FAILED;
	if ((MessageFile = fopen(MessageFileName, "wb+")) == NULL)
		return MSG_FILE_OPEN_FAILED;

	word8 byte;
	for (word32 i = 0; i < ADLength; i++) {
		fread(&byte, sizeof(byte), 1, CipherFile);
		fwrite(&byte, sizeof(byte), 1, ADTemp);
		fwrite(&byte, sizeof(byte), 1, MessageFile);
	}
	if (ADLength % 64 == 0) {
		fwrite(&ByteOne, sizeof(ByteOne), 1, ADTemp);
		for (int i = 1; i <= 63; i++)
			fwrite(&ByteZero, sizeof(ByteZero), 1, ADTemp);
	}
	else {
		fwrite(&ByteOne, sizeof(ByteOne), 1, ADTemp);
		for (unsigned int i = 1; i <= 64 - (ADLength % 64) - 1; i++)
			fwrite(&ByteZero, sizeof(ByteZero), 1, ADTemp);
	}
	fseek(ADTemp, 0, SEEK_SET);
	//处理关联数据
	word64 ADi[8], ti[8];
	word64 T_1[8] = { 0 };
	int a = 0;
	while (fread(&ADi, sizeof(word64), 8, ADTemp) == 8) {
		a++;
		processADBlock_64(CIS, ADi, ctr, a, ti);
		for (int i = 0; i < 8; i++)
			T_1[i] = (T_1[i] + ti[i]) & mod;
	}
	//更新CIS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			CIS[8 * i + j] ^= T_1[4 * i + j];
		}
	}
	piFunction_64(CIS, CIS);
	fclose(ADTemp);
	remove("ADTemp.txt");
	//处理SMN
	word64 C0[8], T_2[8];
	word64 CipherLength = 0;

	fseek(CipherFile, 0, SEEK_END);
	CipherLength = ftell(CipherFile) - ADLength;
	fseek(CipherFile, ADLength, SEEK_SET);

	fread(&C0, sizeof(word64), 8, CipherFile);
	processSMN4Decryption_64(CIS, C0, ctr, a, SMN, T_1, T_2, CIS);
	//解密密文
	word64 T[8], T_in_Cipher[8];
	word64 Mi[8], Ci[8];
	int m = 1;
	for (int i = 0; i < 8; i++)
		T[i] = T_2[i];

	while (fread(&Ci, sizeof(word64), 8, CipherFile) == 8) {
		processCipherBlock_64(CIS, ctr, a, m, Ci, Mi, ti);
		fwrite(&Mi, sizeof(word64), 8, MessageFile);
		for (int i = 0; i < 8; i++)
			T[i] = (T[i] + ti[i]) & mod;
		if (m == ((CipherLength / 64) - 2))
			break;
		else
			m++;
	}
	fread(&T_in_Cipher, sizeof(word64), 8, CipherFile);
	for (int i = 0; i < 8; i++) {
		if (T[i] != T_in_Cipher[i]) {
			fclose(CipherFile);
			fclose(MessageFile);
			remove(MessageFileName);
			return AUTHEN_FAILED;
		}
		else
			Authen[i] = T[i];
	}
	fclose(CipherFile);
	//将填充内容改为全部都为0x00
	fseek(MessageFile, -64L, SEEK_END);
	word8 padding[64];
	if ((fread(&padding, sizeof(word8), 64, MessageFile)) == 64) {
		for (int i = 63; i >= 0; i--) {
			if (padding[i] == 0x01) {
				padding[i] = 0x00;
				break;
			}
		}
		fseek(MessageFile, -64L, SEEK_END);
	}
	fwrite(&padding, sizeof(word8), 64, MessageFile);
	fclose(MessageFile);
	return DECRYPT_SUCCEED;
}


/***********    认证加密(64bit字长 256bit密钥)    ***********
消息M和关联数据AD均采用文件方式输入
PMN:word64[2], SMN:word64[8], K:word64[4]
************************************************************/
int CPiCipherToolDlg::ae64_256_encryption(char* MessageFileName, word32 ADLength, char* CipherFileName, word64* PMN, word64* SMN, word64* K, word64* Authen)
{
	word64 ctr;
	word64 CIS[16] = { 0 };
	//初始化
	ctr = initialization_64_256(K, PMN, CIS);
	//消息、关联数据填充
	word32 MessageLength = 0;
	word8 ByteOne = 0x01;
	word8 ByteZero = 0x00;
	FILE *MessageFile, *MessageTemp, *ADTemp, *CipherFile;
	if ((MessageFile = fopen(MessageFileName, "rb+")) == NULL)
		return MSG_FILE_OPEN_FAILED;
	if ((MessageTemp = fopen("MessageTemp.txt", "wb+")) == NULL)
		return FILE_OPEN_FAILED;
	if ((ADTemp = fopen("ADTemp.txt", "wb+")) == NULL)
		return FILE_OPEN_FAILED;
	if ((CipherFile = fopen(CipherFileName, "wb+")) == NULL)
		return CIPHER_FILE_OPEN_FAILED;
	word8 byte;
	for (word32 i = 0; i < ADLength; i++) {
		if (fread(&byte, sizeof(byte), 1, MessageFile) != 1) {
			_fcloseall();
			remove("MessageTemp.txt");
			remove("ADTemp.txt");
			remove(CipherFileName);
			return AD_LENGTH_NOT_ENOUGH;
		}
		fwrite(&byte, sizeof(byte), 1, ADTemp);
		fwrite(&byte, sizeof(byte), 1, CipherFile);
	}
	fseek(MessageFile, 0, SEEK_END);
	MessageLength = ftell(MessageFile) - ADLength;
	fseek(MessageFile, ADLength, SEEK_SET);
	while (fread(&byte, sizeof(byte), 1, MessageFile) == 1)
		fwrite(&byte, sizeof(byte), 1, MessageTemp);

	if (MessageLength % 64 == 0) {
		fwrite(&ByteOne, sizeof(ByteOne), 1, MessageTemp);
		for (int i = 1; i <= 63; i++)
			fwrite(&ByteZero, sizeof(ByteZero), 1, MessageTemp);
	}
	else {
		fwrite(&ByteOne, sizeof(ByteOne), 1, MessageTemp);
		for (unsigned int i = 1; i <= 64 - (MessageLength % 64) - 1; i++)
			fwrite(&ByteZero, sizeof(ByteZero), 1, MessageTemp);
	}
	if (ADLength % 64 == 0) {
		fwrite(&ByteOne, sizeof(ByteOne), 1, ADTemp);
		for (int i = 1; i <= 63; i++)
			fwrite(&ByteZero, sizeof(ByteZero), 1, ADTemp);
	}
	else {
		fwrite(&ByteOne, sizeof(ByteOne), 1, ADTemp);
		for (unsigned int i = 1; i <= 64 - (ADLength % 64) - 1; i++)
			fwrite(&ByteZero, sizeof(ByteZero), 1, ADTemp);
	}
	fseek(MessageTemp, 0, SEEK_SET);
	fseek(ADTemp, 0, SEEK_SET);
	//处理关联数据
	word64 ADi[8], ti[8];
	word64 T_1[8] = { 0 };
	int a = 0;
	while (fread(&ADi, sizeof(word64), 8, ADTemp) == 8) {
		a++;
		processADBlock_64(CIS, ADi, ctr, a, ti);
		for (int i = 0; i < 8; i++)
			T_1[i] = (T_1[i] + ti[i]) & mod;
	}
	//更新CIS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			CIS[8 * i + j] ^= T_1[4 * i + j];
		}
	}
	piFunction_64(CIS, CIS);
	//处理SMN
	word64 C0[8], T_2[8];
	processSMN4Encryption_64(CIS, SMN, ctr, a, C0, T_1, T_2, CIS);
	fwrite(&C0, sizeof(word64), 8, CipherFile);
	//加密消息
	word64 T[8];
	word64 Mi[8], Ci[8];
	int m = 0;
	for (int i = 0; i < 8; i++)
		T[i] = T_2[i];
	while (fread(&Mi, sizeof(word64), 8, MessageTemp) == 8) {
		m++;
		processMessageBlock_64(CIS, ctr, a, m, Ci, Mi, ti);
		fwrite(&Ci, sizeof(word64), 8, CipherFile);
		for (int i = 0; i < 8; i++)
			T[i] = (T[i] + ti[i]) & mod;
	}
	fwrite(&T, sizeof(word64), 8, CipherFile);
	for (int i = 0; i < 8; i++)
		Authen[i] = T[i];
	fclose(MessageTemp);
	fclose(ADTemp);
	remove("ADTemp.txt");
	remove("MessageTemp.txt");
	fclose(CipherFile);
	return ENCRYPT_SUCCEED;
}


/***********    解密验证(64bit字长 256bit密钥)    ***********
密文C和关联数据AD均采用文件方式输入
PMN:word64[2], K:word64[4]
************************************************************/
int CPiCipherToolDlg::ae64_256_decryption(char* CipherFileName, word32 ADLength, char* MessageFileName, word64* PMN, word64* SMN, word64* K, word64* Authen)
{
	word64 ctr;
	word64 CIS[16] = { 0 };
	//初始化
	ctr = initialization_64_256(K, PMN, CIS);
	//关联数据填充
	word8 ByteOne = 0x01;
	word8 ByteZero = 0x00;
	FILE *CipherFile, *ADTemp, *MessageFile;
	if ((CipherFile = fopen(CipherFileName, "rb+")) == NULL)
		return CIPHER_FILE_OPEN_FAILED;
	if ((ADTemp = fopen("ADTemp.txt", "wb+")) == NULL)
		return FILE_OPEN_FAILED;
	if ((MessageFile = fopen(MessageFileName, "wb+")) == NULL)
		return MSG_FILE_OPEN_FAILED;

	word8 byte;
	for (word32 i = 0; i < ADLength; i++) {
		fread(&byte, sizeof(byte), 1, CipherFile);
		fwrite(&byte, sizeof(byte), 1, ADTemp);
		fwrite(&byte, sizeof(byte), 1, MessageFile);
	}
	if (ADLength % 64 == 0) {
		fwrite(&ByteOne, sizeof(ByteOne), 1, ADTemp);
		for (int i = 1; i <= 63; i++)
			fwrite(&ByteZero, sizeof(ByteZero), 1, ADTemp);
	}
	else {
		fwrite(&ByteOne, sizeof(ByteOne), 1, ADTemp);
		for (unsigned int i = 1; i <= 64 - (ADLength % 64) - 1; i++)
			fwrite(&ByteZero, sizeof(ByteZero), 1, ADTemp);
	}
	fseek(ADTemp, 0, SEEK_SET);
	//处理关联数据
	word64 ADi[8], ti[8];
	word64 T_1[8] = { 0 };
	int a = 0;
	while (fread(&ADi, sizeof(word64), 8, ADTemp) == 8) {
		a++;
		processADBlock_64(CIS, ADi, ctr, a, ti);
		for (int i = 0; i < 8; i++)
			T_1[i] = (T_1[i] + ti[i]) & mod;
	}
	//更新CIS
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			CIS[8 * i + j] ^= T_1[4 * i + j];
		}
	}
	piFunction_64(CIS, CIS);
	fclose(ADTemp);
	remove("ADTemp.txt");
	//处理SMN
	word64 C0[8], T_2[8];
	word64 CipherLength = 0;

	fseek(CipherFile, 0, SEEK_END);
	CipherLength = ftell(CipherFile) - ADLength;
	fseek(CipherFile, ADLength, SEEK_SET);

	fread(&C0, sizeof(word64), 8, CipherFile);
	processSMN4Decryption_64(CIS, C0, ctr, a, SMN, T_1, T_2, CIS);
	//解密密文
	word64 T[8], T_in_Cipher[8];
	word64 Mi[8], Ci[8];
	int m = 1;
	for (int i = 0; i < 8; i++)
		T[i] = T_2[i];

	while (fread(&Ci, sizeof(word64), 8, CipherFile) == 8) {
		processCipherBlock_64(CIS, ctr, a, m, Ci, Mi, ti);
		fwrite(&Mi, sizeof(word64), 8, MessageFile);
		for (int i = 0; i < 8; i++)
			T[i] = (T[i] + ti[i]) & mod;
		if (m == ((CipherLength / 64) - 2))
			break;
		else
			m++;
	}
	fread(&T_in_Cipher, sizeof(word64), 8, CipherFile);
	for (int i = 0; i < 8; i++) {
		if (T[i] != T_in_Cipher[i]) {
			fclose(CipherFile);
			fclose(MessageFile);
			remove(MessageFileName);
			return AUTHEN_FAILED;
		}
		else
			Authen[i] = T[i];
	}
	fclose(CipherFile);
	//将填充内容改为全部都为0x00
	fseek(MessageFile, -64L, SEEK_END);
	word8 padding[64];
	if ((fread(&padding, sizeof(word8), 64, MessageFile)) == 64) {
		for (int i = 63; i >= 0; i--) {
			if (padding[i] == 0x01) {
				padding[i] = 0x00;
				break;
			}
		}
		fseek(MessageFile, -64L, SEEK_END);
	}
	fwrite(&padding, sizeof(word8), 64, MessageFile);
	fclose(MessageFile);
	return DECRYPT_SUCCEED;
}


void CPiCipherToolDlg::OnBnClickedButtonDemo1()
{
	// TODO: 在此添加控件通知处理程序代码
	editCtrl_SMN.SetReadOnly(0);
	UpdateData(TRUE);
	editVal_PMN = _T("");
	editVal_SMN = _T("");
	editVal_Key = _T("");
	
	if (comboVal_CihperType == _T("π16-Cipher096")) {
			editVal_PMN = _T("8d5ec3ef");
			editVal_SMN = _T("123412c45bd84567565ec38978182c55");
			editVal_Key = _T("32904567ab56904567567890");
	}
	if (comboVal_CihperType == _T("π32-Cipher128")) {
		editVal_PMN = _T("42d45e182c556a813e577d91436d8c2a");
		editVal_SMN = _T("223492c45bd84567565ec36978142c55323412c45bd14567565ec38978182c55");
		editVal_Key = _T("643456904567ab8934567812c45bd80a");
	}
	if (comboVal_CihperType == _T("π64-Cipher128")) {
		editVal_PMN = _T("5294531c24556a81305e2d71436d8c2a");
		editVal_SMN = _T("323442c45bd84567165e036978442c55313442c45ba145675d5ec389781f2c55223492c45bd84567565ec36978142c55323412c45bd14567565ec38978182c55");
		editVal_Key = _T("b94567ab123456784567ab8923456789");
	}
	if(comboVal_CihperType == _T("π64-Cipher256")) {
		editVal_PMN = _T("f4263812c45bd895437749034a37813a");
		editVal_SMN = _T("623442c453384567135e333978342c55313432c45ba145675d5ec389781f2355223492c45b484567565ac36173142c55323a12245bd14567565ec38978182c55");
		editVal_Key = _T("456756781234567823454567ab456789124567abd834567890454567ab456789");
	}

	UpdateData(FALSE);
}


void CPiCipherToolDlg::OnBnClickedButtonDemo2()
{
	// TODO: 在此添加控件通知处理程序代码
	editCtrl_SMN.SetReadOnly();
	UpdateData(TRUE);
	editVal_PMN = _T("");
	editVal_SMN = _T("");
	editVal_Key = _T("");

	
	if (comboVal_CihperType == _T("π16-Cipher096")) {
		editVal_PMN = _T("8d5ec3ef");
		editVal_Key = _T("32904567ab56904567567890");
	}
	if (comboVal_CihperType == _T("π32-Cipher128")) {
		editVal_PMN = _T("42d45e182c556a813e577d91436d8c2a");
		editVal_Key = _T("643456904567ab8934567812c45bd80a");
	}
	if (comboVal_CihperType == _T("π64-Cipher128")) {
		editVal_PMN = _T("5294531c24556a81305e2d71436d8c2a");
		editVal_Key = _T("b94567ab123456784567ab8923456789");
	}
	if (comboVal_CihperType == _T("π64-Cipher256")) {
		editVal_PMN = _T("f4263812c45bd895437749034a37813a");
		editVal_Key = _T("456756781234567823454567ab456789124567abd834567890454567ab456789");
	}

	UpdateData(FALSE);
}


void CPiCipherToolDlg::cstring2wordArray(CString src, word16* dst)
{
	BYTE transform[16];
	int num = 0;
	BYTE high, low;
	unsigned short temp;
	for (int i = 0; i < src.GetLength(); i++) {
		temp = src.GetAt(i);
		high = (temp & 0xff00) >> 8;
		low = temp & 0x00ff;
		if (high != 0x00)
			transform[num++] = high;
		transform[num++] = low;
	}
	for (int i = 0; i < num / 2; i++)
		dst[i] = (transform[2 * i] << 8) | transform[2 * i + 1];
}


void CPiCipherToolDlg::cstring2wordArray(CString src, word32* dst)
{
	BYTE transform[32];
	int num = 0;
	BYTE high, low;
	unsigned short temp;
	for (int i = 0; i < src.GetLength(); i++) {
		temp = src.GetAt(i);
		high = (temp & 0xff00) >> 8;
		low = temp & 0x00ff;
		if (high != 0x00)
			transform[num++] = high;
		transform[num++] = low;
	}
	for (int i = 0; i < num / 4; i++)
		dst[i] = (transform[4 * i] << 24) | (transform[4 * i + 1] << 16) | (transform[4 * i + 2] << 8) | transform[4 * i + 3];
}


void CPiCipherToolDlg::cstring2wordArray(CString src, word64* dst)
{
	BYTE transform[64];
	int num = 0;
	BYTE high, low;
	unsigned short temp;
	for (int i = 0; i < src.GetLength(); i++) {
		temp = src.GetAt(i);
		high = (temp & 0xff00) >> 8;
		low = temp & 0x00ff;
		if (high != 0x00)
			transform[num++] = high;
		transform[num++] = low;
	}
	for (int i = 0; i < num / 8; i++)
		dst[i] = (transform[8 * i] << 56) | (transform[8 * i + 1] << 48) | (transform[8 * i + 2] << 40) | (transform[8 * i + 3] << 32) | 
				 (transform[8 * i + 4] << 24) | (transform[8 * i + 5] << 16) | (transform[8 * i + 6] << 8) | transform[8 * i + 7];
}


void CPiCipherToolDlg::OnBnClickedButtonEmpty()
{
	// TODO: 在此添加控件通知处理程序代码
	editVal_Authen = _T("");
	editVal_DecSMN = _T("");
	UpdateData(FALSE);
}


BYTE CPiCipherToolDlg::Hex2Byte(BYTE value)
{
	if ((value >= '0') && (value <= '9'))
		return value - '0';
	else if ((value >= 'A') && (value <= 'F'))
		return	value - 'A' + 10;
	else if ((value >= 'a') && (value <= 'f'))
		return	value - 'a' + 10;
	else return -1;
}


void CPiCipherToolDlg::HexString_to_WordArray(CString src, word16* dst)
{
	BYTE transform[16];
	BYTE high, low;
	for (int i = 0; i < src.GetLength() / 2; i++) {
		high = Hex2Byte(src.GetAt(2 * i));
		low = Hex2Byte(src.GetAt(2 * i + 1));
		transform[i] = (high << 4) + (low & 0xf);
	}
	
	for (int i = 0; i < src.GetLength() / 4; i++)
		dst[i] = (transform[2 * i] << 8) | transform[2 * i + 1];
}


void CPiCipherToolDlg::HexString_to_WordArray(CString src, word32* dst)
{
	BYTE transform[32];
	BYTE high, low;
	for (int i = 0; i < src.GetLength() / 2; i++) {
		high = Hex2Byte(src.GetAt(2 * i));
		low = Hex2Byte(src.GetAt(2 * i + 1));
		transform[i] = (high << 4) + (low & 0xf);
	}

	for (int i = 0; i < src.GetLength() / 8; i++)
		dst[i] = (transform[4 * i] << 24) | (transform[4 * i + 1] << 16) | (transform[4 * i + 2] << 8) | transform[4 * i + 3];
}


void CPiCipherToolDlg::HexString_to_WordArray(CString src, word64* dst)
{
	BYTE transform[64];
	BYTE high, low;
	for (int i = 0; i < src.GetLength() / 2; i++) {
		high = Hex2Byte(src.GetAt(2 * i));
		low = Hex2Byte(src.GetAt(2 * i + 1));
		transform[i] = (high << 4) + (low & 0xf);
	}

	for (int i = 0; i < src.GetLength() / 16; i++)
		dst[i] = (transform[8 * i] << 56) | (transform[8 * i + 1] << 48) | (transform[8 * i + 2] << 40) | (transform[8 * i + 3] << 32) |
				 (transform[8 * i + 4] << 24) | (transform[8 * i + 5] << 16) | (transform[8 * i + 6] << 8) | transform[8 * i + 7];
}

