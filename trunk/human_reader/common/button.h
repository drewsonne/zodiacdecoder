#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <list>
#include <vector>
#include "index_manager.h"

// A Very simple button system

#include "graphics_system.h"

enum EButtonState {
	BUTTON_DISABLED = 1,
	BUTTON_DOWN = 2,
	BUTTON_HOVER = 4 
};

class CButtonManager;

// These enums must be in the same order as the textures are cached in Init
enum EButtonTexture {
	eButton_Generic,
	eButton_DownArrow,
	eButton_UpArrow
};

class CButton {
public:
	friend class CButtonManager;
	CButton();
	CButton(int l, int t, int w, int h, EButtonTexture texture, char *text);
	~CButton();
	CButton& operator =(const CButton &rhs) {
		if (this == &rhs) {
			return *this;
		}

		state = rhs.state;
		top = rhs.top;
		left = rhs.left;
		width = rhs.width;
		height = rhs.height;
		textureType = rhs.textureType;
		strcpy_s(displayText, 256, rhs.displayText);
		return *this;
	}

	bool isDisabled() {return (state & BUTTON_DISABLED) > 0;}
	bool isHovered() {return (state & BUTTON_HOVER) > 0 ;}
	bool isDown() {return (state & BUTTON_DOWN) > 0;}
	int getState() {return state;};
	int getId() {return id;}
protected:
	int top, left, width, height;
	int state;
	IndexType id;
	EButtonTexture textureType;
	char displayText[256];
private:
};

class CButtonManager {
public:
	CButtonManager();
	~CButtonManager();
	IndexType addButton(const CButton &b);
	bool removeButton(IndexType id);
	int processButtons(long x, long y, int buttonDown);
	void renderButtons(CGraphicsSystem &g);
	bool Init(CGraphicsSystem &g);
protected:
	std::list<CButton> buttonList;
	std::vector<TextureHandle> buttonTextures;
private:
	CIndexManager buttonIdManager;
};
#endif