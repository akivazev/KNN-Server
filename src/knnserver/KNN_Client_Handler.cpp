#include "KNN_Client_Handler.h"
#include "../cli/CLI.h"

KNN_Client_Handler::KNN_Client_Handler(int socket) : sock(socket), dataset(KNN_Set()), uploadedData(false) {}

int KNN_Client_Handler::getKVal() {
    return dataset.getK();
}

string KNN_Client_Handler::getDistanceMethod() {
    return dataset.getDistanceMethodName();
}

void KNN_Client_Handler::setUpload() {
    uploadedData = true;
}

bool KNN_Client_Handler::dataExists(DefaultIO& dio) {
    if (!uploadedData) {
        dio.write("please upload data");
        return false;
    }
    return true;
}

bool KNN_Client_Handler::dataClassified(DefaultIO& dio) {
    if(!classifiedData) {
        dio.write("please classify the data");
        return false;
    }

    return true;
}

void KNN_Client_Handler::classify(DefaultIO& dio) {

    if(!dataExists(dio)) return;

    int k = dataset.getK();
    for (auto &vec : unclassified) {
        ClassifiedVector classifiedVector(vec, dataset.getType(vec, k));
        result.push_back(classifiedVector);
    }

    classifiedData = true;

    dio.write("classifying data complete");

//    can be written also like this. but in my opinion less readable
//
//    for (auto &vec : unclassified) {
//        string type = dataset.getType(vec, dataset.getK());
//        ClassifiedVector classifiedVector = ClassifiedVector(vec, type);
//        addVecToResult(classifiedVector);
//    }
}

void KNN_Client_Handler::writeResultsToIO(DefaultIO& dio) {

    if (!(dataExists(dio) || !dataClassified(dio)))
        return;


    // write the results to dio
    int resultSize = result.size();
    for (int i = 1; i <= resultSize; i++) {
        dio.write(getPrintFormat(i));
    }

    dio.write("Done");
}

void KNN_Client_Handler::writeResultsToFile(DefaultIO& dio) {

    if (!(dataExists(dio) || !dataClassified(dio)))
        return;

    ofstream file("result.txt");

    if (!file.is_open()) {
        dio.write("Unable to open file");
        return;
    }

    // write the results to file
    int resultSize = result.size();
    for (int i = 1; i <= resultSize; i++) {
        file << getPrintFormat(i) << endl;
    }

    file.close();

}

string KNN_Client_Handler::getPrintFormat(int i) {
    std::stringstream s;
    s << i << "\t" << result[i-1].getTypeName();
    return s.str();
}

void KNN_Client_Handler::downloadResults(DefaultIO& dio) {
    dio.readToFile("result.txt");
//    ifstream file("result.txt");
//    string line;
//
//    if(file.is_open()) {
//        while (getline(file, line)) {
//            dio.write(line);
//        }
//        file.close();
////        remove(file);
//    }
//    else
//        dio.write("Unable to open file");
//
//    return;
}


void KNN_Client_Handler::runClientHandler(int socket) {

    // Initialize client handler object
    KNN_Client_Handler server(socket);
    StandardIO sio;
    DefaultIO& dio_ref = sio;
    CLI command_interface(server, dio_ref);
    // initialize CLI, KNN_Set
    // run commands until exit
    command_interface.start();
}

void KNN_Client_Handler::runWithoutClient() {
    KNN_Client_Handler server(5);
    StandardIO sio;
    DefaultIO& dio_ref = sio;
    CLI command_interface(server, dio_ref);
    // initialize CLI, KNN_Set
    // run commands until exit
    command_interface.start();
}

/**
 * Parse input string to vector, distance method, and k neighbors.
 * Then classify the vector using the dataset, and return classification.
 * @param inputMsg input string from client
 * @return string representation of classification of the input vector parsed from inputMsg
 */
string KNN_Client_Handler::getTypeFromInput(const string &inputMsg) {
    const static regex vectorPat {R"((\s|^)((\d+\.?|\d+\.\d+|\.\d+)(E(\+|\-)\d+)?(\s))+)"};
    const static regex distancePat {"(AUC|MAN|CHB|CAN|MIN)"};
    const static regex kPat {"(\\d+\\s*$)"};

    // convert input string of vector to actual vector
    vector<double> inputVector = VectorDist::inputToVector(parseFromBuff(inputMsg, vectorPat));

    // parse distance method from message
    string distMethod = parseFromBuff(inputMsg, distancePat);

    // parse k value from message
    int kVal;
    try {
        kVal = stoi(parseFromBuff(inputMsg, kPat));
    } catch (exception &err) {
        throw invalid_argument("error: k value too large");
    }

    // get vector type using KNN algorithm
    return dataset.getType(inputVector, kVal, distMethod);
}

/**
 * Parse input string to distance method, and k.
 * Then set new k and distance values.
 * @param inputMsg input string from client
 * @return string representation of classification of the input vector parsed from inputMsg
 */
string KNN_Client_Handler::setValuesFromInput(const string &inputMsg) {
    const static regex distancePat {"(AUC|MAN|CHB|CAN|MIN)"};
    const static regex kPat {"(^\\s*\\d+)"};
    std::stringstream s;

    if (inputMsg == "/n") {
            return {};
    }

    // parse k value from message
    int kVal;
    string kValStr = parseFromBuff(inputMsg, kPat);
    if (kValStr.empty()) {
        s << "invalid value for K" << endl;
    } else {
        try {
            kVal = stoi(kValStr);
        } catch (exception &err) {
            throw invalid_argument("invalid value for K");
        }

        dataset.setK(kVal);
    }


    // parse distance method from message
    string distMethod = parseFromBuff(inputMsg, distancePat);

    if (distMethod.empty()) {
        s << "invalid value for metric";
    } else {
        dataset.setNewDistanceMetric(distMethod);
    }

    return s.str();
}

/**
 * Parse input string to distance method, and k.
 * Then set new k and distance values.
 * @param inputMsg input string from client
 * @return string representation of classification of the input vector parsed from inputMsg
 */
string KNN_Client_Handler::parseK(const string &inputMsg) {
    const static regex kPat {"(^\\s*\\d+\\s)"};

    // parse k value from message
    string kValStr = parseFromBuff(inputMsg, kPat);

    if (!kValStr.empty()) return kValStr;

    return {};
}

/**
 * Parse input string to distance method, and k.
 * Then set new k and distance values.
 * @param inputMsg input string from client
 * @return string representation of classification of the input vector parsed from inputMsg
 */
string KNN_Client_Handler::parseDistanceMetric(const string &inputMsg) {
    const static regex validateDistancePat {"\\s(AUC|MAN|CHB|CAN|MIN)\\s*$"};
    const static regex distancePat {"(AUC|MAN|CHB|CAN|MIN)"};


    // parse distance method from message
    string distMethod = parseFromBuff(inputMsg, validateDistancePat);

    if (distMethod.empty()) return {};

    // get rid of spaces
    distMethod = parseFromBuff(distMethod, distancePat);

    return distMethod;
}

/**
 * Parse input string to distance method, and k.
 * Then set new k and distance values.
 * @param inputMsg input string from client
 * @return string representation of classification of the input vector parsed from inputMsg
 */
void KNN_Client_Handler::setValues(const string &kValStr, const string &distMethod) {
    int kVal;

    try {
        kVal = stoi(kValStr); // this should work since we already made sure that input is valid
    } catch (exception &err) {
        throw invalid_argument("invalid value for K");
    }

    dataset.setK(kVal);

    dataset.setNewDistanceMetric(distMethod);
}

/**
 * Parse the given regex pattern string from the whole input buffer
 * @param buff buffer
 * @param pat regex pattern
 * @return string matching the pattern
 */
string KNN_Client_Handler::parseFromBuff(string buff, const regex& pat) {

    smatch matches;

    if (regex_search(buff, matches, pat)) {
        return matches[0];
    }
    else {
        return {};
    }
}

// add dataset to server
void KNN_Client_Handler::addDataset(string filename) {
    dataset = KNN_Set::createSetBasic(filename);
}

// add test dataset to server
void KNN_Client_Handler::addUnclassifiedSet(string filename) {
    // read file and return vector of vectors.
    unclassified = KNN_Set::createSetBasicWithTest(filename);
}

void KNN_Client_Handler::operateOnInput(std::string input, DefaultIO& dio) {

    // if input = "/n" exit
    // else:
    //  if valid input: set K and DistMethod
    //  else print error

    if (input.empty()) {
        return;
    } else {

        string kValStr = parseK(input);
        string distMethodStr = parseDistanceMetric(input);

        if (kValStr.empty()) dio.write("invalid value for K");

        if (distMethodStr.empty()) dio.write("invalid value for metric");

        if (!kValStr.empty() && !distMethodStr.empty())
            setValues(kValStr, distMethodStr);
    }
}