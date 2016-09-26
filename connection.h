#ifndef CONNECTION_H
#define CONNECTION_H

#include <functional>
#include <memory>
#include <QTcpSocket>
#include "requesthandler.h"
#include "tpool.h"

class Connection : public QObject, public std::enable_shared_from_this <Connection>
{
    Q_OBJECT

    RequestHandler requestHandler;
    QTcpSocket* tcpSocket;
    TPool& threadPool;
    std::function<void(std::shared_ptr<Connection>)> abortedCallback;

private slots:
    void readyRead();

signals:
    void moveToThisThread(QTcpSocket* socket, QThread* thread);

public:
    void read();
    void write(const std:: string& message);
    void close();

    Connection(QTcpSocket* tcpSocket, const RequestHandler& requestHandler, TPool& tpool,
      const std::function<void (std::shared_ptr<Connection>)>& abortedCallback);

public slots:
    void movedThread();
};

typedef std::shared_ptr<Connection> ConnectionPtr;

#endif // CONNECTION_H
