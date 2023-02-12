
// ASMConverterDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "ASMConverter.h"
#include "ASMConverterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#include <keystone/keystone.h>
#include"../../../include/keystone/keystone.h"


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


// CASMConverterDlg 对话框



CASMConverterDlg::CASMConverterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ASMCONVERTER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CASMConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_hexout);
	DDX_Control(pDX, IDC_EDIT2, m_asm);
	DDX_Control(pDX, IDC_EDIT3, m_adress);
}

BEGIN_MESSAGE_MAP(CASMConverterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CASMConverterDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT2, &CASMConverterDlg::OnEnChangeEdit2)
//	ON_BN_CLICKED(IDC_BUTTON1, &CASMConverterDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CASMConverterDlg 消息处理程序

BOOL CASMConverterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CASMConverterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CASMConverterDlg::OnPaint()
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
HCURSOR CASMConverterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CASMConverterDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

}

QWORD CTOD(CString m_Send)
{
	char* stops, s[3] = { 0 };
	unsigned char  d[16] = { 0 };
	DWORD realLen = m_Send.GetLength();
	DWORD len = m_Send.GetLength() / 2 + m_Send.GetLength() % 2;
	bool bFlag = FALSE;
	if (m_Send.GetLength() % 2 == 1)
	{
		bFlag = TRUE;
	}
	DWORD i = 0;

	while (len)
	{
		if (len == 1 && bFlag)
		{
			s[0] = m_Send.GetAt(realLen - i * 2 - 1);
			s[1] = 0;
			s[2] = 0x0;
			d[i] = (unsigned char)strtoul(s, &stops, 16);
			return *(DWORD*)d;
		}

		s[0] = m_Send.GetAt(realLen - i * 2 - 2);
		s[1] = m_Send.GetAt(realLen - i * 2 - 1);
		s[2] = 0x0;
		d[i] = (unsigned char)strtoul(s, &stops, 16);
		i++;
		len--;
	}
	return *(QWORD*)d;
}

void CASMConverterDlg::OnEnChangeEdit2()
{
	CString outbuff;
	ks_engine* ks;
	ks_err err;
	size_t count;
	unsigned char* encode;
	size_t size;

	err = ks_open(KS_ARCH_X86, KS_MODE_64, &ks);
	if (err == KS_ERR_OK) {
		if (0)
			ks_option(ks, KS_OPT_SYNTAX, 0);


		CString asmstring;
		m_asm.GetWindowTextW(asmstring);

		CString adressstr;
		m_adress.GetWindowTextW(adressstr);
		QWORD ad = CTOD(adressstr);

		adressstr.Format(L"0x%p", ad);
		m_adress.SetWindowTextW(adressstr);
		
		//m_adress.GetWindowTextW()
		if (ks_asm(ks, CW2A(asmstring),ad, &encode, &size, &count)) {

			outbuff.Format(L"ERROR: failed on ks_asm() with count = %lu, error code = %u\n", count, ks_errno(ks));
		}
		else {

			size_t i;
			for (i = 0; i < size; i++) {
				//printf("%02x ", encode[i]);
				outbuff.Format(L"%s%02X ", outbuff, encode[i]);
			}
			outbuff.Format(L"%s \r\nAssembled: %lu bytes, %lu statements\n\n", outbuff, size, count);
		}

		// NOTE: free encode after usage to avoid leaking memory
		ks_free(encode);

		// close Keystone instance when done
		ks_close(ks);
		m_hexout.SetWindowTextW(outbuff);

	}
	else
	{
		outbuff = L"ERROR: failed on ks_open(), quit\n";
	}
	
	// TODO:  在此添加控件通知处理程序代码
}



void CASMConverterDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
}
