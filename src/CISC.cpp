//
// Created by rpng on 7/26/16.
// Delete debug information 11/07/16
//

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <boost/filesystem.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <help.h>
#include <HWAlgorithm.h>
#include <HWAlgorithm2.h>
#include <HWAlgorithm3.h>


using namespace cv;
using namespace std;
using namespace CISC;

void on_mouse( int e, int x, int y, int d, void *ptr ) {
    Point *p = (Point *) ptr;
    p->x = x;
    p->y = y;
}

int main( int argc, char** argv ){

    CISC::help instruction;
    instruction.instructionHelp(argv[0]);

    const char* filename = argc >=2 ? argv[1] : "../data/lena.jpg";

    Mat image, imageCp, imageOutput;

    image = imread(filename,IMREAD_COLOR);

    if(image.empty())
    {
        cout << "Can not load image " << filename << endl;
        return -1;
    }

    namedWindow("Input Image", WINDOW_AUTOSIZE);
    //namedWindow("Output", WINDOW_AUTOSIZE);


    imshow("Input Image", image);
    std::cout<<"The Input image is played!!"<<std::endl<<std::endl;

    /*
     * Part 1 Convolve(I,H)
     * input:     image
     * output:    imageOutput
     * mask:      Kern
     */
    //
    Mat Kern(3,3,CV_32F);
    Kern = (Mat_<float >(3,3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
    CISC::HWAlgorithm HWpart1;
    HWpart1.Convolve(image,imageOutput,Kern);
    namedWindow("The result of Convolve",WINDOW_AUTOSIZE);
    imshow("The result of Convolve",imageOutput);
    waitKey(0);

    /*
     * Part 1 Reduce(I)
     */
    HWpart1.Reduce(image,imageOutput);
    namedWindow("The result of Reduce", WINDOW_AUTOSIZE);
    imshow("The result of Reduce", imageOutput);
    waitKey(0);

    /*
     * Part 1 Expand(I)
     */

    HWpart1.Expand(image,imageOutput, Size(image.cols*2,image.rows*2));
    namedWindow("The result of Expand", WINDOW_AUTOSIZE);
    imshow("The result of Expand", imageOutput);
    waitKey(0);

    /*
     * Part 1 GaussianPyramid(I,n)
     * Part 1 LaplacianPyramid(I,n)
     * Part 1 Reconstruct
     */
    vector<Mat3f> imgOutputArr;
    Mat imgRecon;
    Mat imgReconOut;
    Mat baseMat;
    int numLevel = 5;

    HWpart1.GaussianPyramid(image,imgOutputArr,numLevel);
    HWpart1.LaplacianPyramid(image,imgOutputArr,baseMat,numLevel);
    HWpart1.Reconstruct(imgOutputArr,baseMat,numLevel,imgRecon);
    namedWindow("Reconstruction", WINDOW_AUTOSIZE);
    imshow("Reconstruction",(Mat3b)imgRecon);
    HWpart1.Convolve(imgRecon,imgReconOut,Kern);
    imshow("Sharp",(Mat3b)imgReconOut);
    waitKey(0);


    /*
     * Part 1 Blend two images
     */

    //string fileImg1 = "../data/aero1.jpg";
    //string fileImg2 = "../data/aero3.jpg";
    string fileImg1 = "../pr1_test_images/1/im1.png";
    string fileImg2 = "../pr1_test_images/1/im2.png";

    //string fileImg1 = "../pr1_test_images/re3/re31.png";
    //string fileImg2 = "../pr1_test_images/3/im33.png";
    //string fileImg3 = "../pr1_test_images/3/im33.png";

    //string fileImg1 = "../pr1_test_images/re4/re43.png";
    //string fileImg2 = "../pr1_test_images/4/im45.png";



    //please choose a point in image 1
    cout<<"Please click a point in image 1 :"<<endl;
    //read image1
    Mat targetImg1 = imread(fileImg1,IMREAD_COLOR);
    namedWindow("Target Img1",WINDOW_AUTOSIZE);
    imshow("Target Img1",targetImg1);
    int boundary1 =0;
    int boundary2 =0;
    int boundary3 =0;

    int counter = 0;
    Point p1;

    while (counter ==0){
        setMouseCallback("Target Img1", on_mouse, &p1);
        boundary1 = p1.x;
        cout << "The current selected blending boundary1 is x1 = " << boundary1 << endl;
        char key = (char) waitKey(50);
        if (key == 'p') {
            destroyWindow("Target Img1");
            counter++;
        }
    }
    //setMouseCallback("Target Img1",on_mouse, &p1);

    cout<<"Please click a point in image 2 :"<<endl;
    //read image2
    Mat targetImg2 = imread(fileImg2,IMREAD_COLOR);
    namedWindow("Target Img2",WINDOW_AUTOSIZE);
    imshow("Target Img2",targetImg2);
    Point p2;
    counter =0;
    while (counter ==0){
        setMouseCallback("Target Img2", on_mouse, &p2);
        boundary2 = p2.x;
        cout << "The current x2 = " << boundary2 << endl;
        char key = (char) waitKey(50);
        if (key == 'p') {
            destroyWindow("Target Img2");
            counter++;
        }
    }

 /*   cout<<"Please click a point in image 3 :"<<endl;
    //read image3
    Mat targetImg3 = imread(fileImg3,IMREAD_COLOR);
    namedWindow("Target Img3",WINDOW_AUTOSIZE);
    imshow("Target Img3",targetImg3);
    Point p3;
    counter =0;
    while (counter ==0){
        setMouseCallback("Target Img2", on_mouse, &p2);
        boundary3 = p3.x;
        cout << "The current x2 = " << boundary3 << endl;
        char key = (char) waitKey(50);
        if (key == 'p') {
            destroyWindow("Target Img3");
            counter++;
        }
    }*/





    cout << "The selected blending boundary1 is x1 = " << boundary1 << endl;
    cout << "The selected blending boundary2 is x2 = " << boundary2 << endl;
    //cout << "The selected blending boundary2 is x3 = " << boundary3 << endl;


    HWpart1.Blend(targetImg1,targetImg2,boundary1,boundary2,4,imageOutput);
    namedWindow("Blending",WINDOW_AUTOSIZE);
    imshow("Blending",(Mat3b)imageOutput);
    Mat imageSharp;
    HWpart1.Convolve(imageOutput,imageSharp,Kern);
    imshow("Sharp Output",(Mat3b)imageSharp);
    imwrite("../pr1_test_images/re4/re4.png",imageOutput);
    imwrite("../pr1_test_images/re4/re4s.png",imageSharp);


    waitKey(0);


/*    // Get image path
    boost::filesystem::path image1_path{"../data/rubberwhale1.png"};
    boost::filesystem::path image2_path{"../data/rubberwhale2.png"};

    cv::Mat image1, image2;
    image1 = cv::imread(image1_path.string());
    image2 = cv::imread(image2_path.string());

    if(!image1.data || !image2.data) {
        std::cout << "Could not open or find the image1/image2." << "\n";
        return -1;
    }

    CISC::HWAlgorithm2 part2(image1, image2);

    //part2.Auto();
    part2.Mannual();*/

    cv::waitKey(0);


    /*
     * Automatic unwarp the images
     *
     */

    //const char* inFile1 = "../data/rubberwhale1.png";
    //const char* inFile2 = "../data/rubberwhale2.png";
    //const char* inFile1 = "../data/1.jpg";
    //const char* inFile2 = "../data/2.jpg"
    //The Third group test images
/*    const char* inFile1 = "../pr1_affine_images/3/c1.JPG";
    const char* inFile2 = "../pr1_affine_images/3/c2.JPG";
    const char* inFile3 = "../pr1_affine_images/3/c3.JPG";*/

/*    //The Third group test images
    const char* inFile1 = "../pr1_affine_images/1/1.JPG";
    const char* inFile2 = "../pr1_affine_images/1/2.JPG";
    const char* inFile3 = "../pr1_affine_images/1/3.JPG";*/

    //The Third group test images
    const char* inFile1 = "../pr1_affine_images/2/1.JPG";
    const char* inFile2 = "../pr1_affine_images/2/2.JPG";
    const char* inFile3 = "../pr1_affine_images/2/3.JPG";


    Mat image11 = imread(inFile1);
    Mat image22 = imread(inFile2);
    Mat image33 = imread(inFile3);
/*
    //Mat imageC11(image11,Rect(0,2,image11.cols-5,image11.rows-1));
    Mat imageC11 = image11(Range::all(),Range(2,image11.cols-5));
    imwrite("../pr1_affine_images/3/c1.JPG",imageC11);
    //Mat imageC22(image22,Rect(0,2,image22.cols-5,image22.rows-1));
    Mat imageC22 = image22(Range::all(),Range(2,image22.cols-5));
    imwrite("../pr1_affine_images/3/c2.JPG",imageC22);
    //Mat imageC33(image33,Rect(0,2,image33.cols-5,image33.rows-1));
    Mat imageC33 = image33(Range::all(),Range(2,image33.cols-5));
    imwrite("../pr1_affine_images/3/c3.JPG",imageC33);
*/

    Mat greyImg1, greyImg2, greyImg3;
    cvtColor(image11,greyImg1,CV_RGB2GRAY);
    cvtColor(image22,greyImg2,CV_RGB2GRAY);
    cvtColor(image33,greyImg3,CV_RGB2GRAY);

    imshow("1",image11);
    imshow("2",image22);
    imshow("3",image33);

    cv::waitKey(0);

    Mat panoImg1;
    Mat panoImg2;

    //step1
    HWAlgorithm3 warpImg;
    warpImg.unWarp(image33,image22,panoImg1);
    warpImg.unWarp(panoImg1,image11,panoImg2);
    imshow("final",panoImg2);
    imwrite("../pr1_affine_images/re2/re2.JPG",panoImg2);
    waitKey(0);


    return 0;
}
