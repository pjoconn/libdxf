/*!
 * \file entity.h
 * \author Copyright (C) 2008, 2009 by Bert Timmerman <bert.timmerman@xs4all.nl>.
 * \brief Header file for DXF entity functions.
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
 * For more details see http://www.autodesk.com.\n\n
 * <hr>
 */


/*!
 * \brief DXF entity types.
 */
typedef enum
dxf_entity_type
{
        APPID,
        ARC,
        ATTDEF,
        ATTRIB,
        BLOCK,
        CIRCLE,
        DIMSTYLE,
        HEADER,
        INSERT,
        LAYER,
        LINE,
        LTYPE,
        POINT,
        POLYLINE,
        SHAPE,
        SOLID,
        STYLE,
        TEXT,
        TRACE,
        UCS,
        VIEW,
        VPORT
} DxfEntityType;


char *dxf_entities_list = NULL;


int dxf_skip_entity (char *dxf_entity_name);
int dxf_write_entities (char *dxf_entities_list, int acad_version_number);


/* EOF */