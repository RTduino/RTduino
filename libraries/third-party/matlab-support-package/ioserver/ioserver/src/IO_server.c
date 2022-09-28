/**
 * @file IO_server.c
 *
 * Contains definition of functions used for server scheduling and communication
 *
 * @Copyright 2017-2019 The MathWorks, Inc.
 *
 */

/*
 * Server function - calls the corresponding IO depending on the command from host
 * use_case1- This is the only function if used as a purely IO function
 * use_case2 - Use this as a background task if this file is used in conjunction with Simulink Code Gen work-flow
 *
 */

/*
 * Read the communication channel for commands from MATLAB/Simulink
 */

#include "IO_standardperipherals.h"
#include "IO_server.h"
#include "IO_requestID.h"
#if IO_CUSTOM_ENABLE
#include "customFunction.h"
#endif

static streamMode StreamModeVariable;
static struct streamLinkedList StreamLinkedListVariable;
static uint32_T StreamingDurationCount;
float SchedulerBaseRate = 1;
volatile uint32_T SchedulerCounter = 0;
static volatile boolean_T OverrunFlag = 0;
static distressStatus DistressStatusVariable;
static simIOpacket ErrorPacket;
extern simIOpacket RcvdPacket;
#if CHECKSUM_ENABLE
extern uint8_T crcMallocFailedStreamCon;
#endif
uint8_T processRequest(simIOpacket*);
void clearStreamLinkedListVariableDataMalloc(void);
void sendPacketesInBackground(void);
void initErrorPacker(void);
extern void configureScheduler(float);
extern void disableGlobalInterrupt(void);
extern void disableSchedulerInterrupt(void);
extern void stopScheduler(void);
extern void enableSchedulerInterrupt(void);
extern void enableGlobalInterrupt(void);

#ifdef RUNTIME_DATA
uint8_T customData1 = 50;
uint8_T customData2 = 20;
#endif

void ioServerInit(void)
{

    memset(&StreamModeVariable,0,sizeof(StreamModeVariable));
    memset(&DistressStatusVariable,0,sizeof(DistressStatusVariable));
    memset(&StreamLinkedListVariable,0,sizeof(StreamLinkedListVariable));
    openConnection();           /* open serial channel rtiostream */
    initErrorPacker();                  /* Init error packet and generate the corresponding checksum */
#if IO_STANDARD_ENABLE
    standardPeripheralsInit();  /* Init Standard Peripherals */
#endif
#if IO_CUSTOM_ENABLE
    customFunctionHookInit();   /* Init Custom function */
#endif

}
void connectSimIOResponse(uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    /* connect Response should provide hardware info*/
    *status = 0;
}

#if IO_STACK_HEAP_UTILIZATION
void getHeapPointer(uint8_T *payloadBufferTx, uint16_T *peripheralDataSizeResponse,uint8_T *status)
{
    uint32_T *heapPointer = malloc(sizeof(uint8_T));
    if(NULL!=heapPointer)
    {
        unpack32BitstoByte((uint32_T)heapPointer,payloadBufferTx);
        free(heapPointer);
        *peripheralDataSizeResponse = sizeof(uint32_T);
        *status = 0;
    }
    else
    {
        *status = 1;
    }
}
void getStackPointer(uint8_T *payloadBufferTx, uint16_T *peripheralDataSizeResponse,uint8_T *status)
{
    uint8_T stackPointer;
    unpack32BitstoByte((uint32_T)&stackPointer,payloadBufferTx);
    *peripheralDataSizeResponse = sizeof(uint32_T);
    *status = 0;
}

#endif

/* Function to access the version number of core IO server */
void getCoreVersion(uint8_T* payloadBufferTx,uint16_T* peripheralDataSizeResponse, uint8_T* status)
{

    *peripheralDataSizeResponse += sizeof((uint8_T) Major)+sizeof((uint8_T) Minor)+sizeof((uint8_T) Patch); /* 1 byte each for Major, Minor and Patch */

    *payloadBufferTx = (uint8_T) Major;
    *(payloadBufferTx+1) = (uint8_T) Minor;
    *(payloadBufferTx+2) = (uint8_T) Patch;
    *status = 0;
}

/* Function to access the target IO server version number */
void getBoardVersion(uint8_T* payloadBufferTx,uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    const char* ptrBoardVersion = BOARD_IO_VERSION;
    *peripheralDataSizeResponse += strlen(ptrBoardVersion);
    memcpy(payloadBufferTx, ptrBoardVersion, *peripheralDataSizeResponse);
    *status = 0;
}

/* Function to provide Board Name */
void getBoardName(uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    const char* ptrBoardName = BOARDNAME;
    *peripheralDataSizeResponse += strlen(ptrBoardName);
    memcpy(payloadBufferTx, ptrBoardName, *peripheralDataSizeResponse);
    *status = 0;
}

/* Function to provide list of libraries */
void getLibList(simIOpacket *packet)
{
    /* If library names are too big to accomodate in the payLoadBufferTx it will overflow and cause errors. Using the same strategy used in Add-on. Instead of writing the string in the
    payLoadBufferTx, pass the string directly to writeToMATLAB function.
    */
    const char* ptrLibInfo = LIBNAMES;
    uint16_T libNameSize = strlen(ptrLibInfo);
    packet->ptrTxPayLoad = (uint8_T*)malloc((size_t)(libNameSize*sizeof(uint8_T)));
    if(!packet->ptrTxPayLoad)
    {
        /* If memory allocation fails, then return status as 1 */
        packet->dataPayloadSize = 0;
        packet->status = 1;
        writeToMATLAB(packet);
    }
    else
    {
        packet->dataPayloadSize = libNameSize;
        memcpy(packet->ptrTxPayLoad, ptrLibInfo, libNameSize);
        packet->status = 0;
        writeToMATLAB(packet);
        free(packet->ptrTxPayLoad);
        packet->ptrTxPayLoad = NULL;
    }
}

/* Function to provide custom data */
void getCustomData(uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    const char* ptrCustomData = CUSTOMDATA;
    *peripheralDataSizeResponse += strlen(ptrCustomData);
    memcpy(payloadBufferTx, ptrCustomData, *peripheralDataSizeResponse);
    *status = 0;
}

/* Function to collect target specific custom data at the runtime. eg - IP adress of the target */
#ifdef RUNTIME_DATA
void getRuntimeData(uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    RUNTIME_DATA;
}
#endif

uint8_T addNode(void)
{
    /* AddNode - Add at the head
     *  malloc failure will return a NULL value */

    struct streamNode *pNode=NULL;
    /* Create a node */

    pNode = (struct streamNode*)malloc(sizeof(struct streamNode));

    if(pNode==NULL)
    {
        return 0;
    }


    if((StreamLinkedListVariable.tail==0))
    {
        /* Adding first Node */
        StreamLinkedListVariable.header = pNode;
        StreamLinkedListVariable.tail   = pNode;
    }
    else
    {
        StreamLinkedListVariable.header->pNextStreamNode = pNode;   /* Previous node points to the new Node */
        StreamLinkedListVariable.header = pNode;
    }
    /* Store the data in linked list */
    pNode->packet.uniqueId = RcvdPacket.uniqueId;
    pNode->packet.requestId = RcvdPacket.requestId;
    pNode->packet.isRawRead = RcvdPacket.isRawRead;
    pNode->packet.ptrRxPayLoad = (uint8_T*)malloc(RcvdPacket.dataPayloadSize);
    if(pNode->packet.ptrRxPayLoad==NULL)
    {
        return 0;
    }
    memcpy(pNode->packet.ptrRxPayLoad, RcvdPacket.ptrRxPayLoad,RcvdPacket.dataPayloadSize);
    pNode->executionRate = 1;
    pNode->pNextStreamNode = 0;                                     /* set the next node to zero */
    pNode->numberOfSamplesFilled = 0;
    pNode->samplesperFrame = 1;
    pNode->buffer = NULL;
    pNode->packet.peripheralDataSizeResponse = RcvdPacket.peripheralDataSizeResponse;
    pNode->packet.status = 0;
    pNode->packet.dataPayloadSize = 0;
    return 1;
}

uint8_T initParse(void)
{
    if( (StreamLinkedListVariable.header == 0) && (StreamLinkedListVariable.tail == 0))
    {
        return 0;
        /*Stream linked list is empty*/
    }
    /*
     * Initialize parsing. Start with the tail.
     * */
    StreamLinkedListVariable.parseNode = StreamLinkedListVariable.tail;
    return 1;

}
uint8_T parseStreamingLinkedList()
{
    uint16_T dataPosition = 0;
    uint8_T DummyPayloadBufferForRawWrite[PAYLOAD_SIZE];

    if(StreamModeVariable.streamModeOn && initParse())
    {
        /* Parse from tail to head
         * Before Calling parseStreamingLinkedList. Call initParse, to initialize Parsing
         * Return value from one node while parsing through the linked list */
        while(StreamLinkedListVariable.parseNode != 0)
        {
            if((((uint16_T)SchedulerCounter) % StreamLinkedListVariable.parseNode->executionRate) == 0)     /* Process if execution rate is integral multiple of scheduler counter */
            {
                /* Throw and error and stop streaming if peripheral data size response is changing */
                /*if(peripheralDataSizeResponse != StreamLinkedListVariable.parseNode->peripheralDataSizeResponse)
                 * {
                 * exitStreamingMode(&status);
                 * sendCompletedRecordingAckMessage();
                 * }*/
                if(StreamLinkedListVariable.parseNode->packet.isRawRead)
                {
                    if(StreamLinkedListVariable.parseNode->numberOfSamplesFilled == 0)
                    {
                        if(StreamLinkedListVariable.parseNode->buffer->header->status == (bufferStatusType)BUFFER_EMPTY)
                        {
                            StreamLinkedListVariable.parseNode->buffer->header->status = (bufferStatusType)BUFFER_FILLING;
                            // Add time stamp in the data
                            if(StreamModeVariable.timeStampEnable)
                            {
                                unpack32BitstoByte((uint32_T)SchedulerCounter,(uint8_T*)StreamLinkedListVariable.parseNode->buffer->header->data);
                            }
                        }
                        else
                        {
                            // Set transmissionOverrun flag in DistressStatusVariable
                            if (DistressStatusVariable.transmissionOverrun == 0)
                            {
                                DistressStatusVariable.transmissionOverrun = 1;
                            }
                        }
                    }
                    if(StreamLinkedListVariable.parseNode->buffer->header->status == (bufferStatusType)BUFFER_FILLING)
                    {
                        dataPosition = (uint16_T)(StreamLinkedListVariable.parseNode->numberOfSamplesFilled)*(uint16_T)(StreamLinkedListVariable.parseNode->packet.peripheralDataSizeResponse);
                        if(StreamModeVariable.timeStampEnable && (StreamLinkedListVariable.parseNode->packet.isRawRead == 1))
                        {
                            dataPosition = dataPosition + (uint16_T)TIMESTAMP_SIZE;
                        }
                        StreamLinkedListVariable.parseNode->packet.ptrTxPayLoad = &StreamLinkedListVariable.parseNode->buffer->header->data[dataPosition];
                        processRequest((simIOpacket *)&StreamLinkedListVariable.parseNode->packet);
                        /* Foreground task packets needs to be queue in FIFO. Whenever Idle task is executed this packets can be sent
                         * As a result the isPriority is set as 0 by sending the value of backgroundThread*/

                        /* Can add a code to check if user has filled more than peripheralDataSizeResponse in payloadBufferTx by setting all the values to magic number before calling processRequest*/
                        StreamLinkedListVariable.parseNode->numberOfSamplesFilled = StreamLinkedListVariable.parseNode->numberOfSamplesFilled + 1;
                        if(StreamLinkedListVariable.parseNode->samplesperFrame == StreamLinkedListVariable.parseNode->numberOfSamplesFilled)
                        {
                            StreamLinkedListVariable.parseNode->buffer->header->status = (bufferStatusType)BUFFER_FULL;
                            StreamLinkedListVariable.parseNode->buffer->header = StreamLinkedListVariable.parseNode->buffer->header->pNextBufferNode;
                            StreamLinkedListVariable.parseNode->numberOfSamplesFilled = 0;
                        }
                    }
                }
                else
                {
                    StreamLinkedListVariable.parseNode->packet.ptrTxPayLoad = DummyPayloadBufferForRawWrite;
                    processRequest((simIOpacket *)&StreamLinkedListVariable.parseNode->packet);
                }

            }
            StreamLinkedListVariable.parseNode = StreamLinkedListVariable.parseNode->pNextStreamNode;
        }
    }
    return 0;
}
void setStreamingModeRates(uint8_T *payloadBufferReceived, uint8_T* status)
{
    uint8_T index=0;
    initParse();                                                                        /* Init parse node to tail of linked list */
    if(StreamLinkedListVariable.parseNode == 0)
    {
        *status = 1;                                                                    /* Send status as 1 if no node is present*/
    }
    else
    {
        *status = 0;                                                                    /* Send status as 0 if at least one node is present*/
    }
    while(StreamLinkedListVariable.parseNode != 0)
    {
        /* Update the Execution Rate of each node */
        StreamLinkedListVariable.parseNode->executionRate = packBytesInto16BitInteger((uint8_T*) &payloadBufferReceived[index]);
        index+=2;/* Each execution rate is 2 bytes */
        StreamLinkedListVariable.parseNode = StreamLinkedListVariable.parseNode->pNextStreamNode;
    }

}

uint8_T deleteTailNode(void)
{
    /* Delete from the Tail */

    struct streamNode *pTempStreamNode = 0;

    if(!StreamLinkedListVariable.tail)
    {
        /* Linked list is empty */
        return 0;
    }

    /*Store the next node after the tail before freeing the tail */
    pTempStreamNode = StreamLinkedListVariable.tail->pNextStreamNode;
    /* Free the received data buffer */
    free(StreamLinkedListVariable.tail->packet.ptrRxPayLoad);
    /* Free the node */
    free(StreamLinkedListVariable.tail);

    /* After deleting a node, point to the next node in the linked list */
    StreamLinkedListVariable.tail = pTempStreamNode;

    return 1;

}

uint8_T deleteHeadNode(void)
{
    /* Delete from the Head */

    struct streamNode *pTempStreamNode = StreamLinkedListVariable.tail;

    if(0 == StreamLinkedListVariable.header)
    {
        /* Linked list is empty */
        return 0;
    }
    else if(StreamLinkedListVariable.tail->pNextStreamNode == 0)
    {
        /* Only one element in the linked list */
        /* Free the received data buffer */
        free(StreamLinkedListVariable.header->packet.ptrRxPayLoad);
        /* Free the node */
        free(StreamLinkedListVariable.header);
        StreamLinkedListVariable.header=NULL;
        StreamLinkedListVariable.tail=NULL;
    }
    else
    {
        while(pTempStreamNode->pNextStreamNode != StreamLinkedListVariable.header)
        {
            pTempStreamNode=pTempStreamNode->pNextStreamNode;
        }
        /* Free the received data buffer */
        free(StreamLinkedListVariable.header->packet.ptrRxPayLoad);
        /* Free the node */
        free(StreamLinkedListVariable.header);
        StreamLinkedListVariable.header = pTempStreamNode;
    }

    return 1;

}

void clearLinkedList(void)
{
    /* Need todo - infinite loop here, fix it? */
    while(deleteTailNode());
    StreamLinkedListVariable.header = NULL;
}


void resetStream(void)
{
    /* delete the linked list */
    clearLinkedList();

}

void configureSchedulerRate(void)
{
    disableGlobalInterrupt();
    disableSchedulerInterrupt();
    configureScheduler(SchedulerBaseRate);
    enableSchedulerInterrupt();
    enableGlobalInterrupt();
}

void setStreamingModeBaseRate(uint8_T* payloadBufferRx, uint8_T* status)
{
    SchedulerBaseRate = hexto32BitFloat(payloadBufferRx);
    if (SchedulerBaseRate>0)
    {
        *status = 0;
    }
    else
    {
        *status = 1;
    }
}

/* Streaming Mode set here */
void startStreamingMode(uint8_T* status)
{
    StreamModeVariable.configureStreamMode = 0x00;
    /* Check if SchedulerBaseRate is greater than zero, Streaming mode is not already started and at least one node is present for streaming */
    if(SchedulerBaseRate > 0 && 0 == StreamModeVariable.streamModeOn && 0!=StreamLinkedListVariable.tail)
    {
        SchedulerCounter = 0;
        StreamModeVariable.streamModeOn = 1;
        configureSchedulerRate();
        /*Set the parse node at the tail of the linked list*/
        StreamLinkedListVariable.parseNode = StreamLinkedListVariable.tail;
        parseStreamingLinkedList();
        *status = 0;
    }
    else
    {
        *status = 1;
    }
}

void exitStreamingMode(uint8_T* status)
{
    if(StreamModeVariable.streamModeOn)
    {
        stopScheduler();
        SchedulerCounter = 0;
        StreamingDurationCount = 0;
        StreamModeVariable.streamModeOn = 0;
        if(StreamModeVariable.exitStreamingMode == 0)
        {
            clearStreamLinkedListVariableDataMalloc();
        }
        *status = 0;
    }
    else
    {
        *status = 1;
    }
}

void startConfigureStreamMode(uint8_T* status)
{
    if(StreamModeVariable.streamModeOn)
    {
        *status = 1;
    }
    else
    {
        /* Reset the header, tail, and parsing Node */
        StreamModeVariable.configureStreamMode = 0x01;
        resetStream();
        StreamingDurationCount = 0;
        *status = 0;
    }
}


void stopConfigureStreamMode(uint8_T* status)
{
    if(1==StreamModeVariable.configureStreamMode && 0==StreamModeVariable.streamModeOn)
    {
        StreamModeVariable.configureStreamMode = 0x00;
        *status = 0;
    }
    else
    {
        *status = 1;
    }
}

void setStreamingDurationCount(uint8_T* payloadBufferRx, uint8_T* status)
{
    StreamingDurationCount = packBytesInto32BitInteger(payloadBufferRx);
    if(StreamingDurationCount > 0)
    {
        *status = 0;
    }
    else
    {
        *status = 1;
    }
}

struct bufferNode* createBuffer(uint16_T dataSize)
{
    struct bufferNode *tempBufferNode = (struct bufferNode*)malloc(sizeof(struct bufferNode));
    if(tempBufferNode == NULL)
    {
        return NULL;
    }
    tempBufferNode->data = (uint8_T*)malloc(dataSize);
    if(tempBufferNode->data == NULL)
    {
        free(tempBufferNode);
        return NULL;
    }
    tempBufferNode->status = BUFFER_EMPTY;
    tempBufferNode->pNextBufferNode = NULL;

    return tempBufferNode;
}

struct bufferNode* prependBuffer(struct bufferNode** head,uint16_T dataSize)
{
    struct bufferNode* newBuffernode = createBuffer(dataSize);
    *head = newBuffernode;
    return *head;
}

uint8_T deleteBufferNode(struct bufferLinkedList* buffer)
{
    /* Delete from the Tail */

    struct bufferNode *pTempBufferNode = 0;

    if(!buffer->tail)
    {
        /* Linked list is empty */
        return 0;
    }

    /*Store the next node after the tail before freeing the tail */
    pTempBufferNode = buffer->tail->pNextBufferNode;
    /* Free the data buffer */
    free(buffer->tail->data);
    buffer->tail->data = NULL;
    /* Free the node */
    free(buffer->tail);

    /* After deleting a node, point to the next node in the linked list */
    buffer->tail = pTempBufferNode;

    return 1;

}
void deleteBuffers(struct bufferLinkedList** buffer)
{
    struct bufferNode *temp = (*buffer)->tail->pNextBufferNode;
    (*buffer)->tail->pNextBufferNode = NULL;
    (*buffer)->tail = temp;
    while(deleteBufferNode(*buffer));
    free(*buffer);
    *buffer = NULL;
}
struct bufferLinkedList* getBuffers(uint16_T dataSize, uint16_T numberOfBuffers)
{
    struct bufferLinkedList *tempBuffer = (struct bufferLinkedList*)malloc(sizeof(struct bufferLinkedList));
    uint16_T index = 1;
    if(tempBuffer != NULL)
    {
        memset(tempBuffer,0,sizeof(struct bufferLinkedList));
        tempBuffer->tail = createBuffer(dataSize);
        if(tempBuffer->tail == NULL)
        {
            free(tempBuffer);
            return NULL;
        }
        tempBuffer->header = tempBuffer->tail;
        while(index!=numberOfBuffers && tempBuffer->header!=NULL && numberOfBuffers>0)
        {
            tempBuffer->header = prependBuffer((struct bufferNode **)&tempBuffer->header->pNextBufferNode,dataSize);
            if(tempBuffer->header == NULL)
            {
                break;
            }
            index++;
        }
        //tempBuffer->parseNode = tempBuffer->tail;
        if(index != numberOfBuffers && numberOfBuffers>0)
        {
            while(deleteBufferNode(tempBuffer));
            free(tempBuffer);
            tempBuffer = NULL;
        }
        else
        {
            tempBuffer->header->pNextBufferNode = tempBuffer->tail;
            tempBuffer->header = tempBuffer->tail;
        }
    }
    return tempBuffer;
}

void setSamplesPerFrame(uint8_T *payloadBufferReceived, uint8_T* status)
{
    uint8_T index = 0;
    uint16_T dataSize = 0;
    uint16_T numberOfBuffers = 0;
    initParse();                                                                        /* Init parse node to tail of linked list */
    numberOfBuffers = packBytesInto16BitInteger((uint8_T*) &payloadBufferReceived[index]);  /* First two bytes are number of buffers */
    index+=2;                                                                           /* Number of buffers are stored in 2 bytes */
    if(StreamLinkedListVariable.parseNode == 0 || numberOfBuffers < 1)
    {
        *status = 1;                                                                    /* Send status as 1 if no node is present*/
    }
    else
    {
        *status = 0;                                                                    /* Send status as 0 if at least one node is present*/

        while(StreamLinkedListVariable.parseNode != 0)
        {
            /* Update the Samples per frame of each node */
            StreamLinkedListVariable.parseNode->samplesperFrame = packBytesInto16BitInteger((uint8_T*) &payloadBufferReceived[index]);
            index+=2;/* Each Samples per frame is 2 bytes */
            if(StreamLinkedListVariable.parseNode->packet.isRawRead)
            {
                StreamLinkedListVariable.parseNode->numberOfSamplesFilled = 0;
                dataSize = (StreamLinkedListVariable.parseNode->samplesperFrame)*(StreamLinkedListVariable.parseNode->packet.peripheralDataSizeResponse);
                if(StreamModeVariable.timeStampEnable)
                {
                    dataSize = dataSize + (uint16_T)TIMESTAMP_SIZE;
                }
                StreamLinkedListVariable.parseNode->buffer = getBuffers(dataSize,numberOfBuffers);
                if(StreamLinkedListVariable.parseNode->buffer == NULL)
                    *status = 2;
            }
            StreamLinkedListVariable.parseNode = StreamLinkedListVariable.parseNode->pNextStreamNode;
        }
        if(*status == 2)
        {
            clearStreamLinkedListVariableDataMalloc();
        }
    }
}
void enableTimestamp(uint8_T* payloadBufferRx, uint8_T* status)
{
    *status = 0;
    if (payloadBufferRx[0]==1)
    {
        StreamModeVariable.timeStampEnable = 1;
    }
    else if (payloadBufferRx[0]==0)
    {
        StreamModeVariable.timeStampEnable = 0;
    }
    else
    {
        *status = 1;
    }
}
void clearStreamLinkedListVariableDataMalloc()
{
    initParse();
    while(StreamLinkedListVariable.parseNode != 0)
    {
        if (StreamLinkedListVariable.parseNode->buffer != 0 && StreamLinkedListVariable.parseNode->packet.isRawRead)
        {
            deleteBuffers((struct bufferLinkedList**) &StreamLinkedListVariable.parseNode->buffer);
        }
        StreamLinkedListVariable.parseNode = StreamLinkedListVariable.parseNode->pNextStreamNode;
    }
}

void getHeapsize(uint8_T *payloadBufferTx, uint16_T *peripheralDataSizeResponse,uint8_T *status)
{
    volatile uint32_T heapSize = 0;
    uint8_T *heapPointer = NULL;
    volatile uint32_T counter = 1;
    disableGlobalInterrupt();

    while(1)
    {
        heapPointer = (uint8_T*) malloc((size_t)(heapSize + counter));
        if(heapPointer == NULL)
        {
            if(counter == 1)
            {
                break;
            }
            heapSize = heapSize + (counter/2);
            counter = 1;
        }
        else
        {
            free(heapPointer);
            /* On Due malloc has bug. It allocates memory more than available on the target. Capping the value to 2048 bytes. */
            #ifdef ARDUINO_ARCH_SAM
            if (heapSize + counter > (uint32_T)2048)
            {
                heapSize = (uint32_T)2048;
                break;
            }
            #endif
            counter*=2;
        }
    }

    enableGlobalInterrupt();
    unpack32BitstoByte(heapSize,payloadBufferTx);
    *peripheralDataSizeResponse = 4;
    *status = 0;
}

void getBufferCreationOverhead(uint8_T *payloadBufferTx, uint16_T *peripheralDataSizeResponse,uint8_T *status)
{
    //Calculating malloc overhead for 1 byte
    uint32_T* ptr1 = (uint32_T*)malloc(sizeof(uint32_T));
    uint32_T* ptr2 = (uint32_T*)malloc(sizeof(uint32_T));
    uint8_T malloc_overhead;
    if(ptr1 != NULL && ptr2 != NULL)
    {
        malloc_overhead = abs(((int32_T)ptr1) - ((int32_T)ptr2)) - sizeof(uint32_T); // Subtracted sizeof(uint32_T) because malloc will have an overhead of uint32_T bytes
        free(ptr1);
        free(ptr2);
        payloadBufferTx[0] = (uint8_T)sizeof(struct bufferNode) + (uint8_T)(malloc_overhead * 2) ; // Twice because malloc will be done twice once for structure and other for data buffer
        *peripheralDataSizeResponse = 1;
        *status = 0;
    }
    else
    {
        *status = 1;
    }
}

void getMaximumPacketSize(uint8_T *payloadBufferTx, uint16_T *peripheralDataSizeResponse, uint8_T *status)
{
    uint16_T maxPacketSize = (uint16_T)MAX_PACKET_SIZE;
    byteUnpack16Bits(payloadBufferTx,&maxPacketSize);
    *peripheralDataSizeResponse = 2;
    *status=0;
}
void initErrorPacker(void)
{
    ErrorPacket.payloadSize = UNIQUEID_SIZE + REQUESTID_SIZE + ISRAWREAD_SIZE + STATUS_SIZE + CHECKSUM_SIZE;           /* No payload */
    ErrorPacket.uniqueId = 0;              /* Error packet is independent of unique ID field*/
    ErrorPacket.isRawRead = 0;             /* Error packet is independent of RawRead byte field*/
    ErrorPacket.status = 0;                /* Error packet is independent of Status field*/
    ErrorPacket.dataPayloadSize = 0;
#if CHECKSUM_ENABLE
    ErrorPacket.requestId = (uint16_T)MALLOCFAILEDSTREAMINGCONFIG;   /* Request ID for malloc failure on creating node for configure streaming */
    crcMallocFailedStreamCon = calculateChecksum((simIOpacket*)&ErrorPacket,0); /* Error packet is independent of Status field*/
#else
    ErrorPacket.requestId = (uint16_T)0;   /* Request ID needs to be updated at the time of Tx the error packet depending upon the error*/
#endif
}
void sendDistressMessage(uint16_T requestId, uint8_T *payloadBufferTx, uint16_T *peripheralDataSizeResponse)
{
    simIOpacket packet;
    packet.uniqueId = 0;
    packet.isRawRead = 0;
    packet.status = 0;
    packet.requestId = requestId;
    packet.ptrTxPayLoad = payloadBufferTx;
    packet.dataPayloadSize = *peripheralDataSizeResponse;
    writeToMATLAB((simIOpacket *)&packet);
}
void sendSchedulerOverrunMessage()
{
    uint16_T peripheralDataSizeResponse = 0;
    uint8_T *payloadBufferTx = NULL;
    sendDistressMessage((uint16_T) SCHEDULEROVERRUN,(uint8_T*)payloadBufferTx, (uint16_T*)&peripheralDataSizeResponse);
}

void sendTransmissionOverrunMessage()
{
    uint16_T peripheralDataSizeResponse = 0;
    uint8_T *payloadBufferTx = NULL;
    sendDistressMessage((uint16_T) TRANSMISSIONOVERRUN,(uint8_T*)payloadBufferTx, (uint16_T*)&peripheralDataSizeResponse);
}

void sendCompletedRecordingAckMessage()
{
    uint16_T peripheralDataSizeResponse=0;
    uint8_T *payloadBufferTx = NULL;
    sendDistressMessage((uint16_T) ESBRECORDINGCOMPLETED,(uint8_T*)payloadBufferTx, (uint16_T*)&peripheralDataSizeResponse);
}

void sendPeripheralDatasizeMismatchMessage(uint16_T* errorRequestId,uint16_T* receivedReponseSize,uint16_T* expectedReponseSize)
{
    uint16_T peripheralDataSizeResponse = 6;
    uint8_T payloadBufferTx[6];
    byteUnpack16Bits(payloadBufferTx,errorRequestId);
    byteUnpack16Bits(&payloadBufferTx[2],receivedReponseSize);
    byteUnpack16Bits(&payloadBufferTx[4],expectedReponseSize);
    sendDistressMessage((uint16_T) PERIPHERALRESPONSESIZEMISMATCH,(uint8_T*)payloadBufferTx, (uint16_T*)&peripheralDataSizeResponse);
}

void checkDistressMessgae()
{
    if(DistressStatusVariable.transmissionOverrun == 1)
    {
        sendTransmissionOverrunMessage();
        DistressStatusVariable.transmissionOverrun = 2;
    }
    if(DistressStatusVariable.schedulerOverrun == 1)
    {
        sendSchedulerOverrunMessage();
        DistressStatusVariable.schedulerOverrun = 2;
    }
    if(DistressStatusVariable.esbRecordingCompleted == 1)
    {
        sendCompletedRecordingAckMessage();
        DistressStatusVariable.esbRecordingCompleted = 2;
    }
}


uint8_T sendStreamingPackets(void)
{
    struct streamNode *ptrTempNode = NULL;
    simIOpacket tempPacket;
    ptrTempNode = (struct streamNode*)StreamLinkedListVariable.tail;
    while(ptrTempNode != 0)
    {
        if(ptrTempNode->packet.isRawRead)
        {
            // Parse from tail to head, and break when at least one packet is sent so that on-demand gets an chance to serve
            while(ptrTempNode->buffer->tail->status == (bufferStatusType)BUFFER_FULL)
            {
                /* Critical section*/
                disableSchedulerInterrupt();
                memcpy(&tempPacket,&ptrTempNode->packet,sizeof(simIOpacket));
                enableSchedulerInterrupt();
                tempPacket.dataPayloadSize = (ptrTempNode->samplesperFrame)*(tempPacket.peripheralDataSizeResponse);
                if(StreamModeVariable.timeStampEnable)
                {
                    tempPacket.dataPayloadSize = tempPacket.dataPayloadSize + (uint16_T)TIMESTAMP_SIZE;
                }
                tempPacket.ptrTxPayLoad = ptrTempNode->buffer->tail->data;
                writeToMATLAB((simIOpacket *)&tempPacket);
                ptrTempNode->buffer->tail->status = (bufferStatusType)BUFFER_EMPTY;
                ptrTempNode->buffer->tail = ptrTempNode->buffer->tail->pNextBufferNode;
                return 1;
            }
        }
        ptrTempNode = ptrTempNode->pNextStreamNode;
    }
    return 0;
}
void sendPacketesInBackground(void)
{
    /* Send packets from the buffers if the status is full */
    sendStreamingPackets();

    if(StreamModeVariable.exitStreamingMode == 1)
    {
        StreamModeVariable.exitStreamingMode = 0;
        while(sendStreamingPackets());
        DistressStatusVariable.esbRecordingCompleted = 1;;
        checkDistressMessgae();
        clearStreamLinkedListVariableDataMalloc();
        memset(&DistressStatusVariable,0,sizeof(DistressStatusVariable));
    }
}
void serverScheduler(void)
{
    uint8_T status = 0;
    SchedulerCounter = SchedulerCounter + 1;
    if(StreamingDurationCount!=0)
    {
        if((((uint32_T)SchedulerCounter)>(StreamingDurationCount)) && StreamModeVariable.streamModeOn)
        {
            StreamModeVariable.exitStreamingMode = 1;
            exitStreamingMode(&status);
            StreamingDurationCount = 0;
            return;
        }
    }
    /* Check for overrun. Protect OverrunFlag against preemption */
    if (OverrunFlag++)
    {
        /* Error as the scheduler is interrupted before the previous scheduler task is completed */
        if(DistressStatusVariable.schedulerOverrun == 0)
        {
            DistressStatusVariable.schedulerOverrun = 1;
        }
        OverrunFlag--;
        return;
    }

    enableSchedulerInterrupt();
    parseStreamingLinkedList();
    disableSchedulerInterrupt();
    OverrunFlag--;
}



uint8_T isConfigureStreamMode(void)
{
    return(StreamModeVariable.configureStreamMode);
}

uint8_T isRequestIdCore(uint16_T requestId)
{
    return (requestId>0xFEFF && requestId<0xFFFF);
}

uint8_T isRequestIdStandardPeripherals(uint16_T requestId)
{
    return (requestId<0x0100);
}
uint8_T readRequest(void)
{
    uint8_T readSuccess = 0;
    readSuccess = readFromMATLAB();
    /* Configure Stream*/
    /* The second condition indicates whether the target has received any packet*/
    if(readSuccess && isConfigureStreamMode())
    {
        /* First execution, will reset the stream, and add node
         *  further execution, will add node
         *  No need to add Stop Configuration node */
        if((STOPCONFIGURESTREAMMODE != RcvdPacket.requestId) && (0 == addNode()))
        {
            /*Error as unable to allocate memory for storing the node*/
            /*Needs to send at priority as any function calls may lead to stack and heap over flow*/
            ErrorPacket.requestId = (uint16_T)MALLOCFAILEDSTREAMINGCONFIG;
#if CHECKSUM_ENABLE
            ErrorPacket.checksum = (uint8_T)crcMallocFailedStreamCon;
#endif
            writeToMATLABImmediate((simIOpacket*) &ErrorPacket);
        }
    }
    return readSuccess;
}

uint8_T processRequest(simIOpacket *packet)
{
    uint8_T funNotFound = 0;
    packet->dataPayloadSize = 0;
    if(isRequestIdCore(packet->requestId))
    {
        switch(packet->requestId)
        {
            case CONNECT:
                /* verify connection is up*/
                connectSimIOResponse(&packet->dataPayloadSize, &packet->status);
                break;

            case SETSTREAMINGMODEBASERATE:
                setStreamingModeBaseRate(packet->ptrRxPayLoad, &packet->status);
                break;

            case SETSTREAMINGMODERATES:
                setStreamingModeRates(packet->ptrRxPayLoad, &packet->status);
                break;

            case STARTSTREAMINGMODE:
                startStreamingMode(&packet->status);
                break;

            case EXITSTREAMINGMODE:
                exitStreamingMode(&packet->status);
                break;

            case SETSTREAMINGDURATION:
                setStreamingDurationCount(packet->ptrRxPayLoad, &packet->status);
                break;

            case STARTCONFIGURESTREAMMODE:
                startConfigureStreamMode(&packet->status);
                break;

            case STOPCONFIGURESTREAMMODE:
                stopConfigureStreamMode(&packet->status);
                break;

            case GETCOREIOSERVERVERSION:
                getCoreVersion(packet->ptrTxPayLoad,&packet->dataPayloadSize,&packet->status);
                break;

            case GETBOARDIOSERVERVERSION:
                getBoardVersion(packet->ptrTxPayLoad,&packet->dataPayloadSize,&packet->status);
                break;

            case GETBOARDINFORMATION:
                getBoardName(packet->ptrTxPayLoad,&packet->dataPayloadSize,&packet->status);
                break;

            case GETLIBRARYINFORMATION:
                getLibList(packet);
                break;

            case GETCUSTOMDATAINFORMATION:
                getCustomData(packet->ptrTxPayLoad,&packet->dataPayloadSize,&packet->status);
                break;

            #ifdef RUNTIME_DATA
            case GETRUNTIMEATA:
                getRuntimeData(packet->ptrTxPayLoad,&packet->dataPayloadSize,&packet->status);
                break;
            #endif

            case SETSAMPLESPERFRAME:
                setSamplesPerFrame(packet->ptrRxPayLoad, &packet->status);
                break;

            case ENABLETIMESTAMP:
                enableTimestamp(packet->ptrRxPayLoad, &packet->status);
                break;

            case GETHEAPSIZE:
                getHeapsize(packet->ptrTxPayLoad,&packet->dataPayloadSize,&packet->status);
                break;

            case GETBUFFERCREATIONOVERHEAD:
                getBufferCreationOverhead(packet->ptrTxPayLoad,&packet->dataPayloadSize,&packet->status);
                break;

            case GETMAXIMUMPACKETSIZE:
                getMaximumPacketSize(packet->ptrTxPayLoad,&packet->dataPayloadSize,&packet->status);
                break;
#if IO_STACK_HEAP_UTILIZATION
            case GETHEAPPOINTER:
                getHeapPointer(packet->ptrTxPayLoad,&packet->dataPayloadSize,&packet->status);
                break;

            case GETSTACKPOINTER:
                getStackPointer(packet->ptrTxPayLoad,&packet->dataPayloadSize,&packet->status);
                break;
#endif

            default:
                funNotFound = 1;
        }
    }
    else if(isRequestIdStandardPeripherals((uint16_T)packet->requestId))
    {
#if IO_STANDARD_ENABLE
        funNotFound = standardPeripherals((uint16_T)packet->requestId,(uint8_T*)packet->ptrRxPayLoad,(uint8_T*)packet->ptrTxPayLoad,(uint16_T*)&packet->dataPayloadSize, (uint8_T*)&packet->status);
#else
        funNotFound = 1;
#endif
    }
    else
    {
#if IO_CUSTOM_ENABLE
        customFunctionHook((uint16_T)packet->requestId,(uint8_T*)packet->ptrRxPayLoad,(uint8_T*)packet->ptrTxPayLoad,(uint16_T*)&packet->dataPayloadSize);
#endif
        funNotFound = 0;

    }
    return funNotFound;
}

void server(uint8_T* payloadBufferRx,uint8_T* payloadBufferTx,uint8_T backgroundThread)
{
    RcvdPacket.ptrRxPayLoad = payloadBufferRx;
    RcvdPacket.ptrTxPayLoad = payloadBufferTx;
    if(backgroundThread)
    {/* background */
        if(readRequest())
        {
            /* processRequest function returns 1 if the request ID's corresponding function was not found */
            if(1 == processRequest((simIOpacket *)&RcvdPacket))
            {
                /* Set bit 1 in RcvdPacket.isRawRead byte to signify the function was not found */
                RcvdPacket.isRawRead |= (uint8_T)(1<<FUNCTION_NOT_FOUND);
                if(isConfigureStreamMode())
                {
                    /* Remove the request from head of configuration link list */
                    deleteHeadNode();
                }
            }
            /* if peripheralDataSizeResponse and RcvdPacket.peripheralDataSizeResponse are not equal throw an error */
            /* In case of Add-on the writeToMATLAB function is called inside sendResponseMsg and it does not use payloadBufferTx to store data. Also, in Add-on the peripheralDataSizeResponse is not known to the host. So the following checks are unneccesary. */
            else if((RcvdPacket.dataPayloadSize != RcvdPacket.peripheralDataSizeResponse && 1 == (RcvdPacket.isRawRead & 0x01))&&(RcvdPacket.ptrTxPayLoad == payloadBufferTx))
            {
                /* if RcvdPacket.status is anything other than zero signify some error.*/
                if(0 == RcvdPacket.status)
                {
                    /* Send an error as expected and received peripheralDataSizeResponse are not equal */
                    sendPeripheralDatasizeMismatchMessage(&RcvdPacket.requestId,&RcvdPacket.dataPayloadSize,(uint16_T*)&RcvdPacket.peripheralDataSizeResponse);
                }
                /* In that case data may or may not be equal to expected always. Send data which has size less than or equal to expected response size */
                if(RcvdPacket.dataPayloadSize>RcvdPacket.peripheralDataSizeResponse)
                {
                    /* Return data only as per expected data bytes and send an error message */
                    RcvdPacket.dataPayloadSize = RcvdPacket.peripheralDataSizeResponse;
                }
                if(isConfigureStreamMode())
                {
                    /* Remove the request from head of configuration link list */
                    deleteHeadNode();
                }
            }
            if(RcvdPacket.ptrTxPayLoad == payloadBufferTx)
            {
                /* In case of Add-on the writeToMATLAB function is called inside sendResponseMsg and it does not use payloadBufferTx to store data */
                writeToMATLAB((simIOpacket *)&RcvdPacket);
            }
        }
        else
        {
            /* Idle Task*/
            checkDistressMessgae();
            if(StreamModeVariable.streamModeOn || StreamModeVariable.exitStreamingMode)
            {
                sendPacketesInBackground();
            }
        }
    }
}



