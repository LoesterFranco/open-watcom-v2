/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2015-2016 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Implementation of setsockopt() for Linux and RDOS.
*
****************************************************************************/


#include "variety.h"
#include <sys/types.h>
#include <sys/socket.h>

#if defined( __LINUX__ )
#include "linuxsys.h"
#endif

_WCRTLINK int setsockopt( int s, int level, int optname, const void *optval, socklen_t optlen )
{
#if defined( __LINUX__ )
    unsigned long args[5];

    args[0] = (unsigned long)s;
    args[1] = (unsigned long)level;
    args[2] = (unsigned long)optname;
    args[3] = (unsigned long)optval;
    args[4] = (unsigned long)optlen;
    return( __socketcall( SYS_SETSOCKOPT, args ) );
#elif defined( __RDOS__ )

    /* unused parameters */ (void)s; (void)level; (void)optname; (void)optval; (void)optlen;

    return( -1 );
#else

    /* unused parameters */ (void)s; (void)level; (void)optname; (void)optval; (void)optlen;

    return( -1 );
#endif
}
