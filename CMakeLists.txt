CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(CISC)

SET(CMAKE_MODULE_PATH  ${PROJECT_SOURCE_DIR}/cmake_modules)
find_package(G2O REQUIRED)
find_package(OpenCV 3 REQUIRED)
find_package(SuiteSparse REQUIRED)
find_package(Eigen3 REQUIRED)

INCLUDE_DIRECTORIES(${CHOLMOD_INCLUDE_DIR}
                    ${CSPARSE_INCLUDE_DIR}
                    ${OpenCV_INCLUDE_DIRS}
                    ${G2O_INCLUDE_DIRS}
                    ${EIGEN3_INCLUDE_DIR}
                    include)

ADD_EXECUTABLE( CISC
                src/CISC.cpp include/HWAlgorithm.h src/HWAlgorithm.cpp include/help.h src/help.cpp)

TARGET_LINK_LIBRARIES(  CISC
                        ${OpenCV_LIBS}
                        ${G2O_LIBRARIES}
                        csparse
                        cxsparse)

#SET(CUDA_USE_STATIC_CUDA_RUNTIME = OFF)

#LIST(APPEND CMAKE_MODULE_PATH /home/rpng/linde/project/cmake_modules)

#FIND_Package(G2O)
#FIND_Package(Eigen3)
