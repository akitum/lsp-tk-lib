/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-tk-lib
 * Created on: 10 нояб. 2020 г.
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
        namespace style
        {
            LSP_TK_STYLE_IMPL_BEGIN(CheckBox, Widget)
                // Bind
                sConstraints.bind("size.constraints", this);
                sBorderSize.bind("border.size", this);
                sBorderRadius.bind("border.radius", this);
                sBorderGapSize.bind("border.gap.size", this);
                sCheckRadius.bind("check.radius", this);
                sCheckGapSize.bind("check.gap.size", this);
                sCheckMinSize.bind("check.min.size", this);
                sChecked.bind("checked", this);
                sColor.bind("color", this);
                sHoverColor.bind("hover.color", this);
                sFillColor.bind("fill.color", this);
                sFillHoverColor.bind("fill.hover.color", this);
                sBorderColor.bind("border.color", this);
                sBorderHoverColor.bind("border.hover.color", this);
                sBorderGapColor.bind("border.gap.color", this);
                sBorderGapHoverColor.bind("border.gap.hover.color", this);
                // Configure
                sConstraints.set_all(16);
                sBorderSize.set(1);
                sBorderRadius.set(4);
                sBorderGapSize.set(1);
                sCheckGapSize.set(2);
                sCheckMinSize.set(4);
                sChecked.set(false);
                sColor.set("#00ccff");
                sHoverColor.set("#ff8800");
                sFillColor.set("#ffffff");
                sFillHoverColor.set("#ffeeee");
                sBorderColor.set("#000000");
                sBorderHoverColor.set("#000000");
                sBorderGapColor.set("#cccccc");
                sBorderGapHoverColor.set("#cccccc");
                // Commit
                sConstraints.override();
            LSP_TK_STYLE_IMPL_END
            LSP_TK_BUILTIN_STYLE(CheckBox, "CheckBox", "root");
        }

        const w_class_t CheckBox::metadata      = { "CheckBox", &Widget::metadata };

        CheckBox::CheckBox(Display *dpy):
            Widget(dpy),
            sConstraints(&sProperties),
            sBorderSize(&sProperties),
            sBorderRadius(&sProperties),
            sBorderGapSize(&sProperties),
            sCheckRadius(&sProperties),
            sCheckGapSize(&sProperties),
            sCheckMinSize(&sProperties),
            sChecked(&sProperties),
            sColor(&sProperties),
            sHoverColor(&sProperties),
            sFillColor(&sProperties),
            sFillHoverColor(&sProperties),
            sBorderColor(&sProperties),
            sBorderHoverColor(&sProperties),
            sBorderGapColor(&sProperties),
            sBorderGapHoverColor(&sProperties)
        {
            nRadius         = 0;
            nState          = 0;
            nBMask          = 0;

            sArea.nLeft     = 0;
            sArea.nTop      = 0;
            sArea.nWidth    = 0;
            sArea.nHeight   = 0;

            pClass          = &metadata;
        }

        CheckBox::~CheckBox()
        {
        }

        status_t CheckBox::init()
        {
            status_t res    = Widget::init();
            if (res != STATUS_OK)
                return res;

            // Bind properties
            sConstraints.bind("size.constraints", &sStyle);
            sBorderSize.bind("border.size", &sStyle);
            sBorderRadius.bind("border.radius", &sStyle);
            sBorderGapSize.bind("border.gap.size", &sStyle);
            sCheckRadius.bind("check.radius", &sStyle);
            sCheckGapSize.bind("check.gap.size", &sStyle);
            sCheckMinSize.bind("check.min.size", &sStyle);
            sChecked.bind("checked", &sStyle);
            sColor.bind("color", &sStyle);
            sHoverColor.bind("hover.color", &sStyle);
            sFillColor.bind("fill.color", &sStyle);
            sFillHoverColor.bind("fill.hover.color", &sStyle);
            sBorderColor.bind("border.color", &sStyle);
            sBorderHoverColor.bind("border.hover.color", &sStyle);
            sBorderGapColor.bind("border.gap.color", &sStyle);
            sBorderGapHoverColor.bind("border.gap.hover.color", &sStyle);

            // Additional slots
            handler_id_t id = 0;
            id = sSlots.add(SLOT_SUBMIT, slot_on_submit, self());

            return (id >= 0) ? STATUS_OK : -id;
        }

        void CheckBox::property_changed(Property *prop)
        {
            Widget::property_changed(prop);

            if (sConstraints.is(prop))
                query_resize();
            if (sBorderSize.is(prop))
                query_resize();
            if (sBorderRadius.is(prop))
                query_resize();
            if (sBorderGapSize.is(prop))
                query_resize();
            if (sCheckRadius.is(prop))
                query_resize();
            if (sCheckGapSize.is(prop))
                query_resize();
            if (sCheckMinSize.is(prop))
                query_resize();
            if (sChecked.is(prop))
            {
                nState  = lsp_setflag(nState, XF_CHECKED, sChecked.get());
                query_draw();
            }
            if (sColor.is(prop))
                query_draw();
            if (sHoverColor.is(prop))
                query_draw();
            if (sFillColor.is(prop))
                query_draw();
            if (sFillHoverColor.is(prop))
                query_draw();
            if (sBorderColor.is(prop))
                query_draw();
            if (sBorderHoverColor.is(prop))
                query_draw();
            if (sBorderGapColor.is(prop))
                query_draw();
            if (sBorderGapHoverColor.is(prop))
                query_draw();
        }

        void CheckBox::size_request(ws::size_limit_t *r)
        {
            float scaling       = sScaling.get();
            ssize_t border      = (sBorderSize.get() > 0) ? lsp_max(1.0f, sBorderSize.get() * scaling) : 0;
            ssize_t bgap        = (sBorderGapSize.get() > 0) ? lsp_max(1.0f, sBorderGapSize.get() * scaling) : 0;
            ssize_t ckgap       = (sCheckGapSize.get() > 0) ? lsp_max(1.0f, sCheckGapSize.get() * scaling) : 0;
            ssize_t brad        = (sBorderRadius.get() > 0) ? lsp_max(1.0f, sBorderRadius.get() * scaling) : 0;
            ssize_t irad        = (sCheckRadius.get() > 0) ? lsp_max(1.0f, sCheckRadius.get() * scaling) : 0;
            ssize_t ckmin       = lsp_max(1.0f, sCheckMinSize.get() * scaling);

            // Estimate the size of check box
            border             += lsp_max(ckgap, bgap);
            irad                = lsp_max(irad, brad - border);
            ckmin               = lsp_max(ckmin, irad * 2);

            // Return the size
            r->nMinWidth        = ckmin + border;
            r->nMinHeight       = r->nMinWidth;
            r->nMaxWidth        = -1;
            r->nMaxHeight       = -1;

            // Apply size constraints
            sConstraints.apply(r, scaling);
        }

        void CheckBox::realize(const ws::rectangle_t *r)
        {
            // Call parent widget for realize
            Widget::realize(r);

            float scaling       = sScaling.get();
            nRadius             = (sBorderRadius.get() > 0) ? lsp_max(1.0f, sBorderRadius.get() * scaling) : 0;

            sArea.nWidth        = lsp_min(r->nWidth, r->nHeight);
            sArea.nHeight       = sArea.nWidth;
            sArea.nLeft         = r->nLeft  + (r->nWidth  - sArea.nWidth ) / 2;
            sArea.nTop          = r->nTop   + (r->nHeight - sArea.nHeight) / 2;
        }

        void CheckBox::draw(ws::ISurface *s)
        {
            lsp::Color c;

            float scaling       = sScaling.get();
            float bright        = sBrightness.get();
            ssize_t border      = (sBorderSize.get() > 0) ? lsp_max(1.0f, sBorderSize.get() * scaling) : 0;
            ssize_t bgap        = (sBorderGapSize.get() > 0) ? lsp_max(1.0f, sBorderGapSize.get() * scaling) : 0;
            ssize_t irad        = (sCheckRadius.get() > 0) ? lsp_max(1.0f, sCheckRadius.get() * scaling) : 0;
            ssize_t ckgap       = (sCheckGapSize.get() > 0) ? lsp_max(1.0f, sCheckGapSize.get() * scaling) : 0;

            ssize_t brad        = nRadius;
            size_t state        = nState;
            ws::rectangle_t xr  = sArea;

            ckgap               = lsp_max(ckgap, bgap);
            xr.nLeft           -= sSize.nLeft;
            xr.nTop            -= sSize.nTop;

            // Draw background
            bool aa     = s->set_antialiasing(false);
            get_actual_bg_color(c);
            s->fill_rect(c, 0, 0, sSize.nWidth, sSize.nHeight);
            s->set_antialiasing(true);

            // Draw border
            if (border > 0)
            {
                c.copy((state & XF_HOVER) ? sBorderHoverColor : sBorderColor);
                c.scale_lch_luminance(bright);
                s->fill_round_rect(c, SURFMASK_ALL_CORNER, brad, &xr);

                xr.nLeft           += border;
                xr.nTop            += border;
                xr.nWidth          -= border * 2;
                xr.nHeight         -= border * 2;
                brad                = lsp_max(0, brad - border);
            }

            // Draw border gap
            ws::rectangle_t fr  = xr;
            ssize_t frad        = brad;
            if (bgap > 0)
            {
                c.copy((state & XF_HOVER) ? sBorderGapHoverColor : sBorderGapColor);
                c.scale_lch_luminance(bright);
                s->fill_round_rect(c, SURFMASK_ALL_CORNER, frad, &fr);

                fr.nLeft           += bgap;
                fr.nTop            += bgap;
                fr.nWidth          -= bgap * 2;
                fr.nHeight         -= bgap * 2;
                frad                = lsp_max(0, frad - bgap);
            }

            // Draw fill
            c.copy((state & XF_HOVER) ? sFillHoverColor : sFillColor);
            c.scale_lch_luminance(bright);
            s->fill_round_rect(c, SURFMASK_ALL_CORNER, frad, &fr);

            // Draw check
            if (state & XF_CHECKED)
            {
                xr.nLeft           += ckgap;
                xr.nTop            += ckgap;
                xr.nWidth          -= ckgap * 2;
                xr.nHeight         -= ckgap * 2;
                brad                = lsp_max(irad, brad - ckgap);

                c.copy((state & XF_HOVER) ? sHoverColor : sColor);
                c.scale_lch_luminance(bright);
                s->fill_round_rect(c, SURFMASK_ALL_CORNER, brad, &xr);
            }

            // Restore antialiasing
            s->set_antialiasing(aa);
        }

        status_t CheckBox::on_mouse_down(const ws::event_t *e)
        {
            if (nState & XF_OUT)
                return STATUS_OK;

            if (nBMask == 0)
            {
                bool inside     = Position::rinside(&sArea, e->nLeft, e->nTop, nRadius);
                if ((e->nCode == ws::MCB_LEFT) && (inside))
                    nState     |= XF_ACTIVE;
                else
                    nState     |= XF_OUT;
            }

            nBMask     |= (1 << e->nCode);

            return on_mouse_move(e);
        }

        status_t CheckBox::on_mouse_up(const ws::event_t *e)
        {
            on_mouse_move(e);

            size_t state    = nState;
            nBMask         &= ~(1 << e->nCode);
            if (nBMask == 0)
            {
                bool checked = state & XF_CHECKED;
                if (checked != sChecked.get())
                {
                    sChecked.commit_value(checked);
                    sSlots.execute(SLOT_SUBMIT, this);
                }
                nState         &= ~XF_OUT;
            }

            if (state != nState)
                query_draw();

            return STATUS_OK;
        }

        status_t CheckBox::on_mouse_move(const ws::event_t *e)
        {
            if (nState & XF_OUT)
                return STATUS_OK;

            size_t state    = nState;
            bool checked    = sChecked.get();
            bool inside     = Position::rinside(&sArea, e->nLeft, e->nTop, nRadius);

            nState          = lsp_setflag(nState, XF_HOVER, inside);
            if ((nBMask == ws::MCF_LEFT) && (inside))
                nState          = lsp_setflag(nState, XF_CHECKED, !checked);
            else
                nState          = lsp_setflag(nState, XF_CHECKED, checked);

            if (state != nState)
                query_draw();

            return STATUS_OK;
        }

        status_t CheckBox::on_mouse_out(const ws::event_t *e)
        {
            size_t state    = nState;
            nState          = (sChecked.get()) ? XF_CHECKED : 0;
            nBMask          = 0;

            if (state != nState)
                query_draw();

            return STATUS_OK;
        }

        status_t CheckBox::on_key_down(const ws::event_t *e)
        {
            size_t state    = nState;
            if (e->nCode == ' ')
            {
                bool checked    = !sChecked.get();
                nState          = lsp_setflag(nState, XF_CHECKED, checked);
                sChecked.commit_value(checked);
                sSlots.execute(SLOT_SUBMIT, this);
            }

            if (state != nState)
                query_draw();

            return STATUS_OK;
        }

        status_t CheckBox::slot_on_submit(Widget *sender, void *ptr, void *data)
        {
            CheckBox *_this = widget_ptrcast<CheckBox>(ptr);
            return (_this != NULL) ? _this->on_submit() : STATUS_BAD_ARGUMENTS;
        }

        status_t CheckBox::on_submit()
        {
            return STATUS_OK;
        }
    }
}


