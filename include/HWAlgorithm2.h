#ifndef PART2_H_
#define PART2_H_

#include <Eigen/Core>
#include <Eigen/Dense>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <cmath>
#include <vector>
#include <iostream>

void MouseCallBack(int event, int x, int y, int flags, void* ptr);

namespace CISC {


    class HWAlgorithm2 {
    public:
        HWAlgorithm2();

        HWAlgorithm2(const cv::Mat &image1, const cv::Mat &image2);

        ~HWAlgorithm2();

        void FindCorners();

        void FindCorners_Man(const cv::Point2f &center);

        void FindMatches();

        void FindMatches_Man();

        void FindAffine();

        void ShowImage();

        void Auto();

        void Mannual();

        EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    private:
        uint32_t width1_;
        uint32_t height1_;

        uint32_t width2_;
        uint32_t height2_;

        cv::Mat cvmat1_;
        cv::Mat cvmat2_;

        cv::Mat cvmat1_mono_;
        cv::Mat cvmat2_mono_;

        std::vector<cv::Point2f> corners_;

        std::vector<cv::Point2f> matches_;

        Eigen::MatrixXd affine_;
    };

}
#endif
