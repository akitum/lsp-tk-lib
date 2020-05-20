/*
 * WidgetContainer.cpp
 *
 *  Created on: 16 июн. 2017 г.
 *      Author: sadko
 */

#include <lsp-plug.in/tk/tk.h>

namespace lsp
{
    namespace tk
    {
        const w_class_t WidgetContainer::metadata = { "WidgetContainer", &Widget::metadata };

        WidgetContainer::WidgetContainer(Display *dpy):
            Widget(dpy)
        {
            pClass          = &metadata;
        }

        WidgetContainer::~WidgetContainer()
        {
        }

        status_t WidgetContainer::add(Widget *child)
        {
            return STATUS_NOT_IMPLEMENTED;
        }

        status_t WidgetContainer::remove(Widget *child)
        {
            return STATUS_NOT_FOUND;
        }

        status_t WidgetContainer::remove_all()
        {
            return STATUS_NOT_IMPLEMENTED;
        }
    } /* namespace tk */
} /* namespace lsp */
