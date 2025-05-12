#include "KNN_Set.h"

/**
 * Constructor for KNN_Set without a given number of neighbors,
 * size 0 for vectors in file, and without any given distance method.
 */
KNN_Set::KNN_Set() : kVal(5), sizeOfVectors(0), distanceFinder("AUC") {

}

/**
 * Constructor for KNN_Set with k neighbors, size 0 for vectors in file, and the given distance method.
 * @param k number of neighbors
 * @param distMethod distance method
 */
KNN_Set::KNN_Set(int k, string distMethod) : kVal(k), sizeOfVectors(0), distanceFinder(distMethod) {
}

// get k
int KNN_Set::getK() {
    return kVal;
}

// set k
void KNN_Set::setK(int val) {
    kVal = val;
}

// get distanceMetric
string KNN_Set::getDistanceMethodName() {
    return distanceFinder.getDistanceMetricName();
}

VectorDist KNN_Set::getDistanceMethod() {
    return distanceFinder;
}

void KNN_Set::setNewDistanceMetric(std::string distMethod) {
    distanceFinder.setDistMethod(distMethod);
}

/**
 * This method returns a new KNN_Set with the given arguments and vectors read from the given file
 * @param argc argument count
 * @param argv array of program arguments as c-strings
 * @return new KNN_Set object with given arguments and vectors read from the given file
 */
KNN_Set KNN_Set::createSet(int argc, const char *argv[]) {
    int k;
    string fileName, distanceMethod;

    // checks that args fit program specifications and sets k, fileName, & distanceMethod accordingly
    validateArgs(argc, argv, k, fileName, distanceMethod);

    // constructs a new empty dataset with the given k value and distance method parameters
    KNN_Set newSet = KNN_Set(k, distanceMethod);

    // reads vectors from the given csv file and adds them to the dataset
    newSet.readFromCsv(fileName);

    return newSet;

}

/**
 * Similar to the createSet, this method returns a new KNN_Set.
 * The vectors are read from the given file
 * @param filename database file
 * @return new KNN_Set object - set of k nearest neighbors
 */
KNN_Set KNN_Set::createSetBasic(string filename) {
    KNN_Set newSet = KNN_Set();

    newSet.readFromCsv(filename);

    return newSet;
}

/**
 * Similar to the createSet, this method returns a new KNN_Set.
 * The vectors are read from the given file
 * @param filename database file
 * @return new KNN_Set object - set of k nearest neighbors
 */
vector<vector<double>> KNN_Set::createSetBasicWithTest(string filename) {
    KNN_Set newSet = KNN_Set();

    newSet.readVectorsFromTestCsv(filename);

    return newSet.unclassified;
}

/**
 * Validate all program args
 * @param argc number of args
 * @param argv vector of args
 * @param k reference to save number of neighbors
 * @param fName reference to save file name
 * @param distanceMetric reference to save name of distance metric
 */
void KNN_Set::validateArgs(int argc, const char* argv[], int &k, string &fName, string &distanceMetric) {

    // check if the number of args is correct
    if (argc != EXP_ARGC) {
        cerr << "invalid number of arguments!" << endl;
        exit(1);
    } else {

        // check that given k value is a valid non-negative integer
        k = parseInt(argv[1]);
        fName = argv[2];
        distanceMetric = argv[3];
    }
}

/**
 * This method checks if the given parameter is a non-negative integer
 * @param arg string representing number
 * @return int parsed from arg. note that a negative number won't be returned because of the "-"
 */
int KNN_Set::parseInt(const string& arg) {
    for (char i : arg) {
        if (!isdigit(i)) {
            cerr << "error: " << arg << " is not a valid k-value!" << endl;
            exit(1);
        }
    }
    int parsedNum = stoi(arg);
    if (parsedNum > 0)
        return parsedNum;
    else {
        cerr << "error: given K value " << arg << " is not a positive integer" << endl;
        exit(1);
    }
}

/**
 * Parsed text in the given csv file into ClassifiedVectors and adds them to the database. The
 * data must be formatted as follows: each vector must be a series of double values separated by
 * commas, then a comma and a type name, and finally a line break. The vectors must all be of
 * the same length.
 * @param fName name of csv file
 */
void KNN_Set::readFromCsv(const string& fName) {

    string line;
    bool firstV = true;

    // create file stream to read content of file
    fstream file (fName, ios::in);

    // check if the file name corresponds to a valid file
    if (file.is_open()) {

        // for each line do the following:
        while (getline(file, line)) {

            // create string stream to read content of line
            stringstream str(line);

            // get classified vector from this line
            ClassifiedVector classifiedVector =
                    parseVectorFromString(str, firstV);

            // insert classified vector into the dataset
            this->addVector(classifiedVector);
        }
    }
    // if the file name is invalid, an error is given
    else {
        cerr << "Could not open file" << endl;
        exit(1);
    }
}

/**
 * Each line in the file is an unclassified vector. Each vector must be a series of double values separated by
 * commas, ending with a line break. The vectors must all be of the same length.
 * @param fName name of csv file
 */
void KNN_Set::readVectorsFromTestCsv(const string& fName) {

    string line;

    // create file stream to read content of file
    fstream file (fName, ios::in);

    // check if the file name corresponds to a valid file
    if (file.is_open()) {

        // for each line do the following:
        while (getline(file, line)) {

            // create string stream to read content of line
            stringstream str(line);

            // get classified vector from this line
            vector<double> unclassifiedVector =
                    VectorDist::inputToVector(str.str());

            // insert classified vector into the dataset
            this->addUnclassifiedVector(unclassifiedVector);
        }
    }
        // if the file name is invalid, an error is given
    else {
        cerr << "Could not open file" << endl;
        exit(1);
    }
}

/**
 * This method parses vector's elements and tag from given string, and returns a classified vector
 * containing a vector of doubles and the tag.
 * @param str string containing vector data
 */
ClassifiedVector KNN_Set::parseVectorFromString(stringstream &str, bool &firstV) {
    vector<double> vec;
    string word, typeName;

    // insert each element in str into vector or save as typeName of vector
    while(getline(str, word, ',')) {

        // check if word is a double number, o.w it is the type of the vector
        if (isValidNumber(word)) {

            // compute size of the first vector to check later if the format is valid
            if (firstV)
                sizeOfVectors++;

            vec.push_back(stod(word));
        } else if (vec.size() == sizeOfVectors) {

            // save type of vector
            typeName = word;

            // this is to measure size of valid vector
            firstV = false;

        } else {

            // in case the input file's format is invalid
            const string errMessage{"Invalid format!\n"};
            throw invalid_argument{errMessage};
        }
    }


    // return a new classified vector created using vector and typeName
    return ClassifiedVector(vec, typeName);
}

/**
 * This method adds vector to the dataset and then adds its type to type dictionary if it
 * doesn't exist there yet.
 * @param vec input vector
 */
void KNN_Set::addVector(ClassifiedVector vec) {
    // adds vector to the dataset
    dataset.push_back(vec);

    // adds its type name to the typeNameDict if needed
    addType(vec.getTypeName());
}

/**
 * This method adds vector to the unclassified dataset.
 * @param vec input vector
 */
void KNN_Set::addUnclassifiedVector(vector<double> vec) {
    // adds vector to the dataset
    unclassified.push_back(vec);
}

/**
 * This method checks if the given string is a valid number (double)
 * @param str string
 */
bool KNN_Set::isValidNumber(const string& str) {

    // regex sequence for valid numbers
    const static regex vectorPat {"^(\\d+\\.?|\\d+\\.\\d+|\\.\\d+)(E(\\+|\\-)\\d+)?$"};

    smatch matches;

    // returns true if a valid number was found, otherwise false
    return regex_search(str, matches, vectorPat);
}

/**
 * This method adds a new type to the typeNameDict, does nothing if it already exists there
 * @param typeName type(classification) of vector
 */
void KNN_Set::addType(const string& typeName) {

    /* adds given type name to the typeNameDict with a value of 1 more than the previous.
     * if the type name is already in the dictionary, no action is taken */
    typeNameDict.emplace(typeName, typeNameDict.size());
}

/**
 * This method gets the type of the given vector using the KNN algorithm
 * @param inputVec given vector
 * @return type (classification)
 */
string KNN_Set::getType(vector<double> inputVec, int k) {

    // if vector is not the same size as those in the dataset, throws an exception
    if (inputVec.size() != sizeOfVectors) {
        const string errMsg = "error: input vector must contain " + to_string(sizeOfVectors) + " values";
        throw invalid_argument(errMsg);
    }

    /*vector that each int counts the number of vectors of each type in the k-nearest vectors.
     * each index of typeCounts corresponds to the index value of a typename in typeNameDict */
    vector<int> typeCounts  = kNearestTypeCounts(inputVec, k);

    string typeMost;
    int typeMax = -1;

    // find the most common type (classification)
    for (auto &typeIter : typeNameDict) {

        // index of the current type in the typeNameDict
        int index = typeIter.second;

        // checks if the number of vectors of the current type is largest yet
        if (typeCounts[index] > typeMax) {

            // if so, sets typeMax to the number of occurrences of the current type
            typeMax = typeCounts[index];

            // sets typeMost to the current type
            typeMost = typeIter.first;
        }
    }

    // for some reason typeMost was not assigned
    if (typeMost.length() == 0) {

        // throw exception for invalid return value
        cerr << "type length is 0" << endl;
        exit(1);
    }

    return typeMost;
}

/**
 * A wrapper for getType function without the given distance method.
 * First the method sets the distance metric to be the given distance metric.
 * Then getType is called using the input vector, and k (#neighbors).
 * @param inputVec input vector to be classified
 * @param k number of neighbors
 * @param distMethod distance method/metric
 * @return string representing type(classification) of the input vector.
 */
string KNN_Set::getType(vector<double> inputVec, int k, string distMethod) {
    distanceFinder.setDistMethod(distMethod);
    return getType(inputVec, k);
}

/**
 * This method counts how many times each type of vector appears in the KNN.
 * It returns a vector that each element counts the number of vectors of each type in the k-nearest vectors.
 * @param queryVec input vector
 * @param k number of neighbors
 * @return typeCounts (vector<int>)
 */
vector<int> KNN_Set::kNearestTypeCounts(vector<double> &queryVec, int k) {

    // initialize min heap of distance-typename pairs
    priority_queue<pair<double,string>, vector<pair<double,string>>,
        greater_equal<pair<double,string>>> distanceMinHeap;

    // iterate through all the vectors in the dataset
    for (ClassifiedVector vec : dataset) {

        // get the distance between the current vector and the query vector
        double distance = distanceFinder.getDistance(queryVec, vec.getValues());

        // add the pair of the distance and the type of the current vector to the distanceMinHeap
        distanceMinHeap.push(make_pair(distance, vec.getTypeName()));
    }

    // vector that each int counts the number of vectors of each type in the k-nearest vectors
    vector<int> typeCounts (typeNameDict.size(), 0);

    /* counts the number of occurrences of each type among the k nearest vectors in the dataset (or
     * all if fewer than k exist) */
    for (int i = 0; i < k && !distanceMinHeap.empty(); i++) {

        // get the pair at the top of the heap
        pair<double, string> newPair = distanceMinHeap.top();

        // get the type of the pair
        string type = get<1>(newPair);

        // increment the count of that type
        typeCounts[typeNameDict.at(type)]++;

        // pop the top of the heap
        distanceMinHeap.pop();
    }

    // return vector of the counts
    return typeCounts;
}