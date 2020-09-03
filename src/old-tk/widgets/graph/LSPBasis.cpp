/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-tk-lib
 * Created on: 19 июл. 2017 г.
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

#include <lsp-plug.in/tk-old/widgets/graph/LSPBasis.h>

namespace lsp
{
    namespace tk
    {
        const w_class_t LSPBasis::metadata = { "LSPBasis", &LSPGraphItem::metadata };

        LSPBasis::LSPBasis(LSPDisplay *dpy): LSPGraphItem(dpy)
        {
            nID = -1;
            pClass          = &metadata;
        }

        LSPBasis::~LSPBasis()
        {
        }
    } /* namespace tk */
} /* namespace lsp */
