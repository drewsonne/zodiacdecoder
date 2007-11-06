#include "button.h"


CButton::CButton() {
	state = 0;
	left = 0;
	width = 0;
	height = 0;
	top = 0;
	id = 0;
}

CButton::CButton(int l, int t, int w, int h, EButtonTexture texture, char *text):left(l), width(w), top(t), height(h), textureType(texture)  {
	state = 0;
	id = 0;
	if (text == NULL) {
		displayText[0] = '\0';
	} else {
		strcpy_s (displayText,256,text);
	}
}

CButton::~CButton() {

}

CButtonManager::CButtonManager() {
	buttonList.clear();
	buttonTextures.clear();
}
CButtonManager::~CButtonManager() {
	
}
IndexType CButtonManager::addButton(const CButton &b) {
	IndexType id;
	if (buttonIdManager.getIndex(id) == NO_INDEX) {
		return 0;
	}

	CButton button = b;
	button.id = id;

	buttonList.push_back(button);

	return id;
}
bool CButtonManager::removeButton(IndexType id) {
	std::list<CButton>::iterator it = buttonList.begin();

	bool found = false;
	while(!found && it != buttonList.end()) {
		if((*it).id == id) {
			found = true;
			break;
		}
		++it;
	}

	if (found) {
		buttonList.erase(it);
		buttonIdManager.ReleaseIndex(id);
	}

	return found;
}

int CButtonManager::processButtons(long x, long y, int buttonDown) { //RNL Fixed small error, 0 is a button ID

	int clickedId = -1; 

	// Buttons should only have a few possible states
	// HOVER
	// HOVER + DOWN
	// DISABLED
	// normal;
	std::list<CButton>::iterator it = buttonList.begin();
	while(it != buttonList.end()) {
		if (! ((*it).state & BUTTON_DISABLED)) {
			(*it).state = 0;
			if ( x>= (*it).left && x <= (*it).left + (*it).width && y >= (*it).top && y <= (*it).top + (*it).height) {
				(*it).state |= BUTTON_HOVER;
				if (buttonDown) {
					(*it).state |= BUTTON_DOWN;	
					clickedId = (int)((*it).id);
				} 
			}
		} else {
			(*it).state = BUTTON_DISABLED;
		}
		++it;
	}

	return clickedId;
}

void CButtonManager::renderButtons(CGraphicsSystem &g) {

	STempRenderNode tempNode;

	std::list<CButton>::iterator it = buttonList.begin();
	while (it != buttonList.end()) {
			tempNode.pos = D3DXVECTOR3((*it).left, (*it).top, 0.0f);
			tempNode.scale = D3DXVECTOR3((*it).width, (*it).height, 1.0f);
			tempNode.textureHandle = buttonTextures[(*it).textureType];
			strcpy_s(tempNode.text, RENDERNODE_MAX_TEXT, (*it).displayText);
			switch ((*it).getState()) {
				case BUTTON_DISABLED:
					tempNode.color = kDisabledButtonColor;
					break;
				case BUTTON_DOWN | BUTTON_HOVER:
					tempNode.color = kDownButtonColor;
					break;
				case BUTTON_HOVER:
					tempNode.color = kHoveredButtonColor;
					break;
				default:
					tempNode.color = kButtonColor;
					break;
			}

		g.addRenderNode(tempNode);
		++it;
	}
}

bool CButtonManager::Init(CGraphicsSystem &g) {
	char fileName[256];
	sprintf_s(fileName,256,"%s/buttons/button.png",g.texturePath.c_str());
	TextureHandle buttonTexture = g.getTexture(fileName);
	if (buttonTexture.isNull()) {
		return false;
	} else {
		g.cacheTexture(buttonTexture);
		buttonTextures.push_back(buttonTexture);
	}

	sprintf_s(fileName,256,"%s/buttons/buttonDownArrow.png",g.texturePath.c_str());
	buttonTexture = g.getTexture(fileName);
	if (buttonTexture.isNull()) {
		return false;
	} else {
		g.cacheTexture(buttonTexture);
		buttonTextures.push_back(buttonTexture);
	}

	sprintf_s(fileName,256,"%s/buttons/buttonUpArrow.png",g.texturePath.c_str());
	buttonTexture = g.getTexture(fileName);
	if (buttonTexture.isNull()) {
		return false;
	} else {
		g.cacheTexture(buttonTexture);
		buttonTextures.push_back(buttonTexture);
	}

	return true;
}