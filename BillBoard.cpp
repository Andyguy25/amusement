
#include "BillBoard.h"
#include "libtarga.h"
#include <stdio.h>
#include <GL/glu.h>

BillBoard::~BillBoard(void)
{
	if (initialized)
	{
		glDeleteLists(display_list, 1);
		glDeleteLists(stand, 1);
		glDeleteTextures(1, &texture_obj);
	}
}

bool
BillBoard::Initialize(void)
{
	ubyte *image_data;
	int image_height, image_width;

	if (!(image_data = (ubyte*)tga_load("disneysign.tga", &image_width,
		&image_height, TGA_TRUECOLOR_24)))
	{
		fprintf(stderr, "Ground::Initialize: Couldn't load disneysign.tga\n");
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
	glVertex3f(1.0f, 40.0f, 20.0f);
	glVertex3f(0.0f, 40.0f, 20.0f);
	glVertex3f(0.0f, 0.0f, 20.0f);
	glVertex3f(1.0f, 0.0f, 20.0f);
	
	//bottom
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 40.0f, 0.0f);
	glVertex3f(1.0f, 40.0f, 0.0f);

	//right (front of billboard)
	glNormal3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0f, 40.0f, 0.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0f, 40.0f, 20.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0f, 0.0f, 20.0f);
	
	//left
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(0.0f, 40.0f, 20.0f);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.0f, 40.0f, 0.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.0f, 0.0f, 20.0f);

	//far
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 40.0f, 20.0f);
	glVertex3f(1.0f, 40.0f, 0.0f);
	glVertex3f(0.0f, 40.0f, 0.0f);
	glVertex3f(0.0f, 40.0f, 20.0f);

	//near
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 20.0f);
	glVertex3f(0.0f, 0.0f, 20.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glEndList();


	///////for stand now
	stand = glGenLists(1);
	glNewList(stand, GL_COMPILE);
	glColor3f(0.5859, 0.2929, 0.0);
	glBegin(GL_QUADS);

	//top
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 30.0f);
	glVertex3f(0.0f, 1.0f, 30.0f);
	glVertex3f(0.0f, 0.0f, 30.0f);
	glVertex3f(1.0f, 0.0f, 30.0f);

	//bottom
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);

	//right
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 30.0f);
	glVertex3f(1.0f, 0.0f, 30.0f);

	//left
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 30.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 30.0f);

	//far
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 30.0f);
	glVertex3f(1.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 30.0f);

	//near
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 30.0f);
	glVertex3f(0.0f, 0.0f, 30.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
	glEndList();

	initialized = true;

	return true;
}

void
BillBoard::Draw(void)
{
	glPushMatrix();
	glTranslatef(-70.0,0.0,10.0);
	glScalef(1.0, 1.5, 1.5);
	glCallList(display_list);
	
	glPushMatrix();
	glTranslatef(-1.0, 0.0, -10.0);
	glCallList(stand);

	glTranslatef(0.0, 39.0, 0.0);
	glCallList(stand);
	glPopMatrix();
	glPopMatrix();
}