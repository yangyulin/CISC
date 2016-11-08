//
// Created by rpng on 11/8/16.
//

#include <HWAlgorithm3.h>
#include <opencv2/calib3d.hpp>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

CISC::HWAlgorithm3::HWAlgorithm3() { };

void CISC::HWAlgorithm3::unWarp(Mat &input1, Mat &input2, Mat &outPut) {
    // step1: detect the keypoints using SURF detectors and calculate the descriptors
    int minHessian = 400;

    Ptr<SURF> detector = SURF::create(minHessian);
    std::vector<KeyPoint> Keyp1,Keyp2;
    Mat descImg1, descImg2;

    detector->detectAndCompute(input1, Mat(),Keyp1,descImg1, false);
    detector->detectAndCompute(input2, Mat(),Keyp2,descImg2, false);

    // step2: mathch features with FLANN Matcher
    //Ptr<DescriptorMatcher> descMatcher = DescriptorMatcher::create(FLANN);

    FlannBasedMatcher descMatcher;
    std::vector<DMatch> matches;
    descMatcher.match(descImg1,descImg2,matches);

    //calculate the max and min distance between keypoints
    double maxDist =0;
    double minDist =100;

    for(int i=0;i<descImg1.rows;i++){
        double dist = matches[i].distance;
        if(dist < minDist) minDist = dist;
        if(dist > maxDist) maxDist = dist;
    }

    //try to select the good matches
    std::vector<DMatch> goodMatches;

    for (int i=0;i<descImg1.rows;i++){
        if(matches[i].distance < 3*minDist){
            goodMatches.push_back((matches[i]));
        }
    }

    //get the points from good matches
    std::vector<Point2f> ptImg1;
    std::vector<Point2f> ptImg2;

    for (int i=1;i<goodMatches.size();i++)
    {
        ptImg1.push_back(Keyp1[goodMatches[i].queryIdx].pt);
        ptImg2.push_back(Keyp2[goodMatches[i].trainIdx].pt);
    }

    //find the homography matrix
    Mat H = findHomography(ptImg1,ptImg2,CV_RANSAC);

    warpPerspective(input1,outPut,H,cv::Size(input1.cols+input2.cols,input1.rows), BORDER_REPLICATE,BORDER_TRANSPARENT);
    imshow("first",outPut);
    input2.copyTo(outPut(cv::Rect(0,0,input2.cols,input2.rows)));
    //cv::Mat half(outPut,cv::Rect(0,0,input2.cols,input2.rows));
    //input2.copyTo((half));
    imshow("Pano",outPut);
    waitKey(0);



}