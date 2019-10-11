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
#ifndef _MSGDESCRIPTIONEXT_H_
#define _MSGDESCRIPTIONEXT_H_

#include "MsgDescription.h"

const std::map<DiagnosticRopCode,std::string> ropCode{
                                                    {DiagnosticRopCode::empty,"empty"},
                                                    {DiagnosticRopCode::startlog,"startlog"},
                                                    {DiagnosticRopCode::stoplog,"stoplog"},
                                                    {DiagnosticRopCode::ethlog,"ethernetlog"},
                                                    {DiagnosticRopCode::diag,"diagnostic"} 
                                                 };

const std::map<DiagnosticRopSeverity,std::string> ropSeverity={
                                                {DiagnosticRopSeverity::empty,"empty"},
                                                {DiagnosticRopSeverity::debug,"debug"},
                                                {DiagnosticRopSeverity::trace,"trace"},
                                                {DiagnosticRopSeverity::info,"info"},
                                                {DiagnosticRopSeverity::warning,"warning"},
                                                {DiagnosticRopSeverity::error,"error"},
                                                {DiagnosticRopSeverity::critical,"critical"}};

                                                
#endif

// - end-of-file (leave a blank line after)----------------------------------------------------------------------------
