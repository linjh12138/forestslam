#include <iostream>
#include <opencv2/opencv.hpp>
//#include <chrono>
#include<fstream>
using namespace std;
int main(int argc, char **argv)
{
    string video_file_path = argv[1],image_frame_path,image_path = "../data3/mav0/cam0/";
    string timestamp_path = "../data3/mav0/v2.txt";
    //string timestamp_path_2 = "../data1/mav1/v2.txt";
    ofstream ofs;
    long int Timestamp;
    cv::Mat frame;
    cv::VideoCapture cap(video_file_path);
    if(!cap.isOpened())
    {
        cout<<"请重新输入视频"<<endl;
        return -1;
    }
    double fTimestamp = cap.get(CV_CAP_PROP_POS_MSEC);//时间戳(毫秒)
    int m_fps = cap.get(CV_CAP_PROP_FPS);//得到帧率
    int width = cap.get(CV_CAP_PROP_FRAME_WIDTH);//宽
    int height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);//高
    /*bool c = cap.set(cv::CAP_PROP_FOURCC,cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    bool a = cap.set(cv::CAP_PROP_FRAME_WIDTH, 360);
	bool b = cap.set(cv::CAP_PROP_FRAME_HEIGHT, 640);

    cout<<a<<b<<c<<endl;
    cout<<fTimestamp<<endl<<m_fps<<endl;*/
    cout<<width<<endl<<height<<endl;
    cap.read(frame);
    //cv::imshow("show",frame);
    fTimestamp = cap.get(CV_CAP_PROP_POS_MSEC);
    cout<<fTimestamp<<endl<<m_fps<<endl;
    ofs.open(timestamp_path,ios::out);
    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION); //PNG格式图片的压缩级别  
    compression_params.push_back(2);  //设置保存的图像质量级别
    //cv::imshow("1", frame);
    //cv::waitKey(0);
    while(1)
    {
        cap.read(frame);
        if(frame.empty())//如果某帧为空则退出循环
            break;
        fTimestamp = cap.get(CV_CAP_PROP_POS_MSEC);
        Timestamp = fTimestamp * 1e6 + 1e18;
        //image_frame_path =image_path  + to_string(Timestamp) + ".png";
        //cout<<image_frame_path<<endl;
        //cv::imwrite(image_frame_path, frame, compression_params);
        ofs << Timestamp<<endl;
    }
    ofs.close();
    return 0;
}