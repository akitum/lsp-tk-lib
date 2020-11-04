/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-tk-lib
 * Created on: 12 авг. 2020 г.
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

namespace lsp
{
    namespace tk
    {
        const prop::desc_t TextFitness::DESC[] =
        {
            { "",           PT_STRING   },
            { ".hfit",      PT_FLOAT    },
            { ".vfit",      PT_FLOAT    },
            { NULL,         PT_UNKNOWN  }
        };

        void TextFitness::Listener::notify(atom_t property)
        {
            pValue->commit(property);
        }

        TextFitness::TextFitness(prop::Listener *listener):
            MultiProperty(vAtoms, P_COUNT, listener),
            sListener(this)
        {
            hFit        = 1.0f;
            vFit        = 1.0f;
        }

        TextFitness::~TextFitness()
        {
            MultiProperty::unbind(vAtoms, DESC, &sListener);
        }

        void TextFitness::sync()
        {
            if (pStyle != NULL)
            {
                pStyle->begin(&sListener);
                {
                    // Simple components
                    if (vAtoms[P_HFIT] >= 0)
                        pStyle->set_float(vAtoms[P_HFIT], hFit);
                    if (vAtoms[P_VFIT] >= 0)
                        pStyle->set_float(vAtoms[P_VFIT], vFit);

                    // Compound objects
                    LSPString s;
                    if (vAtoms[P_VALUE] >= 0)
                    {
                        if (s.fmt_ascii("%.4f %.4f", hFit, vFit))
                            pStyle->set_string(vAtoms[P_VALUE], &s);
                    }
                }
                pStyle->end();
            }

            if (pListener != NULL)
                pListener->notify(this);
        }

        void TextFitness::parse(const LSPString *s)
        {
            float v[2];
            size_t n = MultiProperty::parse_floats(v, 2, s);
            switch (n)
            {
                case 1:
                    hFit        = lsp_limit(v[0], -1.0f, 1.0f);
                    vFit        = hFit;
                    break;

                case 2:
                    hFit        = lsp_limit(v[0], -1.0f, 1.0f);
                    vFit        = lsp_limit(v[1], -1.0f, 1.0f);
                    break;

                default:
                    break;
            }
        }

        void TextFitness::commit(atom_t property)
        {
            if ((pStyle == NULL) || (property < 0))
                return;

            float v;
            if ((property == vAtoms[P_HFIT]) && (pStyle->get_float(vAtoms[P_HFIT], &v) == STATUS_OK))
                hFit        = lsp_limit(v, -1.0f, 1.0f);
            if ((property == vAtoms[P_VFIT]) && (pStyle->get_float(vAtoms[P_VFIT], &v) == STATUS_OK))
                vFit        = lsp_limit(v, -1.0f, 1.0f);

            LSPString s;
            if ((property == vAtoms[P_VALUE]) && (pStyle->get_string(vAtoms[P_VALUE], &s) == STATUS_OK))
                parse(&s);

            // Update/notify listeners
            if (pStyle->sync())
                this->sync();
            else if (pListener != NULL)
                pListener->notify(this);
        }

        float TextFitness::set_hfit(float v)
        {
            float prev  = hFit;
            v           = lsp_max(0.0f, v);
            if (prev == v)
                return prev;

            hFit        = v;
            sync();

            return prev;
        }

        float TextFitness::set_vfit(float v)
        {
            float prev  = vFit;
            v           = lsp_max(0.0f, v);
            if (prev == v)
                return prev;

            vFit        = v;
            sync();

            return prev;
        }

        void TextFitness::set(float h, float v)
        {
            h           = lsp_max(0.0f, h);
            v           = lsp_max(0.0f, v);
            if ((hFit == h) && (vFit == v))
                return;

            hFit        = h;
            vFit        = v;
            sync();
        }

        void TextFitness::apply(ws::rectangle_t *r)
        {
            r->nWidth   = lsp_max(0.0f, hFit * r->nWidth);
            r->nHeight  = lsp_max(0.0f, hFit * r->nHeight);
        }

        status_t TextFitness::init()
        {
            pStyle->begin();
            {
                pStyle->create_float(vAtoms[P_HFIT], hfit());
                pStyle->create_float(vAtoms[P_VFIT], vfit());

                // Compound objects
                LSPString s;
                s.fmt_ascii("%.4f %.4f", hfit(), vfit());
                pStyle->create_string(vAtoms[P_VALUE], &s);
            }
            pStyle->end();
            return STATUS_OK;
        }

        status_t TextFitness::override()
        {
            pStyle->begin();
            {
                pStyle->override_float(vAtoms[P_HFIT], hfit());
                pStyle->override_float(vAtoms[P_VFIT], vfit());

                // Compound objects
                LSPString s;
                s.fmt_ascii("%.4f %.4f", hfit(), vfit());
                pStyle->override_string(vAtoms[P_VALUE], &s);
            }
            pStyle->end();
            return STATUS_OK;
        }

        namespace prop
        {
            status_t TextFitness::init(Style *style, float halign, float valign)
            {
                if (pStyle == NULL)
                    return STATUS_BAD_STATE;

                style->begin();
                {
                    style->create_float(vAtoms[P_HFIT], halign);
                    style->create_float(vAtoms[P_VFIT], valign);

                    // Compound objects
                    LSPString s;
                    s.fmt_ascii("%.4f %.4f", halign, valign);
                    style->create_string(vAtoms[P_VALUE], &s);
                }
                style->end();
                return STATUS_OK;
            }

            status_t TextFitness::init(const char *name, Style *style, float hfit, float vfit)
            {
                prop::TextFitness v;
                LSP_STATUS_ASSERT(v.bind(name, style));
                v.set(hfit, vfit);
                return v.init();
            }

            status_t TextFitness::override(const char *name, Style *style, float hfit, float vfit)
            {
                prop::TextFitness v;
                LSP_STATUS_ASSERT(v.bind(name, style));
                v.set(hfit, vfit);
                return v.override();
            }

        }

    } /* namespace tk */
} /* namespace lsp */






