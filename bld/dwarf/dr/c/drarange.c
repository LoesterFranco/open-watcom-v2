/****************************************************************************
*
*                            Open Watcom Project
*
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
* Description:  DWARF debug ranges processing (.debug_aranges section).
*
****************************************************************************/


#include "drpriv.h"
#include "drgettab.h"
#include <string.h>

#include "pushpck1.h"
typedef struct arange_header {
    uint_32     len;
    uint_16     version;
    uint_32     dbg_pos;
    uint_8      addr_size;
    uint_8      seg_size;
} _WCUNALIGNED arange_header;
#include "poppck.h"

/* function prototypes */

typedef struct sec_file {
    dr_handle   pos;
    dr_handle   finish;
}sec_file;


static uint_32 ReadInt( dr_handle offset, int size )
/**************************************************/
{
    uint_32 ret;

    if( size == sizeof( uint_16 ) ) {
        ret = DWRVMReadWord( offset );
    } else if( size == sizeof( uint_32 ) ) {
        ret = DWRVMReadDWord( offset );
    } else if( size == sizeof( uint_8 ) ) {
        ret = DWRVMReadByte( offset );
    } else {
        DWREXCEPT( DREXCEP_BAD_DBG_INFO );
        ret = 0;
    }
    return( ret );
}

static uint_32 SectInt( sec_file *file, int size )
/************************************************/
{
    uint_32 ret;

    ret = ReadInt( file->pos, size );
    file->pos += size;
    return( ret );
}


static void SectRead( sec_file *file, void *buff, int size )
/**********************************************************/
{
    DWRVMRead( file->pos, buff, size  );
    file->pos += size;
}


extern void DRWalkARange( DRARNGWLK callback, void *data )
/********************************************************/
{
    dr_arange_data      arange;
    sec_file            file[1];
    arange_header       header;
    dr_handle           base;
    uint_32             tuple_size;
    uint_32             aligned_addr;
    bool                old_ver;
    bool                zero_padding = TRUE;

    base = DWRCurrNode->sections[DR_DEBUG_INFO].base;
    file->pos = DWRCurrNode->sections[DR_DEBUG_ARANGES].base;
    file->finish = file->pos + DWRCurrNode->sections[DR_DEBUG_ARANGES].size;
    for( ;; ) {
        if( file->pos >= file->finish ) break;
        SectRead( file, &header, sizeof( header ) );
        if( DWRCurrNode->byte_swap ) {
            SWAP_32( header.len );
            SWAP_16( header.version );
            SWAP_32( header.dbg_pos );
        }
        if( header.version != DWARF_VERSION ) DWREXCEPT( DREXCEP_BAD_DBG_VERSION );
        arange.dbg = header.dbg_pos+base;
        arange.addr_size = header.addr_size;
        arange.seg_size = header.seg_size;
        arange.is_start = TRUE;   /* start of bunch */
        tuple_size = (header.addr_size + header.seg_size + header.addr_size);
        /* Open Watcom prior to 1.4 didn't align tuples properly; unfortunately
         * it may be hard to tell if debug info was generated by Watcom!
         */
        /* NB: The writers of the DWARF 2/3 standard clearly didn't think very hard
         * about segmented architectures. If tuple size is 10 (32-bit offset, 16-bit
         * segment, 32-bit size) we assume the data was generated by our tools and
         * no alignment padding was used.
         */
        old_ver = (DWRCurrNode->wat_version == 1) || (DWRCurrNode->wat_version == 2) || (tuple_size == 10);
        aligned_addr = file->pos;
        aligned_addr = (aligned_addr + tuple_size - 1) & ~(tuple_size - 1);
        if( aligned_addr != file->pos ) {
            /* try reading the padding; if it's nonzero, assume it's not there */
            if( ReadInt( file->pos, header.addr_size ) != 0 )
                zero_padding = FALSE;
            if( header.seg_size && ReadInt( file->pos + header.addr_size, header.seg_size ) != 0 )
                zero_padding = FALSE;
            if( !old_ver && zero_padding )
                file->pos = aligned_addr;
        }
        for( ;; ) {
            arange.addr = SectInt( file, header.addr_size );
            if( header.seg_size != 0 ) {
                arange.seg = SectInt( file, header.seg_size );
            } else { /* flat */
                arange.seg = 0;
            }
            arange.len = SectInt( file, header.addr_size );
            if( arange.addr == 0 && arange.len == 0 ) break;
            if( !callback( data, &arange ) ) break;
            arange.is_start = FALSE;
        }
    }
}
