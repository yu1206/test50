#include <iostream>
#include <vector>
#include<stdlib.h>
#include <time.h> 
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "fstream"
#include <direct.h>
#include <io.h>
#include <Windows.h>
#include <direct.h> 
using namespace std;
using namespace cv;

extern int readDir(char *dirName, vector<string> &filesName);

struct StrTxt
{
	string name;
	vector<Point2f>pts;
};
int test50(int argc, char *argv[])
{
	// 1. 读取坐标和对应的txt

	vector<StrTxt> mMP;
	string imgpath = "E:/new_污损模糊 - 副本/统计污损/1";

	vector<string> v_img_;
	readDir((char*)imgpath.c_str(), v_img_);
	for (int i = 0; i < v_img_.size(); i++)
	{
		string imagename = v_img_[i];
		int npos = imagename.find_last_of('/');
		int npos2 = imagename.find_last_of('.');

		string name1 = imagename.substr(npos + 1, npos2 - npos - 1);
		Mat src = imread(imagename.c_str());

		cout << imagename.c_str() << endl;

		if (src.data == NULL)
		{
			printf("img.data = NULL！\n");
			system("pause");
			continue;
		}
		int width = src.cols;
		int height = src.rows;
		string txtpath = imgpath + "/" + name1 + ".txt";
		fstream finRead;
		finRead.open(txtpath, ios::in);

		if (!finRead.is_open())
		{
			cout << "finRead 文件数据打开错误！" << endl;
			system("pause");
			return false;
		}

		string line; int num1 = 0;
		vector<Point2f> vP;
		while (getline(finRead, line))
		{

			if (line.empty())
			{
				break;
			}

			num1++;

			float x, y;
			stringstream str_s(line);
			str_s >> x >> y;
			Point2f pt; pt.x = x; pt.y = y;
			vP.push_back(pt);



		}

		StrTxt st;
		st.name = name1;
		st.pts = vP;

		mMP.push_back(st);

		finRead.close();

	}

	srand((unsigned)time(NULL));

	string imgpath1 = "E:/清晰车牌-wm/val-污损制作1";
	string savepath = "E:/清晰车牌-wm/val-污损1";

	int nmask = 1;//一个原图像，一个mask图像，对mask在原图像的位置随机几次

	vector<string> v1_img_;
	readDir((char*)imgpath1.c_str(), v1_img_);

	for (int i = 0; i < v1_img_.size(); i++)
	{
		string imagename = v1_img_[i];

		//string imagename = "E:/yu1-plate-data/4_正常车牌-制作污损/1-苏E8C93Q.jpg";

		int npos = imagename.find_last_of('/');
		int npos2 = imagename.find_last_of('.');

		string name1 = imagename.substr(npos + 1, npos2 - npos - 1);
		Mat src1 = imread(imagename.c_str());

		cout << imagename.c_str() << endl;

		if (src1.data == NULL)
		{
			printf("img.data = NULL！\n");
			system("pause");
			continue;
		}

		int width = src1.cols; int height = src1.rows;



		int num = 0;
		//for (int j = 0; j < mMP.size(); j++)
		int j = rand() % mMP.size();

		{
			

			//j = 3;

			StrTxt mt = mMP[j];
			vector<Point2f> pts2f = mt.pts;
			string namestd = mt.name;

			vector<Point> pts;

			for (int v = 0; v < pts2f.size(); v++)
			{
				Point pt;
				pt.x = pts2f[v].x*width;
				pt.y = pts2f[v].y*height;
				pts.push_back(pt);
			}

			int minx = 100000; int miny = 10000; int maxx = 0; int maxy = 0;
			for (int v = 0; v < pts.size(); v++)
			{
				Point pt = pts[v];
				if (pt.x < minx)
				{
					minx = pt.x;
				}

				if (pt.x > maxx)
				{
					maxx = pt.x;
				}

				if (pt.y < miny)
				{
					miny = pt.x;
				}

				if (pt.y > maxy)
				{
					maxy = pt.y;
				}

			}
			Rect boundRect = boundingRect(Mat(pts));
			Mat img = src1.clone();

			int  imgH = img.rows; int imgW = img.cols;
			int stdH = boundRect.height; int stdW = boundRect.width;


			int x; int y;
			if (imgW - maxx - 1 == 0)
			{
				x = 0;
			}
			else
			{
				x = rand() % (imgW - maxx - 1);
			}

			if (imgH - maxy - 1 == 0)
			{
				y = 0;
			}
			else
			{
				y = rand() % (imgH - maxy - 1);
			}

			//x = 122; y = 19;
			printf("j=%d,x=%d,y=%d\n",j,x,y);

			vector<Point> ptsroi;
			for (int v = 0; v < pts.size(); v++)
			{
				Point pt = pts[v];
				pt.x = pt.x + x;
				pt.y = pt.y + y;
				ptsroi.push_back(pt);
			}

			int npt[] = { ptsroi.size() };
			const Point * ppt[1] = { &ptsroi[0] };
			fillPoly(img, ppt, npt, 1, cv::Scalar(0, 0, 0));



			char file[1024];

			sprintf(file, "%s/%s-%s-%d.jpg", savepath.c_str(), name1.c_str(), namestd.c_str(),j);
			imwrite(file, img);

			int jjjj = 0;

		}
		int jjjj = 0;
	}




	return 0;
}





