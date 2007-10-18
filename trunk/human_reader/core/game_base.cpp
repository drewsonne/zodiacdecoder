#include "game_base.h"	
#include <fstream>

int convertKeyToChar(int key) {
	if (key >= DIK_1 && key <= DIK_0) {
		return (key-DIK_1) + 26;
	}

	switch(key) {
		case DIK_A: return 0;
		case DIK_B: return 1;
		case DIK_C: return 2;
		case DIK_D: return 3;
		case DIK_E: return 4;
		case DIK_F: return 5;
		case DIK_G: return 6;
		case DIK_H: return 7;
		case DIK_I: return 8;
		case DIK_J: return 9;
		case DIK_K: return 10;
		case DIK_L: return 11;
		case DIK_M: return 12;
		case DIK_N: return 13;
		case DIK_O: return 14;
		case DIK_P: return 15;
		case DIK_Q: return 16;
		case DIK_R: return 17;
		case DIK_S: return 18;
		case DIK_T: return 19;
		case DIK_U: return 20;
		case DIK_V: return 21;
		case DIK_W: return 22;
		case DIK_X: return 23;
		case DIK_Y: return 24;
		case DIK_Z: return 25;
	}

	return -1;
}

/////////////////////////////////////////////////////////////////////
//							CGameBase::CGameBase
/////////////////////////////////////////////////////////////////////
CGameBase::CGameBase() {
	isMinimized = false;
	fatalError = false;
	alphaTextures.clear();
	cipherTextures.clear();
	typingBuffer[0] = -1;
	currentTypingPos = 0;
	focus = FOCUS_TYPING;
	wordList.clear();
	wordPos.clear();
	selectionPos = -1;
}

/////////////////////////////////////////////////////////////////////
//							CGameBase::~CGameBase
/////////////////////////////////////////////////////////////////////
CGameBase::~CGameBase() {

	// TODO: Not sure if we want to call shutdown in the destructor
	// Shutdown call a pure virtual ShutdownSystem function.  When 
	// we get to the destructor the vtbl no longer points to derived
	// classes.
	Shutdown();
	alphaTextures.clear();
	cipherTextures.clear();
	keyMap.clear();
	currentKeyMap.clear();
	conflictingKeyMap.clear();
}

/////////////////////////////////////////////////////////////////////
//							CGameBase::ProcessGame
/////////////////////////////////////////////////////////////////////
bool CGameBase::Process(){

	// Calculate the frame time

	// Read Input and update the state of all input devices.
	if (!input.ReadAllDeviceStates()) {
		return false;
	}

	// Update the render state for the frame
	if (!ProcessRenderState()) {
	}

	// Render the current render state
	graphics.Render();

	// Process Input for this frame based on the input state.
	if (!ProcessInputFrame(1.0f)) {
		return false;
	}

	// Update game state for this time slice.
	if (!ProcessGameFrame(1.0f)) {
		return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////
//										  CGameBase::IsReadyToProcess
/////////////////////////////////////////////////////////////////////
bool CGameBase::ShouldProcessForeground() {

	if (!isActive) {
		return false;
	}

	if (!graphics.IsDeviceReady()) {
		return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////
//										  CGameBase::IsReadyToProcess
/////////////////////////////////////////////////////////////////////
bool CGameBase::CheckForFatal() {
	if (fatalError) {
		return true;
	}

	if (graphics.fatalError) {
		return true;
	}

	return false;
}

//-------------------------------------------------------------------
//											Protected Member function
/////////////////////////////////////////////////////////////////////
//										  CGameBase::InitializeSystem
/////////////////////////////////////////////////////////////////////
bool CGameBase::InitializeSystem(void *initStructure){

	// --------------------------------------------------------------
	//									    Get initialization params
	if (initStructure == NULL) {
		return false;
	}

	SGameBaseInitParams *initParams = (SGameBaseInitParams *)initStructure;

	if (initParams->windowHandle == NULL) {
		return false;
	}

	if (initParams->instance == NULL) {
		return false;
	}

	// --------------------------------------------------------------
	//								 Inititialize the Graphics System
	CGraphicsSystem::SGraphicsScreenStructure graphicsInitParams;
	graphicsInitParams.fullScreen = false;
	graphicsInitParams.screenWidth = 1024;
	graphicsInitParams.screenHeight = 768;
	graphicsInitParams.windowWidth = graphicsInitParams.screenWidth;
	graphicsInitParams.windowHeight = graphicsInitParams.screenHeight;
	graphicsInitParams.windowX = (GetSystemMetrics(SM_CXSCREEN) - graphicsInitParams.windowWidth) / 2;
	graphicsInitParams.windowY  = (GetSystemMetrics(SM_CYSCREEN) - graphicsInitParams.windowHeight) / 2;
	graphicsInitParams.windowHandle = initParams->windowHandle;
	if (!graphics.Initialize(&graphicsInitParams)) {
		return false;
	}

	// --------------------------------------------------------------
	//										  Initialize Input System
	CInputSystem::SInputSystemInfoStructure inputInitParams;
	inputInitParams.windowHandle = initParams->windowHandle;
	inputInitParams.instance = initParams->instance;
	if (!input.Initialize(&inputInitParams)) {
		return false;
	}

	// --------------------------------------------------------------
	//									 Game Specific Initialization
	if (!InitializeGame()) {
		return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////
//							CGameBase::InitializeGame
/////////////////////////////////////////////////////////////////////
bool CGameBase::InitializeGame() {

	// TODO  we would put this in a derived class
	char fileName[256];
	for (int i = 0; i <= 35; i++) {
		sprintf_s(fileName,256,"%s/alphabet/alpha.%d.png",graphics.texturePath.c_str(),i);
		TextureHandle tempHandle = graphics.getTexture(fileName);
		if(tempHandle.isNull()) {
			return false;
		}

		if (!graphics.cacheTexture(tempHandle)) {
			assert(false);
			return false;
		}

		alphaTextures.push_back(tempHandle);
	}

	for (int i = 0; i <= 62; i++) {
		sprintf_s(fileName,256,"%s/cipher/cipher.%d.png",graphics.texturePath.c_str(),i);
		TextureHandle tempHandle = graphics.getTexture(fileName);
		if(tempHandle.isNull()) {
			return false;
		}

		if (!graphics.cacheTexture(tempHandle)) {
			assert(false);
			return false;
		}

		cipherTextures.push_back(tempHandle);
	}

	std::ifstream inFile("340index.txt", std::ios_base::in);
	if(inFile.fail()) {
		assert(false);
		return false;
	}


	while(!inFile.eof()) {
		int tempInt = -1;
		inFile >> tempInt;
		if (tempInt <0) {
			assert(false);
			return false;
		}

		cipherText.push_back(tempInt);
	}

	inFile.close();

	for (int i = 0; i < 63; i++) {
		keyMap.push_back(-1);
		currentKeyMap.push_back(-1);
		conflictingKeyMap.push_back(-1);
	}
	// Base class does nothing
	return true;
}

/////////////////////////////////////////////////////////////////////
//							CGameBase::ShutdownSystem
/////////////////////////////////////////////////////////////////////
bool CGameBase::ShutdownSystem() {

	// --------------------------------------------------------------
	//									       Game Specific Shutdown
	if (!ShutdownGame()) {
		return false;
	}

	return true;
}	

/////////////////////////////////////////////////////////////////////
//							CGameBase::ShutdownGame
/////////////////////////////////////////////////////////////////////
bool CGameBase::ShutdownGame() {

	// Base class does nothing
	return true;
}

/////////////////////////////////////////////////////////////////////
//							CGameBase::ProcessGameFrame
/////////////////////////////////////////////////////////////////////
bool CGameBase::ProcessGameFrame (float dT) {
	return true;
}

/////////////////////////////////////////////////////////////////////
//							CGameBase::ProcessGameFrame
/////////////////////////////////////////////////////////////////////
bool CGameBase::ProcessInputFrame (float dT) {
	static bool keyDown = false;

	const char* buffer = input.getKeyboardStateBuffer();
	bool keyCurrentDown = false;
	for (int i = 0; i < 256; i++) {
		if (buffer[i]) {
			keyCurrentDown = true;
		}
	}

	if (keyDown && keyCurrentDown) {
		return true;
	} else {
		keyDown = false;
	}

	if (!keyCurrentDown) {
		return true;
	}

	keyDown = true;
	if (input.IsKeyDown(DIK_ESCAPE)) {
		return false;
	}

	if (input.IsKeyDown(DIK_F1)) {
		return graphics.ToggleFullscreen();
	}

	if (input.IsKeyDown(DIK_F2)) {
		if (focus == FOCUS_RANDOM) {
			focus = FOCUS_TYPING;
			selectionPos = -1;
		} else {
			focus = FOCUS_RANDOM;
			srand(GetTickCount());
			for (int i = 0; i < 63; i++) {
				keyMap[i] = rand() % 26;
			}
		}
		return true;
	}

	if (input.IsKeyDown(DIK_F3)) {
		if (focus == FOCUS_RANDOM) {
			srand(GetTickCount());
			for (int i = 0; i < 63; i++) {
				keyMap[i] = rand() % 26;
			}
		}
		return true;
	}

	if (input.IsKeyDown(DIK_F4)) {
		if (focus == FOCUS_RANDOM) {
			std::ifstream inFile("key.out", std::ios::in);
			int i = 0;
			while (!inFile.eof() && i < 63) {
				inFile >> keyMap[i];
				i++;
			}
			inFile.close();
		}
		return true;
	}

	if (input.IsKeyDown(DIK_BACK)) {
		std::vector<std::vector<int> >::iterator it;
		std::vector<int>::iterator it2;
		switch (focus) {
			case FOCUS_TYPING:
				if (currentTypingPos > 0) {
					currentTypingPos--;
					typingBuffer[currentTypingPos] = -1;
				}	
				break;
			case FOCUS_SELECTION:
				it = wordList.begin();
				it2 = wordPos.begin();
				for(int i = 0; i < selectionPos; i++) {
					++it;
					++it2;
				}


				wordList.erase(it);
				wordPos.erase(it2);

				if (wordList.size()) {
					--selectionPos;
					if (selectionPos < 0) {
						selectionPos = wordList.size() - 1;
					}
				} else {
					selectionPos = -1;
					focus = FOCUS_TYPING;
				}
				break;
			case FOCUS_POSITION:
				focus = FOCUS_SELECTION;
				break;
		}
		return true;
	}

	if (input.IsKeyDown(DIK_DOWN)) {
		switch (focus) {
			case FOCUS_TYPING:
				if (wordList.size()) {
					selectionPos = 0;
					focus = FOCUS_SELECTION;
				}
				break;
			case FOCUS_SELECTION:
				selectionPos++;
				if (selectionPos == wordList.size()) {
					selectionPos = 0;
				}
				break;
			case FOCUS_POSITION:
				if (wordPos[selectionPos] + 17 < cipherText.size()) {
					wordPos[selectionPos] += 17;
				}
				break;
		}
		return true;
	}

	if (input.IsKeyDown(DIK_UP)) {
		switch (focus) {
			case FOCUS_TYPING:
				if (wordList.size()) {
					selectionPos = wordList.size() - 1;
					focus = FOCUS_SELECTION;
				}
				break;
			case FOCUS_SELECTION:
				selectionPos--;
				if (selectionPos < 0) {
					selectionPos = wordList.size() - 1;
				}
				break;
			case FOCUS_POSITION:
				if (wordPos[selectionPos] - 17 >= 0) {
					wordPos[selectionPos] -= 17;
				}
				break;
		}
		return true;
	}

	if (input.IsKeyDown(DIK_LEFT)) {
		switch (focus) {
			case FOCUS_POSITION:
				if (wordPos[selectionPos] - 1 >= 0) {
					wordPos[selectionPos] -= 1;
				}
				break;
		}
		return true;
	}

	if (input.IsKeyDown(DIK_RIGHT)) {
		switch (focus) {
			case FOCUS_POSITION:
				if (wordPos[selectionPos] + wordList[selectionPos].size() < cipherText.size()) {
					wordPos[selectionPos] += 1;
				}
				break;
		}
		return true;
	}

	if (input.IsKeyDown(DIK_RETURN)) {
		switch (focus) {
			case FOCUS_TYPING:
				if (currentTypingPos != 0) {
					CopyTypingBufferToWordList();
					currentTypingPos = 0;
					typingBuffer[currentTypingPos] = -1;
				}	

				if (wordList.size()) {
					focus = FOCUS_SELECTION;
					selectionPos = wordList.size() - 1;
				}
				break;
			case FOCUS_SELECTION:
				focus = FOCUS_POSITION;
				if (wordPos[selectionPos] == -1) {
					wordPos[selectionPos] = 0;
				}
				break;
		}
		return true;
	}

	for (int i = 0; i < 256; i++) {
		int keyToChar;
		switch (focus) {
			case FOCUS_TYPING:
				selectionPos = -1;
				keyToChar = convertKeyToChar(i);
				if (keyToChar != -1 && input.IsKeyDown(i)) {
					UpdateTypingBuffer(keyToChar);
				}		
				break;
			case FOCUS_SELECTION:
				focus = FOCUS_TYPING;
				break;
			case FOCUS_POSITION:
				focus = FOCUS_TYPING;
				break;
		}
	}

	return true;
}

/////////////////////////////////////////////////////////////////////
//							CGameBase::ProcessRenderState
/////////////////////////////////////////////////////////////////////
bool CGameBase::ProcessRenderState () {
	graphics.SetWorld(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 1, 1));
	graphics.SetCamera(D3DXVECTOR3(512, 384, -10), D3DXVECTOR3(512, 384, 0), D3DXVECTOR3(0, 1, 0));
	graphics.SetProjection();

	graphics.clearRenderNodes();
	CalculateKeyMap();
	for (int i = 0; i < 20; i++) {
		for (int k = 0; k < 17; k++) {
			STempRenderNode tempNode;
			tempNode.scale = D3DXVECTOR3(1,1,1);
			int cipherPosition = (i*17) + k;
			int cipherCharacter = cipherText[(i*17) + k];
			if (keyMap[cipherCharacter] >= 0 && keyMap[cipherCharacter] <= 35) {
				tempNode.textureHandle = alphaTextures[keyMap[cipherCharacter]];
				if (conflictingKeyMap[cipherCharacter] != -1) {
					tempNode.color[0] = 1.0f;
					tempNode.color[1] = 0.0f;	
					tempNode.color[2] = 0.0f;
					tempNode.color[3] = 1.0f;
				} else {
					tempNode.color[0] = 0.0f;
					tempNode.color[1] = 0.0f;	
					tempNode.color[2] = 1.0f;
					tempNode.color[3] = 1.0f;
				}
			} else if (currentKeyMap[cipherCharacter] >= 0 && currentKeyMap[cipherCharacter] <= 35) {
				tempNode.textureHandle = alphaTextures[currentKeyMap[cipherCharacter]];
				if (conflictingKeyMap[cipherCharacter] != -1) {
					tempNode.color[0] = 1.0f;
					tempNode.color[1] = 0.0f;	
					tempNode.color[2] = 0.0f;
					tempNode.color[3] = 1.0f;
				} else if (cipherPosition >= wordPos[selectionPos] && cipherPosition <= wordPos[selectionPos] + wordList[selectionPos].size()) {
					tempNode.color[0] = 1.0f;
					tempNode.color[1] = 1.0f;	
					tempNode.color[2] = 0.0f;
					tempNode.color[3] = 1.0f;
					tempNode.scale = D3DXVECTOR3(1.25,1.25,1);
				} else {
					tempNode.color[0] = 0.0f;
					tempNode.color[1] = 1.0f;	
					tempNode.color[2] = 0.0f;
					tempNode.color[3] = 1.0f;
					tempNode.scale = D3DXVECTOR3(1.1,1.1,1);
				}
			} else {
				tempNode.textureHandle = cipherTextures[cipherCharacter];
				tempNode.color[0] = 1.0f;
				tempNode.color[1] = 1.0f;	
				tempNode.color[2] = 1.0f;
				tempNode.color[3] = 1.0f;
			}
			tempNode.pos = D3DXVECTOR3(35 + k * 30, 650 - i * 30, 0);

			graphics.addRenderNode(tempNode);
		}
	}

	int bufferPos = 0;
	int bufferChar = typingBuffer[bufferPos];
	while(bufferChar >= 0 && bufferChar <= 35 && bufferPos < MAX_TYPING_BUFFER) {
		STempRenderNode tempNode;
		tempNode.textureHandle = alphaTextures[bufferChar];
		tempNode.pos = D3DXVECTOR3(600 + bufferPos * 25, 700, 0);
		tempNode.color[0] = 0.8f;
		tempNode.color[1] = 0.8f;	
		tempNode.color[2] = 0.8f;
		tempNode.color[3] = 1.0f;
		graphics.addRenderNode(tempNode);	
		++bufferPos;
		if (bufferPos >= MAX_TYPING_BUFFER) {
			break;
		}
		bufferChar = typingBuffer[bufferPos];
	}

	std::vector<std::vector<int> >::iterator it = wordList.begin();
	int count = 0;
	while (it != wordList.end()) {
		for (int i = 0; i < (*it).size(); i++) {
			STempRenderNode tempNode;
			tempNode.textureHandle = alphaTextures[(*it)[i]];
			tempNode.pos = D3DXVECTOR3(600 + i * 25, 650 - count * 30, 0);
			if (selectionPos == count) {
				tempNode.color[0] = 0.0f;
				tempNode.color[1] = 0.8f;	
				tempNode.color[2] = 0.0f;
				tempNode.color[3] = 1.0f;
			} else {
				tempNode.color[0] = 0.7f;
				tempNode.color[1] = 0.7f;	
				tempNode.color[2] = 0.7f;
				tempNode.color[3] = 1.0f;
			}
			graphics.addRenderNode(tempNode);			
		}
		++count;
		++it;
	}

	for (int i = 0; i < 63; i++) {
		if (keyMap[i] != -1) {
			STempRenderNode tempNode;
			tempNode.textureHandle = alphaTextures[keyMap[i]];
			tempNode.color[0] = 1.0f;
			tempNode.color[1] = 1.0f;	
			tempNode.color[2] = 1.0f;
			tempNode.color[3] = 1.0f;
			tempNode.pos = D3DXVECTOR3(16 + i * 16, 45, 0);
			tempNode.scale = D3DXVECTOR3(.9, .9, 1);
			graphics.addRenderNode(tempNode);
		} else if (currentKeyMap[i] != -1) {
			STempRenderNode tempNode;
			tempNode.textureHandle = alphaTextures[currentKeyMap[i]];
			tempNode.color[0] = 0.0f;
			tempNode.color[1] = 1.0f;	
			tempNode.color[2] = 0.0f;
			tempNode.color[3] = 1.0f;
			tempNode.pos = D3DXVECTOR3(16 + i * 16, 45, 0);
			tempNode.scale = D3DXVECTOR3(.9, .9, 1);
			graphics.addRenderNode(tempNode);
		}
	}
	return true;
}


void CGameBase::UpdateTypingBuffer(int alphaCharacter) {
	if (currentTypingPos >= MAX_TYPING_BUFFER-1) {
		return;
	}

	typingBuffer[currentTypingPos] = alphaCharacter;
	++currentTypingPos;
	typingBuffer[currentTypingPos] = -1;
}
void CGameBase::CopyTypingBufferToWordList() {

	std::vector<int> temp;

	int pos = 0;
	while(typingBuffer[pos] != -1) {
		temp.push_back(typingBuffer[pos]);
		++pos;
		if (pos >= MAX_TYPING_BUFFER) {
			break;
		}
	}

	wordList.push_back(temp);
	wordPos.push_back(-1);
}

void CGameBase::CalculateKeyMap() {

	if (focus == FOCUS_RANDOM) {
		for (int index = 0; index < 63; index++) {
			currentKeyMap[index] = -1;
			conflictingKeyMap[index] = -1;
		}
		return;
	}

	for (int index = 0; index < 63; index++) {
		keyMap[index] = -1;
		currentKeyMap[index] = -1;
		conflictingKeyMap[index] = -1;
	}
	for(int i = 0; i < wordList.size(); i++) {
		if (i != selectionPos) {
			if (wordPos[i] != -1) {
				for (int k = 0; k < wordList[i].size(); k++) {
					int cipherPos = wordPos[i]+k;
					if (cipherPos >= cipherText.size()) {
						break;
					} else if (keyMap[cipherText[cipherPos]] != -1 && keyMap[cipherText[cipherPos]] != wordList[i][k]) {
						conflictingKeyMap[cipherText[cipherPos]] = i;
					} else {
						keyMap[cipherText[cipherPos]] = wordList[i][k];				
					}
				}
			}
		} 
	}

	if (selectionPos != -1 && wordPos[selectionPos] != -1) {
		int i = selectionPos;
		for (int k = 0; k < wordList[i].size(); k++) {
			int cipherPos = wordPos[i]+k;
			if (cipherPos >= cipherText.size()) {
				break;
			} else {
				if (currentKeyMap[cipherText[cipherPos]] != -1 && currentKeyMap[cipherText[cipherPos]] != wordList[i][k]) {
					conflictingKeyMap[cipherText[cipherPos]] = i;
				} 
				
				currentKeyMap[cipherText[cipherPos]] = wordList[i][k];
		
				if (keyMap[cipherText[cipherPos]] != -1 && keyMap[cipherText[cipherPos]] != currentKeyMap[cipherText[cipherPos]]) {
					conflictingKeyMap[cipherText[cipherPos]] = i;
				}
			}
		}
	}
			
}