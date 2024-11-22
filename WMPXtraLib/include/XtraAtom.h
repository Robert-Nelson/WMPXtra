#pragma once

#include <map>
#include <string>

#include "XtraTag.h"

typedef std::map<std::string, XtraTag> TAG_MAP;

class XtraAtom
{
public:
	static XtraAtom* CreateFromFile(const char* szFileName);
	static XtraAtom* Deserialize(uint8_t const* pData, uint32_t uiDataSize, uint32_t* puiProcessed);

	bool SaveToFile(const char* szFileName) const;

	uint32_t SerializedSize() const;
	uint32_t Serialize(uint8_t* pData, uint32_t uiDataSize) const;

	size_t tagCount() {
		return m_Tags.size();
	}

	XtraTag* tagAdd(const char* name) {
		XtraTag* pTag = new XtraTag(name);
		
		m_Tags.insert(std::pair<std::string, XtraTag>(name, *pTag));

		return pTag;
	}

	bool tagDelete(const char* name) {
		return m_Tags.erase(name) == 1;
	}

	TAG_MAP::const_iterator tagBegin() const {
		return m_Tags.begin();
	}

	TAG_MAP::iterator tagBegin() {
		return m_Tags.begin();
	}

	TAG_MAP::const_iterator tagEnd() const {
		return m_Tags.end();
	}

	TAG_MAP::iterator tagEnd() {
		return m_Tags.end();
	}

	TAG_MAP::const_iterator tagFind(const char *pKey) const {
		return m_Tags.find(pKey);
	}

	TAG_MAP::iterator tagFind(const char* pKey) {
		return m_Tags.find(pKey);
	}

private:
	TAG_MAP m_Tags;
};

