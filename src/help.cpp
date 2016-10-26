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
    << "This program shows how to filter images with mask: the hand-coded way and the filter2d way. " <<endl
    << "usage:"                                                                                       <<endl
    << progName << " [image_name -- default ../data/lena.jpg] [G -- grayscale] "                      <<endl<<endl;
};