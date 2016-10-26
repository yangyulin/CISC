//
// Created by rpng on 7/26/16.
//

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <sstream>
#include <string>
#include <help.h>
#include <HWAlgorithm.h>


using namespace cv;
using namespace std;


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
//    namedWindow("The result of Convolve",WINDOW_AUTOSIZE);
//    imshow("The result of Convolve",imageOutput);

    /*
     * Part 1 Reduce(I)
     */
    HWpart1.Reduce(image,imageOutput);
//    namedWindow("The result of Reduce", WINDOW_AUTOSIZE);
//    imshow("The result of Reduce", imageOutput);

    /*
     * Part 1 Expand(I)
     */

    HWpart1.Expand(image,imageOutput, Size(image.cols*2,image.rows*2));
//    namedWindow("The result of Expand", WINDOW_AUTOSIZE);
//    imshow("The result of Expand", imageOutput);

    /*
     * Part 1 GaussianPyramid(I,n)
     * Part 1 LaplacianPyramid(I,n)
     * Part 1 Reconstruct
     */
    vector<Mat3f> imgOutputArr;
    Mat imgRecon;
    Mat baseMat;
    int numLevel = 5;

    cout<<"This is OK 1"<<endl;
    HWpart1.GaussianPyramid(image,imgOutputArr,numLevel);
    HWpart1.LaplacianPyramid(image,imgOutputArr,baseMat,numLevel);
    cout<<"This is OK 2 2"<<endl;
    HWpart1.Reconstruct(imgOutputArr,baseMat,numLevel,imgRecon);
    cout<<"This OK 1"<<endl;
    namedWindow("Reconstruction", WINDOW_AUTOSIZE);
    imshow("Reconstruction",(Mat3b)imgRecon);


    cout<<"This is oK 1"<<endl;

    /*
     * Part 1 Blend two images
     */

    string fileImg1 = "../data/aero1.jpg";
    string fileImg2 = "../data/aero3.jpg";
    //please choose a point in image 1
    cout<<"Please click a point in image 1 :"<<endl;
    //read image1
    Mat targetImg1 = imread(fileImg1,IMREAD_COLOR);
    namedWindow("Target Img1",WINDOW_AUTOSIZE);
    imshow("Target Img1",targetImg1);
    int boundary1 =0;
    int boundary2 =0;

    int counter = 0;
    Point p1;

    while (counter ==0){
        setMouseCallback("Target Img1", on_mouse, &p1);
        boundary1 = p1.x;
        cout << "The current selected blending boundary1 is x1 = " << boundary1 << endl;
        char key = (char) waitKey(0);
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
        char key = (char) waitKey(0);
        if (key == 'p') {
            destroyWindow("Target Img2");
            counter++;
        }
    }

    cout << "The selected blending boundary1 is x1 = " << boundary1 << endl;
    cout << "The selected blending boundary2 is x2 = " << boundary2 << endl;

    HWpart1.Blend(targetImg1,targetImg2,boundary1,boundary2,4,imageOutput);
    namedWindow("Blending",WINDOW_AUTOSIZE);
    imshow("Blending",(Mat3b)imageOutput);


    waitKey(0);



    return 0;
}
