#include <windows.h>
#include "graphics_system.h"
#include "assert.h"
#include "string_base.h"
#include "tinyxml.h"

/////////////////////////////////////////////////////////////////////
//							         CGraphicsSystem::CGraphicsSystem
/////////////////////////////////////////////////////////////////////
CGraphicsSystem::CGraphicsSystem() {
	d3dDevice = NULL;
	d3dObject = NULL;
	fontObject = NULL;
	mouseTexture = NULL;
	vertexDeclaration = NULL;
	pixelShader = NULL;
	vertexShader = NULL;
	constantTable = NULL;
	quadVB = NULL;
	hardwareGraphics = false;
	fatalError = false;
}

/////////////////////////////////////////////////////////////////////
//							        CGraphicsSystem::~CGraphicsSystem
/////////////////////////////////////////////////////////////////////
CGraphicsSystem::~CGraphicsSystem(){

	clearRenderNodes();

	if (constantTable) {
		constantTable->Release();
		constantTable = NULL;
	}

	if (pixelShader) {
		pixelShader->Release();
		pixelShader = NULL;
	}

	if (vertexShader) {
		vertexShader->Release();
		vertexShader = NULL;
	}

	if (vertexDeclaration) {
		vertexDeclaration->Release();
		vertexDeclaration = NULL;
	}

	if (mouseTexture) {
		mouseTexture->Release();
		mouseTexture = NULL;
	}

	if (quadVB) {
		quadVB->Release();
		quadVB = NULL;
	}

	if (fontObject) {
		fontObject->Release();
		fontObject = NULL;
	}
	// TODO: Not sure if we want to call shutdown in the destructor
	// Shutdown call a pure virtual ShutdownSystem function.  When 
	// we get to the destructor the vtbl no longer points to derived
	// classes.  This could cause problems.
	Shutdown();
}

/////////////////////////////////////////////////////////////////////
//										  CGraphicsSystem::Initialize
/////////////////////////////////////////////////////////////////////
bool CGraphicsSystem::InitializeSystem(void *initStructure){

	TiXmlDocument xmlDoc("graphics_config.xml");
	if (!xmlDoc.LoadFile()) {
		return false;
	}

	TiXmlElement * xmlElement = NULL;

	if (!(xmlElement = xmlDoc.FirstChildElement("AssetPath"))) {
		return false;
	}

	assetPath = xmlElement->GetText();

	if (!(xmlElement = xmlDoc.FirstChildElement("TexturePath"))) {
		return false;
	}

	texturePath = assetPath + "/" + xmlElement->GetText();

	if (!(xmlElement = xmlDoc.FirstChildElement("StockTexturePath"))) {
		return false;
	}

	stockTexturePath = xmlElement->GetText();

	if (!(xmlElement = xmlDoc.FirstChildElement("ShaderPath"))) {
		return false;
	}

	string shadersPath = xmlElement->GetText();
	shadersPath = assetPath + "/" + shadersPath;

	if (!(xmlElement = xmlDoc.FirstChildElement("Textures"))) {
		return false;
	} else {
		TiXmlElement * textureElement = NULL;
		for (textureElement = xmlElement->FirstChildElement("Texture"); textureElement != NULL; textureElement = textureElement->NextSiblingElement()) {
			string textureName = textureElement->Attribute("name");
			textureName = texturePath + "/" + textureName;
			addTexture(textureName);
		}
	}
	// --------------------------------------------------------------
	//					            Get the initialization parameters
	if (initStructure == NULL) {
		return false;
	}

	SGraphicsScreenStructure *initParams = (SGraphicsScreenStructure*)initStructure;

	if (initParams->windowHandle == NULL) {
		return false;
	}

	// --------------------------------------------------------------
	//										      Initialize Direct3D
	if (d3dObject == NULL) {
		if ((d3dObject = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) {
			return false;
		}
	}

	// Choose the desired displayMode.
	// initParams will contain the choosen displayMode.
	if (!ChooseGraphicsMode(initParams)){
		return false;
	} 

	// Synce up our screenState data
	screenState = initParams->displayMode;
	screenState.fullScreen = initParams->fullScreen;
	screenState.windowHandle = initParams->windowHandle;
	screenState.windowWidth = initParams->windowWidth;
	screenState.windowHeight = initParams->windowHeight;
	screenState.windowX = initParams->windowX;
	screenState.windowY = initParams->windowY;

	HRESULT hr = CreateD3DDevice();
	if (FAILED(hr)) {
		d3dDevice = NULL;
		return false;
	}

	// TODO: this is bad place we need to load this stuff
	// when we load textures
	string mousePointerPath = texturePath + "/" + stockTexturePath + "/mouse_default.png";
	if (FAILED(hr = D3DXCreateTextureFromFileEx(
		d3dDevice,
		mousePointerPath.c_str(),
		0,0,
		1,
		0,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_FILTER_NONE,
		0xFF000000,
		NULL,NULL,
		&mouseTexture
		))) {
		return false;
	}

	mouseTexture->GetSurfaceLevel(0,&mouseSurface);
	if (FAILED (d3dDevice->SetCursorProperties(0,0,mouseSurface))) {
		return false;
	}	

	D3DVERTEXELEMENT9 decl[] =	{{
								0,
								0,
								D3DDECLTYPE_FLOAT3,
								D3DDECLMETHOD_DEFAULT,
								D3DDECLUSAGE_POSITION,
								0
								},
								{
								0,
								sizeof(float) * 3,
								D3DDECLTYPE_D3DCOLOR,
								D3DDECLMETHOD_DEFAULT,
								D3DDECLUSAGE_COLOR,
								0
								},
								{
								0,
								sizeof(float) * 3 + sizeof(D3DCOLOR),
								D3DDECLTYPE_FLOAT2,
								D3DDECLMETHOD_DEFAULT,
								D3DDECLUSAGE_TEXCOORD,
								0},
								D3DDECL_END()
								};


	if (FAILED (hr = d3dDevice->CreateVertexDeclaration(decl,&vertexDeclaration))) {
		return false;
	}


	// Determine shader version to use.
	LPCSTR vertexShaderVersion = NULL;
	LPCSTR pixelShaderVersion = NULL;

	vertexShaderVersion = D3DXGetVertexShaderProfile(d3dDevice);
	pixelShaderVersion = D3DXGetPixelShaderProfile(d3dDevice);

	// Does not support shaders.
	if (vertexShaderVersion == NULL || pixelShaderVersion == NULL) {
		return false;
	}

	LPD3DXBUFFER                 code = NULL;
	LPD3DXBUFFER                 errors = NULL;

	// Compile the vertex shader.
	string shaderFileName = shadersPath + "/vertex.vsh";
	hr = D3DXCompileShaderFromFile(shaderFileName.c_str(),    //filepath
                                   NULL,            //macro's
                                   NULL,            //includes
                                   "main",       //main function
                                   vertexShaderVersion,        //shader profile
                                   0,               //flags
                                   &code,           //compiled operations
                                   NULL,            //errors
                                   &constantTable); //constants
	if(FAILED(hr)) {
		return false;
	}


	d3dDevice->CreateVertexShader((DWORD*)code->GetBufferPointer(),
										&vertexShader);

	if (code) {
		code->Release();
		code = NULL;
	}

	if (errors) {
		errors->Release();
		errors = NULL;
	}

	// Compile the pixel shader.
	shaderFileName = shadersPath + "/pixel.psh";
	hr = D3DXCompileShaderFromFile(shaderFileName.c_str(),   //filepath
									   NULL,          //macro's            
									   NULL,          //includes           
									   "main",     //main function      
									   pixelShaderVersion,      //shader profile     
									   0,             //flags              
									   &code,         //compiled operations
									   NULL,          //errors
									   NULL);         //constants

	if(FAILED(hr)) {
		return false;
	}

	d3dDevice->CreatePixelShader((DWORD*)code->GetBufferPointer(),
									   &pixelShader);

	if (code) {
		code->Release();
		code = NULL;
	}

	if (errors) {
		errors->Release();
		errors = NULL;
	}

	// Setup a vertex buffer.

	//set up vertex buffer
	float width = QUAD_WIDTH;
	float height = QUAD_HEIGHT;
	D3DCOLOR color = 0xFFFFFFFF;

	vertexTypeTemp quad[4] = 
						{
							{
								D3DXVECTOR3(0.0f, -height, 0.0f),
								color,
								D3DXVECTOR2(0.0f, 1.0f)
							},
							{
								D3DXVECTOR3(0.0f,  0.0f, 0.0f),
								color,
								D3DXVECTOR2(0.0f, 0.0f)
							},
							{
								D3DXVECTOR3( width, -height, 0.0f),
								color,
								D3DXVECTOR2(1.0f, 1.0f)
							},
							{
								D3DXVECTOR3( width,  0.0f, 0.0f),
								color,
								D3DXVECTOR2(1.0f, 0.0f)
							}
						};

	vertexTypeTemp *ptr = NULL;
	d3dDevice->CreateVertexBuffer(sizeof(quad),
										D3DUSAGE_WRITEONLY,
										0,
										D3DPOOL_MANAGED,
										&quadVB,
										NULL);
	quadVB->Lock(0, 0, (void**)&ptr, 0);
	memcpy((void*)ptr, (void*)quad, sizeof(quad));
	quadVB->Unlock();

	// Create a D3DX font object
	hr = D3DXCreateFont( d3dDevice, 20, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &fontObject );
	if(FAILED(hr)) {
		return false;
	}
	return  true;
}

/////////////////////////////////////////////////////////////////////
//										    CGraphicsSystem::Shutdown
/////////////////////////////////////////////////////////////////////
bool CGraphicsSystem::ShutdownSystem(){

	// --------------------------------------------------------------
	//							       Release all graphics resources

	if (mouseTexture != NULL) {
		mouseTexture->Release();
		mouseTexture = NULL;
	}

	if (d3dDevice != NULL) {
		d3dDevice->Release();
		d3dDevice = NULL;
	}

	if (d3dObject != NULL) {
		d3dObject->Release();
		d3dObject = NULL;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////
//								  CGraphicsSystem::ChooseGraphicsMode
/////////////////////////////////////////////////////////////////////
bool CGraphicsSystem::ChooseGraphicsMode(SGraphicsScreenStructure *desiredScreenState)
{

	// Make sure we have a graphics object
	assert(d3dObject != NULL);
	assert(desiredScreenState != NULL);
	
	int nNumGraphicsModes = d3dObject->GetAdapterModeCount(D3DADAPTER_DEFAULT, desiredScreenState->screenFormat);

	// No available graphics modes
	if (nNumGraphicsModes <= 0) {
		return false;
	}

	bool modeSelected = false;	// Assume failure
	D3DDISPLAYMODE curmode;		// Current mode under consideration

	for(int i = 0; i < nNumGraphicsModes; i++){
		d3dObject->EnumAdapterModes(
			D3DADAPTER_DEFAULT,
			desiredScreenState->screenFormat,
			i,
			&curmode); 

		// Try to find the fastest refresh rate that matches our desired resolution
		if( curmode.Width == desiredScreenState->screenWidth && 
			curmode.Height == desiredScreenState->screenHeight ) 
		{
			if( !modeSelected || 
				curmode.RefreshRate == 0 ||
				curmode.RefreshRate>desiredScreenState->refreshRate )
			{
				desiredScreenState->displayMode = curmode; 
				desiredScreenState->refreshRate = curmode.RefreshRate; 
				modeSelected = true;
			}
		}
	}

	return modeSelected;
}

/////////////////////////////////////////////////////////////////////
//								     CGraphicsSystem::CreateD3DDevice
/////////////////////////////////////////////////////////////////////
HRESULT CGraphicsSystem::CreateD3DDevice() {

	SetDeviceParameters();

	::SetWindowPos(
		screenState.windowHandle,
		NULL,
		screenState.windowX,
		screenState.windowY,
		screenState.windowWidth, 
		screenState.windowHeight,
		SWP_NOZORDER|SWP_SHOWWINDOW
		);

	// Now we can create the device
	HRESULT hr = d3dObject->CheckDeviceType(
								D3DADAPTER_DEFAULT, 
								D3DDEVTYPE_HAL, 
								D3DFMT_X8R8G8B8, 
								D3DFMT_X8R8G8B8, 
								!screenState.fullScreen);

	// Determine if we are in hardware or software mode
	if(D3D_OK == hr) {
		hardwareGraphics = true;
		hr = d3dObject->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				screenState.windowHandle,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dPresentParams,
				&d3dDevice);

		if(FAILED(hr)){
			return hr;
		}

	} else {
		hardwareGraphics=false;

		hr = d3dObject->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_SW ,
				screenState.windowHandle,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dPresentParams,
				&d3dDevice);
		if (FAILED(hr)) {	
			return hr;
		}
	}

	return hr;
}

/////////////////////////////////////////////////////////////////////
//								              CGraphicsSystem::Render
/////////////////////////////////////////////////////////////////////
void CGraphicsSystem::Render() {
	
	d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	d3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	// Handle the mouse cursor
	d3dDevice->ShowCursor(true);

	d3dDevice->Clear(0,NULL,D3DCLEAR_TARGET,0xFF000000,1.0f,0L);  //RNL Black to White
	d3dDevice->BeginScene();
        //communicate with shader (NEW)
        D3DXMATRIXA16 matWorld, matView, matProj;

        d3dDevice->SetVertexDeclaration(vertexDeclaration);
        d3dDevice->SetVertexShader(vertexShader);
        d3dDevice->SetPixelShader(pixelShader);


        d3dDevice->SetStreamSource(0, quadVB, 0, sizeof(vertexTypeTemp));

		std::list<STempRenderNode>::iterator it = renderNodes.begin();
		while(it != renderNodes.end()) {
			SetWorld((*it).pos, D3DXVECTOR3(0, PI, PI), (*it).scale);
			d3dDevice->GetTransform(D3DTS_WORLD, &matWorld);
			d3dDevice->GetTransform(D3DTS_VIEW, &matView);
			d3dDevice->GetTransform(D3DTS_PROJECTION, &matProj);

			D3DXMATRIXA16 matWorldViewProj = matWorld * matView * matProj;

			constantTable->SetMatrix(d3dDevice,
									 "WorldViewProj",
									 &matWorldViewProj);

			constantTable->SetFloatArray(d3dDevice,
										"globalColor",
										((*it).color.rgba),4);
			selectTexture((*it).textureHandle,0);

			d3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

			// Determine if we print TEXT;
			if ((*it).text[0] != '\0') {
				// Create a colour for the text - in this case blue
				D3DCOLOR fontColor = D3DCOLOR_COLORVALUE((*it).textColor.rgba[0], (*it).textColor.rgba[1], (*it).textColor.rgba[2], (*it).textColor.rgba[3]);
				// Determine the RECT;
				RECT rct;
				rct.left=	(*it).pos.x;
				rct.right=(*it).pos.x + (*it).scale.x;
				rct.top=(*it).pos.y ;
				rct.bottom=((*it).pos.y +(*it).scale.y);
				// Draw some text
				fontObject->DrawText(NULL, (*it).text, -1, &rct, DT_CENTER | DT_VCENTER, fontColor );
			}

			++it;
		}


        d3dDevice->EndScene();

		d3dDevice->Present(NULL,NULL,NULL,NULL);


}

/////////////////////////////////////////////////////////////////////
//								       CGraphicsSystem::IsDeviceReady
/////////////////////////////////////////////////////////////////////
bool CGraphicsSystem::IsDeviceReady() {

	// We don't even have a device.  That's bad.
	if (d3dDevice == NULL) {
		fatalError = true;
		return false;
	}

	// Check the state of the device
    HRESULT result;
    result = d3dDevice->TestCooperativeLevel();

    switch(result){
        case D3DERR_DEVICELOST:
			// The device is lost so we are not ready.
			return false;
        case D3DERR_DEVICENOTRESET:
            // Try and reset the Device
			if (!ResetDevice()) {
				fatalError = true;
				return false;
			}
			return true;
        case D3DERR_DRIVERINTERNALERROR:
			// Bad error.  MSDN recommends program shutdown.
			fatalError = true;
			return false;
		default:
        case D3D_OK:
			return true;
    }
}

/////////////////////////////////////////////////////////////////////
//											CGraphicsSystem::SetWorld
/////////////////////////////////////////////////////////////////////
bool CGraphicsSystem::SetWorld(const D3DXVECTOR3 &vecTranslation,const D3DXVECTOR3 &vecRotaion,const D3DXVECTOR3 &vecScaling) {

	// Assertions
	assert(d3dDevice != NULL);

	D3DXMATRIXA16 matWorld;   
	D3DXMATRIXA16 MatTemp;
	D3DXMATRIXA16 MatRot;
	D3DXMatrixIdentity(&MatRot);

	D3DXMatrixScaling(&MatTemp,vecScaling.x,vecScaling.y,vecScaling.z);
	D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);

	D3DXMatrixRotationZ(&MatTemp, vecRotaion.z);          // Roll
	D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
	D3DXMatrixRotationY(&MatTemp, vecRotaion.y);          // Roll
	D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
	D3DXMatrixTranslation(&matWorld,vecTranslation.x,vecTranslation.y,vecTranslation.z);

	D3DXMatrixMultiply(&matWorld, &MatRot, &matWorld);

	return SUCCEEDED( d3dDevice->SetTransform(D3DTS_WORLD,&matWorld ));
}

/////////////////////////////////////////////////////////////////////
//								       CGraphicsSystem::SetProjection
/////////////////////////////////////////////////////////////////////
bool CGraphicsSystem::SetProjection() {

	// Assertions
	assert(d3dDevice != NULL);
	
	D3DXMATRIXA16 matProj;

	// If we are windowed then the window size might not match the buffer size.
	// We want to set the aspect ratio to the window size.  If fullscreen then 
	// we use the screen size.
	float fAspect;
	float windowWidth,windowHeight, screenWidth, screenHeight;
	windowWidth = (float) screenState.windowWidth;
	windowHeight = (float) screenState.windowHeight;
	screenWidth = (float) screenState.screenWidth;
	screenHeight = (float) screenState.screenHeight;
	if (screenState.fullScreen) {
		fAspect = screenWidth / screenHeight;
	} else {
		fAspect = windowWidth / windowHeight;
	}

	D3DXMatrixOrthoLH(&matProj, screenWidth, screenHeight, 1.0f, 20.0f);
	return SUCCEEDED( d3dDevice->SetTransform(D3DTS_PROJECTION, &matProj));
}

/////////////////////////////////////////////////////////////////////
//										   CGraphicsSystem::SetCamera
/////////////////////////////////////////////////////////////////////
bool CGraphicsSystem::SetCamera(const D3DXVECTOR3 &vecPosition,const D3DXVECTOR3 &vecLookAt,const D3DXVECTOR3 &vecUp) {

	// Assertions
	assert(d3dDevice != NULL);

	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView,&vecPosition,&vecLookAt,&vecUp);
	return SUCCEEDED(d3dDevice->SetTransform(D3DTS_VIEW,&matView));
}

/////////////////////////////////////////////////////////////////////
//									   CGraphicsSystem::SetFullscreen
/////////////////////////////////////////////////////////////////////
bool CGraphicsSystem::SetFullscreen(bool fullScreen) {

	// If now change is needed then don't do anything
	if (screenState.fullScreen == fullScreen) {
		return true;
	}

	screenState.fullScreen = fullScreen;

	// Before we reset the device we restore the window.
	// Windows doesn't seem to like going into fullscreen mode if the
	// window is maximized or minimized
	WINDOWPLACEMENT winPlacement;
	winPlacement.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(screenState.windowHandle, &winPlacement);
	winPlacement.showCmd = SW_RESTORE;
	SetWindowPlacement(screenState.windowHandle, &winPlacement);

	// Do the resest and bail if it fails.
	if (!ResetDevice()) {
		return false;
	}

	// If we are coming out of fullscreen then we restore the window pos.
	// More importantly the window seems to be stuck as a TOP MOST 
	// window when coming out of fullscreen.  So we clear that here.
	if (!screenState.fullScreen) {
		SetWindowLong(screenState.windowHandle, GWL_STYLE, screenState.previousWindowStyle);
		SetWindowPos(
			screenState.windowHandle, 
			HWND_NOTOPMOST,
			screenState.windowX,
			screenState.windowY,
			screenState.windowWidth, 
			screenState.windowHeight,
			0
			);
	} else {

		// In fullscreen mode we want to insure that we don't have a thick frame
		screenState.previousWindowStyle = SetWindowLong(screenState.windowHandle, GWL_STYLE, WS_VISIBLE);
	}

	return true;
}

/////////////////////////////////////////////////////////////////////
//									CGraphicsSystem::ToggleFullscreen
/////////////////////////////////////////////////////////////////////
bool CGraphicsSystem::ToggleFullscreen() {
	return SetFullscreen(!screenState.fullScreen);
}

/////////////////////////////////////////////////////////////////////
//									   CGraphicsSystem::SetDeviceParameters
/////////////////////////////////////////////////////////////////////
bool CGraphicsSystem::SetDeviceParameters() {

	// Set up device specs data structure
	ZeroMemory (&d3dPresentParams,sizeof(D3DPRESENT_PARAMETERS)); 

	// Assume fullscreen
	// TODO: go over these and decide what they should be set to or how we shouldd determine them
	d3dPresentParams.BackBufferWidth = screenState.displayMode.Width; 
	d3dPresentParams.BackBufferHeight = screenState.displayMode.Height; 
	d3dPresentParams.BackBufferFormat = screenState.displayMode.Format; 
	d3dPresentParams.BackBufferCount = 2;						// TODO: should this be set to 2?
	d3dPresentParams.MultiSampleType = D3DMULTISAMPLE_NONE;		// TODO: What should this be set to?
	d3dPresentParams.SwapEffect = D3DSWAPEFFECT_FLIP;			// TODO: flip pages?
	d3dPresentParams.hDeviceWindow = NULL;						//full screen
	d3dPresentParams.Windowed = 0;								//full screen
	d3dPresentParams.EnableAutoDepthStencil = FALSE;			// TODO: ?
	d3dPresentParams.AutoDepthStencilFormat = (D3DFORMAT)0;		// TODO: ?
	d3dPresentParams.Flags = 0;									// TODO: ?
	d3dPresentParams.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	//refresh rate - use from mode descriptor if available
	if (screenState.displayMode.RefreshRate) {
		d3dPresentParams.FullScreen_RefreshRateInHz = screenState.displayMode.RefreshRate;
	} else {
		d3dPresentParams.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;
	}

	// Check if we want a window
	if(!screenState.fullScreen)
	{
		d3dPresentParams.Windowed = 1;							//windowed
		d3dPresentParams.BackBufferCount = 1;					//1 back buffer
		d3dPresentParams.SwapEffect = D3DSWAPEFFECT_COPY;		//blit instead of swapping
		d3dPresentParams.FullScreen_RefreshRateInHz = 0;		//unused
	}

	return true;
}

/////////////////////////////////////////////////////////////////////
//									     CGraphicsSystem::ResetDevice
/////////////////////////////////////////////////////////////////////
bool CGraphicsSystem::ResetDevice() {

	// Set device parameters according to the current screenState values.
	SetDeviceParameters();

	if (FAILED(d3dDevice->Reset(&d3dPresentParams))) {
		return false;
	}

	// TODO: this is temporary.  We probably need a less naive approach
	mouseTexture->GetSurfaceLevel(0,&mouseSurface);
	if (FAILED (d3dDevice->SetCursorProperties(0,0,mouseSurface))) {
		return false;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////
//							   CGraphicsSystem::SyncWindowInformation
/////////////////////////////////////////////////////////////////////
void CGraphicsSystem::SyncWindowInformation() {

	RECT windowRect;
	GetWindowRect(screenState.windowHandle, &windowRect);
	screenState.windowX = windowRect.left;
	screenState.windowY = windowRect.top;
	GetClientRect(screenState.windowHandle, &windowRect);
	screenState.windowWidth = windowRect.right;
	screenState.windowHeight= windowRect.bottom;

}
/////////////////////////////////////////////////////////////////////
//										  CGraphicsSystem::addTexture
/////////////////////////////////////////////////////////////////////
TextureHandle CGraphicsSystem::addTexture(string _fileName) {
	return textureManager.add(_fileName);
}
/////////////////////////////////////////////////////////////////////
//										  CGraphicsSystem::getTexture
/////////////////////////////////////////////////////////////////////
TextureHandle CGraphicsSystem::getTexture(string _fileName) {
	return textureManager.get(_fileName);
}
/////////////////////////////////////////////////////////////////////
//									   CGraphicsSystem::selectTexture
/////////////////////////////////////////////////////////////////////
bool CGraphicsSystem::selectTexture(const TextureHandle &_handle, int stage) {
	STextureResource *tResource = NULL;
	textureManager.access(_handle, tResource);
	if (tResource && tResource->data) {
		d3dDevice->SetTexture(stage,tResource->data);
		return true;
	} 

	return false;
}
/////////////////////////////////////////////////////////////////////
//									   CGraphicsSystem::selectTexture
/////////////////////////////////////////////////////////////////////
bool CGraphicsSystem::cacheTexture(const TextureHandle &_handle) {

	STextureResource *tResource = NULL;
	if (TM_OK != textureManager.access(_handle, tResource)) {
		return false;
	}

	if (tResource == NULL) {
		return false;
	}

	if (tResource->data) {
		assert(false);  // Why are we caching things twice.
		return true;
	}

	if (FAILED(D3DXCreateTextureFromFileEx(
		d3dDevice,
		tResource->name.c_str(),
		0,0,
		1,
		0,
		D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_FILTER_NONE,
		0xFF000000,
		NULL,NULL,
		&tResource->data
		))) {
		return false;
	}	

	return true;
}
/////////////////////////////////////////////////////////////////////
//									   CGraphicsSystem::selectTexture
/////////////////////////////////////////////////////////////////////
bool CGraphicsSystem::uncacheTexture(const TextureHandle &_handle) {
	STextureResource *tResource = NULL;
	if (TM_OK != textureManager.access(_handle, tResource)) {
		return false;
	}

	if (tResource == NULL) {
		return false;
	}

	if (tResource->data) {
		assert(false); // Why are we uncaching more than once?
		return true;
	}

	tResource->release();
	return true;
}