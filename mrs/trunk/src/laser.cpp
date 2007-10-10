//
// C++ Implementation: laser
//
// Description: 
//
//
// Author: Markus Knapp <webmaster@direcs.de>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

/*********************************************************
 *
 * This source code is part of the Carnegie Mellon Robot
 * Navigation Toolkit (CARMEN)
 *
 * CARMEN Copyright (c) 2002 Michael Montemerlo, Nicholas
 * Roy, Sebastian Thrun, Dirk Haehnel, Cyrill Stachniss,
 * and Jared Glover
 *
 * CARMEN is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public 
 * License as published by the Free Software Foundation; 
 * either version 2 of the License, or (at your option)
 * any later version.
 *
 * CARMEN is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied 
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more 
 * details.
 *
 * You should have received a copy of the GNU General 
 * Public License along with CARMEN; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, 
 * Suite 330, Boston, MA  02111-1307 USA
 *
 ********************************************************/
#include "laser.h"

#include <carmen/carmen_graphics.h>
#include <gdk/gdkx.h>
//#include <X11/Xlib.h>
//#include <X11/extensions/shape.h>

#define WINDOWSIZE 500


static carmen_laser_laser_message laser;
static int received_laser = 0;
static int laser_num = 0;
//static GtkWidget *drawing_area;

// laser reichweite
//static double laser_range = 5.0;
static int laser_count = 0;
static double start_time = 0.0;


//static GdkColor gradient[256];


//--------------------------------------
// Daten-Telegramm von Laser empfangen
//--------------------------------------
static void laser_handler(void)
{
	received_laser = 1;
	laser_count++;
/*	
	int numreadings;
	float *laserrange = NULL;
	int i=0;

	//
	// Daten aus aus Laser-Objekt holen < < < <
	//
	numreadings = laser.num_readings;
	laserrange = laser.range;
*/
	/*
	printf("numreadings=%d\n\n", numreadings);
	for(i = 0; i < numreadings; i++)
	{
	
		// x Koordinate der Entfernung
		x = laserrange[i];
		printf("x=%f\n\n", x);
	}
	*/
}


int getLaserNumReadings(void)
{
	//
	// Daten aus aus Laser-Objekt holen 
	//
	return laser.num_readings;
}


float getLaserDistance(int angle)
{
	int numreadings;
	float *laserrange = NULL;
	
	
	if (received_laser != 1)
	{
		return -1;
	}

	
	//
	// Daten aus aus Laser-Objekt holen < < < <
	//
	numreadings = laser.num_readings;
	laserrange = laser.range;
	
	
	// greater than 180° ?
	if (angle > laser.num_readings)
	{
		return -2;
	}
	
	
	return laserrange[angle];
}


//------------------------------------
// Bei Programm-Ende aufrufen!!
//------------------------------------
void shutdown_laserview(void)
{
    carmen_ipc_disconnect();
}


static gint updateIPC(gpointer *data __attribute__ ((unused))) 
{
  carmen_ipc_sleep(0.01);
  
  // DIESE Zeile kann scheinbar weggelassen werden!
  //carmen_graphics_update_ipc_callbacks((GdkInputFunction)updateIPC);
  return 1;
}


//------------------------------------
// DAS HIER ZEICHNET NEU!
//------------------------------------
/*static void Redraw(void)
{
  float origin_x, origin_y, x = 0, y = 0,  angle, scale;
  int i;
  static GdkGC *Drawing_GC = NULL;
  static GdkPixmap *pixmap = NULL;
  GdkPoint *poly = NULL;
  char str[20];
  static double framerate = 0.0;


  // Anzahl der Laserstrahlen, die der Laser gemessen hat
  int numreadings;
  // Entfernung, die der Laser gemessen hat
  float *laserrange = NULL;
  
  //
  // Daten aus aus Laser-Objekt holen < < < <
  //
  numreadings = laser.num_readings;
  printf("numreadings in redraw: %d\n", numreadings);
  laserrange = laser.range;

  if(Drawing_GC == NULL)
  {
    carmen_graphics_setup_colors();
    Drawing_GC = gdk_gc_new(drawing_area->window);
	setup_colors();
  }

  // setup graphics parameters
  //
  // Einmal pixmap Objekt erzeugen
  if(pixmap == NULL)
    pixmap = gdk_pixmap_new(drawing_area->window, drawing_area->allocation.width, drawing_area->allocation.height, -1);
  if(pixmap == NULL)
    return;

  // erase window with light blue
  gdk_gc_set_foreground(Drawing_GC, &carmen_light_blue);
  
  // pixmap einfügen
  gdk_draw_rectangle(pixmap, Drawing_GC, TRUE, 0, 0, drawing_area->allocation.width, drawing_area->allocation.height);

  
  // zeichnen (Hintergrund...)
  gdk_gc_set_line_attributes(Drawing_GC, 4, GDK_LINE_SOLID, GDK_CAP_NOT_LAST, GDK_JOIN_MITER);
  gdk_gc_set_foreground(Drawing_GC, &carmen_blue);
  
  gdk_draw_arc(pixmap, Drawing_GC, FALSE, 0, 0, drawing_area->allocation.width,drawing_area->allocation.height * 2, 0, 180 * 64);
  gdk_draw_line(pixmap, Drawing_GC, 0, drawing_area->allocation.height, drawing_area->allocation.width, drawing_area->allocation.height);
  gdk_gc_set_line_attributes(Drawing_GC, 1, GDK_LINE_SOLID, GDK_CAP_NOT_LAST, GDK_JOIN_MITER);

  
  // wenn daten telegramm empfangen wurde
  // ist nach dem ersten Aufruf IMMER 1 !!
  if (received_laser)
  {
    // draw the empty space
   poly = (GdkPoint *)realloc(poly, sizeof(GdkPoint) * (numreadings + 1));
 //   carmen_test_alloc(poly);
    
    // Ursprung setzen
    origin_x = drawing_area->allocation.width / 2; 
    origin_y = drawing_area->allocation.height - 1;
    
    // skalierung für Zeichenfläche
    scale = drawing_area->allocation.height / (double)laser_range;
    
    // numreadings = beams gem. laserconfig
    for(i = 0; i < numreadings; i++)
    {
      if (laser.config.fov > M_PI+0.0001)
      {
	//scale to 180 deg fov (fov = Sichtfeld (field of view))
	angle = i / (float)(numreadings - 1) * M_PI;
      }
      else
      {
	// use real fov
	angle = 0.5*M_PI+laser.config.start_angle + i * laser.config.angular_resolution;
      }
      
      // x Koordinate der Entfernung
      x = laserrange[i] * cos(angle);
      // y Koordinate der Entfernung
      y = laserrange[i] * sin(angle);
      
      // punkte setzen
      poly[i].x = origin_x + x * scale;
      poly[i].y = origin_y - y * scale;
    }
    
    
    poly[numreadings].x = origin_x;
    poly[numreadings].y = origin_y;
    
    // ZEICHNEN
    gdk_gc_set_foreground(Drawing_GC, &carmen_white);
    gdk_draw_polygon(pixmap, Drawing_GC, TRUE, poly, numreadings + 1);

    // connect nearby laser points with lines
    gdk_gc_set_line_attributes(Drawing_GC, 2, GDK_LINE_SOLID, GDK_CAP_NOT_LAST, GDK_JOIN_MITER);
    
    free(poly); 
  } 

  if (laser_count == 1)
  {
    if (laser.config.fov > M_PI+0.0001)
    {
      carmen_warn("WARNING: the field of view of your laser is bigger than 180 degrees!\nThe display, however, is currently limited to 180 degrees\n\n");
    }
  }

  // fps anzeige
  if (laser_count % 10 == 0 || laser_count < 10)
    framerate = laser_count / (carmen_get_time() - start_time);
  sprintf(str, "Laser %d : %.1f fps", laser_num, framerate);
  
  gdk_gc_set_foreground(Drawing_GC, &carmen_black);
  gdk_draw_string(pixmap, gtk_style_get_font(drawing_area->style), Drawing_GC, 10, drawing_area->allocation.height - 40, str);

  // Sichtbereichswert (fied of view) anzeigen
  sprintf(str, "fov %.1f deg / %d pts", carmen_radians_to_degrees(laser.config.fov), numreadings);
  gdk_gc_set_foreground(Drawing_GC, &carmen_black);
  gdk_draw_string(pixmap, gtk_style_get_font(drawing_area->style), Drawing_GC, 10, drawing_area->allocation.height - 25, str);

  // 1st beam "links" liegt i.d.R. bei -90°
  sprintf(str, "1st beam %.1f deg", carmen_radians_to_degrees(laser.config.start_angle));
  gdk_gc_set_foreground(Drawing_GC, &carmen_black);
  gdk_draw_string(pixmap, gtk_style_get_font(drawing_area->style), Drawing_GC, 10, drawing_area->allocation.height - 10, str);

  // reflection config wert des Lasers anzeigen
  if (laser.num_remissions > 0)
  {
    sprintf(str, "remission: on");
    gdk_gc_set_foreground(Drawing_GC, &carmen_black);
    gdk_draw_string(pixmap, gtk_style_get_font(drawing_area->style), Drawing_GC, drawing_area->allocation.width - 155.0, drawing_area->allocation.height - 40, str);
  }

  sprintf(str, "Sensor maxrange  = %.1fm", laser.config.maximum_range);
  gdk_gc_set_foreground(Drawing_GC, &carmen_black);
  gdk_draw_string(pixmap, gtk_style_get_font(drawing_area->style), Drawing_GC, drawing_area->allocation.width - 155.0, drawing_area->allocation.height - 25, str);

  sprintf(str, "Viewer maxrange  = %.1fm", laser_range);
  gdk_gc_set_foreground(Drawing_GC, &carmen_black);
  gdk_draw_string(pixmap, gtk_style_get_font(drawing_area->style), Drawing_GC, drawing_area->allocation.width - 155.0, drawing_area->allocation.height - 10, str);

  
  // udpate the whole window 
  gdk_draw_pixmap(drawing_area->window, 
		  drawing_area->style->fg_gc[GTK_WIDGET_STATE (drawing_area)],
                  pixmap, 0, 0, 0, 0, 
		  drawing_area->allocation.width, 
		  drawing_area->allocation.height);
}
*/


// OHNE diese Methode wird der laser_handler auch nicht aufgerufen!!
/*static void start_graphics(int argc, char *argv[]) 
{
  GtkWidget *main_window;
  Pixmap mask;
  int r, c, byte, bit, rowspan;
  Display *display;
  Window window;
  unsigned char data[WINDOWSIZE * WINDOWSIZE];

  gtk_init(&argc, &argv);
  main_window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (GTK_WINDOW (main_window), "Laser View");

  drawing_area = gtk_drawing_area_new ();

  gtk_widget_set_usize (drawing_area, WINDOWSIZE, WINDOWSIZE / 2);
  gtk_container_add(GTK_CONTAINER(main_window), drawing_area);

  gtk_signal_connect(GTK_OBJECT(drawing_area), "expose_event", (GtkSignalFunc)Expose_Event, NULL);

  gtk_widget_add_events(drawing_area,  GDK_EXPOSURE_MASK);


  //---------------------------------------------------------------------
  // OHNE diese Zeile wird der laser_handler auch nicht aufgerufen!!
  //---------------------------------------------------------------------
  carmen_graphics_update_ipc_callbacks((GdkInputFunction)updateIPC);


  gtk_widget_realize(main_window);
  //  gdk_window_set_decorations(main_window->window, GDK_DECOR_TITLE);
  gdk_window_set_decorations(main_window->window, 0);
  gdk_window_set_functions(main_window->window, 0);
  
  gtk_widget_show(drawing_area);
  gtk_widget_show(main_window);

  //---------------------------------------------
  // Daten ausserhalb Halbkreis ausmaskieren
  //---------------------------------------------
  // rowspan = Spalten?
  // 1000 / 8 = 125
  rowspan = ceil(WINDOWSIZE / 8.0);
  
  // row 500
  for(r = 0; r < WINDOWSIZE / 2; r++)
  {
    // column 1000
    for(c = 0; c < WINDOWSIZE; c++)
    {
      byte = r * rowspan + c / 8;
      bit =  c % 8;
      
      if(hypot(WINDOWSIZE / 2 - r, WINDOWSIZE / 2 - c) < WINDOWSIZE / 2)
      {
	// set bit
	data[byte] = data[byte] | (1 << (bit));
      }
      else
      {
	// delete bit
	data[byte] = data[byte] & (~(1 << (bit)));
      }
    }
  }
  display = GDK_WINDOW_XDISPLAY(main_window->window);
  window = GDK_WINDOW_XWINDOW(main_window->window);
  
  mask = XCreatePixmapFromBitmapData(display, window, (char *)data, WINDOWSIZE, WINDOWSIZE, 1, 0, 1);
  
  
  //------------------
  // Bild rund machen
  //------------------
  XShapeCombineMask(display, window, ShapeBounding, 0, 0, mask, ShapeIntersect);
   
  gtk_main();
}
*/


// ehemals "main"
void startlaser (void)
{
	int argc = 1;
	char *argv[] = { "./mrs" };

	//qDebug("Vor ipc initialize");
	carmen_ipc_initialize(argc, argv);
	laser_num  = 0;
	//qDebug("Nach ipc initialize");
	//printf("argc=%d  /  argv='%s' /  laser_num=%d\n\n", argc, argv[0], laser_num);

	carmen_laser_subscribe_frontlaser_message (&laser, (carmen_handler_t)laser_handler, CARMEN_SUBSCRIBE_LATEST);

	//signal(SIGINT, shutdown_laserview);
	start_time = carmen_get_time();

	carmen_graphics_update_ipc_callbacks((GdkInputFunction)updateIPC);
	gtk_main();

	//return 0;
}
