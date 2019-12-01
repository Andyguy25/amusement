/*
 * Track.h: Header file for a class that draws the train and its track.
 *
 * (c) 2001: Stephen Chenney, University of Wisconsin at Madison.
 */


#ifndef _TRAINTRACK_H_
#define _TRAINTRACK_H_

#include <Fl/gl.h>
#include <vector>
#include "CubicBspline.h"

class Track {
  private:
    GLubyte 	    track_list;	    // The display list for the track.
    GLubyte 	    train_list;	    // The display list for the train.
	GLubyte			trackpole;
	GLuint			traintext;
	GLuint			tracktext;

    bool    	    initialized;    // Whether or not we have been initialized.
    CubicBspline    *track;	    // The spline that defines the track.
    float	    posn_on_track;  // The train's parametric position on the
				    // track.
	
	
	

    float	    speed;	    // The train's speed, in world coordinates

    static const int	TRACK_NUM_CONTROLS;	// Constants about the track.
    static const float 	TRACK_CONTROLS[][3];
    static const float 	TRAIN_ENERGY;

  public:
    // Constructor
	  Track(void) { initialized = false; posn_on_track = 0.0f; stop = false, speed = 0.0f; };

    // Destructor
    ~Track(void);

    bool    Initialize(void);	// Gets everything set up for drawing.
    void    Update(float);	// Updates the location of the train
    void    Draw(void);		// Draws everything.
	void	parametricCar(float, float, float, float, float, float);

	std::vector<float> posnvals[3];
	std::vector<float> tangentvals[3];
	std::vector<float> refinetrackpos[3];
	const int maxLengthOfTrail = 500;
	const int dfc = 50;  //distance from front cart
	const int amountOfCars = 5;
	std::vector<float> randomNums[6];
	bool stop;
};

#endif

