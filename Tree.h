#pragma once

#include <FL/gl.h>
#include <GL/GLU.h>

class Tree {
private:
	GLubyte tree_list;
	GLUquadricObj *quad = gluNewQuadric();
	//gluQuadricDrawStyle(GLU_LINE);


	bool initialized;

public:
	Tree(void) { tree_list = 0; initialized = false; };
	~Tree(void);

	bool Initialize(void);
	void Draw(void);
	//float for size, (scales whole tree evenly), and int for season.
	//SEE IF POSSIBLE TO DRAW DIFFERENT VERTICES FOR DIFFERENT PARAMETRIC CALL
	//EX. NOT DRAW CONE FOR TREE WHEN WINTER IS USED
	//void parametricTree(float, int); 

};