#pragma once

#include <vector>
#include <string>

#include "XtraTagValue.h"

typedef std::vector<XtraTagValue> TAG_VALUE_LIST;

class XtraTag
{
public:
	static XtraTag Deserialize(uint8_t const* pData, uint32_t uiDataSize, uint32_t* puiProcessed);

	XtraTag()
	{
	}

	XtraTag(char const * pName)
	{
		Name(pName);
	}

	~XtraTag()
	{
	}

	char const * Name()
	{
		return m_Name.c_str();
	}

	void Name(char const * pName)
	{
		m_Name.assign(pName);
	}

	uint32_t SerializedSize() const;

	uint32_t Serialize(uint8_t* pData, uint32_t uiDataSize) const;

	size_t valuesCount() {
		return m_Values.size();
	}

	TAG_VALUE_LIST::const_iterator valuesBegin() const {
		return m_Values.begin();
	}

	TAG_VALUE_LIST::iterator valuesBegin() {
		return m_Values.begin();
	}

	TAG_VALUE_LIST::const_iterator valuesEnd() const {
		return m_Values.end();
	}

	TAG_VALUE_LIST::iterator valuesEnd() {
		return m_Values.end();
	}

	const XtraTagValue& valuesAt(size_t pos) const {
		return m_Values.at(pos);
	}

	const XtraTagValue& valuesAt(size_t pos) {
		return m_Values.at(pos);
	}

	bool ClearValues();

	bool AddValue(const XtraTagValue& value);

	bool SetValues(const TAG_VALUE_LIST& valueList);

private:
	std::string m_Name;
 
	TAG_VALUE_LIST m_Values;
};

