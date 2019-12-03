#pragma once

#include <FL/gl.h>
#include <GL/glu.h>

class Tree {
private:
	GLubyte tree_list;
	GLubyte branch_list;
	GLubyte branch_list2;
	
	bool initialized;

public:
	Tree(void) { tree_list = 0; initialized = false; };
	~Tree(void);

	bool Initialize(void);
	void Draw(void);
	//float for size, (scales whole tree evenly), and int for season.
	void parametricTree(float, int); 
};