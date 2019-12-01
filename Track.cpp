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
#include <time.h>


// The control points for the track spline.
const int   Track::TRACK_NUM_CONTROLS = 13;
const float Track::TRACK_CONTROLS[TRACK_NUM_CONTROLS][3] =
{ {55.0,38.0,40.0},{20.0,45.0,38.0},{15.0,15.0,36.0},{35.0,15.0,34.0},{45.0,30.0,32.0},
  {60.0,25.0,30.0},{70.0,55.0,28.0},{-10.0,75.0,26.0},
  {-17.0,15.0,22.0},{-16.0,-25.0,17.0},{2.0,-23.0,12.0},{39.0,-10.0,30.0},{42.0,-1.0,45.0}
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
	glDeleteLists(trackpole, 1);
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


	ubyte   *image_datat;
	int	    image_heightt, image_widtht;


	if (!(image_datat = (ubyte*)tga_load("road3.tga", &image_widtht,
		&image_heightt, TGA_TRUECOLOR_24)))
	{
		fprintf(stderr, "Ground::Initialize: Couldn't load road3.tga\n");
		return false;
	}

	glGenTextures(1, &tracktext);
	glBindTexture(GL_TEXTURE_2D, tracktext);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image_widtht, image_heightt,
		GL_RGB, GL_UNSIGNED_BYTE, image_datat);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // Create the display list for the track - just a set of line segments
    // We just use curve evaluated at integer paramer values, because the
    // subdivision has made sure that these are good enough.
    track_list = glGenLists(1);
    glNewList(track_list, GL_COMPILE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tracktext);
	glColor3f(1.0f, 1.0f, 1.0f);
	double points[2][4][3];
	
	int start_i = 0;
	int end_i = 1;
	int x = 0;
	int y = 1;
	int z = 2;
	int trackSize = 3.2;
	
	refined.Evaluate_Point((float)n_refined-1, p);
	
	float lastPosx = p[0];
	float lastPosy = p[1];
	float lastPosz = p[2];

	    for ( i = 0 ; i <= n_refined ; i++ )
	    {

		refined.Evaluate_Point((float)i, p);

		glBegin(GL_TRIANGLE_STRIP);
		float currDirx = abs(p[0] - lastPosx);
		float currDiry = abs(p[1] - lastPosy);
		float currDirz = abs(p[2] - lastPosz);
		float comboxy = currDirx + currDiry;

		float dirx = p[0] - lastPosx;
		float diry = p[1] - lastPosy;
		float dirz = p[2] - lastPosz;
		
		for (int j = 0; j < 4; j++) {
			
			
			float movex = currDirx / comboxy;
			float movey = currDiry / comboxy;

			//bottom left of roller coaster track
			if (j == 0) {
				//>^
				if (dirx >= 0 && diry >= 0) {
					points[end_i][j][x] = p[0] + ((1 - movex)*-trackSize);
					points[end_i][j][y] = p[1] + ((1 - movey)*trackSize);
				}
				//<^
				if (dirx < 0 && diry >= 0) {
					points[end_i][j][x] = p[0] + ((1 - movex)*-trackSize);
					points[end_i][j][y] = p[1] + ((1 - movey)*-trackSize);
				}
				//<v
				if (dirx < 0 && diry < 0) {
					points[end_i][j][x] = p[0] + ((1 - movex)*trackSize);
					points[end_i][j][y] = p[1] + ((1 - movey)*-trackSize);
				}
				//>v
				if (dirx >= 0 && diry < 0) {
					points[end_i][j][x] = p[0] + ((1 - movex)*trackSize);
					points[end_i][j][y] = p[1] + ((1 - movey)*trackSize);
				}
				points[end_i][j][z] = p[2]-0.2;
			}
			
			//bottom right
			if (j == 1) {
				//>^
				if (dirx >= 0 && diry >= 0) {
					points[end_i][j][x] = p[0] + ((1 - movex)*trackSize);
					points[end_i][j][y] = p[1] + ((1 - movey)*-trackSize);
				}
				//<^
				if (dirx < 0 && diry >= 0) {
					points[end_i][j][x] = p[0] + ((1 - movex)*trackSize);
					points[end_i][j][y] = p[1] + ((1 - movey)*trackSize);
				}
				//<v
				if (dirx < 0 && diry < 0) {
					points[end_i][j][x] = p[0] + ((1 - movex)*-trackSize);
					points[end_i][j][y] = p[1] + ((1 - movey)*trackSize);
				}
				//>v
				if (dirx >= 0 && diry < 0) {
					points[end_i][j][x] = p[0] + ((1 - movex)*-trackSize);
					points[end_i][j][y] = p[1] + ((1 - movey)*-trackSize);
				}
				points[end_i][j][z] = p[2]-0.2;
			}

			//top right
			if (j == 2) {
				//>^
				if (dirx >= 0 && diry >= 0) {
					points[end_i][j][x] = p[0] + ((1 - movex)*trackSize);
					points[end_i][j][y] = p[1] + ((1 - movey)*-trackSize);
				}
				//<^
				if (dirx < 0 && diry >= 0) {
					points[end_i][j][x] = p[0] + ((1 - movex)*trackSize);
					points[end_i][j][y] = p[1] + ((1 - movey)*trackSize);
				}
				//<v
				if (dirx < 0 && diry < 0) {
					points[end_i][j][x] = p[0] + ((1 - movex)*-trackSize);
					points[end_i][j][y] = p[1] + ((1 - movey)*trackSize);
				}
				//>v
				if (dirx >= 0 && diry < 0) {
					points[end_i][j][x] = p[0] + ((1 - movex)*-trackSize);
					points[end_i][j][y] = p[1] + ((1 - movey)*-trackSize);
				}
				points[end_i][j][z] = p[2]+0.1;
			}

			//top left
			if (j == 3) {
				//>^
				if (dirx >= 0 && diry >= 0) {
					points[end_i][j][x] = p[0] + ((1 - movex)*-trackSize);
					points[end_i][j][y] = p[1] + ((1 - movey)*trackSize);
				}
				//<^
				if (dirx < 0 && diry >= 0) {
					points[end_i][j][x] = p[0] + ((1 - movex)*-trackSize);
					points[end_i][j][y] = p[1] + ((1 - movey)*-trackSize);
				}
				//<v
				if (dirx < 0 && diry < 0) {
					points[end_i][j][x] = p[0] + ((1 - movex)*trackSize);
					points[end_i][j][y] = p[1] + ((1 - movey)*-trackSize);
				}
				//>v
				if (dirx >= 0 && diry < 0) {
					points[end_i][j][x] = p[0] + ((1 - movex)*trackSize);
					points[end_i][j][y] = p[1] + ((1 - movey)*trackSize);
				}
				points[end_i][j][z] = p[2]+0.1;
			}

			if (j == 3) {
				glTexCoord2f(1.0, 1.0);
				glVertex3f(points[start_i][j][x], points[start_i][j][y], points[start_i][j][z]);
				glTexCoord2f(1.0, 0.0);
				glVertex3f(points[end_i][j][x], points[end_i][j][y], points[end_i][j][z]);
			}
			if (j == 0) {
				glTexCoord2f(0.0, 1.0);
				glVertex3f(points[start_i][j][x], points[start_i][j][y], points[start_i][j][z]);
				glTexCoord2f(0.0, 0.0);
				glVertex3f(points[end_i][j][x], points[end_i][j][y], points[end_i][j][z]);
			}
			
			if (j != 3 && j != 0) {

				glVertex3f(points[start_i][j][x], points[start_i][j][y], points[start_i][j][z]);

				glVertex3f(points[end_i][j][x], points[end_i][j][y], points[end_i][j][z]);
			}
		}
		glVertex3f(points[start_i][0][x], points[start_i][0][y], points[start_i][0][z]);
		glVertex3f(points[end_i][0][x], points[end_i][0][y], points[end_i][0][z]);
		glEnd();
		int temp = start_i; start_i = end_i; end_i = temp;

		lastPosx = p[0];
		lastPosy = p[1];
		lastPosz = p[2];


		refinetrackpos[0].push_back(p[0]);
		refinetrackpos[1].push_back(p[1]);
		refinetrackpos[2].push_back(p[2]);
	    }

	glDisable(GL_TEXTURE_2D);
    glEndList();



	//////////////////////////////train
	ubyte   *image_data;
	int	    image_height, image_width;


	if (!(image_data = (ubyte*)tga_load("car.tga", &image_width,
		&image_height, TGA_TRUECOLOR_24)))
	{
		fprintf(stderr, "Ground::Initialize: Couldn't load thundercar2.tga\n");
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
	///////////////////////////////////////////////////////////

    // Set up the train. At this point a cube is drawn. NOTE: The
    // x-axis will be aligned to point along the track. The origin of the
    // train is assumed to be at the bottom of the train.
    train_list = glGenLists(1);
    glNewList(train_list, GL_COMPILE);
    //glColor3f(1.0f, 1.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, traintext);

    glBegin(GL_QUADS);

	//top    (top of coaster)
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(1.0, 0.5);
	glVertex3f(0.5f, 0.5f, 0.6f);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-0.5f, 0.5f, 0.6f);
	glTexCoord2f(0.5, 1.0);
	glVertex3f(-0.5f, -0.5f, 0.6f);
	glTexCoord2f(0.5, 0.5);
	glVertex3f(0.5f, -0.5f, 0.6f);

	//bottom
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(0.5f, -0.5f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);

	//right  (front of coaster) front texture
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.5, 0.5); glVertex3f(0.5f, 0.5f, 0.0f);
	glTexCoord2f(0.5, 0.0); glVertex3f(0.5f, 0.5f, 1.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.5f, -0.5f, 1.0f);
	glTexCoord2f(1.0, 0.5); glVertex3f(0.5f, -0.5f, 0.0f);
	//right inside (front of coaster)
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.0f);
	glVertex3f(0.5f, -0.5f, 1.0f);
	glVertex3f(0.5f, 0.5f, 1.0f);

	//left back texture
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 0.5); glVertex3f(-0.5f, 0.5f, 1.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(-0.5f, 0.5f, 0.0f);
	glTexCoord2f(0.5, 1.0); glVertex3f(-0.5f, -0.5f, 0.0f);
	glTexCoord2f(0.5, 0.5); glVertex3f(-0.5f, -0.5f, 1.0f);
	//left inside
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 1.0f);
	glVertex3f(-0.5f, -0.5f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 1.0f);

	//far
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.5, 0.0);
	glVertex3f(0.5f, 0.5f, 1.0f);
	glTexCoord2f(0.5, 0.5);
	glVertex3f(0.5f, 0.5f, 0.0f);
	glTexCoord2f(0.0, 0.5);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.5f, 0.5f, 1.0f);
	//far inside
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 1.0f);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);
	glVertex3f(0.5f, 0.5f, 1.0f);

	//near
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(0.5, 0.5);
	glVertex3f(0.5f, -0.5f, 0.0f);
	glTexCoord2f(0.5, 0.0);
	glVertex3f(0.5f, -0.5f, 1.0f);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.5f, -0.5f, 1.0f);
	glTexCoord2f(0.0, 0.5);
	glVertex3f(-0.5f, -0.5f, 0.0f);
	//near inside
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 1.0f);
	glVertex3f(0.5f, -0.5f, 1.0f);
	glVertex3f(0.5f, -0.5f, 0.0f);

    glEnd();
	glDisable(GL_TEXTURE_2D);
    glEndList();


	/////////trackpole
	trackpole = glGenLists(1);
	glNewList(trackpole, GL_COMPILE);
	glColor3f(0.75, 0.75, 0.75);
	float sizeOfPole = 0.5;

	glBegin(GL_QUADS);

	//top
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(sizeOfPole, sizeOfPole, 1.0f);
	glVertex3f(0.0f, sizeOfPole, 1.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(sizeOfPole, 0.0f, 1.0f);

	//bottom
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(sizeOfPole, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, sizeOfPole, 0.0f);
	glVertex3f(sizeOfPole, sizeOfPole, 0.0f);

	//right
	glNormal3f(0.1f, 0.0f, 0.0f);
	glVertex3f(sizeOfPole, 0.0f, 0.0f);
	glVertex3f(sizeOfPole, sizeOfPole, 0.0f);
	glVertex3f(sizeOfPole, sizeOfPole, 1.0f);
	glVertex3f(sizeOfPole, 0.0f, 1.0f);

	//left
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, sizeOfPole, 1.0f);
	glVertex3f(0.0f, sizeOfPole, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);

	//far
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(sizeOfPole, sizeOfPole, 1.0f);
	glVertex3f(sizeOfPole, sizeOfPole, 0.0f);
	glVertex3f(0.0f, sizeOfPole, 0.0f);
	glVertex3f(0.0f, sizeOfPole, 1.0f);

	//near
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(sizeOfPole, 0.0f, 0.0f);
	glVertex3f(sizeOfPole, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();
	glEndList();

	//////////////////initialize random numbers for parametric instancing
	srand(time(NULL));
	float sizeLO = 0.75;
	float sizeHI = 4.0;
	float colorLO = 0.1;
	float colorHI = 1.0;
	int realAmountOfCars = floor(maxLengthOfTrail / dfc);
	if (amountOfCars < realAmountOfCars) {
		realAmountOfCars = amountOfCars;
	}
	
	for (int i = 0; i <= realAmountOfCars; i++) { 
		for (int j = 0; j < 6; j++) { //6 for number of items needing to be randomized per car
			if (j < 3) {
				randomNums[i].push_back(sizeLO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (sizeHI - sizeLO))));
			}
			else {
				randomNums[i].push_back(colorLO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (colorHI - colorLO))));
			}
		}
	}

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


	for (int i = 0; i < refinetrackpos[0].size(); i++) {
		if (i % 12 == 0) {
			glPushMatrix();
			glTranslatef(refinetrackpos[0][i], refinetrackpos[1][i], 0.0);
			glScalef(1.0, 1.0, refinetrackpos[2][i]);
			glCallList(trackpole);
			glPopMatrix();
		}
	}

    glPushMatrix();

    // Figure out where the train is
    track->Evaluate_Point(posn_on_track, posn);
	
	if (!stop) {
		posnvals[0].insert(posnvals[0].begin(), posn[0]);
		posnvals[1].insert(posnvals[1].begin(), posn[1]);
		posnvals[2].insert(posnvals[2].begin(), posn[2]);

		if (posnvals[0].size() >= maxLengthOfTrail) {
			posnvals[0].pop_back();
			posnvals[1].pop_back();
			posnvals[2].pop_back();
		}
	}


    // ...and what it's orientation is
    track->Evaluate_Derivative(posn_on_track, tangent);
    Normalize_3(tangent);

	if (!stop) {
		tangentvals[0].insert(tangentvals[0].begin(), tangent[0]);
		tangentvals[1].insert(tangentvals[1].begin(), tangent[1]);
		tangentvals[2].insert(tangentvals[2].begin(), tangent[2]);

		if (tangentvals[0].size() >= maxLengthOfTrail) {
			tangentvals[0].pop_back();
			tangentvals[1].pop_back();
			tangentvals[2].pop_back();
		}
	}

    // Draw the trains
	////////////////////////////////////////////
	glPopMatrix();
	int realAmountOfCars = floor(maxLengthOfTrail / dfc);
	if (amountOfCars < realAmountOfCars) {
		realAmountOfCars = amountOfCars;
	}

	for (int i = 0; i <= realAmountOfCars; i++) {
		glPushMatrix();
		if (posnvals[0].size() < ((dfc*i) + 1)) {
			glTranslatef(posnvals[0][0], posnvals[1][0], posnvals[2][0]);
		}
		else {
			glTranslatef(posnvals[0][dfc*i], posnvals[1][dfc*i], posnvals[2][dfc*i]);
		}

		if (tangentvals[0].size() < (dfc*i) + 1) {
			angle = atan2(tangent[1], tangent[0]) * 180.0 / M_PI;
		}
		else {
			angle = atan2(tangentvals[1][dfc*i], tangentvals[0][dfc*i]) * 180.0 / M_PI;
		}
		glRotatef((float)angle, 0.0f, 0.0f, 1.0f);

		if (tangentvals[0].size() < (dfc*i) + 1) {
			angle = asin(-tangent[2]) * 180.0 / M_PI;
		}
		else {
			angle = asin(-tangentvals[2][dfc*i]) * 180.0 / M_PI;
		}
		glRotatef((float)angle, 0.0f, 1.0f, 0.0f);

		//call parametric instancing function to change appearance of cars

		if (i == 0) {
			parametricCar(3.0,2.0,1.0, 0.75,0.75,1.0);
		}
		if (i == realAmountOfCars) {
		parametricCar(5.0, 3.0, 1.0,  1.0, 0.75, 0.75);
		}
		if (i != 0 && i != realAmountOfCars) {
			parametricCar(randomNums[i][0], randomNums[i][1], randomNums[i][2],
				randomNums[i][3], randomNums[i][4], randomNums[i][5]);
		}
		//only call each train to display when the position is ready
		if (posnvals[0].size() > ((dfc*i) + 1)) {
			glCallList(train_list);
		}
		glPopMatrix();
	}
	///////////////////////////////////////////
    glPopMatrix();
    glPopMatrix();
}

void
Track::parametricCar(float sizex, float sizey, float sizez, float red, float green, float blue) {
	glScalef(sizex, sizey, sizez);
	glColor3f(red, green, blue);
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

