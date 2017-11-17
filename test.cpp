#include "stdafx.h"
#include <cv.h>
#include <highgui.h>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    Mat image, canny, dst, res, intImg;
    image = imread("original.png",0);

    if(!image.data )
    {
        cout <<"File not open" << endl;
        return -1;
    };

    Canny(image,canny,40,200);

    Mat_<double> doubleImg(image.rows,image.cols);

    for (int i=0; i<image.rows;i++)
        for(int j=0; j<image.cols;j++){
            canny.at<uchar>(i,j)=255-canny.at<uchar>(i,j);
            doubleImg.at<double>(i,j) = (double)image.at<uchar>(i,j);
            }

    distanceTransform(canny, dst, CV_DIST_L2, 3);
    cv::integral(doubleImg,intImg);
    image.copyTo(res);

    for (int i = 1; i<image.rows+1;i++)
        for(int j=1; j<image.cols+1;j++) {
            int T = 1;
            T = (int)(dst.at<float>(i-1,j-1) * T);
            if (T%2==0) T++;

            while ((i-T/2<0)||(j-T/2<0)||(i+T/2+1>image.rows)||(j+T/2+1>image.cols)) {
                T-=2;
                } 
            if(T>0){
                int A = intImg.at<double>(i-T/2,j-T/2);
                int B = intImg.at<double>(i-T/2,j+T/2+1);
                int C = intImg.at<double>(i+T/2+1,j-T/2);
                int D = intImg.at<double>(i+T/2+1,j+T/2+1);
                res.at<uchar>(i-1,j-1) = (A+D-B-C)/(T*T);
                }
            }

    imwrite("result.png",res);

    namedWindow( "Original", WINDOW_AUTOSIZE );
    imshow( "Original", image );

    namedWindow( "Canny", WINDOW_AUTOSIZE );
    imshow( "Canny", canny);

    namedWindow( "Result", WINDOW_AUTOSIZE );
    imshow( "Result", res);

    waitKey(0);
    return 0;
}
