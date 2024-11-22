// WMPXtraUtil.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <mp4v2/mp4v2.h>

#include "XtraAtom.h"
#include "XtraTag.h"
#include "XtraTagValue.h"

int main(int argc, char** argv)
{
    XtraAtom* pAtom = XtraAtom::CreateFromFile(argv[1]);

    if (pAtom != NULL) {
        uint8_t* pNewData = NULL;
        uint32_t uiNewDataSize = 0;
        uint32_t uiNewLength = 0;

        TAG_MAP::const_iterator iterTag;

        for (iterTag = pAtom->tagBegin(); iterTag != pAtom->tagEnd(); iterTag++) {
            printf("%-30s", iterTag->first.c_str());

            TAG_VALUE_LIST::const_iterator iterValue;
            bool bFirstValue = true;

            for (iterValue = iterTag->second.valuesBegin(); iterValue != iterTag->second.valuesEnd(); iterValue++) {
                printf(bFirstValue ? "    " : "; ");
                bFirstValue = false;

                switch (iterValue->ValueType()) {
                case XTRA_VT_FILETIME: {
                    unsigned long filetime = static_cast<unsigned long>((((unsigned long long)iterValue->ValueContent().filetime.dwHighDateTime << 32) | iterValue->ValueContent().filetime.dwLowDateTime) / 1000);
                    char szTime[80];

                    ctime_s(szTime, sizeof(szTime), reinterpret_cast<time_t*>(&filetime));
                    printf("%s", szTime);
                    break;
                }
                case XTRA_VT_GUID: {
                    break;
                }
                case XTRA_VT_INT64: {
                    printf("%lld (0x%llX)s", iterValue->ValueContent().uint64, iterValue->ValueContent().uint64);
                    break;
                }
                case XTRA_VT_UNICODE: {
                    printf("%ls", iterValue->ValueContent().pUnicode);
                    break;
                }
                case XTRA_VT_UNKNOWN: {
                    break;
                }
                }
            }
            printf("\n");
        }

#if 0
        uiNewDataSize = pAtom->SerializedSize();

        pNewData = new uint8_t[uiNewDataSize];

        uiNewLength = pAtom->Serialize(pNewData, uiNewDataSize);

        printf("Atom Size %ld, processed %ld\n", uiNewDataSize, uiNewLength);

        bool bSuccess = pAtom->SaveToFile(argv[2]);

        printf("Save to %s - %s\n", argv[2], bSuccess ? "True" : "False");
#endif
    }
}
