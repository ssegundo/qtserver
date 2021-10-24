#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Server)
{
    ui->setupUi(this);
}

Server::~Server()
{
    delete ui;
}

void Server::initServer()
{
    qDebug() << "initServer() start ... ";
}

void Server::sendRandomMessage()
{

}

