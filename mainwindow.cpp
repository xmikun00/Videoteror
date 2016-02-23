/*Face recognization*/
/*Lea Mikundova*/
/*FIT VUT BRNO 2015*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

const int DETECTION = 0;
const int RECOGNITION = 1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ControlBoxes(1);
/*set up the signals for functions*/
    connect(&videoTimer, SIGNAL(timeout()), this, SLOT(timerVideoFrame()));
    connect(ui->pushButton_StopDetectCollect, SIGNAL(clicked()), this, SLOT(on_pushButton_StopDetectCollect_clicked()));
    connect(ui->pushButton_StartDetectCollect, SIGNAL(clicked()), this, SLOT(Collect()));
    connect(ui->pushButton_Recognize, SIGNAL(clicked()), this, SLOT(Recognize()));
    scaledWidth = 10000;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Collect() {
    mode = DETECTION;
}

void MainWindow::Recognize() {
    mode = RECOGNITION;
}

void MainWindow::timerVideoFrame() {
    if( !mCapture.isOpened() )
    {
        if (videoName != "") {
            string VN = videoName.toStdString();
            mCapture.open(VN);
        }
        else {
            mCapture.open(0);
        }

        mCapture.set(CV_CAP_PROP_FOURCC ,CV_FOURCC('Y', 'U', 'Y', 'V') );
        /*mCapture.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
        mCapture.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);*/
        qDebug() << "Video input is active.";
    }


   mCapture >> videoFrame;

   Mat detectedFace;
   if (mode == RECOGNITION) {
        detectedFace = faceRecognition(videoFrame, scaleFactor, scaledWidth, mFaceDetector, model, im_width, im_height);
   }
   if (mode == DETECTION){
       detectedFace = faceDetection(videoFrame, scaleFactor, scaledWidth, eyesDetect, mouthDetect, mFaceDetector, mMouthDetector, mEyeDetector, personName);
   }

   Mat color;
   cv::cvtColor( detectedFace, color, CV_BGR2RGB );

   QImage resultVideo = QImage( (uchar*)color.data, color.cols, color.rows, color.step, QImage::Format_RGB888 );

    if(resultVideo.isNull())
        return;
    QPixmap labelVideo  = QPixmap::fromImage(resultVideo);



    if (mode == RECOGNITION) {
        ui->videoWidget_Recognization->setPixmap(labelVideo);
    }
    if (mode == DETECTION) {
        ui->videoWidget->setPixmap(labelVideo);
    }

}

void MainWindow::on_pushButton_StartDetectCollect_clicked()
{
    QString file;


    if( mFaceDetector.empty() )
        {
            if ((ui->checkBox_LBP->isChecked()) & (ui->checkBox_HAAR->isChecked() )) {
                        QMessageBox::warning((QWidget *)this,tr("Error"),
                                                     tr("Please, check ONLY ONE cascade file."));
                        videoTimer.stop();
                        return;
                    }
            if (ui->checkBox_HAAR->isChecked() ) {
                file = tr("%1/%2").arg(QApplication::applicationDirPath()).arg(FEAT_FACE_FILE_HAAR);
            }
            else if (ui->checkBox_LBP->isChecked()) {
                file = tr("%1/%2").arg(QApplication::applicationDirPath()).arg(FEAT_FACE_FILE_LBP);
            }
            else {
                QMessageBox::warning((QWidget *)this,tr("Error"),
                                             tr("Please, check ONE cascade file."));
                videoTimer.stop();
                return;
            }

            if(!mFaceDetector.load( file.toLatin1().constData() ))
                qDebug() << tr("Cannot find %1").arg(file);
        }

    if( mEyeDetector.empty() )
    {
        file = tr("%1/%2").arg(QApplication::applicationDirPath()).arg(FEAT_EYE_FILE);
        if(!mEyeDetector.load( file.toLatin1().constData() ))
            qDebug() << tr("Cannot find %1").arg(file);
    }

    if( mMouthDetector.empty() )
    {
        file = tr("%1/%2").arg(QApplication::applicationDirPath()).arg(FEAT_MOUTH_FILE);
        if(!mMouthDetector.load( file.toLatin1().constData() ))
            qDebug() << tr("Cannot find %1").arg(file);
    }

    if( (ui->checkBox_eyes->isChecked() == 0) && ( ui->checkBox_mouth->isChecked() == 0) ) {
            QMessageBox::warning((QWidget *)this,tr("Warning"),
                                         tr("Without \"Detect by\" the results could be bad."));
        }

    if (ui->checkBox_eyes->isChecked() == 1)
        eyesDetect = 1;
    else
        eyesDetect = 0;

    if (ui->checkBox_mouth->isChecked() == 1)
        mouthDetect = 1;
    else
        eyesDetect = 0;

    ControlBoxes(0);

    scaleFactor = ui->doubleSpinBox_scaleFactor->value();
    personName  = ui->personName->toPlainText();

    if (personName == "") {
        QMessageBox::warning((QWidget *)this,tr("Error"),
                                     tr("Cannot start detecting without name of detected person."));
        videoTimer.stop();
        return;
    }

    createFolders(personName);
    videoTimer.start(20);

}

void MainWindow::on_pushButton_StopDetectCollect_clicked()
{
    videoTimer.stop();
    mCapture.release();

    ControlBoxes(1);
}

void MainWindow::on_pushButton_LoadVideo_clicked()
{
    videoName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Videos (*.MP4 *.mp4 *.avi *.webm)"));
    createCSV();

}

void MainWindow::on_pushButton_LoadVideo_Recognizer_clicked()
{
    videoName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Videos (*.MP4 *.mp4 *.avi *.webm)"));
    createCSV();
}



void MainWindow::ControlBoxes(int on) {
    if (on == 1) {
        ui->checkBox_eyes->setEnabled( true );
        ui->checkBox_mouth->setEnabled( true );
        ui->checkBox_LBP->setEnabled( true );
        ui->checkBox_HAAR->setEnabled( true );
        ui->doubleSpinBox_scaleFactor->setEnabled(true);
        ui->pushButton_LoadVideo->setEnabled(true);
    }
    else if (on == 0) {
        ui->checkBox_eyes->setEnabled( false );
        ui->checkBox_mouth->setEnabled( false );
        ui->checkBox_LBP->setEnabled( false );
        ui->checkBox_HAAR->setEnabled( false );
        ui->doubleSpinBox_scaleFactor->setEnabled(false);
        ui->pushButton_LoadVideo->setEnabled(false);
    }
    else {
        qDebug() << tr("Some problem with boxes");
    }
}

void MainWindow::on_pushButton_Recognize_clicked()
{
    QString file;

    eyesDetect = 0;
    mouthDetect = 0;

    if( mFaceDetector.empty() )
        {
            if ((ui->checkBox_LBP->isChecked()) & (ui->checkBox_HAAR->isChecked() )) {
                        QMessageBox::warning((QWidget *)this,tr("Error"),
                                                     tr("Please, check ONLY ONE cascade file."));
                        videoTimer.stop();
                        return;
                    }
            if (ui->checkBox_HAAR->isChecked() ) {
                file = tr("%1/%2").arg(QApplication::applicationDirPath()).arg(FEAT_FACE_FILE_HAAR);
            }
            else if (ui->checkBox_LBP->isChecked()) {
                file = tr("%1/%2").arg(QApplication::applicationDirPath()).arg(FEAT_FACE_FILE_LBP);
            }
            else {
                QMessageBox::warning((QWidget *)this,tr("Error"),
                                             tr("Please, check ONE cascade file."));
                videoTimer.stop();
                return;
            }

            if(!mFaceDetector.load( file.toLatin1().constData() ))
                qDebug() << tr("Cannot find %1").arg(file);
        }


    ControlBoxes(0);

    scaleFactor = ui->doubleSpinBox_scaleFactor->value();

    vector<Mat> images;
    vector<int> labels;
    string CSV;

    if (csvName != "") {
        CSV = csvName.toStdString();
    }
    else {
        QMessageBox::warning((QWidget *)this,tr("Error"),
                                     tr("Please, upload ONE CSV file."));
        return;
    }

    try {
            readCsv(CSV, images, labels);
        } catch (cv::Exception& e) {
            cerr << "Error opening file \"" << "pokus.csv" << "\". Reason: " << e.msg << endl;
            // nothing more we can do
            exit(1);
        }

    im_width = images[0].cols;
    im_height = images[0].rows;
    // Create a FaceRecognizer and train it on the given images:
    if (modelName == "") {
        model = createEigenFaceRecognizer();
    }
    else if (modelName == "FisherFace") {
        model = createFisherFaceRecognizer();
    }
    else if (modelName == "EigenFace") {
        model = createEigenFaceRecognizer();
    }
    else if (modelName == "LBPH") {
        model = createLBPHFaceRecognizer();
    }
    else
        return;

cout << "model name " << modelName;
    qDebug() << "Model is training.";
    model->train(images, labels);
    qDebug() << "Model is ready.";

    videoTimer.start(20);

}

void MainWindow::on_pushButton_StopRecognize_clicked()
{
    videoTimer.stop();
    mCapture.release();
    qDebug() << "Video input is unactive.";
}




void MainWindow::on_pushButton_LoadCSV_clicked()
{
    csvName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("*.csv *.CSV"));
}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    modelName = arg1.toStdString();

}
