#pragma once

#include <FL/gl.h>

class Speaker {
private:
	GLubyte display_list;
	GLubyte stand;
	GLuint texture_obj;
	bool initialized;

public:
	Speaker(void) { display_list = 0; stand = 0; initialized = false; };
	~Speaker(void);

	bool Initialize(void);
	void Draw(void);
	void parametricSpeaker(float, float, float, float, float, float, bool);

};