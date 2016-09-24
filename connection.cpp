#include "connection.h"

Connection::Connection(QTcpSocket* tcpSocket, const RequestHandler& requestHandler, TPool& tpool,
                       const std::function<void (std::shared_ptr<Connection>)>& abortedCallback)
            : requestHandler(requestHandler), tcpSocket(tcpSocket), threadPool(tpool),
              abortedCallback(abortedCallback) {
    connect(tcpSocket, &QTcpSocket::readyRead, this, &Connection::readyRead);
}

void Connection::read() {
    QByteArray data;

    while(tcpSocket->bytesAvailable()) {
        data.append(tcpSocket->readAll());
    }

    std::string message(QString(data).toStdString());
    requestHandler.handle(message, std::bind(&Connection::write, this, std::placeholders::_1));
    close();
}

void Connection:: write(const std::string& message) {
    tcpSocket->write(message.c_str(), message.size());
    tcpSocket->waitForBytesWritten();
}

void Connection:: close() {
    if(tcpSocket->isOpen())
        tcpSocket->close();
    abortedCallback(shared_from_this());
}

void Connection::readyRead() {
    threadPool.addTask(std::bind(&Connection::read, this));
}
