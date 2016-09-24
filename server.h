#ifndef SERVER_H
#define SERVER_H

#include <string>
#include "tpool.h"
#include <QTcpServer>
#include <set>
#include "connection.h"

class Server : public QObject {
    Q_OBJECT

    int mPort;
    RequestHandler reqHandler;
    TPool thread_pool;
    QTcpServer tcpServer;
    std::set <ConnectionPtr> connected;
    std::mutex serverMutex;

private slots:
    void newConnection();

public:
    void listen();
    Server(int port, const std::string &directory,
           size_t workers);
};

#endif // SERVER_H
