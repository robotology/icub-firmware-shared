
/*
 * Copyright (C) 2016 iCub Facility - Istituto Italiano di Tecnologia
 * Author:  Marco Accame
 * email:   marco.accame@iit.it
 * website: www.robotcub.org
 * Permission is granted to copy, distribute, and/or modify this program
 * under the terms of the GNU General Public License, version 2 or any
 * later version published by the Free Software Foundation.
 *
 * A copy of the license can be found at
 * http://www.robotcub.org/icub/license/gpl.txt
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details
*/

// - include guard ----------------------------------------------------------------------------------------------------

#ifndef _EOUPDATERPROTOCOL_H_
#define _EOUPDATERPROTOCOL_H_


#ifdef __cplusplus
extern "C" {
#endif

/** @file       EoUpdaterProtocol.h
    @brief      This header file contains the types used for data exchange between host and the ETH board for FW update and maintenance
    @author     marco.accame@iit.it
    @date       09 jun 2016
**/

/** @defgroup eo_cevcwervcrev666 Configuation of the vrfverver
    Tcecece 
    
    @{        
 **/



// - external dependencies --------------------------------------------------------------------------------------------

#include "EoCommon.h"
//#include "EoBoards.h"


// - public #define  --------------------------------------------------------------------------------------------------


#define EOUPROT_PROTOCOL_VERSION        1

#define EOUPROT_VALUE_OF_UNUSED_BYTE    255


// - declaration of public user-defined types ------------------------------------------------------------------------- 


typedef enum               
{
    eLoader                 = 0,    /**< the eLoader: the one executed just after reset which loads any other eProcess */
    eUpdater                = 1,    /**< the eUpdater: the one responsible for performing FW update */
    eApplication            = 2,    /**< the eApplication: the standard application */
    eApplPROGupdater        = 3,    /**< the application used to perform programming of a new eUpdater */
    uprot_proc_None         = 255,
    
    uprot_proc_Loader       = eLoader,
    uprot_proc_Updater      = eUpdater,
    uprot_proc_Application  = eApplication,
    uprot_proc_ApplPROGupdater  = eApplPROGupdater
} eOuprot_process_t;

enum { uprot_proc_numberofthem = 4 };


typedef enum 
{
    uprot_OPC_NONE              = 0,
    
    uprot_OPC_LEGACY_SCAN       = 0xFF,     /**< this command was used to discover existing ETH boards. old versions of eUpdater and of eApplication reply only to it */ 
    uprot_OPC_LEGACY_PROCS      = 0x09,     /**< this command was used to query about extra info on detected ETH boards. old versions of eUpdater and of eApplication reply only to it */ 
    uprot_OPC_LEGACY_CANGATEWAY = 0x20,
    uprot_OPC_LEGACY_IP_ADDR_SET= 0x07,
    uprot_OPC_LEGACY_EEPROM_ERASE = 0x12, // former SYSEEPROMERASE
    
    
    uprot_OPC_DISCOVER          = 0x7F,         
    uprot_OPC_MOREINFO          = 0x79,
    
    uprot_OPC_PROG_START        = 0x01,
    uprot_OPC_PROG_DATA         = 0x02,    
    uprot_OPC_PROG_END          = 0x04,


    uprot_OPC_DEF2RUN           = 0x05,
    uprot_OPC_RESTART           = 0x06, 
    uprot_OPC_IP_ADDR_SET       = 0x77, 
    uprot_OPC_EEPROM_ERASE      = 0x72, 
    uprot_OPC_EEPROM_READ       = 0x13,
    
    uprot_OPC_BLINK             = 0x0B,
    uprot_OPC_JUMP2UPDATER      = 0x0C,
        
    uprot_OPC_PAGE_CLR          = 0x30,
    uprot_OPC_PAGE_SET          = 0x31,
    uprot_OPC_PAGE_GET          = 0x32,
    uprot_OPC_JUMP2ADDRESS      = 0x33,
    
    uprot_OPC_CANGATEWAY        = 0x2f,
    
    // kept but ... maybe useless
    uprot_OPC_LEGACY_MAC_SET    = 0x11,
    uprot_OPC_LEGACY_IP_MASK_SET= 0x08  // former MASKSET 

} eOuprot_opcodes_t;


typedef enum 
{
    uprot_RES_OK                = 0,
    uprot_RES_ERR_PROT          = 1,
    uprot_RES_ERR_FLASH         = 2,
    uprot_RES_ERR_LOST          = 3,
    uprot_RES_ERR_UNK           = 4,
    uprot_RES_ERR_UNSUPPORTED   = 5,
    uprot_RES_ERR_FAILED        = 6,
    uprot_RES_ERR_TRYAGAIN      = 7       
} eOuprot_result_t;


typedef enum
{
    uprot_partitionLOADER       = 0x55,
    uprot_partitionUPDATER      = 0xAA,
    uprot_partitionAPPLICATION  = 0x5A
} eOuprot_partition2prog_t;


typedef enum
{
    uprot_page8         = 8,
    uprot_page32        = 32,
    uprot_page64        = 64,
    uprot_page128       = 128
} eOuprot_pagesize_t;

enum { uprot_pagemaxsize = 128, uprot_UDPmaxsize = 1200, uprot_PROGmaxsize = 512, uprot_EEPROMmaxsize = 1024, uprot_TEXTmaxsize = 1024 }; 


typedef enum
{   // to be used as bitfields
    uprot_canDO_nothing             = 0x0,          
    uprot_canDO_reply2discover      = 0x1 << 29,
    uprot_canDO_reply2moreinfo      = 0x1 << 30,
    uprot_canDO_PROG_loader         = 0x1 << 1, 
    uprot_canDO_PROG_updater        = 0x1 << 2, 
    uprot_canDO_PROG_application    = 0x1 << 3, 
    uprot_canDO_restart             = 0x1 << 4, 
    uprot_canDO_cangateway          = 0x1 << 5, 
    uprot_canDO_IPaddr_set          = 0x1 << 6, 
    uprot_canDO_EEPROM_erase        = 0x1 << 7, 
    uprot_canDO_EEPROM_read         = 0x1 << 8, 
    uprot_canDO_DEF2RUN_set         = 0x1 << 9, 
    uprot_canDO_JUMP2UPDATER        = 0x1 << 10,
    uprot_canDO_PAGE_get            = 0x1 << 11,
    uprot_canDO_PAGE_set            = 0x1 << 12,
    uprot_canDO_PAGE_clr            = 0x1 << 13,
    uprot_canDO_LEGACY_IPmask_set   = 0x1 << 14,
    uprot_canDO_LEGACY_MAC_set      = 0x1 << 15,
    uprot_canDO_LEGACY_cangateway   = 0x1 << 16,
    uprot_canDO_LEGACY_scan         = 0x1 << 17,
    uprot_canDO_LEGACY_IPaddr_set   = 0x1 << 18,
    uprot_canDO_LEGACY_EEPROM_erase = 0x1 << 19,
    uprot_canDO_LEGACY_procs        = 0x1 << 20,
    uprot_canDO_blink               = 0x1 << 21,
    uprot_canDO_JUMP2ADDRESS        = 0x1 << 22
} eOuprot_proc_capabilities_t;

typedef struct
{
    uint8_t             type;               // use eOuprot_process_t
    uint8_t             filler[1];          // if not used it must be EOUPROT_VALUE_OF_UNUSED_BYTE
    eOversion_t         version;            // the version of the firmware
    eOdate_t            date;               // the date associated to the firmware as it is in repository
    eOdate_t            compilationdate;    // the compilation date, as set by the compiler with a macro.
    uint16_t            rom_addr_kb;        // the rom address in kb from where the process starts
    uint16_t            rom_size_kb;        // the max size in kb of the rom of the process.
} eOuprot_procinfo_t;  EO_VERIFYsizeof(eOuprot_procinfo_t, 16)


typedef struct 
{     
    uint8_t             numberofthem;       // number of processes. it typically is 3, but it can be 2 if the eApplication is not available   
    uint8_t             startup;            // it is the process launched by the eLoader at bootstrap. it typically is the eUpdater but it may be the eApplication
    uint8_t             def2run;            // it is the process launched after the 5 second safe time is terminated
    uint8_t             runningnow;         // it is the process which is running at the time of processing teh reply. it may be eUpdater or eApplication or eApplPROGupdater
    eOuprot_procinfo_t  info[3];
} eOuprot_proctable_t;  EO_VERIFYsizeof(eOuprot_proctable_t, 52)




// - format of commands and of their replies (if any)
//   there is the rule: one udp packet one command. there is no aggregation of commands inside a udp packet.

// - legacy commands
//   Legacy commands were used by old ethLoader to query / command actions on ETH boards with old versions of eUpdader and eApplication.
//   Their functionalities are superseded by the new commands, but the legacy commands must be supported by the new ethLoader because 
//   many ETH boards with old FW versions are still around the world. 


// its new version is eOuprot_cmd_DISCOVER_t (uprot_OPC_DISCOVER)
// the legacy boards don't check the length of the command. they check only the first byte of the packet.
typedef struct
{
    uint8_t             opc;    // it must be 0xFF (uprot_OPC_LEGACY_SCAN) 
} eOuprot_cmd_LEGACY_SCAN_t;   EO_VERIFYsizeof(eOuprot_cmd_LEGACY_SCAN_t, 1)


typedef struct
{
    uint8_t             opc;            // it must be 0xFF (uprot_OPC_LEGACY_SCAN)
    eOversion_t         version;        // of the process which sends this command. it may be either eUpdater or eApplication. 
    uint8_t             unused;         // old ETH boards send up the value 0x0A
    uint8_t             ipmask[4];      // the value is fixed at 255.255.255.0. as such, it does not have any meaning.
    uint8_t             mac48[6];       // it is ordered so that positions 0, 1, 2 hold the OUI (Organizationally Unique Identifier), whereas positions 3, 4, 5 hold a value unique for the ETH boards.          
} eOuprot_cmd_LEGACY_SCAN_REPLY_t; EO_VERIFYsizeof(eOuprot_cmd_LEGACY_SCAN_REPLY_t, 14)


// its new version is eOuprot_cmd_GETFULLINFO_t (uprot_OPC_GETFULLINFO)
// the legacy boards don't check the length of the command. they check only the first byte of the packet.
typedef struct
{
    uint8_t             opc;    // it must be 0x09 (uprot_OPC_LEGACY_PROCS) 
} eOuprot_cmd_LEGACY_PROCS_t;   EO_VERIFYsizeof(eOuprot_cmd_LEGACY_PROCS_t, 1)


typedef struct
{
    uint8_t             opc;            // it must be 0x09 (uprot_OPC_LEGACY_PROCS) 
    uint8_t             numofprocesses; // tells how many they are 
    uint8_t             description[uprot_TEXTmaxsize]; // contains a string which is prepared by the eLoader.          
} eOuprot_cmd_LEGACY_PROCS_REPLY_t; EO_VERIFYsizeof(eOuprot_cmd_LEGACY_PROCS_REPLY_t, 1026)


// its new version is eOuprot_cmd_CANGATEWAY_t (uprot_OPC_CANGATEWAY)
// the legacy boards don't check the length of the command. they check only the first byte of the packet.
typedef struct
{
    uint8_t             opc;    // it must be 0x20 (uprot_OPC_LEGACY_CANGATEWAY) 
} eOuprot_cmd_LEGACY_CANGATEWAY_t;   EO_VERIFYsizeof(eOuprot_cmd_LEGACY_CANGATEWAY_t, 1)


// former IPSET
// used to change the IP address of the board.
// this command sends back: NOTHING.
typedef struct
{
    uint8_t             opc;            // it must be 0x07 (uprot_OPC_LEGACY_IP_ADDR_SET)
    uint8_t             address[4];     // from 0 to 3: 10.0.1.99
} eOuprot_cmd_LEGACY_IP_ADDR_SET_t;  EO_VERIFYsizeof(eOuprot_cmd_LEGACY_IP_ADDR_SET_t, 5)


typedef struct
{
    uint8_t             opc;            // it must be 0x12 (uprot_OPC_LEGACY_EEPROM_ERASE)    
} eOuprot_cmd_LEGACY_EEPROM_ERASE_t;  EO_VERIFYsizeof(eOuprot_cmd_LEGACY_EEPROM_ERASE_t, 1)


// - new commands (or old revisited).


// typical reply. it is 4 bytes ... but it can hold more if sizeofextra is not zero.
typedef struct 
{   // keep opc | res as first for backwards compatibility with old protocol
    uint8_t             opc;            // use eOuprot_opcodes_t
    uint8_t             res;            // use eOuprot_result_t
    uint8_t             protversion;    // use EOUPROT_PROTOCOL_VERSION
    uint8_t             sizeofextra;    // put in here the size of the extra data in the reply
} eOuprot_cmdREPLY_t;   EO_VERIFYsizeof(eOuprot_cmdREPLY_t, 4)


// if used in broadcast mode, it finds out which ETH boards are available. if used in unicast mode it refreshes the info of an ETH board.
// the command is backwards compatible with legacy eUpdater and eApplication. They check just the first byte and will reply with a  
// eOuprot_cmd_LEGACY_SCAN_REPLY_t which contains less information but still manages the discovery task. 
// at reception the board sends back: a eOuprot_cmd_DISCOVERY_REPLY_t (if a legacy board: a eOuprot_cmd_LEGACY_SCAN_REPLY_t).
typedef struct
{
    uint8_t             opc;        // it must be 0xFF (uprot_OPC_LEGACY_SCAN).
    uint8_t             opc2;       // it must be uprot_OPC_DISCOVER
    uint8_t             filler[2];  // it must be {EOUPROT_VALUE_OF_UNUSED_BYTE}
} eOuprot_cmd_DISCOVER_t; EO_VERIFYsizeof(eOuprot_cmd_DISCOVER_t, 4)

typedef struct
{
    eOuprot_cmdREPLY_t  reply;          // opc = uprot_OPC_DISCOVER, res = uprot_RES_OK, protversion = EOUPROT_PROTOCOL_VERSION, sizeofextra = sizeof(eOuprot_cmd_DISCOVER_REPLy_t)-sizeof(eOuprot_cmdREPLY_t)
    uint8_t             boardtype;      // use a eObrd_type_t value ... which of course it must be of eth type
    uint8_t             unused[1];      // it must be {EOUPROT_VALUE_OF_UNUSED_BYTE}
    uint8_t             mac48[6];       // it is ordered so that positions 0, 1, 2 hold the OUI (Organizationally Unique Identifier), whereas positions 3, 4, 5 hold a value unique for the ETH boards.
    uint32_t            capabilities;   // it contains a mask of eOuprot_proc_capabilities_t which tells what the remote board can do.
    eOuprot_proctable_t processes;
    uint8_t             boardinfo32[32];// where: boardinfo32[0] = strlen(&boardinfo32[1]) but if boardinfo32[0] is 255 then it means that it is not used
} eOuprot_cmd_DISCOVER_REPLY_t;  EO_VERIFYsizeof(eOuprot_cmd_DISCOVER_REPLY_t, 100)


// used to achieve more information about a specific board. this command is now superseded by what teh command eOuprot_cmd_DISCOVERY_t retrieves.
// we nevertheless use it because of presence of legacy boards. this command is recognised by legacy boards as a eOuprot_cmd_LEGACY_PROCS_t.
// at reception the board sends back: a eOuprot_cmd_MOREINFO_REPLY_t (if a legacy board: a eOuprot_cmd_LEGACY_PROCS_REPLY_t).
typedef struct
{
    uint8_t             opc;            // it must be 0x09 (uprot_OPC_LEGACY_PROCS).
    uint8_t             opc2;           // it must be uprot_OPC_MOREINFO
    uint8_t             plusdescription;// if 1 the receiver must send also a description in string format
    uint8_t             filler[1];      // it must be {EOUPROT_VALUE_OF_UNUSED_BYTE}
} eOuprot_cmd_MOREINFO_t; EO_VERIFYsizeof(eOuprot_cmd_MOREINFO_t, 4)


typedef struct
{   // it is equal to eOuprot_cmd_DISCOVERY_REPLY_t and then there are some more bytes.
    eOuprot_cmd_DISCOVER_REPLY_t    discover;           // only change to eOuprot_cmd_DISCOVER_REPLY_t is that discover.reply.opc = uprot_OPC_MOREINFO.
    uint8_t                         hasdescription;     // tells if there is a string which follows. the string can have at most 384 chars
    uint8_t                         description[3];     // from here we have a string.
} eOuprot_cmd_MOREINFO_REPLY_t;     EO_VERIFYsizeof(eOuprot_cmd_MOREINFO_REPLY_t, 104)


// used to start programming onto flash the code of a process.
// the format of the command has changed from the legacy one because the legacy one was of length two. however, this command of length 4 can be
// correctly decoded by legacy boards which do not check for message length. only it is 4 bytes long instead of 2. 
// at reception the board sends back: a eOuprot_cmdREPLY_t with fields {.opc = uprot_OPC_PROG_START, .res = uprot_RES_OK, .protversion = EOUPROT_PROTOCOL_VERSION, .sizeofextra = 0}
typedef struct
{
    uint8_t             opc;            // it must be 0x01 (uprot_OPC_PROG_START) 
    uint8_t             partition;      // use eOuprot_partition2prog_t
    uint8_t             filler[2];      // it must be {EOUPROT_VALUE_OF_UNUSED_BYTE}      
} eOuprot_cmd_PROG_START_t; EO_VERIFYsizeof(eOuprot_cmd_PROG_START_t, 4)
//#warning potremmo mettere anche uno start, size in modo da spere prima dove scrivo. se sono entrambi zero non li valuto.

   
// used to carry data to be programmed onto flash. it is a variable sized command.  
// the format of the command has NOT changed from the legacy one.
// at reception the board sends back: a eOuprot_cmdREPLY_t with fields {.opc = uprot_OPC_PROG_DATA, .res = uprot_RES_OK, .protversion = EOUPROT_PROTOCOL_VERSION, .sizeofextra = 0}
typedef struct
{
    uint8_t             opc;            // it must be 0x02 (uprot_OPC_PROG_DATA) 
    uint8_t             address[4];     // contains the flash address where to write the data. we use little endian convention (i.e., address[0] is the lsb).
    uint8_t             size[2];        // contains the size of the data to be written. we use little endian convention (i.e., size[0] is the lsb). 
    uint8_t             data[uprot_PROGmaxsize]; // data is of variable size and up to uprot_PROGmaxsize (=512) bytes
    uint8_t             filler[1];
} eOuprot_cmd_PROG_DATA_t; EO_VERIFYsizeof(eOuprot_cmd_PROG_DATA_t, uprot_PROGmaxsize+8)


// used to end programming onto flash the code of a process. 
// the format of the command has NOT changed from the legacy one. only it is 4 bytes long instead of 3. 
// at reception the board sends back: a eOuprot_cmdREPLY_t with fields {.opc = uprot_OPC_PROG_END, .res = uprot_RES_OK, .protversion = EOUPROT_PROTOCOL_VERSION, .sizeofextra = 0}
typedef struct
{
    uint8_t             opc;            // it must be 0x04 (uprot_OPC_PROG_END) 
    uint8_t             numberofpkts[2];// number of packets of type eOuprot_cmd_PROG_DATA_t + 1 that the programmer has sent. we use little endian convention.
    uint8_t             filler[1];      // it must be {EOUPROT_VALUE_OF_UNUSED_BYTE}
} eOuprot_cmd_PROG_END_t;    EO_VERIFYsizeof(eOuprot_cmd_PROG_END_t, 4)


// former BOOT
// used to change the def2run process (the one executing after the 5 seconds). this command also forces the startup process to be the eUpdater.
// the format of the command has NOT changed from the legacy one. only it is 4 bytes long instead of 2. 
// this command sends back: NOTHING.
typedef struct
{
    uint8_t             opc;        // it must be 0x05 (uprot_OPC_DEF2RUN) 
    uint8_t             proc;       // use eOuprot_process_t. it can be only: eUpdater or eApplication
    uint8_t             filler[2];  // it must be {EOUPROT_VALUE_OF_UNUSED_BYTE}
} eOuprot_cmd_DEF2RUN_t;  EO_VERIFYsizeof(eOuprot_cmd_DEF2RUN_t, 4)


// former RESET
// used to restart the ETH board as if power supply is removed and applied again.
// the format of the command has NOT changed from the legacy one. only it is 4 bytes long instead of 1. 
// this command sends back: NOTHING.
typedef struct
{
    uint8_t             opc;        // it must be 0x06 (uprot_OPC_RESTART)
    uint8_t             filler[3];  // it must be {EOUPROT_VALUE_OF_UNUSED_BYTE}
} eOuprot_cmd_RESTART_t;  EO_VERIFYsizeof(eOuprot_cmd_RESTART_t, 4)

// former IPSET
// used to change the IP address of the board.
// the format of the command has changed from the legacy one. in here we have a flag which tells if the ETH board must restart so that 
// the change of address can be effective
// this command sends back: NOTHING.
typedef struct
{
    uint8_t             opc;            // it must be 0x07 uprot_OPC_LEGACY_IP_ADDR_SET. 
    uint8_t             address[4];     // from 0 to 3: 10.0.1.99
    uint8_t             opc2;           // it must be 0x77 (uprot_OPC_IP_ADDR_SET).
    uint8_t             sysrestart;     // if 1 the system also restarts so that the IP change is effective immediately.
    uint8_t             filler[1];      // it must be {EOUPROT_VALUE_OF_UNUSED_BYTE}
} eOuprot_cmd_IP_ADDR_SET_t;  EO_VERIFYsizeof(eOuprot_cmd_IP_ADDR_SET_t, 8)


//// former MASKSET
//// used to change the IP mask of the board.
//// the format of the command has NOT changed from the legacy one. only it is 8 bytes long instead of 5. 
//// we keep this message just in case, however we don't plan to ever change the IP mask. 
//// this command sends back: NOTHING.
//typedef struct
//{
//    uint8_t             opc;            // it must be 0x08 (uprot_OPC_IP_MASK_SET) 
//    uint8_t             mask[4];        // from 0 to 3: 255.255.255.0
//    uint8_t             filler[3];      // it must be {EOUPROT_VALUE_OF_UNUSED_BYTE}
//} eOuprot_cmd_IP_MASK_SET_t;  EO_VERIFYsizeof(eOuprot_cmd_IP_MASK_SET_t, 8);


// used to force blinking of LEDs on the ETH board so that one can recognise it amongst others.
// the format of the command has NOT changed from the legacy one. only it is 4 bytes long instead of 1. 
// this command sends back: NOTHING.
typedef struct
{
    uint8_t             opc;            // it must be 0x0B (uprot_OPC_BLINK) 
    uint8_t             filler[3];      // it must be {EOUPROT_VALUE_OF_UNUSED_BYTE}
} eOuprot_cmd_BLINK_t;  EO_VERIFYsizeof(eOuprot_cmd_BLINK_t, 4)


// former CMD_UPD_ONCE
// it is used to force an eApplication to jump into the eUpdater and keep its execution even after the 5 seconds. 
// this command is used mainly when the eApplication is a eMaintainer, which has capability to program the FLASH
// of the eUpdater. We send a uprot_OPC_JUMP2UPDATER when we want to verify whether the newly programmed eUpdater
// executes fine before we send a uprot_OPC_DEF2RUN<eUpdater> which could make the ETH board unreachable if the eUpdater fails.
// the format of the command has NOT changed from the legacy one. only it is 4 bytes long instead of 1. 
// this command sends back: NOTHING.
typedef struct
{
    uint8_t             opc;            // it must be 0x0C (uprot_OPC_JUMP2UPDATER)
    uint8_t             filler[3];      // it must be {EOUPROT_VALUE_OF_UNUSED_BYTE}
} eOuprot_cmd_JUMP2UPDATER_t;  EO_VERIFYsizeof(eOuprot_cmd_JUMP2UPDATER_t, 4)



// former SYSEEPROMERASE
// in legacy mode is used to reset the EEPROM of the board between 6k and 8k. 
// In there the EEPROM contains: 
// @6k for 1k bytes:        partition table (uses type partTableStorage_t of 808 bytes, privately declared in shalPart.c)
// @7k for 80 bytes:        boardinfo as set by the eLoader (uses type infoBoardInfoStorage_t of 80 bytes, privately declared in shalInfo.c).
// @7k+80 for 272 bytes:    device info containing: mac, ip address, ip mask, four pages of 8, 32, 64, and 128 bytes (uses type infoDeviceInfoStorage_t of 272 bytes, privately declared in shalInfo.c) 
// 
// the erase operation will force any process which enters in its main() (eLoader, eUpdater or eApplication) to restore the EEPROM to
// its default value which is: IP = 10.0.1.99, startup = def2run = eUpdater, pages all empty (all 0xff).
// thus: pay attention to send the command in broadcast or many boards because they will become all mixed up and indistinguishable (apart the MAC address).
//
// the format of the command has changed from the legacy one. 
// in legacy one: it erase EEPROM only from address 6k to 8k-1 (2k in total) and then keeps on executing without resetting to default values.
// in the new format: it may erase either the 6k-8k portion or a specified range. it may restart after the eeprom erase oepration   
// at reception the board sends back: a eOuprot_cmdREPLY_t with fields {.opc = uprot_OPC_EEPROM_ERASE, .res = uprot_RES_OK, .protversion = EOUPROT_PROTOCOL_VERSION, .sizeofextra = 0}
typedef struct
{
    uint8_t             opc;            // it must be 0x12 (uprot_OPC_LEGACY_EEPROM_ERASE).   
    uint8_t             opc2;           // it must be 0x72 (uprot_OPC_EEPROM_ERASE)  
    uint8_t             sysrestart;     // if 1, it restarts after. in this case it sends back nothing.
    uint8_t             filler[1];      // it must be {EOUPROT_VALUE_OF_UNUSED_BYTE} 
    uint16_t            address;        // 
    uint16_t            size;           // if size is not zero, then we just delete [address, address+size). if zero, we delete the sys-eeprom 
} eOuprot_cmd_EEPROM_ERASE_t;  EO_VERIFYsizeof(eOuprot_cmd_EEPROM_ERASE_t, 8)



// used to read the EEPROM of the board. The EEPROM contains many things: .... 
// 0    -> +6k:         managed by the eApplication
// 6k   -> +1k:         partition table, defined by type partTableStorage_t (privately declared in shalPart.c)
// 7k   -> +678:        info, contains boardinfo e deviceinfo (infoBoardInfoStorage_t: 80 bytes @ 7k, and infoDeviceInfoStorage_t: 272 bytes @ 7k+80 )
// at reception the board sends back: a eOuprot_cmd_EEPROM_READ_REPLY_t
typedef struct
{
    uint8_t             opc;            // it must be 0x13 (uprot_OPC_EEPROM_READ)
    uint8_t             filler[1];      // it must be {EOUPROT_VALUE_OF_UNUSED_BYTE}
    uint16_t            address;        // the starting address;
    uint16_t            size;           // the number of bytes to read (at most uprot_EEPROMmaxsize = 1024)
    uint8_t             filler1[2];     // it must be {EOUPROT_VALUE_OF_UNUSED_BYTE}
} eOuprot_cmd_EEPROM_READ_t;  EO_VERIFYsizeof(eOuprot_cmd_EEPROM_READ_t, 8)


typedef struct
{  
    eOuprot_cmdREPLY_t  reply;          // opc = uprot_OPC_EEPROM_READ, res = uprot_RES_OK, protversion = EOUPROT_PROTOCOL_VERSION, sizeofextra = 4 + size;
    uint16_t            address;
    uint16_t            size;
    uint8_t             eeprom[uprot_EEPROMmaxsize];
} eOuprot_cmd_EEPROM_READ_REPLY_t;  EO_VERIFYsizeof(eOuprot_cmd_EEPROM_READ_REPLY_t, 1032)




// it is used to clear to 0xff the value of the page stored in EEPROM which is in here specified.
// there are four of such pages, one for each different size of: 8, 32, 64, 128 bytes.
// this command sends back: NOTHING.
typedef struct
{
    uint8_t             opc;            // it must be 0x30 (uprot_OPC_PAGE_CLR)
    uint8_t             filler[1];      // it must be {EOUPROT_VALUE_OF_UNUSED_BYTE}
    uint16_t            pagesize;       // it can be 8, 32, 64 or 128
} eOuprot_cmd_PAGE_CLR_t;  EO_VERIFYsizeof(eOuprot_cmd_PAGE_CLR_t, 4)


// it is used to set the value of the page stored in EEPROM which is in here specified.
// there are four of such pages, one for each different size of: 8, 32, 64, 128 bytes.
// this command sends back: NOTHING.
typedef struct
{
    uint8_t             opc;            // it must be 0x31 (uprot_OPC_PAGE_SET)
    uint8_t             filler[1];      // it must be {EOUPROT_VALUE_OF_UNUSED_BYTE}
    uint16_t            pagesize;       // it can be one of eOuprot_pagesize_t values: 8, 32, 64 or 128 
    uint8_t             page[uprot_pagemaxsize];      // it contains the value of the page
} eOuprot_cmd_PAGE_SET_t;  EO_VERIFYsizeof(eOuprot_cmd_PAGE_SET_t, 132)


// it is used to get the value of the page stored in EEPROM which is in here specified.
// there are four of such pages, one for each different size of: 8, 32, 64, 128 bytes.
// at reception the board sends back: a eOuprot_cmd_PAGE_GET_REPLY_t
typedef struct
{
    uint8_t             opc;            // it must be 0x31 (uprot_OPC_PAGE_GET)
    uint8_t             filler[1];      // it must be {EOUPROT_VALUE_OF_UNUSED_BYTE}
    uint16_t            pagesize;       // it can be one of eOuprot_pagesize_t values: 8, 32, 64 or 128 
} eOuprot_cmd_PAGE_GET_t;  EO_VERIFYsizeof(eOuprot_cmd_PAGE_GET_t, 4)


typedef struct
{  
    eOuprot_cmdREPLY_t  reply;          // opc = uprot_OPC_PAGE_GET, res = uprot_RES_OK, protversion = EOUPROT_PROTOCOL_VERSION, sizeofextra = 4 + 2 + pagesize;
    uint16_t            pagesize;       // it can be one of eOuprot_pagesize_t values: 8, 32, 64 or 128 
    uint8_t             page[uprot_pagemaxsize];      // it contains the value of the page
} eOuprot_cmd_PAGE_GET_REPLY_t;  EO_VERIFYsizeof(eOuprot_cmd_PAGE_GET_REPLY_t, 134)


// it is used to attempt a jump to a given address
typedef struct
{
    uint8_t             opc;            // it must be 0x0C (uprot_OPC_JUMP2ADDRESS)
    uint8_t             filler[3];      // it must be {EOUPROT_VALUE_OF_UNUSED_BYTE}
    uint32_t            address;
} eOuprot_cmd_JUMP2ADDRESS_t;  EO_VERIFYsizeof(eOuprot_cmd_JUMP2ADDRESS_t, 8)


// it is used to start the CAN gateway mode in the manner specified by the parameters.
// if the ETH board is already in CAN gateway mode, only rxcanbufferclear and ackrequired are evaluated.
// this command sends back: NOTHING is ackrequired is 0, otherwise a proper eOuprot_cmdREPLY_t 
typedef struct
{
    uint8_t             opc;                // it must be 0x20 (uprot_OPC_LEGACY_CANGATEWAY) to keep backward compatibility or 0x2f (uprot_OPC_CANGATEWAY)
    uint8_t             sendcanbroadcast;   // it is 0 or 1. if 1 the ETH board sends a message ICUBCANPROTO_BL_BROADCAST to CAN boards after time4canstable ms 
    uint16_t            time4canstable;     // is the time in ms that the ETH board waits after CAN power on for its stabilisation
    uint16_t            time2waitcanreply;  // it is the extra time waited to receive CAN replies and before entering gateway mode. this time is waited also if sendcanbroadcast is 0.
    uint8_t             rxcanbufferclear;   // if 1,  the ETH board will clear the buffers of rx can before it enters in gateway mode.
    uint8_t             ackrequired;        // it is 0 or 1. if 1 the command sends an ack back to inform the host.    
} eOuprot_cmd_CANGATEWAY_t;  EO_VERIFYsizeof(eOuprot_cmd_CANGATEWAY_t, 8)

 

typedef struct
{
    uint8_t             opc;        // it must be 0x11 (uprot_OPC_LEGACY_MAC_SET) 
    uint8_t             mac48[6];   // it is the mac 
} eOuprot_cmd_LEGACY_MAC_SET_t;  EO_VERIFYsizeof(eOuprot_cmd_LEGACY_MAC_SET_t, 7)



typedef struct
{
    uint8_t             opc;        // it must be 0x08 (uprot_OPC_LEGACY_IP_MASK_SET) 
    uint8_t             mask[4];    // it is the ip mask 
} eOuprot_cmd_LEGACY_IP_MASK_SET_t;  EO_VERIFYsizeof(eOuprot_cmd_LEGACY_IP_MASK_SET_t, 5)



// - declaration of extern public variables, ... but better using use _get/_set instead -------------------------------
// empty-section

// - declaration of extern public functions ---------------------------------------------------------------------------


extern const char * eouprot_process2string(eOuprot_process_t proc);

extern eOuprot_process_t eouprot_string2process(const char * name);

// retrieves the mask of the capabilities given the process and the protocol version.
extern uint32_t eouprot_get_capabilities(eOuprot_process_t proc, uint8_t protocolversion);

extern eObool_t eouprot_can_process_opcode(eOuprot_process_t proc, uint8_t protocolversion, eOuprot_opcodes_t opcode, uint32_t param);

extern uint8_t eouprot_process2index(eOuprot_process_t process);

extern eOuprot_process_t eouprot_raw2process(uint8_t rawvalue);


/** @}            
    end of group eo_cevcwervcrev666  
 **/

#ifdef __cplusplus
}       // closing brace for extern "C"
#endif 
 
#endif  // include-guard


// - end-of-file (leave a blank line after)----------------------------------------------------------------------------



