/*Face recognization*/
/*Lea Mikundova*/
/*FIT VUT BRNO 2015*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>

#include <stdlib.h>
#include <vector>

#include "facedetection.h"
#include "filemanagement.h"
#include "facerecognization.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    cv::VideoCapture mCapture;
    QTimer videoTimer;

private slots:

    void on_pushButton_StartDetectCollect_clicked();

    void on_pushButton_StopDetectCollect_clicked();

    void on_pushButton_LoadVideo_clicked();

    void timerVideoFrame();

    void ControlBoxes(int on);

    void on_pushButton_Recognize_clicked();

    void on_pushButton_StopRecognize_clicked();

    void Recognize();
    void Collect();

    void on_pushButton_LoadVideo_Recognizer_clicked();

    void on_pushButton_LoadCSV_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;

    int mode;

    float scaleFactor;
    int mCameraEventId;
    int frameRate;
    cv::VideoCapture capture;
    cv::Mat videoFrame;
    cv::Mat mOrigImage;
    cv::Mat mElabImage;
    QString videoName;
    QString csvName;
    string modelName;

    // ---> Face detectors
    cv::CascadeClassifier mFaceDetector;
    cv::CascadeClassifier mEyeDetector;
    cv::CascadeClassifier mMouthDetector;
    // <--- Face detectors

int scaledWidth;
    bool mouthDetect;
    bool eyesDetect;
    QString personName;

    Ptr<FaceRecognizer> model;
    int im_width;
    int im_height;



};

#endif // MAINWINDOW_H
