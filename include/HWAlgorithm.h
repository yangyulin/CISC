//
// Created by rpng on 7/27/16.
//

#ifndef MASKOPERATION_OPERATIONALGORITHM_H
#define MASKOPERATION_OPERATIONALGORITHM_H


#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;
using namespace cv;

namespace CISC{

    class HWAlgorithm{
    public:

        HWAlgorithm();

        //void Sharpen(const Mat& myImage, Mat& Result);

        void Convolve(const Mat& inputImage, Mat& outputImage, const Mat& maskH);

        void Reduce(const Mat& inputImage, Mat& outputImage);

        void Expand(const Mat& inputImage, Mat& outputImage, Size sizeTarget);

        void GaussianPyramid(const Mat& inputImage, vector<Mat3f > &outputPyramid, int numLevel);

        void LaplacianPyramid(const Mat& inputImage, vector<Mat3f > &outputPyramid, Mat &baseMat, int numLevel);

        void Reconstruct(const vector<Mat3f> & inputPyramid, const Mat &baseMat, int numLevel, Mat & outputImage);

        void Blend(const Mat& inImg1, const Mat& inImg2, int boundary1, int boundary2, int numLevel, Mat& outputImage);

        void FillIn(const Mat& inImg, Mat& outImg, int startRow, int startCol);
    };
}



#endif //MASKOPERATION_OPERATIONALGORITHM_H
