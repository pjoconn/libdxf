/*!
 * \file hatch.c
 * \author Copyright (C) 2008, 2010 by Bert Timmerman <bert.timmerman@xs4all.nl>.
 * \brief DXF hatch entity (\c HATCH).
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

#include "global.h"
#include "hatch.h"

/*!
 * \brief Write DXF output to a file for a hatch entity.
 *
 * This entity requires AutoCAD version R14 or higher.\n
 * A hatch entity is a rather complex and intricate object to write to a DXF
 * file, nevertheless, we'll have to try.\n
 * First a header and common values for the hatch is written to file, then
 * follows the hatch boundary data and the hatch pattern data.\n\n
 * dxf_write_hatch (): write hatch header and common values.\n
 * Now follows the hatch boundary path data and the hatch pattern data:\n
 * <ul>
 *   <li>dxf_write_hatch_boundaries (): write boundary data which consists of
 *   one or more of either:\n
 *   <ul>
       <li>dxf_write_hatch_boundary_path_edges (): write the edges of one or
 *     more boundary paths, edges consisting of either:\n
 *     <ul>
 *       <li>boundary_path_edge_line (): write a line entity.\n
 *       <li>boundary_path_edge_arc (): write an arc entity.\n
 *       <li>boundary_path_edge_ellipse (): write an ellipse entity.\n
 *       <li>boundary_path_edge_spline (): write a spline entity.\n
 *     </ul>
 *     The edge entities making a boundary path should form a closed loop, for
 *     else the hatch pattern will "escape" from this area through the "gaps"
 *     in the boundary path.\n
 *     In case a very dense hatch pattern is chosen, it may take very long for
 *     AutoCAD (or any other CAD application using the DXF file) to load the
 *     hatch entity, for it will try to apply the hatch pattern to it's entire
 *     universe ;-(\n
 *     It is for this reason that applying a very dense hatch pattern is
 *     considered a Bad Thing (TM); better use the SOLID hatch pattern to
 *     obtain a similar result.\n
 *     <li>dxf_write_hatch_boundary_path_polyline (): write a polyline entity
 *     and vertices.\n
 *     <ul>
 *       <li>dxf_write_hatch_boundary_polyline_vertex (): write a vertex\n
 *     </ul>
 *     The polyline making a boundary path should form a closed loop, for else
 *     the hatch pattern will "escape" from this area through the "gaps" in the
 *     boundary path.\n
 *     In case a very dense hatch pattern is chosen, it may take very long for
 *     AutoCAD (or any other CAD application using the DXF file) to load the
 *     hatch entity, for it will try to apply the hatch pattern to it's entire
 *     universe ;-(\n
 *     It is for this reason that applying a very dense hatch pattern is
 *     considered a Bad Thing (TM); better use the SOLID hatch pattern to
 *     obtain a similar result.\n
 *   </ul>
 *     <li>dxf_write_hatch_pattern_data (): write one or more pattern definition
 *     lines which are either continous or dashed.\n
 *   <ul>
 *     <li>dxf_write_hatch_pattern_def_line_dashes\n
 *   </ul>
 * </ul>
 */
int
dxf_write_hatch
(
        FILE *fp,
                /*!< file pointer to output file (or device). */
        char *pattern_name,
                /*!< group code = 2. */
        int id_code,
                /*!< group code = 5. */
        char *linetype,
                /*!< group code = 6\n
                 * optional, defaults to BYLAYER. */
        char *layer,
                /*!< group code = 8. */
        double x0,
                /*!< group code = 10\n
                 * base point. */
        double y0,
                /*!< group code = 20\n
                 * base point. */
        double z0,
                /*!< group code = 30\n
                 * base point. */
        double extr_x0,
                /*!< group code = 210\n
                 * extrusion direction\n
                 * optional, if ommited defaults to 0.0. */
        double extr_y0,
                /*!< group code = 220\n
                 * extrusion direction\n
                 * optional, if ommited defaults to 0.0. */
        double extr_z0,
                /*!< group code = 230\n
                 * extrusion direction\n
                 * optional, if ommited defaults to 1.0. */
        double thickness,
                /*!< group code = 39\n
                 * optional, defaults to 0.0. */
        double pattern_scale,
                /*!< group code 41\n
                 * pattern fill only. */
        double pixel_size,
                /*!< group code 47. */
        double pattern_angle,
                /*!< group code 52\n
                 *  pattern fill only. */
        int color,
                /*!< group code = 62\n
                 * optional, defaults to BYLAYER. */
        int paperspace,
                /*!< group code = 67\n
                 * optional, defaults to 0 (modelspace). */
        int solid_fill,
                /*!< group code = 70\n
                 * 0 = pattern fill\n
                 * 1 = solid fill. */
        int associative,
                /*!< group code = 71\n
                 * 0 = non-associative\n
                 * 1 = associative. */
        int style,
                /*!< group code = 75\n
                 * 0 = hatch "odd parity" area (Normal style)\n
                 * 1 = hatch outermost area only (Outer style)\n
                 * 2 = hatch through entire area (Ignore style). */
        int pattern_style,
                /*!< group code = 76\n
                 * 0 = user defined\n
                 * 1 = predefined\n
                 * 2 = custom. */
        int pattern_double,
                /*!< group code = 77\n
                 * pattern fill only\n
                 * 0 = not double\n
                 * 1 = double. */
        int pattern_def_lines,
                /*!< group code = 78\n
                 * number of pattern definition lines. */
        int pattern_boundary_paths,
                /*!< group code = 91\n
                 * number of boundary paths (loops). */
        int seed_points,
                /*!< group code = 98\n
                 * number of seed points. */
                double *seed_x0,
                        /*!< group code = 10\n
                         * seed point X-value. */
                double *seed_y0
                        /*!< group code = 20\n
                         * seed point Y-value. */
)
{
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Entering dxf_write_hatch () function.\n", __FILE__, __LINE__);
#endif
        char *dxf_entity_name = strdup ("HATCH");
        int i;
        if (strcmp (layer, "") == 0)
        {
                fprintf (stderr, "Warning: empty layer string for the %s entity with id-code: %x\n", dxf_entity_name, id_code);
                fprintf (stderr, "    %s entity is relocated to layer 0", dxf_entity_name);
                layer = strdup (DXF_DEFAULT_LAYER);
        }
        fprintf (fp, "  0\n%s\n", dxf_entity_name);
        fprintf (fp, "100\nAcDbHatch\n");
        fprintf (fp, "  2\n%s\n", pattern_name);
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
        fprintf (fp, "210\n%f\n", extr_x0);
        fprintf (fp, "220\n%f\n", extr_y0);
        fprintf (fp, "230\n%f\n", extr_z0);
        if (thickness != 0.0)
        {
                fprintf (fp, " 39\n%f\n", thickness);
        }
        if (!solid_fill)
        {
                fprintf (fp, " 42\n%f\n", pattern_scale);
        }
        fprintf (fp, " 47\n%f\n", pixel_size);
        if (!solid_fill)
        {
                fprintf (fp, " 52\n%f\n", pattern_angle);
        }
        if (color != DXF_COLOR_BYLAYER)
        {
                fprintf (fp, " 62\n%d\n", color);
        }
        if (paperspace == DXF_PAPERSPACE)
        {
                fprintf (fp, " 67\n%d\n", DXF_PAPERSPACE);
        }
        fprintf (fp, " 70\n%d\n", solid_fill);
        fprintf (fp, " 71\n%d\n", associative);
        fprintf (fp, " 75\n%d\n", style);
        if (!solid_fill)
        {
                fprintf (fp, " 77\n%d\n", pattern_double);
        }
        fprintf (fp, " 78\n%d\n", pattern_def_lines);
        fprintf (fp, " 98\n%d\n", seed_points);
        if (!seed_points)
        {
                for (i = 0; i < seed_points; i++)
                {
                        fprintf (fp, " 10\n%f\n", seed_x0[i]);
                        fprintf (fp, " 20\n%f\n", seed_y0[i]);
                }
        }
        fprintf (fp, " 91\n%d\n", pattern_boundary_paths);
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Leaving dxf_write_hatch () function.\n", __FILE__, __LINE__);
#endif
        return (EXIT_SUCCESS);
}

/*!
\brief Write DXF output to a file for hatch boundary entities.

Requires AutoCAD version R14 or higher.
*/
int
dxf_write_hatch_boundaries
(
        FILE *fp,
                /*!< file pointer to output file (or device). */
        int hatch_boundary_paths,
                /*!< group code = 91\n
                 * number of boundary paths (loops). */
        int hatch_boundary_path_type_flag,
                /*!< group code = 92\n
                 * boundary path type flag\n
                 * bit coded:\n
                 * 0 = default\n
                 * 1 = external\n
                 * 2 = polyline\n
                 * 4 = derived\n
                 * 8 = textbox\n
                 * 16 = outermost. */
                int hatch_boundary_path_edges,
                        /*!< group code = 93\n
                         * number of edges in this boundary path\n
                         * (only if boundary is not a polyline). */
                int hatch_boundary_path_edge_type,
                        /*!< group code = 72\n
                         * (only if boundary is not a polyline).\n */
                /*! <hr>
                 * <ul>
                 * <li>1 = line\n
                 * </ul> */
                        double hatch_boundary_path_edge_line_x0,
                                /*!< group code = 10. */
                        double hatch_boundary_path_edge_line_y0,
                                /*!< group code = 20. */
                        double hatch_boundary_path_edge_line_x1,
                                /*!< group code = 11. */
                        double hatch_boundary_path_edge_line_y1,
                                /*!< group code = 21.\n */

                /*! <hr>
                 * <ul>
                 * <li>2 = circular arc\n
                 * </ul> */
                        double hatch_boundary_path_edge_arc_x0,
                                /*!< group code = 10\n
                                 * X-value of center point coordinate. */
                        double hatch_boundary_path_edge_arc_y0,
                                /*!< group code = 20\n
                                 * Y-value of center point coordinate. */
                        double hatch_boundary_path_edge_arc_radius,
                                /*!< group code = 40. */
                        double hatch_boundary_path_edge_arc_start_angle,
                                /*!< group code = 50. */
                        double hatch_boundary_path_edge_arc_end_angle,
                                /*!< group code = 51. */
                        int hatch_boundary_path_edge_arc_is_ccw,
                                /*!< group code = 73\n
                                 * arc is counterclockwise flag.\n */

                /*! <hr>
                 * <ul>
                 * <li>3 = elliptic arc\n
                 * </ul> */
                        double hatch_boundary_path_edge_ellipse_x0,
                                /*!< group code = 10\n
                                 * X-value of center point coordinate. */
                        double hatch_boundary_path_edge_ellipse_y0,
                                /*!< group code = 20\n
                                 * Y-value of center point coordinate. */
                        double hatch_boundary_path_edge_ellipse_x1,
                                /*!< group code = 11\n
                                 * X-value of end point coordinate of major
                                 * axis (relative to center point. */
                        double hatch_boundary_path_edge_ellipse_y1,
                                /*!< group code = 21\n
                                 * Y-value of end point coordinate of major
                                 * axis (relative to center point. */
                        double boundary_path_edge_ellipse_minor_axis,
                                /*!< group code = 40\n
                                 * length of minor axis (percentage of major
                                 * axis length). */
                        double hatch_boundary_path_edge_ellipse_start_angle,
                                /*!< group code = 50. */
                        double hatch_boundary_path_edge_ellipse_end_angle,
                                /*!< group code = 51. */
                        int hatch_boundary_path_edge_ellipse_is_ccw,
                                /*!< group code = 73\n
                                 * ellipse is counterclockwise flag.\n */

                /*! <hr>
                 * <ul>
                 * <li>4 = spline
                 * </ul> */
                        int hatch_boundary_path_edge_spline_degree,
                                /*!< group code = 94. */
                        int hatch_boundary_path_edge_spline_rational,
                                /*!< group code = 73. */
                        int hatch_boundary_path_edge_spline_periodic,
                                /*!< group code = 74. */
                        int hatch_boundary_path_edge_spline_knots,
                                /*!< group code = 95\n
                                 * number of knots. */
                        int hatch_boundary_path_edge_spline_control_points,
                                /*!< group code = 96\n
                                 * number of control points. */
                                int *hatch_boundary_path_edge_spline_knot_value,
                                        /*!< group code = 40. */
                                double *hatch_boundary_path_edge_spline_cp_x0,
                                        /*!< group code = 10. */
                                double *hatch_boundary_path_edge_spline_cp_y0,
                                        /*!< group code = 20. */
                                double *hatch_boundary_path_edge_spline_cp_weight,
                                        /*!< group code = 42\n
                                         * optional, defaults to 1.0. */
                int hatch_boundary_path_polyline_has_bulge,
                        /*!< group code = 72\n
                         * polyline boundary data group only. */
                int hatch_boundary_path_polyline_is_closed,
                        /*!< group code = 73\n
                         * polyline boundary data group only. */
                int hatch_boundary_path_polyline_vertices,
                        /*!< group code = 93\n
                         * number of polyline vertices to follow. */
                        double *hatch_boundary_path_polyline_x0,
                                /*!< group code = 10. */
                        double *hatch_boundary_path_polyline_y0,
                                /*!< group code = 20. */
                        double *hatch_boundary_path_polyline_bulge,
                                /*!< group code = 42. */
        int hatch_boundary_objects,
                /*!< group code = 97\n
                 * number of source boundary objects. */
        char *hatch_boundary_objects_ref
                /*!< group code = 330\n
                 * reference to source boundary objects (multiple entries). */
)
{
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Entering dxf_write_hatch_boundary () function.\n", __FILE__, __LINE__);
#endif
        int i;
        int j;
        for (i = 0; i < hatch_boundary_paths; i++)
        {
                if (hatch_boundary_path_type_flag == 0)
                {
                        /* default type boundary */
                        for (j = 0; j < hatch_boundary_path_edges; j++)
                        {
                                switch (hatch_boundary_path_edge_type)
                                case 1: /* line type */
                                case 2: /* circular arc type */
                                case 3: /* elliptic arc type */
                                case 4: /* spline type */
                                default:
                                        fprintf (stderr, "Error: unsupported boundary path edge type encountered in dxf_write_hatch_boundary ().\n");
                                        return (EXIT_FAILURE);
                        }
                }
                else if (hatch_boundary_path_type_flag == 2)
                {
                        /* a polyline boundary */
                        dxf_write_hatch_boundary_path_polyline
                        (
                                fp,
                                hatch_boundary_path_polyline_has_bulge,
                                hatch_boundary_path_polyline_is_closed,
                                hatch_boundary_path_polyline_vertices,
                                hatch_boundary_path_polyline_x0,
                                hatch_boundary_path_polyline_y0,
                                hatch_boundary_path_polyline_bulge
                        );
                }
                else
                {
                        fprintf (stderr, "Error: unsupported boundary path type encountered in dxf_write_hatch_boundary ().\n");
                        return (EXIT_FAILURE);
                }
        }
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Leaving dxf_write_hatch_boundary () function.\n", __FILE__, __LINE__);
#endif
		return (EXIT_SUCCESS);
}

/*!
\brief Write DXF output to a file for a hatch boundary polyline vertex.
*/
int
dxf_write_hatch_boundary_polyline_vertex
(
        FILE *fp,
                /*!< file pointer to output file (or device). */
        double x0,
                /*!< group code = 10\n
                 * X-value of vertex point. */
        double y0,
                /*!< group code = 20\n
                 * Y-value of vertex point. */
        double bulge
                /*!< group code 42\n
                 * bulge of polyline vertex\n
                 * optional, defaults to 0.0. */
)
{
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Entering dxf_write_hatch_boundary_polyline_vertex () function.\n", __FILE__, __LINE__);
#endif
        fprintf (fp, " 10\n%f\n", x0);
        fprintf (fp, " 20\n%f\n", y0);
        if (bulge != 0.0) fprintf (fp, " 42\n%f\n", bulge);
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Leaving dxf_write_hatch_boundary_polyline_vertex () function.\n", __FILE__, __LINE__);
#endif
        return (EXIT_SUCCESS);
}

/*!
\brief Write DXF output to a file for a hatch boundary path polyline.
*/
int
dxf_write_hatch_boundary_path_polyline
(
        FILE *fp,
                /*!< file pointer to output file (or device). */
        int has_bulge,
                /*!< group code = 72\n
                 * polyline boundary data group only. */
        int is_closed,
                /*!< group code = 73\n
                 * polyline boundary data group only. */
        int vertices,
                /*!< group code = 93\n
                 * number of polyline vertices to follow. */
        double *x0,
                /*!< group code = 10\n
                 * pointer to array of [vertices] X-values. */
        double *y0,
                /*!< group code = 20\n
                 * pointer to array of [vertices] Y-values. */
        double *bulge
                /*!< group code = 42\n
                 * pointer to array of [vertices] bulge values. */
)
{
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Entering dxf_write_hatch_boundary_path_polyline () function.\n", __FILE__, __LINE__);
#endif
        int i;
        fprintf (fp, " 72\n%d\n", has_bulge);
        fprintf (fp, " 73\n%d\n", is_closed);
        fprintf (fp, " 93\n%d\n", vertices);
        /* draw hatch boundary, write (n_coords) XY-coordinate pairs */
        for (i = 0; i < vertices; i++)
        {
                if (!has_bulge) bulge[i] = 0.0;
                dxf_write_hatch_boundary_polyline_vertex
                (
                        fp,
                        x0[i],
                        y0[i],
                        bulge[i]
                );
                /* close polyline with first coordinate XY-pair */
                if (is_closed)
                {
                        if (!has_bulge) bulge[0] = 0.0;
                        dxf_write_hatch_boundary_polyline_vertex
                        (
                                fp,
                                x0[0],
                                y0[0],
                                bulge[0]
                        );
                }
        }
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Leaving dxf_write_hatch_boundary_path_polyline () function.\n", __FILE__, __LINE__);
#endif
        return (EXIT_SUCCESS);
}

/*!
\brief Write DXF output to a file for a hatch pattern definition line dash items.
*/
int
dxf_write_hatch_pattern_def_line_dashes
(
        FILE *fp,
                /*!< file pointer to output file (or device). */
        int dash_items,
                /*!< group code = 79\n
                 * array of number of hatch pattern definition line dash
                 * items. */
        double *dash_length
                /*!< group code = 49\n
                 * array of dash lengths for an array of hatch pattern
                 * definition lines. */
)
{
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Entering dxf_write_hatch_pattern_def_line_dashes () function.\n", __FILE__, __LINE__);
#endif
        if (dash_items != 0)
        {
                int i;
                for (i = 0; i < dash_items; i++)
                {
                        fprintf (fp, " 49\n%f\n", dash_length[i]);
                }
        }
        else
        {
                fprintf (stderr, "Warning: no definition line dash items found in dxf_write_hatch_pattern_def_line_dashes () function.\n");
        }
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Leaving dxf_write_hatch_pattern_def_line_dashes () function.\n", __FILE__, __LINE__);
#endif
        return (EXIT_SUCCESS);
}

/*!
\brief Write DXF output to a file for a hatch pattern data.
*/
int
dxf_write_hatch_pattern_data
(
        FILE *fp,
                /*!< file pointer to output file (or device). */
        int def_lines,
                /*!< group code = 78\n
                 * number of hatch pattern definition lines. */
        double *def_line_angle,
                /*!< group code = 53\n
                 * array of hatch pattern definition line angles. */
        double *def_line_x0,
                /*!< group code = 43\n
                 * array of hatch pattern line base point X-values. */
        double *def_line_y0,
                /*!< group code = 44\n
                 * array of hatch pattern line base point Y-values. */
        double *def_line_x1,
                /*!< group code = 45\n
                 * array of hatch pattern line offset point X-values. */
        double *def_line_y1,
                /*!< group code = 46\n
                 * array of hatch pattern line offset point Y-values. */
        int *def_line_dash_items,
                /*!< group code = 79\n
                 * array of number of hatch pattern definition line dash
                 * items. */
        double **def_line_dash_length
                /*!< group code = 49\n
                 * array of dash lengths for an array of hatch pattern
                 * definition lines. */
)
{
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Entering dxf_write_hatch_pattern_data () function.\n", __FILE__, __LINE__);
#endif
        int i;
        int j;
        if (def_lines != 0)
        {
                /* draw hatch pattern definition lines */
                for (i = 0; i < def_lines; i++)
                {
                        fprintf (fp, " 53\n%f\n", def_line_angle[i]);
                        fprintf (fp, " 43\n%f\n", def_line_x0[i]);
                        fprintf (fp, " 44\n%f\n", def_line_y0[i]);
                        fprintf (fp, " 45\n%f\n", def_line_x1[i]);
                        fprintf (fp, " 46\n%f\n", def_line_y1[i]);
                        fprintf (fp, " 79\n%d\n", def_line_dash_items[i]);
                        if (!def_line_dash_items)
                        {
                                for (j = 0; j < *def_line_dash_items; j++)
                                {
                                        dxf_write_hatch_pattern_def_line_dashes
                                        (
                                                fp,
                                                *def_line_dash_items,
                                                *def_line_dash_length
                                        );
                                }
                        }
                }
        }
        else
        {
                fprintf (stderr, "Warning: no definition lines found in dxf_write_hatch_pattern_data () function.\n");
        }
#if DEBUG
        fprintf (stderr, "[File: %s: line: %d] Leaving dxf_write_hatch_pattern_data () function.\n", __FILE__, __LINE__);
#endif
        return (EXIT_SUCCESS);
}

/* EOF */
