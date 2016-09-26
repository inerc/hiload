#include <QCoreApplication>
#include "server.h"
#include <iostream>
#include <unistd.h>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    if (argc < 7){
        std::cout << "No parametr";
        return 1;
    }
    std::string root_dir;
    int param, port, workers;
    while ((param = getopt(argc, argv, "p:d:t:")) != -1) {
        switch (param) {
                        case 'p':
                            port= std::atoi(optarg);
                            break;
                        case 'd':
                            root_dir  = optarg;
                            break;
                        case 't':
                            workers =std::atoi(optarg);
                            break;
                        default:
                            throw std::invalid_argument("Unsupported flag");
                    }
    }

    Server server(port, root_dir, workers);
    server.listen();

    return a.exec();
}
