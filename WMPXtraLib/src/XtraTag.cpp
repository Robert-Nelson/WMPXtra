#include "XtraTag.h"

uint32_t XtraTag::SerializedSize() const
{
    uint32_t size = static_cast<uint32_t>(sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t));    // length of tag + name length + # values

    size += static_cast<uint32_t>(m_Name.length());

    TAG_VALUE_LIST::const_iterator iterValues = m_Values.begin();

    for (; iterValues != m_Values.end(); iterValues++) {
        size += (*iterValues).SerializedSize();
    }

    return size;
}

uint32_t XtraTag::Serialize(uint8_t* pData, uint32_t uiDataSize) const
{
    uint32_t uiIndex = 4;
    uint32_t uiLength = 0;

    if ((uiIndex + 4) <= uiDataSize) {
        WriteUint32(&pData[uiIndex], static_cast<uint32_t>(m_Name.length()));
        uiIndex += 4;
    }

    if ((uiIndex + m_Name.length()) <= uiDataSize) {
        memcpy(&pData[uiIndex], m_Name.c_str(), m_Name.length());

        uiIndex += static_cast<uint32_t>(m_Name.length());
    }

    if ((uiIndex + 4) <= uiDataSize) {
        WriteUint32(&pData[uiIndex], static_cast<uint32_t>(m_Values.size()));
        uiIndex += 4;
    }

    TAG_VALUE_LIST::const_iterator iterValue = m_Values.begin();

    for (; iterValue != m_Values.end(); iterValue++) {
        uiIndex += (*iterValue).Serialize(&pData[uiIndex], uiDataSize - uiIndex);
    }

    WriteUint32(pData, uiIndex);

    return uiIndex;
}

XtraTag XtraTag::Deserialize(uint8_t const* pData, uint32_t uiDataSize, uint32_t* puiProcessed)
{
    uint32_t uiIndex = 0;
    uint32_t uiNameSize = 0;

    if ((uiIndex + 4) > uiDataSize)
    {
        return NULL;
    }

    uiNameSize = ReadUint32(pData);

    uiIndex += 4;

    if ((uiIndex + uiNameSize + 1) > uiDataSize)
    {
        return NULL;
    }

    XtraTag tag;

    char * pName = new char[uiNameSize + 1];
    memcpy(pName, &pData[uiIndex], uiNameSize);
    pName[uiNameSize] = '\0';
    uiIndex += uiNameSize;

    tag.m_Name.assign(pName);

    uint32_t uiValueCount = 0;

    if ((uiIndex + 4) > uiDataSize)
    { 
        return NULL;
    }

    uiValueCount = ReadUint32(&pData[uiIndex]);

    uiIndex += 4;

    for (uint32_t uiValue = 0; uiValue < uiValueCount; uiValue++)
    {
        uint32_t uiValueLength = 0;
        XtraTagValue tagValue = XtraTagValue::Deserialize(&pData[uiIndex], uiDataSize - uiIndex, &uiValueLength);
        uiIndex += uiValueLength;

        tag.m_Values.push_back(tagValue);
    }

    *puiProcessed = uiIndex;

    return tag;
}

bool XtraTag::ClearValues()
{
    try {
        m_Values.clear();
    } catch (...) {
        return false;
    }
    return true;
}

bool XtraTag::AddValue(const XtraTagValue& value)
{
    try {
        m_Values.push_back(value);
    } catch (...) {
        return false;
    }
    return true;
}

bool XtraTag::SetValues(const TAG_VALUE_LIST& valueList)
{
    try {
        m_Values = valueList;
    }
    catch (...) {
        return false;
    }
    return true;
}
