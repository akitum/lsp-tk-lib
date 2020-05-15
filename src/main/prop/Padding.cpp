/*
 * Padding.cpp
 *
 *  Created on: 12 июл. 2017 г.
 *      Author: sadko
 */

#include <lsp-plug.in/tk/tk.h>

namespace lsp
{
    namespace tk
    {
        const prop::desc_t Padding::DESC[] =
        {
            { "",           PT_STRING   },
            { ".left",      PT_INT      },
            { ".right",     PT_INT      },
            { ".top",       PT_INT      },
            { ".bottom",    PT_INT      },
            { ".css",       PT_STRING   },
            { NULL,         PT_UNKNOWN  }
        };

        void Padding::Listener::notify(atom_t property)
        {
            pValue->commit(property);
        }

        Padding::Padding(prop::Listener *listener):
            MultiProperty(listener),
            sListener(this)
        {
            sValue.nLeft      = 0;
            sValue.nRight     = 0;
            sValue.nTop       = 0;
            sValue.nBottom    = 0;
        }

        Padding::~Padding()
        {
            MultiProperty::unbind(vAtoms, DESC, &sListener);
        }

        void Padding::parse(const LSPString *s)
        {
            ssize_t vv[4];
            size_t n        = parse_ints(vv, 4, s);
            padding_t &p    = sValue;

            for (size_t i=0; i<n; ++i)
                vv[i] = lsp_max(vv[i], 0);

            switch (n)
            {
                case 1:
                    p.nLeft     = vv[0];
                    p.nRight    = vv[0];
                    p.nTop      = vv[0];
                    p.nBottom   = vv[0];
                    break;

                case 2:
                    p.nLeft     = vv[0];
                    p.nRight    = vv[0];
                    p.nTop      = vv[1];
                    p.nBottom   = vv[1];
                    break;

                case 3:
                    p.nLeft     = vv[0];
                    p.nRight    = vv[1];
                    p.nTop      = vv[2];
                    p.nBottom   = vv[2];
                    break;

                case 4:
                    p.nLeft     = vv[0];
                    p.nRight    = vv[1];
                    p.nTop      = vv[2];
                    p.nBottom   = vv[3];
                    break;

                default:
                    break;
            }
        }

        void Padding::parse_css(const LSPString *s)
        {
            ssize_t vv[4];
            size_t n        = parse_ints(vv, 4, s);
            padding_t &p    = sValue;

            for (size_t i=0; i<n; ++i)
                vv[i] = lsp_max(vv[i], 0);

            switch (n)
            {
                case 1:
                    p.nLeft     = vv[0];
                    p.nRight    = vv[0];
                    p.nTop      = vv[0];
                    p.nBottom   = vv[0];
                    break;

                case 2:
                    p.nTop      = vv[0];
                    p.nBottom   = vv[0];
                    p.nLeft     = vv[1];
                    p.nRight    = vv[1];
                    break;

                case 3:
                    p.nTop      = vv[0];
                    p.nLeft     = vv[1];
                    p.nRight    = vv[1];
                    p.nBottom   = vv[2];
                    break;

                case 4:
                    p.nTop      = vv[0];
                    p.nRight    = vv[1];
                    p.nBottom   = vv[2];
                    p.nLeft     = vv[3];
                    break;

                default:
                    break;
            }
        }

        void Padding::commit(atom_t property)
        {
            if ((pStyle == NULL) || (property < 0))
                return;

            padding_t &p    = sValue;

            ssize_t v;
            if ((property == vAtoms[P_LEFT]) && (pStyle->get_int(vAtoms[P_LEFT], &v) == STATUS_OK))
                p.nLeft     = lsp_max(v, 0);
            if ((property == vAtoms[P_RIGHT]) && (pStyle->get_int(vAtoms[P_RIGHT], &v) == STATUS_OK))
                p.nRight    = lsp_max(v, 0);
            if ((property == vAtoms[P_TOP]) && (pStyle->get_int(vAtoms[P_TOP], &v) == STATUS_OK))
                p.nTop      = lsp_max(v, 0);
            if ((property == vAtoms[P_BOTTOM]) && (pStyle->get_int(vAtoms[P_BOTTOM], &v) == STATUS_OK))
                p.nTop      = lsp_max(v, 0);

            LSPString s;
            if ((property == vAtoms[P_CSS]) && (pStyle->get_string(vAtoms[P_CSS], &s) == STATUS_OK))
                parse_css(&s);
            if ((property == vAtoms[P_VALUE]) && (pStyle->get_string(vAtoms[P_VALUE], &s) == STATUS_OK))
                parse(&s);

            if (pListener != NULL)
                pListener->notify(this);
        }

        void Padding::sync()
        {
            if (pStyle != NULL)
            {
                pStyle->begin(&sListener);
                {
                    padding_t &p    = sValue;

                    // Simple components
                    if (vAtoms[P_LEFT] >= 0)
                        pStyle->set_int(vAtoms[P_LEFT], p.nLeft);
                    if (vAtoms[P_RIGHT] >= 0)
                        pStyle->set_int(vAtoms[P_RIGHT], p.nRight);
                    if (vAtoms[P_TOP] >= 0)
                        pStyle->set_int(vAtoms[P_TOP], p.nTop);
                    if (vAtoms[P_BOTTOM] >= 0)
                        pStyle->set_int(vAtoms[P_BOTTOM], p.nBottom);

                    // Compound objects
                    LSPString s;
                    if (vAtoms[P_CSS] >= 0)
                    {
                        if (s.fmt_ascii("%ld %ld %ld %ld", long(p.nTop), long(p.nRight), long(p.nBottom), long(p.nLeft)))
                            pStyle->set_string(vAtoms[P_CSS], &s);
                    }
                    if (vAtoms[P_VALUE] >= 0)
                    {
                        if (s.fmt_ascii("%ld %ld %ld %ld", long(p.nLeft), long(p.nRight), long(p.nTop), long(p.nBottom)))
                            pStyle->set_string(vAtoms[P_VALUE], &s);
                    }
                }
                pStyle->end();
            }

            if (pListener != NULL)
                pListener->notify(this);
        }

        void Padding::get(size_t *left, size_t *right, size_t *top, size_t *bottom) const
        {
            *left       = sValue.nLeft;
            *right      = sValue.nRight;
            *top        = sValue.nTop;
            *bottom     = sValue.nBottom;
        }

        void Padding::get(size_t &left, size_t &right, size_t &top, size_t &bottom) const
        {
            left        = sValue.nLeft;
            right       = sValue.nRight;
            top         = sValue.nTop;
            bottom      = sValue.nBottom;
        }

        size_t Padding::set_left(size_t value)
        {
            size_t old      = sValue.nLeft;
            if (old == value)
                return value;

            sValue.nLeft    = value;
            sync();
            return old;
        }

        size_t Padding::set_right(size_t value)
        {
            size_t old      = sValue.nRight;
            if (old == value)
                return value;

            sValue.nRight   = value;
            sync();
            return old;
        }

        size_t Padding::set_top(size_t value)
        {
            size_t old      = sValue.nTop;
            if (old == value)
                return value;

            sValue.nTop     = value;
            sync();
            return old;
        }

        size_t Padding::set_bottom(size_t value)
        {
            size_t old      = sValue.nBottom;
            if (old == value)
                return value;

            sValue.nBottom  = value;
            sync();
            return old;
        }

        void Padding::set_all(size_t value)
        {
            if ((sValue.nLeft == value) &&
                (sValue.nTop == value) &&
                (sValue.nRight == value) &&
                (sValue.nBottom == value))
                return;

            sValue.nLeft      = value;
            sValue.nTop       = value;
            sValue.nRight     = value;
            sValue.nBottom    = value;
            sync();
        }

        void Padding::set_horizontal(size_t left, size_t right)
        {
            if ((sValue.nLeft == left) &&
                (sValue.nRight == right))
                return;

            sValue.nLeft      = left;
            sValue.nRight     = right;
            sync();
        }

        void Padding::set_vertical(size_t top, size_t bottom)
        {
            if ((sValue.nTop == top) &&
                (sValue.nBottom == bottom))
                return;

            sValue.nTop       = top;
            sValue.nBottom    = bottom;
            sync();
        }

        void Padding::set(size_t left, size_t right, size_t top, size_t bottom)
        {
            if ((sValue.nLeft == left) &&
                (sValue.nRight == right) &&
                (sValue.nTop == top) &&
                (sValue.nBottom == bottom))
                return;

            sValue.nLeft      = left;
            sValue.nRight     = right;
            sValue.nTop       = top;
            sValue.nBottom    = bottom;
            sync();
        }

        void Padding::set(const padding_t *p)
        {
            if ((sValue.nLeft == p->nLeft) &&
                (sValue.nRight == p->nRight) &&
                (sValue.nTop == p->nTop) &&
                (sValue.nBottom == p->nBottom))
                return;

            sValue            = *p;
            sync();
        }

        void Padding::set(const Padding *p)
        {
            if ((sValue.nLeft == p->sValue.nLeft) &&
                (sValue.nRight == p->sValue.nRight) &&
                (sValue.nTop == p->sValue.nTop) &&
                (sValue.nBottom == p->sValue.nBottom))
                return;

            sValue            = p->sValue;
            sync();
        }

        void Padding::compute(padding_t *padding, float scale)
        {
            scale               = lsp_max(scale, 0.0f);
            padding->nLeft      = sValue.nLeft * scale;
            padding->nRight     = sValue.nRight * scale;
            padding->nTop       = sValue.nTop * scale;
            padding->nBottom    = sValue.nBottom * scale;
        }

        void Padding::add(ws::size_limit_t *dst, float scale)
        {
            scale               = lsp_max(scale, 0.0f);
            size_t hor          = (sValue.nLeft + sValue.nRight) * scale;
            size_t vert         = (sValue.nTop + sValue.nBottom) * scale;

            dst->nMinWidth      = (dst->nMinWidth < 0)  ? hor  : dst->nMinWidth  + hor;
            dst->nMinHeight     = (dst->nMinHeight < 0) ? vert : dst->nMinHeight + vert;
            if (dst->nMaxWidth >= 0)
                dst->nMaxWidth     += hor;
            if (dst->nMaxHeight >= 0)
                dst->nMaxHeight    += vert;
        }

        void Padding::sub(ws::rectangle_t *dst, float scale)
        {
            scale               = lsp_max(scale, 0.0f);
            ssize_t hor         = (sValue.nLeft + sValue.nRight) * scale;
            ssize_t vert        = (sValue.nTop + sValue.nBottom) * scale;
            dst->nWidth         = lsp_max(0, ssize_t(dst->nWidth)  - hor );
            dst->nHeight        = lsp_max(0, ssize_t(dst->nHeight) - vert);
        }

        namespace prop
        {
            status_t Padding::init(Style *style, size_t left, size_t right, size_t top, size_t bottom)
            {
                if (pStyle == NULL)
                    return STATUS_BAD_STATE;

                style->begin();
                {
                    style->create_int(vAtoms[P_LEFT], left);
                    style->create_int(vAtoms[P_RIGHT], right);
                    style->create_int(vAtoms[P_TOP], top);
                    style->create_int(vAtoms[P_BOTTOM], bottom);
                }
                style->end();
                return STATUS_OK;
            }

            status_t Padding::init(Style *style, const padding_t *p)
            {
                if (pStyle == NULL)
                    return STATUS_BAD_STATE;

                style->begin();
                {
                    style->create_int(vAtoms[P_LEFT], p->nLeft);
                    style->create_int(vAtoms[P_RIGHT], p->nRight);
                    style->create_int(vAtoms[P_TOP], p->nTop);
                    style->create_int(vAtoms[P_BOTTOM], p->nBottom);
                }
                style->end();
                return STATUS_OK;
            }
        }
    } /* namespace tk */
} /* namespace lsp */
