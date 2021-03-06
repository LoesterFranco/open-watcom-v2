/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2018 The Open Watcom Contributors. All Rights Reserved.
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  Identifiers for internal use by iswctype() and wctype().
*
****************************************************************************/


#ifndef INTWCTYP_H
#define INTWCTYP_H

#define WCTYPE_ALNUM            1
#define WCTYPE_ALPHA            2
#define WCTYPE_CNTRL            3
#define WCTYPE_DIGIT            4
#define WCTYPE_GRAPH            5
#define WCTYPE_LOWER            6
#define WCTYPE_PRINT            7
#define WCTYPE_PUNCT            8
#define WCTYPE_SPACE            9
#define WCTYPE_UPPER            10
#define WCTYPE_XDIGIT           11
#define WCTYPE_BLANK            12

extern char __wctype( const char *property );

#endif
