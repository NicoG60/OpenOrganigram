#include "ConnexionHTTP.h"

ConnexionHTTP::ConnexionHTTP(QTcpSocket *socket, QObject *parent) :
	QObject(parent),
	socket(socket)
{
	connect(socket, SIGNAL(readyRead()), this, SLOT(traitement()));
}

void ConnexionHTTP::traitement()
{
	qDebug() << socket->bytesAvailable();
	QDataStream in(socket);
	in.setVersion(QDataStream::Qt_4_0);

	QString donnees;
	in >> donnees;

	qDebug() << donnees;
}
