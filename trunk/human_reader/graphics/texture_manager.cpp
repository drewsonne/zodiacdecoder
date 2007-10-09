#include "texture_manager.h"

CTextureManager::CTextureManager() {
	textureHandleManager = CHandleManager<STextureResource>::instance();
}

CTextureManager::~CTextureManager() {
	nameLookUp.clear();
	delete textureHandleManager;
}

CTextureManager::CTextureManager(const CTextureManager& _textureManager) {
}

TextureHandle CTextureManager::add(string _fileName) {
	// For testing we are just going to use filenames
	// but we will want to divide this up into name and ext

	// First check if we already have this name in our database.
	TextureHandle t = get(_fileName);
	if (!t.isNull()) {
		assert(false);	// We probably shouldn't be adding textures twice
		return t;	}

	STextureNameToHandle nameToHandle;
	STextureResource* textureResource = textureHandleManager->acquire(nameToHandle.handle);
	assert (textureResource != NULL);
	assert (!nameToHandle.handle.isNull());

	textureResource->name = _fileName;
	nameToHandle.name = _fileName;

	nameLookUp.push_back(nameToHandle);

	return nameToHandle.handle;
}

TextureHandle CTextureManager::get(string _name) {
	NameToHandleLookUp::iterator it = nameLookUp.begin();
	
	while (it != nameLookUp.end()) {
		if ((*it).name == _name) {
			// Make sure its valid;
			if (textureHandleManager->isValid((*it).handle)) {
				return (*it).handle;
			} else {
				return TextureHandle();
			}
		}

		++it;
	}

	return TextureHandle();
}
CTextureManager::NameToHandleLookUp::iterator CTextureManager::get(const TextureHandle &_handle) {
	NameToHandleLookUp::iterator it = nameLookUp.begin();
	while (it != nameLookUp.end()) {
		if ((*it).handle == _handle) {
			return it;
		}
		++it;
	}
	return it;
}

textureManagerError CTextureManager::remove(string _fileName) {

	// For now we will just remove the name.
	// But eventually we would like to add reference counting.
	CTextureManager::NameToHandleLookUp::iterator it = nameLookUp.begin();
	while (it != nameLookUp.end()) {
		if ((*it).name == _fileName) {
			break;
		}
		++it;
	}

	if (it != nameLookUp.end()) {
		if (textureHandleManager->getReferenceCount((*it).handle) != 1) {
			assert(false);
			return TM_FAIL;
		}


		TextureHandle tempHandle = (*it).handle;
		nameLookUp.erase(it);
		STextureResource *tResource = textureHandleManager->dereference(tempHandle);
		tResource->release();
		textureHandleManager->release(tempHandle);
		return TM_OK;
	}

	return TM_FAIL;
}

const STextureResource* CTextureManager::access(const TextureHandle &_handle) const{

	const STextureResource *tResource = textureHandleManager->dereference(_handle);

	if (tResource == NULL) {
		return textureHandleManager->getNull();
	}

	return tResource;
}

textureManagerError CTextureManager::access(const TextureHandle &_handle, STextureResource *&_textureResource) {
	STextureResource *tResource = textureHandleManager->dereference(_handle);

	if (tResource == NULL) {
		_textureResource = NULL;
		return TM_INVALID_HANDLE;
	}

	_textureResource = tResource;
	return TM_OK;
}
