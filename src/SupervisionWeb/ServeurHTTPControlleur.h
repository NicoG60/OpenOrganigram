#ifndef SERVEURHTTPCONTROLLEUR_H
#define SERVEURHTTPCONTROLLEUR_H

#include <QObject>
#include <QTcpServer>

#include "ConnexionHTTP.h"

class ServeurHTTPControlleur : public QObject
{
		Q_OBJECT
	public:
		explicit ServeurHTTPControlleur(QObject *parent = 0);

	private:
		QTcpServer* serveur;
		QList<ConnexionHTTP*> connections;

	signals:

	public slots:
		void newConnection();

};

#endif // SERVEURHTTPCONTROLLEUR_H
