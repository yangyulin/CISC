//
// Created by rpng on 10/28/16.
//

#include "HWAlgorithm2.h"

using namespace CISC;
/**
 * Implementations
 */
HWAlgorithm2::HWAlgorithm2() {
}

HWAlgorithm2::HWAlgorithm2(const cv::Mat& image1, const cv::Mat& image2) {
    cvmat1_ = image1;
    cvmat2_ = image2;

    height1_ = image1.rows;
    width1_ = image1.cols;
    std::cout << "Image 1 resolution: " << width1_ << "x" << height1_ << "\n";

    height2_ = image2.rows;
    width2_ = image2.cols;
    std::cout << "Image 2 resolution: " << width2_ << "x" << height2_ << "\n";

    cv::cvtColor(image1, cvmat1_mono_, CV_RGB2GRAY);
    cv::cvtColor(image2, cvmat2_mono_, CV_RGB2GRAY);
}

HWAlgorithm2::~HWAlgorithm2() {
}

void HWAlgorithm2::FindCorners() {
    corners_.clear();

    uint32_t max_features = 10;
    double quality = 1e-2;
    double min_distance = 10;
    uint32_t block_size = 5;
    bool use_harris = true;

    // Detection
    cv::goodFeaturesToTrack(cvmat1_mono_, corners_, max_features, quality, min_distance, cv::Mat(), block_size, use_harris);

    // Refinement
    uint32_t subpix_refinement_window_size = 5;
    uint32_t subpix_refinement_iterations = 5;
    double subpix_refinement_min_accuracy = 1e-2;

    cv::Size subPixWinSize(subpix_refinement_window_size, subpix_refinement_window_size);
    cv::Size zeroZone = cv::Size(-1, -1);
    cv::TermCriteria subPixCriteria(cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS,
                                    subpix_refinement_iterations,
                                    subpix_refinement_min_accuracy);

    std::cout << "Number of corners: " << corners_.size() << "\n";

    if (corners_.size() != 0)
        cornerSubPix(cvmat1_mono_, corners_, subPixWinSize, zeroZone, subPixCriteria);
}

void HWAlgorithm2::FindCorners_Man(const cv::Point2f& center) {
    std::vector<cv::Point2f> corners;

    uint32_t patch_size = 30;
    cv::Mat patch = cvmat1_mono_(cv::Rect(center.x - patch_size / 2, center.y - patch_size / 2, patch_size, patch_size));

    uint32_t max_features = 1;
    double quality = 1e-2;
    double min_distance = 10;
    uint32_t block_size = 5;
    bool use_harris = false;

    // Detection
    cv::goodFeaturesToTrack(patch, corners, max_features, quality, min_distance, cv::Mat(), block_size, use_harris);

    // Refinement
    uint32_t subpix_refinement_window_size = 5;
    uint32_t subpix_refinement_iterations = 5;
    double subpix_refinement_min_accuracy = 1e-2;

    cv::Size subPixWinSize(subpix_refinement_window_size, subpix_refinement_window_size);
    cv::Size zeroZone = cv::Size(-1, -1);
    cv::TermCriteria subPixCriteria(cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS,
                                    subpix_refinement_iterations,
                                    subpix_refinement_min_accuracy);

    if (corners.size() != 0) {
        cornerSubPix(cvmat1_mono_, corners, subPixWinSize, zeroZone, subPixCriteria);
        corners[0].x += center.x;
        corners[0].y += center.y;
        corners_.push_back(corners[0]);

        cv::namedWindow("Corners");
        CvScalar blue = CV_RGB(64, 64, 255);
        cv::circle(cvmat1_, corners_.back(), 5, blue, 0);
        cv::imshow("Corners", cvmat1_);
    }
}

void HWAlgorithm2::FindMatches() {
    matches_.clear();

    // Template
    cv::Mat templ;
    uint32_t templ_size = 15;

    cv::Mat result;

    for (int i = 0; i < corners_.size(); ++i) {
        double x = corners_.at(i).x;
        double y = corners_.at(i).y;
        std::cout << "point: (" << x << ", " << y << ")" << "\n";

        if (x < templ_size / 2) x = templ_size / 2;
        if (y < templ_size / 2) y = templ_size / 2;
        if (width1_ - x < templ_size / 2) x = width1_ - templ_size / 2 - 1;
        if (height1_ - y < templ_size / 2) y = height1_ - templ_size / 2 - 1;

        templ = cvmat1_mono_(cv::Rect(x - templ_size / 2, y - templ_size /2, templ_size, templ_size));

        // Do the Matching and Normalize
        cv::matchTemplate(cvmat2_mono_, templ, result, CV_TM_CCORR_NORMED);
        cv::normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

        // Localizing the best match with maxLoc (NCC)
        double minVal, maxVal;
        cv::Point minLoc, maxLoc;

        cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

        matches_.push_back(maxLoc);
    }

    std::cout << "Number of matches: " << matches_.size() << "\n";
}

void HWAlgorithm2::FindMatches_Man() {
    // Template
    cv::Mat templ;
    uint32_t templ_size = 15;

    cv::Mat result;

    double x = corners_.back().x;
    double y = corners_.back().y;

    if (x < templ_size / 2) x = templ_size / 2;
    if (y < templ_size / 2) y = templ_size / 2;
    if (width1_ - x < templ_size / 2) x = width1_ - templ_size / 2 - 1;
    if (height1_ - y < templ_size / 2) y = height1_ - templ_size / 2 - 1;

    templ = cvmat1_mono_(cv::Rect(x - templ_size / 2, y - templ_size /2, templ_size, templ_size));

    // Do the Matching and Normalize
    cv::matchTemplate(cvmat2_mono_, templ, result, CV_TM_CCORR_NORMED);
    cv::normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

    // Localizing the best match with maxLoc (NCC)
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;

    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

    matches_.push_back(maxLoc);

    cv::namedWindow("Matches");
    CvScalar red = CV_RGB(255, 64, 64);
    cv::circle(cvmat2_, matches_.back(), 5, red, 0);
    cv::line(cvmat2_, corners_.back(), matches_.back(), red);
    cv::imshow("Matches", cvmat2_);
}

void HWAlgorithm2::FindAffine() {
    if (corners_.size() < 3 || matches_.size() < 3) {
        std::cout << "Not enough points for computing affine parameters." << "\n";
        return;
    }

    uint32_t n_points = corners_.size();

    Eigen::MatrixXd A(2 * n_points, 6);
    Eigen::MatrixXd b(2 * n_points, 1);
    Eigen::MatrixXd x(6, 1);

    for (int i = 0; i < n_points; ++i) {
        A.block(2 * i + 0, 0, 1, 2) << matches_.at(i).x, matches_.at(i).y;
        A.block(2 * i + 1, 2, 1, 2) << matches_.at(i).x, matches_.at(i).y;
        A.block(2 * i + 0, 4, 2, 2) << Eigen::Matrix2d::Identity();

        b.block(2 * i + 0, 0, 2, 1) << corners_.at(i).x, corners_.at(i).y;
    }

    // x = A^(-1)*b
    x = A.colPivHouseholderQr().solve(b);
    affine_ = x;

    std::cout << "Affine parameters:" << "\n";
    std::cout << affine_ << "\n";
}

void HWAlgorithm2::ShowImage() {
    CvScalar red = CV_RGB(255, 64, 64);
    CvScalar blue = CV_RGB(64, 64, 255);

    for (int i = 0; i < corners_.size(); ++i) {
        cv::circle(cvmat1_, corners_.at(i), 5, blue, 0);
    }

    for (int i = 0; i < matches_.size(); ++i) {
        cv::circle(cvmat2_, matches_.at(i), 5, red, 0);
        cv::line(cvmat2_, corners_.at(i), matches_.at(i), red);
    }

    cv::namedWindow("Corners");
    cv::namedWindow("Matches");
    cv::imshow("Corners", cvmat1_);
    cv::imshow("Matches", cvmat2_);

    cv::waitKey(0);
}

void HWAlgorithm2::Auto() {
    FindCorners();
    FindMatches();
    FindAffine();
    ShowImage();
}

void MouseCallBack(int event, int x, int y, int flags, void* ptr)
{
    HWAlgorithm2 * p = (HWAlgorithm2 *) ptr;

    if (event == cv::EVENT_LBUTTONDOWN) {
        cv::Point2f center = cv::Point2f(x, y);
        p->FindCorners_Man(center);
        p->FindMatches_Man();
        std::cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << "\n";
    } else if (event == cv::EVENT_RBUTTONDOWN) {
        p->FindAffine();
        std::cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << "\n";
    } else if (event == cv::EVENT_MOUSEMOVE) {
        //std::cout << "Mouse move over the window - position (" << x << ", " << y << ")" << "\n";
    }
}

void HWAlgorithm2::Mannual() {
    corners_.clear();
    matches_.clear();

    cv::namedWindow("HWAlgorithm2");

    cv::setMouseCallback("HWAlgorithm2", MouseCallBack, this);

    cv::imshow("HWAlgorithm2", cvmat1_);
}
