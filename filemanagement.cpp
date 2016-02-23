/*Face recognization*/
/*Lea Mikundova*/
/*FIT VUT BRNO 2015*/

#include "filemanagement.h"


int counter;
const string jpg = ".jpg";
const string facesFolder = "FacesFolder";
const string facesFolderNamed = "temp_FacesFolder";
const string facesFolderCreate = "mkdir " + facesFolder;
const string facesFolderNamedCreate = "mkdir " + facesFolderNamed;

/*save face to specified folder*/
void saveFace (Mat image, QString personName) {
    stringstream s;
    string path;
    string ff = personName.toStdString();
    s << facesFolder << "/" << ff << "/" << "img" << counter<<jpg;
    path = s.str();
    cv::imwrite(path, image);
    counter ++;
    s.str("");
}

void saveFaceNamed (Mat &image, string person, int counterPerson) {
    stringstream s;
    string path;
    s<<facesFolderNamed<<"/"<<person<<counterPerson<<jpg;
    path = s.str();
    cv::imwrite(path,image);
    s.str("");
}

/*create new folders for photo saving*/
void createFolders(QString personName) {
    string ff = personName.toStdString();
    const string facesFolderPersonName = "mkdir " + facesFolder + "/" + ff;
    system(facesFolderCreate.c_str());
    system(facesFolderNamedCreate.c_str());
    system(facesFolderPersonName.c_str());
}

void createCSV() {
    struct dirent *entry;
    int ret = 1;
    DIR *dir;
    const char * c = facesFolder.c_str();
    dir = opendir(c);

    while ((entry = readdir (dir)) != NULL) {
        printf("\n%s",entry->d_name);
        cout << entry->d_name;
    }
}

/*walking throught csv with names of folders*/
void readCsv(const string& filename, vector<Mat>& images, vector<int>& labels) {
    char separator = ';';
    ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}


