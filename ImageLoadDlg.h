
// ImageLoadDlg.h : 头文件
//

#pragma once


#include "dirent.h"
#include "CvvImage.h"

// CImageLoadDlg 对话框
class CImageLoadDlg : public CDialogEx
{
// 构造
public:
	CImageLoadDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IMAGELOAD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
//	afx_msg void OnBnClickedButton1();//显示选择文件夹窗口
	CString m_Path;    //存储浏览路径
	char* m_ImageDir ;//指向浏览到的路径
					 //读取所有图像相关变量
	DIR * m_pDir;//头文件要包含#include "dirent.h"
	struct dirent *m_pEnt;    //dirent 存储目录中的文件信息（文件名、扩展名等等）
						  //界面显示相关变量
	CWnd *m_pwnd;//指向显示视频内容的窗口指针
	CDC *m_pDC;//显示视频内容的窗口的设备上下文
	HDC m_hDC;//窗口的设备上下文的绘图指针
	HDC m_HDCImg;
	void DoSSD();
	void ShowTheImgSet(char* ImageName);//获取图片集的dir后负责显示一副图像
	CRect  m_RectImg;
	char* pJpg;
	char* pBmp;
	char* pPng;
	char* pJPG;
	int pos;
	int sumimage;
	char images_name[1000][100];
	afx_msg void OnBnClickedFolders();
	afx_msg void OnBnClickedButtonnext();
	afx_msg void OnBnClickedBefore();
//	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedSsd();
	afx_msg void OnBnClickedCancel();
};
