#include <mp4v2/mp4v2.h>

#include "XtraAtom.h"

XtraAtom* XtraAtom::CreateFromFile(const char* szFileName)
{
    XtraAtom* pAtom = NULL;

    MP4FileHandle h = MP4Read(szFileName);

    if (h == MP4_INVALID_FILE_HANDLE) {
        fprintf(stderr, "Could not open '%s'... aborting\n", szFileName);
        return NULL;
    }

    bool bHaveXtra = MP4HaveAtom(h, "moov.udta.Xtra");

    if (bHaveXtra)
    {
        uint8_t* pData = NULL;
        uint32_t uiDataSize = 0;
        uint32_t uiLength = 0;

        bool bSuccess = MP4GetBytesProperty(h, "moov.udta.Xtra.data", &pData, &uiDataSize);

        if (bSuccess)
        {
            pAtom = XtraAtom::Deserialize(pData, uiDataSize, &uiLength);
        }
    }

    MP4Close(h);

    return pAtom;
}

bool XtraAtom::SaveToFile(const char* szFileName) const
{
    MP4FileHandle h = MP4Modify(szFileName);

    if (h == MP4_INVALID_FILE_HANDLE) {
        fprintf(stderr, "Could not open '%s'... aborting\n", szFileName);
        return false;
    }

    uint8_t* pData = NULL;
    uint32_t uiDataSize = 0;
    uint32_t uiNewLength = 0;

    uiDataSize = SerializedSize();

    pData = new uint8_t[uiDataSize];

    uiNewLength = Serialize(pData, uiDataSize);

    bool bHaveXtra = MP4HaveAtom(h, "moov.udta.Xtra");
    bool bSuccess = false;

    if (!bHaveXtra)
    {
        bSuccess = MP4AddCustomAtom(h, "moov.udta", "Xtra", pData, uiDataSize);
    }
    else
    {
        bSuccess = MP4SetBytesProperty(h, "moov.udta.Xtra.data", pData, uiDataSize);
    }

    MP4Close(h);

    return bSuccess;
}

uint32_t XtraAtom::SerializedSize() const
{
    uint32_t size = 0;

    TAG_MAP::const_iterator iterTags = m_Tags.begin();

    for (; iterTags != m_Tags.end(); iterTags++) {
        size += static_cast<uint32_t>((*iterTags).second.SerializedSize());
    }

    return size;
}

uint32_t XtraAtom::Serialize(uint8_t* pData, uint32_t uiDataSize) const
{
    uint32_t uiIndex = 0;

    TAG_MAP::const_iterator iterTags = m_Tags.begin();

    for (; iterTags != m_Tags.end(); iterTags++) {
        uint32_t uiTagSize = (*iterTags).second.Serialize(&pData[uiIndex], uiDataSize - uiIndex);
        if (uiTagSize == 0) {
            return 0;
        }
        uiIndex += uiTagSize;
    }

    return uiIndex;
}

XtraAtom * XtraAtom::Deserialize(uint8_t const* pData, uint32_t uiDataSize, uint32_t* puiProcessed)
{
    uint32_t uiIndex = 0;

    XtraAtom* pAtom = new XtraAtom();

    while ((uiIndex + 4) <= uiDataSize)
    {
        uint32_t uiTagSize = ReadUint32(&pData[uiIndex]);

        if ((uiIndex + uiTagSize) > uiDataSize)
        {
            return 0;
        }

        uiIndex += 4;
        uiTagSize -= 4;

        uint32_t uiTagProcessedSize = 0;

        XtraTag tag = XtraTag::Deserialize(&pData[uiIndex], uiTagSize, &uiTagProcessedSize);

        if (uiTagProcessedSize == 0)
        {
            break;
        }

        pAtom->m_Tags[tag.Name()] = tag;
        uiIndex += uiTagProcessedSize;
    }

    *puiProcessed = uiIndex;

    return pAtom;
}
