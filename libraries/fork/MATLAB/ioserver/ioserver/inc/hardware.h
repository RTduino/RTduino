#ifndef HWOBJECT
#define HWOBJECT

#include "LibraryBase.h"
#include "IO_include.h"
#include "IO_packet.h"

#define MAX_NUM_LIBRARIES 16


class  Hardware {

    public:
        Hardware()
        {
          noOfLibraries = 0;
        }
    void registerLibrary(LibraryBase *l)
        {
            //Register the library in the hardware object
            arrayOfLibraries[noOfLibraries]=l;
            noOfLibraries++;
        }

    LibraryBase* arrayOfLibraries[MAX_NUM_LIBRARIES];
    int     noOfLibraries;
};

extern void sendResponseMsg(uint8_T commandID, uint8_T* dataOut, uint16_T payloadSize);

#endif
