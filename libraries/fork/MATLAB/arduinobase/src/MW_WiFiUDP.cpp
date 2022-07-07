/*
 * Wrappers to make WiFi UDP Send functions available with C linkage. This allows C++
 * methods to be called from C code.
 *
 * Copyright 2017-2019 The MathWorks, Inc. */
#include "MW_WiFiUDP.h"
#if !( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
       #include <inttypes.h>
       #include "Arduino.h"
       #if ((defined(_RTT_WIFI_EXTMODE_) && (_RTT_WIFI_EXTMODE_ == 1)) || (defined(_RTT_WIFI_UDP_) && (_RTT_WIFI_UDP_ != 0)) || (defined(_RTT_WIFI_TCP_) && (_RTT_WIFI_TCP_ != 0))|| defined(_RTT_THINGSPEAK_WIFI_)) && (!defined(_RTT_WIFI_ESP8266_))
           #if (defined(_ROTH_MKR1000_))
                #include <WiFi101.h>
            #elif (defined(_ROTH_MKRWIFI1010_))
                #include <WiFiNINA.h>
            #else
                #include <WiFi.h>
            #endif
            #include <WiFiUdp.h>
            #include <SPI.h>
       #endif

       #ifdef _RTT_WIFI_ESP8266_
           #include "ESP8266.h"
           ESP8266 esp8266obj;
       #endif

        /*#define that helps to stringify build flags
        //Double evaluation is needed so that the double quotes can be derived out
        //of the build flag and can be assigned to a character array */
        #define RTT_StringifyBuildFlag(x) RTT_StringParamExpanded(x)
        #define RTT_StringParamExpanded(x)  #x

        #if _RTT_WIFI_UDP_ != 0
            #ifndef _RTT_WIFI_ESP8266_
            WiFiUDP WifiUdpObj[_RTT_WIFI_UDP_];
            unsigned int wifilocalport[_RTT_WIFI_UDP_] = {_RTT_WIFI_UDP_LOCAL_PORT_ORDERED};
            int configureSuccess=WL_IDLE_STATUS;
            int trialcount;
            #endif
            IPAddress wifiLocalIpAddress(_RTT_WIFI_Local_IP1, _RTT_WIFI_Local_IP2, _RTT_WIFI_Local_IP3, _RTT_WIFI_Local_IP4);
            char ssid[] = RTT_StringifyBuildFlag(_RTT_WIFI_SSID);

            #ifdef _RTT_WIFI_WEP
                char key[] = RTT_StringifyBuildFlag(_RTT_WIFI_KEY);
                int keyIndex = _RTT_WIFI_KEY_INDEX;
            #endif

            #ifdef _RTT_WIFI_WPA
                char wpapass[] = RTT_StringifyBuildFlag(_RTT_WIFI_WPA_PASSWORD);
            #endif

            #ifdef _RTT_WIFI_ESP8266_
                uint8_t hardSerialPort = _RTT_WIFI_ESP8266_PORT;
            #endif

            extern "C" uint8_t MW_WifiAndUDPBegin(uint8_t wifiudpindex,uint8_t remoteIPOctet1,uint8_t remoteIPOctet2,uint8_t remoteIPOctet3,uint8_t remoteIPOctet4, uint16_t remoteport, uint16_t localport,uint8_t isSender)
            {
                #ifdef _RTT_WIFI_ESP8266_
                esp8266obj.setHardwareSerialPort(hardSerialPort);
                esp8266obj.beginSerial();
                esp8266obj.resetChip();
                if (esp8266obj.testSerialConnection())
                {
                    //Connection success
                    //TO DO : Notify the user that the connection is successful
                }
                else
                {
                    //Connection failed
                    //TO DO : Show error message the the connection is failed
                    //and troubleshooting steps.
                }
                esp8266obj.setMode(EspWiFi::WifiMode::WIFI_MODE_STATION);

                #ifdef _RTT_WIFI_WPA
                bool connectionSuccess = esp8266obj.connectToWifi(ssid,wpapass);
                #endif

                #ifdef _RTT_WIFI_NONE
                bool connectionSuccess = esp8266obj.connectToWifi(ssid);
                #endif

                #if (defined(_RTT_DISABLE_Wifi_DHCP_) && (_RTT_DISABLE_Wifi_DHCP_ !=0))
                esp8266obj.setStaticIP(wifiLocalIpAddress);
                #endif

                #if (!(defined(_RTT_WIFI_EXTMODE_) && (_RTT_WIFI_EXTMODE_ == 1)))
                if (connectionSuccess)
                {
                    // Successfully connected to Wi-Fi, relay back the assigned IP address.
                    IPAddress ip = esp8266obj.getIpAddress();
                    IPAddress invalidIp(0,0,0,0);
                    if(ip == invalidIp)
                    {
                        Serial.println("<<< IP address :Failed ESP8266. >>>");
                    }
                    else
                    {
                        Serial.print("<<<IP address: ");
                        Serial.print(ip);
                        Serial.println(">>>");
                    }
                }
                else
                {
                    //Connection to Wi-Fi failed,relay back the error message.
                    Serial.println("<<< IP address :Failed ESP8266. >>>");
                }
                esp8266obj.setMux(true);
                if(isSender == 1)
                {
                    IPAddress remoteIpAddress(remoteIPOctet1,remoteIPOctet2,remoteIPOctet3,remoteIPOctet4);
                    esp8266obj.setTcpIpConnection(EspWiFi::TcpipConnectionType::WIFI_UDP, true, remoteIpAddress, remoteport, localport, true);
                }
                else
                {
                    IPAddress remoteIpAddress(0,0,0,0);
                    esp8266obj.setTcpIpConnection(EspWiFi::TcpipConnectionType::WIFI_UDP, false, remoteIpAddress, 0, localport, true);
                }
                #endif

                #else/*Not _RTT_WIFI_ESP8266_, another Wi-Fi shield or MKR1000*/
                static int only_one_wifi_begin;
                if (only_one_wifi_begin==0)
                {
                    while(configureSuccess!=WL_CONNECTED)
                    {
                        #if (defined(_RTT_DISABLE_Wifi_DHCP_) && (_RTT_DISABLE_Wifi_DHCP_ !=0))
                            WiFi.config(wifiLocalIpAddress);
                        #endif

                        #ifdef _RTT_WIFI_WEP
                            configureSuccess=WiFi.begin(ssid,keyIndex,key);
                        #endif

                        #ifdef _RTT_WIFI_WPA
                            configureSuccess=WiFi.begin(ssid,wpapass);
                        #endif

                        #ifdef _RTT_WIFI_NONE
                            configureSuccess=WiFi.begin(ssid);
                        #endif

                        //Wait for 10 seconds for Wifi connection to happen
                        delay(10000);
                    }
                    for(trialcount=0;trialcount<5;trialcount++)
                    {
                        if(Serial)
                        {
                            break;
                        }
                        else
                        {
                            delay(1000);
                        }
                    }

                    //Relay the IP address assigned to the WiFi shield back to the host
                    #if (!(defined(_RTT_WIFI_EXTMODE_) && (_RTT_WIFI_EXTMODE_ == 1)))
                        if (configureSuccess==WL_CONNECTED)
                        {
                            // If the Configuration is successful, relay back the assigned IP address.
                            IPAddress ip = WiFi.localIP();
                            Serial.print("<<<IP address: ");
                            Serial.print(ip);
                            Serial.println(">>>");
                        }
                        else
                        {
                            // If the Configuration failed,relay back the error message.
                            Serial.println("<<< IP address :Failed to configure. >>>");
                        }
                    #endif

                    /* Initialize UDP connections*/
                    for(int i = 0;i < _RTT_WIFI_UDP_;i++){
                       WifiUdpObj[i].begin(wifilocalport[i]);
                    }
                    only_one_wifi_begin++;
                }
                return 0;
                #endif/*_RTT_WIFI_ESP8266_*/
            }

            extern "C" int MW_WifiUDPFinalWrite(uint8_t wifiudpindex, void* data, uint8_t remoteIPOctet1,uint8_t remoteIPOctet2,uint8_t remoteIPOctet3,uint8_t remoteIPOctet4, uint16_t remoteport, uint16_t dataSize)
            {
                #ifdef _RTT_WIFI_ESP8266_
                esp8266obj.tcpipSend((uint8_t*)data, dataSize);
                #else
                IPAddress remoteIpAddress(remoteIPOctet1, remoteIPOctet2, remoteIPOctet3, remoteIPOctet4);
                WifiUdpObj[wifiudpindex].beginPacket(remoteIpAddress, remoteport);
                WifiUdpObj[wifiudpindex].write((uint8_t *)data, (size_t)dataSize);
                return(WifiUdpObj[wifiudpindex].endPacket());
                #endif

            }

            extern "C" void MW_WifiUDPFinalRead(uint8_t wifiudpindex, uint8_t *data, uint16_t dataSizeinBytes, uint16_t dataTypeLength, uint16_t *outSize)
            {
                #ifdef _RTT_WIFI_ESP8266_

                uint16_t packetSizeinBytes = esp8266obj.tcpipReceive(data, dataSizeinBytes, true);

                if (packetSizeinBytes > 0)
                {
                    if(packetSizeinBytes == dataSizeinBytes)
                    {
                        //do nothing
                    }
                    else if(packetSizeinBytes < dataSizeinBytes)
                    {
                        for(uint16_t count = packetSizeinBytes; count < dataSizeinBytes; count++)
                        {
                            data[count] = 0;
                        }
                    }
                    else
                    {
                        //do nothing
                    }
                    *outSize = packetSizeinBytes/dataTypeLength;
                }
                else
                {
                    for(uint16_t count = 0; count < dataSizeinBytes; count++)
                    {
                        *data++ = 0;
                    }
                    *outSize = 0;
                }

                #else
                /*Check available data bytes in the Packet*/
                int packetSizeinBytes = WifiUdpObj[wifiudpindex].parsePacket();
                if(packetSizeinBytes > 0)
                {
                    int  sizeReceived = 0;
                    /*When available number of data bytes in Packet is=
                     *requested number of bytes output requested number of
                     *bytes
                     */
                    if (packetSizeinBytes == dataSizeinBytes)
                    {
                        sizeReceived = WifiUdpObj[wifiudpindex].read(data, (size_t)dataSizeinBytes);
                    }
                    /*When available number of bytes in Packet is <
                     *requested number of bytes output
                     *received number of data bytes appended with zeros
                     *to make the output data of same size as dataSizeinBytes
                     */
                    if (packetSizeinBytes < dataSizeinBytes)
                    {
                        sizeReceived = WifiUdpObj[wifiudpindex].read(data, (size_t)dataSizeinBytes);
                        for(uint16_t count = sizeReceived; count < dataSizeinBytes; count++)
                        {
                            data[count] = 0;
                        }
                    }

                    /*When available number of bytes in Packet is >
                     *requested number of bytes output
                     *data[0:(dataSizeinBytes-1)] = actual data and
                     *trash extra available bytes
                     */
                    if (packetSizeinBytes > dataSizeinBytes)
                    {
                        int extraBytes = packetSizeinBytes - dataSizeinBytes;
                        uint8_t tmpBuf[extraBytes];
                        int tmpSize;

                        sizeReceived = WifiUdpObj[wifiudpindex].read(data, (size_t)dataSizeinBytes);
                        tmpSize = WifiUdpObj[wifiudpindex].read(tmpBuf, (size_t)extraBytes);
                    }

                    /* Output size = received number of bytes represented in
                     *terms of requested datatype.
                     */
                    *outSize = (uint16_t)(packetSizeinBytes/dataTypeLength);
                }

                else
                {
                    /*When no data is available in Packet output:
                     *data[0:dataSizeinBytes-1] = 0
                     *and size = 0.
                     */
                    for(uint16_t count = 0; count < dataSizeinBytes; count++)
                    {
                        *data++ = 0;
                    }
                    *outSize = 0;
                }/*packetSizeinBytes >= dataSizeinBytes*/
                #endif /*_RTT_WIFI_ESP8266_*/
            }/*MW_WifiUDPFinalRead*/

         #endif /*_RTT_WIFI_UDP_ != 0*/

#endif /*Rapid Accel condition check*/
