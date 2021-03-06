/*!
 * \file class.c
 * \author Copyright (C) 2009, 2010, 2011 by Bert Timmerman <bert.timmerman@xs4all.nl>.
 * \brief Functions for a DXF class entity (\c CLASS).
 *
 * The group codes described in this chapter are found only in DXF files.\n
 * The CLASSES section holds the information for application-defined classes
 * whose instances appear in the BLOCKS, ENTITIES, and OBJECTS sections of
 * the database.\n
 * It is assumed that a class definition is permanently fixed in the class
 * hierarchy.\n
 * All fields are required.\n
 * \n
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


#include "global.h"
#include "class.h"


/*!
 * \brief Allocate memory for a \c DxfClass.
 *
 * Fill the memory contents with zeros.
 */
DxfClass *
dxf_class_new ()
{
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Entering dxf_class_new () function.\n",
                __FILE__, __LINE__);
#endif
        DxfClass *dxf_class = NULL;
        size_t size;

        size = sizeof (DxfClass);
        /* avoid malloc of 0 bytes */
        if (size == 0) size = 1;
        if ((dxf_class = malloc (size)) == NULL)
        {
                fprintf (stderr, "ERROR in dxf_class_new () could not allocate memory for a DxfClass struct.\n");
                dxf_class = NULL;
        }
        else
        {
                memset (dxf_class, 0, size);
        }
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Leaving dxf_class_new () function.\n",
                __FILE__, __LINE__);
#endif
        return (dxf_class);
}


/*!
 * \brief Allocate memory and initialize data fields in a \c CLASS entity.
 * 
 * \return \c NULL when no memory was allocated, a pointer to the
 * allocated memory when succesful.
 */
DxfClass *
dxf_class_init
(
        DxfClass *dxf_class
                /*!< DXF class entity. */
)
{
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Entering dxf_class_init () function.\n",
                __FILE__, __LINE__);
#endif
        dxf_class = dxf_class_new ();
        if (dxf_class == NULL)
        {
              fprintf (stderr, "ERROR in dxf_class_init () could not allocate memory for a DxfClass struct.\n");
              return (NULL);
        }
        dxf_class->record_type = strdup ("");
        dxf_class->record_name = strdup ("");
        dxf_class->class_name = strdup ("");
        dxf_class->app_name = strdup ("");
        dxf_class->proxy_cap_flag = 0;
        dxf_class->was_a_proxy_flag = 0;
        dxf_class->is_an_entity_flag = 0;
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Leaving dxf_class_init () function.\n",
                __FILE__, __LINE__);
#endif
        return (dxf_class);
}


/*!
 * \brief Read data from a DXF file into an \c CLASS entity.
 *
 * The last line read from file contained the string "CLASS". \n
 * Now follows some data for the \c CLASS, to be terminated with a "  0"
 * string announcing the following entity, or the end of the \c ENTITY
 * section marker \c ENDCLASS. \n
 * While parsing the DXF file store data in \c dxf_class. \n
 *
 * \return \c EXIT_SUCCESS when done, or \c EXIT_FAILURE when an error
 * occurred.
 */
int
dxf_class_read
(
        char *filename,
                /*!< filename of input file (or device). */
        FILE *fp,
                /*!< filepointer to the input file (or device). */
        int *line_number,
                /*!< current line number in the input file (or device). */
        DxfClass *dxf_class,
                /*!< DXF class entity. */
        int acad_version_number
                /*!< AutoCAD version number. */
)
{
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Entering dxf_class_read () function.\n",
                __FILE__, __LINE__);
#endif
        char *temp_string = NULL;

        if (!dxf_class)
        {
                dxf_class = dxf_class_new ();
        }
        (*line_number)++;
        fscanf (fp, "%[^\n]", temp_string);
        while (strcmp (temp_string, "0") != 0)
        {
                if (ferror (fp))
                {
                        fprintf (stderr, "Error in dxf_class_read () while reading from: %s in line: %d.\n",
                                filename, *line_number);
                        fclose (fp);
                        return (EXIT_FAILURE);
                }
                if (strcmp (temp_string, "0") == 0)
                {
                        /* Now follows a string containing a record type
                         * number. */
                        /*! \bug \c record_type has group code 0 which
                         * will cause a parsing error and \c record_type
                         * and other \c class variables  will not be
                         * read. See the while condition above.
                         */
                        (*line_number)++;
                        fscanf (fp, "%s\n", &dxf_class->record_type);
                }
                else if (strcmp (temp_string, "1") == 0)
                {
                        /* Now follows a string containing a record
                         * name. */
                        (*line_number)++;
                        fscanf (fp, "%s\n", dxf_class->record_name);
                }
                else if (strcmp (temp_string, "2") == 0)
                {
                        /* Now follows a string containing a class name.
                         */
                        (*line_number)++;
                        fscanf (fp, "%s\n", dxf_class->class_name);
                }
                else if (strcmp (temp_string, "3") == 0)
                {
                        /* Now follows a string containing the
                         * application name. */
                        (*line_number)++;
                        fscanf (fp, "%s\n", &dxf_class->app_name);
                }
                else if (strcmp (temp_string, "90") == 0)
                {
                        /* Now follows a string containing the
                         * proxy cap flag value. */
                        (*line_number)++;
                        fscanf (fp, "%d\n", &dxf_class->proxy_cap_flag);
                }
                else if (strcmp (temp_string, "280") == 0)
                {
                        /* Now follows a string containing the
                         * was a proxy flag value. */
                        (*line_number)++;
                        fscanf (fp, "%d\n", &dxf_class->was_a_proxy_flag);
                }
                else if (strcmp (temp_string, "281") == 0)
                {
                        /* Now follows a string containing the
                         * is an entity flag value. */
                        (*line_number)++;
                        fscanf (fp, "%d\n", &dxf_class->is_an_entity_flag);
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
                        fprintf (stderr, "Warning: in dxf_class_read () unknown string tag found while reading from: %s in line: %d.\n",
                                filename, *line_number);
                }
        }
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Leaving dxf_class_read () function.\n",
                __FILE__, __LINE__);
#endif
        return (EXIT_SUCCESS);
}


/*!
 * \brief Write DXF output to a file for a class section.
 *
 * Each entry in the CLASSES section contains the groups described
 * in the following table, as stated in the documentation for
 * AutoCAD DXF release 2000.
 */
int
dxf_class_write_lowlevel
(
        FILE *fp,
                /*!< file pointer to output file (or device). */
        char *record_name,
                /*!< group code = 1.\n
                 * Class DXF record name.\n
                 * These should always be unique. */
        char *class_name,
                /*!< group code = 2.\n
                 * C++ class name.\n
                 * Used to bind with software that defines object class behavior.\n
                 * These are always unique. */  
        char *app_name,
                /*!< group code = 3.\n
                 * Application name.\n
                 * Posted in Alert box when a class definition listed in this
                 * section is not currently loaded. */ 
        int proxy_cap_flag,
                /*!< group code = 90.\n
                 * Proxy capabilities flag.\n
                 * Bit coded value that indicates the capabilities of this object as a proxy.\n
                 * 0 = No operations allowed (0)\n
                 * 1 = Erase allowed (0x1)\n
                 * 2 = Transform allowed (0x2)\n
                 * 4 = Color change allowed (0x4)\n
                 * 8 = Layer change allowed (0x8)\n
                 * 16 = Linetype change allowed (0x10)\n
                 * 32 = Linetype scale change allowed (0x20)\n
                 * 64 = Visibility change allowed (0x40)\n
                 * 127 = All operations except cloning allowed (0x7F)\n
                 * 128 = Cloning allowed (0x80)\n
                 * 255 = All operations allowed (0xFF)\n
                 * 32768 = R13 format proxy (0x8000). */
        int was_a_proxy_flag,
                /*!< group code = 280.\n
                 * Was-a-proxy flag.\n
                 * Set to 1 if class was not loaded when this DXF file was
                 * created, and 0 otherwise. */
        int is_an_entity_flag
                /*!< group code = 281.\n
                 * Is-an-entity flag.\n
                 * Set to 1 if class was derived from the AcDbEntity class and
                 * can reside in the BLOCKS or ENTITIES section.\n
                 * If 0, instances may appear only in the OBJECTS section. */
)
{
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Entering dxf_class_write_lowlevel () function.\n",
                __FILE__, __LINE__);
#endif
        char *dxf_entity_name = strdup ("CLASS");
        fprintf (fp, "  0\n%s\n", dxf_entity_name);
        fprintf (fp, "  1\n%s\n", record_name);
        fprintf (fp, "  2\n%s\n", class_name);
        fprintf (fp, "  3\n%s\n", app_name);
        fprintf (fp, " 90\n%d\n", proxy_cap_flag);
        fprintf (fp, "280\n%d\n", was_a_proxy_flag);
        fprintf (fp, "281\n%d\n", is_an_entity_flag);
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Leaving dxf_class_write_lowlevel () function.\n",
                __FILE__, __LINE__);
#endif
        return (EXIT_SUCCESS);
}


/*!
 * \brief Write DXF output to a file for a class section.
 */
int
dxf_class_write
(
        FILE *fp,
                /*!< file pointer to output file (or device). */
        DxfClass dxf_class
                /*!< DXF class section. */
)
{
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Entering dxf_class_write () function.\n",
                __FILE__, __LINE__);
#endif
        char *dxf_entity_name = strdup ("CLASS");
        fprintf (fp, "  0\n%s\n", dxf_entity_name);
        fprintf (fp, "  1\n%s\n", dxf_class.record_name);
        fprintf (fp, "  2\n%s\n", dxf_class.class_name);
        fprintf (fp, "  3\n%s\n", dxf_class.app_name);
        fprintf (fp, " 90\n%d\n", dxf_class.proxy_cap_flag);
        fprintf (fp, "280\n%d\n", dxf_class.was_a_proxy_flag);
        fprintf (fp, "281\n%d\n", dxf_class.is_an_entity_flag);
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Leaving dxf_class_write () function.\n",
                __FILE__, __LINE__);
#endif
        return (EXIT_SUCCESS);
}


/*!
 * \brief Write DXF output to a file for an end of class section marker.
 */
int
dxf_class_write_endclass
(
        FILE *fp
)
{
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Entering dxf_class_write_endclass () function.\n", __FILE__, __LINE__);
#endif
        fprintf (fp, "  0\nENDSEC\n");
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Leaving dxf_class_write_endclass () function.\n", __FILE__, __LINE__);
#endif
        return (EXIT_SUCCESS);
}


/* EOF */
