/*
 * Copyright (C) 2020 Linux Studio Plugins Project <https://lsp-plug.in/>
 *           (C) 2020 Vladimir Sadovnikov <sadko4u@gmail.com>
 *
 * This file is part of lsp-tk-lib
 * Created on: 15 июн. 2017 г.
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

#ifndef LSP_PLUG_IN_TK_BASE_WIDGET_H_
#define LSP_PLUG_IN_TK_BASE_WIDGET_H_

#ifndef LSP_PLUG_IN_TK_IMPL
    #error "use <lsp-plug.in/tk/tk.h>"
#endif

namespace lsp
{
    namespace tk
    {
        class Display;

        // Style definition
        namespace style
        {
            LSP_TK_STYLE_DEF_BEGIN(Widget, Style)
                prop::Allocation    sAllocation;        // Widget allocation
                prop::Float         sScaling;           // UI scaling factor
                prop::Float         sFontScaling;       // UI font scaling factor
                prop::Float         sBrightness;        // Brightness
                prop::Float         sBgBrightness;      // Brightness for background
                prop::Padding       sPadding;           // Widget padding
                prop::Color         sBgColor;           // Color of the background of the widget
                prop::Boolean       sBgInherit;         // Inhert background color from parent container
                prop::Boolean       sVisibility;        // Visibility
                prop::Pointer       sPointer;           // Mouse pointer
                prop::Integer       sTag;               // Some tag associated with widget
                prop::DrawMode      sDrawMode;          // Drawing mode
            LSP_TK_STYLE_DEF_END
        }

        /** Basic widget class for any widget in the toolkit
         *
         */
        class Widget: public ws::IEventHandler
        {
            private:
                Widget & operator = (const Widget &);
                Widget(const Widget &);

            public:
                static const w_class_t    metadata;

            protected:
                enum flags_t
                {
                    INITIALIZED     = 1 << 0,       // Widget is initialized
                    FINALIZED       = 1 << 1,       // Widget is in destroy state
                    REDRAW_SURFACE  = 1 << 2,       // Need to redraw surface
                    REDRAW_CHILD    = 1 << 3,       // Need to redraw child only
                    SIZE_INVALID    = 1 << 4,       // Size limit structure is valid
                    RESIZE_PENDING  = 1 << 5,       // The resize request is pending
                    REALIZE_ACTIVE  = 1 << 6        // Realize is active, no need to trigger for realize
                };

            protected:
                class PropListener: public prop::Listener
                {
                    protected:
                        Widget     *pWidget;

                    public:
                        inline PropListener(Widget *w)  { pWidget = w; }
                        virtual void notify(Property *prop);
                };

            protected:
                size_t              nFlags;             // Flags
                const w_class_t    *pClass;             // Widget class descriptor
                Display            *pDisplay;           // Pointer to display
                Widget             *pParent;            // Parent widget
                ws::ISurface       *pSurface;           // Drawing surface

                ws::size_limit_t    sLimit;             // Cached pre-computed size limit
                ws::rectangle_t     sSize;              // Real allocated geometry of widget

                SlotSet             sSlots;             // Slots
                Style               sStyle;             // Style
                PropListener        sProperties;        // Properties listener

                prop::Allocation    sAllocation;        // Widget allocation
                prop::Float         sScaling;           // UI scaling factor
                prop::Float         sFontScaling;       // UI font scaling factor
                prop::Float         sBrightness;        // Brightness
                prop::Float         sBgBrightness;      // Brightness for background
                prop::Padding       sPadding;           // Widget padding
                prop::Color         sBgColor;           // Color of the background of the widget
                prop::Boolean       sBgInherit;         // Inhert background color from parent container
                prop::Boolean       sBgApplyBrightness; // Apply brightness settings to the background
                prop::Boolean       sVisibility;        // Visibility
                prop::Pointer       sPointer;           // Mouse pointer
                prop::Integer       sTag;               // Some tag associated with widget
                prop::DrawMode      sDrawMode;          // Drawing mode of widget

            //---------------------------------------------------------------------------------
            // Slot handlers
            protected:
                static status_t     slot_mouse_move(Widget *sender, void *ptr, void *data);
                static status_t     slot_mouse_down(Widget *sender, void *ptr, void *data);
                static status_t     slot_mouse_up(Widget *sender, void *ptr, void *data);
                static status_t     slot_mouse_click(Widget *sender, void *ptr, void *data);
                static status_t     slot_mouse_dbl_click(Widget *sender, void *ptr, void *data);
                static status_t     slot_mouse_tri_click(Widget *sender, void *ptr, void *data);
                static status_t     slot_mouse_scroll(Widget *sender, void *ptr, void *data);
                static status_t     slot_mouse_in(Widget *sender, void *ptr, void *data);
                static status_t     slot_mouse_out(Widget *sender, void *ptr, void *data);
                static status_t     slot_key_down(Widget *sender, void *ptr, void *data);
                static status_t     slot_key_up(Widget *sender, void *ptr, void *data);
                static status_t     slot_hide(Widget *sender, void *ptr, void *data);
                static status_t     slot_show(Widget *sender, void *ptr, void *data);
                static status_t     slot_destroy(Widget *sender, void *ptr, void *data);
                static status_t     slot_resize(Widget *sender, void *ptr, void *data);
                static status_t     slot_resize_parent(Widget *sender, void *ptr, void *data);
                static status_t     slot_focus_in(Widget *sender, void *ptr, void *data);
                static status_t     slot_focus_out(Widget *sender, void *ptr, void *data);
                static status_t     slot_drag_request(Widget *sender, void *ptr, void *data);
                static status_t     slot_realized(Widget *sender, void *ptr, void *data);

            //---------------------------------------------------------------------------------
            // Interface for nested classes
            protected:
                void                    do_destroy();

                void                    unlink_widget(Widget *widget);

                /**
                 * Callback on call when property has been change
                 * @param prop property that has been changed
                 */
                virtual void            property_changed(Property *prop);

                /** Request widget for it's minimum and maximum size.
                 * Since parent widget is responsible for padding,
                 * widget should return it's maximum size without applying
                 * it's padding parameter
                 *
                 * @param limit the widget size constraints to fill
                 */
                virtual void            size_request(ws::size_limit_t *r);

                /**
                 * Realize widget internally
                 * @param r real area allocated to the widget
                 */
                virtual void            realize(const ws::rectangle_t *r);

                /** Hide widget
                 *
                 */
                virtual void            hide_widget();

                /** Show widget
                 *
                 */
                virtual void            show_widget();

            //---------------------------------------------------------------------------------
            // Construction and destruction
            public:
                explicit Widget(Display *dpy);
                virtual ~Widget();

                /** Initialize wiget
                 *
                 */
                virtual status_t        init();

                /** Destroy widget
                 *
                 */
                virtual void            destroy();

            //---------------------------------------------------------------------------------
            // Metadata, casting and type information
            public:
                /** Get widget class
                 *
                 * @return actual widget class metadata
                 */
                inline const w_class_t *get_class() const { return pClass; }

                /**
                 * Get style class
                 * @return style class
                 */
                virtual const char *style_class() const;

                /** Check wheter the widget is instance of some class
                 *
                 * @param wclass widget class
                 * @return true if widget is instance of some class
                 */
                bool instance_of(const w_class_t *wclass) const;

                inline bool instance_of(const w_class_t &wclass) const { return instance_of(&wclass); }

                /** Another way to check if widget is instance of some class
                 *
                 * @return true if widget is instance of some class
                 */
                template <class LSPTarget>
                    inline bool instance_of() const { return instance_of(&LSPTarget::metadata); };

                /** Cast widget to another type
                 *
                 * @return pointer to widget or NULL if cast failed
                 */
                template <class LSPTarget>
                    inline LSPTarget *cast() { return instance_of(&LSPTarget::metadata) ? static_cast<LSPTarget *>(this) : NULL; }

                /** Cast widget to another type
                 *
                 * @return pointer to widget or NULL if cast failed
                 */
                template <class LSPTarget>
                    inline const LSPTarget *cast() const { return instance_of(&LSPTarget::metadata) ? static_cast<const LSPTarget *>(this) : NULL; }

                /** Get pointer to self as pointer to Widget class
                 *
                 * @return pointer to self
                 */
                inline Widget *self()                           { return this;  }

            //---------------------------------------------------------------------------------
            // Properties
            public:
                /** Get display
                 *
                 * @return display
                 */
                inline Display         *display()                                   { return pDisplay;                  }

                /** Get actual widget dimensions without padding
                 *
                 * @param r real widget dimensions
                 */
                inline void             get_rectangle(ws::rectangle_t *r)           { *r = sSize;                       }

                /** Get actual widget dimensions with padding applied
                 *
                 * @param r real widget dimensions
                 */
                inline void             get_padded_rectangle(ws::rectangle_t *r)    { sPadding.leave(r, &sSize, sScaling.get()); }

                /**
                 * Get position of the widget relative to the screen
                 * @param r pointer to store result
                 * @param sr source rectangle that belongs to the widget
                 * @return status of operation
                 */
                virtual status_t        get_screen_rectangle(ws::rectangle_t *r, const ws::rectangle_t *sr);
                virtual status_t        get_screen_rectangle(ws::rectangle_t *r);

                /**
                 * Get position of the widget relative to the screen with applied padding
                 * @param r pointer to store result
                 * @return status of operation
                 */
                virtual status_t        get_padded_screen_rectangle(ws::rectangle_t *r, const ws::rectangle_t *sr);
                virtual status_t        get_padded_screen_rectangle(ws::rectangle_t *r);

                /** Request widget for it's minimum and maximum size.
                 * Since parent widget is responsible for padding,
                 * widget should return it's maximum size without applying
                 * it's padding parameter
                 *
                 * @param l the widget size constraints to fill
                 */
                void                    get_size_limits(ws::size_limit_t *l);

                /**
                 * Get size limit with padding
                 * @param l size limit to calculate
                 */
                void                    get_padded_size_limits(ws::size_limit_t *l);

                /**
                 * Get actual padding in pixels
                 * @param p pointer to store computed padding
                 */
                void                    get_padding(padding_t *p);

                /**
                 * Check whether widget is focused
                 * @return true if widget is focsed
                 */
                bool                    has_focus() const;

                /**
                 * Make widget being focused
                 * @return true if widget has been focused, false if widget already had focus or it is impossible
                 * to take the focus
                 */
                virtual bool            take_focus();

                /**
                 * Release focus previously acquired by widget
                 * @return true if focus has been released, false if widget had no focus
                 */
                virtual bool            kill_focus();

                /**
                 * Get current mouse pointer
                 * @return current mouse pointer
                 */
                virtual ws::mouse_pointer_t current_pointer();

                /** Check if there is redraw request pending
                 *
                 * @return true if there is redraw request pending
                 */
                inline bool             redraw_pending() const              { return nFlags & (REDRAW_SURFACE | REDRAW_CHILD); }

                /** Check if there is resize request pending
                 *
                 * @return true if there is resize request pending
                 */
                inline bool             resize_pending() const              { return nFlags & (SIZE_INVALID | RESIZE_PENDING); }

                /** Check that specified window coordinate lies within widget's bounds
                 * Always returns false for invisible widgets
                 *
                 * @param left x coordinate
                 * @param top y coordinate
                 * @return true on success
                 */
                virtual bool            inside(ssize_t left, ssize_t top);

                /** Get parent widget
                 *
                 * @return parent widget
                 */
                inline Widget          *parent()                { return pParent; }

                /**
                 * Find parent widget of the specified type
                 * @param meta metadata
                 * @return the parent widget of the specified type or NULL
                 */
                Widget                 *parent(const w_class_t *meta);

                /** Get slots
                 *
                 * @return slots
                 */
                inline SlotSet         *slots()                 { return &sSlots; }

                /** Get slot
                 *
                 * @param id slot identifier
                 * @return pointer to slot or NULL
                 */
                inline Slot            *slot(slot_t id)         { return sSlots.slot(id); }

                /**
                 * Check that widget is in valid state
                 * @return true if widget is in valid state
                 */
                inline bool             valid() const           { return (nFlags & (INITIALIZED | FINALIZED)) == INITIALIZED; }

                /**
                 * Check whether widget is visible child of parent widget
                 * @param parent parent widget
                 * @return true if widget is visible child of parent widget
                 */
                bool                    is_visible_child_of(const Widget *parent) const;

                /**
                 * Compute the actual background color and store it to the passed color value
                 * @param color color to store the actual value
                 * @param brightness apply brightness value, if negative then bg_brightness will be applied
                 */
                virtual void            get_actual_bg_color(lsp::Color *color, float brightness = -1.0f) const;
                void                    get_actual_bg_color(lsp::Color &color, float brightness = -1.0f) const;

                /**
                 * Return widget's style
                 * @return widget's style
                 */
                LSP_TK_PROPERTY(Style,              style,              &sStyle)

                /** Get widget padding. All widgets should properly handle this parameter
                 *
                 * @return widget padding
                 */
                LSP_TK_PROPERTY(Padding,            padding,            &sPadding)

                /**
                 * Get background color of the widget
                 * @return background color of the widget
                 */
                LSP_TK_PROPERTY(Color,              bg_color,           &sBgColor)

                /**
                 * Get the inheritance flag of the background color.
                 * If enabled, the color is taken from the parent widget container if it
                 * is present. Otherwise, the bg_color() property is used to draw the color.
                 *
                 * @return inheritance flag of the background color
                 */
                LSP_TK_PROPERTY(Boolean,            bg_inherit,         &sBgInherit)

                /**
                 * Get brightness property
                 * @return brightness property
                 */
                LSP_TK_PROPERTY(Float,              brightness,         &sBrightness)

                /**
                 * Get brightness property for background
                 * @return brightness property for background
                 */
                LSP_TK_PROPERTY(Float,              bg_brightness,      &sBgBrightness)

                /**
                 * Get widget scaling property
                 * @return widget scaling property
                 */
                LSP_TK_PROPERTY(Float,              scaling,            &sScaling)

                /**
                 * Get font scaling property
                 * @return font scaling property
                 */
                LSP_TK_PROPERTY(Float,              font_scaling,       &sFontScaling)

                /**
                 * Widget allocation flags
                 * @return widget allocation flags
                 */
                LSP_TK_PROPERTY(Allocation,         allocation,         &sAllocation)

                /**
                 * Visibility of the widget
                 * @return visibility of the widget
                 */
                LSP_TK_PROPERTY(Boolean,            visibility,         &sVisibility)

                /**
                 * Style of mouse pointer
                 * @return mouse pointer style
                 */
                LSP_TK_PROPERTY(Pointer,            pointer,            &sPointer)

                /**
                 * Some tag associated with widget, can be used as the user decides
                 */
                LSP_TK_PROPERTY(Integer,            tag,                &sTag)

                /**
                 * Drawing mode of the widget
                 */
                LSP_TK_PROPERTY(DrawMode,           draw_mode,          &sDrawMode)

            //---------------------------------------------------------------------------------
            // Manipulation
            public:
                /**
                 * Return mouse handler
                 * @param x left coordinate of the mouse pointer relative to the top-level widget
                 * @param y top coordinate of the mouse pointer relative to the top-level widget
                 * @return pointer to widget that will handle mouse events
                 */
                virtual Widget         *find_widget(ssize_t x, ssize_t y);

                /** Query widget for redraw
                 *
                 * @param flags redraw flags
                 */
                virtual void            query_draw(size_t flags = REDRAW_SURFACE);

                /**
                 * Put the widget to the destroy queue of the main loop
                 * @return status of operation
                 */
                virtual status_t        queue_destroy();

                /** Query widget for resize
                 *
                 */
                virtual void            query_resize();

                /** Get widget surface
                 *
                 * @param s base surface
                 * @return widget surface or NULL
                 */
                ws::ISurface           *get_surface(ws::ISurface *s);

                /** Get widget surface
                 *
                 * @param s base surface
                 * @param width requested width
                 * @param height requested height
                 * @return widget surface or NULL
                 */
                ws::ISurface           *get_surface(ws::ISurface *s, ssize_t width, ssize_t height);

                /** Render widget to the external surface
                 *
                 * @param surface surface to perform rendering
                 * @param area the actual area that will be used for drawing
                 * @param force force child rendering
                 */
                virtual void            render(ws::ISurface *s, const ws::rectangle_t *area, bool force);

                /** Draw widget on the internal surface
                 *
                 * @param surface surface to perform drawing
                 */
                virtual void            draw(ws::ISurface *s);

                /**
                 * Realize widget by passing it's dimensions.
                 * Since parent widget is responsible for padding, widget dimensions should
                 * be passed without padding applied to the widget
                 *
                 * @param r real area allocated to the widget
                 */
                void                    realize_widget(const ws::rectangle_t *r);

                /** Handle UI event from the display
                 *
                 * @param e UI event
                 * @return status of operation
                 */
                virtual status_t        handle_event(const ws::event_t *e);

                /** Set parent widget of this widget
                 *
                 * @param parent parent widget
                 */
                void                    set_parent(Widget *parent);

                /** Commit widet redraw
                 *
                 */
                virtual void            commit_redraw();

                /**
                 * Show widget
                 */
                virtual void            show();

                /**
                 * Hide widget
                 */
                virtual void            hide();

                /** Get most top-level widget
                 *
                 * @return most top-level widget
                 */
                Widget                 *toplevel();

            //---------------------------------------------------------------------------------
            // Event handling
            public:
                /** Widget has taken focus
                 *
                 * @param e event
                 * @return status of operation
                 */
                virtual status_t        on_focus_in(const ws::event_t *e);

                /** Widget has lost focus
                 *
                 * @param e event
                 * @return status of operation
                 */
                virtual status_t        on_focus_out(const ws::event_t *e);

                /** Handle key press event
                 * @param e event
                 * @return status of operation
                 */
                virtual status_t        on_key_down(const ws::event_t *e);

                /** Handle key release event
                 * @param e event
                 * @return status of operation
                 */
                virtual status_t        on_key_up(const ws::event_t *e);

                /** Handle mouse button press event
                 * @param e event
                 * @return status of operation
                 */
                virtual status_t        on_mouse_down(const ws::event_t *e);

                /** Handle mouse button release event
                 * @param e event
                 * @return status of operation
                 */
                virtual status_t        on_mouse_up(const ws::event_t *e);

                /** Handle mouse motion event
                 * @param e event
                 * @return status of operation
                 */
                virtual status_t        on_mouse_move(const ws::event_t *e);

                /** Handle mouse cursor moved into the zone of widget
                 *
                 * @param e mouse event that triggered MouseIn
                 * @return status of operation
                 */
                virtual status_t        on_mouse_in(const ws::event_t *e);

                /** Handle mouse cursor moved outside the zone of widget
                 *
                 * @param e mouse event that triggered MouseIn
                 * @return status of operation
                 */
                virtual status_t        on_mouse_out(const ws::event_t *e);

                /** Handle mouse scroll event
                 *
                 * @param e event
                 * @return status of operation
                 */
                virtual status_t        on_mouse_scroll(const ws::event_t *e);

                /** Handle single mouse click
                 *
                 * @param e event
                 * @return status of operation
                 */
                virtual status_t        on_mouse_click(const ws::event_t *e);

                /** Handle mouse double click event
                 *
                 * @param e event
                 * @return status of operation
                 */
                virtual status_t        on_mouse_dbl_click(const ws::event_t *e);

                /** Handle mouse triple click
                 *
                 * @param e event
                 * @return status of operation
                 */
                virtual status_t        on_mouse_tri_click(const ws::event_t *e);

                /** Geometry has changed: size or position
                 *
                 * @param r the new size of the widget. Handler should not rely
                 *   on data returned by get_rectangle() and get_padded_rectangle()
                 *   since it will contain the previous values
                 * @return status of operation
                 */
                virtual status_t        on_resize(const ws::rectangle_t *r);

                /** Geometry of parent widget has changed: size or position
                 *
                 * @param e event
                 * @return status of operation
                 */
                virtual status_t        on_resize_parent(const ws::rectangle_t *r);

                /** The widget becomes hidden
                 *
                 * @return status of operation
                 */
                virtual status_t        on_hide();

                /** The widget becomes visible
                 *
                 * @return status of operation
                 */
                virtual status_t        on_show();

                /** The widget becomes destroyed
                 *
                 * @return status of operation
                 */
                virtual status_t        on_destroy();

                /**
                 * Process the drag request event
                 * @param e drag request event
                 * @param ctype NULL-terminated list of provided content types
                 * @return status of operation
                 */
                virtual status_t        on_drag_request(const ws::event_t *e, const char * const *ctype);

                /** The widget has just been realized
                 * @param size the actual widget size
                 * @return status of operation
                 */
                virtual status_t        on_realized(const ws::rectangle_t *size);
        };
    }

    // LLTL interface specifications for widget
    namespace lltl
    {
        template <>
            struct hash_spec<tk::Widget>: public ptr_hash_iface {};

        template <>
            struct compare_spec<tk::Widget>: public ptr_compare_iface {};
    }

} /* namespace lsp */

#endif /* LSP_PLUG_IN_TK_BASE_WIDGET_H_ */
