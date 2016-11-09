//
// Created by rpng on 11/8/16.
//

#ifndef CISC_HWALGORITHM3_H
#define CISC_HWALGORITHM3_H

#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <HWAlgorithm.h>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

namespace CISC{
    class HWAlgorithm3{
    public:
        HWAlgorithm3();

        void unWarp(Mat& input1, Mat& input2, Mat& outPut );
    };
}








#endif //CISC_HWALGORITHM3_H
