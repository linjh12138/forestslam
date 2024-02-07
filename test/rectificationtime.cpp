#include <iostream>
#include <opencv2/opencv.hpp>
//#include <chrono>
#include<fstream>
#include <sstream>
using namespace std;
int main(int argc, char **argv)
{
    string video_file_path_1 = argv[1],image_frame_path_1,image_path_1 = "../data3/mav0/cam0/";
    string video_file_path_2 = argv[2],image_frame_path_2,image_path_2 = "../data3/mav0/cam1/";
    string timestamp_path = "../data3/mav0/v3.txt";
    string s;
    stringstream ss;
    ifstream ifs;
    ofstream ofs;
    long int Timestamp, t, threshold = 1.0/58.0 * 1e9;
    cv::Mat frame_1, frame_2;
    cv::VideoCapture cap_1(video_file_path_1);
    cv::VideoCapture cap_2(video_file_path_2);
    if(!cap_1.isOpened())
    {
        cout<<"请重新输入视频"<<endl;
        return -1;
    }
    if(!cap_2.isOpened())
    {
        cout<<"请重新输入视频"<<endl;
        return -1;
    }
    double fTimestamp = cap_1.get(CV_CAP_PROP_POS_MSEC);//时间戳(毫秒)
    int m_fps_1 = cap_1.get(CV_CAP_PROP_FPS);//得到帧率
    double fTimestamp_2 = cap_2.get(CV_CAP_PROP_POS_MSEC);//时间戳(毫秒)
    int m_fps_2 = cap_2.get(CV_CAP_PROP_FPS);//得到帧率
    cout<<fTimestamp<<endl<<m_fps_1<<endl;
    cout<<fTimestamp_2<<endl<<m_fps_2<<endl;
    ofs.open(timestamp_path,ios::out);
    vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION); //PNG格式图片的压缩级别  
    compression_params.push_back(2);  //设置保存的图像质量级别
    ifs.open("../data2/mav1/v2.txt",ios::in);
    if(!ifs.is_open())
    {
        cout<<"未打开v1"<<endl;
        return -1;
    }
    //for(int i = 0; i < 69; i++)//iphone先录
    //{
    //    cap_1.read(frame_1);
    //}
    //for(int i = 0; i < 50; i++)
    while(1)
    {
        //cv::Mat frame;
        cap_1.read(frame_1);
        if(frame_1.empty())//如果某帧为空则退出循环
            break;
        fTimestamp = cap_1.get(CV_CAP_PROP_POS_MSEC) + 33.0;
        //Timestamp = fTimestamp * 1e6 + 1e18 - 866666624.0;//02数据
        //Timestamp = fTimestamp * 1e6 + 1e18 - 2300000000.0;
        Timestamp = fTimestamp * 1e6 + 1e18;

        cap_2.read(frame_2);
        if(frame_2.empty())//如果某帧为空则退出循环
            break;

        getline(ifs,s);
        if(s.empty())
        {   
            break;
        }
        ss << s;
        ss >> t;
        ss.clear();

        /*if(t-Timestamp >= threshold)
        {
            cap_1.read(frame_1);
            if(frame_1.empty())//如果某帧为空则退出循环
                break;
            fTimestamp = cap_1.get(CV_CAP_PROP_POS_MSEC);
            //Timestamp = fTimestamp * 1e6 + 1e18 - 866666624.0;
            //Timestamp = fTimestamp * 1e6 + 1e18 - 2300000000.0;
            Timestamp = fTimestamp * 1e6 + 1e18;
        }*/
        //cout<<t<<endl<<Timestamp<<endl;
            //continue;

        image_frame_path_1 =image_path_1  + to_string(Timestamp) + ".png";
        //cout<<image_frame_path<<endl;
        cv::imwrite(image_frame_path_1, frame_1, compression_params);

        image_frame_path_2 =image_path_2  + to_string(Timestamp) + ".png";
        cv::imwrite(image_frame_path_2, frame_2, compression_params);
        ofs << Timestamp<<endl;
    }
    ofs.close();
    ifs.close();
    return 0;
}