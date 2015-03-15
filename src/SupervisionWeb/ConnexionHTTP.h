#ifndef CONNEXIONHTTP_H
#define CONNEXIONHTTP_H

#include <QObject>
#include <QTcpSocket>

class ConnexionHTTP : public QObject
{
		Q_OBJECT
	public:
		explicit ConnexionHTTP(QTcpSocket* socket, QObject *parent = 0);

	private:
		QTcpSocket* socket;

	signals:

	public slots:
		void traitement();

};

#endif // CONNEXIONHTTP_H
