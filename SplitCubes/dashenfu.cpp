/*
 * 分割九宫格
 * 张彪 2017-12-10
 */
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp> 
#include <opencv2/core/core.hpp>
#include <stdio.h>
#include <iostream>
using namespace cv;
using namespace std;
int main( int argc, char** argv )
{
  VideoCapture cap("2861.avi");//读取视频
  if(!cap.isOpened())
  {
    return -1;
  }
  cout<<  " * 分割九宫格"<<endl;
  cout<<  " * 张彪 2017-12-10"<<endl;
  cout<<  " * 按任意键播放/暂停"<<endl;
 int delay=30;
 Mat srcImage;
 while (1)
 {
   cap>>srcImage;//读取帧
   imshow("视频",srcImage);
   Mat res=srcImage.clone();
   Mat hsvImage,binary,tempImage;
   cvtColor(srcImage,hsvImage,CV_BGR2HSV);//转到HSV空间
   Vec3b lower(0,0,100);
   Vec3b upper(180,40,255);
   inRange(hsvImage,lower,upper,binary);//提取灰白区域
   //imshow("灰白区域",binary);
   Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
   dilate(binary,binary, element);//膨胀
   erode(binary, binary, element);//腐蚀
   vector<std::vector<cv::Point> > contours;
   findContours(binary, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//连通域轮廓提取
   //Mat resultImage = Mat::zeros(binary.size(),CV_8UC1);
   //drawContours(resultImage, contours, -1,Scalar(255, 0, 255));
  // imshow("conntours",resultImage);
    vector<RotatedRect> box(contours.size()); 
    Point2f rect[4];
    for(int i=0; i<contours.size(); i++)
    {
        float contour_area=fabs(contourArea(contours[i]));//连通域面积
        box[i] = minAreaRect(Mat(contours[i]));  //计算每个轮廓最小外接矩形
        box[i].points(rect);  //把最小外接矩形四个端点复制给rect数组
        float length=box[i].size.height;//长
	float width= box[i].size.width;//宽
	float ratio;
	if (length<=width) 
	ratio=length/width;//计算宽长比
	else 
	ratio=width/length;//计算宽长比
	float area=length*width;//计算矩形面积
	float angle=box[i].angle*3.1416/180;//矩形旋转角度
        if (abs(abs(angle)-3.1416/2)<0.08||abs(angle)<0.08)//旋转角度判断
	{
	if(abs(ratio-0.55)<=0.08)//长宽比判断
	{
	 if (1200<=area&&area<=3200)//面积判断
	 {
	 if(contour_area/area>=0.6)//连通域面积与矩形面积之比判断
	 {
        for(int j=0; j<4; j++)
        {
            line(res, rect[j], rect[(j+1)%4], Scalar( 0,255, 255), 2, 8);  //绘制最小外接矩形每条边
        }
	}
	}
	}
	}
    }    
    imshow("结果", res);
    if(delay>=0&&waitKey(delay)>=0)//按任意键播放暂停
     waitKey(0);
 }

/*
 //单帧调试
   Mat srcImage=imread("19.png");
   Mat hsvImage,binary,tempImage;
   cvtColor(srcImage,hsvImage,CV_BGR2HSV);
   Vec3b lower(0,0,100);
   Vec3b upper(180,40,255);
   inRange(hsvImage,lower,upper,binary);
   imshow("灰白区域",binary);
   Mat element1 = getStructuringElement(MORPH_RECT, Size(5, 5));
   Mat element2 = getStructuringElement(MORPH_RECT, Size(5, 5));
    dilate(binary,binary, element1);
     imshow("膨胀",binary);
 //     morphologyEx(binary,binary,MORPH_CLOSE,Mat(5,5,CV_8U),Point(-1,-1),1);
 //  imshow("闭",binary);
     erode(binary, binary, element1);
     imshow("腐蚀",binary);
  
  
   //
  // medianBlur(binary,binary, 3);
   vector<std::vector<cv::Point> > contours;
   findContours(binary, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
   Mat resultImage = Mat::zeros(binary.size(),CV_8UC1);
   Mat dstImg= Mat::zeros(binary.size(),CV_8UC1);
   drawContours(resultImage, contours, -1,Scalar(255, 0, 255));
   imshow("conntours",resultImage);
    vector<RotatedRect> box(contours.size()); 
    Point2f rect[4];
    for(int i=0; i<contours.size(); i++)
    {
        float contour_area=fabs(contourArea(contours[i]));
        box[i] = minAreaRect(Mat(contours[i]));  //计算每个轮廓最小外接矩形
        box[i].points(rect);  //把最小外接矩形四个端点复制给rect数组
        float length=box[i].size.height;//sqrt(pow((rect[0].x-rect[1].x),2)+pow((rect[0].y-rect[1].y),2));
	float width= box[i].size.width;//sqrt(pow((rect[2].x-rect[1].x),2)+pow((rect[2].y-rect[1].y),2));
	float ratio;
	if (length<=width) 
	ratio=length/width;
	else 
	ratio=width/length;
	float area=length*width;
	float angle=box[i].angle*3.1416/180;//atan2(rect[0].y-rect[1].y,rect[0].x-rect[1].x);
        if (abs(abs(angle)-3.1416/2.0)<0.08||abs(angle)<0.08)
	{
	if(abs(ratio-0.55)<=0.08)
	{
	 if (1200<=area&&area<=3200)
	{
	  if(contour_area/area>=0.6)
	  {
        for(int j=0; j<4; j++)
        {
            line(dstImg, rect[j], rect[(j+1)%4], Scalar(255, 0, 255), 2, 8);  //绘制最小外接矩形每条边
        }
        cout<<angle<<" "<<ratio<<" "<<area<<" "<<contour_area<<endl;
	}
	}
	}
	}
    }    
    imshow("dst", dstImg);
   waitKey(0);
*/
 return 0;
}
