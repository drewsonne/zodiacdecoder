#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

#include <vector>
#include "../common/string_base.h"
#include "../common/handles.h"
#include "../common/index_manager.h"

struct STextureResource {
	STextureResource() : name(""), data(NULL) {};
	void release() {
		if(data) {
			data->Release();
			data = NULL;
		}
	};

	~STextureResource() {
		release();
	}
	string name;
	TextureData data;
};

enum textureManagerError {
	TM_OK = 1,
	TM_FAIL = 0,
	TM_INVALID_HANDLE = -1
};
typedef CHandle<STextureResource> TextureHandle;

class CTextureManager {
public:
	CTextureManager();
	~CTextureManager();

	/// Adds a STextureResource to the Texture Manager
	/// and returns a handle so we can reference the entry.
	/// This does not allocate memory for the texture.
	/// Return NULL handle if the texture could not be added.
	TextureHandle add(string _fileName);
	/// Gets the a TextureHandle for the named texture resource.
	/// Return NULL handle if texture could not be found.
	TextureHandle get(string _name);
	/// Removes the STextureResource reference by _handle.
	/// This will unload the texture data if it has been
	/// cached in memory.  After this call the _handle will
	/// be invalid.
	textureManagerError remove(string _fileName);
	/// Access a const Texture resource.
	/// If successfull will return the Texture resource associated with handle.
	/// If Failure the STextureResource will be a NULL resource.
	const STextureResource* access(const TextureHandle &_handle) const;

	/// Access a Texture resource.
	/// If successfull _textureResource will point to the resource associated with _handle.
	/// If failure _textureResource will be NULL;
	textureManagerError access(const TextureHandle &_handle, STextureResource *&_textureResource);
protected:
private:
	struct STextureNameToHandle {
		string name;
		string ext;
		TextureHandle handle;
	} ;

	typedef std::vector<STextureNameToHandle> NameToHandleLookUp;
	NameToHandleLookUp nameLookUp;
	NameToHandleLookUp::iterator get(const TextureHandle &_handle);
	CHandleManager<STextureResource> *textureHandleManager;
	explicit CTextureManager(const CTextureManager& _textureManager);

};
#endif