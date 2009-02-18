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
#include <QFile>
#include <QFileInfo>
#include <QHttp>
#include <QHttpResponseHeader>
#include <QUrl>
#include <QSsl>
#include <QSslError>
#include <QAuthenticator>
#include <QImage>
#include <QDebug>
//-------------------------------------------------------------------

/**
\brief Handles the network access for the webcam.
TODO: to be filled with content.
*/
class Network : public QObject
{
    Q_OBJECT

	public:
		Network();
		~Network();


	signals:
		/**
		Emits a QImage to the GUI, which was received over the network.
		*/
		void dataComplete(QImage* image);
		
		/**
		Sends a string over the network.
		*/
		void sendNetworkString(QString text);
		
		
	public slots:
		/**
		Takes the URL for loading the file.
		*/
		void setUrl(QString url);

	
	private slots:
		void downloadFile();
		void cancelDownload();
		void httpRequestFinished(int requestId, bool error);
		void readResponseHeader(const QHttpResponseHeader &responseHeader);
		void slotAuthenticationRequired(const QString &, quint16, QAuthenticator *);
		#ifndef QT_NO_OPENSSL
		void sslErrors(const QList<QSslError> &errors);
		#endif
		void takeData();
	
 
	private:
		QHttp *http;
		QByteArray data;
		int httpGetId;
		bool httpRequestAborted;
		QString addressToOpen;
		QImage *image;
};

#endif
