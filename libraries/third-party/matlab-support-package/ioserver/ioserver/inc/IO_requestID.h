/**
 * @file IO_requestID.h
 *
 * Contains request ID for standard peripheral and core server
 *
 * @Copyright 2017-2019 The MathWorks, Inc.
 *
 */
#ifndef IO_REQUESTID_H_
#define IO_REQUESTID_H_


/* data contained in the packet*/
typedef enum requestIdValuesTag
{
    /*================================
     * IO core/standard Request ID
     *==============================*/

    /* Digital IO actions */
    CONFIGUREDIGITALPIN             = 0x00,
    WRITEDIGITALPIN                 = 0x01,
    READDIGITALPIN                  = 0x02,
    UNCONFIGUREDIGITALPIN           = 0x03,

    /* I2C actions */
    ENABLEI2C                       = 0x04,
    DISABLEI2C                      = 0x05,
    SCANI2CBUS                      = 0x06,
    OPENI2CBUS                      = 0x07,
    RAWI2CWRITE                     = 0x08,
    RAWI2CREAD                      = 0x09,
    REGISTERI2CREAD                 = 0x0A,
    REGISTERI2CWRITE                = 0x0B,
    CLOSEI2CBUS                     = 0x0C,
    SETI2CBUSSPEED                  = 0x0D,

    /* Analog Input Action */
    CONFIGUREANALOGINSINGLE         = 0x0E,
    CONFIGUREANALOGINGROUP          = 0x0F,
    SETTRIGGERSOURCEANALOGIN        = 0x10,
    ENABLENOTIFICATIONANALOGIN      = 0x11,
    DISABLENOTIFICATIONANALOGIN     = 0x12,
    ENABLECONTCONVERSIONANALOGIN    = 0x13,
    SETCHANNELCONVRANKANALOGIN      = 0x14,
    GETSTATUSANALOGIN               = 0x15,
    READRESULTANALOGINSINGLE        = 0x16,
    READRESULTANALOGINGROUP         = 0x17,
    STARTANALOGINCONVERSION         = 0x18,
    STOPANALOGINCONVERSION          = 0x19,
    UNCONFIGUREANALOGINSINGLE       = 0x1A,

    /* PWM Input Action */
    CONFIGUREPWMPIN                 = 0x1B,
    STARTPWM                        = 0x1C,
    SETDUTYCYCLE                    = 0x1D,
    SETFREQUENCY                    = 0x1E,
    DISABLEPWMNOTIFICATION          = 0x1F,
    ENABLEPWMNOTIFICATION           = 0x20,
    SETPWMOUTPUTTOIDLE              = 0x21,
    GETPWMOUTPUTSTATUS              = 0x22,
    STOPPWM                         = 0x23,
    UNCONFIGUREPWMPIN               = 0x24,

    /* SPI Actions */
    ENABLESPI                       = 0x25,
    DISABLESPI                      = 0x26,
    OPENSPI                         = 0x27,
    SETFORMATSPI                    = 0x28,
    SETBUSSPEEDSPI                  = 0x29,
    WRITEREADSPI                    = 0x2A,
    CLOSESPI                        = 0x2B,

    /* Generic */
    RESETALLDIGITALPINS             = 0x2C,
    RESETALLANALOGPINS              = 0x2D,
    RESETALLPWMPINS                 = 0x2E,

    /*Add-on*/
    ADDON                           = 0x2F,

    /* SCI */
    OPENSCIBUS                      = 0x30,
    CONFIGURESCIHARDWAREFLOWCONTROL = 0x31,
    SETSCIBAUDRATE                  = 0x32,
    SETSCIFRAMEFORMAT               = 0x33,
    SCIRECEIVE                      = 0x34,
    SCITRASMIT                      = 0x35,
    GETSCISTATUS                    = 0x36,
    SCISENDBREAK                    = 0x37,
    SCICLOSE                        = 0x38,
    SETSCITIMEOUT                   = 0x39,

    /* connection actions  */
    IDNOTDEFINED                    = 0xFF00,
    CONNECT                         = 0xFF01,

    /* Streaming mode actions */
    STARTSTREAMINGMODE              = 0xFF02,
    STARTCONFIGURESTREAMMODE        = 0xFF03,
    STOPCONFIGURESTREAMMODE         = 0xFF04,
    EXITSTREAMINGMODE               = 0xFF05,
    SETSTREAMINGMODEBASERATE        = 0xFF06,
    SETSTREAMINGMODERATES           = 0xFF07,
    SETSTREAMINGDURATION            = 0xFF08,

    /* Core IO server versioning action */
    GETCOREIOSERVERVERSION          = 0xFF09,

    /* Target IO server versioning action */
    GETBOARDIOSERVERVERSION         = 0xFF0A,

    /* Extracting target information actions */
    GETBOARDINFORMATION             = 0xFF0B,
    GETLIBRARYINFORMATION           = 0xFF0C,
    GETCUSTOMDATAINFORMATION        = 0xFF0D,

    /* Request ID for debug info */
    DEBUGINFORMATION                = 0xFF0E,

    /* Error handling */
    TRANSMISSIONOVERRUN             = 0xFF0F,
    MALLOCFAILEDSTREAMINGCONFIG     = 0xFF10,
    SCHEDULEROVERRUN                = 0xFF11,
    PERIPHERALRESPONSESIZEMISMATCH  = 0xFF12,

    /* Honour samples per frame*/
    SETSAMPLESPERFRAME              = 0xFF13,
    ENABLETIMESTAMP                 = 0xFF14,
    GETHEAPSIZE                     = 0xFF15,
    GETBUFFERCREATIONOVERHEAD       = 0xFF16,

    /* ESB - Specific */
    ESBRECORDINGCOMPLETED           = 0xFF17,

    /* Get maximum packet size */
    GETMAXIMUMPACKETSIZE            = 0xFF18,
#if IO_STACK_HEAP_UTILIZATION
    /* Test heap and stack collision */
    GETHEAPPOINTER                  = 0xFF19,
    GETSTACKPOINTER                 = 0xFF1A,
#endif
    GETRUNTIMEATA                   = 0xFF1B,
    INTERNALDEBUGINFORMATION        = 0xFF1C,
    LASTSHOULDNOTEXCEEDTHISSIZE     = 2,



} requestIdValues;
#endif /* IO_REQUESTID_H_ */
