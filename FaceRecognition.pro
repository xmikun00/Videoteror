#-------------------------------------------------
#
# Project created by QtCreator 2015-05-17T18:20:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FaceRecognition
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    facedetection.cpp \
    filemanagement.cpp \
    facerecognization.cpp

HEADERS  += mainwindow.h \
    facedetection.h \
    filemanagement.h \
    facerecognization.h

FORMS    += mainwindow.ui

OPENCV_PATH += /usr/local/include/opencv2/

LIBS += /usr/local/lib/libopencv_calib3d.so \
    /usr/local/lib/libopencv_contrib.so \
    /usr/local/lib/libopencv_core.so \
    /usr/local/lib/libopencv_features2d.so \
    /usr/local/lib/libopencv_flann.so \
    /usr/local/lib/libopencv_gpu.so \
    /usr/local/lib/libopencv_highgui.so \
    /usr/local/lib/libopencv_imgproc.so \
    /usr/local/lib/libopencv_legacy.so \
    /usr/local/lib/libopencv_ml.so \
    /usr/local/lib/libopencv_objdetect.so \
    /usr/local/lib/libopencv_video.so

INCLUDEPATH += $$OPENCV_PATH/modules/core/include/ \ #core module
    $$OPENCV_PATH/modules/highgui/include/ \ #highgui module
    $$OPENCV_PATH/modules/objdetect/include/ #obj_detect module

message("OpenCV path: $$OPENCV_PATH")
message("Includes path: $$INCLUDEPATH")
message("Libraries: $$LIBS")

