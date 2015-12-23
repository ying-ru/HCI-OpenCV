#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <opencv/cv.h>
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace cv;
using namespace std;

#define MOVEMENT_THRESHOLD 70
#define ROI_WIDTH 80
#define ROI_HEIGHT 250
#define COVERAGE 0.75
#define BLACK_WHITE 255

int main(int argc, char *argv[])
{

    CvCapture *capture;
    bool first = true;
    IplImage *frame, *bkframe;
    IplImage *reference_frame, *diff;
    IplImage *mainImg, *img1, *img2, *img3, *img4, *img5;
    IplImage *cam, *cam1, *cam2, *cam3;
    IplImage *diff2, *diff3;
    IplImage *pic, *dst;
    CvMat *mat_roi;
    int check_img1, check_img2, check_img3, check_img4, check_img5;
    
    capture = cvCaptureFromCAM(0);
    cvNamedWindow("Webcam", CV_WINDOW_OPENGL);
    cvMoveWindow("Webcam", 0, 0);
    //cvNamedWindow("FrameDifference", CV_WINDOW_AUTOSIZE);
    //cvMoveWindow("FrameDifference", 0, 500);
    
    mainImg = cvLoadImage("mainImg.png", CV_LOAD_IMAGE_UNCHANGED);
    img1 = cvLoadImage("img1.png", CV_LOAD_IMAGE_UNCHANGED);
    img2 = cvLoadImage("img2.png", CV_LOAD_IMAGE_UNCHANGED);
    img3 = cvLoadImage("img3.png", CV_LOAD_IMAGE_UNCHANGED);
    img4 = cvLoadImage("img4.png", CV_LOAD_IMAGE_UNCHANGED);
    img5 = cvLoadImage("img5.png", CV_LOAD_IMAGE_UNCHANGED);
    
    cam1 = cvLoadImage("cam1.png", CV_LOAD_IMAGE_UNCHANGED);
    cam2 = cvLoadImage("cam2.png", CV_LOAD_IMAGE_UNCHANGED);
    cam3 = cvLoadImage("cam3.png", CV_LOAD_IMAGE_UNCHANGED);
    
    check_img1 = 0;
    check_img2 = 0;
    check_img3 = 0;
    check_img4 = 0;
    check_img5 = 0;
    
    cvShowImage("Webcam", mainImg);

    while (true) {
        
        
        frame = cvQueryFrame(capture);
        if (frame && first) {
            first = false;
            bkframe = cvCreateImage(cvSize(frame->width, frame->height), frame->depth, 1);
            cvCvtColor(frame, bkframe, CV_BGR2GRAY);
            
            reference_frame = cvCreateImage(cvSize(frame->width, frame->height), frame->depth, 1);
            cvCopy(bkframe, reference_frame);
            
            diff = cvCreateImage(cvSize(frame->width, frame->height), frame->depth, 1);
            diff2 = cvCreateImage(cvSize(frame->width, frame->height), frame->depth, 1);
            diff3 = cvCreateImage(cvSize(frame->width, frame->height), frame->depth, 1);
            
            pic = cvCreateImage(cvSize(frame->width/2, frame->height/2), frame->depth, 3);

        }
        
        cvCvtColor(frame, bkframe, CV_BGR2GRAY);//frame to gray -> bkframe
        cvAbsDiff(bkframe, reference_frame, diff);//bkframe - regrence -> diff
        cvAbsDiff(diff, diff2, diff);
//        cvAbsDiff(bkframe, reference_frame, diff3);//bkframe - regrence -> diff
        
        cvThreshold(diff, diff, MOVEMENT_THRESHOLD, 255, CV_THRESH_BINARY);
//        cvThreshold(diff2, diff2, 2*MOVEMENT_THRESHOLD, 255, CV_THRESH_BINARY);
//        cvThreshold(diff3, diff3, 2*2*MOVEMENT_THRESHOLD, 255, CV_THRESH_BINARY);
        //cvShowImage("FrameDifference", diff);
        
        if (cvWaitKey(100)>0) {
            break;
        }
        
        if (check_img1 > 10) {
            cvNamedWindow("pic", CV_WINDOW_AUTOSIZE);
            cvMoveWindow("pic", 100, 230);

            
            cvResize(frame, pic, CV_INTER_LINEAR);
            cvSaveImage("pic.png", pic);
            pic = cvLoadImage("pic.png", CV_LOAD_IMAGE_UNCHANGED);
            cvShowImage("Webcam", img1);
            cvShowImage("pic", pic);
            check_img1 = 0;
        }
        
        //ROI
        
        mat_roi = cvCreateMat(ROI_HEIGHT, ROI_WIDTH, CV_8UC3);
        cvGetSubRect(diff, mat_roi, cvRect(ROI_WIDTH * 0, 0, ROI_WIDTH * 5, ROI_HEIGHT));
        
        cvGetImage(mat_roi, diff3);
        int count = 0;
        
        for (int i=0; i < ROI_HEIGHT; i++) {
            for (int j=0; j < ROI_WIDTH * 5; j++) {
                if (cvGet2D(diff3, i, j).val[0] == BLACK_WHITE) {
                    count++;
                }
            }
        }
        if (count > ROI_HEIGHT * ROI_WIDTH * 5 * COVERAGE) {
            continue;
        }
        
        //for img1 camera
        mat_roi = cvCreateMat(ROI_HEIGHT, ROI_WIDTH, CV_8UC3);
        cvGetSubRect(diff, mat_roi, cvRect(ROI_WIDTH * 0, 0, ROI_WIDTH, ROI_HEIGHT));
        
        cvGetImage(mat_roi, diff3);
        count = 0;
        
        for (int i=0; i < ROI_HEIGHT; i++) {
            for (int j=0; j < ROI_WIDTH; j++) {
                if (cvGet2D(diff3, i, j).val[0] == BLACK_WHITE) {
                    count++;
                }
            }
        }
        printf("%d ", count);
        
        if (count > ROI_HEIGHT * ROI_WIDTH * COVERAGE) {
            check_img1++;
            check_img2 = 0;
            check_img3 = 0;
            check_img4 = 0;
            check_img5 = 0;
            cvShowImage("Webcam", img1);
        }
        
        if (check_img1 > 10) {
            cvShowImage("Webcam", cam3);
            if (cvWaitKey(1500)>0) {
            }
            cvShowImage("Webcam", cam2);
            if (cvWaitKey(1500)>0) {
            }
            cvShowImage("Webcam", cam1);
            if (cvWaitKey(1500)>0) {
            }
            continue;
        }
        
        //for img2 big
        mat_roi = cvCreateMat(ROI_HEIGHT, ROI_WIDTH, CV_8UC3);
        cvGetSubRect(diff, mat_roi, cvRect(ROI_WIDTH * 1, 0, ROI_WIDTH, ROI_HEIGHT));
        
        cvGetImage(mat_roi, diff3);
        count = 0;
        
        for (int i=0; i < ROI_HEIGHT; i++) {
            for (int j=0; j < ROI_WIDTH; j++) {
                if (cvGet2D(diff3, i, j).val[0] == BLACK_WHITE) {
                    count++;
                }
            }
        }
        printf("%d ", count);
        
        if (count > ROI_HEIGHT * ROI_WIDTH * COVERAGE) {
            check_img1 = 0;
            check_img2++;
            check_img3 = 0;
            check_img4 = 0;
            check_img5 = 0;
            cvShowImage("Webcam", img2);
        }
        
        if (check_img2 > 10) {
            dst = cvCreateImage(cvSize(pic->width * 1.05, pic->height * 1.05), pic->depth, 3);
            cvResize(pic, dst, CV_INTER_LINEAR);
            pic = dst;
            cvShowImage("pic", pic);
            check_img2 = 0;
        }
        
        //for img3 small
        mat_roi = cvCreateMat(ROI_HEIGHT, ROI_WIDTH, CV_8UC3);
        cvGetSubRect(diff, mat_roi, cvRect(ROI_WIDTH * 2, 0, ROI_WIDTH, ROI_HEIGHT));
        
        cvGetImage(mat_roi, diff3);
        count = 0;
        
        for (int i=0; i < ROI_HEIGHT; i++) {
            for (int j=0; j < ROI_WIDTH; j++) {
                if (cvGet2D(diff3, i, j).val[0] == BLACK_WHITE) {
                    count++;
                }
            }
        }
        //printf("%d ", count);
        
        if (count > ROI_HEIGHT * ROI_WIDTH * COVERAGE) {
            check_img1 = 0;
            check_img2 = 0;
            check_img3++;
            check_img4 = 0;
            check_img5 = 0;
            cvShowImage("Webcam", img3);
        }
        
        if (check_img3 > 10) {
            dst = cvCreateImage(cvSize(pic->width * 0.95, pic->height * 0.95), pic->depth, 3);
            cvResize(pic, dst, CV_INTER_LINEAR);
            pic = dst;
            cvShowImage("pic", pic);
            check_img3 = 0;
        }
        
        //for img4 save
        mat_roi = cvCreateMat(ROI_HEIGHT, ROI_WIDTH, CV_8UC3);
        cvGetSubRect(diff, mat_roi, cvRect(ROI_WIDTH * 3, 0, ROI_WIDTH, ROI_HEIGHT));
        
        cvGetImage(mat_roi, diff3);
        count = 0;
        
        for (int i=0; i < ROI_HEIGHT; i++) {
            for (int j=0; j < ROI_WIDTH; j++) {
                if (cvGet2D(diff3, i, j).val[0] == BLACK_WHITE) {
                    count++;
                }
            }
        }
        //printf("%d ", count);
        
        if (count > ROI_HEIGHT * ROI_WIDTH * COVERAGE) {
            check_img1 = 0;
            check_img2 = 0;
            check_img3 = 0;
            check_img4++;
            check_img5 = 0;
            cvShowImage("Webcam", img4);
        }
        
        if (check_img4 > 10) {
            cvSaveImage("pic.png", pic);
            check_img4 = 0;
        }
        
        //for img5 close
        mat_roi = cvCreateMat(ROI_HEIGHT, ROI_WIDTH, CV_8UC3);
        cvGetSubRect(diff, mat_roi, cvRect(ROI_WIDTH * 4, 0, ROI_WIDTH, ROI_HEIGHT));
        
        cvGetImage(mat_roi, diff3);
        count = 0;
        
        for (int i=0; i < ROI_HEIGHT; i++) {
            for (int j=0; j < ROI_WIDTH; j++) {
                if (cvGet2D(diff3, i, j).val[0] == BLACK_WHITE) {
                    count++;
                }
            }
        }
        //printf("%d ", count);
        
        if (count > ROI_HEIGHT * ROI_WIDTH * COVERAGE) {
            check_img1 = 0;
            check_img2 = 0;
            check_img3 = 0;
            check_img4 = 0;
            check_img5++;
            cvShowImage("Webcam", img5);
        }
        
        if (check_img5 > 10) {
            cvDestroyWindow("pic");
            printf("pic close \n");
            check_img5 = 0;
        }
        
        
        //cvShowImage("Webcam", diff3);
//        cvShowImage("FrameDifference2", diff2);
//        cvShowImage("FrameDifference3", diff3);
        //cvAbsDiff(bkframe, reference_frame, diff2);//bkframe - regrence -> diff

        
    }
    
    cvReleaseCapture(&capture);
    cvDestroyWindow("Webcam");
    //cvDestroyWindow("FrameDifference");
    cvDestroyWindow("pic");
    cvReleaseImage(&bkframe);
    cvReleaseImage(&reference_frame);
    cvReleaseImage(&diff);
    
    return 0;
}