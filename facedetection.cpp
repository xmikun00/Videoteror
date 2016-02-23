/*Face recognization*/
/*Lea Mikundova*/
/*FIT VUT BRNO 2015*/

#include "facedetection.h"

Mat oldFace;
double oldTime = 0;
int faceSize = 75;

Mat faceDetection(Mat &videoFrame, double scaleFactor, int scaledWidth, bool eyesDetect, bool mouthDetect, CascadeClassifier &mFaceDetector, CascadeClassifier &mMouthDetector, CascadeClassifier &mEyeDetector, QString personName ) {
    Mat defaultFace;
    Mat grayscaleFace;
    Mat equalizedFace;
    Mat resizedFace;
    Mat savedFace;

const double CHANGE_IMAGE = 0.5; //0.3;      // Change in face from last time
const double CHANGE_TIME = 0.5;//1.0;       // Time change old - new

/*grayscale image*/
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

/*shrinked image*/
     float scale = defaultFace.cols / (float)scaledWidth;
     if (defaultFace.cols > scaledWidth) {
         int scaledHeight = cvRound(defaultFace.rows / scale);
         resize(grayscaleFace, resizedFace, Size(scaledWidth, scaledHeight));
     }
     else {
         resizedFace = grayscaleFace;
     }

/**/
     vector< cv::Rect > faceVec;
     mFaceDetector.detectMultiScale( resizedFace, faceVec, scaleFactor );


     for( size_t i=0; i<faceVec.size(); i++ ) {
          cv::rectangle( defaultFace, faceVec[i], CV_RGB(255,200,0), 2 );
          cv::Mat face = resizedFace( faceVec[i] );
          // EyesDetection
          if( eyesDetect == 1 ) {
                vector< cv::Rect > eyeVec;
                mEyeDetector.detectMultiScale(face, eyeVec);
                for( size_t j=0; j<eyeVec.size(); j++ ) {
                    cv::Rect rect = eyeVec[j];
                    rect.x += faceVec[i].x;
                    rect.y += faceVec[i].y;
                    //saveFace(face);
                    cv::rectangle(resizedFace, rect, CV_RGB(0,255,0), 2 );
                }
            }
         // MouthDetection
         /*if mouth was detected on second part of face, face is equalized, normalized by size and saved, if it is not as the face saved last time.*/
         if( mouthDetect == 1 ) {
                vector< cv::Rect > mouthVec;
                Rect halfRect = faceVec[i];
                halfRect.height /= 2;
                halfRect.y += halfRect.height;
                Mat halfFace = videoFrame( halfRect );
                mMouthDetector.detectMultiScale( halfFace, mouthVec, 3 );
                for( size_t j=0; j<mouthVec.size(); j++ ) {
                    cv::Rect rect = mouthVec[j];
                    rect.x += halfRect.x;
                    rect.y += halfRect.y;

                    Mat equalizedImg;
                    equalizeHist(face, equalizedImg);

                    Mat face_resized;
                    resize(equalizedImg, face_resized, Size(faceSize, faceSize), 1.0, 1.0, INTER_CUBIC);

                    double imageDiff = 10000000000.0;
                    if (oldFace.data) {
                        imageDiff = norm(face_resized, oldFace, NORM_L2)/(double)(face_resized.rows * face_resized.cols);
                    }



                    double current_time = (double)getTickCount();
                    double timeDiff_seconds = (current_time - oldTime)/getTickFrequency();
                    cout << "imageDiff " << imageDiff << "  and timeDiff " << timeDiff_seconds <<  "  and oldtime " << oldTime << "  and currentTime " <<current_time << "\n";

                    if ((imageDiff > CHANGE_IMAGE) && (timeDiff_seconds > CHANGE_TIME)) {

                            saveFace(face_resized, personName);
                            qDebug() << "Photo was saved";
                            cv::rectangle( defaultFace, rect, CV_RGB(255,255,255), 2 );

                            Mat displayedFaceRegion = defaultFace( faceVec[i]);
                            displayedFaceRegion += CV_RGB(90,90,90);

                            oldFace = face_resized.clone();
                            oldTime = current_time;

                }
            }
         }

            return defaultFace;
        }
}

