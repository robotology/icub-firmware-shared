-- embObj ROP dissector
-- SPDX-FileCopyrightText: Fondazione Istituto Italiano di Tecnologia (IIT)
-- SPDX-License-Identifier: BSD-3-Clause

-- The starting point for this dissector was these ChatGPT conversations:
-- https://chatgpt.com/share/6811f209-2edc-8006-bb06-93580b399e61
-- https://chatgpt.com/share/6812222c-fbe4-8006-9215-f175e661af3d
-- https://chatgpt.com/share/68123fba-2654-8006-8d2a-a330a31d4f28
-- The code has been then modified based on tests.

local p_rop   = Proto("embobj_rop","embObj ROP")

-- Frame header
local f_start = ProtoField.uint32 ("embobj_rop.start"   , "Start-of-Frame",   base.HEX)
local f_size  = ProtoField.uint16 ("embobj_rop.ropsize" , "ROPs byte-count",  base.DEC)
local f_nrops = ProtoField.uint16 ("embobj_rop.nrops"   , "# ROPs",           base.DEC)
local f_age   = ProtoField.uint64 ("embobj_rop.age"     , "Frame age [µs]" ,  base.DEC)
local f_seq   = ProtoField.uint64 ("embobj_rop.seq"     , "Sequence #",       base.DEC)
local f_end   = ProtoField.uint32 ("embobj_rop.end"     , "End-of-Frame",     base.HEX)

-- ROP header
local f_ctrl        = ProtoField.uint8  ("embobj_rop.ctrl"    , "CTRL"   , base.HEX)
local f_ctrl_cinfo  = ProtoField.uint8  ("embobj_rop.ctrl.cinfo","Confirmation",base.DEC,nil,0x03)
local f_ctrl_ptime  = ProtoField.bool   ("embobj_rop.ctrl.ptime","+TIME"       ,8,{"yes","no"},0x04)
local f_ctrl_psign  = ProtoField.bool   ("embobj_rop.ctrl.psign","+SIGN"       ,8,{"yes","no"},0x08)
local f_ctrl_rtime  = ProtoField.bool   ("embobj_rop.ctrl.rqtime","Req.TIME"   ,8,nil,0x10)
local f_ctrl_rconf  = ProtoField.bool   ("embobj_rop.ctrl.rqconf","Req.CONF"   ,8,nil,0x20)
local f_ctrl_ver    = ProtoField.uint8  ("embobj_rop.ctrl.ver","Ver"           ,base.DEC,nil,0xC0)
local f_opcode      = ProtoField.uint8  ("embobj_rop.opc"     , "OpCode",base.DEC,{
                           [1]="ASK",[2]="SAY",[3]="SET",[4]="SIG",[5]="RST"})
local f_dsize       = ProtoField.uint16 ("embobj_rop.dsize"   , "Data len",base.DEC)
local f_id32        = ProtoField.uint32 ("embobj_rop.id32"    , "ID32",   base.HEX)

-- sub-fields for ID32 (one byte each)
local f_id32_ep     = ProtoField.uint8 ("embobj_rop.id32.ep"  , "Endpoint",base.DEC)
local f_id32_ent    = ProtoField.uint8 ("embobj_rop.id32.ent" , "Entity"  ,base.DEC)
local f_id32_idx    = ProtoField.uint8 ("embobj_rop.id32.idx" , "Index"   ,base.DEC)
local f_id32_tag    = ProtoField.uint8 ("embobj_rop.id32.tag" , "Tag"     ,base.HEX)

local f_sign        = ProtoField.uint32 ("embobj_rop.sign"    , "Signature",base.HEX)
local f_time        = ProtoField.uint64 ("embobj_rop.time"    , "Time [µs]",base.DEC)

p_rop.fields = {f_start,f_size,f_nrops,f_age,f_seq,f_end,
                f_ctrl,f_ctrl_cinfo,f_ctrl_ptime,f_ctrl_psign,
                f_ctrl_rtime,f_ctrl_rconf,f_ctrl_ver,
                f_opcode,f_dsize,f_id32,f_id32_ep,f_id32_ent,
                f_id32_idx,f_id32_tag,f_sign,f_time}

-- These are endpoint names, as defined in eOprot_endpoint_t
-- see eth/embobj/plus/comm-v2/protocol/api/EoProtocol.h
local function endpoint_name(val)
    local lookup = {
        [0]   = "management",
        [1]   = "motioncontrol",
        [2]   = "analogsensors",
        [3]   = "skin",
        [254] = "all",
        [255] = "none",
      }
    return lookup[val] or "?"
end

-- These are entity names, as defined in eOprot_entity_t
-- see eth/embobj/plus/comm-v2/protocol/api/EoProtocol.h
local function entity_name(endpoint_name, val)
    if val == 255 then return "none" end

    local ent_names = {
        management = { [0]="comm",[1]="appl",[2]="info",[3]="service" },
        motioncontrol = { [0]="joint",[1]="motor",[2]="controller"   },
        analogsensors = { [0]="strain",[1]="mais",[2]="temperature",[3]="inertial3",[4]="psc",[5]="pos",[6]="ft",[7]="battery" },
        skin = { [0]="skin" },
    }

    -- Defensive lookup
    local endpoint = ent_names[endpoint_name]
    return endpoint and endpoint[val] or "?"
end

-- These are tag names, as defined in the header of each end point, see inline comments :
local function tag_name(endpoint_name, entity_name, val)
    if val == 255 then return "none" end

    local tag_names = {

        --   eth/embobj/plus/comm-v2/protocol/api/EoProtocolMN.h
        management = {
            -- eOprot_tag_mn_comm_t
            comm = {
                [0] = "wholeitem",
                [1] = "status",
                [2] = "status_managementprotocolversion",
                [3] = "cmmnds_command_querynumof",
                [4] = "cmmnds_command_queryarray",
                [5] = "cmmnds_command_replynumof",
                [6] = "cmmnds_command_replyarray",
                [7] = "cmmnds_command_config",
            },
            -- eOprot_tag_mn_appl_t
            appl = {
                [0] = "wholeitem",
                [1] = "config",
                [2] = "config_txratedivider",
                [3] = "status",
                [4] = "cmmnds_go2state",
                [5] = "cmmnds_timeset",
            },
            -- eOprot_tag_mn_info_t
            info = {
                [0] = "wholeitem",
                [1] = "config",
                [2] = "config_enabled",
                [3] = "status",
                [4] = "status_basic",
            },
            -- eOprot_tag_mn_service_t
            service = {
                [0] = "wholeitem",
                [1] = "status_commandresult",
                [2] = "cmmnds_command",
            },
        },

        --   eth/embobj/plus/comm-v2/protocol/api/EoProtocolMC.h
        motioncontrol = {
            -- eOprot_tag_mc_joint_t
            joint = {
                [0]  = "wholeitem",
                [1]  = "config",
                [2]  = "config_pidposition",
                [3]  = "config_pidvelocity",
                [4]  = "config_pidtorque",
                [5]  = "config_userlimits",
                [6]  = "config_impedance",
                [7]  = "config_motor_params",
                [8]  = "config_tcfiltertype",
                [9]  = "status",
                [10] = "status_core",
                [11] = "status_target",
                [12] = "status_core_modes_controlmodestatus",
                [13] = "status_core_modes_interactionmodestatus",
                [14] = "status_core_modes_ismotiondone",
                [15] = "status_addinfo_multienc",
                [16] = "status_debug",
                [17] = "inputs",
                [18] = "inputs_externallymeasuredtorque",
                [19] = "cmmnds_calibration",
                [20] = "cmmnds_setpoint",
                [21] = "cmmnds_stoptrajectory",
                [22] = "cmmnds_controlmode",
                [23] = "cmmnds_interactionmode",
            },
            -- eOprot_tag_mc_motor_t
            motor = {
                [0]  = "wholeitem",
                [1]  = "config",
                [2]  = "config_currentlimits",
                [3]  = "config_gearbox_M2J",
                [4]  = "config_rotorencoder",
                [5]  = "config_pwmlimit",
                [6]  = "config_temperaturelimit",
                [7]  = "config_pidcurrent",
                [8]  = "config_pidspeed",
                [9]  = "status",
                [10] = "status_basic",
            },
            -- eOprot_tag_mc_controller_t
            controller = {
                [0] = "wholeitem",
                [1] = "config",
                [2] = "status",
            },
        },

        --   eth/embobj/plus/comm-v2/protocol/api/EoProtocolAS.h
        analogsensors = {
            -- eOprot_tag_as_strain_t
            strain = {
                [0] = "wholeitem",
                [1] = "config",
                [2] = "status",
                [3] = "status_fullscale",
                [4] = "status_calibratedvalues",
                [5] = "status_uncalibratedvalues",
            },
            -- eOprot_tag_as_mais_t
            mais = {
                [0] = "wholeitem",
                [1] = "config",
                [2] = "config_mode",
                [3] = "config_datarate",
                [4] = "config_resolution",
                [5] = "status",
                [6] = "status_the15values",
            },
            -- eOprot_tag_as_temperature_t
            temperature = {
                [0] = "wholeitem",
                [1] = "config",
                [2] = "status",
                [3] = "cmmnds_enable",
            },
            -- eOprot_tag_as_inertial3_t
            inertial3 = {
                [0] = "wholeitem",
                [1] = "config",
                [2] = "status",
                [3] = "cmmnds_enable",
            },
            -- eOprot_tag_as_psc_t
            psc = {
                [0] = "wholeitem",
                [1] = "config",
                [2] = "status",
                [3] = "cmmnds_enable",
            },
            -- eOprot_tag_as_pos_t
            pos = {
                [0] = "wholeitem",
                [1] = "config",
                [2] = "status",
                [3] = "cmmnds_enable",
            },
            -- eOprot_tag_as_ft_t
            ft = {
                [0] = "config",
                [1] = "cmmnds_enable",
                [2] = "status",
                [3] = "status_timedvalue",
                [4] = "status_fullscale",
            },
            -- eOprot_tag_as_battery_t
            battery = {
                [0] = "config",
                [1] = "cmmnds_enable",
                [2] = "status",
                [3] = "status_timedvalue",
            },
        },

        -- ===========================  skin  ==============================
        --   eth/embobj/plus/comm-v2/protocol/api/EoProtocolSK.h
        skin = {
            -- eOprot_tag_sk_skin_t
            skin = {
                [0] = "wholeitem",
                [1] = "config_sigmode",
                [2] = "status_arrayofcandata",
                [3] = "cmmnds_boardscfg",
                [4] = "cmmnds_trianglescfg",
            },
        },
    }

    -- Defensive lookup
    local ep   = tag_names[endpoint_name]
    local ent  = ep and ep[entity_name]
    return ent and ent[val] or "?"
end



-- constants
local START_MAGIC = 0x12345678
local END_MAGIC   = 0x87654321
local HEADER_LEN  = 24
local ROP_HEADLEN =  8

-- helpers
local function name_or(tbl,val)   -- small helper: returns a string or "?"
    local v = tbl[val]
    return v and v or "?"
end

-- ROP dissection
local function dissect_single_rop(tvb,off,tree,idx)
    local sub = tree:add(p_rop,string.format("ROP [%d]",idx))

    -- CTRL
    sub:add_le(f_ctrl ,tvb(off,1))
    sub:add_le(f_ctrl_cinfo ,tvb(off,1))
    sub:add_le(f_ctrl_ptime ,tvb(off,1))
    sub:add_le(f_ctrl_psign ,tvb(off,1))
    sub:add_le(f_ctrl_rtime ,tvb(off,1))
    sub:add_le(f_ctrl_rconf ,tvb(off,1))
    sub:add_le(f_ctrl_ver   ,tvb(off,1))

    local ctrl  = tvb(off,1):le_uint()
    local ptime = bit.band(ctrl,0x04)~=0
    local psign = bit.band(ctrl,0x08)~=0
    off = off+1

    -- OPC / DSIZE
    sub:add(f_opcode,tvb(off,1));      off=off+1
    local dlen = tvb(off,2):le_uint()
    sub:add_le(f_dsize ,tvb(off,2));   off=off+2

    -- ID32
    sub:add_le(f_id32  ,tvb(off,4))

    -- break the 32 bits into 4 bytes (little-endian on the wire)
    local id32_le = tvb(off,4):le_uint()
    local ep   = bit.rshift(id32_le,24)       & 0xFF
    local ent  = bit.rshift(id32_le,16)       & 0xFF
    local idx  = bit.rshift(id32_le, 8)       & 0xFF
    local tag  = bit.band (id32_le,0xFF)

    local epname  = endpoint_name(ep)
    local entname = entity_name(epname,ent)
    local tagname = tag_name(epname,entname,tag)

    local idnode = sub:add(p_rop, tvb(off,4),
        string.format("ID32 → EP:%d (%s)  ENT:%d (%s)  IDX:%d  TAG:0x%02X (%s)",
            ep,epname, ent,entname, idx, tag, tagname))

    idnode:add_le(f_id32_ep ,tvb(off+3,1)):append_text(" ("..epname..")")
    idnode:add_le(f_id32_ent,tvb(off+2,1)):append_text(" ("..entname..")")
    idnode:add_le(f_id32_idx,tvb(off+1,1))
    idnode:add_le(f_id32_tag,tvb(off  ,1)):append_text(" ("..tagname..")")

    off = off+4

    -- optional SIGN / TIME
    if psign then sub:add_le(f_sign ,tvb(off,4)); off=off+4 end
    if ptime then sub:add_le(f_time ,tvb(off,8)); off=off+8 end

    -- DATA
    if dlen>0 then
        sub:add(tvb(off,dlen),"Data ("..dlen.." B)")
        off = off + dlen
    end

    -- 32-bit padding
    if off %4 ~=0 then off = off + (4 - (off%4)) end
    return off
end

-- dissector (frame) / registration
function p_rop.dissector(tvb,pinfo,tree)
    pinfo.cols.protocol = "embObj-ROP"
    local len = tvb:len()
    if len < HEADER_LEN+4 then return end
    if tvb(0,4):le_uint() ~= START_MAGIC then return end

    local roptree = tree:add(p_rop, tvb(),"embObj ROP Frame")
    roptree:add_le(f_start,tvb(0,4))
    roptree:add_le(f_size ,tvb(4,2))
    roptree:add_le(f_nrops,tvb(6,2))
    roptree:add_le(f_age  ,tvb( 8,8))
    roptree:add_le(f_seq  ,tvb(16,8))

    local off = HEADER_LEN
    for i=1,tvb(6,2):le_uint() do
        off = dissect_single_rop(tvb,off,roptree,i)
    end

    roptree:add_le(f_end,tvb(off,4))
end

local udp_table = DissectorTable.get("udp.port")
udp_table:add(12345,p_rop)
