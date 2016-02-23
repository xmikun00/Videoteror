/*Face recognization*/
/*Lea Mikundova*/
/*FIT VUT BRNO 2015*/

#ifndef FILEMANAGEMENT
#define FILEMANAGEMENT

#include <iostream>
#include <fstream>
#include <sstream>

#include <stdio.h>

#include <QString>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <dirent.h>

using namespace std;
using namespace cv;


void saveFaceNamed (Mat image, string person, int counterPerson);
void saveFace (Mat image, QString personName);
void createFolders (QString personName);
bool on_pushButton_LoadVideo_clicked();
void createCSV();
void readCsv(const string& filename, vector<Mat>& images, vector<int>& labels);

#endif // FILEMANAGEMENT

