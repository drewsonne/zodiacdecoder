#ifndef __INDEX_MANAGER_H__
#define __INDEX_MANAGER_H__

#include <vector>
#include "assert.h"

#define DEFAULT_MAX_INDEX (~0)
#define DEFAULT_MIN_INDEX 0

typedef unsigned int IndexType;

enum GetIndexResult {
	NO_INDEX,
	REUSED_INDEX,
	NEW_INDEX
};

class CIndexManager {
public:
	CIndexManager();
	CIndexManager(const IndexType &_minIndex, const IndexType &_maxIndex);
	~CIndexManager();
	GetIndexResult getIndex( IndexType &_index);
	void ReleaseIndex(const IndexType &_id);
	unsigned int IndexCount() const { return (nextIndex - minIndex)- freeIndex.size(); };
	unsigned int GetMaxIndex()  const { return maxIndex; };
protected:

private:
	explicit CIndexManager(const CIndexManager &idManager);

	std::vector<IndexType> freeIndex;
	IndexType nextIndex;
	const IndexType maxIndex;
	const IndexType minIndex; 

};

#endif // __INDEX_MANAGER_H__