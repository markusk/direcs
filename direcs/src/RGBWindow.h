/***************************************************************************
 *  This file is part of the OpenKinect Project. http://www.openkinect.org *
 *                                                                         *
 *  Copyright (c) 2010 individual OpenKinect contributors.                 *
 *  See the CONTRIB file for details.                                      *
 *                                                                         *
 *  This file is part of direcs.                                           *
 *  www.direcs.de                                                          *
 *                                                                         *
 *  direcs is free software: you can redistribute it and/or modify it      *
 *  under the terms of the GNU General Public License as published         *
 *  by the Free Software Foundation, version 3 of the License.             *
 *                                                                         *
 *  direcs is distributed in the hope that it will be useful,              *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with direcs. If not, see <http://www.gnu.org/licenses/>.         *
 *                                                                         *
 ***************************************************************************/

#ifndef __RGB_WINDOW_H__
#define __RGB_WINDOW_H__

#include <QtOpenGL>
#include <QTime>
#include "QKinect.h"
#include <QTime>

/// @file RGBWindow.h
/// @brief a basic Qt GL window class for ngl demos
/// @author Jonathan Macey
/// @version 1.0
/// @date 10/10/10
/// Revision History :
/// Initial Version 10/10/10 (Binary day ;-0 )
/// @class RGBWindow
/// @brief our main glwindow widget for  all drawing elements are
/// put in this file


class RGBWindow : public QGLWidget
{
Q_OBJECT        // must include this if you use Qt signals/slots

public :
	/// @brief Constructor for RGBWindow
	/// @param [in] _parent the parent window to create the GL context in
	RGBWindow(QWidget *_parent);

	~RGBWindow();

	/// @brief a method to set the draw mode (RGB or Depth)
	/// @param [in] _m the mode to use
	inline void setMode(int _m){m_mode=_m;}


private :
	/// @brief the image data to draw put into a GL texture
	std::vector<uint8_t> m_rgb;

	/// @brief the texture object pointer
	GLuint m_rgbTexture;

	/// @brief the draw mode we are using
	int m_mode;


protected:
  /// @brief  The following methods must be implimented in the sub class
  /// this is called when the window is created
  /// \note these are part of the Qt API so can't be changed to the coding standard
  /// so it can't be called initializeGL )
  void initializeGL();

  /// @brief this is called whenever the window is re-sized
  /// @param[in] _w the width of the resized window
  /// @param[in] _h the height of the resized window
  /// \note these are part of the Qt API so can't be changed to the coding standard
  /// so it can't be called resizeGL )
  void resizeGL(const int _w, const int _h);

  /// @brief this is the main gl drawing routine which is called whenever the window needs to
  // be re-drawn
  void paintGL();


private :
  /// @brief this method is called every time a mouse is moved
  /// @param _event the Qt Event structure
  /// \note these are part of the Qt API so can't be changed to the coding standard
  /// so it can't be called MouseMoveEvent )
  void mouseMoveEvent (QMouseEvent * _event);

  /// @brief this method is called everytime the mouse button is pressed
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  /// \note these are part of the Qt API so can't be changed to the coding standard
  /// so it can't be called MousePressEvent )
  void mousePressEvent (QMouseEvent *_event);

  /// @brief this method is called everytime the mouse button is released
  /// inherited from QObject and overridden here.
  /// @param _event the Qt Event structure
  /// \note these are part of the Qt API so can't be changed to the coding standard
  /// so it can't be called MousePressEvent )
  void mouseReleaseEvent (QMouseEvent *_event);

  /// @brief called when the timer is triggered
  void timerEvent(QTimerEvent *_event);
};

#endif
