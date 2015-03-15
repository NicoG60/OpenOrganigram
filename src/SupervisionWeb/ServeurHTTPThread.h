#ifndef SERVEURHTTPTHREAD_H
#define SERVEURHTTPTHREAD_H

#include <QThread>
#include <QTcpServer>

#include "ServeurHTTPControlleur.h"

class ServeurHTTPThread : public QThread
{
		Q_OBJECT
	public:
		explicit ServeurHTTPThread(QObject *parent = 0);

	private:
		ServeurHTTPControlleur* controleur;

	protected:
		void run() Q_DECL_OVERRIDE;

	signals:

	public slots:

};

#endif // SERVEURHTTPTHREAD_H
