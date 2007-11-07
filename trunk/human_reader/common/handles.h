#ifndef __HANDLES_H__
#define __HANDLES_H__
/////////////////////////////////////////////////////////////////////
// Handle implementation based from Scott Bilas' implementation in GPGs
/////////////////////////////////////////////////////////////////////
#include "assert.h"
#include "index_manager.h"
#include <fstream>
#define MAX_BITS_INDEX 16
#define MAX_BITS_MAGIC 16

#define	MAX_INDEX ((1 << MAX_BITS_INDEX) - 1)
#define	MAX_MAGIC ((1 << MAX_BITS_MAGIC) - 1)

class CHandledObject {
	CHandledObject();
	virtual ~CHandledObject();
	virtual bool release() = 0;
};

template <class T> class CHandleManager;

//===================================================================
//						      class CHandle
//===================================================================
template <class T>
class CHandle
{
private:
	union {

		struct {
			unsigned index : MAX_BITS_INDEX;
			unsigned magicNumber : MAX_BITS_MAGIC;
		};

		unsigned int handleValue;
	};

public:

	CHandle() : handleValue(0) { };
	CHandle(const CHandle<T>& _handle);
	~CHandle();
	void init(const IndexType &_index);

    IndexType getIndex( void) const  {  return (  index  );  }
    unsigned int getMagic ( void ) const  {  return (  magicNumber  );  }
    unsigned int getHandle( void ) const  {  return (  handleValue );  }
    bool		 isNull   ( void ) const  {  return ( !handleValue );  }
	void null();
	static unsigned int GetMaxIndex() { return MAX_INDEX; }
	CHandle<T>& operator= (const CHandle<T>& _handle);
	operator unsigned int ( void ) const  {  return (  handleValue );  }


}; 
//=================================================================== class CHandle
//-------------------------------------------------------------------
//												     CHandle<T>::Init
template <class T>
void CHandle<T>::init(const IndexType &_index) {
	assert(isNull());
	assert(_index <= MAX_INDEX);

	static unsigned int nextMagicNumber = 1;

	index = _index;
	magicNumber = nextMagicNumber;

	++nextMagicNumber;

	if (nextMagicNumber > MAX_MAGIC) {
		nextMagicNumber = 1;
	}
}
template <class T>
void CHandle<T>::null() {
	if (!this->isNull()) {
		CHandleManager<T>::instance()->unreference(this);
		index = 0;
		magicNumber = 0;
	}
}
template <class T>
CHandle<T>::~CHandle() {
	if (!this->isNull()) {
		CHandleManager<T>::instance()->unreference(this);
	}
}
template <class T>
CHandle<T>::CHandle(const CHandle<T>& _handle) {
	handleValue = _handle.handleValue;

	if (!this->isNull()) {
		CHandleManager<T>::instance()->reference(this);
	} 
}
//-------------------------------------------------------------------
//												CHandle<T>::operator=
template <class T>
CHandle<T>& CHandle<T>::operator= (const CHandle<T>& _handle) {
	if (!this->isNull()) {
		CHandleManager<T>::instance()->unreference(this);
	} 

	handleValue = _handle.handleValue;

	if (!this->isNull()) {
		CHandleManager<T>::instance()->reference(this);
	} 

	return *this;
}
//-------------------------------------------------------------------
//					         operator != (CHandle<T> l, CHandle<T> r)
template <class T>
inline bool operator != (CHandle<T> l, CHandle<T> r) {  
	return l.getHandle() != r.getHandle();  
}
//-------------------------------------------------------------------
//					         operator == (CHandle<T> l, CHandle<T> r)
template <class T>
inline bool operator == (CHandle<T> l, CHandle<T> r) {  
	return l.getHandle() == r.getHandle();  
}

//===================================================================
//						   class CHandleManager
//===================================================================
template <class T>
class CHandleManager 
{
	friend class CHandle<T>;
public:
	~CHandleManager();
	static CHandleManager<T>* const instance();
	unsigned int getUsedHandleCount() const;
	bool hasUsedHandles() const;
	bool isValid(const CHandle<T> &_handle);
//protected:
	T* acquire(CHandle<T> &_handle);
	void release (CHandle<T> &_handle);

	T* dereference(const CHandle<T> &_handle);
	const T* dereference(const CHandle<T> &_handle) const;
	const T* getNull() const;
	int getReferenceCount(const CHandle<T> &_handle);
private:
	void reference(CHandle<T> *_handle);
	void unreference(CHandle<T> *_handle);
	static CHandleManager<T>* singleInstance;
	CHandleManager();
	explicit CHandleManager(const CHandleManager<T> &_handleMgr);
    std::vector <T>				data;
	std::vector <unsigned int>	magicNumbers;
	std::vector <int>			referenceCounts;
	CIndexManager				indexManager;
};
//=================================================================== class CHandleManager
template <class T>
CHandleManager<T>* CHandleManager<T>::singleInstance = NULL;
//-------------------------------------------------------------------
//								          CHandleManager<T>::instance
template <class T>
CHandleManager<T>* const CHandleManager<T>::instance() {
	if (singleInstance ==NULL) {
		singleInstance = new CHandleManager<T>();
	} 

	return singleInstance;
}
//-------------------------------------------------------------------
//								    CHandleManager<T>::CHandleManager
template <class T>
CHandleManager<T>::CHandleManager() : indexManager(1,CHandle<T>::GetMaxIndex()){
	data.push_back(T());
	magicNumbers.push_back(0);
	referenceCounts.push_back(0);
}
//-------------------------------------------------------------------
//								    CHandleManager<T>::CHandleManager
template <class T>
CHandleManager<T>::CHandleManager(const CHandleManager<T> &_handleMgr) : indexManager(1,CHandle<T>::GetMaxIndex()) {

}
//-------------------------------------------------------------------
//								   CHandleManager<T>::~CHandleManager
template <class T>
CHandleManager<T>::~CHandleManager() {
}

//-------------------------------------------------------------------
//								CHandleManager<T>::getUsedHandleCount
template <class T>
unsigned int CHandleManager<T>::getUsedHandleCount() const {
	return indexManager.IndexCount();
}
//-------------------------------------------------------------------
//								 CHandleManager<T>::getReferenceCount
template <class T>
int CHandleManager<T>::getReferenceCount(const CHandle<T> &_handle) {
	assert (_handle.getIndex() < data.size());
	assert (magicNumbers[_handle.getIndex()] == _handle.getMagic());
	return referenceCounts[_handle.getIndex()];
}
//-------------------------------------------------------------------
//									CHandleManager<T>::hasUsedHandles
template <class T>
bool CHandleManager<T>::hasUsedHandles(void) const {
	return indexManager.IndexCount() > 0;
}
//-------------------------------------------------------------------
//										   CHandleManager<T>::isValid
template <class T>
inline bool CHandleManager<T>::isValid(const CHandle<T> &_handle) {
	assert (_handle.getIndex() < data.size());
	assert (magicNumbers[_handle.getIndex()] == _handle.getMagic());
	return true;
}
//-------------------------------------------------------------------
//										   CHandleManager<T>::acquire
template <class T>
T* CHandleManager<T>::acquire(CHandle<T> &_handle) {

	// Handles cannot be reassigned.
	assert(_handle.isNull());

	IndexType index;
	GetIndexResult indexResultType = indexManager.getIndex(index);

	switch (indexResultType) {
		default:
		case NO_INDEX:
			return NULL;
		case REUSED_INDEX:
			_handle.init(index);
			data[index] = T();
			magicNumbers[index] = _handle.getMagic();
			referenceCounts[index] = 1;
			return &(data[index]);
		case NEW_INDEX:
			_handle.init(index);
			data.push_back(T());
			magicNumbers.push_back(_handle.getMagic());
			referenceCounts.push_back(1);
			return &(data[index]);
	}
}
//-------------------------------------------------------------------
//										   CHandleManager<T>::release
template <class T>
void CHandleManager<T>::release(CHandle<T> &_handle) {
	IndexType index = _handle.getIndex();

	assert (index < data.size());
	assert (magicNumbers[index] == _handle.getMagic());
	assert (referenceCounts[index] == 1);

	unsigned int magic = ~(_handle.getMagic());

	_handle.null();

	magicNumbers[index] = ~(magic);
	referenceCounts[index] = 0;
	indexManager.ReleaseIndex(index);


}
//-------------------------------------------------------------------
//								       CHandleManager<T>::Dereference
template <class T>
T* CHandleManager<T>::dereference(const CHandle<T> &_handle) {

	// Always pass null when asking for a null handle.
	if (_handle.isNull()) {
		return NULL;
	}

	IndexType index = _handle.getIndex();

	// Validate the Handle.
	if (index >= data.size() || magicNumbers[index] != _handle.getMagic()) {
		assert(false);
		return NULL;
	}

	return &(data[index]);
}
//-------------------------------------------------------------------
//								           CHandleManager<T>::getNull
template <class T>
const T* CHandleManager<T>::getNull() const{
	return &(data[0]);
}
//-------------------------------------------------------------------
//								 CHandleManager<T>::Dereference const
template <class T>
const T* CHandleManager<T>::dereference(const CHandle<T> &_handle) const{

	// AS long as the non const version of Dereference does not modify anything
	// we should be ok with a const cast

	typedef CHandleManager<T> thisType;
	return (const_cast <thisType*> (this)->dereference(_handle));
}
//-------------------------------------------------------------------
//								         CHandleManager<T>::reference
template <class T>
void CHandleManager<T>::reference(CHandle<T> *_handle) {
	assert(!_handle->isNull());

	IndexType index = _handle->getIndex();

	// Validate the Handle.
	if (index >= data.size() || magicNumbers[index] != _handle->getMagic()) {
		assert(false);
		return;
	}

	referenceCounts[_handle->index]++;
	return;
}
//-------------------------------------------------------------------
//								       CHandleManager<T>::unreference
template <class T>
void CHandleManager<T>::unreference(CHandle<T> *_handle) {
	assert(!_handle->isNull());

	IndexType index = _handle->getIndex();

	// Validate the Handle.
	if (index >= data.size() || magicNumbers[index] != _handle->getMagic()) {
		assert(false);
		return;
	}

	--referenceCounts[_handle->index];
	assert(referenceCounts[_handle->index] >= 0);
	return;
}
#endif // __HANDLES_H__