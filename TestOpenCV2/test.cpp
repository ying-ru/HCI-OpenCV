//#include "opencv2/highgui/highgui.hpp"
//#include <iostream>
//
//using namespace cv;
//using namespace std;
//
//int main( int argc, const char** argv )
//{
//    Mat img = imread("a.png", CV_LOAD_IMAGE_UNCHANGED);
//    //從資料夾讀取指定圖片
//    Mat red_img(500, 500, CV_8UC3, Scalar(0,0, 256));
//    //建立一張500*500的全紅圖片
//    
//    if (img.empty())//判斷圖片讀取是否成功
//    {
//        cout << "圖片存取失敗" << endl;
//        return -1;
//    }
//    
//    namedWindow("ShowWindow", CV_WINDOW_AUTOSIZE);
//    namedWindow("RedWindow", CV_WINDOW_AUTOSIZE);
//    //建立視窗
//    
//    imshow("ShowWindow", img);
//    imshow("RedWindow", red_img);
//    //顯示圖片於指定視窗
//    
//    waitKey(0);
//    //等待按鍵繼續執行
//    
//    destroyWindow("ShowWindow");
//    destroyWindow("RedWindow");
//    //關閉視窗並釋放記憶體
//    return 0;
//}