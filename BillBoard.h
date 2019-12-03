#pragma once

#include <FL/gl.h>

class BillBoard {
private:
	GLubyte display_list;
	GLubyte stand;
	GLuint texture_obj;
	bool initialized;

public:
	BillBoard(void) { display_list = 0; stand = 0; initialized = false; };
	~BillBoard(void);

	bool Initialize(void);
	void Draw(void);

};
