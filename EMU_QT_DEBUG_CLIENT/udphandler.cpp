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
}
*/
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
    qDebug() <<  "Gönderilen Data : " << sendData.data();
    qint64 bytesWritten = UDPSocket->write(sendData.data(), size);
    if(bytesWritten < 0)
    {
        qDebug() <<  "transmissionErrorOccured " + UDPSocket->errorString();;
        emit transmissionErrorOccured(QString::number(bytesWritten));
    }
    return bytesWritten;
}*/
qint64 UdpHandler::sendMessage(QByteArray &sendData, quint64 size)
{
    QHostAddress serverAddress("127.0.0.1"); // Server'ın IP adresi
    quint16 serverPort = 8090;               // Server'ın dinlediği port

    qint64 bytesWritten = UDPSocket->writeDatagram(sendData, serverAddress, serverPort);
    if (bytesWritten < 0) {
        qDebug() << "transmissionErrorOccured: " << UDPSocket->errorString();
        emit transmissionErrorOccured(QString::number(bytesWritten));
    }
    return bytesWritten;
}


void UdpHandler::readyRead()
{
    while (UDPSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        //datagram.resize(static_cast<int>(UDPSocket->pendingDatagramSize()));
        datagram.resize(UDPSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        qint64 bytesRead = UDPSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        if (bytesRead == -1) {
            qDebug() << "Error reading datagram: " << UDPSocket->errorString();
            return;
        }

        //qDebug() << "Received data:" << datagram << "from" << sender.toString() << ":" << senderPort;

        QString hexData;
        for (int i = 0; i < datagram.size(); ++i) {
            hexData.append(QString("\\x%1").arg(static_cast<unsigned char>(datagram[i]), 0, 16));
        }
        qDebug() << "Received data:" << hexData << "from" << sender.toString() << ":" << senderPort;

        emit receivedUdpMessage(datagram);
    }
}

/*void MainWindow::processPendingDatagrams()
{
    udpSocket->hasPendingDatagrams();
    QByteArray datagram;
    datagram.resize(udpSocket->pendingDatagramSize());

    // Veriyi al
    udpSocket->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);

    qDebug() << "Received data:" << datagram << "from" << senderAddress << ":" << senderPort;

    ui->lineEdit_ip_address->setText(senderAddress.toString());
    ui->lineEdit_port_id->setText(QString::number(senderPort));

    QString hexData;
    for (int i = 0; i < datagram.size(); ++i) {
        hexData.append(QString("\\x%1").arg(static_cast<unsigned char>(datagram[i]), 0, 16));
    }
    ui->textEdit->append(hexData);

    // Yanıt gönder
    QByteArray response = hexData.toUtf8();

    udpSocket->writeDatagram(response, senderAddress, senderPort);


}*/

void UdpHandler::handleDatagram(const QByteArray& datagram) {
    // Gelen paketi işleme kodunuz
    qDebug() << "Received datagram:" << datagram;
    // Her paketi burada ayrı ayrı ele alabilirsiniz
}
void UdpHandler::bytesSend(qint64 size)
{

    emit udpBytesWritten(size);

}
