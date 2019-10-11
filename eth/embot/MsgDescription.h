/*
 * Copyright (C) 2019 iCub Facility - Istituto Italiano di Tecnologia
 * Author:  Luca Tricerri
 * email:   luca.tricerri@iit.it
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
#ifndef _MSGDESCRIPTION_H_
#define _MSGDESCRIPTION_H_

#include <map>
#include <string>

enum class DiagnosticRopCode : uint16_t
{
    empty = 0,
    startlog,
    stoplog,
    ethlog,
    diag,
    forceflush,
    unforceflush,
};

enum class DiagnosticRopSeverity : uint16_t
{
    empty = 0,
    debug,
    trace,
    info,
    warning,
    error,
    critical
};

enum class DiagnosticRopString : uint16_t
{
    empty = 0,
    ethdown,
    ethup,
};  

#endif // include-guard DiagnosticRopSeverity::error,

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------
