/*Face recognization*/
/*Lea Mikundova*/
/*FIT VUT BRNO 2015*/

#include "facerecognization.h"
#include "facedetection.h"
/*
FUnction from OpenCV, detect image, transform it to black/white, shrink and compare with result of model of face recognition*/
Mat faceRecognition(Mat &videoFrame, double scaleFactor, int scaledWidth, CascadeClassifier &mFaceDetector, Ptr<FaceRecognizer> &model, int im_width, int im_height) {
    Mat defaultFace;
    Mat grayscaleFace;
    Mat equalizedFace;
    Mat resizedFace;
    Mat savedFace;

    videoFrame.copyTo( defaultFace );

     if (defaultFace.channels() == 3) {
         cvtColor(defaultFace, grayscaleFace, CV_BGR2GRAY);
     }
     else if (defaultFace.channels() == 4) {
         cvtColor(defaultFace, grayscaleFace, CV_BGRA2GRAY);
     }
     else {
         grayscaleFace = defaultFace;
     }

     float scale = defaultFace.cols / (float)scaledWidth;
     if (defaultFace.cols > scaledWidth) {
         int scaledHeight = cvRound(defaultFace.rows / scale);
         resize(grayscaleFace, resizedFace, Size(scaledWidth, scaledHeight));
     }
     else {
         resizedFace = grayscaleFace;
     }


     vector< cv::Rect > faceVec;
     mFaceDetector.detectMultiScale( resizedFace, faceVec, scaleFactor );


     for( size_t i=0; i<faceVec.size(); i++ ) {
          cv::rectangle( defaultFace, faceVec[i], CV_RGB(255,0,0), 2 );
          cv::Mat face = resizedFace( faceVec[i] );

          Mat face_resized;
          resize(face, face_resized, Size(im_width, im_height), 1.0, 1.0, INTER_CUBIC);

          Mat equalizedImg;
          equalizeHist(face_resized, equalizedImg);

          int prediction = model->predict(face_resized);

          rectangle(defaultFace, faceVec[i], CV_RGB(0, 255,0), 1);
          string box_text = format("Person = %d", prediction);

          int pos_x = std::max(faceVec[i].tl().x - 10, 0);
          int pos_y = std::max(faceVec[i].tl().y - 10, 0);
          putText(defaultFace, box_text, Point(pos_x, pos_y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(100,100,200), 2.0);
          return defaultFace;
        }
}


