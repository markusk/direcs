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
	stopped = false;
    http = new QHttp(this);

	connect(http, SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestFinished(int, bool)));
	connect(http, SIGNAL(dataReadProgress(int, int)), this, SLOT(updateDataReadProgress(int, int)));
	connect(http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)), this, SLOT(readResponseHeader(const QHttpResponseHeader &)));
	connect(http, SIGNAL(authenticationRequired(const QString &, quint16, QAuthenticator *)), this, SLOT(slotAuthenticationRequired(const QString &, quint16, QAuthenticator *)));
#ifndef QT_NO_OPENSSL
	connect(http, SIGNAL(sslErrors(const QList<QSslError> &)), this, SLOT(sslErrors(const QList<QSslError> &)));
#endif
	
	connect(http, SIGNAL( readyRead(const QHttpResponseHeader &) ), this, SLOT( takeData() ));
	//connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelDownload()));
	//connect(downloadButton, SIGNAL(clicked()), this, SLOT(downloadFile()));
	
	//
	// start download !!
	//
	downloadFile();
}


Network::~Network()
{
	cancelDownload();
}


void Network::stop()
{
	stopped = true;
	cancelDownload();
}


void Network::run()
{
	//
	//  start "ing"...
	//
	while (!stopped)
	{
		// let the  sleep some time
		// for having more time for the other s
		msleep(SLEEPTIME);
		
		// TODO: do stuff here
	}
	stopped = false;
}


void Network::downloadFile()
{
#ifndef QT_NO_OPENSSL
	//filename = "http://www.familie-knapp.de/hello.txt";
	filename = "http://localhost:8080";
#else
	filename = "https://localhost:8080";
#endif
	
	QUrl url( filename );
	QFileInfo fileInfo(url.path());
	QString fileName = fileInfo.fileName();
	if (fileName.isEmpty())
		fileName = "pic.jpg"; // < < < <

	if (QFile::exists(fileName))
	{
		//qDebug("There already exists a file called %1 in the current directory");
		//QFile::remove(fileName);
	}

	file = new QFile(fileName);
	if (!file->open(QIODevice::WriteOnly))
	{
		qDebug("Unable to save the file %1: %2.");
		//.arg(fileName).arg(file->errorString()));
		delete file;
		file = 0;
		return;
	}

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
	// get the file
	//------------------------------------------------------------------------------
	//httpGetId = http->get(path, file);
	
	// no file given, so we get a readyRead Signal, every time new data is available!
	httpGetId = http->get(path);

	//progressDialog->setLabelText(tr("Downloading %1.").arg(fileName));
	qDebug("Downloading....");
}


void Network::cancelDownload()
{
	//statusLabel->setText(tr("Download canceled."));
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
		if (file)
		{
			file->close();
			file->remove();
			delete file;
			file = 0;
		}

		//progressDialog->hide();
		return;
	}

	if (requestId != httpGetId)
		return;

	//progressDialog->hide();
	file->close();

	if (error)
	{
		file->remove();
		//QMessageBox::information(this, tr("HTTP"), tr("Download failed: %1.").arg(http->errorString()));
		qDebug("Download failed: %1.");
	}
	else
	{
		QString fileName = QFileInfo(QUrl( filename ).path()).fileName();
		//statusLabel->setText(tr("Downloaded %1 to current directory.").arg(fileName));
		qDebug("File downloaded.");
	}

	//downloadButton->setEnabled(true);
	delete file;
	file = 0;
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
		qDebug("Download failed: %1.");
		httpRequestAborted = true;
		//progressDialog->hide();
		http->abort();
	}
}


void Network::updateDataReadProgress(int bytesRead, int totalBytes)
{
	if (httpRequestAborted)
		return;

	//qDebug("bytesRead: %d / totalBytes", bytesRead, totalBytes);
	// TODO: read until 'Content-length'
	
	//progressDialog->setMaximum(totalBytes);
	//progressDialog->setValue(bytesRead);
}


void Network::slotAuthenticationRequired(const QString &hostName, quint16, QAuthenticator *authenticator)
{
	/*
	QDialog dlg;
	Ui::Dialog ui;
	ui.setupUi(&dlg);
	dlg.adjustSize();
	ui.siteDescription->setText(tr("%1 at %2").arg(authenticator->realm()).arg(hostName));

	if (dlg.exec() == QDialog::Accepted) {
		authenticator->setUser(ui.userEdit->text());
		authenticator->setPassword(ui.passwordEdit->text());
	}
	*/
}


#ifndef QT_NO_OPENSSL
void Network::sslErrors(const QList<QSslError> &errors)
{
	QString errorString;
	foreach (const QSslError &error, errors) {
		if (!errorString.isEmpty())
			errorString += ", ";
		errorString += error.errorString();
	}

	qDebug("One or more SSL errors has occurred: %1");
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
		/* From 'motion':
		// the following string has an extra 16 chars at end for length
		const char jpeghead[] = "--BoundaryString\r\n"								// 18
								"Content-type: image/jpeg\r\n"						// 26
								"Content-Length:                ";					// 31
		int headlength = sizeof(jpeghead) - 1;    // don't include terminator		//
																					//-----
																					// 75
																					//=====
		
	*/
		// remove the first 75 chars!
		data.remove(0, 75);
		
		/*
		// -- - - - -- - - - - - - - ---  -- -  - -  - - -
		file = new QFile("cam.jpg");
		if (!file->open(QIODevice::WriteOnly))
		{
			qDebug("Unable to save the file %1: %2.");
			//.arg(fileName).arg(file->errorString()));
			delete file;
			file = 0;
			return;
		}
		
		// write data to file
		file->write(data);
		file->close();
		// -- - - - -- - - - - - - - ---  -- -  - -  - - -
		*/
		image = new QImage();
		image->loadFromData(data);
		
		emit( dataComplete(image) );
		
		delete image;
	}
}
