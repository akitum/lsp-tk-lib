/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-tk-lib
 * Created on: 26 авг. 2020 г.
 *
 * lsp-tk-lib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * lsp-tk-lib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with lsp-tk-lib. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef LSP_PLUG_IN_TK_HELPERS_GRAPHICS_H_
#define LSP_PLUG_IN_TK_HELPERS_GRAPHICS_H_

#include <lsp-plug.in/tk/tk.h>

namespace lsp
{
    namespace tk
    {
        bool line2d_equation
        (
            float x1, float y1,
            float x2, float y2,
            float &a, float &b, float &c
        );

        bool line2d_delta_equation
        (
            float x1, float y,
            float dx, float dy,
            float &a, float &b, float &c
        );


        bool line2d_equation
        (
            float dx, float dy,
            float &a, float &b, float &c
        );

        bool line2d_intersection
        (
            float a1, float b1, float c1,
            float a2, float b2, float c2,
            float &x, float &y
        );

        float distance2d(float x1, float y1, float x2, float y2);

        float scalar_product2d(float x1, float y1, float x2, float y2);

        float vector_product2d(float x1, float y1, float x2, float y2);

        float get_angle_2d
        (
            float x0, float y0, // Coordinates of center
            float x, float y    // Coordinates of dot
        );

        bool clip_line2d_vec(
            float dx, float dy,                             // Line direction
            float lc, float rc, float tc, float bc,         // Corners from left, right, top, bottom
            float error,                                    // Allowed error
            float &cx1, float &cy1, float &cx2, float &cy2  // Results
        );

        bool clip_line2d_eq(
            float a, float b, float c,                      // Line equation
            float lc, float rc, float tc, float bc,         // Corners from left, right, top, bottom
            float error,                                    // Allowed error
            float &cx1, float &cy1, float &cx2, float &cy2  // Results
        );

        bool clip_line2d_coord(
            float x1, float x2, float y1, float y2,         // Coordinates of two points laying on line
            float lc, float rc, float tc, float bc,         // Corners from left, right, top, bottom
            float error,                                    // Allowed error
            float &cx1, float &cy1, float &cx2, float &cy2  // Results
        );

        void locate_line2d(
            float a, float b, float c,                      // Line equation
            float px, float py,                             // Point of the line
            float &ma, float &mb, float &mc                 // New equation
        );

        bool locate_line2d(
            float dx, float dy,                             // Line equation
            float px, float py,                             // Point of the line
            float &ma, float &mb, float &mc                 // New equation
        );

        bool clip2d(
            float x, float y,                               // Coordinates of point
            float lc, float rc, float tc, float bc          // Corners: left, right, top, bottom
        );
    }
}


#endif /* LSP_PLUG_IN_TK_HELPERS_GRAPHICS_H_ */
