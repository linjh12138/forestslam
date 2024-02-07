#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main(int argc, char **argv)
{
    //cv::Mat cameraMatrix1 = (cv::Mat_<double>(3, 3) << 500.0, 0.0, 320.0, 0.0, 500.0, 240.0, 0.0, 0.0, 1.0);
    cv::Mat cameraMatrix1 = (cv::Mat_<double>(3, 3) << 1303.5, 0.0, 550.4, 0.0, 1367.9, 1172.4, 0.0, 0.0, 1.0);
    cv::Mat k1 = (cv::Mat_<double>(1, 4) << 0.239827852907792, -0.117799758922530, 0.0, 0.0, 0.0);
    cv::Mat k2 = (cv::Mat_<double>(1, 4) << 0.480223876015388, -2.71247238395345, 0.0, 0.0, 0.0);
    //cv::Mat cameraMatrix2 = (cv::Mat_<double>(3, 3) << 500.0, 0.0, 320.0, 0.0, 500.0, 240.0, 0.0, 0.0, 1.0);
    cv::Mat cameraMatrix2 = (cv::Mat_<double>(3, 3) << 1415.2, 0.0, 501.0369, 0.0, 1470.9, 1103.6, 0.0, 0.0, 1);
    //cv::Mat R = (cv::Mat_<double>(3, 3) <<  1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 );
    //cv::Mat R = (cv::Mat_<double>(3, 3) << 0.9991570217473601, -0.03625243058530075, 0.01926154639698378, 0.03647915268869904, 0.9992676362501824, -0.01155259979777116, -0.01882863011638867, 0.01224550609941618, 0.9997477333149144);
    //cv::Mat R = (cv::Mat_<double>(3, 3) << 0.9987277571766294, 0.03934509366290351, -0.03154093609259227, -0.03904766821246272, 0.9991873976511302, 0.009991195240403834, 0.0319084103663759, -0.008746884006308393, 0.9994525228184035);
    cv::Mat R = (cv::Mat_<double>(3, 3) << 0.997581231360509, -0.00300043586290643, -0.0694455486110025, -0.000555339473201235, 0.998692031068178, -0.0511264968385970, 0.0695081177656252, 0.0510413995257786, 0.996274759842449);
    R = R.t();
    cv::Mat t = (cv::Mat_<double>(3, 1) << -143.693557028534, -1.35199478750058, 24.2165468727571);
    //cv::Mat t = (cv::Mat_<double>(3, 1) << -100.0, 0.0, 0.0);
    cv::Mat R1, R2, P1, P2, Q;
    cv::stereoRectify(cameraMatrix1, k1, cameraMatrix2, k2, cv::Size(1280,720), R, t, R1, R2, P1, P2, Q,cv::CALIB_ZERO_DISPARITY,0);
    cout<< R1<<endl<<R2<<endl<<P1<<endl<<P2<<endl<<Q<<endl;

    string file_path_1 = argv[1], file_path_2 = argv[2];
    cv::Mat M1l, M2l,M1r, M2r, imLeft, imLeftRect, imRight, imRightRect;
    imLeft = cv::imread(file_path_1, CV_LOAD_IMAGE_UNCHANGED);
    imRight = cv::imread(file_path_2, CV_LOAD_IMAGE_UNCHANGED);
    cv::initUndistortRectifyMap(cameraMatrix1,k1,R1,P1.rowRange(0,3).colRange(0,3),cv::Size(720,1280),CV_32F,M1l,M2l);
    cv::remap(imLeft,imLeftRect,M1l,M2l,cv::INTER_LINEAR);
    cv::initUndistortRectifyMap(cameraMatrix2,k1,R2,P2.rowRange(0,3).colRange(0,3),cv::Size(720,1280),CV_32F,M1r,M2r);
    cv::remap(imRight,imRightRect,M1r,M2r,cv::INTER_LINEAR);
    cv::imshow("1", imLeftRect);
    cv::waitKey(0);
    cv::imshow("2", imRightRect);
    cv::waitKey(0);
    cv::imwrite("22.png", imLeftRect);
    cv::imwrite("23.png", imRightRect);
    /*string image_path_1 = "/home/lin/ORB_SLAM2/data1/mav1/cam0/";
    string image_path_2 = "/home/lin/ORB_SLAM2/data1/mav1/cam1/1000000002900000000.png";
    string time_path_1 = "/home/lin/ORB_SLAM2/data1/mav1/v3.txt";
    string image_frame_path_1, image_frame_path_2;
    string s;
    stringstream ss;
    long int t;
    ifstream ifs;
    ifs.open(time_path_1,ios::in);
    if(!ifs.is_open())
    {
        cout<<"未打开v3"<<endl;
        return -1;
    }
    for(int i = 0; i < 16*6; i++)
    {
        getline(ifs,s);
    }
    for(int i = 0; i < 2*6; i++)
    {
        getline(ifs,s);
        if(s.empty())
        {
            break;
        }
        ss << s;
        ss >> t;
        ss.clear();
        image_frame_path_1 =image_path_1  + to_string(t) + ".png";
        //image_frame_path_2 =image_path_2  + to_string(t) + ".png";
        Mat img_1 = imread(image_frame_path_1, CV_LOAD_IMAGE_COLOR);
        Mat img_2 = imread(image_path_2, CV_LOAD_IMAGE_COLOR);

        assert(img_1.data != nullptr && img_2.data != nullptr);*/

        //-- 初始化
        std::vector<KeyPoint> keypoints_1, keypoints_2;
        Mat descriptors_1, descriptors_2;
        Ptr<FeatureDetector> detector = ORB::create();
        Ptr<DescriptorExtractor> descriptor = ORB::create();
        Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");

        //-- 第一步:检测 Oriented FAST 角点位置
        chrono::steady_clock::time_point t1 = chrono::steady_clock::now();
        detector->detect(imLeftRect, keypoints_1);
        detector->detect(imRightRect, keypoints_2);

        //-- 第二步:根据角点位置计算 BRIEF 描述子
        descriptor->compute(imLeftRect, keypoints_1, descriptors_1);
        descriptor->compute(imRightRect, keypoints_2, descriptors_2);
        chrono::steady_clock::time_point t2 = chrono::steady_clock::now();
        chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
        cout << "extract ORB cost = " << time_used.count() << " seconds. " << endl;

        Mat outimg1;
        drawKeypoints(imLeftRect, keypoints_1, outimg1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
        imshow("ORB features", outimg1);

        //-- 第三步:对两幅图像中的BRIEF描述子进行匹配，使用 Hamming 距离
        vector<DMatch> matches;
        t1 = chrono::steady_clock::now();
        matcher->match(descriptors_1, descriptors_2, matches);
        t2 = chrono::steady_clock::now();
        time_used = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
        cout << "match ORB cost = " << time_used.count() << " seconds. " << endl;

        //-- 第四步:匹配点对筛选
        // 计算最小距离和最大距离
        auto min_max = minmax_element(matches.begin(), matches.end(),
                                        [](const DMatch &m1, const DMatch &m2) { return m1.distance < m2.distance; });
        double min_dist = min_max.first->distance;
        double max_dist = min_max.second->distance;

        printf("-- Max dist : %f \n", max_dist);
        printf("-- Min dist : %f \n", min_dist);

        //当描述子之间的距离大于两倍的最小距离时,即认为匹配有误.但有时候最小距离会非常小,设置一个经验值30作为下限.
        std::vector<DMatch> good_matches;
        for (int i = 0; i < descriptors_1.rows; i++) {
            if (matches[i].distance <= max(2 * min_dist, 30.0)) {
            good_matches.push_back(matches[i]);
            }
        }

        //-- 第五步:绘制匹配结果
        Mat img_match;
        Mat img_goodmatch;
        drawMatches(imLeftRect, keypoints_1, imRightRect, keypoints_2, matches, img_match);
        drawMatches(imLeftRect, keypoints_1, imRightRect, keypoints_2, good_matches, img_goodmatch);
        imshow("all matches", img_match);
        imshow("good matches", img_goodmatch);
        waitKey(0);
        cout<<good_matches.size()<<endl;
   /*}
    ifs.close();*/
    return 0;
}
