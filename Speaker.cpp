
#include "Speaker.h"
#include "libtarga.h"
#include <stdio.h>
#include <GL/glu.h>

Speaker::~Speaker(void)
{
	if (initialized)
	{
		glDeleteLists(display_list, 1);
		glDeleteLists(stand, 1);
		glDeleteTextures(1, &texture_obj);
	}
}

bool
Speaker::Initialize(void)
{
	ubyte *image_data;
	int image_height, image_width;

	if (!(image_data = (ubyte*)tga_load("speaker.tga", &image_width,
		&image_height, TGA_TRUECOLOR_24)))
	{
		fprintf(stderr, "Ground::Initialize: Couldn't load speaker.tga\n");
		return false;
	}

	glGenTextures(1, &texture_obj);
	glBindTexture(GL_TEXTURE_2D, texture_obj);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image_width, image_height,
		GL_RGB, GL_UNSIGNED_BYTE, image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST_MIPMAP_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	display_list = glGenLists(1);
	glNewList(display_list, GL_COMPILE);
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture_obj);
	glBegin(GL_QUADS);

	//top
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 0.0); glVertex3f(10.0f, 10.0f, 10.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.0f, 10.0f, 10.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.0f, 0.0f, 10.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(10.0f, 0.0f, 10.0f);

	//bottom
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(10.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 10.0f, 0.0f);
	glVertex3f(10.0f, 10.0f, 0.0f);

	//right (front of billboard)
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 0.0); glVertex3f(10.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(10.0f, 10.0f, 0.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(10.0f, 10.0f, 10.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(10.0f, 0.0f, 10.0f);

	//left
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.0f, 10.0f, 10.0f);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.0f, 10.0f, 0.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.0f, 0.0f, 10.0f);

	//far
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(10.0f, 10.0f, 10.0f);
	glTexCoord2f(0.0, 0.0); glVertex3f(10.0f, 10.0f, 0.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.0f, 10.0f, 0.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.0f, 10.0f, 10.0f);

	//near
	glNormal3f(0.0f, -1.0f, 0.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(10.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 0.0); glVertex3f(10.0f, 0.0f, 10.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.0f, 0.0f, 10.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glEndList();


	///////for stand now
	stand = glGenLists(1);
	glNewList(stand, GL_COMPILE);
	glColor3f(0.1, 0.1, 0.1);

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

	initialized = true;

	return true;
}

void
Speaker::Draw(void)
{

	//parametric call for speakers

	parametricSpeaker(-80, -10, 0, 1.5, 1.5, 1.5, false);
	parametricSpeaker(-77, -10, 15, 1.2, 1.2, 1.2, false);
	parametricSpeaker(-75, -10, 27, 1.0, 1.0, 1.0, false);

	parametricSpeaker(-76, -20.0, 40.0, 0.5, 0.5, 0.5, true);
	parametricSpeaker(-80, -35, 30, 1.0, 1.0, 1.0, true);
	parametricSpeaker(-80, -50, 30, 1.0, 1.0, 1.0, true);
	parametricSpeaker(-76, -60.0, 40.0, 0.5, 0.5, 0.5, true);
	
	parametricSpeaker(-80, -80, 0, 1.5, 1.5, 1.5, false);
	parametricSpeaker(-77, -77, 15, 1.2, 1.2, 1.2, false);
	parametricSpeaker(-75, -75, 27, 1.0, 1.0, 1.0, false);

}

void
Speaker::parametricSpeaker(float posx, float posy, float posz, float sizex, float sizey, float sizez, bool haveStand) {
	
	glPushMatrix();
	glTranslatef(posx, posy, posz);
	glScalef(sizex, sizey, sizez);
	glCallList(display_list);
	glPopMatrix();
	if (haveStand) {
		glPushMatrix();
		float centerx = (sizex * 10) / 2;
		float centery = (sizey * 10) / 2;

		glTranslatef(posx+centerx, posy+centery, 0);
		
		glScalef(1.0, 1.0, (posz - 0.001));
		glCallList(stand);
		glPopMatrix();
	}
}