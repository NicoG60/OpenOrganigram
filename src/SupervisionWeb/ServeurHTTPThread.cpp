#include "ServeurHTTPThread.h"

ServeurHTTPThread::ServeurHTTPThread(QObject *parent) :
	QThread(parent)
{
}

void ServeurHTTPThread::run()
{
	qDebug() << "Lancement du serveur Web.";
	controleur = new ServeurHTTPControlleur();
	exec();
}
