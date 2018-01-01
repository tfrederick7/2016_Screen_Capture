#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <Windows.h>
#include <iostream>

using namespace std;
using namespace cv;

Mat hwnd2mat(HWND hwnd)
{
	HDC hwindowDC, hwindowCompatibleDC;

	int height, width, srcheight, srcwidth;
	HBITMAP hbwindow;
	Mat src;
	BITMAPINFOHEADER  bi;

	hwindowDC = GetDC(hwnd);
	hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
	SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

	RECT windowsize;    // get the height and width of the screen
	GetClientRect(hwnd, &windowsize);

	srcheight = windowsize.bottom;
	srcwidth = windowsize.right;
	height = windowsize.bottom / 1;  //change this to whatever size you want to resize to
	width = windowsize.right / 1;

	src.create(height, width, CV_8UC4);

	// create a bitmap
	hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
	bi.biSize = sizeof(BITMAPINFOHEADER);    //http://msdn.microsoft.com/en-us/library/windows/window/dd183402%28v=vs.85%29.aspx
	bi.biWidth = width;
	bi.biHeight = -height;  //this is the line that makes it draw upside down or not
	bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	// use the previously created device context with the bitmap
	SelectObject(hwindowCompatibleDC, hbwindow);
	// copy from the window device context to the bitmap device context
	StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, 0, 0, srcwidth, srcheight, SRCCOPY); //change SRCCOPY to NOTSRCCOPY for wacky colors !
	GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO *)&bi, DIB_RGB_COLORS);  //copy from hwindowCompatibleDC to hbwindow

																									   // avoid memory leak
	DeleteObject(hbwindow);
	DeleteDC(hwindowCompatibleDC);
	ReleaseDC(hwnd, hwindowDC);

	return src;
}

int main(int argc, char **argv)
{
	HWND hwndDesktop = GetDesktopWindow();
	HWND hwnd = FindWindow(NULL, L"Field Control"); // Yes, the name of the window is "Quake 2", case for case.
	int key = 0, key2=0;
	if (hwnd != 0)
	{
		cout << " You caught the window!" << endl;
	}

	else
	{
		cout << "open the window" << endl;
		key = 30;
	}
	namedWindow("output1", CV_WINDOW_NORMAL);
	namedWindow("output2", CV_WINDOW_NORMAL);
	namedWindow("output3", CV_WINDOW_NORMAL);
	

	while (key != 30)
	{
		Mat src = hwnd2mat(hwnd);
		// you can do some image processing here
		cv::Rect roi_dropDown = cv::Rect(.37*(src.cols), src.rows*.67, src.cols*0.25, src.rows*0.06);
		cv::Mat dropDownIMG;
		dropDownIMG = src(roi_dropDown);
		
		cv::Rect roi_field1 = cv::Rect(.07*dropDownIMG.cols, .4*dropDownIMG.rows, .1*dropDownIMG.cols, .5*dropDownIMG.rows);
		cv::Mat Field_1_IMG = dropDownIMG(roi_field1);
		cv::Scalar field_1 = cv::sum(Field_1_IMG);
		imshow("output1", Field_1_IMG);

		cv::Rect roi_field2 = cv::Rect(.15*dropDownIMG.cols, .4*dropDownIMG.rows, .1*dropDownIMG.cols, .5*dropDownIMG.rows);
		cv::Mat Field_2_IMG = dropDownIMG(roi_field2);
		cv::Scalar field_2 = cv::sum(Field_2_IMG);
		imshow("output2", Field_2_IMG);

		cv::Rect roi_field3 = cv::Rect(.3*dropDownIMG.cols, .4*dropDownIMG.rows, .1*dropDownIMG.cols, .5*dropDownIMG.rows);
		cv::Mat Field_3_IMG = dropDownIMG(roi_field3);
		cv::Scalar field_3 = cv::sum(Field_3_IMG);
		imshow("output3", Field_3_IMG);

		if (field_1 != field_2 && field_1 != field_3) {
			cout << "Set Camera to Field 1" << endl;
		}
		if (field_2 != field_1 && field_2 != field_3) {
			cout << "Set Camera to Field 2" << endl;
		}
		if (field_3 != field_1 && field_3 != field_2) {
			cout << "Set Camera to Field 3" << endl;
		}

		// List COM Ports
		// Send serial messages out COM port
		// Build out Form?
		// Provide information for message sent and when confirmation is recieved
		// Embed images of the test locations?

		key = waitKey(30); // you can change wait time
	}
	ReleaseCapture();
	destroyAllWindows();
	return 0;
}