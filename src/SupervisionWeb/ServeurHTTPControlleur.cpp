#include "ServeurHTTPControlleur.h"

ServeurHTTPControlleur::ServeurHTTPControlleur(QObject *parent) :
	QObject(parent),
	serveur(new QTcpServer)
{
	connect(serveur, SIGNAL(newConnection()), this, SLOT(newConnection()));
	serveur->listen(QHostAddress::Any, 8080);
	qDebug() << "En attente de connexions...";
}

void ServeurHTTPControlleur::newConnection()
{
	QTcpSocket* socket = serveur->nextPendingConnection();

	connections.append(new ConnexionHTTP(socket));
	qDebug() << serveur->isListening();
}
