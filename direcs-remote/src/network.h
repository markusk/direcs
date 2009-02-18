/*************************************************************************
 *   Copyright (C) 2009 by Markus Knapp                                  *
 *   www.direcs.de                                                       *
 *                                                                       *
 *   This file is part of direcs.                                        *
 *                                                                       *
 *   direcs is free software: you can redistribute it and/or modify it   *
 *   under the terms of the GNU General Public License as published      *
 *   by the Free Software Foundation, version 3 of the License.          *
 *                                                                       *
 *   direcs is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the        *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with direcs. If not, see <http://www.gnu.org/licenses/>.      *
 *                                                                       *
 *************************************************************************/

#ifndef NETWORK_H
#define NETWORK_H

//-------------------------------------------------------------------
#include <QThread>
#include <QDebug>
//-------------------------------------------------------------------
#include <QFile>
#include <QFileInfo>
#include <QHttp>
#include <QHttpResponseHeader>
#include <QUrl>
#include <QSsl>
#include <QSslError>
#include <QAuthenticator>
#include <QMessageBox> // for qmessagebox
//-------------------------------------------------------------------

/**
\brief Handles the network access for the webcam.
TODO: to be filled with content.
*/
class Network : public QThread
{
    Q_OBJECT

	public:
		Network();
		~Network();
		void stop();
		virtual void run();


	signals:
		/// Emits TODO: text
		void dataComplete(QString text);
		
		/**
		Emits a info message to a slot.
		This slot can be used to display a text on a splash screen, log file, to print it to a console...
		*/
		void message(QString text);
		
		/**
		Sends a string over the network.
		*/
		void sendNetworkString(QString text);

	
	private slots:
		void downloadFile();
		void cancelDownload();
		void httpRequestFinished(int requestId, bool error);
		void readResponseHeader(const QHttpResponseHeader &responseHeader);
		void updateDataReadProgress(int bytesRead, int totalBytes);
		void slotAuthenticationRequired(const QString &, quint16, QAuthenticator *);
	#ifndef QT_NO_OPENSSL
		void sslErrors(const QList<QSslError> &errors);
	#endif
	void takeData();
	
 
	private:
		volatile bool stopped;
		QHttp *http;
		QFile *file;
		QByteArray *data;
		int httpGetId;
		bool httpRequestAborted;
		QString filename;
	
		// Every  sleeps some time, for having a bit more time fo the other s!
		// Time in milliseconds
		static const unsigned long SLEEPTIME = 150; // Default: 150 ms
};

#endif
