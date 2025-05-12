cmake_minimum_required(VERSION 3.23)
project(ex4)

set(CMAKE_CXX_STANDARD 11)

add_executable(server src/main/server_main.cpp
        src/knnset/VectorDist.cpp src/knnset/KNN_Set.cpp src/knnset/ClassifiedVector.cpp
        src/knnserver/AbstractPeer.cpp src/knnserver/Client.cpp src/knnserver/KNN_ClientServer.cpp
        src/knnserver/KNN_MainServer.cpp src/cli/Command.cpp src/cli/DefaultIO.cpp src/cli/DisplayResults.cpp
        src/cli/DownloadResults.cpp src/cli/Exit.cpp src/cli/StandardIO.cpp src/cli/UploadCSV.cpp src/cli/CLI.cpp
        src/cli/ClassifyData.cpp src/cli/AlgoSettings.cpp)

add_executable(client src/main/client_main.cpp
        src/knnset/VectorDist.cpp src/knnset/KNN_Set.cpp src/knnset/ClassifiedVector.cpp
        src/knnserver/AbstractPeer.cpp src/knnserver/Client.cpp src/knnserver/KNN_ClientServer.cpp
        src/knnserver/KNN_MainServer.cpp src/cli/Command.cpp src/cli/DefaultIO.cpp src/cli/DisplayResults.cpp
        src/cli/DownloadResults.cpp src/cli/Exit.cpp src/cli/StandardIO.cpp src/cli/UploadCSV.cpp src/cli/CLI.cpp
        src/cli/ClassifyData.cpp src/cli/AlgoSettings.cpp)

#add_executable(server src/main/server_main.cpp)
#add_executable(client src/main/client_main.cpp)
#add_library(MyLib src/knnset/VectorDist.cpp src/knnset/KNN_Set.cpp src/knnset/ClassifiedVector.cpp
#        src/knnserver/AbstractPeer.cpp src/knnserver/Client.cpp src/knnserver/KNN_ClientServer.cpp
#        src/knnserver/KNN_MainServer.cpp src/cli/Command.cpp src/cli/DefaultIO.cpp src/cli/DisplayResults.cpp
#        src/cli/DownloadResults.cpp src/cli/Exit.cpp src/cli/StandardIO.cpp src/cli/UploadCSV.cpp src/cli/CLI.cpp
#        src/cli/ClassifyData.cpp src/cli/AlgoSettings.cpp)
#
#target_link_libraries(server MyLib)
#target_link_libraries(client MyLib)
