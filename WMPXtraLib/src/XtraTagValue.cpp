#include "XtraTagValue.h"

uint32_t XtraTagValue::SerializedSize() const
{
    uint32_t size = static_cast<uint32_t>(sizeof(uint32_t) + sizeof(uint16_t)); // size of value + value type

    switch (m_ValueType)
    {
        case XTRA_VT_UNICODE:
        {
            size += static_cast<uint32_t>((wcslen(m_ValueContent.pUnicode) + 1) * sizeof(wchar_t));
            break;
        }

        case XTRA_VT_INT64:
        {
            size += static_cast<uint32_t>(sizeof(m_ValueContent.uint64));
            break;
        }

        case XTRA_VT_FILETIME:
        {
            size += static_cast<uint32_t>(sizeof(m_ValueContent.filetime));
            break;
        }

        case XTRA_VT_GUID:
        {
            size += static_cast<uint32_t>(sizeof(m_ValueContent.guid));
            break;
        }
    }

    return size;
}

uint32_t XtraTagValue::Serialize(uint8_t* pData, uint32_t uiDataSize) const
{
    uint32_t uiIndex = 4;

    if ((uiIndex + 2) <= uiDataSize) {
        WriteUint16(&pData[uiIndex], static_cast<uint16_t>(m_ValueType));
        uiIndex += 2;
    }

    size_t size = 0;

    switch (m_ValueType)
    {
        case XTRA_VT_UNICODE: {
            size = (wcslen(m_ValueContent.pUnicode) + 1) * sizeof(wchar_t);
            if ((uiIndex + size) <= uiDataSize) {
                memcpy(&pData[uiIndex], reinterpret_cast<char*>(m_ValueContent.pUnicode), size);
            }
            break;
        }

        case XTRA_VT_INT64: {
            size = sizeof(m_ValueContent.uint64);
            if ((uiIndex + size) <= uiDataSize) {
                memcpy(&pData[uiIndex], reinterpret_cast<const char*>(&m_ValueContent.uint64), size);
            }
            break;
        }

        case XTRA_VT_FILETIME:
        {
            size = sizeof(m_ValueContent.filetime);
            if ((uiIndex + size) <= uiDataSize) {
                memcpy(&pData[uiIndex], reinterpret_cast<const char*>(&m_ValueContent.filetime), size);
            }
            break;
        }

        case XTRA_VT_GUID:
        {
            size = sizeof(m_ValueContent.guid);
            if ((uiIndex + size) <= uiDataSize) {
                memcpy(&pData[uiIndex], reinterpret_cast<const char*>(&m_ValueContent.guid), size);
            }
            break;
        }
    }

    uiIndex += static_cast<uint32_t>(size);

    WriteUint32(pData, uiIndex);

	return uiIndex;
}

XtraTagValue XtraTagValue::Deserialize(uint8_t const * pData, uint32_t uiDataSize, uint32_t *puiProcessed)
{
    uint32_t uiIndex = 0;
    uint32_t uiValueSize = 0;

    XtraTagValue tagValue;

    if (uiDataSize >= 4)
    {
        uiValueSize = ReadUint32(pData);

        uiIndex += 4;

        if (uiDataSize >= uiValueSize)
        {
            if ((uiIndex + 2) <= uiDataSize)
            {
                tagValue.m_ValueType = static_cast<XTRA_VALUE_TYPE>(ReadUint16(&pData[uiIndex]));
                uiIndex += 2;

                if (tagValue.m_ValueType == XTRA_VT_UNICODE) {
                    tagValue.m_ValueContent.pUnicode = new wchar_t[uiValueSize / 2];
                    memcpy(tagValue.m_ValueContent.pUnicode, &pData[uiIndex], static_cast<size_t>(uiValueSize - uiIndex));
                }
                else
                {
                    memcpy((void *)&tagValue.m_ValueContent, &pData[uiIndex], static_cast<size_t>(uiValueSize - uiIndex));
                }
            }
        }
    }

    *puiProcessed = uiValueSize;

    return tagValue;
}
