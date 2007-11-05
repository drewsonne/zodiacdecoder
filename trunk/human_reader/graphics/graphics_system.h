#ifndef __GRAPHICS_SYSTEM__
#define __GRAPHICS_SYSTEM__
/////////////////////////////////////////////////////////////////////
//	graphics_system.h
//	Initial Version : HAM
//
//	class CGraphicsSystem
//		this class should provide a wrapper to whatever graphics
//		API we are using
/////////////////////////////////////////////////////////////////////

#include <d3d9.h>
#include "../core/system_base.h"
#include "texture_manager.h"
#include <list>
#include <d3dx9.h>

#define QUAD_WIDTH 1
#define QUAD_HEIGHT 1
#define PI 3.14159265
class CTextureManager;
struct STextureResource;
struct vertexTypeTemp {
	D3DXVECTOR3 pos;
	D3DCOLOR color;
	D3DXVECTOR2 tex1;
};

struct SRenderNodeColor {
	float rgba[4];
};

const SRenderNodeColor kBlackColor = {0.0f, 0.0f, 0.0f, 1.0f};
const SRenderNodeColor kWhiteColor = {1.0f, 1.0f, 1.0f, 1.0f};
const SRenderNodeColor kCodedCipherColor = kWhiteColor;
const SRenderNodeColor kMovingWordColor = {0.0f, 0.0f, 1.0f, 1.0f};
const SRenderNodeColor kDecodedCipherColor = {0.0f, 1.0f, 0.0f, 1.0f};
const SRenderNodeColor kConflictingCipherColor = {1.0f, 0.0f, 0.0f, 1.0f};
const SRenderNodeColor kTypedCharacterColor = {0.8f, 0.8f, 0.8f, 1.0f};
const SRenderNodeColor kTypedCursorCharacterColor = {1.0f, 0.5f, 0.5f, 1.0f};
const SRenderNodeColor kButtonColor = kWhiteColor;
const SRenderNodeColor kHoveredButtonColor = {1.0f, 0.9f, 0.9f, 1.0f};
const SRenderNodeColor kDownButtonColor = {1.0f, 0.8f, 0.8f, 1.0f};
const SRenderNodeColor kDisabledButtonColor = {0.8f, 0.8f, 0.8f, 1.0f};
const SRenderNodeColor kBorderColor = {0.8f, 0.8f, 0.8f, 1.0f};
const SRenderNodeColor kLogoColor = kWhiteColor;
const SRenderNodeColor kWordListCharacterColor = {0.0f, 0.7f, 0.0f, 1.0f};
const SRenderNodeColor kWordListSelectedCharacterColor = {0.0f, 1.0f, 0.0f, 1.0f};
const SRenderNodeColor kWordListSeperatorLineColor = {0.1f, 0.1f, 0.1f, 1.0f};

#define RENDERNODE_MAX_TEXT 256
struct STempRenderNode {
	STempRenderNode() {
		pos = D3DXVECTOR3(0,0,0);
		scale = D3DXVECTOR3(1,1,1);
		text[0] = '\0';
		textColor = kBlackColor;
	}
	D3DXVECTOR3 pos;
	D3DXVECTOR3 scale;
	SRenderNodeColor color;
	SRenderNodeColor textColor;
	char text[RENDERNODE_MAX_TEXT];
	TextureHandle textureHandle;
};


class CGraphicsSystem : public CSystemBase {
public:

	// --------------------------------------------------------------
	//											           Structures
	struct SGraphicsScreenStructure {
		SGraphicsScreenStructure(){
			fullScreen = false;
			screenWidth = 800;
			screenHeight = 600; 
			windowWidth = screenWidth;
			windowHeight = screenHeight;
			windowX = 0;
			windowY = 0;
			screenFormat = D3DFMT_X8R8G8B8;
			refreshRate = 0;
			windowHandle = 0;
		};

		SGraphicsScreenStructure& operator= (const D3DDISPLAYMODE &mode) {
			if (&displayMode != &mode) {
				displayMode = mode;
			}

			refreshRate = displayMode.RefreshRate;
			screenHeight = displayMode.Height;
			screenWidth = displayMode.Width;
			screenFormat = displayMode.Format;
			

			return *this;
		};

		bool fullScreen;
		int screenWidth, screenHeight;
		int windowWidth, windowHeight;
		int windowX, windowY;
		D3DFORMAT screenFormat;
		D3DDISPLAYMODE displayMode;
		unsigned int refreshRate;
		HWND windowHandle;
		long previousWindowStyle;
	} screenState;

	bool fatalError;

	// --------------------------------------------------------------
	//										 Constructors/Destructors
	CGraphicsSystem();
	~CGraphicsSystem();

	// --------------------------------------------------------------
	//									 	       Public: Interfaces
	void Render();
	bool SetWorld(const D3DXVECTOR3 &vecTranslation,const D3DXVECTOR3 &vecRotaion,const D3DXVECTOR3 &vecScaling);
	bool SetCamera(const D3DXVECTOR3 &vecPosition,const D3DXVECTOR3 &vecLookAt,const D3DXVECTOR3 &vecUp);
	bool SetProjection();
	bool SetFullscreen(bool fullScreen);
	bool ToggleFullscreen();
	bool ResetDevice();
	bool IsDeviceReady();
	void SyncWindowInformation();
	TextureHandle addTexture(string _fileName);
	TextureHandle getTexture(string _fileName);
	bool selectTexture(const TextureHandle &_handle, int _stage);
	bool cacheTexture(const TextureHandle &_handle);
	bool uncacheTexture(const TextureHandle &_handle);
	void clearRenderNodes() {renderNodes.clear();}
	void addRenderNode(const STempRenderNode& node) {renderNodes.push_back(node);}
	string assetPath, texturePath, stockTexturePath;
	void getClientRect(RECT &rct) {GetWindowRect(screenState.windowHandle, &rct);}
protected:
	IDirect3DDevice9*		d3dDevice;
	IDirect3D9*				d3dObject;
	ID3DXFont *fontObject;
	virtual bool InitializeSystem(void *initStructure);
	virtual bool ShutdownSystem();
	bool SetDeviceParameters();
	D3DPRESENT_PARAMETERS d3dPresentParams;
	bool hardwareGraphics;
private:
	std::list<STempRenderNode> renderNodes;
	CTextureManager textureManager;
	bool ChooseGraphicsMode(SGraphicsScreenStructure *desiredScreenState);
	HRESULT  CreateD3DDevice();

	// TODO: these resources are temporary
	TextureData mouseTexture;
	SurfaceData mouseSurface;
	LPDIRECT3DVERTEXDECLARATION9 vertexDeclaration;
	LPDIRECT3DVERTEXSHADER9      vertexShader; 
	LPD3DXCONSTANTTABLE          constantTable; 
	LPDIRECT3DPIXELSHADER9       pixelShader;
	LPDIRECT3DVERTEXBUFFER9      quadVB; //VertexBuffer

};
#endif