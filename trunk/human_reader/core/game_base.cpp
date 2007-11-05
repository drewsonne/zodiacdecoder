#include "game_base.h"
#include <fstream>
#include <string>
#include <iostream>
//#include "Stl.h"
//#include "hash_fun.h"
#include "hash_table.h"

//RNL Uncomment the below code when the server is running
#define WE_HAVE_ZODIAC_SERVER

//RNL Uncomment the below code when this code can access the dictionary
#define WE_HAVE_DICTIONARY

#ifdef WE_HAVE_ZODIAC_SERVER
#include "Client.h"
//#include "../../SuperClient/Client.h"
//#include "../../SuperClient/ZodiacClient.h"
#endif 

//#ifdef WE_HAVE_DICTIONARY
//#include "../../zDecoder/Scorer.h" //RNL NOTE: need access to Dictionary
//#endif 

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
	borderTextures.clear();  //RNL
	miscTextures.clear(); //RNL
	typingBuffer[0] = -1;
	currentTypingPos = 0;
	focus = FOCUS_TYPING;
	wordList.clear();
	wordPos.clear();
	keyWordList.clear(); //RNL
	selectionPos = -1;
	Dictionary.clear();
	underlinecounter = 0;
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
	borderTextures.clear();  //RNL
	miscTextures.clear(); //RNL
	keyMap.clear();
	currentKeyMap.clear();
	conflictingKeyMap.clear();
	keyWordList.clear(); //RNL
	Dictionary.clear();
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

	//RNL Border images
	for (int i = 0; i <= 7; i++) {
		sprintf_s(fileName,256,"%s/border/border.%d.png",graphics.texturePath.c_str(),i);
		TextureHandle tempHandle = graphics.getTexture(fileName);
		if(tempHandle.isNull()) {
			return false;
		}

		if (!graphics.cacheTexture(tempHandle)) {
			assert(false);
			return false;
		}

		borderTextures.push_back(tempHandle);
	}

	//RNL Misc images
	sprintf_s(fileName,256,"%s/stock/Logo.png",graphics.texturePath.c_str());
	TextureHandle miscHandle = graphics.getTexture(fileName);
	if(miscHandle.isNull()) {
		return false;
	}

	if (!graphics.cacheTexture(miscHandle)) {
		assert(false);
		return false;
	}
	miscTextures.push_back(miscHandle);
	sprintf_s(fileName,256,"%s/stock/white.png",graphics.texturePath.c_str());
	miscHandle = graphics.getTexture(fileName);
	if(miscHandle.isNull()) {
		return false;
	}

	if (!graphics.cacheTexture(miscHandle)) {
		assert(false);
		return false;
	}
	miscTextures.push_back(miscHandle);
	miscWhiteTextureIndex = miscTextures.size() - 1;

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

	InitializeDictionary(); //RNL

	buttons.Init(graphics);
	ScrollUpButtonId = buttons.addButton(CButton(kScrollBarLeft,kScrollBarTop,kScrollBarWidth,kScrollBarWidth,eButton_UpArrow,NULL));
	ScrollDownButtonId = buttons.addButton(CButton(kScrollBarLeft,kScrollBarTop + kScrollBarHeight - kScrollBarWidth,kScrollBarWidth,kScrollBarWidth,eButton_DownArrow,NULL));
	// Position Exit Button Below the scroll bar
	ExitButtonId = buttons.addButton(CButton(kWordListLeft,kScrollBarTop + kScrollBarHeight,100,30,eButton_Generic,"Exit"));
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

	// BIG FUCKING KLUDGE.
	// Figure out our mouse position within our drawing area.
	DIMOUSESTATE mouseState;
	input.getMouseState(mouseState);
	int x = mouseState.lX;
	int y = mouseState.lY;
	POINT p;
	input.getMousePos(p);
	RECT r;
	graphics.getClientRect(r);
	int windowsBorder = 5;
	int windowsMenuBarHieght = 23;
	p.x -= (r.left + windowsBorder);
	p.y -= (r.top + windowsMenuBarHieght);
	// The window menu and border actually take up some of our area.  Our area gets 'scruntched' and we have to figure out that ratio to
	// get accurate mouse position over our drawn buttons.
	float actualDrawWidthRatio = (float)(graphics.screenState.windowWidth - windowsBorder * 2) / (float)graphics.screenState.windowWidth;
	float actualDrawHeightRatio = (float)(graphics.screenState.windowHeight - (windowsMenuBarHieght + windowsBorder)) / (float)graphics.screenState.windowHeight ;
	IndexType buttonPressed = buttons.processButtons(p.x / actualDrawWidthRatio, p.y / actualDrawHeightRatio, mouseState.rgbButtons[0]);

	if (buttonPressed != 0) {
		if (buttonPressed == ExitButtonId) {
			// exit the app;
			return false;
		}
	}
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

	// Full Screen seems broken
	/*if (input.IsKeyDown(DIK_F1)) {
		return graphics.ToggleFullscreen();
	}*/

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
			#ifdef WE_HAVE_DICTIONARY
			  makeNewKeyWordList();
            #endif
		}
		return true;
	}

	if (input.IsKeyDown(DIK_F3)) {
		if (focus == FOCUS_RANDOM) {
			srand(GetTickCount());
			for (int i = 0; i < 63; i++) {
				keyMap[i] = rand() % 26;
			}
			#ifdef WE_HAVE_DICTIONARY
			  makeNewKeyWordList();
            #endif
		}
		return true;
	}

	if (input.IsKeyDown(DIK_F4)) {
		if(focus == FOCUS_TYPING) focus = FOCUS_RANDOM;
		if (focus == FOCUS_RANDOM) {
			std::ifstream inFile;
			string solution ="";
			inFile.open("key.out");

			
			int i = 0;
			char inputchar = 0;
			while (!inFile.eof() && i < 63) {
				inputchar = inFile.get();
				keyMap[i] = (int)(inputchar - 'A') ;
				i++;
			}
			
			inFile.close();
			
			#ifdef WE_HAVE_DICTIONARY
			  makeNewKeyWordList();
			#endif
		}

		return true;
	}

	/*
	//RNL Read from standard input
	if (input.IsKeyDown(DIK_F5)) {
		if (focus == FOCUS_RANDOM) {
			std::string inputstring;
			std::getline(std::cin, inputstring);  //Is there a version for c-strings?
			const char* inputstringchars = inputstring.std::string::c_str(); //Does this work? Cannot test.
			for(int i = 0; i < 63; i++) {
				//WARNING: Assumes keyMap and the input c-string are in the same format
				//Note: KeyMap is a vector of ints, so you can't use that directly
				keyMap[i] = (int)(inputstringchars[i++] - 'A'); 
			}
			#ifdef WE_HAVE_DICTIONARY
			  makeNewKeyWordList();
            #endif
		}
		return true;
	}

*/

#ifdef WE_HAVE_ZODIAC_SERVER
	//RNL Write to server
	//One problem: this code does not take the delay of sending keys into consideration
	if (input.IsKeyDown(DIK_F6)) {
		if (focus == FOCUS_RANDOM) {
			char* servername = "zodiacdecoder.dyndns.org";
			char* port = "10002";
			//Server Name
			//WARNING: The following assumes keyMap is in same format as the server's key map
			//Note: KeyMap is a vector of ints, so you can't use that directly
			char charKeyMap[63];
			for(int i = 0;i<63;i++){
				charKeyMap[i] = (char)keyMap[i] + 'A';
			}
						HWND hwnd2 = GetActiveWindow();
			MessageBox(hwnd2, "Attempting to send key to server. Please ensure you are connected to the internet and have set your firewall to allow traffic over port 10002. Please click OK when you are ready.", "Sending key", 0);
			if (sendToServer(servername, port, charKeyMap)==0){

				//Put something here to say the key was sent, perhaps flag something
				HWND hwnd1 = GetActiveWindow();
				MessageBox(hwnd1, "Your good key was sent!", "Transfer Succeeded", 0);
				
			}
			else {
				HWND hwnd1 = GetActiveWindow();
				MessageBox(hwnd1, "Key could not be sent to server. Check your firewall settings and your internet connection to ensure you are connected and may send over port 10002", "Connection Error", 0);
				//Put something here to say the key was not sent, perhaps flag something
			} 
			#ifdef WE_HAVE_DICTIONARY
			  makeNewKeyWordList();
            #endif
		}
		return true;
	}

	//RNL Write to server
	//One problem: this code does not take the delay of sending keys into consideration
	if (input.IsKeyDown(DIK_F7)) {
		if (focus == FOCUS_RANDOM) {
			char* servername = "zodiacdecoder.dyndns.org"; //Server Name
			//WARNING: The following assumes keyMap is in same format as the server's key map
			//Note: KeyMap is a vector of ints, so you can't use that directly
			HWND hwnd2 = GetActiveWindow();
			MessageBox(hwnd2, "Attempting to retrieve key from server. Please ensure you are connected to the internet and have set your firewall to allow traffic over port 10001. Please click OK when you are ready.", "Retrieving key", 0);
			if (getKeysFromServer(serverKeys)==0){ 
				//Put something here to say the key was received?
				HWND hwnd1 = GetActiveWindow();
				MessageBox(hwnd1, "Key retrieved! Click OK and press F4.", "Transfer Succeeded", 0);
				std::ofstream fout;
				fout.open("key.out", std::ios::out);
				for(int i = 0; i < 63; i++){
					if(serverKeys[i] <= 122 && serverKeys[i] >= 97)
						fout << (char)(serverKeys[i] - 32);
				}
				fout.close();
			}
			else {
				//Put something here to say the key was not received, perhaps flag something
				HWND hwnd1 = GetActiveWindow();
				MessageBox(hwnd1, "Key could not be retrieved from server. Check your firewall settings and your internet connection to ensure you are connected and may send over port 10001", "Connection Error", 0);
				
			}
			#ifdef WE_HAVE_DICTIONARY
			  makeNewKeyWordList();
            #endif
		}
		return true;
	}

#endif

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
//							CGameBase::DrawBorder
/////////////////////////////////////////////////////////////////////
// A helper function to draw borders around things.
void CGameBase::DrawBorder(float left, float top, float width, float height, float borderWidth, SRenderNodeColor color) {
	STempRenderNode borderNode;

	// Calculate the edge width and heights
	const float vEdgeWidth = borderWidth;
	const float vEdgeHeight = height - borderWidth * 2;
	const float hEdgeWidth = width - borderWidth * 2;
	const float hEdgeHeight = borderWidth;

	//--------------------------------------------------------
	// Do the four corners
	borderNode.scale = D3DXVECTOR3(borderWidth,borderWidth,1);
	borderNode.color = color;

	//Top Left
	borderNode.textureHandle = borderTextures[0];
	borderNode.pos = D3DXVECTOR3(left,top,0);
	graphics.addRenderNode(borderNode);

	//Top Right
	borderNode.textureHandle = borderTextures[2];
	borderNode.pos = D3DXVECTOR3(left + hEdgeWidth + borderWidth,top,0);
	graphics.addRenderNode(borderNode);

	//Bottom Left
	borderNode.textureHandle = borderTextures[5];
	borderNode.pos = D3DXVECTOR3(left, top + vEdgeHeight + borderWidth, 0);
	graphics.addRenderNode(borderNode);

	//Bottom Right
	borderNode.textureHandle = borderTextures[7];
	borderNode.pos = D3DXVECTOR3(left + hEdgeWidth + borderWidth, top + vEdgeHeight + borderWidth, 0);
	graphics.addRenderNode(borderNode);

	//--------------------------------------------------------
	// Do the edge rails

	borderNode.scale = D3DXVECTOR3(hEdgeWidth,borderWidth,1);
	// Top Rail
	borderNode.textureHandle = borderTextures[1];
	borderNode.pos = D3DXVECTOR3(left + borderWidth,top,0);
	graphics.addRenderNode(borderNode);

	// Bottom Rail
	borderNode.textureHandle = borderTextures[6];
	borderNode.pos = D3DXVECTOR3(left + borderWidth,top + vEdgeHeight + borderWidth,0);
	graphics.addRenderNode(borderNode);

	borderNode.scale = D3DXVECTOR3(borderWidth,vEdgeHeight,1);
	// Left Rail
	borderNode.textureHandle = borderTextures[3];
	borderNode.pos = D3DXVECTOR3(left,top + borderWidth,0);
	graphics.addRenderNode(borderNode);

	// Right Rail
	borderNode.textureHandle = borderTextures[4];
	borderNode.pos = D3DXVECTOR3(left + hEdgeWidth + borderWidth,top + borderWidth,0);
	graphics.addRenderNode(borderNode);
}
/////////////////////////////////////////////////////////////////////
//							CGameBase::DrawColoredQuad
/////////////////////////////////////////////////////////////////////
// A helper function to colored rectangles
void CGameBase::DrawColoredQuad(float left, float top, float width, float height, SRenderNodeColor color) {
		STempRenderNode rectangleNode;
		rectangleNode.color = color;
		rectangleNode.pos = D3DXVECTOR3(left, top, 0);
		rectangleNode.scale = D3DXVECTOR3(width,height,1);
		rectangleNode.textureHandle = miscTextures[miscWhiteTextureIndex];
		graphics.addRenderNode(rectangleNode);
}
/////////////////////////////////////////////////////////////////////
//							CGameBase::ProcessRenderState
/////////////////////////////////////////////////////////////////////
bool CGameBase::ProcessRenderState () {


	graphics.SetWorld(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 1, 1));
	graphics.SetCamera(D3DXVECTOR3(512, 384, 10), D3DXVECTOR3(512, 384, 0), D3DXVECTOR3(0, -1, 0));
	graphics.SetProjection();

	graphics.clearRenderNodes();

	CalculateKeyMap();

	//RNL Border Placement
	DrawBorder(0,0,cipherBorderWidthTotal,cipherBorderHeightTotal,borderWidth, kBorderColor);

	for (int i = 0; i < cipherHeightInCharacters; i++) {
		for (int k = 0; k < cipherWidthInCharacters; k++) {
			STempRenderNode tempNode;
			tempNode.scale = D3DXVECTOR3(cipherCharacterWidth,cipherCharacterHeight,1);
			int cipherPosition = (i*cipherWidthInCharacters) + k;
			int cipherCharacter = cipherText[(i*cipherWidthInCharacters) + k];
			if (keyMap[cipherCharacter] >= 0 && keyMap[cipherCharacter] <= 35) {
				tempNode.textureHandle = alphaTextures[keyMap[cipherCharacter]];
				if (conflictingKeyMap[cipherCharacter] != -1) {
					tempNode.color= kConflictingCipherColor;
				} else {
					tempNode.color = kDecodedCipherColor;
				}
			} else if (currentKeyMap[cipherCharacter] >= 0 && currentKeyMap[cipherCharacter] <= 35) {
				tempNode.textureHandle = alphaTextures[currentKeyMap[cipherCharacter]];
				if (conflictingKeyMap[cipherCharacter] != -1) {
					tempNode.color = kConflictingCipherColor;
				} else if (cipherPosition >= wordPos[selectionPos] && cipherPosition < wordPos[selectionPos] + wordList[selectionPos].size()) {
					//RNL fixed a small bug in the above condition
					tempNode.color = kMovingWordColor;
					tempNode.scale = D3DXVECTOR3(cipherCharacterWidth,cipherCharacterHeight,1);
					
				} else {
					tempNode.color = kDecodedCipherColor;
					tempNode.scale = D3DXVECTOR3(cipherCharacterWidth * 1.1,cipherCharacterHeight * 1.1,1);
				}
			} else {
				tempNode.textureHandle = cipherTextures[cipherCharacter];
				tempNode.color = kCodedCipherColor;
			}
			tempNode.pos = D3DXVECTOR3(borderWidth + k * cipherCharacterWidth, borderWidth + i * cipherCharacterHeight, 0);

			graphics.addRenderNode(tempNode);

		}
	}

	DrawBorder(cipherBorderWidthTotal, 0, sideBarWidth, typedCharacterHeight + typedBufferPadHeight, borderWidth, kBorderColor);
	const float kTypingLeft = cipherBorderWidthTotal + (typedBufferPadHeight/ 2);
	const float kTypingTop = (typedBufferPadHeight/ 2);

	int bufferPos = 0;
	int bufferChar = typingBuffer[bufferPos];
	while(bufferChar >= 0 && bufferChar <= 35 && bufferPos < MAX_TYPING_BUFFER) {
		STempRenderNode tempNode;
		tempNode.textureHandle = alphaTextures[bufferChar];
		tempNode.pos = D3DXVECTOR3(kTypingLeft + bufferPos * typedCharacterWidth, kTypingTop, 0);
		tempNode.scale = D3DXVECTOR3(typedCharacterWidth, typedCharacterHeight, 1);
		tempNode.color = kTypedCharacterColor;
		graphics.addRenderNode(tempNode);	
		++bufferPos;
		if (bufferPos >= MAX_TYPING_BUFFER) {
			break;
		}
		bufferChar = typingBuffer[bufferPos];
	}

	underlinecounter++;
	if(bufferPos < MAX_TYPING_BUFFER - 1 && underlinecounter < 15){
		//RNL fancy flashing underline
		DrawColoredQuad( kTypingLeft + bufferPos * typedCharacterWidth, kTypingTop + typedCharacterWidth , typedCharacterWidth, 2, kTypedCursorCharacterColor);
	}
	if(underlinecounter >= 30){
		underlinecounter = 0;
	}

//RNL changed this to display two different word lists depending on focus
	DrawBorder (kWordListLeft, kWordListTop, kWordListWidth, kWordListHeight, borderWidth, kBorderColor);

	if(focus != FOCUS_RANDOM){
		std::vector<std::vector<int> >::iterator it = wordList.begin();
		int count = 0;
		while (it != wordList.end()) {
			for (int i = 0; i < (*it).size(); i++) {
				STempRenderNode tempNode;
				tempNode.scale = D3DXVECTOR3(typedCharacterWidth, typedCharacterHeight, 1);
				tempNode.textureHandle = alphaTextures[(*it)[i]];
				tempNode.pos = D3DXVECTOR3(kWordListLeft + i * typedCharacterWidth, kWordListTop + count * kWordListEntryHeight, 0);
				if (selectionPos == count) {
					tempNode.color = kWordListSelectedCharacterColor;
				} else {
					tempNode.color = kWordListCharacterColor;
				}
				graphics.addRenderNode(tempNode);			
			}
			// Draw A line seperating entries
			DrawColoredQuad( kWordListLeft + borderWidth, ( kWordListTop + count * kWordListEntryHeight) + typedCharacterHeight , kWordListWidth - borderWidth * 2, 2, kWordListSeperatorLineColor);
			++count;
			++it;
		}
	}
	else{
#ifdef WE_HAVE_DICTIONARY
		std::vector<std::vector<int> >::iterator it = keyWordList.begin();
			int count = 0;
			while (it != keyWordList.end()) {
				for (int i = 0; i < (*it).size(); i++) {
					STempRenderNode tempNode;
					tempNode.scale = D3DXVECTOR3(typedCharacterWidth, typedCharacterHeight, 1);
					tempNode.textureHandle = alphaTextures[(*it)[i]];
					tempNode.pos = D3DXVECTOR3(kWordListLeft + i * typedCharacterWidth, kWordListTop + count * 30, 0);
					tempNode.color = kWordListSelectedCharacterColor;
					graphics.addRenderNode(tempNode);			
				}
				DrawColoredQuad( kWordListLeft + borderWidth, ( kWordListTop + count * kWordListEntryHeight) + typedCharacterHeight , kWordListWidth - borderWidth * 2, 2, kWordListSeperatorLineColor);
				++count;
				++it;
			}
#endif
	}

	
	//RNL Cipher Image Placement
	for(int i=0;i<63;i++){
		STempRenderNode tempNode;
		tempNode.textureHandle = cipherTextures[i];
		tempNode.color = kCodedCipherColor;
		tempNode.pos = D3DXVECTOR3((cipherImageWidth  / 2.0f)+ i * cipherImageWidth, graphics.screenState.screenHeight - cipherImageHeight * 2, 0);
		tempNode.scale = D3DXVECTOR3(cipherImageWidth, cipherImageHeight, 1);
		graphics.addRenderNode(tempNode);
	}

	for (int i = 0; i < 63; i++) {
		if (keyMap[i] != -1) {
			STempRenderNode tempNode;
			tempNode.textureHandle = alphaTextures[keyMap[i]];
			tempNode.color = kDecodedCipherColor;
			tempNode.pos = D3DXVECTOR3((cipherImageWidth  / 2.0f)+ i * cipherImageWidth, graphics.screenState.screenHeight - cipherImageHeight, 0);
			tempNode.scale = D3DXVECTOR3(cipherImageWidth, cipherImageHeight, 1);  //RNL Scaled down
			graphics.addRenderNode(tempNode);
		} else if (currentKeyMap[i] != -1) {
			STempRenderNode tempNode;
			tempNode.textureHandle = alphaTextures[currentKeyMap[i]];
			tempNode.color = kTypedCharacterColor;
			tempNode.pos = D3DXVECTOR3((cipherImageWidth  / 2.0f)+ i * cipherImageWidth, graphics.screenState.screenHeight - cipherImageHeight, 0);
			tempNode.scale = D3DXVECTOR3(cipherImageWidth, cipherImageHeight, 1);  //RNL Scaled down
			graphics.addRenderNode(tempNode);
		}
	}

	buttons.renderButtons(graphics);
	char fileName[256];
	sprintf_s(fileName,256,"%s/stock/white.png",graphics.texturePath.c_str());
	TextureHandle whiteTexture = graphics.getTexture(fileName);
	STempRenderNode tempNode;	
	tempNode.color = kWhiteColor;
	tempNode.pos = D3DXVECTOR3(100,100,0);
	tempNode.scale = D3DXVECTOR3(5,3,1);
	tempNode.textureHandle = whiteTexture;

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

	if (wordList.size() == MAX_WORD_LIST_LENGTH) return;
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

void CGameBase::makeNewKeyWordList(){

#ifdef WE_HAVE_DICTIONARY

	std::ofstream fout;
    fout.open("test.txt", std::ios::out);
	int stringcount = 0;
	keyWordList.clear();

	string solution = "";
	string word = "";

	for(int i = 0; i < 340; i++){
		solution += (char)(keyMap[cipherText[i]]) + 'a';

	}

	for(int c = 0; c < 339; c++)
	{
		for(int s = 3; s <= 12; s++){
			word = solution.substr(c, s);
			if(Dictionary.GetWordScore(Dictionary, word) != 0) {
				CopyKeyWordToKeyWordList(word);
				fout << stringcount++ << ": " << word << std::endl;
				fout << keyWordList.size() << std::endl;
			}
			if (keyWordList.size() >= MAX_WORD_LIST_LENGTH) break;
		}
		if (keyWordList.size() >= MAX_WORD_LIST_LENGTH) break;
	}

    fout.close();

#endif

}

void CGameBase::CopyKeyWordToKeyWordList(string s){

#ifdef WE_HAVE_DICTIONARY

	std::vector<int> temp;

	int pos = 0;
	while(pos < s.size()){
		temp.push_back((int)(s[pos] - 'a'));
		++pos;
	}

	keyWordList.push_back(temp);
	temp.clear();
#endif

}

void CGameBase::InitializeDictionary()
{
	int Scores[1001];
	for (int i = 0; i < 1001; i++) 
	{								
	  Scores[i] = i;
	}

	Dictionary.FillDictionary(Dictionary, "words.txt", Scores, 1000); 

}