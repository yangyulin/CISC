//
// Created by rpng on 7/27/16.
//

#include "HWAlgorithm.h"

using namespace std;
using namespace cv;


CISC::HWAlgorithm::HWAlgorithm(){};


void CISC::HWAlgorithm::Convolve(const Mat& inputImage, Mat& outputImage, const Mat& maskH){

    //directly use the filter2D for the mask operation
    filter2D(inputImage, outputImage,inputImage.depth(),maskH);

    //display the results after the Convolve operation
    //namedWindow("Output After Convolve",WINDOW_AUTOSIZE);
    //imshow("Output After Convolve",outputImage);
}

void CISC::HWAlgorithm::Reduce(const Mat &inputImage, Mat &outputImage) {

    //directly use pyrDown
    pyrDown(inputImage, outputImage, Size((inputImage.cols+1)/2,(inputImage.rows+1)/2));
}

void CISC::HWAlgorithm::Expand(const Mat &inputImage, Mat &outputImage, Size sizeTarget) {
    //directly use pryUp
    //pyrUp(inputImage,outputImage,Size(inputImage.cols*2,inputImage.rows*2));
    pyrUp(inputImage,outputImage,sizeTarget);
}

void CISC::HWAlgorithm::GaussianPyramid(const Mat &inputImage, vector<Mat3f> &outputPyramid, int numLevel) {

    outputPyramid.clear();
    buildPyramid(inputImage,outputPyramid,numLevel);


    //for debug
/*    string s="GLevel";
    for(int i=0; i<numLevel+1; i++){
        std::ostringstream sin;
        sin << i;
        //std::string val = sin.str();
        std::string winName = sin.str() + s;
        namedWindow(winName,WINDOW_AUTOSIZE);
        imshow(winName,outputPyramid[i]);

    }
*/

}

void CISC::HWAlgorithm::LaplacianPyramid(const Mat &inputImage, vector<Mat3f>  &outputPyramid, Mat &baseMat, int numLevel) {
    outputPyramid.clear();
    Mat tempImage = inputImage;
    for(int nl =0; nl<numLevel; nl++){
        Mat down, up;
        Reduce(tempImage,down);
        Size sizeTarget = tempImage.size();
        Expand(down,up,sizeTarget);
        Mat lapPyr = tempImage - up;
        outputPyramid.push_back(lapPyr);
        tempImage = down;
    }
    cout<< "OK99"<<endl;
    tempImage.copyTo(baseMat);


/*    //for debug
    string s="LLevel";
    for(int i=0; i<outputPyramid.size(); i++){
        std::ostringstream sin;
        sin << i;
        //std::string val = sin.str();
        std::string winName = sin.str() + s;
        namedWindow(winName,WINDOW_AUTOSIZE);
        imshow(winName,outputPyramid[i]);
    }
    imshow("Lbase",baseMat);*/
}

void CISC::HWAlgorithm::Reconstruct(const vector<Mat3f> &inputPyramid, const Mat &baseMat, int numLevel, Mat &outputImage) {

    //int realNum = inputPyramid.size();
    baseMat.copyTo(outputImage);

    for(int nl=numLevel-1; nl>=0; nl--){
        Mat up;
        Expand(outputImage, up, inputPyramid[nl].size());
        outputImage = (Mat3f)up + inputPyramid[nl];

/*        //for debug
        std::ostringstream sin;
        string s = "R Level ";
        sin << nl;
        std::string winName = sin.str()+s;
        namedWindow(winName,WINDOW_AUTOSIZE);
        imshow(winName,(Mat3b)outputImage);
*/
    }

}


void CISC::HWAlgorithm::Blend(const Mat &inImg1, const Mat &inImg2, int boundary1, int boundary2, int numLevel, Mat& outputImage) {

    int left1 = boundary1;
    int right1 = inImg1.cols-left1;
    int left2 = boundary2;
    int right2 = inImg2.cols-left2;

    Mat leftCom, rightCom, midCom, result;
    vector<Mat3f> mask;
    vector<Mat3f> leftM,rightM,resultM;

    Size reSize;
    Size maskSize;
    //reSize.width = inImg1.cols+inImg2.cols-max
    reSize.height=inImg1.rows;
    maskSize.height = inImg1.rows;
    double ratio;
    double boundary;
    double len1;
    double len2;
    double len3;
    double len4;
    if(left1>=left2){
        boundary = left1;
        maskSize.width = left2+right1;
        reSize.width = inImg1.cols+inImg2.cols-maskSize.width;
        ratio = (left1-left2)/reSize.width;
        len1 = left1-left2;
        len2 = left2;
        len3 = right1;
        len4 = right2-right1;

    }else {
        boundary = left2;
        maskSize.width = left1+right2;
        reSize.width = inImg1.cols+inImg2.cols-maskSize.width;
        cout<<reSize.height<<endl<<reSize.width<<endl;
        ratio = (left2-left1)/reSize.width;
        len1 = left2-left1;
        len2 = left1;
        len3 = right2;
        len4 = right1-right2;
    }
    cout<<reSize.height<<endl<<reSize.width<<endl;

    //Mat_<float> blendMask1(maskSize.height,maskSize.width,0.0);
    //blendMask1(Range::all(),Range(0,(int)len2))=1.0;
    cout<<"OK"<<endl;
    Mat_<float> blendMask(reSize.height,reSize.width,0.0);
    blendMask(Range::all(),Range(0,(int)boundary))=1.0;


    cout<<"OK2"<<endl;
    //cout<<inImg1.depth()<<endl;

    //Mat leftImg(reSize.height,reSize.width,CV_8UC3,Scalar(0,0,0));
    //Mat leftImg(inImg1);
    //leftImg.reshape(0,reSize.height,reSize.width);
    //Mat Img(reSize.height,reSize.width,CV_32F);

    //cout<<"the type for leftImg "<<leftImg.type()<<endl;
    //cout<<leftImg.channels()<<endl;
    cout<<"the type for inImg1 "<<inImg1.type()<<endl;
    cout<<inImg1.channels()<<endl;

    //Mat leftImg;
    //cvtColor(Img,leftImg,CV_GRAY2BGR);
    cout<<"reSize"<<reSize.height<<endl<<reSize.width<<endl;
    cout<<inImg1.rows<<endl<<inImg1.cols<<endl;
//    (Mat3b)inImg1;
    //inImg1.copyTo(leftImg(cv::Rect(0,0,inImg1.rows,inImg1.cols)));
//    leftImg(Range::all(),Range(0,(int)inImg1.cols-1)) = inImg1;
//    imshow("img1",inImg1);
//    imshow("Left",leftImg);
    //Mat leftImg(inImg1);
    Mat leftImg(reSize.height,reSize.width,CV_8UC3,Scalar(0,0,0));
    //resize(inImg1,leftImg,reSize);
    //leftImg.setTo(0);
    //inImg1.copyTo(leftImg(cv::Rect(0,0,inImg1.rows,inImg1.cols)));
    FillIn(inImg1,leftImg,0,0);
    cout<<"OOOK"<<endl;
    imshow("img1",inImg1);
    imshow("Left",leftImg);

    cout<<"OK1"<<endl;
    Mat rightImg(reSize.height,reSize.width,CV_8UC3,Scalar(0,0,0));
    //rightImg(Range::all(),Range((int)(len1-1),(int)(reSize.width-1))) = inImg2;
    //Mat rightImg(inImg2);
    FillIn(inImg2,rightImg, 0,rightImg.cols-inImg2.cols);
    //rightImg.convertTo(rightImg,CV_8UC3,1.0);
    //resize(inImg2,rightImg,reSize);
    imshow("Right",rightImg);

    cout<<"OK33"<<endl;
    //first construct the lap mask
    Mat blendMask3;
    cvtColor(blendMask,blendMask3,CV_GRAY2BGR);
    cout<<"OK44"<<endl;
    GaussianPyramid(blendMask3,mask,numLevel);
    cout<<"OK55"<<endl;
    LaplacianPyramid(leftImg,leftM,leftCom,numLevel);
    cout<<"OK66"<<endl;
    LaplacianPyramid(rightImg,rightM,rightCom,numLevel);

    cout<<"OK4"<<endl;
    //blend the laplacian pyr
    cout<<leftCom.cols<<endl<<leftCom.rows<<endl<<endl;
    cout<<mask.back().cols<<endl<<mask.back().rows<<endl;

    Mat3f lC;
    Mat3f rC;
    leftCom.convertTo(lC,CV_32F,1.0);
    cout<<"this is OK6666"<<endl;
    rightCom.convertTo(rC,CV_32F,1.0);
    //result = (Mat3f)leftCom.mul(mask.back()) + (Mat3f)rightCom.mul((Scalar(1.0,1.0,1.0)-mask.back()));
    result = lC.mul(mask.back()) + rC.mul((Scalar(1.0,1.0,1.0)-mask.back()));
    cout<<"OK7"<<endl;
    for(int nl=0;nl<numLevel;nl++){
        Mat A = leftM[nl].mul(mask[nl]);
        Mat B = rightM[nl].mul(Scalar(1.0,1.0,1.0)-mask[nl]);
        midCom =A+B;
        resultM.push_back(midCom);
    }

    Reconstruct(resultM,result,numLevel,outputImage);

}

void CISC::HWAlgorithm::FillIn(const Mat &inImg, Mat &outImg, int startRow, int startCol) {

    Mat_<Vec3f> _inImg = inImg;
    Mat_<Vec3f> _outImg = outImg;
    //inImg.convertTo(_inImg,CV_32F,1.0);
    //Mat_<Vec3b> _inImg = inImg;
    //Mat_<Vec3b> _outImg = outImg;


    for(int i = startRow; i< startRow+inImg.rows;++i){
        for(int j = startCol;j< startCol+inImg.cols;++j){
            _outImg(i,j)[0] = _inImg(i-startRow,j-startCol)[0];
            _outImg(i,j)[1] = _inImg(i-startRow,j-startCol)[1];
            _outImg(i,j)[3] = _inImg(i-startRow,j-startCol)[3];
        }
    }
    outImg = _outImg;
}























































































