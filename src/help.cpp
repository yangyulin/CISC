//
// Created by rpng on 7/27/16.
//

#include "help.h"

using namespace std;
using namespace CISC;

help::help(){};

void CISC::help::instructionHelp(char *progName) {
    cout
    << "\n--------------------------------------------------------------------" <<endl
    << "This program shows how to blend images and calculate the affine transformation between 2 images. " <<endl
    << "usage:"                                                                                       <<endl
    << progName << " [image_name -- default ../data/lena.jpg]  "                      <<endl
    <<" Press P to choose the point coordinate form images for image blending! "<<endl
    <<"Directly select the point for affine transformation with left click, and to end the selection by right click!"<<endl<<endl;
};