#pragma once
#include <stdint.h>
#include <memory.h>
#include <string.h>

inline
uint32_t ReadUint32(uint8_t const* pData)
{
    return (uint32_t)((pData[0] << 24) | (pData[1] << 16) | (pData[2] << 8) | pData[3]);
}

inline
void WriteUint32(uint8_t * pData, uint32_t value)
{
    pData[0] = (value >> 24) & 0xFF;
    pData[1] = (value >> 16) & 0xFF;
    pData[2] = (value >> 8) & 0xFF;
    pData[3] = value & 0xFF;
}

inline
uint16_t ReadUint16(uint8_t const* pData)
{
    return (uint16_t)((pData[0] << 8) | pData[1]);
}

inline
void WriteUint16(uint8_t* pData, uint16_t value)
{
    pData[0] = (value >> 8) & 0xFF;
    pData[1] = value & 0xFF;
}

enum XtraValueType {
    XTRA_VT_UNKNOWN = 0,
    XTRA_VT_UNICODE = 8,
    XTRA_VT_INT64 = 19,
    XTRA_VT_FILETIME = 21,
    XTRA_VT_GUID = 72
};

typedef enum XtraValueType XTRA_VALUE_TYPE;

typedef struct MP4FILETIME_s
{
    unsigned long dwLowDateTime;
    unsigned long dwHighDateTime;
} MP4FILETIME;

typedef struct MP4GUID_s {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[ 8 ];
} MP4GUID;

union XtraValueContent {
    wchar_t* pUnicode;
    uint64_t uint64;
    MP4FILETIME filetime;
    MP4GUID guid;
};

typedef union XtraValueContent XTRA_VALUE_CONTENT;

class XtraTagValue
{
public:
    static XtraTagValue Deserialize(uint8_t const* pData, uint32_t uiDataSize, uint32_t* puiProcessed);

    XtraTagValue() :
        m_ValueType(XTRA_VT_UNKNOWN)
    {
        memset((void*)&m_ValueContent, 0, sizeof(m_ValueContent));
    }

    XtraTagValue(XTRA_VALUE_TYPE type, XTRA_VALUE_CONTENT content) :
        m_ValueType(type)
    {
        switch (m_ValueType)
        {
            case XTRA_VT_UNICODE:
            {
                m_ValueContent.pUnicode = _wcsdup(content.pUnicode);
                break;
            }

            case XTRA_VT_INT64:
            {
                m_ValueContent.uint64 = content.uint64;
                break;
            }

            case XTRA_VT_FILETIME:
            {
                m_ValueContent.filetime = content.filetime;
                break;
            }

            case XTRA_VT_GUID:
            {
                m_ValueContent.guid = content.guid;
                break;
            }
            default:
            {
                memset((void*)&m_ValueContent, 0, sizeof(m_ValueContent));
                break;
            }
        }
    }

    XtraValueType ValueType() const
    {
        return m_ValueType;
    }

    void ValueType(XtraValueType valueType)
    {
        m_ValueType = valueType;
    }

    const XTRA_VALUE_CONTENT ValueContent() const
    {
        return m_ValueContent;
    }

    void ValueContent(const XTRA_VALUE_CONTENT valueContent)
    {
        m_ValueContent = valueContent;
    }

    uint32_t SerializedSize() const;

    uint32_t Serialize(uint8_t* pData, uint32_t uiDataSize) const;

private:
    XTRA_VALUE_TYPE m_ValueType;
    XTRA_VALUE_CONTENT m_ValueContent;
};
