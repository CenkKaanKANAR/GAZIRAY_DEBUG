#include "udphandler.h"
#include <unistd.h>

UdpHandler::UdpHandler(QObject *parent)
    : QObject{parent}, UDPSocket(new QUdpSocket())
{
    connect(UDPSocket, &QUdpSocket::bytesWritten, this, &UdpHandler::bytesSend);
    connect(UDPSocket, &QUdpSocket::readyRead,this, &UdpHandler::readyRead);
}

UdpHandler::~UdpHandler()
{
    delete UDPSocket;
}
/*
bool UdpHandler::initUdpConnection(const QString IpAdress, const uint16_t portAdress)
{

    UDPSocket->connectToHost(QHostAddress(IpAdress), portAdress);


    if (UDPSocket->waitForConnected(3000)) {
        qDebug() << "UDP Socket is Connected";
        return true;
    } else {
        qDebug() << "UDP Network Connection failed";
        emit udpSocketConnectionFailed();
        return false;
    }
}*/

bool UdpHandler::initUdpConnection(const QString IpAdress, const uint16_t portAdress)
{
    if (!UDPSocket->bind(QHostAddress(IpAdress), portAdress)) {
        qDebug() << "UDP Socket bind failed";
        emit udpSocketConnectionFailed();
        return false;
    }
    qDebug() << "UDP Socket bound to port" << portAdress;
    return true;
}


void UdpHandler::disconnectUdpConnection()
{
    UDPSocket->disconnectFromHost();
    UDPSocket->waitForDisconnected(3000);
}

/*
qint64 UdpHandler::sendMessage(QByteArray &sendData, quint64 size)
{



    qDebug() <<  "Mesaj gönderiliyor mu? ";
    qint64 bytesWritten = UDPSocket->write(sendData.data(), size);
    QString hexData;
    for (int i = 0; i < size; ++i) {
        hexData.append(QString("\\x%1").arg(static_cast<unsigned char>(sendData.data()[i]), 0, 16));
    }

    // Yanıt gönder
    QByteArray response = hexData.toUtf8();
    qDebug() <<  "Nedir bu gönderilen mesaj : " << response;
    //QHostAddress sender;
    //quint16 senderPort = 8080;
    UDPSocket->writeDatagram(response, sender, senderPort);

    if(bytesWritten < 0)
    {
        qDebug() <<  "transmissionErrorOccured " + UDPSocket->errorString();
        emit transmissionErrorOccured(QString::number(bytesWritten));
    }
    return bytesWritten;
}*/

qint64 UdpHandler::sendMessage(QByteArray &sendData, quint64 size)
{
    QHostAddress clientAddress("127.0.0.1"); // Client'ın IP adresi
    quint16 clientPort = 8091; // Client'ın dinlediği port

    qint64 bytesWritten = UDPSocket->writeDatagram(sendData, clientAddress, clientPort);
    qDebug() << "Gönderilen data büyüklüğü : " << sendData.size();
    if(bytesWritten < 0) {
        qDebug() << "transmissionErrorOccured " + UDPSocket->errorString();
        emit transmissionErrorOccured(QString::number(bytesWritten));
    }
    return bytesWritten;
}


void UdpHandler::readyRead()
{
    qDebug() << "Burası işliyor mu aga?";
    //QByteArray data = UDPSocket->readAll();
    while (UDPSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(static_cast<int>(UDPSocket->pendingDatagramSize()));


        UDPSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        qDebug() << "OKUNAN DATA BU : " << datagram;

        //handleDatagram(datagram);
        emit receivedUdpMessage(datagram);
    }


}

void UdpHandler::handleDatagram(const QByteArray& datagram) {
    // Gelen paketi işleme kodunuz
    qDebug() << "Received datagram:" << datagram;
    // Her paketi burada ayrı ayrı ele alabilirsiniz
}
void UdpHandler::bytesSend(qint64 size)
{

    emit udpBytesWritten(size);

}
