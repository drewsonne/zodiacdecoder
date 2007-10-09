#include "index_manager.h"

CIndexManager::CIndexManager() : 
	nextIndex(DEFAULT_MIN_INDEX), 
	maxIndex(DEFAULT_MAX_INDEX), 
	minIndex(DEFAULT_MIN_INDEX) {
	freeIndex.clear();
}

CIndexManager::CIndexManager(const IndexType &_minIndex, const IndexType &_maxIndex) : 
	nextIndex(_minIndex),
	maxIndex(_maxIndex),
	minIndex(_minIndex) {
	freeIndex.clear();
}

CIndexManager::CIndexManager(const CIndexManager &idManager) :
	nextIndex(DEFAULT_MIN_INDEX),
	maxIndex(DEFAULT_MAX_INDEX),
	minIndex(DEFAULT_MIN_INDEX) {}

CIndexManager::~CIndexManager() {}

GetIndexResult CIndexManager::getIndex(IndexType &_index) {
	if (freeIndex.size()) {
		_index = freeIndex.back();
		freeIndex.pop_back();
		return REUSED_INDEX;
	} else {
		if (nextIndex == maxIndex) {
			_index = maxIndex;
			return NO_INDEX;
		} else {
			_index = nextIndex++;
			return NEW_INDEX;
		}
	}
}

void CIndexManager::ReleaseIndex(const IndexType &_id) {

	// If index is < to our minIndex then don't do anything
	if (_id < minIndex) {
		assert(false);
		return;
	}

#if defined(_DEBUG) || defined (INDEX_MANAGER_SAFE_RELEASE)

	// Lets check first that we haven't already released this index.
	// We don't want to put it on our free stack twice.
	if (freeIndex.size()) {
		unsigned int i;
		bool alreadyFreed = false;
		for (i = 0; i < freeIndex.size(); i++) {
			if (freeIndex[i] == _id) {
				assert(false);
				alreadyFreed = true;
				break;
			}
		}

		if (!alreadyFreed) {
			freeIndex.push_back(_id);
		}

	} else {
		freeIndex.push_back(_id);
	}
#else
	freeIndex.push_back(_id);
#endif

}

