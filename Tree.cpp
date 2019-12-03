
#include "Tree.h"
#include "libtarga.h"
#include <stdio.h>
#include <GL/glu.h>

Tree::~Tree(void)
{
	if (initialized)
	{
		glDeleteLists(tree_list, 1);
	}
}

bool
Tree::Initialize(void)
{

	tree_list = glGenLists(1);
	glNewList(tree_list, GL_COMPILE);
	glColor3f(1.0, 1.0, 1.0);

	float sizeOfPole = 0.5;



	/*glBegin(GL_QUADS);

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
	*/

	glEnd();
	glEndList();

	initialized = true;

	return true;
}

void
Tree::Draw(void)
{

	//parametric call for speakers
	glPushMatrix();
	glScalef(1, 1, 20);
	glTranslatef(-50.0, 40.0, 0.0);
	glCallList(tree_list);
	glPopMatrix();
}

/*
void
Tree::parametricTree(float scale, int season) {
	
}
*/