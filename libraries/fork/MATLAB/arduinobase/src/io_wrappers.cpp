/*
 * Wrappers to make I/O functions available with C linkage. This allows C++
 * methods to be called from C code.
 *
 * Copyright 2011-2019 The MathWorks, Inc. */

#include <inttypes.h>
#include "Arduino.h"
#include "rtwtypes.h"

#if (defined(_RTT_TCPIP_EXTMODE_) && (_RTT_TCPIP_EXTMODE_ == 1)) || (defined(_RTT_UDP_) && (_RTT_UDP_ != 0)) || (defined(_RTT_TCP_) && (_RTT_TCP_ != 0) || defined(_RTT_THINGSPEAK_))
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#endif

#if (defined(_ROTH_MKR1000_) || defined(_ROTH_MKRZERO_) || defined(_ROTH_MKRWIFI1010_))
    extern "C" void __libc_init_array(void);
#endif

extern "C" {
    void __cxa_pure_virtual(void);
}

extern "C" void Serial_begin(int port, long r)
{
#if (defined(_ROTH_MEGA2560_) || defined(_ROTH_DUE_) || defined(_ROTH_MEGAADK_))
    switch(port) {
        case 0:
            Serial.begin(r);
            break;
        case 1:
            Serial1.begin(r);
            break;
        case 2:
            Serial2.begin(r);
            break;
        case 3:
            Serial3.begin(r);
            break;
    }
#else

    #if (defined(_ROTH_LEONARDO_) || defined(_ROTH_MKR1000_) || defined(_ROTH_MKRZERO_) || defined(_ROTH_MKRWIFI1010_))
    switch(port) {
        case 0:
           Serial.begin(r);
            break;
        case 1:
           Serial1.begin(r);
            break;
    }
    #else
        Serial.begin(r);
    #endif
#endif
}

extern "C" void Serial_read(int port, int showOutStatus, uint8_t *outData, int16_t *outStatus)
{
    int libFcnOutput;
#if (defined(_ROTH_MEGA2560_) || defined(_ROTH_DUE_) || defined(_ROTH_MEGAADK_))
    switch(port) {
        case 0:
            libFcnOutput = Serial.read();
            break;
        case 1:
            libFcnOutput = Serial1.read();
            break;
        case 2:
            libFcnOutput = Serial2.read();
            break;
        case 3:
            libFcnOutput = Serial3.read();
            break;
    }
#else

#if (defined(_ROTH_LEONARDO_) || defined(_ROTH_MKR1000_) || defined(_ROTH_MKRZERO_) || defined(_ROTH_MKRWIFI1010_))
    switch(port) {
        case 0:
            libFcnOutput = Serial.read();
            break;
        case 1:
            libFcnOutput = Serial1.read();
            break;
    }
#else

            libFcnOutput = Serial.read();
#endif
#endif
            *outData = (uint8_t) libFcnOutput;
            *outStatus = (libFcnOutput != -1);
    }

    extern "C" void Serial_write(int port, uint8_t * c, size_t s)
    {
#if (defined(_ROTH_MEGA2560_) || defined(_ROTH_DUE_) || defined(_ROTH_MEGAADK_))
        switch(port) {
            case 0:
                Serial.write(c, s);
                break;
            case 1:
                Serial1.write(c, s);
                break;
            case 2:
                Serial2.write(c, s);
                break;
            case 3:
                Serial3.write(c, s);
                break;
        }
#else
#if (defined(_ROTH_LEONARDO_) || defined(_ROTH_MKR1000_) || defined(_ROTH_MKRZERO_) || defined(_ROTH_MKRWIFI1010_))
        switch(port) {
            case 0:
                Serial.write(c, s);
                break;
            case 1:
                Serial1.write(c, s);
                break;
        }
#else
        Serial.write(c, s);
#endif
#endif
    }

#if _RTT_UDP_ != 0
    EthernetUDP Udp[_RTT_UDP_];
    IPAddress localIpAddress(_RTT_Local_IP1, _RTT_Local_IP2, _RTT_Local_IP3, _RTT_Local_IP4);
    byte mac[] = { _RTT_Local_MAC1, _RTT_Local_MAC2, _RTT_Local_MAC3, _RTT_Local_MAC4, _RTT_Local_MAC5, _RTT_Local_MAC6 };
    unsigned int localport[_RTT_UDP_] = {_RTT_UDP_LOCAL_PORT_ORDERED};
    int configureSuccess;
    int trialcount=0;
    extern "C" uint8_t MW_EthernetAndUDPBegin(uint8_t udpindex, uint32_t localPort)
    {
        static int only_one_ethernet_begin;
        if (only_one_ethernet_begin==0)
        {
          #if (defined(_RTT_DISABLE_Ethernet_DHCP_) && (_RTT_DISABLE_Ethernet_DHCP_ !=0))
            Ethernet.begin(mac, localIpAddress);
            configureSuccess = 1;
          #else
            configureSuccess = Ethernet.begin(mac);
          #endif
            only_one_ethernet_begin++;

            for(trialcount=0;trialcount<5;trialcount++)
            {
                if(Serial)
                {
                    break;
                }else
                {
                    delay(1000);
                }
            }
            // Depending on if the IP address assignment using DHCP has been successful or not,
            // send the appropriate information back to the host.
            // If External mode over TCP/IP or Wifi, do not print this Info. Information of IP address will be printed as
            // a part of rtiostream_open
          #if (!(defined(_RTT_TCPIP_EXTMODE_) && (_RTT_TCPIP_EXTMODE_ == 1)))
                if (configureSuccess==1)
                {
                   if(Ethernet.localIP()[0] == 0 || Ethernet.localIP()[0] == 255)
                    {
                        /*Tried to assign Static IP, failed due to one of the following reasons :
                         * 1. Ethernet shield is not connected properly with Arduino.
                         * 2. Ethernet shield is not connected to a network.
                         **/
                        Serial.println("<<<IP address: Failed to configure.>>>");
                    }
                    else
                    {
                        // If the Configuration is successful, relay back the assigned IP address.
                        Serial.print("<<<IP address: ");
                        for (byte thisByte = 0; thisByte < 4; thisByte++)
                        {
                            // Print the value of each byte of the IP address:
                            Serial.print(Ethernet.localIP()[thisByte], DEC);
                            if (thisByte<3)
                            {
                                Serial.print(".");
                            }
                        }
                        Serial.println(">>>");
                    }
                }
               else
               {
                   // If the Configuration failed,relay back the error message.
                   Serial.println("<<<IP address: Failed to configure.>>>");
               }
          #endif
        }
        return (Udp[udpindex].begin(localport[udpindex]));
    }

    extern "C" int MW_UDPFinalWrite(uint8_t udpindex, uint8_t data, uint8_t remoteIPOctect1,uint8_t remoteIPOctect2,uint8_t remoteIPOctect3,uint8_t remoteIPOctect4, uint32_t remoteport)
    {
        IPAddress remoteIpAddress(remoteIPOctect1,remoteIPOctect2, remoteIPOctect3, remoteIPOctect4);
        Udp[udpindex].beginPacket(remoteIpAddress, remoteport);
        Udp[udpindex].write(data);
        return(Udp[udpindex].endPacket());
    }

    extern "C" void MW_UDPFinalRead(uint8_t udpindex, uint8_t *data, int32_t *outStatus)
    {
        int packetSize = Udp[udpindex].parsePacket();
        if(packetSize)
        {
            int libFcnOutput;
            libFcnOutput = Udp[udpindex].read();
            *data = (uint8_t) libFcnOutput;
            *outStatus = (libFcnOutput != -1);
        }

        else
        {
            //When the client is not connected the status flag is accordingly toggled.
            *outStatus = 0;
        }
    }
#endif

#if defined(_RTT_THINGSPEAK_) || (_RTT_TCP_ != 0)
    IPAddress localIpAddress(_RTT_Local_IP1, _RTT_Local_IP2, _RTT_Local_IP3, _RTT_Local_IP4);
    byte mac[] = { _RTT_Local_MAC1, _RTT_Local_MAC2, _RTT_Local_MAC3, _RTT_Local_MAC4, _RTT_Local_MAC5, _RTT_Local_MAC6 };
    int configureSuccess;
    int trialcount;
#if _RTT_TCP_ != 0
    EthernetServer server[_RTT_TCP_] = {_RTT_TCP_SERVER_PORT_ORDERED};
#endif
    extern "C" void MW_EthernetAndTCPServerBegin(uint8_t tcpindex, uint32_t serverport)
    {
    static int only_one_tcp_server_and_ethernet_begin = 0;

        if (only_one_tcp_server_and_ethernet_begin==0)
        {
          #if (defined(_RTT_DISABLE_Ethernet_DHCP_) && (_RTT_DISABLE_Ethernet_DHCP_ !=0))
            Ethernet.begin(mac, localIpAddress);
            configureSuccess = 1;
          #else
            configureSuccess = Ethernet.begin(mac);
          #endif
            only_one_tcp_server_and_ethernet_begin++;

             for(trialcount=0;trialcount<5;trialcount++)
            {
                if(Serial)
                {
                    break;
                }else
                {
                    delay(1000);
                }
            }

            // Depending on if the IP address assignment using DHCP has been successful or not,
            // send the appropriate information back to the host.
            // If External mode over TCP/IP or Wifi, do not print this Info. Information of IP address will be printed as
            // a part of rtiostream_open
          #if (!(defined(_RTT_TCPIP_EXTMODE_) && (_RTT_TCPIP_EXTMODE_ == 1)))
                if (configureSuccess==1)
                {
                   if(Ethernet.localIP()[0] == 0 || Ethernet.localIP()[0] == 255)
                    {
                        /*Tried to assign Static IP, failed due to one of the following reasons :
                         * 1. Ethernet shield is not connected properly with Arduino.
                         * 2. Ethernet shield is not connected to a network.
                         **/
                        Serial.println("<<<IP address: Failed to configure.>>>");
                    }
                    else
                    {
                        // If the Configuration is successful, relay back the assigned IP address.
                        Serial.print("<<<IP address: ");
                        for (byte thisByte = 0; thisByte < 4; thisByte++)
                        {
                            // Print the value of each byte of the IP address:
                            Serial.print(Ethernet.localIP()[thisByte], DEC);
                            if (thisByte<3)
                            {
                                Serial.print(".");
                            }
                        }
                        Serial.println(">>>");
                    }
                }
               else
               {
                   // If the Configuration failed,relay back the error message.
                   Serial.println("<<<IP address: Failed to configure.>>>");
               }
          #endif

          #if _RTT_TCP_ != 0
                 // Begin all servers based on the number of different TCP ports.
               for (int i = 0;i < _RTT_TCP_;i++)
               {
                     server[i].begin();
                }
          #endif
          only_one_tcp_server_and_ethernet_begin = 1;
        }
    }
#endif
#if _RTT_TCP_ != 0
    extern "C" void MW_TCPFinalread(uint8_t tcpindex, uint8_t *data, uint32_t serverport, int32_t *outStatus)
    {
        int libFcnOutput;
        EthernetClient client = server[tcpindex].available();
        if (int(client) == true) {
            libFcnOutput = client.read();
            *data = (uint8_t) libFcnOutput;
            *outStatus = (libFcnOutput != -1);

        }
        else
        {
            //When the client is not connected the status flag is accordingly toggled.
            *outStatus = 0;
        }

    }
    extern "C" void MW_TCPFinalWrite(uint8_t tcpindex, uint8_t data, uint32_t serverport)
    {
        server[tcpindex].write(data);
    }
#endif


#if defined(_ROTH_LEONARDO_)
extern "C" void MW_usbattach()
{
    USBDevice.attach();
}
#endif

#if (defined(_ROTH_MKR1000_) || defined(_ROTH_MKRZERO_) || defined(_ROTH_MKRWIFI1010_))
extern "C" void MW_usbattach()
{
  __libc_init_array();
  USBDevice.init();
  USBDevice.attach();

}
#endif

extern "C" void MW_pinModeAnalogInput(uint32_t pinNumber)
{
    if (pinNumber < A0)
        pinNumber += A0;
    pinMode(pinNumber, INPUT);
}

extern "C" void MW_delay_in_milliseconds(uint32_T period)
{
    uint32_T start_time = 0;
    start_time = millis();
    while(millis()- start_time <= period)
    {
        1;
    }
}
