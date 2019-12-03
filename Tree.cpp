
#include "Tree.h"
#include "libtarga.h"
#include <stdio.h>
#include <GL/glu.h>

Tree::~Tree(void)
{
	if (initialized)
	{
		glDeleteLists(tree_list, 1);
		glDeleteLists(branch_list, 1);
		glDeleteLists(branch_list2, 1);
	}
}

bool
Tree::Initialize(void)
{

	tree_list = glGenLists(1);
	glNewList(tree_list, GL_COMPILE);
	GLUquadricObj *quadtrunk = gluNewQuadric();
	gluCylinder(quadtrunk, 0.5f, 0.5f, 20.0f, 32, 32);
	glEndList();


	branch_list = glGenLists(1);
	glNewList(branch_list, GL_COMPILE);
	GLUquadricObj *quadbranch = gluNewQuadric();
	gluCylinder(quadbranch, 5.0f, 2.5f, 10.0f, 32, 32);
	glEndList();

	branch_list2 = glGenLists(1);
	glNewList(branch_list2, GL_COMPILE);
	GLUquadricObj *quadbranch2 = gluNewQuadric();
	gluCylinder(quadbranch2, 2.5f, 0.0f, 10.0f, 32, 32);
	glEndList();

	initialized = true;

	return true;
}

void
Tree::Draw(void)
{
	glTranslatef(-20.0, -80.0, 0.0);
	parametricTree(1.5, 0);
	glTranslatef(30.0, 0.0, 0.0);
	parametricTree(1.5, 1);
	glTranslatef(30.0, 0.0, 0.0);
	parametricTree(1.5, 2);
	glTranslatef(30.0, 0.0, 0.0);
	parametricTree(1.5, 3);

	glTranslatef(0.0, 20.0, 0.0);
	parametricTree(1.0, 3);
	glTranslatef(-15.0, 0.0, 0.0);
	parametricTree(1.0, 2);
	glTranslatef(-15.0, 0.0, 0.0);
	parametricTree(1.0, 1);
	glTranslatef(-15.0, 0.0, 0.0);
	parametricTree(1.0, 0);

	glTranslatef(-15.0, 0.0, 0.0);
	parametricTree(0.5, 3);
	glTranslatef(-10.0, 0.0, 0.0);
	parametricTree(0.5, 2);
	glTranslatef(-10.0, 0.0, 0.0);
	parametricTree(0.5, 1);
	glTranslatef(-10.0, 0.0, 0.0);
	parametricTree(0.5, 0);
}

//scale value scales entire tree
//season, 0 is summer (light green), 1 fall (orange), 2 winter (green with snow top), 3 spring pink
void
Tree::parametricTree(float scale, int season) {
	glPushMatrix();

	glScalef(scale, scale, scale);
	glColor3f(0.5859, 0.2929, 0.0);
	glCallList(tree_list);
	glTranslatef(0.0, 0.0, 5.0);
	if (season == 0) 
		glColor3f(0.59375, 0.9804, 0.59375);
	if (season == 1)
		glColor3f(0.59375, 0.2812, 0.1679);
	if (season == 2)
		glColor3f(0.2265, 0.3711, 0.04296);
	if (season == 3)
		glColor3f(1.0,0.75, 0.7929);
	glCallList(branch_list);
	glTranslatef(0.0, 0.0, 10.0);
	if (season == 2)
		glColor3f(1.0, 1.0, 1.0);
	glCallList(branch_list2);

	glPopMatrix();
}
