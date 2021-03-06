/*!
 * \file arc.c
 * \author Copyright (C) 2008, 2010 by Bert Timmerman <bert.timmerman@xs4all.nl>.
 * \brief Functions for a DXF arc entity (\c ARC).
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


#include "arc.h"


/*!
 * \brief Allocate memory for a \c DxfArc.
 *
 * Fill the memory contents with zeros.
 */
DxfArc *
dxf_arc_new ()
{
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Entering dxf_arc_new () function.\n",
                __FILE__, __LINE__);
#endif
        DxfArc *dxf_arc = NULL;
        size_t size;

        size = sizeof (DxfArc);
        /* avoid malloc of 0 bytes */
        if (size == 0) size = 1;
        if ((dxf_arc = malloc (size)) == NULL)
        {
                fprintf (stderr, "ERROR in dxf_arc_new () could not allocate memory for a DxfArc struct.\n");
                dxf_arc = NULL;
        }
        else
        {
                memset (dxf_arc, 0, size);
        }
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Leaving dxf_arc_new () function.\n",
                __FILE__, __LINE__);
#endif
        return (dxf_arc);
}


/*!
 * \brief Allocate memory and initialize data fields in a \c ARC entity.
 * 
 * \return \c NULL when no memory was allocated, a pointer to the
 * allocated memory when succesful.
 */
DxfArc *
dxf_arc_init
(
        DxfArc *dxf_arc
                /*!< DXF arc entity. */
)
{
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Entering dxf_arc_init () function.\n",
                __FILE__, __LINE__);
#endif
        dxf_arc = dxf_arc_new ();
        if (dxf_arc == NULL)
        {
              fprintf (stderr, "ERROR in dxf_arc_init () could not allocate memory for a DxfArc struct.\n");
              return (NULL);
        }
        dxf_arc->common.id_code = 0;
        dxf_arc->common.linetype = strdup (DXF_DEFAULT_LINETYPE);
        dxf_arc->common.layer = strdup (DXF_DEFAULT_LAYER);
        dxf_arc->x0 = 0.0;
        dxf_arc->y0 = 0.0;
        dxf_arc->z0 = 0.0;
        dxf_arc->extr_x0 = 0.0;
        dxf_arc->extr_y0 = 0.0;
        dxf_arc->extr_z0 = 0.0;
        dxf_arc->common.thickness = 0.0;
        dxf_arc->radius = 0.0;
        dxf_arc->start_angle = 0.0;
        dxf_arc->end_angle = 0.0;
        dxf_arc->common.color = DXF_COLOR_BYLAYER;
        dxf_arc->common.paperspace = DXF_MODELSPACE;
        dxf_arc->common.acad_version_number = 0;
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Leaving dxf_arc_init () function.\n",
                __FILE__, __LINE__);
#endif
        return (dxf_arc);
}


/*!
 * \brief Read data from a DXF file into an \c ARC entity.
 *
 * The last line read from file contained the string "ARC". \n
 * Now follows some data for the \c ARC, to be terminated with a "  0"
 * string announcing the following entity, or the end of the \c ENTITY
 * section marker \c ENDSEC. \n
 * While parsing the DXF file store data in \c dxf_arc. \n
 *
 * \return \c EXIT_SUCCESS when done, or \c EXIT_FAILURE when an error
 * occurred.
 */
int
dxf_arc_read
(
        char *filename,
                /*!< filename of input file (or device). */
        FILE *fp,
                /*!< filepointer to the input file (or device). */
        int *line_number,
                /*!< current line number in the input file (or device). */
        DxfArc *dxf_arc,
                /*!< DXF arc entity. */
        int acad_version_number
                /*!< AutoCAD version number. */
)
{
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Entering dxf_arc_read () function.\n",
                __FILE__, __LINE__);
#endif
        char *temp_string = NULL;

        if (!dxf_arc)
        {
                dxf_arc = dxf_arc_new ();
        }
        (*line_number)++;
        fscanf (fp, "%[^\n]", temp_string);
        while (strcmp (temp_string, "0") != 0)
        {
                if (ferror (fp))
                {
                        fprintf (stderr, "Error in dxf_arc_read () while reading from: %s in line: %d.\n",
                                filename, *line_number);
                        fclose (fp);
                        return (EXIT_FAILURE);
                }
                if (strcmp (temp_string, "5") == 0)
                {
                        /* Now follows a string containing a sequential
                         * id number. */
                        (*line_number)++;
                        fscanf (fp, "%x\n", &dxf_arc->common.id_code);
                }
                else if (strcmp (temp_string, "6") == 0)
                {
                        /* Now follows a string containing a linetype
                         * name. */
                        (*line_number)++;
                        fscanf (fp, "%s\n", dxf_arc->common.linetype);
                }
                else if (strcmp (temp_string, "8") == 0)
                {
                        /* Now follows a string containing a layer name. */
                        (*line_number)++;
                        fscanf (fp, "%s\n", dxf_arc->common.layer);
                }
                else if (strcmp (temp_string, "10") == 0)
                {
                        /* Now follows a string containing the
                         * X-coordinate of the center point. */
                        (*line_number)++;
                        fscanf (fp, "%lf\n", &dxf_arc->x0);
                }
                else if (strcmp (temp_string, "20") == 0)
                {
                        /* Now follows a string containing the
                         * Y-coordinate of the center point. */
                        (*line_number)++;
                        fscanf (fp, "%lf\n", &dxf_arc->y0);
                }
                else if (strcmp (temp_string, "30") == 0)
                {
                        /* Now follows a string containing the
                         * Z-coordinate of the center point. */
                        (*line_number)++;
                        fscanf (fp, "%lf\n", &dxf_arc->z0);
                }
                else if ((acad_version_number <= AutoCAD_11)
                        && (strcmp (temp_string, "38") == 0)
                        && (dxf_arc->z0 = 0.0))
                {
                        /* Elevation is a pre AutoCAD R11 variable
                         * so additional testing for the version should
                         * probably be added.
                         * Now follows a string containing the
                         * elevation. */
                        (*line_number)++;
                        fscanf (fp, "%lf\n", &dxf_arc->z0);
                }
                else if (strcmp (temp_string, "39") == 0)
                {
                        /* Now follows a string containing the
                         * thickness. */
                        (*line_number)++;
                        fscanf (fp, "%lf\n", &dxf_arc->common.thickness);
                }
                else if (strcmp (temp_string, "40") == 0)
                {
                        /* Now follows a string containing the
                         * radius. */
                        (*line_number)++;
                        fscanf (fp, "%lf\n", &dxf_arc->radius);
                }
                else if (strcmp (temp_string, "50") == 0)
                {
                        /* Now follows a string containing the
                         * start angle. */
                        (*line_number)++;
                        fscanf (fp, "%lf\n", &dxf_arc->start_angle);
                }
                else if (strcmp (temp_string, "51") == 0)
                {
                        /* Now follows a string containing the
                         * end angle. */
                        (*line_number)++;
                        fscanf (fp, "%lf\n", &dxf_arc->end_angle);
                }
                else if (strcmp (temp_string, "62") == 0)
                {
                        /* Now follows a string containing the
                         * color value. */
                        (*line_number)++;
                        fscanf (fp, "%d\n", &dxf_arc->common.color);
                }
                else if (strcmp (temp_string, "67") == 0)
                {
                        /* Now follows a string containing the
                         * paperspace value. */
                        (*line_number)++;
                        fscanf (fp, "%d\n", &dxf_arc->common.paperspace);
                }
                else if ((acad_version_number >= AutoCAD_12)
                        && (strcmp (temp_string, "100") == 0))
                {
                        /* Subclass markers are post AutoCAD R12
                         * variable so additional testing for the
                         * version should probably be added here.
                         * Now follows a string containing the
                         * subclass marker value. */
                        (*line_number)++;
                        fscanf (fp, "%s\n", temp_string);
                }
                else if (strcmp (temp_string, "210") == 0)
                {
                        /* Now follows a string containing the
                         * X-value of the extrusion vector. */
                        (*line_number)++;
                        fscanf (fp, "%lf\n", &dxf_arc->extr_x0);
                }
                else if (strcmp (temp_string, "220") == 0)
                {
                        /* Now follows a string containing the
                         * Y-value of the extrusion vector. */
                        (*line_number)++;
                        fscanf (fp, "%lf\n", &dxf_arc->extr_y0);
                }
                else if (strcmp (temp_string, "230") == 0)
                {
                        /* Now follows a string containing the
                         * Z-value of the extrusion vector. */
                        (*line_number)++;
                        fscanf (fp, "%lf\n", &dxf_arc->extr_z0);
                }
                else if (strcmp (temp_string, "999") == 0)
                {
                        /* Now follows a string containing a comment. */
                        (*line_number)++;
                        fscanf (fp, "%s\n", temp_string);
                        fprintf (stdout, "DXF comment: %s\n", temp_string);
                }
                else
                {
                        fprintf (stderr, "Warning: in dxf_arc_read () unknown string tag found while reading from: %s in line: %d.\n",
                                filename, *line_number);
                }
        }
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Leaving dxf_arc_read () function.\n",
                __FILE__, __LINE__);
#endif
        return (EXIT_SUCCESS);
}


/*!
 * \brief Write DXF output for an arc entity.
 *
 * \return \c EXIT_SUCCESS when done, or \c EXIT_FAILURE when an error
 * occurred.
 */
int
dxf_arc_write_lowlevel
(
        FILE *fp,
                /*!< File pointer to output file (or device). */
        int id_code,
                /*!< Identification number for the entity.\n
                 * this is to be an unique (sequential) number in the DXF
                 * file.\n
                 * Group code = 5. */
        char *linetype,
                /*!< The linetype of the entity.\n
                 * Defaults to \c BYLAYER if ommitted in the DXF file.\n
                 * Group code = 6. */
        char *layer,
                /*!< Layer on which the entity is drawn.\n
                 * Defaults to layer "0" if no valid layername is given.\n
                 * Group code = 8. */
        double x0,
                /*!< X-value of the starting point coordinate.\n
                 * Group code = 10. */
        double y0,
                /*!< Y-value of the starting point coordinate.\n
                 * Group code = 20. */
        double z0,
                /*!< Z-value of the starting point coordinate.\n
                 * Group code = 30. */
        double extr_x0,
                /*!< X-value of the extrusion vector.\n
                 * Defaults to 0.0 if ommitted in the DXF file.\n
                 * Group code = 210. */
        double extr_y0,
                /*!< Y-value of the extrusion vector.\n
                 * Defaults to 0.0 if ommitted in the DXF file.\n
                 * Group code = 220. */
        double extr_z0,
                /*!< Z-value of the extrusion vector.\n
                 * Defaults to 1.0 if ommitted in the DXF file.\n
                 * Group code = 230. */
        double thickness,
                /*!< Thickness of the arc in the local Z-direction.\n
                 * Defaults to 0.0 if ommitted in the DXF file.\n
                 * Group code = 39. */
        double radius,
                /*!< Radius of the arc.\n
                 * Group code = 40. */
        double start_angle,
                /*!< Start angle of the arc.\n
                 * Group code = 50. */
        double end_angle,
                /*!< End angle of the arc.\n
                 * Group code = 51. */
        int color,
                /*!< Color of the entity.\n
                 * Defaults to \c BYLAYER if ommitted in the DXF file.\n
                 * Note that entities encapsulated in a block with the
                 * color \c BYBLOCK are represented in the "native" color of
                 * the \c BLOCK entity.\n
                 * Group code = 62. */
        int paperspace,
                /*!< Entities are to be drawn on either \c PAPERSPACE or
                 * \c MODELSPACE.\n
                 * Defaults to \c MODELSPACE if ommitted in the DXF file.\n
                 * Group code = 67. */
        int acad_version_number
                /*!< AutoCAD version number. */
)
{
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Entering dxf_arc_write_lowlevel () function.\n",
                __FILE__, __LINE__);
#endif
        char *dxf_entity_name = strdup ("ARC");

        if (start_angle == end_angle)
        {
                fprintf (stderr, "Error in dxf_arc_write_lowlevel () start angle and end angle are identical for the %s entity with id-code: %x.\n",
                        dxf_entity_name, id_code);
                fprintf (stderr, "    skipping %s entity.\n", dxf_entity_name);
                return (EXIT_FAILURE);
        }
        if (start_angle > 360.0)
        {
                fprintf (stderr, "Error in dxf_arc_write_lowlevel () start angle is greater than 360 degrees for the %s entity with id-code: %x.\n",
                        dxf_entity_name, id_code);
                fprintf (stderr, "    skipping %s entity.\n", dxf_entity_name);
                return (EXIT_FAILURE);
        }
        if (start_angle < 0.0)
        {
                fprintf (stderr, "Error in dxf_arc_write_lowlevel () start angle is lesser than 0 degrees for the %s entity with id-code: %x.\n",
                        dxf_entity_name, id_code);
                fprintf (stderr, "    skipping %s entity.\n", dxf_entity_name);
                return (EXIT_FAILURE);
        }
        if (end_angle > 360.0)
        {
                fprintf (stderr, "Error in dxf_arc_write_lowlevel () end angle is greater than 360 degrees for the %s entity with id-code: %x.\n",
                        dxf_entity_name, id_code);
                fprintf (stderr, "    skipping %s entity.\n", dxf_entity_name);
                return (EXIT_FAILURE);
        }
        if (end_angle < 0.0)
        {
                fprintf (stderr, "Error in dxf_arc_write_lowlevel () end angle is lesser than 0 degrees for the %s entity with id-code: %x.\n",
                        dxf_entity_name, id_code);
                fprintf (stderr, "    skipping %s entity.\n", dxf_entity_name);
                return (EXIT_FAILURE);
        }
        if (radius == 0.0)
        {
                fprintf (stderr, "Error in dxf_arc_write_lowlevel () radius value equals 0.0 for the %s entity with id-code: %x.\n",
                        dxf_entity_name, id_code);
                fprintf (stderr, "    skipping %s entity.\n", dxf_entity_name);
                return (EXIT_FAILURE);
        }
        if (strcmp (layer, "") == 0)
        {
                fprintf (stderr, "Warning in dxf_arc_write_lowlevel () empty layer string for the %s entity with id-code: %x.\n",
                        dxf_entity_name, id_code);
                fprintf (stderr, "    %s entity is relocated to default layer.\n",
                        dxf_entity_name);
                layer = strdup (DXF_DEFAULT_LAYER);
        }
        fprintf (fp, "  0\n%s\n", dxf_entity_name);
        if (acad_version_number >= AutoCAD_14)
        {
                fprintf (fp, "100\nAcDbCircle\n");
        }
        if (id_code != -1)
        {
                fprintf (fp, "  5\n%x\n", id_code);
        }
        if (strcmp (linetype, DXF_DEFAULT_LINETYPE) != 0)
        {
                fprintf (fp, "  6\n%s\n", linetype);
        }
        fprintf (fp, "  8\n%s\n", layer);
        fprintf (fp, " 10\n%f\n", x0);
        fprintf (fp, " 20\n%f\n", y0);
        fprintf (fp, " 30\n%f\n", z0);
        if (acad_version_number >= AutoCAD_12)
        {
                fprintf (fp, "210\n%f\n", extr_x0);
                fprintf (fp, "220\n%f\n", extr_y0);
                fprintf (fp, "230\n%f\n", extr_z0);
        }
        if (thickness != 0.0)
        {
                fprintf (fp, " 39\n%f\n", thickness);
        }
        fprintf (fp, " 40\n%f\n", radius);
        if (acad_version_number >= AutoCAD_14)
        {
                fprintf (fp, "100\nAcDbArc\n");
        }
        fprintf (fp, " 50\n%f\n", start_angle);
        fprintf (fp, " 51\n%f\n", end_angle);
        if (color != DXF_COLOR_BYLAYER)
        {
                fprintf (fp, " 62\n%d\n", color);
        }
        if (paperspace == DXF_PAPERSPACE)
        {
                fprintf (fp, " 67\n%d\n", DXF_PAPERSPACE);
        }
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Leaving dxf_arc_write_lowlevel () function.\n",
                __FILE__, __LINE__);
#endif
        return (EXIT_SUCCESS);
}


/*!
 * \brief Write DXF output to a file for an arc entity.
 *
 * \return \c EXIT_SUCCESS when done, or \c EXIT_FAILURE when an error
 * occurred.
 */
int
dxf_arc_write
(
        FILE *fp,
                /*!< file pointer to output file (or device). */
        DxfArc dxf_arc
                /*!< DXF arc entity. */
)
{
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Entering dxf_arc_write () function.\n",
                __FILE__, __LINE__);
#endif
        char *dxf_entity_name = strdup ("ARC");

        if (&dxf_arc == NULL)
        {
                return (EXIT_FAILURE);
                fprintf (stderr, "Error in dxf_arc_write () a NULL pointer was passed.\n");
        }
        if (dxf_arc.start_angle == dxf_arc.end_angle)
        {
                fprintf (stderr, "Error in dxf_arc_write () start angle and end angle are identical for the %s entity with id-code: %x.\n",
                        dxf_entity_name, dxf_arc.common.id_code);
                fprintf (stderr, "\tskipping %s entity.\n",
                        dxf_entity_name);
                return (EXIT_FAILURE);
        }
        if (dxf_arc.start_angle > 360.0)
        {
                fprintf (stderr, "Error in dxf_arc_write () start angle is greater than 360 degrees for the %s entity with id-code: %x.\n",
                        dxf_entity_name, dxf_arc.common.id_code);
                fprintf (stderr, "\tskipping %s entity.\n",
                        dxf_entity_name);
                return (EXIT_FAILURE);
        }
        if (dxf_arc.start_angle < 0.0)
        {
                fprintf (stderr, "Error in dxf_arc_write () start angle is lesser than 0 degrees for the %s entity with id-code: %x.\n",
                        dxf_entity_name, dxf_arc.common.id_code);
                fprintf (stderr, "\tskipping %s entity.\n",
                        dxf_entity_name);
                return (EXIT_FAILURE);
        }
        if (dxf_arc.end_angle > 360.0)
        {
                fprintf (stderr, "Error in dxf_arc_write () end angle is greater than 360 degrees for the %s entity with id-code: %x.\n",
                        dxf_entity_name, dxf_arc.common.id_code);
                fprintf (stderr, "\tskipping %s entity.\n",
                        dxf_entity_name);
                return (EXIT_FAILURE);
        }
        if (dxf_arc.end_angle < 0.0)
        {
                fprintf (stderr, "Error in dxf_arc_write () end angle is lesser than 0 degrees for the %s entity with id-code: %x.\n",
                        dxf_entity_name, dxf_arc.common.id_code);
                fprintf (stderr, "\tskipping %s entity.\n",
                        dxf_entity_name);
                return (EXIT_FAILURE);
        }
        if (dxf_arc.radius == 0.0)
        {
                fprintf (stderr, "Error in dxf_arc_write () radius value equals 0.0 for the %s entity with id-code: %x.\n",
                        dxf_entity_name, dxf_arc.common.id_code);
                fprintf (stderr, "    skipping %s entity.\n",
                        dxf_entity_name);
                return (EXIT_FAILURE);
        }
        if (strcmp (dxf_arc.common.layer, "") == 0)
        {
                fprintf (stderr, "Warning in dxf_arc_write () empty layer string for the %s entity with id-code: %x.\n",
                        dxf_entity_name, dxf_arc.common.id_code);
                fprintf (stderr, "\t%s entity is relocated to default layer.\n",
                        dxf_entity_name);
                dxf_arc.common.layer = DXF_DEFAULT_LAYER;
        }
        fprintf (fp, "  0\n%s\n", dxf_entity_name);
        if (dxf_arc.common.acad_version_number >= AutoCAD_14)
        {
                fprintf (fp, "100\nAcDbArc\n");
        }
        if (dxf_arc.common.id_code != -1)
        {
                fprintf (fp, "  5\n%x\n", dxf_arc.common.id_code);
        }
        if (strcmp (dxf_arc.common.linetype, DXF_DEFAULT_LINETYPE) != 0)
        {
                fprintf (fp, "  6\n%s\n", dxf_arc.common.linetype);
        }
        fprintf (fp, "  8\n%s\n", dxf_arc.common.layer);
        fprintf (fp, " 10\n%f\n", dxf_arc.x0);
        fprintf (fp, " 20\n%f\n", dxf_arc.y0);
        fprintf (fp, " 30\n%f\n", dxf_arc.z0);
        if (dxf_arc.common.acad_version_number >= AutoCAD_12)
        {
                fprintf (fp, "210\n%f\n", dxf_arc.extr_x0);
                fprintf (fp, "220\n%f\n", dxf_arc.extr_y0);
                fprintf (fp, "230\n%f\n", dxf_arc.extr_z0);
        }
        if (dxf_arc.common.thickness != 0.0)
        {
                fprintf (fp, " 39\n%f\n", dxf_arc.common.thickness);
        }
        fprintf (fp, " 40\n%f\n", dxf_arc.radius);
        fprintf (fp, " 50\n%f\n", dxf_arc.start_angle);
        fprintf (fp, " 51\n%f\n", dxf_arc.end_angle);
        if (dxf_arc.common.color != DXF_COLOR_BYLAYER)
        {
                fprintf (fp, " 62\n%d\n", dxf_arc.common.color);
        }
        if (dxf_arc.common.paperspace == DXF_PAPERSPACE)
        {
                fprintf (fp, " 67\n%d\n", DXF_PAPERSPACE);
        }
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Leaving dxf_arc_write () function.\n",
                __FILE__, __LINE__);
#endif
        return (EXIT_SUCCESS);
}


/* EOF*/
