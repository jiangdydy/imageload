
// ImageLoadDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "ImageLoad.h"
#include "ImageLoadDlg.h"
#include "afxdialogex.h"
#include <cv.h>
#include <highgui.h>
#include "cv.h"  
#include<string>
using namespace cv;
using namespace std;
using namespace cv::dnn;
#pragma comment( lib, "vfw32.lib" )  
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// CImageLoadDlg 对话框
CImageLoadDlg::CImageLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_IMAGELOAD_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImageLoadDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Folders, &CImageLoadDlg::OnBnClickedFolders)
	ON_BN_CLICKED(IDC_Buttonnext, &CImageLoadDlg::OnBnClickedButtonnext)
	ON_BN_CLICKED(IDC_BEFORE, &CImageLoadDlg::OnBnClickedBefore)
	//	ON_BN_CLICKED(IDCANCEL, &CImageLoadDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_SSD, &CImageLoadDlg::OnBnClickedSsd)
	ON_BN_CLICKED(ID_CANCEL, &CImageLoadDlg::OnBnClickedCancel)
END_MESSAGE_MAP()
// CImageLoadDlg 消息处理程序
BOOL CImageLoadDlg::OnInitDialog()
{

	namedWindow("view", WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle("view");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_ShowImg)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);


	// TODO: Add extra initialization here
	CDialogEx::OnInitDialog();
	m_pwnd = GetDlgItem(IDC_ShowImg);//通过图像控件ID号获取它的窗口指针
	m_pDC = m_pwnd->GetDC();//通过窗口指针获得绘制句柄
	m_hDC = m_pDC->GetSafeHdc();//通过绘制句柄获取其HDC
	m_HDCImg = m_hDC;
	m_pwnd->GetClientRect(&m_RectImg);//获取图像控件的客户区域坐标保存在m_RectImg中
	return TRUE;  // return TRUE  unless you set the focus to a control
				  //CDialogEx::OnInitDialog();
				  //// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
				  ////  执行此操作
				  //SetIcon(m_hIcon, TRUE);			// 设置大图标
				  //SetIcon(m_hIcon, FALSE);		// 设置小图标
				  //// TODO: 在此添加额外的初始化代码
				  //return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CImageLoadDlg::OnPaint()
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
HCURSOR CImageLoadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CImageLoadDlg::OnBnClickedFolders()
{
	// TODO: 在此添加控件通知处理程序代码
	//显示窗口，获取被搜索文件路径
	CString str;
	BROWSEINFO bi;
	TCHAR name[MAX_PATH];
	name[0] = 'd';
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = GetSafeHwnd();
	bi.pszDisplayName = name;

	bi.lpszTitle = _T("Select folder");//窗口标题
	bi.ulFlags = 0x80;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi); //浏览文件
	if (idl == NULL)
		return;
	SHGetPathFromIDList(idl, str.GetBuffer(MAX_PATH));
	str.ReleaseBuffer();
	m_Path = str;//文件路径存储在m_Path中，m_Path为Dlg类CString型成员变量
				 //至此我们已经获取到浏览的文件夹路径
	if (str.GetAt(str.GetLength() - 1) != '\\')
		m_Path += "\\";
	UpdateData(false);
	//	m_ImageDir = (LPSTR)(LPCSTR)m_Path;
	m_ImageDir = m_Path.GetBuffer(m_Path.GetLength());//把路径存储在m_ImageDir中，CString转换为char*
													  //把路径存储在m_ImageDir中，CString转换为char*
													  //将图像的dir设置到界面  
	SetDlgItemText(IDC_EDIT1, m_ImageDir);//IDC_EDIT1编辑框ID
										  //获取给路径下的第一个文件  
	m_pDir = opendir(m_ImageDir);//opendir函数获取文件夹，返回一个DIR,在头文件dirent.h中定义
	for (int i = 0; i < 2; i++)//文件夹下有默认的两个路径，"cd ../","cd ./"过滤掉两个点和一个点的目录
	{
		m_pEnt = readdir(m_pDir);//获取每个文件夹下每个文件的信息，返回一个dirent类型结构体，同样在头文件//dirent.h中定义
	}
	//	GetNextBigImg();
	int sum = 0;
		memset(images_name, 0, sizeof(images_name));
	while (m_pDir && (m_pEnt = readdir(m_pDir)) != NULL)
	{
		int flag = 0;
		//判断名字中有没有.jpg .bmp .png等照片格式
		char* pJpg = strstr(m_pEnt->d_name, ".jpg");
		if (pJpg != NULL)
			flag = 1;
		char* pBmp = strstr(m_pEnt->d_name, ".bmp");
		if (pBmp != NULL)
			flag = 1;
		char* pPng = strstr(m_pEnt->d_name, ".png");
		if (pPng != NULL)
			flag = 1;
		char* pJPG = strstr(m_pEnt->d_name, ".JPG");
		if (pJPG != NULL)
			flag = 1;
		char* pdib = strstr(m_pEnt->d_name, ".dib");
		if (pdib != NULL)
			flag = 1;
		char* pjpe = strstr(m_pEnt->d_name, ".jpe");
		if (pjpe != NULL)
			flag = 1;
		char* pjp2 = strstr(m_pEnt->d_name, ".jp2");
		if (pjp2 != NULL)
			flag = 1;
		char* ppbm = strstr(m_pEnt->d_name, ".pbm");
		if (ppbm != NULL)
			flag = 1;
		char* ppgm = strstr(m_pEnt->d_name, ".pgm");
		if (ppgm != NULL)
			flag = 1;
		char* pppm = strstr(m_pEnt->d_name, ".ppm");
		if (pppm != NULL)
			flag = 1;
		char* psr = strstr(m_pEnt->d_name, ".sr");
		if (psr != NULL)
			flag = 1;
		char* pras = strstr(m_pEnt->d_name, ".ras");
		if (pras != NULL)
			flag = 1;
		char* ptiff = strstr(m_pEnt->d_name, ".tiff");
		if (ptiff != NULL)
			flag = 1;
		char* ptif = strstr(m_pEnt->d_name, ".tif");
		if (ptif != NULL)
			flag = 1;
		if (flag == 0)
		{
			;//获取下一个文件
		}
		else//如果文件名是上述之一，就显示它
		{
			sprintf(images_name[sum], "%s", m_pEnt->d_name);
			sum++;
		}
	}
	pos = 0;
	sumimage = sum;
	ShowTheImgSet(images_name[pos]);
}
void CImageLoadDlg::ShowTheImgSet(char* ImageName)
{
	char imageFullName[500];//保存图像文件全路径
	sprintf(imageFullName, "%s%s", m_ImageDir, ImageName);//目录和图片文件名拼接起来->图像文件全路径
	IplImage* src;
	CvvImage srcCvvImg;
	src = cvLoadImage(imageFullName);
	srcCvvImg.CopyOf(src);
	//将src图像画到HDC指向的控件中，图像控件HDC：m_HDCImg
	srcCvvImg.DrawToHDC(m_HDCImg, &m_RectImg);//要获取Picture Control控件的HDC，在对话框初始化时，参见下一节
	cvReleaseImage(&src);
}
void CImageLoadDlg::OnBnClickedButtonnext()
{
	if (pos < sumimage - 1)
	{
		pos++;
		ShowTheImgSet(images_name[pos]);
	}
}

void CImageLoadDlg::OnBnClickedBefore()
{
	if (pos >0)
	{
		pos--;
		ShowTheImgSet(images_name[pos]);
	}
	// TODO: 在此添加控件通知处理程序代码
}
const char* classNames[] = { "background",
"aeroplane", "bicycle", "bird", "boat",
"bottle", "bus", "car", "cat", "chair",
"cow", "diningtable", "dog", "horse",
"motorbike", "person", "pottedplant",
"sheep", "sofa", "train", "tvmonitor" };
void CImageLoadDlg::DoSSD()
{
	char *ImageName = images_name[pos];
	char imageFullName[500];//保存图像文件全路径
	sprintf(imageFullName, "%s%s", m_ImageDir, ImageName);//目录和图片文件名拼接起来->图像文件全路径
	String modelConfiguration = "deploy.prototxt";
	String modelBinary = "VGG_VOC0712_SSD_300x300_iter_120000.caffemodel";
	dnn::Net net = readNetFromCaffe(modelConfiguration, modelBinary);
	if (net.empty())
	{
		cerr << "Can't load network by using the following files: " << endl;
		cerr << "prototxt:   " << modelConfiguration << endl;
		cerr << "caffemodel: " << modelBinary << endl;
		cerr << "Models can be downloaded here:" << endl;
		cerr << "https://github.com/weiliu89/caffe/tree/ssd#models" << endl;
		exit(-1);
	}
	cv::Mat frame;
//	VideoCapture cap;//使用这个在别的电脑上面会有识别不出汉字崩溃的bug
	//cap.open(imageFullName);
	Mat imagedst=imread(imageFullName);
	//cap >> imagedst;
	CRect rect;
	GetDlgItem(IDC_ShowImg)->GetClientRect(&rect);
	Rect dst(rect.left, rect.top, rect.right, rect.bottom);
	resize(imagedst, frame, cv::Size(rect.Width(), rect.Height()));
	if (frame.channels() == 4)
		cvtColor(frame, frame, COLOR_BGRA2BGR);
	Mat inputBlob = blobFromImage(frame, 1.0f, Size(300, 300), Scalar(104, 117, 123), false, false); 
	net.setInput(inputBlob, "data");  //set the network input // ! [Set input blob] // ! [Make forward pass]
	Mat detection = net.forward("detection_out"); //compute output  //! [Make forward pass]
	vector<double> layersTimings;
	double freq = getTickFrequency() / 1000;
	double time = net.getPerfProfile(layersTimings) / freq;
	ostringstream ss;		
	ss << "FPS: " << 1000 / time << " ; time: " << time << " ms";
	putText(frame, ss.str(), Point(20, 20), 0, 0.5, Scalar(0, 0, 255));	
	Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());
	float confidenceThreshold = 0.5;
	for (int i = 0; i < detectionMat.rows; i++)
	{
		float confidence = detectionMat.at<float>(i, 2);
		if (confidence > confidenceThreshold)
		{
			size_t objectClass = (size_t)(detectionMat.at<float>(i, 1));
			int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);	
			int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
			int xRightTop = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
			int yRightTop = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);
			ss.str("");
			ss << confidence;
			String conf(ss.str());
			Rect object(xLeftBottom, yLeftBottom,
					xRightTop - xLeftBottom,
					yRightTop - yLeftBottom);
			rectangle(frame, object, Scalar(0, 255, 0));
			String label = String(classNames[objectClass]) + ": " + conf;
			int baseLine = 0;
			Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
			rectangle(frame, Rect(Point(xLeftBottom, yLeftBottom - labelSize.height),	
				Size(labelSize.width, labelSize.height + baseLine)),
			   	Scalar(255, 255, 255), FILLED);
			putText(frame, label, Point(xLeftBottom, yLeftBottom),
			FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
		 }
	}
	imshow("view", frame);
	//	imshow("detections", frame);
	/*if (waitKey(1) >= 0) break;*/
	// TODO: 在此添加控件通知处理程序代码
}
void CImageLoadDlg::OnBnClickedSsd()
{
	DoSSD();
}
void CImageLoadDlg::OnBnClickedCancel()
{
	exit(0);
	// TODO: 在此添加控件通知处理程序代码
}
