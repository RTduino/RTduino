/*
 * Wrappers to make CAN available with C linkage.
 * This allows C++ methods to be called from C code.
 *
 * /* Copyright 2019 The MathWorks, Inc. */

#include "MW_ArduinoCAN.h"

#if !( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )

#include <Arduino.h>
#include <SPI.h>

#define REG_BFPCTRL                0x0c
#define REG_TXRTSCTRL              0x0d

#define REG_CANCTRL                0x0f

#define REG_CNF3                   0x28
#define REG_CNF2                   0x29
#define REG_CNF1                   0x2a

#define REG_CANINTE                0x2b
#define REG_CANINTF                0x2c

#define FLAG_RXnIE(n)              (0x01 << n)
#define FLAG_RXnIF(n)              (0x01 << n)
#define FLAG_TXnIF(n)              (0x04 << n)

#define REG_RXFnSIDH(n)            (0x00 + (n * 4))
#define REG_RXFnSIDL(n)            (0x01 + (n * 4))
#define REG_RXFnEID8(n)            (0x02 + (n * 4))
#define REG_RXFnEID0(n)            (0x03 + (n * 4))

#define REG_RXMnSIDH(n)            (0x20 + (n * 0x04))
#define REG_RXMnSIDL(n)            (0x21 + (n * 0x04))
#define REG_RXMnEID8(n)            (0x22 + (n * 0x04))
#define REG_RXMnEID0(n)            (0x23 + (n * 0x04))

#define REG_TXBnCTRL(n)            (0x30 + (n * 0x10))
#define REG_TXBnSIDH(n)            (0x31 + (n * 0x10))
#define REG_TXBnSIDL(n)            (0x32 + (n * 0x10))
#define REG_TXBnEID8(n)            (0x33 + (n * 0x10))
#define REG_TXBnEID0(n)            (0x34 + (n * 0x10))
#define REG_TXBnDLC(n)             (0x35 + (n * 0x10))
#define REG_TXBnD0(n)              (0x36 + (n * 0x10))

#define REG_RXBnCTRL(n)            (0x60 + (n * 0x10))
#define REG_RXBnSIDH(n)            (0x61 + (n * 0x10))
#define REG_RXBnSIDL(n)            (0x62 + (n * 0x10))
#define REG_RXBnEID8(n)            (0x63 + (n * 0x10))
#define REG_RXBnEID0(n)            (0x64 + (n * 0x10))
#define REG_RXBnDLC(n)             (0x65 + (n * 0x10))
#define REG_RXBnD0(n)              (0x66 + (n * 0x10))

#define REG_EFLG                    0x2d

#define FLAG_IDE                   0x08
#define FLAG_SRR                   0x10
#define FLAG_RTR                   0x40
#define FLAG_EXIDE                 0x08

#define FLAG_RXM0                  0x20
#define FLAG_RXM1                  0x40


typedef struct
{
    uint8_T     status;
    uint8_T     extended;
    uint32_T    ID;
    uint8_T     Length;
    uint8_T     Remote;
    uint8_T     Error;
    uint8_T     Data[8];
}CANMsgType;

#ifdef MW_NUM_CAN_RECEIVE_RAW
volatile CANMsgType globalCANRxBuffer[MW_NUM_CAN_RECEIVE_RAW];
#endif
volatile CANMsgType newCANRxBuffer;
static uint8_T canRxIdAssigner = 0;

SPISettings _spiSettings(_RTT_SPI_CLOCK_, _RTT_SPI_BITORDER_, _RTT_SPI_MODE_);
int32_T _rxId;
uint8_T _rxExtended;
bool _rxRtr;
uint8_T _rxDlc;
uint8_T _rxLength;
uint8_T _rxData[8];
uint8_T rxErrors;

static uint8_T mcpInitialized = false;
static uint8_T mcpInterruptInitialized = false;
static uint8_T val=0;
extern "C" void MW_GetCANBaud(uint8_T* value1,uint8_T* value2,uint8_T* value3)
{
    const uint8_T* cnf = NULL;
    const struct {
        long clockFrequency;
        long baudRate;
        uint8_T cnf[3];
    } CNF_MAPPER[] = {
        {  8, 1000, { 0x00, 0x80, 0x00 } },
        {  8,  500, { 0x00, 0x90, 0x02 } },
        {  8,  250, { 0x00, 0xb1, 0x05 } },
        {  8,  200, { 0x00, 0xb4, 0x06 } },
        {  8,  125, { 0x01, 0xb1, 0x05 } },
        {  8,  100, { 0x01, 0xb4, 0x06 } },
        {  8,   80, { 0x01, 0xbf, 0x07 } },
        {  8,   50, { 0x03, 0xb4, 0x06 } },
        {  8,   40, { 0x03, 0xbf, 0x07 } },
        {  8,   20, { 0x07, 0xbf, 0x07 } },
        {  8,   10, { 0x0f, 0xbf, 0x07 } },
        {  8,    5, { 0x1f, 0xbf, 0x07 } },

        { 16, 1000, { 0x00, 0xd0, 0x82 } },
        { 16,  500, { 0x00, 0xf0, 0x86 } },
        { 16,  250, { 0x41, 0xf1, 0x85 } },
        { 16,  200, { 0x01, 0xfa, 0x87 } },
        { 16,  125, { 0x03, 0xf0, 0x86 } },
        { 16,  100, { 0x03, 0xfa, 0x87 } },
        { 16,   80, { 0x03, 0xff, 0x87 } },
        { 16,   50, { 0x07, 0xfa, 0x87 } },
        { 16,   40, { 0x07, 0xff, 0x87 } },
        { 16,   20, { 0x0f, 0xff, 0x87 } },
        { 16,   10, { 0x1f, 0xff, 0x87 } },
        { 16,    5, { 0x3f, 0xff, 0x87 } },
    };

    for (unsigned int i = 0; i < (sizeof(CNF_MAPPER) / sizeof(CNF_MAPPER[0])); i++) {
        if (CNF_MAPPER[i].clockFrequency == (long)_RTT_CAN_OSC_ && CNF_MAPPER[i].baudRate == (long)_RTT_CAN_BAUD_) {
            cnf = CNF_MAPPER[i].cnf;
            break;
        }
    }

    if (cnf == NULL) {
        return 0;
    }
    *value1 = cnf[0];
    *value2 = cnf[1];
    *value3 = cnf[2];
}

extern "C" void MW_GetCANFilters(uint8_T* allowAll,uint8_T* buffer0Extended,uint32_T* mask0,uint32_T* filter0,uint32_T* filter1,uint8_T* buffer1Extended,uint32_T* mask1,uint32_T* filter2,uint32_T* filter3,uint32_T* filter4,uint32_T* filter5)
{
    *allowAll           = _RTT_CAN_ALLOW_ALL_;
    *buffer0Extended    = _RTT_CAN_BUFFER0_TYPE_;
    *mask0              = _RTT_CAN_MASK0_;
    *filter0            = _RTT_CAN_FILTER0_;
    *filter1            = _RTT_CAN_FILTER1_;
    *buffer1Extended    = _RTT_CAN_BUFFER1_TYPE_;
    *mask1              = _RTT_CAN_MASK1_;
    *filter2            = _RTT_CAN_FILTER2_;
    *filter3            = _RTT_CAN_FILTER3_;
    *filter4            = _RTT_CAN_FILTER4_;
    *filter5            = _RTT_CAN_FILTER5_;
}

extern "C" uint32_T MW_GetCANMillis(void)
{
    return millis();
}

extern "C" uint8_T MW_GetMCPInitStatus(void)
{
    return mcpInitialized;
}

extern "C" void MW_SetMCPInitStatus(void)
{
    mcpInitialized = true;
}

static uint8_T readRegister(uint8_T address)
{
    uint8_T value;
    SPI.beginTransaction(_spiSettings);
    digitalWrite(_RTT_CAN_SPI_SSPIN_, LOW);
    SPI.transfer(0x03);
    SPI.transfer(address);
    value = SPI.transfer(0x00);
    digitalWrite(_RTT_CAN_SPI_SSPIN_, HIGH);
    SPI.endTransaction();

    return value;
}

static void modifyRegister(uint8_T address, uint8_T mask, uint8_T value)
{
    SPI.beginTransaction(_spiSettings);
    digitalWrite(_RTT_CAN_SPI_SSPIN_, LOW);
    SPI.transfer(0x05);
    SPI.transfer(address);
    SPI.transfer(mask);
    SPI.transfer(value);
    digitalWrite(_RTT_CAN_SPI_SSPIN_, HIGH);
    SPI.endTransaction();
}

static void writeRegister(uint8_T address, uint8_T value)
{
    SPI.beginTransaction(_spiSettings);
    digitalWrite(_RTT_CAN_SPI_SSPIN_, LOW);
    SPI.transfer(0x02);
    SPI.transfer(address);
    SPI.transfer(value);
    digitalWrite(_RTT_CAN_SPI_SSPIN_, HIGH);
    SPI.endTransaction();
}

static int parsePacket()
{
    uint8_T receiveBuffer;
    /* Read interrupt flags */
    uint8_T intf = readRegister(REG_CANINTF);

    if (intf & FLAG_RXnIF(0)) {
        /* Receive Buffer 0 Full */
        receiveBuffer = 0;
    } else if (intf & FLAG_RXnIF(1)) {
        /* Receive Buffer 1 Full */
        receiveBuffer = 1;
    } else {
        /* No message received*/
        _rxId = -1;
        _rxExtended = false;
        _rxRtr = false;
        _rxLength = 0;
        return 0;
    }

    /*Check Extended Identifier Flag bit */
    _rxExtended = (readRegister(REG_RXBnSIDL(receiveBuffer)) & FLAG_IDE) ? true : false;

    /* Read Identifier */
    uint32_T idA = ((readRegister(REG_RXBnSIDH(receiveBuffer)) << 3) & 0x07f8) | ((readRegister(REG_RXBnSIDL(receiveBuffer)) >> 5) & 0x07);
    if (_rxExtended) {
        /* In case of extended, read additional bits */
        uint32_T idB = (((uint32_T)(readRegister(REG_RXBnSIDL(receiveBuffer)) & 0x03) << 16) & 0x30000) | ((readRegister(REG_RXBnEID8(receiveBuffer)) << 8) & 0xff00) | readRegister(REG_RXBnEID0(receiveBuffer));

        _rxId = (idA << 18) | idB;
        /* Remote request check */
        _rxRtr = (readRegister(REG_RXBnDLC(receiveBuffer)) & FLAG_RTR) ? true : false;
    } else {
        _rxId = idA;
        /* Remote request check */
        _rxRtr = (readRegister(REG_RXBnSIDL(receiveBuffer)) & FLAG_SRR) ? true : false;
    }
    /* Read datalength */
    _rxDlc = readRegister(REG_RXBnDLC(receiveBuffer)) & 0x0f;

    if (_rxRtr) {
        _rxLength = 0;
    } else {
        _rxLength = _rxDlc;

        for (int i = 0; i < _rxLength; i++) {
            /* Read data bytes */
            _rxData[i] = readRegister(REG_RXBnD0(receiveBuffer) + i);
        }
    }
    /* Clear interrupt */
    modifyRegister(REG_CANINTF, FLAG_RXnIF(receiveBuffer), 0x00);

    /* Store in global buffer for CANBus Type System Object */
    newCANRxBuffer.ID = _rxId;
    newCANRxBuffer.status = 1U;
    newCANRxBuffer.extended = _rxExtended;
    newCANRxBuffer.Length = _rxDlc;
    newCANRxBuffer.Remote = _rxRtr;
    newCANRxBuffer.Error = 0;       //TODO
    for(int j=0;j<8;j++)
    {
        if(j < _rxLength)
        {
            newCANRxBuffer.Data[j] = _rxData[j];
        }
        else
        {
            newCANRxBuffer.Data[j] = 0;
        }

    }
    #ifdef MW_NUM_CAN_RECEIVE_RAW
    /* Store in global buffer for Raw Data Type System Object */
    for(int i=0;i<MW_NUM_CAN_RECEIVE_RAW;i++)
    {
        if((globalCANRxBuffer[i].ID == _rxId) && (globalCANRxBuffer[i].extended == _rxExtended) && (globalCANRxBuffer[i].Length == _rxDlc))
        {
            globalCANRxBuffer[i].status = 1U;
            globalCANRxBuffer[i].extended = _rxExtended;
            globalCANRxBuffer[i].Length = _rxDlc;
            globalCANRxBuffer[i].Remote = _rxRtr;
            globalCANRxBuffer[i].Error = 0;     //TODO
            for(int j=0;j<8;j++)
            {
                if(j < _rxLength)
                {
                    globalCANRxBuffer[i].Data[j] = _rxData[j];
                }
                else
                {
                    globalCANRxBuffer[i].Data[j] = 0;
                }

            }
        }
    }
    #endif
    return _rxDlc;
}

static void readErrors(void)
{
    uint8_T readValue;
    readValue =  readRegister(REG_EFLG);
    /* rxErrors derived from EFLG - ERROR FLAG register. Only receive errors are extracted */
    rxErrors = ((readValue & 0x01) >> 1) | ((readValue & 0x08) >> 2) | ((readValue & 0xC0) >> 4);
    /* Clear error interrupt */
    modifyRegister(REG_CANINTF, 0xE0, 0x00);
}

static void interruptCallback()
{
    uint8_T canIntf;
    canIntf = readRegister(REG_CANINTF);
    if (canIntf == 0) {
        return;
    }
    if((canIntf & 0x20) != 0U)
    {
        readErrors();
    }
    parsePacket();
}

extern "C" void MW_CANInitializeInterrupt()
{
    writeRegister(REG_CANCTRL, 0x80);
    if (readRegister(REG_CANCTRL) != 0x80) {
        return;
    }
    writeRegister(REG_CANCTRL, 0x00);
    if (readRegister(REG_CANCTRL) != 0x00) {
        return;
    }
    if(mcpInterruptInitialized == 0)
    {
        mcpInterruptInitialized = 1;
        pinMode(_RTT_CAN_INTERRPUT_PIN_, INPUT);
        SPI.usingInterrupt(digitalPinToInterrupt(_RTT_CAN_INTERRPUT_PIN_));
        attachInterrupt(digitalPinToInterrupt(_RTT_CAN_INTERRPUT_PIN_), interruptCallback, LOW);
    }
}

#ifdef MW_NUM_CAN_RECEIVE_RAW
extern "C" void MW_GetCANMessageWithID(uint32_T id,uint8_T* data1, uint8_T dataLength, uint8_T* status, uint8_T extended, uint8_T* remote, uint8_T* error)
{
    for(int i=0;i<MW_NUM_CAN_RECEIVE_RAW;i++)
    {
        if((id == globalCANRxBuffer[i].ID) && (extended == globalCANRxBuffer[i].extended))
        {
            for(int j=0;j<8;j++)
            {
                *(data1+j) = globalCANRxBuffer[i].Data[j];
            }
            *status = globalCANRxBuffer[i].status;
            *remote = globalCANRxBuffer[i].Remote;
            globalCANRxBuffer[i].status = 0U;
            break;
        }
    }
    *error = rxErrors;
}
#endif

extern "C" void MW_GetCANMessageNew(uint32_T* id,uint8_T* data1, uint8_T* dataLength, uint8_T* status, uint8_T* extended, uint8_T* remote, uint8_T* error)
{
    for(int j=0;j<8;j++)
    {
        *(data1+j) = newCANRxBuffer.Data[j];
    }
    *dataLength = newCANRxBuffer.Length;
    *status = newCANRxBuffer.status;
    *extended = newCANRxBuffer.extended;
    *remote = newCANRxBuffer.Remote;
    *id = newCANRxBuffer.ID;
    *error = rxErrors;
    newCANRxBuffer.status = 0U;
}

extern "C" void MW_CANAssignIdAndLength(uint32_T id,uint8_T extended,uint8_T msgLength)
{
    #ifdef MW_NUM_CAN_RECEIVE_RAW
    globalCANRxBuffer[canRxIdAssigner].ID = id;
    globalCANRxBuffer[canRxIdAssigner].extended = extended;
    globalCANRxBuffer[canRxIdAssigner].Length = msgLength;
    canRxIdAssigner = canRxIdAssigner + 1U;
    #endif
}

extern "C" uint8_T MW_CANGetChipSelect(void)
{
    return _RTT_CAN_SPI_SSPIN_;
}
#endif /*Rapid Accel condition check*/
/* [EOF] */
