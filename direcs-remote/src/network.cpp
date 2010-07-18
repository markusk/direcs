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

#include "network.h"


Network::Network()
{
	http = new QHttp(this);

	connect(http, SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestFinished(int, bool)));
	connect(http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)), this, SLOT(readResponseHeader(const QHttpResponseHeader &)));
	#ifndef QT_NO_OPENSSL
	connect(http, SIGNAL(sslErrors(const QList<QSslError> &)), this, SLOT(sslErrors(const QList<QSslError> &)));
	#endif
	connect(http, SIGNAL( readyRead(const QHttpResponseHeader &) ), this, SLOT( takeData() ));

	// the camera image
	image = new QImage();


	// start 'download'
	//downloadFile();
}


Network::~Network()
{
	cancelDownload();
	delete image;
}


void Network::downloadFile()
{
	QUrl url( addressToOpen );


	// https mode or not
	QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;

	// set the HTTP server
	http->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());

	if (!url.userName().isEmpty())
		http->setUser(url.userName(), url.password());

	httpRequestAborted = false;

	QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");

	if (path.isEmpty())
		path = "/";


	//------------------------------------------------------------------------------
	// no file given as second parameter, so we get a readyRead Signal,
	// every time new data is available!
	//------------------------------------------------------------------------------
	httpGetId = http->get(path);
}


void Network::cancelDownload()
{
	qDebug("Download canceled.");

	httpRequestAborted = true;
	http->abort();
}


void Network::httpRequestFinished(int requestId, bool error)
{
	if (requestId != httpGetId)
		return;

	if (httpRequestAborted)
	{
		return;
	}

	if (requestId != httpGetId)
		return;


	if (error)
	{
		//QMessageBox::information(this, tr("HTTP"), tr("Download failed: %1.").arg(http->errorString()));
		qDebug("Download failed.");
	}
	else
	{
		//statusLabel->setText(tr("Downloaded %1 to current directory.").arg(fileName));
		qDebug("File downloaded.");
	}
}


void Network::readResponseHeader(const QHttpResponseHeader &responseHeader)
{
	switch (responseHeader.statusCode()) {
	case 200:                   // Ok
	case 301:                   // Moved Permanently
	case 302:                   // Found
	case 303:                   // See Other
	case 307:                   // Temporary Redirect
		// these are not error conditions
		break;
	default:
		//QMessageBox::information(this, tr("HTTP"), tr("Download failed: %1.").arg(responseHeader.reasonPhrase()));
		qDebug("Download failed.");
		httpRequestAborted = true;
		http->abort();
	}
}


#ifndef QT_NO_OPENSSL
void Network::sslErrors(const QList<QSslError> &errors)
{
	QString errorString;
	foreach (const QSslError &error, errors)
	{
		if (!errorString.isEmpty())
			errorString += ", ";
		errorString += error.errorString();
	}

	qDebug("One or more SSL errors has occurred.");
	/*
	if (QMessageBox::warning(this, tr("HTTP Example"), tr("One or more SSL errors has occurred: %1").arg(errorString), QMessageBox::Ignore | QMessageBox::Abort) == QMessageBox::Ignore)
	{
		*/
		http->ignoreSslErrors();
	//}
}
#endif


void Network::takeData()
{
	// read ALL available data
	data = http->readAll();


	// start of new frame
	if (data.startsWith("--BoundaryString"))
	{
		/*
		From the original 'motion' source code

		// the following string has an extra 16 chars at end for length
		const char jpeghead[] = "--BoundaryString\r\n"								// 18
								"Content-type: image/jpeg\r\n"						// 26
								"Content-Length:                ";					// 31
		int headlength = sizeof(jpeghead) - 1;    // don't include terminator		//
																					//-----
																					// 75
																					//=====

		http://www.lavrsen.dk/twiki/bin/view/Motion/WebHome
		*/

		// Sow we remove the first 75 chars!
		data.remove(0, 75);

		// put data to a QImage
		image->loadFromData(data);

		// emit image to the GUI
		emit( dataComplete(image) );
	}
}


void Network::setUrl(QString url)
{
	addressToOpen = url;
	downloadFile();
}
