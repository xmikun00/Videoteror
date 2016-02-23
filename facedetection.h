/*Face recognization*/
/*Lea Mikundova*/
/*FIT VUT BRNO 2015*/

#ifndef FACEDETECTION
#define FACEDETECTION

#include <opencv2/opencv.hpp>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <QDebug>

#include "filemanagement.h"

#define FEAT_FACE_FILE_HAAR  "cascadeFiles/haarcascade_frontalface_default.xml"
#define FEAT_FACE_FILE_LBP  "cascadeFiles/lbpcascade_frontalface.xml"
#define FEAT_EYE_FILE   "cascadeFiles/haarcascade_mcs_eyepair_big.xml"
#define FEAT_MOUTH_FILE "cascadeFiles/haarcascade_mcs_mouth.xml"

using namespace cv;




Mat faceDetection(Mat &videoFrame, double scaleFactor, int scaledWidth, bool eyesDetect, bool mouthDetect, CascadeClassifier &mFaceDetector, CascadeClassifier &mMouthDetector, CascadeClassifier &mEyeDetector, QString personName);



#endif // FACEDETECTION

