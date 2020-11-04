/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-tk-lib
 * Created on: 9 июл. 2017 г.
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

#include <lsp-plug.in/tk/tk.h>
#include <private/tk/style/BuiltinStyle.h>

namespace lsp
{
    namespace tk
    {
        STYLE_INITIALIZER_BEGIN(Separator, Widget);

            prop::Orientation::init("orientation", style, O_VERTICAL);
            prop::Color::init("color", style, "#000000");
            prop::SizeRange::init("size", style, -1, -1);
            prop::Integer::init("thickness", style, 1);

        STYLE_INITIALIZER_END(Separator, "Separator");
        LSP_BUILTIN_STYLE_DEPRECATED(Separator);

        const w_class_t Separator::metadata = { "Separator", &Widget::metadata };

        Separator::Separator(Display *dpy):
            Widget(dpy),
            sOrientation(&sProperties),
            sColor(&sProperties),
            sSizeRange(&sProperties),
            sThickness(&sProperties)
        {
            pClass          = &metadata;
        }
        
        Separator::~Separator()
        {
            nFlags     |= FINALIZED;
        }

        status_t Separator::init()
        {
            status_t result = Widget::init();
            if (result != STATUS_OK)
                return result;

            sOrientation.bind("orientation", &sStyle);
            sColor.bind("color", &sStyle);
            sSizeRange.bind("size", &sStyle);
            sThickness.bind("thickness", &sStyle);

//            Style *sclass = style_class();
//            if (sclass != NULL)
//            {
//                sOrientation.init(sclass, O_VERTICAL);
//                sColor.init(sclass, "#000000");
//                sSizeRange.init(sclass, -1, -1);
//                sThickness.init(sclass, 1);
//            }

            return STATUS_OK;
        }

        void Separator::property_changed(Property *prop)
        {
            Widget::property_changed(prop);

            if (sOrientation.is(prop))
                query_resize();
            if (sColor.is(prop))
                query_draw();
            if (sSizeRange.is(prop))
                query_resize();
            if (sColor.is(prop))
                query_draw();
            if (sThickness.is(prop))
                query_resize();
        }

        void Separator::render(ws::ISurface *s, bool force)
        {
            float bright    = sBrightness.get();

            // Prepare palette
            lsp::Color color(sColor);
            color.scale_lightness(bright);

            // Draw self
            s->fill_rect(color, sSize.nLeft, sSize.nTop, sSize.nWidth, sSize.nHeight);
        }

        void Separator::size_request(ws::size_limit_t *r)
        {
            ssize_t min, max;
            float scaling   = lsp_max(0.0f, sScaling.get());
            size_t thick    = lsp_max(0, sThickness.get()) * scaling;

            sSizeRange.get(&min, &max);

            if (sOrientation.horizontal())
            {
                r->nMinWidth    = lsp_max(0, min);
                r->nMaxWidth    = (max >= 0) ? lsp_max(r->nMinWidth, max) : -1;
                r->nMinHeight   = thick;
                r->nMaxHeight   = thick;
            }
            else
            {
                r->nMinWidth    = thick;
                r->nMaxWidth    = thick;
                r->nMinHeight   = lsp_max(0, min);
                r->nMaxHeight   = (max >= 0) ? lsp_max(r->nMinHeight, max) : -1;
            };

            r->nPreWidth    = -1;
            r->nPreHeight   = -1;
        }
    
    } /* namespace tk */
} /* namespace lsp */
