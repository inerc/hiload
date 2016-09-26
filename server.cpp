#include "server.h"

Server::Server( int port, const std::string &directory,
               size_t workers)
    : QObject(nullptr), mPort(port), reqHandler(directory), thread_pool(workers) {
    connect(&tcpServer, &QTcpServer::newConnection, this, &Server::newConnection);
}

void Server::listen() {
    //forever {
        tcpServer.listen(QHostAddress::Any, mPort);
    //}
}

void Server::newConnection() {
    QTcpSocket* client = tcpServer.nextPendingConnection();

    ConnectionPtr connection = std::make_shared<Connection>(client, reqHandler, thread_pool,
                                                            [this](ConnectionPtr ptr) {
                                                                std::unique_lock <std::mutex> lock(serverMutex);
                                                                connected.erase(ptr);
                                                            } );
    connect(connection.get(), &Connection::moveToThisThread, this, &Server::moveToThisThread);
    connect(this, &Server::movedThread, connection.get(), &Connection::movedThread);

    {
        std::unique_lock <std::mutex> lock(serverMutex);
        connected.insert(connection);
    }
}

void Server::moveToThisThread(QTcpSocket* socket, QThread* thread) {
    socket->setParent(nullptr);
    socket->moveToThread(thread);
    emit movedThread();
}
