//
// Created by omer on 1/15/23.
//

#include "UploadCSV.h"

const string cmdDescription{"upload an unclassified csv data file"};

// constructor. how to reference the KNN_Server?
UploadCSV::UploadCSV(KNN_Client_Handler& clientServerPtr, DefaultIO& dio) :
    Command(cmdDescription, dio, clientServerPtr){
}

void UploadCSV::execute() {
    dio.write("Please upload your local train CSV file.");
    string linkToCSV = dio.read();
    // parse name of file to get fileName.
    // when using socketIO we need to create the file

    // open file
    fopen(linkToCSV.c_str(), "a");// do we need this line here? should it be in socketIO's read?

    // upload content to dataset here using KNN_Set.readFromCsv(fileName)
    client_server.addDataset(linkToCSV); // or maybe like this?

    dio.write("Upload complete.");
    dio.write("Please upload your local test CSV file.");

    //... upload content to unclassified dataset using a new function we create in KNN_Set
    string linkToTestCSV = dio.read();
    client_server.addUnclassifiedSet(linkToTestCSV);

    client_server.setUpload();

    dio.write("Upload complete.");


}