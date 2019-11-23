/*
 * Track.cpp: A class that draws the train and its track.
 *
 * (c) 2001-2002: Stephen Chenney, University of Wisconsin at Madison.
 */


#include "Track.h"
#include "libtarga.h"
#include <stdio.h>
#include <FL/math.h>
#include <GL/glu.h>
#include <string>
#include <vector>
#include <iostream>

// The control points for the track spline.
/*const int   Track::TRACK_NUM_CONTROLS = 4;
const float Track::TRACK_CONTROLS[TRACK_NUM_CONTROLS][3] =
		{ { -20.0, -20.0, -18.0 }, { 20.0, -20.0, 40.0 },
		  { 20.0, 20.0, -18.0 }, { -20.0, 20.0, 40.0 } };*/
const int   Track::TRACK_NUM_CONTROLS = 15;
const float Track::TRACK_CONTROLS[TRACK_NUM_CONTROLS][3] =
{ {45.0,30.0,40.0},{40.0,35.0,38.0},{35.0,30.0,36.0},{40.0,25.0,34.0},{45.0,30.0,32.0},
  {40.0,35.0,30.0},{35.0,30.0,28.0},{40.0,25.0,26.0},{42.0,31.0,25.0},{-10.0,30.0,15.0},
  {-17.0,15.0,22.0},{-16.0,-25.0,17.0},{2.0,-23.0,12.0},{39.0,-10.0,30.0},{42.0,-1.0,40.0}
};

// The carriage energy and mass
const float Track::TRAIN_ENERGY = 450.0f;


// Normalize a 3d vector.
static void
Normalize_3(float v[3])
{
    double  l = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

    if ( l == 0.0 )
	return;

    v[0] /= (float)l;
    v[1] /= (float)l;
    v[2] /= (float)l;
}


// Destructor
Track::~Track(void)
{
    if ( initialized )
    {
	glDeleteLists(track_list, 1);
	glDeleteLists(train_list, 1);
    }
}


// Initializer. Would return false if anything could go wrong.
bool
Track::Initialize(void)
{
    CubicBspline    refined(3, true);
    int		    n_refined;
    float	    p[3];
    int		    i;

    // Create the track spline.
    track = new CubicBspline(3, true);
    for ( i = 0 ; i < TRACK_NUM_CONTROLS ; i++ )
	track->Append_Control(TRACK_CONTROLS[i]);

    // Refine it down to a fixed tolerance. This means that any point on
    // the track that is drawn will be less than 0.1 units from its true
    // location. In fact, it's even closer than that.
    track->Refine_Tolerance(refined, 0.1f);
    n_refined = refined.N();

    // Create the display list for the track - just a set of line segments
    // We just use curve evaluated at integer paramer values, because the
    // subdivision has made sure that these are good enough.
    track_list = glGenLists(1);
    glNewList(track_list, GL_COMPILE);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	    for ( i = 0 ; i <= n_refined ; i++ )
	    {
		refined.Evaluate_Point((float)i, p);
		glVertex3fv(p);
	    }
	glEnd();
    glEndList();



	//////////////////////////////train
	ubyte   *image_data;
	int	    image_height, image_width;


	if (!(image_data = (ubyte*)tga_load("thundercar.tga", &image_width,
		&image_height, TGA_TRUECOLOR_24)))
	{
		fprintf(stderr, "Ground::Initialize: Couldn't load .tga\n");
		return false;
	}
	
	glGenTextures(1, &traintext);
	glBindTexture(GL_TEXTURE_2D, traintext);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image_width, image_height,
		GL_RGB, GL_UNSIGNED_BYTE, image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);



    // Set up the train. At this point a cube is drawn. NOTE: The
    // x-axis will be aligned to point along the track. The origin of the
    // train is assumed to be at the bottom of the train.
    train_list = glGenLists(1);
    glNewList(train_list, GL_COMPILE);
    glColor3f(1.0, 1.0, 1.0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, traintext);

    glBegin(GL_QUADS);

	//top    (top of coaster)
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.5f, 0.5f, 0.6f);
	glVertex3f(-0.5f, 0.5f, 0.6f);
	glVertex3f(-0.5f, -0.5f, 0.6f);
	glVertex3f(0.5f, -0.5f, 0.6f);

	//bottom
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(0.5f, -0.5f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);

	//right  (front of coaster)
	glNormal3f(1.0f, 0.0f, 0.0f);

	glVertex3f(0.5f, 0.5f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.6f);
	glVertex3f(0.5f, -0.5f, 0.6f);
	glVertex3f(0.5f, -0.5f, 0.0f);

	//left
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.6f);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.6f);

	//far
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.5f, 0.5f, 0.6f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.5f, 0.5f, 0.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.5f, 0.5f, 0.6f);

	//near
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.5f, -0.5f, 0.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.5f, -0.5f, 0.6f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.5f, -0.5f, 0.6f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-0.5f, -0.5f, 0.0f);

    glEnd();
	glDisable(GL_TEXTURE_2D);
    glEndList();


	//second cart

	train_list2 = glGenLists(1);
	glNewList(train_list2, GL_COMPILE);
	glColor3f(1.0, 1.0, 1.0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, traintext);

	glBegin(GL_QUADS);

	//top    (top of coaster)
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.5f, 0.5f, 0.6f);
	glVertex3f(-0.5f, 0.5f, 0.6f);
	glVertex3f(-0.5f, -0.5f, 0.6f);
	glVertex3f(0.5f, -0.5f, 0.6f);

	//bottom
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(0.5f, -0.5f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);

	//right  (front of coaster)
	glNormal3f(1.0f, 0.0f, 0.0f);

	glVertex3f(0.5f, 0.5f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.6f);
	glVertex3f(0.5f, -0.5f, 0.6f);
	glVertex3f(0.5f, -0.5f, 0.0f);

	//left
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.6f);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.6f);

	//far
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.5f, 0.5f, 0.6f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.5f, 0.5f, 0.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.5f, 0.5f, 0.6f);

	//near
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.5f, -0.5f, 0.0f);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(0.5f, -0.5f, 0.6f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.5f, -0.5f, 0.6f);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-0.5f, -0.5f, 0.0f);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glEndList();

    initialized = true;

    return true;
}


// Draw
void
Track::Draw(void)
{
    float   posn[3];
    float   tangent[3];
    double  angle;
	
    if ( ! initialized )
	return;

    glPushMatrix();

    // Draw the track
    glCallList(track_list);

    glPushMatrix();

    // Figure out where the train is
    track->Evaluate_Point(posn_on_track, posn);
	
	posnvals[0].insert(posnvals[0].begin(), posn[0]);
	posnvals[1].insert(posnvals[1].begin(), posn[1]);
	posnvals[2].insert(posnvals[2].begin(), posn[2]);

	if (posnvals[0].size() >= maxLengthOfTrail) {
		posnvals[0].pop_back();
		posnvals[1].pop_back();
		posnvals[2].pop_back();
	}

    // Translate the train to the point

	//if (posnvals[0].size() < maxLengthOfTrail-2) {
		glTranslatef(posn[0], posn[1], posn[2]);
	/*}
	else {
		glTranslatef(posnvals[0][100], posnvals[1][100], posnvals[2][100]);
	}*/


    // ...and what it's orientation is
    track->Evaluate_Derivative(posn_on_track, tangent);
    Normalize_3(tangent);

	tangentvals[0].insert(tangentvals[0].begin(), tangent[0]);
	tangentvals[1].insert(tangentvals[1].begin(), tangent[1]);
	tangentvals[2].insert(tangentvals[2].begin(), tangent[2]);

	if (tangentvals[0].size() >= maxLengthOfTrail) {
		tangentvals[0].pop_back();
		tangentvals[1].pop_back();
		tangentvals[2].pop_back();
	}

    // Rotate it to poitn along the track, but stay horizontal
	//if (tangentvals[0].size() < maxLengthOfTrail - 2) {
		angle = atan2(tangent[1], tangent[0]) * 180.0 / M_PI;
	/*}
	else {
		angle = atan2(tangentvals[1][100], tangentvals[0][100]) * 180.0 / M_PI;
	}*/
    glRotatef((float)angle, 0.0f, 0.0f, 1.0f);
	
	

    // Another rotation to get the tilt right.
	//if (tangentvals[0].size() < maxLengthOfTrail - 2) {
		angle = asin(-tangent[2]) * 180.0 / M_PI;
	/*}
	else {
		angle = asin(-tangentvals[2][100]) * 180.0 / M_PI;
	}*/
    glRotatef((float)angle, 0.0f, 1.0f, 0.0f);

	

    // Draw the train
    glCallList(train_list);
	
	////////////////////////////////////////////
	//glPushMatrix();
	glPopMatrix();
	
	if (posnvals[0].size() < dfc+1) {
		glTranslatef(posnvals[0][0], posnvals[1][0], posnvals[2][0]);
	}
	else {
		glTranslatef(posnvals[0][dfc], posnvals[1][dfc], posnvals[2][dfc]);
	}

	if (tangentvals[0].size() < dfc+1) {
		angle = atan2(tangent[1], tangent[0]) * 180.0 / M_PI;
	}
	else {
		angle = atan2(tangentvals[1][dfc], tangentvals[0][dfc]) * 180.0 / M_PI;
	}
	glRotatef((float)angle, 0.0f, 0.0f, 1.0f);

	if (tangentvals[0].size() < dfc+1) {
		angle = asin(-tangent[2]) * 180.0 / M_PI;
	}
	else {
		angle = asin(-tangentvals[2][dfc]) * 180.0 / M_PI;
	}
	glRotatef((float)angle, 0.0f, 1.0f, 0.0f);
	
	/*glTranslatef(posnvals[0][0], posnvals[1][0], posnvals[2][0]);
	angle = atan2(tangentvals[1][0], tangentvals[0][0]) * 180.0 / M_PI;
	glRotatef((float)angle, 0.0f, 0.0f, 1.0f);
	angle = asin(-tangentvals[2][0]) * 180.0 / M_PI;
	glRotatef((float)angle, 0.0f, 1.0f, 0.0f); 
	*/

	glCallList(train_list2);
	
	//glPopMatrix();
	///////////////////////////////////////////
    glPopMatrix();
    glPopMatrix();
}


void
Track::Update(float dt)
{
    float   point[3];
    float   deriv[3];
    double  length;
    double  parametric_speed;

    if ( ! initialized )
	return;

    // First we move the train along the track with its current speed.

    // Get the derivative at the current location on the track.
    track->Evaluate_Derivative(posn_on_track, deriv);

    // Get its length.
    length = sqrt(deriv[0]*deriv[0] + deriv[1]*deriv[1] + deriv[2]*deriv[2]);
    if ( length == 0.0 )
		return;

    // The parametric speed is the world train speed divided by the length
    // of the tangent vector.
    parametric_speed = speed / length;

    // Now just evaluate dist = speed * time, for the parameter.
    posn_on_track += (float)(parametric_speed * dt);
	
    // If we've just gone around the track, reset back to the start.
    if ( posn_on_track > track->N() )
		posn_on_track -= track->N();

    // As the second step, we use conservation of energy to set the speed
    // for the next time.
    // The total energy = z * gravity + 1/2 speed * speed, assuming unit mass
    track->Evaluate_Point(posn_on_track, point);

    if ( TRAIN_ENERGY - 9.81 * point[2] < 0.0 )
	speed = 0.0;
    else
	speed = (float)sqrt(2.0 * ( TRAIN_ENERGY - 9.81 * point[2] ));



}


