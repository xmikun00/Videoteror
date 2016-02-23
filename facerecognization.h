/*Face recognization*/
/*Lea Mikundova*/
/*FIT VUT BRNO 2015*/

#ifndef FACERECOGNIZATION
#define FACERECOGNIZATION

#include <opencv2/opencv.hpp>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

#include "filemanagement.h"

Mat faceRecognition(Mat &videoFrame, double scaleFactor, int scaledWidth, CascadeClassifier &mFaceDetector, Ptr<FaceRecognizer> &model, int im_width, int im_height);

#endif // FACERECOGNIZATION

