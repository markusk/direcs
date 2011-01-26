/*************************************************************************
 *   Copyright (C) 2011 by Markus Knapp                                  *
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

#include "camThread.h"


CamThread::CamThread() : QThread()
{
	stopped = false;
	initDone = false;;
	cameraIsOn = false;
	faceDetectionIsEnabled = false;
	faceDetectionWasActive = false;
	haarClassifierCascadeFilename = "none";
}


CamThread::~CamThread()
{
	stopped = true;
}


void CamThread::stop()
{
	stopped = true;
}

// Do not call directly even in child
void CamThread::VideoCallback(void* _rgb, uint32_t timestamp)
{
	std::cout << "RGB callback" << std::endl;
	m_rgb_mutex.lock();
	uint8_t* rgb = static_cast<uint8_t*>(_rgb);
	rgbMat.data = rgb;
	m_new_rgb_frame = true;
	m_rgb_mutex.unlock();
};


void CamThread::run()
{

	if (initDone==false)
	{
		// first load haar classifier cascade etc. ...
		if ( init() == true )
		{
			stopped = true;
		}
	}


	//  start "threading"...
	while (!stopped)
	{
		if (cameraIsOn == true)
		{

			// let the thread sleep some time
			msleep(THREADSLEEPTIME);

		} // cameraIsOn
	} // while !stopped


	stopped = false;
}


bool CamThread::isConnected(void)
{
	return cameraIsOn;
}


void CamThread::enableFaceDetection(int state)
{
	if (state == Qt::Checked)
	{
		faceDetectionIsEnabled = true;
		faceDetectionWasActive = true;
	}
	else
	{
		faceDetectionIsEnabled = false;
	}
}


void CamThread::setCascadePath(QString haarClassifierCascade)
{
	if (haarClassifierCascade == "none")
	{
		faceDetectionIsEnabled = false;
		// disable checkBoxes in the GUI
		emit disableFaceDetection();
	}


	haarClassifierCascadeFilename = haarClassifierCascade;
}


bool CamThread::init()
{
	if (initDone == false)
	{
		// do only *one* init!
		initDone = true;

		if ( 1 )
		{
			emit message(QString("<font color=\"#FF0000\">ERROR: could not initialize capturing from /dev/video%1.</font>").arg( 0 ));

			cameraIsOn = false;
			stopped = true;

			return false;
		}
	}

		cameraIsOn = true;

	return true;
}
