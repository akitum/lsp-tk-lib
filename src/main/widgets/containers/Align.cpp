/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-tk-lib
 * Created on: 17 июл. 2017 г.
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
#include <lsp-plug.in/common/debug.h>
#include <private/tk/style/BuiltinStyle.h>

namespace lsp
{
    namespace tk
    {
        STYLE_INITIALIZER_BEGIN(Align, WidgetContainer);

            prop::Layout::init("layout", style, 0.0f, 0.0f, 0.0f, 0.0f);
            prop::SizeConstraints::init("size.constraints", style);

        STYLE_INITIALIZER_END(Align, "Align");
        LSP_BUILTIN_STYLE_DEPRECATED(Align);

        const w_class_t Align::metadata         = { "Align", &WidgetContainer::metadata };

        Align::Align(Display *dpy):
            WidgetContainer(dpy),
            sLayout(&sProperties),
            sSizeConstraints(&sProperties)
        {
            pWidget         = NULL;

            pClass          = &metadata;
        }

        Align::~Align()
        {
            nFlags     |= FINALIZED;
            do_destroy();
        }

        status_t Align::init()
        {
            status_t result = WidgetContainer::init();
            if (result != STATUS_OK)
                return result;

            sLayout.bind("layout", &sStyle);
            sSizeConstraints.bind("size.constraints", &sStyle);

//            Style *sclass = style_class();
//            if (sclass != NULL)
//            {
//                sLayout.init(sclass, 0.0f, 0.0f, 0.0f, 0.0f);
//                sSizeConstraints.init(sclass);
//            }

            return STATUS_OK;
        }

        void Align::destroy()
        {
            nFlags     |= FINALIZED;
            do_destroy();
            WidgetContainer::destroy();
        }

        void Align::do_destroy()
        {
            if (pWidget != NULL)
            {
                unlink_widget(pWidget);
                pWidget = NULL;
            }
        }

        Widget *Align::find_widget(ssize_t x, ssize_t y)
        {
            if ((pWidget == NULL) || (!pWidget->is_visible_child_of(this)))
                return NULL;

            return (pWidget->inside(x, y)) ? pWidget : NULL;
        }

        void Align::property_changed(Property *prop)
        {
            WidgetContainer::property_changed(prop);
            if (sLayout.is(prop))
                query_resize();
            if (sSizeConstraints.is(prop))
                query_resize();
        }

        void Align::render(ws::ISurface *s, const ws::rectangle_t *area, bool force)
        {
            if (nFlags & REDRAW_SURFACE)
                force = true;

            // Initialize palette
            lsp::Color bg_color(sBgColor);

            // Draw background if child is invisible or not present
            if ((pWidget == NULL) || (!pWidget->visibility()->get()))
            {
                s->clip_begin(area);
                    s->fill_rect(bg_color, &sSize);
                s->clip_end();
                return;
            }

            if ((force) || (pWidget->redraw_pending()))
            {
                // Draw the child only if it is visible in the area
                ws::rectangle_t xr;
                pWidget->get_rectangle(&xr);
                if (Size::intersection(&xr, area))
                    pWidget->render(s, &xr, force);

                pWidget->commit_redraw();
            }

            if (force)
            {
                ws::rectangle_t cr;

                pWidget->get_rectangle(&cr);
                if (Size::overlap(area, &sSize))
                {
                    s->clip_begin(area);
                    {
                        bg_color.copy(pWidget->bg_color()->color());
                        s->fill_frame(bg_color, &sSize, &cr);
                    }
                    s->clip_end();
                }
            }
        }

        status_t Align::add(Widget *widget)
        {
            if ((widget == NULL) || (widget == this))
                return STATUS_BAD_ARGUMENTS;
            if (pWidget != NULL)
                return STATUS_ALREADY_EXISTS;

            widget->set_parent(this);
            pWidget = widget;
            query_resize();
            return STATUS_OK;
        }

        status_t Align::remove(Widget *widget)
        {
            if (pWidget != widget)
                return STATUS_NOT_FOUND;

            unlink_widget(pWidget);
            pWidget  = NULL;

            return STATUS_OK;
        }

        void Align::size_request(ws::size_limit_t *r)
        {
            float scaling   = lsp_max(0.0f, sScaling.get());

            if ((pWidget == NULL) || (!pWidget->visibility()->get()))
            {
                r->nMinWidth    = -1;
                r->nMinHeight   = -1;
                r->nMaxWidth    = -1;
                r->nMaxHeight   = -1;

            }
            else
            {
                pWidget->get_padded_size_limits(r);
                r->nMaxWidth    = -1;
                r->nMaxHeight   = -1;
            }

            r->nPreWidth    = -1;
            r->nPreHeight   = -1;

            sSizeConstraints.apply(r, scaling);
        }

        void Align::realize(const ws::rectangle_t *r)
        {
//            lsp_trace("width=%d, height=%d", int(r->nWidth), int(r->nHeight));
            WidgetContainer::realize(r);

            if ((pWidget == NULL) || (!pWidget->visibility()->get()))
                return;

            // Realize child widget
            ws::rectangle_t xr;
            ws::size_limit_t sr;

            pWidget->get_padded_size_limits(&sr);
            sLayout.apply(&xr, r, &sr);
            pWidget->padding()->enter(&xr, pWidget->scaling()->get());
            pWidget->realize_widget(&xr);
        }
    } /* namespace tk */
} /* namespace lsp */
