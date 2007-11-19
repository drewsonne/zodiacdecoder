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
	currentScrollPos = 0; //RNL
	cipherClicked = false; //RNL
	editRandom = false; //RNL
	focus = FOCUS_TYPING;
	wordList.clear();
	wordPos.clear();
	keyWordList.clear(); //RNL
	cipherPos.clear(); //RNL
	savedKeyMap.clear(); //RNL
	selectionPos = -1;  //RNL
	Dictionary.clear();  //RNL
	underlinecounter = 0;  //RNL
	lastClick = 0.0f; //RNL
	firstRandom = true; //RNL
	currentWordHover = -1; //RNL
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
	cipherPos.clear(); //RNL
	savedKeyMap.clear(); //RNL
	Dictionary.clear(); //RNL
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
		savedKeyMap.push_back(-1); //RNL
		currentKeyMap.push_back(-1);
		conflictingKeyMap.push_back(-1);
	}
	// Base class does nothing

	InitializeDictionary(); //RNL

	buttons.Init(graphics);

	ScrollUpButtonId = buttons.addButton(CButton(kScrollBarLeft,kScrollBarTop,kScrollBarWidth,kScrollBarWidth,eButton_UpArrow,NULL));
	ScrollDownButtonId = buttons.addButton(CButton(kScrollBarLeft,kScrollBarTop + kScrollBarHeight - kScrollBarWidth,kScrollBarWidth,kScrollBarWidth,eButton_DownArrow,NULL));
	// Position Exit Button Below the scroll bar
	
	ToggleButtonId = buttons.addButton(CButton(0,0,150,buttonHeight,eButton_Generic,"Toggle Mode"));
	RandomKeyButtonId = buttons.addButton(CButton(151,0,150,buttonHeight,eButton_Generic,"Random Key"));
	LoadFromFileButtonId = buttons.addButton(CButton(302,0,200,buttonHeight,eButton_Generic,"Load Key from File"));
	LoadFromServerButtonId = buttons.addButton(CButton(503,0,200,buttonHeight,eButton_Generic,"Load Key from Server"));
	VoteOnKeyButtonId = buttons.addButton(CButton(704,0,200,buttonHeight,eButton_Generic,"Send Key to Server"));
//	HRHelpButtonId = buttons.addButton(CButton(855,0,80,buttonHeight,eButton_Generic,"Help"));
	ExitButtonId = buttons.addButton(CButton(905,0,graphics.screenState.screenWidth - 905,buttonHeight,eButton_Generic,"Exit"));
	
	
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
	int windowsBorder = 5; //Warning: actually a variable
	int windowsMenuBarHeight = 23; //Warning: actually a variable
	p.x -= (r.left + windowsBorder);
	p.y -= (r.top + windowsMenuBarHeight);
	// The window menu and border actually take up some of our area.  Our area gets 'scruntched' and we have to figure out that ratio to
	// get accurate mouse position over our drawn buttons.
	float actualDrawWidthRatio = (float)(graphics.screenState.windowWidth - windowsBorder * 2) / (float)graphics.screenState.windowWidth;
	float actualDrawHeightRatio = (float)(graphics.screenState.windowHeight - (windowsMenuBarHeight + windowsBorder)) / (float)graphics.screenState.windowHeight ;
	int buttonPressed = buttons.processButtons(p.x / actualDrawWidthRatio, p.y / actualDrawHeightRatio, mouseState.rgbButtons[0]);

	lastClick += dT;

	if (buttonPressed != -1) { //RNL Fixed small error, 0 is a button ID
		if(lastClick < 5.0f) return true;

		lastClick = 0.0f;

		if (buttonPressed == ExitButtonId) {
			// exit the app;
			return false;
		}
		else if (buttonPressed == ScrollUpButtonId){ //RNL
			// scroll up
			if (currentScrollPos > 0) currentScrollPos--;
			if (currentScrollPos < 0) currentScrollPos = 0;
			return true;
		}
		else if (buttonPressed == ScrollDownButtonId){ //RNL
			// scroll down
			if(currentScrollPos + MAX_WORD_LIST_LENGTH < keyWordList.size()) currentScrollPos++;
			return true;
		}
		else if (buttonPressed == ToggleButtonId){
			if (focus == FOCUS_RANDOM) {
				focus = FOCUS_TYPING;
				selectionPos = -1;
				editRandom = false;
				for(int i = 0; i < 63; i++){
					savedKeyMap[i] = keyMap[i];
				}
				for(int i = 0; i < 17; i++) typingBuffer[i] = -1;
			} else {
				focus = FOCUS_RANDOM;
				if(firstRandom){
					firstRandom = false;
					srand(GetTickCount());
					for (int i = 0; i < 63; i++) {
						keyMap[i] = rand() % 26;
					}
				}
				else{
					for(int i = 0; i < 63; i++){
						keyMap[i] = savedKeyMap[i];
					}
				}
				#ifdef WE_HAVE_DICTIONARY
				  makeNewKeyWordList();
				#endif
				currentScrollPos = 0;
			}
			return true;
		}
		else if(buttonPressed == LoadFromServerButtonId){
		if (focus == FOCUS_RANDOM) {
			serverKeys="aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			char serverTmp[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			char* servername = "localhost"; //Server Name
			//Note: KeyMap is a vector of ints, so you can't use that directly
			HWND hwnd2 = GetActiveWindow();
			MessageBox(hwnd2, "Attempting to retrieve key from server. Please ensure you are connected to the internet and have set your firewall to allow traffic over port 10001. Please click OK when you are ready.", "Retrieving key", 0);
			std::ofstream foutq;
			foutq.open("key.out", std::ios::out);
			if (getKeysFromServer(serverTmp)==0){
				for(int i=0; i < 63; i++){
					foutq << serverTmp[i];
				}
			

				//Put something here to say the key was received?
				HWND hwnd1 = GetActiveWindow();
				MessageBox(hwnd1, "Key retrieved! Click OK and press F4.", "Transfer Succeeded", 0);
				/*std::ofstream fout;
				fout.open("key.out", std::ios::out);
				for(int i = 0; i < 63; i++){
					if(serverKeys[i] <= 122 && serverKeys[i] >= 97)
						fout << (char)(serverKeys[i] - 32);
				}
				fout.close();*/
			}
			else {
				//Put something here to say the key was not received, perhaps flag something
				HWND hwnd1 = GetActiveWindow();
				MessageBox(hwnd1, "Key could not be retrieved from server. Check your firewall settings and your internet connection to ensure you are connected and may send over port 10001", "Connection Error", 0);
				
			}
			foutq.close();
			#ifdef WE_HAVE_DICTIONARY
			  makeNewKeyWordList();
            #endif
		}
			/*if (focus == FOCUS_RANDOM) {
				char* servername = "zodiacdecoder.dyndns.org"; //Server Name
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
			}*/
			return true;
		}
		else if(buttonPressed == LoadFromFileButtonId){
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
				currentScrollPos = 0;
			}
			return true;
		}
		else if(buttonPressed == VoteOnKeyButtonId){
			if (focus == FOCUS_RANDOM) {
				//char* servername = "zodiacdecoder.dyndns.org";
				char* servername = "zodiacdecoder.dyndns.org";
				char* port = "10002";
				//Server Name
				
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
				currentScrollPos = 0;
			}
			return true;
		}
		else if(buttonPressed == RandomKeyButtonId){
			if (focus == FOCUS_RANDOM) {
				srand(GetTickCount());
				for (int i = 0; i < 63; i++) {
					keyMap[i] = rand() % 26;
				}
				#ifdef WE_HAVE_DICTIONARY
				  makeNewKeyWordList();
				#endif
				currentScrollPos = 0;
			}
			return true;
		}
	}

	//RNL Check to see if pointer hovers a cipher character
	POINT ap; ap.x = p.x / actualDrawWidthRatio; ap.y = p.y / actualDrawHeightRatio;
	//Just check to see if cursor is in cipher so we don't have to go into the double for-loop all the time
	currentCursorHover = -1;
	cipherClicked = false;
	if( (ap.x > (borderWidth)) && 
		(ap.x < (borderWidth + cipherCharacterWidth * cipherWidthInCharacters)) &&
		(ap.y > (borderWidth + buttonHeight)) &&
		(ap.y < (borderWidth + buttonHeight + cipherCharacterHeight * cipherHeightInCharacters)) &&
		(!editRandom))
	{
		for (int i = 0; i < cipherHeightInCharacters; i++) {
			for (int k = 0; k < cipherWidthInCharacters; k++) { 
				if( (ap.x > (borderWidth + k * cipherCharacterWidth)) && 
					(ap.x < (borderWidth + (k+1) * cipherCharacterWidth)) &&
					(ap.y > (borderWidth + buttonHeight + i * cipherCharacterHeight)) &&
					(ap.y < (borderWidth + buttonHeight + (i+1) * cipherCharacterHeight))){
						currentCursorHover = i * cipherWidthInCharacters + k;
						if (mouseState.rgbButtons[0] && focus == FOCUS_RANDOM) 
						{
							cipherClicked = true; 
							editRandom = true; 
							currentRandomEdit = currentCursorHover;
						}
						break;
				}
			}
			if( currentCursorHover != -1) break;
		}
	}
	else if((ap.x > (borderWidth + kWordListLeft)) && 
			(ap.x < (borderWidth + kWordListLeft + kWordListWidth)) &&
			(ap.y > (borderWidth + kWordListTop )) &&
			(ap.y < (borderWidth + kWordListTop + kWordListHeight)) &&
			(!editRandom))
	{
		for(int i = 0; i < MAX_WORD_LIST_LENGTH; i++){
			if((ap.x > (borderWidth + kWordListLeft)) && 
			   (ap.x < (borderWidth + kWordListLeft + kWordListWidth)) &&
			   (ap.y > (borderWidth + kWordListTop + i * cipherCharacterHeight)) &&
			   (ap.y < (borderWidth + kWordListTop + (i+1) * cipherCharacterHeight)))
			{
				if(focus == FOCUS_RANDOM && i >= keyWordList.size()) break;
				else if(focus != FOCUS_RANDOM && i >= wordList.size()) break;
				currentWordHover = i;
				if (focus == FOCUS_SELECTION) selectionPos = i;
				if (mouseState.rgbButtons[0] && focus != FOCUS_RANDOM) 
				{
					focus = FOCUS_POSITION;
					if (wordPos[i] == -1) {
						wordPos[i] = 0;
					}
					currentWordHover = -1;
				}
				break;
			}
		}
	}
	else currentWordHover = -1;

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
			editRandom = false;
			for(int i = 0; i < 63; i++){
				savedKeyMap[i] = keyMap[i];
			}
			for(int i = 0; i < 17; i++) typingBuffer[i] = -1;
		} else {
			focus = FOCUS_RANDOM;
			if(firstRandom){
				firstRandom = false;
				srand(GetTickCount());
				for (int i = 0; i < 63; i++) {
					keyMap[i] = rand() % 26;
				}
			}
			else{
				for(int i = 0; i < 63; i++){
						keyMap[i] = savedKeyMap[i];
				}
			}
			#ifdef WE_HAVE_DICTIONARY
			  makeNewKeyWordList();
            #endif
			currentScrollPos = 0;
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
			currentScrollPos = 0;
		}
		return true;
	}

	if (input.IsKeyDown(DIK_F4)) {
//		if(focus == FOCUS_TYPING) focus = FOCUS_RANDOM;
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
			currentScrollPos = 0;
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
			currentScrollPos = 0;
		}
		return true;
	}

	//RNL Write to server
	//One problem: this code does not take the delay of sending keys into consideration
	if (input.IsKeyDown(DIK_F7)) {
		if (focus == FOCUS_RANDOM) {
			serverKeys="aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
			char serverTmp[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaac";
			char* servername = "zodiacdecoder.dyndns.org"; //Server Name
			//Note: KeyMap is a vector of ints, so you can't use that directly
			HWND hwnd2 = GetActiveWindow();
			MessageBox(hwnd2, "Attempting to retrieve key from server. Please ensure you are connected to the internet and have set your firewall to allow traffic over port 10001. Please click OK when you are ready.", "Retrieving key", 0);
			std::ofstream foutq;
			foutq.open("key.out", std::ios::out);
			if (getKeysFromServer(serverTmp)==0){
				for(int i=0; i < 64; i++){
					foutq << serverTmp[i];
				}
			

				//Put something here to say the key was received?
				HWND hwnd1 = GetActiveWindow();
				MessageBox(hwnd1, "Key retrieved! Click OK and press F4.", "Transfer Succeeded", 0);
				/*std::ofstream fout;
				fout.open("key.out", std::ios::out);
				for(int i = 0; i < 63; i++){
					if(serverKeys[i] <= 122 && serverKeys[i] >= 97)
						fout << (char)(serverKeys[i] - 32);
				}
				fout.close();*/
			}
			else {
				//Put something here to say the key was not received, perhaps flag something
				HWND hwnd1 = GetActiveWindow();
				MessageBox(hwnd1, "Key could not be retrieved from server. Check your firewall settings and your internet connection to ensure you are connected and may send over port 10001", "Connection Error", 0);
				
			}
			foutq.close();
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
			case FOCUS_RANDOM:
				editRandom = false;
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
			case FOCUS_RANDOM:
				if(currentScrollPos + MAX_WORD_LIST_LENGTH < keyWordList.size()) currentScrollPos++;
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
			case FOCUS_RANDOM:
				if (currentScrollPos > 0) currentScrollPos--;
				if (currentScrollPos < 0) currentScrollPos = 0;
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
			case FOCUS_RANDOM:
				editRandom = false;
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
			case FOCUS_RANDOM:  //RNL
				if(editRandom)
				{
					keyToChar = convertKeyToChar(i);
					if (keyToChar != -1 && input.IsKeyDown(i)) 
					{
						replaceCipherChar(keyToChar);
						editRandom = false;
					}
				}
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
	DrawBorder(0,buttonHeight,cipherBorderWidthTotal,cipherBorderHeightTotal,borderWidth, kBorderColor);

	//RNL Logo
	STempRenderNode logoNode;
	logoNode.textureHandle = miscTextures[0];
	logoNode.color = kCodedCipherColor;
	logoNode.scale = D3DXVECTOR3(256,256,1);
	logoNode.pos = D3DXVECTOR3(kWordListLeft + 250,kScrollBarTop + kScrollBarHeight,1);
	graphics.addRenderNode(logoNode);

    //RNL Bottom Border thing
	STempRenderNode smallBorderNode;
	smallBorderNode.textureHandle = borderTextures[1];
	smallBorderNode.color = kBorderColor;
	smallBorderNode.scale = D3DXVECTOR3(graphics.screenState.screenWidth,borderWidth,0);
	smallBorderNode.pos = D3DXVECTOR3(0,kScrollBarTop + kScrollBarHeight, 1);
	graphics.addRenderNode(smallBorderNode);

	for (int i = 0; i < cipherHeightInCharacters; i++) {
		for (int k = 0; k < cipherWidthInCharacters; k++) {
			STempRenderNode tempNode;
			tempNode.scale = D3DXVECTOR3(cipherCharacterWidth,cipherCharacterHeight,1);
			int cipherPosition = (i*cipherWidthInCharacters) + k;
			int cipherCharacter = cipherText[(i*cipherWidthInCharacters) + k];
			if (keyMap[cipherCharacter] >= 0 && keyMap[cipherCharacter] <= 35) {
				tempNode.textureHandle = alphaTextures[keyMap[cipherCharacter]];
				if (conflictingKeyMap[cipherCharacter] != -1 || 
					(editRandom && 
					 currentRandomEdit >= 0 && 
					 cipherCharacter == cipherText[currentRandomEdit])
				) {
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
			tempNode.pos = D3DXVECTOR3(borderWidth + k * cipherCharacterWidth, borderWidth + buttonHeight + i * cipherCharacterHeight, 0);

			graphics.addRenderNode(tempNode);

		}
	}



	DrawBorder(cipherBorderWidthTotal, buttonHeight, sideBarWidth, typedCharacterHeight + typedBufferPadHeight, borderWidth, kBorderColor);
	const float kTypingLeft = cipherBorderWidthTotal + (typedBufferPadHeight/ 2);
	const float kTypingTop = (typedBufferPadHeight/ 2) + buttonHeight;

	int bufferPos = 0;
	int spacePos = -1;
	if(focus == FOCUS_RANDOM){ //RNL Word Count, this will erase the typing buffer on random key focus
		for(int i = 0; i < 17; i++) typingBuffer[i] = -1;
		int totalWords = keyWordList.size();
		int tkWords = totalWords/10000; //Most likely impossible, mostly a failsafe
		int kWords = totalWords/1000; //Very unlikely
		int hWords = totalWords/100; //Will happen on a good key
		int tWords = totalWords/10; 
		int oWords = totalWords%10;
		if(tkWords > 0) {
			typingBuffer[bufferPos++] = tkWords + 25;
		}
		if(kWords > 0 || tkWords > 0) {
			if(kWords != 0)	typingBuffer[bufferPos++] = kWords + 25;
			else typingBuffer[bufferPos++] = 35;
		}
		if(hWords > 0 || kWords > 0 || tkWords > 0) {
			if(hWords != 0)	typingBuffer[bufferPos++] = hWords + 25;
			else typingBuffer[bufferPos++] = 35;
		}
		if(tWords > 0 || hWords > 0 || kWords > 0 || tkWords > 0) {
			if(tWords != 0)	typingBuffer[bufferPos++] = tWords + 25;
			else typingBuffer[bufferPos++] = 35;
		}
		if(oWords != 0)	typingBuffer[bufferPos++] = oWords + 25;
		else typingBuffer[bufferPos++] = 35;

		//Add a useless character for a space, then add "words"
		spacePos = bufferPos;
		typingBuffer[bufferPos++] = 0;
		typingBuffer[bufferPos++] = 22;
		typingBuffer[bufferPos++] = 14;
		typingBuffer[bufferPos++] = 17;
		typingBuffer[bufferPos++] = 3;
		typingBuffer[bufferPos] = 18;

	}
	bufferPos = 0;
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
	if( (bufferPos < MAX_TYPING_BUFFER - 1) && 
		(underlinecounter < 15) &&
		(focus != FOCUS_RANDOM) &&
		(!editRandom)){
		//RNL fancy flashing underline
		DrawColoredQuad( kTypingLeft + 3 + bufferPos * typedCharacterWidth, kTypingTop + typedCharacterWidth , typedCharacterWidth - 6, 2, kTypedCursorCharacterColor);
	}
	if(underlinecounter >= 30){
		underlinecounter = 0;
	}
	if(spacePos != -1){
		SRenderNodeColor SpaceColor = {0.0f,0.0f,0.0f,1.0f};
		DrawColoredQuad( kTypingLeft + spacePos * typedCharacterWidth, kTypingTop, 
						 typedCharacterWidth, typedCharacterHeight, SpaceColor);
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
			if(count != MAX_WORD_LIST_LENGTH - 1) DrawColoredQuad( kWordListLeft + borderWidth, ( kWordListTop + count * kWordListEntryHeight) + typedCharacterHeight , kWordListWidth - borderWidth * 2, 2, kWordListSeperatorLineColor);
			++count;
			++it;
		}
	}
	else{
#ifdef WE_HAVE_DICTIONARY
		std::vector<std::vector<int> >::iterator it = keyWordList.begin();
			int count = 0;
			if (keyWordList.size() > MAX_WORD_LIST_LENGTH){
				for(int j = 0; (j < currentScrollPos) && (it != keyWordList.end()); j++) ++it;
			}
			while (it != keyWordList.end() && count < MAX_WORD_LIST_LENGTH) {
				for (int i = 0; i < (*it).size(); i++) {
					STempRenderNode tempNode;
					tempNode.scale = D3DXVECTOR3(typedCharacterWidth, typedCharacterHeight, 1);
					tempNode.textureHandle = alphaTextures[(*it)[i]];
					tempNode.pos = D3DXVECTOR3(kWordListLeft + i * typedCharacterWidth, kWordListTop + count * 30, 0);
					tempNode.color = kWordListSelectedCharacterColor;
					graphics.addRenderNode(tempNode);			
				}
				if(count != MAX_WORD_LIST_LENGTH - 1)DrawColoredQuad( kWordListLeft + borderWidth, ( kWordListTop + count * kWordListEntryHeight) + typedCharacterHeight , kWordListWidth - borderWidth * 2, 2, kWordListSeperatorLineColor);
				++count;
				++it;
			}
#endif
	}

	//RNL Shaded Squares and Word
	SRenderNodeColor shadeColor = {1.0f,1.0f,1.0f,0.4f};
	SRenderNodeColor editColor = {0.0f,0.0f,0.0f,1.0f};
	if( editRandom ){
		DrawColoredQuad( borderWidth + (currentRandomEdit % cipherWidthInCharacters) * cipherCharacterWidth, 
			             borderWidth + buttonHeight + (int)(currentRandomEdit / cipherWidthInCharacters) * cipherCharacterHeight,
						 cipherCharacterWidth, cipherCharacterHeight, editColor);
		DrawColoredQuad( borderWidth + (currentRandomEdit % cipherWidthInCharacters) * cipherCharacterWidth + 3, 
			             borderWidth + buttonHeight + (int)((currentRandomEdit / cipherWidthInCharacters) + 1 ) * cipherCharacterHeight - 3, 
						 cipherCharacterWidth - 6, 2, kTypedCursorCharacterColor);
	}
	else if(currentCursorHover != -1){
		DrawColoredQuad( borderWidth + (currentCursorHover % cipherWidthInCharacters) * cipherCharacterWidth, 
			             borderWidth + buttonHeight + (int)(currentCursorHover / cipherWidthInCharacters) * cipherCharacterHeight,
						 cipherCharacterWidth, cipherCharacterHeight, shadeColor);
	}
	else if(currentWordHover != -1 && focus == FOCUS_RANDOM){
		//Highlight Word on List
		DrawColoredQuad( borderWidth + kWordListLeft, borderWidth + kWordListTop + currentWordHover * (cipherCharacterHeight),
						 kWordListWidth - borderWidth * 2, cipherCharacterHeight - 10, shadeColor);
			//Highlight Word on Cipher
			std::vector<std::vector<int> >::iterator it = keyWordList.begin();
			std::vector<int>::iterator pt = cipherPos.begin();
	
			for(int j = 0; (j < currentWordHover + currentScrollPos) && (it != keyWordList.end()); j++) {++it; ++pt;}
			for (int i = 0; i < (*it).size(); i++) { //(*pt) was listedWord
				DrawColoredQuad( borderWidth + (((*pt)+i) % cipherWidthInCharacters) * cipherCharacterWidth, 
					 borderWidth + buttonHeight + (int)(((*pt)+i) / cipherWidthInCharacters) * cipherCharacterHeight,
					 cipherCharacterWidth, cipherCharacterHeight, shadeColor);		
			}
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
			if(editRandom && i == cipherText[currentRandomEdit])	tempNode.color = kConflictingCipherColor;
			else													tempNode.color = kDecodedCipherColor;
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
	cipherPos.clear();

	string solution = "";
	string word = "";

	for(int i = 0; i < 340; i++){
		solution += (char)(keyMap[cipherText[i]]) + 'a';

	}

	for(int c = 0; c < 339; c++)
	{
		for(int s = 3; (s <= 12) && (s + c <= 340); s++){
			word = solution.substr(c, s);
			if(Dictionary.GetWordScore(Dictionary, word) != 0) {
				CopyKeyWordToKeyWordList(word);
				cipherPos.push_back(c);
				fout << stringcount++ << ": " << word << std::endl;
				fout << keyWordList.size() << std::endl;
			}
//			if (keyWordList.size() >= MAX_WORD_LIST_LENGTH) break;
		}
//		if (keyWordList.size() >= MAX_WORD_LIST_LENGTH) break;
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

void CGameBase::replaceCipherChar(int theChar) 
{
	int oldCharPos = cipherText[currentRandomEdit];
	keyMap[oldCharPos] = theChar;
	#ifdef WE_HAVE_DICTIONARY
		makeNewKeyWordList();
	#endif
	currentScrollPos = 0;
}
