#include "CLI.h"

using std::stringstream;

CLI::CLI(KNN_ClientServer &clientServer, DefaultIO &dio) : dio(dio), commands(clientServer, dio, noExit),
                                                           noExit(true){}
void CLI::start() {
    string choice;
    map<int, Command&> commandMap = commands.command_map;
    while (noExit) {
        printMenu();

        // get choice as int from client/user
        int cmd_num = getChoice();

        // -1 to identify an invalid input
        if (cmd_num == -1) {
            dio.write("invalid input");
            continue;
        }

        // set command to given choice
        Command& cmd = commands.command_map.at(cmd_num);

        // execute command
        cmd.execute();
    }
}

void CLI::printMenu() {
    dio.write("Welcome to the KNN Classifier Server. Please choose an option:");
    for (auto cmd = commands.command_map.cbegin(); cmd != commands.command_map.cend(); cmd++) {
        stringstream s;
        s << cmd->first << ". " << cmd->second.getDescription();
        dio.write(s.str());
    }
}

int CLI::getChoice() {
    string choice = dio.read();
    map<int, Command&> commandMap = commands.command_map;

    // validate input. can also be done using regex (remove conditions)
    if (choice.size() == 1 && isdigit(choice.at(0))) {
        int cmd_num = stoi(choice);
        if (commandMap.find(cmd_num) != commandMap.end())
            return cmd_num; // valid input
    }

    return -1; // invalid input
}

Commands::Commands(KNN_ClientServer& clientServer, DefaultIO& dio, bool& noExit)
        : uploadCsv(clientServer, dio), algoSettings(clientServer, dio),
          classifyData(clientServer, dio), displayResults(clientServer, dio),
          downloadResults(clientServer, dio), _exit(clientServer, dio, noExit),
          command_map{{1,uploadCsv},{2,algoSettings},{3,classifyData},{4,displayResults},
                      {5,downloadResults},{8, _exit}}{}

Command &Commands::at(int key) {
    return command_map.at(key);
}