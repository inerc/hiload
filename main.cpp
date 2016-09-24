#include <QCoreApplication>
#include "server.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    Server server(8080, "/home/inerc/web/", 10);
    server.listen();

    return a.exec();
}
