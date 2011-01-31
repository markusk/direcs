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

#include "direcsKinect.h"

/*
DirecsKinect::DirecsKinect() : QThread()
{
	stopped = false;
	initDone = false;;
	cameraIsOn = false;
	faceDetectionIsEnabled = false;
	faceDetectionWasActive = false;
	haarClassifierCascadeFilename = "none";


	Mat depthMat(Size(640,480),CV_16UC1);
	Mat depthf  (Size(640,480),CV_8UC1);
	Mat rgbMat(Size(640,480),CV_8UC3,Scalar(0));
	Mat ownMat(Size(640,480),CV_8UC3,Scalar(0));

	for (unsigned int i = 0 ; i < 2048 ; i++)
	{
		float v = i/2048.0;
		v = std::pow(v, 3)* 6;
		m_gamma[i] = v*6*256;
	}


}
*/

DirecsKinect::~DirecsKinect()
{
//	device.stopVideo();
//	device.stopDepth();
}


void DirecsKinect::VideoCallback(void* _rgb, uint32_t timestamp)
{
	//
	// Do not call directly even in child
	//
	std::cout << "RGB callback" << std::endl;
	m_rgb_mutex.lock();
	uint8_t* rgb = static_cast<uint8_t*>(_rgb);
	rgbMat.data = rgb;
	m_new_rgb_frame = true;
	m_rgb_mutex.unlock();
}


void DirecsKinect::DepthCallback(void* _depth, uint32_t timestamp)
{
	//
	// Do not call directly even in child
	//
	std::cout << "Depth callback" << std::endl;
	m_depth_mutex.lock();
	uint16_t* depth = static_cast<uint16_t*>(_depth);
	depthMat.data = (uchar*) depth;
	m_new_depth_frame = true;
	m_depth_mutex.unlock();
}


bool DirecsKinect::getVideo(Mat& output)
{
	m_rgb_mutex.lock();
	if(m_new_rgb_frame)
	{
		cv::cvtColor(rgbMat, output, CV_RGB2BGR);
		m_new_rgb_frame = false;
		m_rgb_mutex.unlock();
		return true;
	}
	else
	{
		m_rgb_mutex.unlock();
		return false;
	}
}


bool DirecsKinect::getDepth(Mat& output)
{
	m_depth_mutex.lock();
	if(m_new_depth_frame)
	{
		depthMat.copyTo(output);
		m_new_depth_frame = false;
		m_depth_mutex.unlock();
		return true;
	}
	else
	{
		m_depth_mutex.unlock();
		return false;
	}
}


bool DirecsKinect::isConnected(void)
{
	return cameraIsOn;
}


void DirecsKinect::enableFaceDetection(int state)
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


void DirecsKinect::setCascadePath(QString haarClassifierCascade)
{
	if (haarClassifierCascade == "none")
	{
		faceDetectionIsEnabled = false;
		// disable checkBoxes in the GUI
		emit disableFaceDetection();
	}


	haarClassifierCascadeFilename = haarClassifierCascade;
}


bool DirecsKinect::init()
{
	if (initDone == false)
	{
		// do only *one* init!
		initDone = true;

		if ( 1 )
		{
			emit message(QString("<font color=\"#FF0000\">ERROR: could not initialize capturing from /dev/video%1.</font>").arg( 0 ));

			cameraIsOn = false;

			return false;
		}
	}

		cameraIsOn = true;

	return true;
}
