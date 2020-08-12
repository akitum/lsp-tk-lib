/*
 * ComboBox.cpp
 *
 *  Created on: 8 авг. 2020 г.
 *      Author: sadko
 */

#include <lsp-plug.in/tk/tk.h>
#include <lsp-plug.in/stdlib/math.h>

namespace lsp
{
    namespace tk
    {
        const w_class_t ComboBox::metadata              = { "ComboBox", &WidgetContainer::metadata };

        ComboBox::ComboBox(Display *dpy):
            WidgetContainer(dpy),
            sLBox(dpy),
            sBorderSize(&sProperties),
            sBorderGap(&sProperties),
            sBorderRadius(&sProperties),
            sSpinSize(&sProperties),
            sSpinSeparator(&sProperties),
            sColor(&sProperties),
            sSpinColor(&sProperties),
            sTextColor(&sProperties),
            sSpinTextColor(&sProperties),
            sBorderColor(&sProperties),
            sBorderGapColor(&sProperties),
            sOpened(&sProperties),
            sTextFit(&sProperties),
            sFont(&sProperties),
            sConstraints(&sProperties),
            sTextLayout(&sProperties),
            sEmptyText(&sProperties),
            sSelected(&sProperties)
        {
            sTArea.nLeft    = 0;
            sTArea.nTop     = 0;
            sTArea.nWidth   = 0;
            sTArea.nHeight  = 0;

            sSArea.nLeft    = 0;
            sSArea.nTop     = 0;
            sSArea.nWidth   = 0;
            sSArea.nHeight  = 0;

            sVArea.nLeft    = 0;
            sVArea.nTop     = 0;
            sVArea.nWidth   = 0;
            sVArea.nHeight  = 0;

            pClass          = &metadata;
        }

        ComboBox::~ComboBox()
        {
            do_destroy();
        }

        void ComboBox::destroy()
        {
            do_destroy();
            WidgetContainer::destroy();
        }

        void ComboBox::do_destroy()
        {
        }

        status_t ComboBox::init()
        {
            // Initialize widgets
            status_t result = WidgetContainer::init();
            if (result == STATUS_OK)
                result  = sLBox.init();
            if (result != STATUS_OK)
                return result;

            // Configure ListBox
            sLBox.allocation()->set_embed(true);
            sLBox.set_parent(this);

            sBorderSize.bind("border.size", &sStyle);
            sBorderGap.bind("border.gap", &sStyle);
            sBorderRadius.bind("border.radius", &sStyle);
            sSpinSize.bind("spin.size", &sStyle);
            sSpinSeparator.bind("spin.separator", &sStyle);
            sColor.bind("color", &sStyle);
            sSpinColor.bind("spin.color", &sStyle);
            sTextColor.bind("text.color", &sStyle);
            sSpinTextColor.bind("spin.text.color", &sStyle);
            sBorderColor.bind("border.color", &sStyle);
            sBorderGapColor.bind("border.gap.color", &sStyle);
            sOpened.bind("opened", &sStyle);
            sTextFit.bind("text.fit", &sStyle);
            sFont.bind("font", &sStyle);
            sConstraints.bind("size.constraints", &sStyle);
            sTextLayout.bind("text.layout", &sStyle);
            sEmptyText.bind(&sStyle, pDisplay->dictionary());

            Style *sclass = style_class();
            if (sclass != NULL)
            {
                sBorderSize.init(sclass, 1);
                sBorderGap.init(sclass, 1);
                sBorderRadius.init(sclass, 4);
                sSpinSize.init(sclass, 10);
                sSpinSeparator.init(sclass, 1);
                sColor.init(sclass, "#ffffff");
                sSpinColor.init(sclass, "#ffffff");
                sTextColor.init(sclass, "#000000");
                sSpinTextColor.init(sclass, "#000000");
                sBorderColor.init(sclass, "#000000");
                sBorderGapColor.init(sclass, "#cccccc");
                sOpened.init(sclass, false);
                sTextFit.init(sclass);
                sFont.init(sclass);
                sConstraints.init(sclass, -1, -1, -1, 0);
                sTextLayout.init(sclass, -1.0f, 0.0f);
            }

            return STATUS_OK;
        }

        void ComboBox::property_changed(Property *prop)
        {
            WidgetContainer::property_changed(prop);

            if (sBorderSize.is(prop))
                query_resize();
            if (sBorderGap.is(prop))
                query_resize();
            if (sBorderRadius.is(prop))
                query_resize();
            if (sSpinSize.is(prop))
                query_resize();
            if (sSpinSeparator.is(prop))
                query_resize();
            if (sColor.is(prop))
                query_draw();
            if (sSpinColor.is(prop))
                query_draw();
            if (sTextColor.is(prop))
                query_draw();
            if (sSpinTextColor.is(prop))
                query_draw();
            if (sBorderColor.is(prop))
                query_draw();
            if (sBorderGapColor.is(prop))
                query_draw();
            if (sOpened.is(prop))
            {
                // TODO
            }
            if (sTextFit.is(prop))
                query_resize();
            if (sFont.is(prop))
                query_resize();
            if (sConstraints.is(prop))
                query_resize();
            if (sTextLayout.is(prop))
                query_draw();
            if (sEmptyText.is(prop))
                query_resize();
            if (sSelected.is(prop))
                query_draw();
        }

        void ComboBox::estimate_parameters(alloc_t *alloc, float scaling)
        {
            alloc->radius       = (sBorderRadius.get() > 0) ? lsp_max(1.0f, sBorderRadius.get() * scaling) : 0;
            alloc->border       = (sBorderSize.get() > 0) ? lsp_max(1.0f, sBorderSize.get() * scaling) : 0;
            alloc->bgap         = ((alloc->border > 0) && (sBorderGap.get() > 0)) ? lsp_max(0.0f, sBorderGap.get()) : 0;
            alloc->bsize        = alloc->border + alloc->bgap;
            alloc->swidth       = (sSpinSize.get() > 0) ? lsp_max(1.0f, sSpinSize.get() * scaling) : 0;
            alloc->sborder      = ((alloc->swidth > 0) && (sSpinSeparator.get() > 0)) ? lsp_max(1.0f, sSpinSeparator.get() * scaling) : 0;
            alloc->sgap         = (alloc->sborder > 0) ? alloc->bgap : 0;

            ssize_t rgap        = alloc->radius - lsp_max(0.0f, truncf(M_SQRT1_2 * (alloc->radius - alloc->bsize)));
            alloc->spad         = lsp_max(alloc->bsize, rgap);
        }

        void ComboBox::size_request(ws::size_limit_t *r)
        {
            alloc_t a;
            ws::rectangle_t ta, sa, va; // text area, spin area, split area

            float scaling       = lsp_max(0.0f, sScaling.get());
            estimate_parameters(&a, scaling);

            // Estimate size of the spin
            sa.nWidth           = (a.swidth > 0) ? a.swidth + a.spad : 0;
            sa.nHeight          = (a.swidth > 0) ? a.swidth + a.spad * 2 : 0;

            va.nWidth           = a.sborder + a.sgap * 2;
            va.nHeight          = sa.nHeight;

            // Estimate sizes of the text area
            ws::font_parameters_t fp;
            ws::text_parameters_t tp;
            LSPString text;

            sFont.get_parameters(pDisplay, scaling, &fp);
            ta.nWidth           = 0;
            ta.nHeight          = fp.Height;

            // Estimate the maximum text width
            sEmptyText.format(&text);
            sFont.get_text_parameters(pDisplay, &tp, scaling, &text);
            ta.nWidth           = lsp_max(ta.nWidth,  tp.Width);
            ta.nHeight          = lsp_max(ta.nHeight, tp.Height);

            WidgetList<ListBoxItem> *wl = sLBox.items();
            for (size_t i=0, n=wl->size(); i<n; ++i)
            {
                ListBoxItem *it = wl->get(i);
                if ((it == NULL) || (!it->visibility()->get()))
                    continue;

                it->text()->format(&text);
                sFont.get_text_parameters(pDisplay, &tp, scaling, &text);
                ta.nWidth           = lsp_max(ta.nWidth,  tp.Width);
                ta.nHeight          = lsp_max(ta.nHeight, tp.Height);
            }
            sTextFit.apply(&ta);

            ta.nWidth          += a.spad;
            ta.nHeight         += a.spad * 2;

            // Set-up size of the
            ssize_t extra       = a.radius * 2;
            r->nMinWidth        = lsp_max(extra, ta.nWidth + sa.nWidth + va.nWidth);
            r->nMinHeight       = lsp_max(extra, lsp_max(ta.nHeight, sa.nHeight));
            r->nMaxWidth        = -1;
            r->nMaxHeight       = -1;

            // Apply size constraints
            sConstraints.apply(r, scaling);
        }

        void ComboBox::realize(const ws::rectangle_t *r)
        {
            alloc_t a;
            float scaling       = lsp_max(0.0f, sScaling.get());
            estimate_parameters(&a, scaling);

            // Realize widget
            Widget::realize(r);

            // Estimate size of spin
            sSArea.nWidth       = (a.swidth > 0) ? a.swidth + a.spad : 0;
            sSArea.nHeight      = r->nHeight;
            sSArea.nLeft        = r->nLeft + r->nWidth - sSArea.nWidth;
            sSArea.nTop         = r->nTop;

            // Estimate size of vsep
            sVArea.nWidth       = a.sgap * 2 + a.sborder;
            sVArea.nHeight      = r->nHeight;
            sVArea.nLeft        = sSArea.nLeft - sVArea.nWidth;
            sVArea.nTop         = r->nTop;

            // Estimate size of the text area
            sTArea.nWidth       = sVArea.nLeft - r->nLeft;
            sTArea.nHeight      = r->nHeight;
            sTArea.nLeft        = r->nLeft;
            sTArea.nTop         = r->nTop;
        }

        void ComboBox::draw(ws::ISurface *s)
        {
            alloc_t a;
            bool aa;
            ws::rectangle_t ta, sa, va; // text area, spin area
            float scaling       = lsp_max(0.0f, sScaling.get());
            float bright        = sBrightness.get();
            estimate_parameters(&a, scaling);

            ta                  = sTArea;
            sa                  = sSArea;
            va                  = sVArea;

            ta.nLeft           -= sSize.nLeft;
            ta.nTop            -= sSize.nTop;
            sa.nLeft           -= sSize.nLeft;
            sa.nTop            -= sSize.nTop;
            va.nLeft           -= sSize.nLeft;
            va.nTop            -= sSize.nTop;

            // Draw background
            lsp::Color c(sBgColor);
            s->clear(c);
            aa                  = s->get_antialiasing();

            // Draw the border
            if (a.border > 0)
            {
                c.copy(sBorderColor);
                c.scale_lightness(bright);
                s->set_antialiasing(true);
                s->fill_round_rect(c, SURFMASK_ALL_CORNER, a.radius, 0, 0, sSize.nWidth, sSize.nHeight);

                a.radius        = lsp_max(0, a.radius - a.border);

                ta.nLeft       += a.border;
                ta.nTop        += a.border;
                ta.nWidth      -= a.border;
                ta.nHeight     -= a.border * 2;

                sa.nTop        += a.border;
                sa.nWidth      -= a.border;
                sa.nHeight     -= a.border * 2;

                va.nTop        += a.border;
                va.nHeight     -= a.border * 2;
            }

            // Draw the text area
            {
                ssize_t radius      = a.radius;
                s->set_antialiasing(true);

                // Draw border gap
                if (a.bgap > 0)
                {
                    c.copy(sBorderGapColor);
                    c.scale_lightness(bright);
                    s->fill_round_rect(c, SURFMASK_L_CORNER, radius, &ta);

                    ta.nLeft       += a.bgap;
                    ta.nTop        += a.bgap;
                    ta.nWidth      -= a.bgap;
                    ta.nHeight     -= a.bgap*2;
                    radius          = lsp_max(0, radius - a.bgap);
                }

                // Draw the prime color
                c.copy(sColor);
                c.scale_lightness(bright);
                s->fill_round_rect(c, SURFMASK_L_CORNER, radius, &ta);

                // Now reset parameters of ta before rendering the text
                ta.nLeft        = a.spad;
                ta.nWidth       = sTArea.nWidth  - a.spad;
                ta.nTop         = a.spad;
                ta.nHeight      = sTArea.nHeight - a.spad * 2;

                // Render the text
                LSPString text;
                ws::font_parameters_t fp;
                ws::text_parameters_t tp;
                float halign    = lsp_limit(sTextLayout.halign() + 1.0f, 0.0f, 2.0f);
                float valign    = lsp_limit(sTextLayout.valign() + 1.0f, 0.0f, 2.0f);

                ListBoxItem *sel = sSelected.get();
                if (sel != NULL)
                {
                    if (!sLBox.items()->contains(sel))
                        sel     = NULL;
                    else if (!sel->visibility()->get())
                        sel     = NULL;
                }

                if (sel != NULL)
                    sel->text()->format(&text);
                else
                    sEmptyText.format(&text);

                sFont.get_parameters(s, scaling, &fp);
                sFont.get_text_parameters(s, &tp, scaling, &text);
                float dy        = (ta.nHeight - fp.Height) * 0.5f;
                float dx        = (ta.nWidth  - tp.Width ) * 0.5f;
                ssize_t y       = ta.nTop   + dy * valign + fp.Ascent;
                ssize_t x       = ta.nLeft  + dx * halign - tp.XBearing;

                s->clip_begin(&ta);
                {
                    c.copy(sTextColor);
                    c.scale_lightness(bright);
                    sFont.draw(s, c, x, y, scaling, &text);
                }
                s->clip_end();
            }

            // Draw the background for spin area
            if (sa.nWidth > 0)
            {
                ssize_t radius  = a.radius;
                s->set_antialiasing(true);

                // Draw border gap
                if (a.bgap > 0)
                {
                    c.copy(sBorderGapColor);
                    c.scale_lightness(bright);
                    s->fill_round_rect(c, SURFMASK_R_CORNER, radius, &sa);

                    sa.nTop        += a.bgap;
                    sa.nWidth      -= a.bgap;
                    sa.nHeight     -= a.bgap*2;
                    radius          = lsp_max(0, radius - a.bgap);
                }

                // Draw the prime color
                c.copy(sSpinColor);
                c.scale_lightness(bright);
                s->fill_round_rect(c, SURFMASK_R_CORNER, radius, &sa);

                // Draw arrows
                c.copy(sSpinTextColor);
                c.scale_lightness(bright);
                s->fill_triangle(
                    sa.nLeft + sa.nWidth/6.0f, sa.nTop + (sa.nHeight*3.0f)/7.0f,
                    sa.nLeft + sa.nWidth*0.5f, sa.nTop + sa.nHeight/7.0f,
                    sa.nLeft + (sa.nWidth*5.0f)/6.0f, sa.nTop + (sa.nHeight*3.0f)/7.0f,
                    c);
                s->fill_triangle(
                    sa.nLeft + sa.nWidth/6.0f, sa.nTop + (sa.nHeight*4.0f)/7.0f,
                    sa.nLeft + (sa.nWidth*5.0f)/6.0f, sa.nTop + (sa.nHeight*4.0f)/7.0f,
                    sa.nLeft + sa.nWidth*0.5f, sa.nTop + (sa.nHeight*6.0f)/7.0f,
                    c);
            }

            // Draw the spin separator
            if (va.nWidth > 0)
            {
                s->set_antialiasing(false);

                // Draw separator gap
                if (a.sgap > 0)
                {
                    c.copy(sBorderGapColor);
                    c.scale_lightness(bright);
                    s->fill_rect(c, &va);

                    va.nLeft       += a.sgap;
                    va.nWidth      -= a.sgap*2;
                }

                if (va.nWidth > 0)
                {
                    c.copy(sBorderColor);
                    c.scale_lightness(bright);
                    s->fill_rect(c, &va);
                }
            }

            // Restore antialiaing
            s->set_antialiasing(aa);
        }

        status_t ComboBox::add(Widget *child)
        {
            return sLBox.add(child);
        }

        status_t ComboBox::remove(Widget *child)
        {
            return sLBox.remove(child);
        }

        status_t ComboBox::remove_all()
        {
            return sLBox.remove_all();
        }

        status_t ComboBox::on_mouse_down(const ws::event_t *e)
        {
            return STATUS_OK;
        }

        status_t ComboBox::on_mouse_up(const ws::event_t *e)
        {
            return STATUS_OK;
        }

        status_t ComboBox::on_mouse_move(const ws::event_t *e)
        {
            return STATUS_OK;
        }

        status_t ComboBox::on_mouse_scroll(const ws::event_t *e)
        {
            return STATUS_OK;
        }

        status_t ComboBox::on_key_down(const ws::event_t *e)
        {
            return STATUS_OK;
        }

        status_t ComboBox::on_key_up(const ws::event_t *e)
        {
            return STATUS_OK;
        }
    }
}


