
#include "BillBoard.h"
#include "libtarga.h"
#include <stdio.h>
#include <GL/glu.h>

BillBoard::~BillBoard(void)
{
	if (initialized)
	{
		glDeleteLists(display_list, 1);
		glDeleteTextures(1, &texture_obj);
	}
}

bool
BillBoard::Initialize(void)
{
	ubyte *image_data;
	int image_height, image_width;

	if (!(image_data = (ubyte*)tga_load("mcgrass.tga", &image_width,
		&image_height, TGA_TRUECOLOR_24)))
	{
		fprintf(stderr, "Ground::Initialize: Couldn't load grass.tga\n");
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
	glVertex3f(1.0f, 30.0f, 30.0f);
	glVertex3f(0.0f, 30.0f, 30.0f);
	glVertex3f(0.0f, 0.0f, 30.0f);
	glVertex3f(1.0f, 0.0f, 30.0f);
	
	//bottom
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(1.0f, 0.0f, 10.0f);
	glVertex3f(0.0f, 0.0f, 10.0f);
	glVertex3f(0.0f, 30.0f, 10.0f);
	glVertex3f(1.0f, 30.0f, 10.0f);

	//right
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 10.0f);
	glVertex3f(1.0f, 0.0f, 30.0f);
	glVertex3f(1.0f, 30.0f, 30.0f);
	glVertex3f(1.0f, 30.0f, 10.0f);

	//left
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 30.0f);
	glVertex3f(-0.5f, 0.5f, 10.0f);
	glVertex3f(-0.5f, -0.5f, 10.0f);
	glVertex3f(-0.5f, -0.5f, 30.0f);

	//far
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.5f, 0.5f, 30.0f);
	glVertex3f(0.5f, 0.5f, 10.0f);
	glVertex3f(-0.5f, 0.5f, 10.0f);
	glVertex3f(-0.5f, 0.5f, 30.0f);

	//near
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.5f, -0.5f, 10.0f);
	glVertex3f(0.5f, -0.5f, 30.0f);
	glVertex3f(-0.5f, -0.5f, 30.0f);
	glVertex3f(-0.5f, -0.5f, 10.0f);

	glEnd();

	glDisable(GL_TEXTURE_2D);
	glEndList();

	initialized = true;

	return true;
}

void
BillBoard::Draw(void)
{
	glPushMatrix();
	glCallList(display_list);
	glPopMatrix();
}