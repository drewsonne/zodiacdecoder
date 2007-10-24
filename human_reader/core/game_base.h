#ifndef __GAME_BASE__
#define __GAME_BASE__
/////////////////////////////////////////////////////////////////////
//	game_base.h
//	Initial Version : HAM
//
//	class CGameBase	:
//		handles project independent game systems and initialization
/////////////////////////////////////////////////////////////////////

#include "graphics_system.h"
#include "input_system.h"
#include "system_base.h"
#include "string.h"
#include <vector>

enum EControlFocus {
	FOCUS_TYPING,
	FOCUS_SELECTION,
	FOCUS_POSITION,
	FOCUS_RANDOM
};
// TODO - we would like this to be a pure base class
// projects would derive a game class from CGameBase
#define MAX_TYPING_BUFFER 17
class CGameBase : public CSystemBase
{
public:

	// --------------------------------------------------------------
	//													   Structures
	struct SGameBaseInitParams {
		SGameBaseInitParams (){
			windowHandle = 0;
			instance = 0;

		};
		HWND windowHandle;
		HINSTANCE instance;
	};

	CGameBase();
	virtual ~CGameBase();

	// --------------------------------------------------------------
	//										 Public: Member functions
	/// ProcessGame should essentially be called once a frame by the
	/// application.  
	/// Returns true if the game is running
	/// Returns false if the game has stopped
	bool Process();

	// --------------------------------------------------------------
	//								Public: Member flags and switches
	/// isActive flag tells us if our game is active or has
	/// focus.  isActive should be true when our game app has input 
	/// focus and is in the foreground
	bool isActive;
	bool fatalError;
	bool isMinimized;
	bool ShouldProcessForeground();
	bool CheckForFatal();
	bool SetWindowSize(int width, int height);

	// --------------------------------------------------------------
	//									   Public: Member sub-systems
	/// graphics provides all the graphics functionality
	CGraphicsSystem graphics;
	CInputSystem input;
	
	std::vector<TextureHandle> alphaTextures;
	std::vector<TextureHandle> cipherTextures;
	std::vector<TextureHandle> borderTextures;  //RNL
	std::vector<TextureHandle> miscTextures; //RNL
	std::vector<int> cipherText;
	std::vector<int> keyMap;
	std::vector<int> currentKeyMap;
	std::vector<int> conflictingKeyMap;
	int typingBuffer[MAX_TYPING_BUFFER];
	int currentTypingPos;
	EControlFocus focus;
	std::vector<std::vector<int> > wordList;
	int selectionPos;
	std::vector<int> wordPos;
protected:

	// --------------------------------------------------------------
	//								      Overridable game interfaces
	/// ProcessGameFrame should be overloaded by a game derived class.
	/// This fucntion should contain the game simulation code for a 
	/// time slice represented by dT
	/// TODO: Probably Make this pure virtual eventually
	virtual bool ProcessGameFrame (float dT);

	/// ProcessInputFrame should be overloaded by a game derived class.
	/// This fucntion should contain the input processing code to 
	/// modify game/application state based on input state
	virtual bool ProcessInputFrame (float dT);

	/// TODO: Probably Make this pure virtual eventually
	
	/// ProcessRenderState should be overloaded by a game derived class.
	/// This function should set the render state based on the current
	/// game state.
	/// TODO: Probably Make this pure virtual eventually
	virtual bool ProcessRenderState ();

	///	InitializeGame shoudl be overloaded by a game derived class.
	/// It will be called from InitializeSystem
	/// TODO: Probably Make this pure virtual eventually
	bool InitializeGame();

	/// ShutdownGame should be overloaded by a game derived class.
	/// It will be called from ShutdownSystem
	/// TODO: Probably Make this pure virtual eventually but be carefull
	/// about calling it during the destructor
	bool ShutdownGame();

	// --------------------------------------------------------------
	//										     Protected Interfaces
	/// InitializeSystem handles the setup all game subsystems and begins
	/// processing config files.
	bool InitializeSystem(void *initStructure);
	/// ShutdownSystem shuts down all game subsystems
	bool ShutdownSystem();

	void UpdateTypingBuffer(int alphaCharacter);
	void CopyTypingBufferToWordList();
	void CalculateKeyMap();
private:

};

#endif