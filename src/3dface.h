/*!
 * \file 3dface.h
 * \author Copyright (C) 2010 by Bert Timmerman <bert.timmerman@xs4all.nl>.
 * \brief Header file for a DXF 3D face entity (\c 3DFACE).
 *
 * <hr>
 * <h1><b>Copyright Notices.</b></h1>\n
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.\n\n
 * This program is distributed in the hope that it will be useful, but
 * <b>WITHOUT ANY WARRANTY</b>; without even the implied warranty of
 * <b>MERCHANTABILITY</b> or <b>FITNESS FOR A PARTICULAR PURPOSE</b>.\n
 * See the GNU General Public License for more details.\n\n
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to:\n
 * Free Software Foundation, Inc.,\n
 * 59 Temple Place,\n
 * Suite 330,\n
 * Boston,\n
 * MA 02111 USA.\n
 * \n
 * Drawing eXchange Format (DXF) is a defacto industry standard for the
 * exchange of drawing files between various Computer Aided Drafting
 * programs.\n
 * DXF is an industry standard designed by Autodesk(TM).\n
 * For more details see http://www.autodesk.com.
 * <hr>
 */


#ifndef _3DFACE_H
#define _3DFACE_H


#include "global.h"
#include "entity.h"


/*!
 * \brief DXF definition of an AutoCAD 3D face entity (\c 3DFACE).
 */
typedef struct
dxf_3dface
{
        DxfEntity common;
                /*!< common properties for DXF entities. */
        double x0;
                /*!< group code = 10\n
                 * base point. */
        double y0;
                /*!< group code = 20\n
                 * base point. */
        double z0;
                /*!< group code = 30\n
                 * base point. */
        double x1;
                /*!< group code = 11\n
                 * alignment point. */
        double y1;
                /*!< group code = 21\n
                 * alignment point. */
        double z1;
                /*!< group code = 31\n
                 * alignment point. */
        double x2;
                /*!< group code = 12\n
                 * alignment point. */
        double y2;
                /*!< group code = 22\n
                 * alignment point. */
        double z2;
                /*!< group code = 32\n
                 * alignment point. */
        double x3;
                /*!< group code = 13\n
                 * alignment point. */
        double y3;
                /*!< group code = 23\n
                 * alignment point. */
        double z3;
                /*!< group code = 33\n
                 * alignment point. */
        int flag;
                /*!< group code = 70\n
                 * optional, defaults to 0\n
                 * bit coded:\n
                 * 1 = First edge is invisible\n
                 * 2 = Second edge is invisible\n
                 * 4 = Third edge is invisible\n
                 * 8 = Fourth edge is invisible\n */
} Dxf3dface, * Dxf3dfacePtr;


Dxf3dface *
dxf_3dface_new ();
Dxf3dface *
dxf_3dface_init
(
        Dxf3dface *dxf_3dface
);
int
dxf_3dface_read
(
        char *filename,
        FILE *fp,
        int *line_number,
        Dxf3dface *dxf_3dface,
        int acad_version_number
);
int
dxf_3dface_write_lowlevel
(
        FILE *fp,
        int id_code,
        char *linetype,
        char *layer,
        double x0,
        double y0,
        double z0,
        double x1,
        double y1,
        double z1,
        double x2,
        double y2,
        double z2,
        double x3,
        double y3,
        double z3,
        double thickness,
        int color,
        int paperspace,
        int flag
);
int
dxf_3dface_write
(
        FILE *fp,
        Dxf3dface dxf_3dface
);


#endif /* _3DFACE_H */


/* EOF */
