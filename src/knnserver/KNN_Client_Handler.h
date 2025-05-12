#ifndef AP_1_EX_2_SERVER_H
#define AP_1_EX_2_SERVER_H

#include <fstream>
#include "../knnset/KNN_Set.h"
#include "AbstractPeer.h"
#include "../cli/DefaultIO.h"

#define BUFFER_LEN 4096

using namespace std;

class KNN_Client_Handler {
public:

    static void runClientHandler(int socket);

    static void runWithoutClient();


    // add a dataset to the server
    void addDataset(string filename);

    // add an unclassified dataset to the server
    void addUnclassifiedSet(string filename);

    // operate on the input given by the algoSetting command
    void operateOnInput(string input, DefaultIO& dio);

    int getKVal();

    string getDistanceMethod();

    void setUpload();

    void classify(DefaultIO& dio);

    void writeResultsToIO(DefaultIO& dio);

    void writeResultsToFile(DefaultIO& dio);

    static void downloadResults(DefaultIO& dio);

private:

    // server's socket descriptor
    int sock;

    // dataset of k nearest neighbors
    KNN_Set dataset;

    // dataset of unclassified vectors
    vector<vector<double>> unclassified;

    // dataset of classified vector (previously unclassified)
    vector<ClassifiedVector> result;

    bool uploadedData; // please document

    bool classifiedData; // please document

    // queue of files to be downloaded by client
    queue<string> download_queue;

    explicit KNN_Client_Handler(int socket);

    /**
     * Parse the given regex pattern string from the whole input buffer
     * @param buff buffer
     * @param pat regex pattern
     * @return string matching the pattern
     */
    static string parseFromBuff(string buff, const regex& pat);

    /**
     * Parse input string to vector, distance method, and k neighbors.
     * Then classify the vector using the dataset, and return classification.
     * @param inputMsg input string from client
     * @return string representation of classification of the input vector parsed from inputMsg
     */
    string getTypeFromInput(const string &inputMsg);

    // set k and dist based on input from client
    string setValuesFromInput(const string &input);

    string parseK(const string &inputMsg);

    string parseDistanceMetric(const string &inputMsg);

    void setValues(const string &kValStr, const string &distMethod);

    bool dataExists(DefaultIO& dio); // please document

    bool dataClassified(DefaultIO& dio); // please document

    /**
     * takes result vector i and returns a formatted result string
     * @param i index of result vector
     * @return string for result vector formatted as follows: <index>   <typename>
     */
    string getPrintFormat(int i);

};

#endif //AP_1_EX_2_SERVER_H
