// WMPXtra.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

        uiNewDataSize = pAtom->SerializedSize();

        pNewData = new uint8_t[uiNewDataSize];

        uiNewLength = pAtom->Serialize(pNewData, uiNewDataSize);

        printf("Atom Size %ld, processed %ld\n", uiNewDataSize, uiNewLength);

        bool bSuccess = pAtom->SaveToFile(argv[2]);

        printf("Save to %s - %s\n", argv[2], bSuccess ? "True" : "False");
    }
}
