/*
 * Wrappers to make WiFi TCP Send functions available with C linkage. This allows C++
 * methods to be called from C code.
 *
 * Copyright 2017-2019 The MathWorks, Inc. */
#include "MW_WiFiTCP.h"
#if !( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
       #include <inttypes.h>
       #include "Arduino.h"
       #if (defined(_RTT_WIFI_EXTMODE_) && (_RTT_WIFI_EXTMODE_ == 1)) || (defined(_RTT_WIFI_UDP_) && (_RTT_WIFI_UDP_ != 0)) || (defined(_RTT_WIFI_TCP_) && (_RTT_WIFI_TCP_ != 0))|| defined(_RTT_THINGSPEAK_WIFI_)
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
           #include "CircularBuffer.h"
           ESP8266 esp8266obj;
           CircularBuffer espTcpDataBuffer(128);
       #endif

        /*#define that helps to stringify build flags
        //Double evaluation is needed so that the double quotes can be derived out
        //of the build flag and can be assigned to a character array */
        #define RTT_StringifyBuildFlag(x) RTT_StringParamExpanded(x)
        #define RTT_StringParamExpanded(x)  #x

    #if (defined(_RTT_THINGSPEAK_WIFI_) || (_RTT_WIFI_TCP_ !=0))
        IPAddress wifiLocalIpAddress(_RTT_WIFI_Local_IP1, _RTT_WIFI_Local_IP2, _RTT_WIFI_Local_IP3, _RTT_WIFI_Local_IP4);
        char ssid[] = RTT_StringifyBuildFlag(_RTT_WIFI_SSID);
        int configureSuccess = WL_IDLE_STATUS;
        int trialcount;
        #if (defined(_RTT_WIFI_TCP_SERVER_) && (_RTT_WIFI_TCP_SERVER_!=0))
             WiFiServer server[_RTT_WIFI_TCP_SERVER_]  = {_RTT_WIFI_TCP_SERVER_PORT_ORDERED};
             WiFiClient client[_RTT_WIFI_TCP_SERVER_] ;
             static int oneTimeServerFlushFlag[7] = {0,0,0,0,0,0,0};
        #endif

        #if (defined(_RTT_WIFI_TCP_CLIENT_) && (_RTT_WIFI_TCP_CLIENT_!=0))
             uint8_t remoteIPOctet1_array[_RTT_WIFI_TCP_CLIENT_]    = {_RTT_WIFI_TCP_REMOTEIPOCTET1_ORDERED};
             uint8_t remoteIPOctet2_array[_RTT_WIFI_TCP_CLIENT_]    = {_RTT_WIFI_TCP_REMOTEIPOCTET2_ORDERED};
             uint8_t remoteIPOctet3_array[_RTT_WIFI_TCP_CLIENT_]    = {_RTT_WIFI_TCP_REMOTEIPOCTET3_ORDERED};
             uint8_t remoteIPOctet4_array[_RTT_WIFI_TCP_CLIENT_]    = {_RTT_WIFI_TCP_REMOTEIPOCTET4_ORDERED};
             uint16_t remoteServerPort_array[_RTT_WIFI_TCP_CLIENT_] = {_RTT_WIFI_TCP_REMOTEPORT_ORDERED}    ;
             WiFiClient localClient[_RTT_WIFI_TCP_CLIENT_];
             static int oneTimeClientFlushFlag[7] = {0,0,0,0,0,0,0};
        #endif

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

        extern "C" void MW_WifiAndWifiTCPServerBegin(uint8_t wifitcpindex, uint16_t serverport, uint8_t isServer, uint8_t remoteIPOctet1,uint8_t remoteIPOctet2,uint8_t remoteIPOctet3,uint8_t remoteIPOctet4)
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
            if(isServer == 1)//TCP Server
            {
                IPAddress remoteIpAddress(0,0,0,0);
                esp8266obj.setTcpIpConnection(EspWiFi::TcpipConnectionType::WIFI_TCP, true,remoteIpAddress, 0, serverport, false);
            }
            else//TCP Client
            {
                IPAddress remoteIpAddress(remoteIPOctet1,remoteIPOctet2,remoteIPOctet3,remoteIPOctet4);
                esp8266obj.setTcpIpConnection(EspWiFi::TcpipConnectionType::WIFI_TCP, true, remoteIpAddress, serverport, 0, true);
                if(esp8266obj.isClientAbleToConnect())
                {
                    Serial.print("**1**");
                }
                else
                {
                    Serial.print("**0**");
                }
            }
            #endif

            #else/*Not _RTT_WIFI_ESP8266_, another Wi-Fi shield or MKR1000*/
            static int only_one_wifi_tcp_server_and_wifi_begin;
            if (only_one_wifi_tcp_server_and_wifi_begin==0)
            {
                /* Begin WiFi*/
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
                        unsigned long startMillis  = millis();
                        while(millis()- startMillis < 10000)
                        {
                        };
                }

                /* Wait for Serial0 to be up*/
                for(trialcount=0;trialcount<5;trialcount++)
                {
                    if(Serial)
                    {
                        break;
                    }else
                    {
                        unsigned long startMillis  = millis();
                        while(millis()- startMillis < 1000)
                        {
                        };
                    }
                }

             #if (!(defined(_RTT_WIFI_EXTMODE_) && (_RTT_WIFI_EXTMODE_ == 1)))
                if (configureSuccess==WL_CONNECTED)
                {
                   // If the Configuration is successful, relay back the assigned IP address.
                   IPAddress ip = WiFi.localIP();
                   Serial.print("<<< IP address :");
                   Serial.print(ip);
                   Serial.println(" >>>");
                }
               else
               {
                   // If the Configuration failed,relay back the error message.
                   Serial.println("<<< IP address :Failed to configure. >>>");
               }
            #endif

              #if (defined(_RTT_WIFI_TCP_CLIENT_) && (_RTT_WIFI_TCP_CLIENT_ !=0))
                   for(int i = 0;i < _RTT_WIFI_TCP_CLIENT_;i++)
                   {
                        IPAddress remoteServerAddress(remoteIPOctet1_array[i],remoteIPOctet2_array[i],remoteIPOctet3_array[i],remoteIPOctet4_array[i]);
                        uint8_t connectStatus = (uint8_t)localClient[i].connect(remoteServerAddress,remoteServerPort_array[i]);
                        Serial.print("**");
                        Serial.print(connectStatus);
                        Serial.println("**");
                   }
              #endif

                #if (defined(_RTT_WIFI_TCP_SERVER_) && (_RTT_WIFI_TCP_SERVER_!=0))
                   // Begin all servers based on the number of different TCP ports.
                    for (int i = 0;i < _RTT_WIFI_TCP_SERVER_;i++)
                    {
                            server[i].begin();
                    }
                #endif

                only_one_wifi_tcp_server_and_wifi_begin ++;
            }
            #endif/*_RTT_WIFI_ESP8266_*/
        }
    #endif /*defined(_RTT_THINGSPEAK_WIFI_) || (_RTT_WIFI_TCP_ !=0))*/

    #if _RTT_WIFI_TCP_ !=0
            extern "C" void MW_WifiTCPFinalWrite(uint8_t wifitcpindex, void *data, uint8_t isServer, uint16_t dataSize)
            {
                #ifdef _RTT_WIFI_ESP8266_
                esp8266obj.tcpipSend((uint8_t*)data, dataSize);
                #else
                 if (!isServer){
                    #if (defined(_RTT_WIFI_TCP_CLIENT_) && (_RTT_WIFI_TCP_CLIENT_!=0))
                            localClient[wifitcpindex].write((uint8_t *)data, (size_t)dataSize);
                            if (!localClient[wifitcpindex].connected()){
                                    localClient[wifitcpindex].stop();
                                }
                    #endif
                 }
                 else{
                    #if (defined(_RTT_WIFI_TCP_SERVER_) && (_RTT_WIFI_TCP_SERVER_!=0))
                         if(!client[wifitcpindex].connected()){
                             /*Check for available clients only when not connected*/
                             client[wifitcpindex] = server[wifitcpindex].available();
                         }
                        if (client[wifitcpindex]) {
                            client[wifitcpindex].write((uint8_t *)data, (size_t)dataSize);
                        }
                    #endif
                 }
                #endif /*_RTT_WIFI_ESP8266_*/
            }/*MW_WifiTCPFinalWrite*/

            extern "C" void MW_WifiTCPFinalread(uint8_t wifitcpindex, uint8_t *data, uint16_t dataSizeinBytes, int32_t *outStatus,uint8_t isServer, uint32_t blockingTimeout)
            {
                #ifdef _RTT_WIFI_ESP8266_

                if(blockingTimeout)
                {
                    if(!checkESP8266BlockingMode(data,dataSizeinBytes,outStatus,blockingTimeout))
                    {
                        //No data received, and timeout expired, so output all zeroes.
                        for(uint16_t count = 0; count < dataSizeinBytes; count++)
                        {
                            *data++ = 0;
                        }
                        *outStatus = 0;
                    }
                }
                else
                {
                    uint16_t packetSizeInBytes = 0;
                    uint8_t dataFromPacket[40];

                    //Read data from the chip
                    packetSizeInBytes = esp8266obj.tcpipReceive(dataFromPacket, dataSizeinBytes, false);

                    int bufferSize = espTcpDataBuffer.getNumberOfDataBytes();

                    if(bufferSize == 0)//circular buffer is empty
                    {
                        if(packetSizeInBytes == 0) //no data is received from ESP8266
                        {
                            //output nothing
                            for(uint16_t count = 0; count < dataSizeinBytes; count++)
                            {
                                *data++ = 0;
                            }
                            *outStatus = 0;
                        }
                        else //some data is received from ESP8266
                        {
                            if(packetSizeInBytes == dataSizeinBytes)//output data,status = 1
                            {
                                memcpy(data,dataFromPacket,dataSizeinBytes);
                                *outStatus = 1;
                            }
                            else if(packetSizeInBytes < dataSizeinBytes)//add data to circular buffer,status = 0
                            {
                                for(uint16_t count = 0; count < packetSizeInBytes; count++)
                                {
                                    espTcpDataBuffer.push(dataFromPacket[count]);
                                }
                                for(uint16_t count = 0; count < dataSizeinBytes; count++)
                                {
                                    data[count] = 0;
                                }
                                *outStatus = 0;
                            }
                            else//output data,status = 1,add excess data to circular buffer
                            {
                                memcpy(data,dataFromPacket,dataSizeinBytes);
                                for(uint16_t count = dataSizeinBytes; count < packetSizeInBytes; count++)
                                {
                                    espTcpDataBuffer.push(dataFromPacket[count]);
                                }
                                *outStatus = 1;
                            }
                        }
                    }
                    else//circular buffer has some data bytes
                    {
                        if(bufferSize >= dataSizeinBytes)//output buffer data, push received data from chip to buffer
                        {
                            for(uint16_t count = 0; count < dataSizeinBytes; count++)
                            {
                                data[count] = espTcpDataBuffer.pop();
                            }
                            *outStatus = 1;
                            for(uint16_t count = 0; count < packetSizeInBytes; count++)
                            {
                                espTcpDataBuffer.push(dataFromPacket[count]);
                            }
                        }
                        else//output buffer data, output received data from chip(if any), push excess data to buffer
                        {
                            int remainingNumOfBytes = dataSizeinBytes - bufferSize;
                            if(packetSizeInBytes >= remainingNumOfBytes)
                            {
                                for(uint16_t count = 0; count < bufferSize; count++)
                                {
                                    data[count] = espTcpDataBuffer.pop();
                                }
                                for(uint16_t count = 0; count < remainingNumOfBytes; count++)
                                {
                                    data[bufferSize+count] = dataFromPacket[count];
                                }
                                *outStatus = 1;
                                //push excess data to buffer
                                for(uint16_t count = remainingNumOfBytes; count < packetSizeInBytes; count++)
                                {
                                    espTcpDataBuffer.push(dataFromPacket[count]);
                                }
                            }
                            else//not enough data available in both buffer and received data combined. Add data to buffer.
                            {
                                for(uint16_t count = 0; count < packetSizeInBytes; count++)
                                {
                                    espTcpDataBuffer.push(dataFromPacket[count]);
                                }
                                for(uint16_t count = 0; count < dataSizeinBytes; count++)
                                {
                                    data[count] = 0;
                                }
                                *outStatus = 0;
                            }
                        }
                    }
                }

                #else
                if (!isServer){
                    #if (defined(_RTT_WIFI_TCP_CLIENT_) && (_RTT_WIFI_TCP_CLIENT_!=0))
                       /*Flush client input buffer at beginning*/
                        if (oneTimeClientFlushFlag[wifitcpindex] == 0)
                        {
                            localClient[wifitcpindex].flush();
                            oneTimeClientFlushFlag[wifitcpindex]++;
                        }
                        /*Check if bytes are available to read*/
                        int availableBytes = localClient[wifitcpindex].available();
                        if(availableBytes)
                        {
                             int sizeReceived = 0;
                            /*When available number of data bytes in TCP frame is > =
                             *requested number of bytes output requested number of
                             *bytes only (leaving the rest in Packet) and status 1.
                             */
                             if (availableBytes >= dataSizeinBytes)
                             {
                                 #if defined(_ROTH_MKR1000_) || defined(_ROTH_MKRWIFI1010_)
                                    sizeReceived = localClient[wifitcpindex].read(data, (size_t)dataSizeinBytes);
                                 #else
                                    /*Using WiFiClient::read() method instead of multiple byte read due to g16048138*/
                                    for(int i=0; i<dataSizeinBytes; i++){
                                        data[i] = localClient[wifitcpindex].read();
                                    }
                                 #endif

                                 *outStatus = 1;
                             }

                             /*When available number of data bytes in TCP frame <
                              *requested number of bytes output:
                              *data[0:dataSizeinBytes-1] = 0
                              *and status 0.
                              */
                             if(availableBytes < dataSizeinBytes)
                             {
                                 /*When blocking mode is enabled, wait for requested number of data bytes or  blockingTimeout.
                                 If requested number of data bytes are not received till blockingTimeout then data = 0, status = 0.*/
                                 if(blockingTimeout)
                                 {
                                    checkDataBlockingMode(wifitcpindex, data, dataSizeinBytes, outStatus, isServer, (unsigned long)blockingTimeout);
                                 }
                                 else
                                 {
                                    for(uint16_t count = 0; count < dataSizeinBytes; count++)
                                    {
                                        *data++ = 0;
                                    }
                                    *outStatus = 0;
                                 }
                             }
                        }
                        else
                        {
                            /*When blocking mode is enabled, wait for requested number of data bytes or  blockingTimeout.
                            If requested number of data bytes are not received till blockingTimeout then data = 0, status = 0.*/
                             if(blockingTimeout)
                             {
                                checkDataBlockingMode(wifitcpindex, data, dataSizeinBytes, outStatus, isServer, (unsigned long)blockingTimeout);
                             }
                             else
                             {
                                /*When no data is available in TCP frame output:
                                 *data[0:dataSizeinBytes-1] = 0
                                 *and status 0.
                                 */
                                for(uint16_t count = 0; count < dataSizeinBytes; count++)
                                {
                                    *data++ = 0;
                                }
                                *outStatus = 0;
                             }
                        }/*availableBytes*/

                        /*If client is disconnected from Server stop the Client*/
                        if (!localClient[wifitcpindex].connected()){
                            localClient[wifitcpindex].stop();
                        }
                    #endif /*_RTT_WIFI_TCP_CLIENT_*/
                }
                else{
                    #if (defined(_RTT_WIFI_TCP_SERVER_) && (_RTT_WIFI_TCP_SERVER_!=0))
                         if(!client[wifitcpindex].connected()){
                             /*Check for available clients only when not connected*/
                             client[wifitcpindex] = server[wifitcpindex].available();
                         }
                        if (client[wifitcpindex]) {
                            /*Flush client input buffer at beginning*/
                            if (oneTimeServerFlushFlag[wifitcpindex] == 0)
                            {
                                client[wifitcpindex].flush();
                                oneTimeServerFlushFlag[wifitcpindex]++;
                            }
                            /*Check if bytes are available to read*/
                            int availableBytes = client[wifitcpindex].available();
                            if(availableBytes)
                            {
                                 int sizeReceived = 0;
                                /*When available number of data bytes in TCP frame is > =
                                 *requested number of bytes output requested number of
                                 *bytes only (leaving the rest in Packet) and status 1.
                                 */
                                 if (availableBytes >= dataSizeinBytes)
                                 {
                                     #if defined(_ROTH_MKR1000_) || defined(_ROTH_MKRWIFI1010_)
                                        sizeReceived = client[wifitcpindex].read(data, (size_t)dataSizeinBytes);
                                     #else
                                        for(int i=0; i<dataSizeinBytes; i++){
                                           data[i] = client[wifitcpindex].read();
                                        }
                                     #endif
                                     *outStatus = 1;
                                 }

                                 /*When available number of data bytes in TCP frame <
                                  *requested number of bytes output:
                                  *data[0:dataSizeinBytes-1] = 0
                                  *and status 0.
                                  */
                                 if(availableBytes < dataSizeinBytes)
                                 {
                                     /*When blocking mode is enabled, wait for requested number of data bytes or  blockingTimeout.
                                     If requested number of data bytes are not received till blockingTimeout then data = 0, status = 0.*/
                                     if(blockingTimeout)
                                     {
                                        checkDataBlockingMode(wifitcpindex, data, dataSizeinBytes, outStatus, isServer, (unsigned long)blockingTimeout);
                                     }
                                     else
                                     {

                                       for(uint16_t count = 0; count < dataSizeinBytes; count++)
                                        {
                                            *data++ = 0;
                                        }
                                        *outStatus = 0;
                                     }
                                 }
                         }
                        else
                        {
                            /*When blocking mode is enabled, wait for requested number of data bytes or  blockingTimeout.
                            If requested number of data bytes are not received till blockingTimeout then data = 0, status = 0.*/
                             if(blockingTimeout)
                             {
                                checkDataBlockingMode(wifitcpindex, data, dataSizeinBytes, outStatus, isServer, (unsigned long)blockingTimeout);
                             }
                             else
                             {
                                /*When no data is available in TCP frame output:
                                *data[0:dataSizeinBytes-1] = 0
                                *and status 0.
                                */
                                for(uint16_t count = 0; count < dataSizeinBytes; count++)
                                {
                                    *data++ = 0;
                                }
                                *outStatus = 0;
                             }
                        } /*(availableBytes >= dataSizeinBytes)*/
                      }
                      else
                      {
                            /* When blocking mode is enabled, wait for a client connection + requested number of data bytes or
                             * blockingTimeout.If no client connects or requested number of data bytes are not received after
                             * client connection till blockingTimeout then data = 0, status = 0.
                             */
                             if(blockingTimeout)
                             {
                                checkClientConnectionBlockingMode(wifitcpindex, data, dataSizeinBytes, outStatus,(unsigned long)blockingTimeout);
                             }
                             else
                             {
                                /*When no client is connected output:
                                *data[0:dataSizeinBytes-1] = 0
                                *and status 0.
                                */
                                for(uint16_t count = 0; count < dataSizeinBytes; count++)
                                {
                                    *data++ = 0;
                                }
                                *outStatus = 0;
                             }
                      }/*(client[wifitcpindex])*/
                    #endif /*_RTT_WIFI_TCP_SERVER_*/
                }
                #endif /*_RTT_WIFI_ESP8266_*/

            }/*MW_WifiTCPFinalread*/

    #endif /* _RTT_WIFI_TCP_ !=0 */

    #ifdef _RTT_WIFI_ESP8266_
    uint8_t checkESP8266BlockingMode(uint8_t *data, uint16_t dataSizeinBytes, int32_t *outStatus, unsigned long blockingTimeout)
    {
        uint16_t packetSizeInBytes = 0;
        uint8_t dataFromPacket[40];
        unsigned long startMillis = millis();
        while(millis() - startMillis < blockingTimeout)
        {
            packetSizeInBytes = esp8266obj.tcpipReceive(dataFromPacket, dataSizeinBytes, false);

            int bufferSize = espTcpDataBuffer.getNumberOfDataBytes();

            if(bufferSize == 0)//circular buffer is empty
            {
                if(packetSizeInBytes == 0) //no data is received from ESP8266
                {
                }
                else //some data is received from ESP8266
                {
                    if(packetSizeInBytes == dataSizeinBytes)//output data,status = 1
                    {
                        memcpy(data,dataFromPacket,dataSizeinBytes);
                        *outStatus = 1;
                        return 1;
                    }
                    else if(packetSizeInBytes < dataSizeinBytes)//add data to circular buffer,status = 0
                    {
                        for(uint16_t count = 0; count < packetSizeInBytes; count++)
                        {
                            espTcpDataBuffer.push(dataFromPacket[count]);
                        }
                    }
                    else//output data,status = 1,add excess data to circular buffer
                    {
                        memcpy(data,dataFromPacket,dataSizeinBytes);
                        for(uint16_t count = dataSizeinBytes; count < packetSizeInBytes; count++)
                        {
                            espTcpDataBuffer.push(dataFromPacket[count]);
                        }
                        *outStatus = 1;
                        return 1;
                    }
                }
            }
            else//circular buffer has some data bytes
            {
                if(bufferSize >= dataSizeinBytes)//output buffer data, push received data from chip to buffer
                {
                    for(uint16_t count = 0; count < dataSizeinBytes; count++)
                    {
                        data[count] = espTcpDataBuffer.pop();
                    }
                    *outStatus = 1;
                    for(uint16_t count = 0; count < packetSizeInBytes; count++)
                    {
                        espTcpDataBuffer.push(dataFromPacket[count]);
                    }
                    return 1;
                }
                else//output buffer data, output received data from chip(if any), push excess data to buffer
                {
                    int remainingNumOfBytes = dataSizeinBytes - bufferSize;
                    if(packetSizeInBytes >= remainingNumOfBytes)
                    {
                        for(uint16_t count = 0; count < bufferSize; count++)
                        {
                            data[count] = espTcpDataBuffer.pop();
                        }
                        for(uint16_t count = 0; count < remainingNumOfBytes; count++)
                        {
                            data[bufferSize+count] = dataFromPacket[count];
                        }
                        *outStatus = 1;
                        //push excess data to buffer
                        for(uint16_t count = remainingNumOfBytes; count < packetSizeInBytes; count++)
                        {
                            espTcpDataBuffer.push(dataFromPacket[count]);
                        }
                        return 1;
                    }
                    else//not enough data available in both buffer and received data combined. Add data to buffer.
                    {
                        for(uint16_t count = 0; count < packetSizeInBytes; count++)
                        {
                            espTcpDataBuffer.push(dataFromPacket[count]);
                        }
                    }
                }
            }
        }
        return 0;
    }
    #else
    void checkDataBlockingMode(uint8_t wifitcpindex, uint8_t *data, uint16_t dataSizeinBytes, int32_t *outStatus,uint8_t isServer, unsigned long blockingTimeout)
    {
        int currentSizeReceived = 0;
        int currentAvailableBytes = 0;
        unsigned long startMillis = millis();

        /*Check if milliseconds elapsed is less than blockingTimeout*/
        while(millis() - startMillis < blockingTimeout)
        {
            /*Check the currently available number of bytes in TCP frame*/
            if (!isServer)
            {
                #if (defined(_RTT_WIFI_TCP_CLIENT_) && (_RTT_WIFI_TCP_CLIENT_!=0))
                    currentAvailableBytes = localClient[wifitcpindex].available();
                    /*When available number of data bytes in TCP frame is > =
                    *requested number of bytes output requested number of
                    *bytes only (leaving the rest in Packet) and status 1.
                    */
                    if(currentAvailableBytes >= dataSizeinBytes)
                    {
                        #if defined(_ROTH_MKR1000_) || defined(_ROTH_MKRWIFI1010_)
                            currentSizeReceived = localClient[wifitcpindex].read(data, (size_t)dataSizeinBytes);
                        #else
                            for(int i=0; i<dataSizeinBytes; i++){
                               data[i] = localClient[wifitcpindex].read();
                            }
                        #endif
                        *outStatus = 1;
                        return;
                    }
                #endif
            }
            else
            {
                #if (defined(_RTT_WIFI_TCP_SERVER_) && (_RTT_WIFI_TCP_SERVER_!=0))
                    if(!client[wifitcpindex].connected()){
                        client[wifitcpindex] = server[wifitcpindex].available();
                    }
                    /* Check for available bytes only if a client is valid */
                    if (client[wifitcpindex]){
                        currentAvailableBytes = client[wifitcpindex].available();
                        /*When available number of data bytes in TCP frame is > =
                        *requested number of bytes output requested number of
                        *bytes only (leaving the rest in Packet) and status 1.
                        */
                        if(currentAvailableBytes >= dataSizeinBytes)
                        {
                            #if defined(_ROTH_MKR1000_) || defined(_ROTH_MKRWIFI1010_)
                                currentSizeReceived =  client[wifitcpindex].read(data, (size_t)dataSizeinBytes);
                            #else
                                for(int i=0; i<dataSizeinBytes; i++){
                                   data[i] = client[wifitcpindex].read();
                                }
                            #endif
                            *outStatus = 1;
                            return;
                        }
                    }/*(client[wifitcpindex])*/
                #endif
            } /*(!isServer)*/
        }/*while*/

        /*If available data bytes in TCP frame is not same as requested
         *number of bytes till blockingTimeout then output:Data=0,Status=0*/
        for(uint16_t count = 0; count < dataSizeinBytes; count++)
        {
            *data++ = 0;
        }
        *outStatus = 0;
    } /*checkDataBlockingMode*/

    void checkClientConnectionBlockingMode(uint8_t wifitcpindex, uint8_t *data, uint16_t dataSizeinBytes, int32_t *outStatus, unsigned long blockingTimeout)
    {
        uint8_t  gotClient                  = 0;
        int currentSizeReceived             = 0;
        int currentAvailableBytes           = 0;
        unsigned long startMillis           = millis();

        /*Check if milliseconds elapsed is less than blockingTimeout*/
        while(millis() - startMillis < blockingTimeout)
        {
            #if (defined(_RTT_WIFI_TCP_SERVER_) && (_RTT_WIFI_TCP_SERVER_!=0))
                /*Check if a client is connected*/
                if(!gotClient)
                {
                    if(!client[wifitcpindex].connected()){
                        client[wifitcpindex] = server[wifitcpindex].available();
                     }
                    if(client[wifitcpindex])
                    {
                        gotClient = 1;
                    }
                }
                /*If client is connected block till requested number of data bytes are received*/
                if(gotClient)
                {
                    /*Flush client input buffer at beginning*/
                    if (oneTimeServerFlushFlag[wifitcpindex] == 0)
                    {
                        client[wifitcpindex].flush();
                        oneTimeServerFlushFlag[wifitcpindex]++;
                    }
                    currentAvailableBytes = client[wifitcpindex].available();

                    /*When available number of data bytes in TCP frame is > =
                    *requested number of bytes output requested number of
                    *bytes only (leaving the rest in Packet) and status 1.
                    */
                    if(currentAvailableBytes >= dataSizeinBytes)
                    {
                        #if defined(_ROTH_MKR1000_) || defined(_ROTH_MKRWIFI1010_)
                            currentSizeReceived =  client[wifitcpindex].read(data, (size_t)dataSizeinBytes);
                        #else
                            for(int i=0; i<dataSizeinBytes; i++){
                               data[i] = client[wifitcpindex].read();
                            }
                        #endif
                        *outStatus = 1;
                        return;
                    }
                 }/*gotClient*/
              #endif
         }/*while*/

        /* If no client connected till blockingTimeout or client got connected but available
         * data bytes in TCP frame are not same as requested number of bytes till
         * blockingTimeout output data[0:dataSizeinBytes-1] = 0 and status 0.
         */
        for(uint16_t count = 0; count < dataSizeinBytes; count++)
        {
            *data++ = 0;
        }
        *outStatus = 0;
    }/*checkClientConnectionBlockingMode*/
    #endif //_RTT_WIFI_ESP8266_
#endif /*Rapid Accel condition check*/
