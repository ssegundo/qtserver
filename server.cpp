#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Server)
{
    ui->setupUi(this);
    initServer();
    //construyendo los mensajes
    for (int i=1;i<=50 ;i++ ) {
        randomMessages << "Mensaje aleatorio " + QString::number(i);
    }
    //Botón cerrar
    connect(ui->pBtn_Close, &QAbstractButton::clicked, this, &QWidget::close);
    // intento de conexión por el lado del cliente
    connect(serverSocket, &QTcpServer::newConnection, this, &Server::sendRandomMessage);

}

Server::~Server()
{
    delete ui;
}

void Server::initServer()
{
    qDebug() << "initServer() start ... ";
    serverSocket = new QTcpServer(this);
    if(!serverSocket->listen())
    {
        qDebug() << "Server Error: " << serverSocket->errorString();
        return;
    }
    QString port = QString::number(serverSocket->serverPort(),10);
    qDebug() << "port = " << port;
    QString ipAddress = "127.0.0.1";

    //Todas las IP's de mi estación
    QList<QHostAddress> ipAddressList = QNetworkInterface::allAddresses();
    for(int i=0; i < ipAddressList.size(); i++){
        if(ipAddressList.at(i) != QHostAddress::LocalHost && ipAddressList.at(i).toIPv4Address()){
            ipAddress= ipAddressList.at(i).toString();
            qDebug() << "ip: " << ipAddress;
            break;
        }
    }

    QString address = "IP: " + ipAddress + ", puerto: " + port;
    ui->lblAddress->setText(address);
    qDebug() <<"address: " << address;
    qDebug() << "initServer() end ... ";
}

void Server::sendRandomMessage()
{
    qDebug() << "sendRandomMessage() star ...";
    //Seleccionar mensaje
    QString randomMessage = randomMessages[QRandomGenerator::global()->bounded(randomMessages.size())];
    //Pintamos el mensaje en la interfaz
    ui->lblMessage->setText(randomMessage);
    // Formato estándar del dato
    QByteArray qByteArray;
    // Creo el canal para el flujo de datos
    QDataStream qDataStream(&qByteArray, QIODevice::WriteOnly);
    qDataStream << randomMessage;

    //Delegación de la atención al cliente
    QTcpSocket *socket = serverSocket->nextPendingConnection();
    // Liberación del objeto
    connect(socket, &QAbstractSocket::disconnected, socket, &QObject::deleteLater);
    // escritura del socket
    socket->write(qByteArray);
    socket->disconnectFromHost();


    qDebug() << "sendRandomMessage() end ...";
}

