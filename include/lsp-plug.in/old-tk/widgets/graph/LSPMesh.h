/*
 * LSPMesh.h
 *
 *  Created on: 19 июл. 2017 г.
 *      Author: sadko
 */

#ifndef UI_TK_LSPMESH_H_
#define UI_TK_LSPMESH_H_

#include <lsp-plug.in/tk-old/types.h>
#include <lsp-plug.in/tk-old/version.h>

namespace lsp
{
    namespace tk
    {
        class LSPMesh: public LSPGraphItem
        {
            public:
                static const w_class_t    metadata;

            protected:
                size_t              nWidth;         // Width of the line
                size_t              nCenter;
                size_t              nDimensions;
                size_t              nPoints;
                ssize_t             nXIndex;
                ssize_t             nYIndex;
                float              *vBuffer;
                size_t              nBufSize;
                LSPColor            sColor;

                float              *vData;          // Buffer to store actual point data
                size_t              nItems;         // Number of items in buffer

            protected:
                void        drop_data();

            public:
                explicit LSPMesh(LSPDisplay *dpy);
                virtual ~LSPMesh();

                virtual status_t init();
                virtual void destroy();

            public:
                inline size_t       line_width() const      { return nWidth; };
                inline size_t       center_id() const       { return nCenter; };
                inline size_t       dimensions() const      { return nDimensions; };
                inline size_t       points() const          { return nPoints; };
                inline LSPColor    *color()                 { return &sColor; };
                inline ssize_t      x_index() const         { return nXIndex; };
                inline ssize_t      y_index() const         { return nYIndex; };

            public:
                void                set_line_width(size_t value);
                void                set_center_id(size_t value);
                status_t            set_data(size_t dimensions, size_t points, const float **data);
                void                set_x_index(ssize_t value);
                void                set_y_index(ssize_t value);

            public:
                virtual void render(ISurface *s, bool force);
        };
    
    } /* namespace tk */
} /* namespace lsp */

#endif /* UI_TK_LSPMESH_H_ */
