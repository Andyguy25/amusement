/*
 * World.cpp: Main program file for Project 3, CS 559
 *
 * (c) 2001-2002: Stephen Chenney
 */


#include <Fl/Fl.h>
#include "WorldWindow.h"
#include <stdio.h>
#include <iostream>


// The time per frame, in seconds (enforced only by timeouts.)
static const float  FRAME_TIME = 0.025f;

static WorldWindow  *world_window; // The window with world view in it


// This callback is called every 40th of a second if the system is fast
// enough. You should change the variable FRAME_TIME defined above if you
// want to change the frame rate.
static void
Timeout_Callback(void *data)
{
    // Update the motion in the world. This both moves the view and
    // animates the train.
    world_window->Update(FRAME_TIME);
    world_window->redraw();

    // Do the timeout again for the next frame.
    Fl::repeat_timeout(FRAME_TIME, Timeout_Callback);
}


int
main(int argc, char *argv[])
{
    //Fl::visual(FL_RGB);

    world_window = new WorldWindow(400, 100, 800, 600, "World");

    world_window->show(argc, argv);

    Fl::add_timeout(0.0, Timeout_Callback, NULL);

	std::cout << "Camera Controls:\n"
		<< " LEFT CTRL - front seat\n"
		<< " LEFT SHIFT - default move camera\n"
		<< " TAB - back seat\n"
		<< " RIGHT CTRL - to stop\n"
		;

    return Fl::run();
}


