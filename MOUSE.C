/****************************************************************************/
/*********************************  Mouse.C  ********************************/
/****************************************************************************/
// This file includes modules for Mashitup mouse interface.
// Mouse.C is included in the project.
// Mouse.C size is XX.X KB.

/****************************************************************************/

#include "basic.h"

/*********************  Defines for mouse actual work  **********************/

// Buttons definition.

#define  LEFT_BUTTON      (1)       // Bit code of left   button.
#define  RIGHT_BUTTON     (2)       // Bit code of right  button.
#define  CENTER_BUTTON    (4)       // Bit code of center button.
#define  ANY_BUTTON       (7)       // Bit code of any    button.

// Button codes for get_mouse_state() function.
#define L_BUTT            (0)       // Code for info. about left  button.
#define R_BUTT            (1)       // Code for info. about right button.

// Ratio between VGA coordinates and mouse standard coordinates.
// Font is (8x8)

#define  TEXT_X_RATION    (8)
#define  TEXT_Y_RATION    (8)

// Mouse events that activate event handler.
//#define E_MOUSE_MOVE      (0x1)       // Mouse was moved.
//#define E_L_PRESSED       (0x2)       // Left   button pressed.
//#define E_L_RELEASED      (0x4)       // Left   button released.
//#define E_R_PRESSED       (0x8)       // Right  button pressed.
//#define E_R_RELEASED      (0x10)      // Right  button released.
//#define E_C_PRESSED       (0x20)      // Center button pressed.
//#define E_C_RELEASED      (0x40)      // Center button released.

// Mouse events as passed to the event handler procedure.
#define E_L_PRESSED       (0x1)       // Left   button pressed.
#define E_R_PRESSED       (0x2)       // Right  button pressed.
#define E_C_PRESSED       (0x4)       // Center button pressed.

// Mouse current coordinates.
static coord_t  mc = { MID_X, MID_Y };

// Mouse dragging indicator.
static m_drag_t md = { FALSE, FALSE, FALSE };

// Indicates whether mouse is visible or not.
static boolean is_mouse_focused = FALSE;

// Indicates whether mouse has moved minimum one VGA cell ( 80 * 50 );
static m_move_t has_mouse = { FALSE, FALSE, FALSE };

/****************************************************************************/

extern cdrom_data_t cd_data;

extern int M_layer;
extern volume_handler volhdr;

extern unsigned char chg_pprog_add;

/****************************************************************************/
/**************************  Modules declarations  **************************/

// Returns true if mouse is installed. Otherwise returns False.
// Parameters    : None.
// Side affects  : None.
// Modules Calls : None.
boolean is_mouse_installed(void);

/****************************************************************************/

// Returns the number of mouse buttons.
// Parameters    : None.
// Side affects  : None.
// Modules Calls : None.
int mouse_num_buttons(void);

/****************************************************************************/

// Returns the mouse and all its parameters to initial phase.
// Parameters    : None.
// Side affects  : None.
// Modules Calls : None.
void reset_mouse(void);

/****************************************************************************/

// Makes the mouse cursor be visible.
// Parameters    : None.
// Side affects  : None.
// Modules Calls : None.
void mouse_on(void);

/****************************************************************************/

// Hides the mouse cursor.
// Parameters    : None.
// Side affects  : None.
// modules Calls : None.
void mouse_off(void);

/****************************************************************************/

// Gets mouse page.
// Parameters    : None.
// Side affects  : None.
// modules Calls : None.
char get_mouse_page(void);

/****************************************************************************/

// Sets mouse page.
// Parameters    : None.
// Side affects  : None.
// modules Calls : None.
void set_mouse_page( char page );

/****************************************************************************/

// Sets mouse cursor to be character of shape argument with colors of color
// parameter.
// Parameters    : color, shape.
// Side affects  : None.
// modules Calls : None.
void set_mouse_shape_and_color( const char color, const char shape );

/****************************************************************************/

// Sets mouse cursor to be character of shape argument with color of the
// screen segments byte where cursor position is ( transparent background ).
// By default mouse forground is white.
// Parameters    : shape.
// Side affects  : None.
// modules Calls : set_mouse_shape_and_color().
void set_chameleon_mouse_shape( const char shape );

/****************************************************************************/

// Sets apparatus ( hardware ) blinking mouse cursor.
// Parameters    : y_up - upper scan line, y_dw - end scan line. In Mashitups
//                 (8X8) Mode scan lines are ranging in 0..7.
// Side affects  : None.
// modules Calls : None.
void set_mouse_blink( int y_up, int y_dw );

/****************************************************************************/

// Checks if mouse is installed, Loads its basic parameters and make it
// appear in Mashitup's shape. Returns true if function could perform those
// tasks.
// Parameters    : None.
// Side affects  : md.r, md.l, md.c are set to False.
// modules Calls : All initial modules, set_mouse_event_handler().
boolean new_mouse(void);

/****************************************************************************/

// Makes the mouse to appear on top of all the Mashitup's components.
// Parameters    : None.
// Side affects  : is_mouse_focused is set to True.
// modules Calls : is_mouse_pressed(), set_chameleon_mouse_shape(),
//                mouse_on().
void focus_mouse(void);

/****************************************************************************/

// Take from the mouse its focus.
// Parameters    : None.
// Side affects  : is_mouse_focused is set to False.
// modules Calls : None.
void defocus_mouse(void);

/****************************************************************************/

// Hides the mouse cursor and disables all the work with it.
// Parameters    : None.
// Side affects  : is_mouse_focused is set to False, event handler is
//                 distructed.
// modules Calls : set_mouse_page(), distruct_event_handler(), mouse_off().
void free_mouse(void);

/****************************************************************************/

// Turns val from VGA ( 80 * 50 ) coordinate to mouse standard coordinate
// in X pivot.
// Parameters    : val.
// Side affects  : None.
// modules Calls : None.
void VGA_to_mstd_x( int *val );

/****************************************************************************/

// Turns val from VGA ( 80 * 50 ) coordinate to mouse standard coordinate
// in Y pivot.
// Parameters    : val.
// Side affects  : None.
// modules Calls : None.
void VGA_to_mstd_y( int *val );

/****************************************************************************/

// Turns val from mouse standard coordinate to VGA ( 80 * 50 ) coordinate
// in X pivot.
// Parameters    : val.
// Side affects  : None.
// modules Calls : None.
void mstd_to_VGA_x( int *val );

/****************************************************************************/

// Turns val from mouse standard coordinate to VGA ( 80 * 50 ) coordinate
// in Y pivot.
// Parameters    : val.
// Side affects  : None.
// modules Calls : None.
void mstd_to_VGA_y( int *val );

/****************************************************************************/

// Returns mouse cursor position in VGA ( 80 * 50 ) coordinates into x & y
// arguments.
// Parameters    : x, y.
// Side affects  : None.
// modules Calls : mstd_to_VGA_x(), mstd_to_VGA_y().
void m_where_xy( int *x, int *y );

/****************************************************************************/

// Returns mouse cursor X position in VGA ( 80 * 50 ) coordinates.
// Parameters    : None.
// Side affects  : None.
// modules Calls : mstd_to_VGA_x().
int m_where_x(void);

/****************************************************************************/

// Returns mouse cursor Y position in VGA ( 80 * 50 ) coordinates.
// Parameters    : None.
// Side affects  : None.
// modules Calls : mstd_to_VGA_y().
int m_where_y(void);

/****************************************************************************/

// Returns the position of mouse pointer in (x,y) arguments and the state of
// mouse buttons in butt argument.
// Parameters    : butt, x, y.
// Side affects  : None.
// modules Calls : mstd_to_VGA_x(), mstd_to_VGA_y().
void get_mouse_state( int *butt, int *x, int *y );

/****************************************************************************/

// Returns True if any button is pressed. Otherwise returns False.
// Uses third interrup.
// Parameters    : None.
// Side affects  : None.
// modules Calls : None.
boolean is_mouse_pressed(void);

/****************************************************************************/

// Returns true if left button is pressed.
// Doesn't activate any interrupt, instead uses "ax" button status argument.
// Parameters    : ax.
// Side affects  : None.
// modules Calls : None.
boolean lb_pressed( int ax );

/****************************************************************************/

// Returns true if right button is pressed.
// Doesn't activate any interrupt, instead uses "ax" button status argument.
// Parameters    : ax.
// Side affects  : None.
// modules Calls : None.
boolean rb_pressed( int ax );

/****************************************************************************/

// Returns true if center button is pressed.
// Doesn't activate any interrupt, instead uses "ax" button status argument.
// Parameters    : ax.
// Side affects  : None.
// modules Calls : None.
boolean cb_pressed( int ax );

/****************************************************************************/

// Returns true if currently any button on mouse is pressed.
// Doesn't activate any interrupt, instead uses "ax" button status argument.
// Parameters    : ax.
// Side affects  : None.
// modules Calls : None.
boolean is_press_event( int ax );

/****************************************************************************/

// Returns true if left button is released.
// Doesn't activate any interrupt, instead uses "ax" button status argument.
// Parameters    : ax.
// Side affects  : None.
// modules Calls : None.
boolean lb_released( int ax );

/****************************************************************************/

// Returns true if right button is released.
// Doesn't activate any interrupt, instead uses "ax" button status argument.
// Parameters    : ax.
// Side affects  : None.
// modules Calls : None.
boolean rb_released( int ax );

/****************************************************************************/

// Returns true if center button is released.
// Doesn't activate any interrupt, instead uses "ax" button status argument.
// Parameters    : ax.
// Side affects  : None.
// modules Calls : None.
boolean cb_released( int ax );

/****************************************************************************/

// Returns true if currently any button on mouse is released.
// Doesn't activate any interrupt, instead uses "ax" button status argument.
// Parameters    : ax.
// Side affects  : None.
// modules Calls : None.
boolean is_release_event( int ax );

/****************************************************************************/

// Gets the button (L_BUTT,R_BUTT) in butt argument and returns information
// about that button ( if it was pressed, where and how much times ).
// Parameters    : butt, event.
// Side affects  : None.
// modules Calls : None.
void mouse_butt_pressed( int butt, button_event_t *event );

/****************************************************************************/

// Gets the button (L_BUTT,R_BUTT) in butt argument and returns information
// about that button ( if it was released, where and how much times ).
// Parameters    : butt, event.
// Side affects  : None.
// modules Calls : None.
void mouse_butt_released( int butt, button_event_t *event );

/****************************************************************************/

// Moves mouse cursor to x,y coordinates.
// Parameters    : x, y.
// Side affects  : None.
// modules Calls : mstd_to_VGA_x(), mstd_to_VGA_y().
void m_goto_xy( int x, int y );

/****************************************************************************/

// Sets mouse movement speed. This speed will be horizontal and vertical.
// Parameters    : speed.
// Side affects  : None.
// modules Calls : None.
void set_mouse_speed( int speed );

/****************************************************************************/

// Make cage for mouse. It may be only between lx -> rx and ly -> ry.
// Parameters    : lx, ly, rx, ry.
// Side affects  : None.
// modules Calls : mstd_to_VGA_x(), mstd_to_VGA_y().
void set_mouse_window( int lx, int ly, int rx, int ry );

/****************************************************************************/

// Returns True if mouse is inside window which coordinates are arguments.
// Uses third interrupt.
// Parameters    : lx, ly, rx, ry.
// Side affects  : None.
// modules Calls : mstd_to_VGA_x(), mstd_to_VGA_y().
boolean intr_mouse_in( int lx, int ly, int rx, int ry );

/****************************************************************************/

// Returns True if mouse is inside window which coordinates are arguments.
// No calls for interrupts.
// Parameters    : lx, ly, rx, ry.
// Side affects  : None.
// modules Calls : None.
boolean mouse_in( int lx, int ly, int rx, int ry );

/****************************************************************************/

// Procedure which is specified by its argument address will be called each
// time that:
// Mouse is moved.
// Mouse button is clicked.
// Mouse button is released.
// Parameters    : handler_address - address of handler procedure.
// Side affects  : Each specified above mouse event calls handler module.
// modules Calls : None.
void inlay_mouse_event_handler( void far *handler_address );

/****************************************************************************/

// Event handler interrupt.
// Parameters    : None.
// Side affects  : mc, md & has_mouse variables are appropriately changed.
// modules Calls : is_press_event(), is_release_event(),
//                All XXX_pressed(), on_mouse_XXX_click(),
//                on_mouse_XXX_drag() modules.
void interrupt mouse_event_handler(void);

/****************************************************************************/

// Activates the handler for each mouse event.
// Parameters    : None.
// Side affects  : Activates the inlay_mouse_event_handler().
// modules Calls : inlay_mouse_event_handler().
void set_mouse_event_handler(void);

/****************************************************************************/

// Deactivates the mouse interrupt handler.
// Parameters    : None.
// Side affects  : Mouse handler will not be called anymore.
// modules Calls : None.
void distruct_event_handler(void);

/****************************************************************************/

// What happens when left button is clicked.
// Parameters    : None.
// Side affects  : None.
// modules Calls : mouse_in(), k_move_finger(),
//                push_con_char(), push_con_mextend_char().
void on_mouse_left_click(void);

/****************************************************************************/

// What happens when right button is clicked.
// Parameters    : None.
// Side affects  : None.
// modules Calls : mouse_in(), k_move_finger(),
//                push_con_char(), push_con_mextend_char().
void on_mouse_right_click(void);

/****************************************************************************/

// What happens when center button is clicked.
// Parameters    : None.
// Side affects  : None.
// modules Calls : None.
void on_mouse_center_click(void);

/****************************************************************************/

// What happens when left button is pressed and mouse drags something.
// Parameters    : None.
// Side affects  : None.
// modules Calls : mouse_in(), k_move_finger(),
//                push_con_char(), push_con_mextend_char().
void on_mouse_left_drag(void);

/****************************************************************************/

// What happens when right button is pressed and mouse drags something.
// Parameters    : None.
// Side affects  : None.
// modules Calls : mouse_in(), k_move_finger(),
//                push_con_char(), push_con_mextend_char().
void on_mouse_right_drag(void);

/****************************************************************************/

// What happens when center button is pressed and mouse drags something.
// Parameters    : None.
// Side affects  : None.
// modules Calls : None.
void on_mouse_center_drag(void);

/****************************************************************************/
/****************************************************************************/
/*************************  modules implementation  *************************/

// Returns true if mouse is installed. Otherwise returns False.
boolean is_mouse_installed(void)
    {
    int mouseok;
    asm {
        Mov Ax,0H
        Int 33H
        Mov mouseok,Ax
    }
    return (boolean)!!mouseok;
}

/****************************************************************************/

// Returns the number of mouse buttons.
int mouse_num_buttons(void)
    {
    int but_num;
    asm {
        Mov Ax,0H
        Int 33H
        Mov but_num,Bx
    }
    return but_num;
}

/****************************************************************************/

// Returns the mouse and all its parameters to initial phase.
void reset_mouse(void)
    {
    asm {
        Mov Ax,21H
        Int 33H
    }
    return;
}

/****************************************************************************/

// Makes the mouse cursor be visible.
void mouse_on(void)
    {
    asm {
        Mov Ax,1H
        Int 33H
    }
    return;
}

/****************************************************************************/

// Hides the mouse cursor.
void mouse_off(void)
    {
    asm {
        Mov Ax,2H
        Int 33H
    }
    return;
}

/****************************************************************************/

// Gets mouse page.
char get_mouse_page(void)
    {
    char page;
    asm {
        Mov Ax,1EH
        Int 33H
        Mov page,Bl
    }
    return page;
}

/****************************************************************************/

// Sets mouse page.
void set_mouse_page( char page )
    {
    asm {
        Mov Ax,1DH
        Mov Bl,page
        Xor Bh,Bh
        Int 33H
    }
    return;
}

/****************************************************************************/

// Sets mouse cursor to be character of shape argument with colors of color
// parameter.
void set_mouse_shape_and_color( const char color, const char shape )
    {
    asm {
        Mov Ax,0AH
        Xor Bx,Bx
        Xor Cx,Cx
        Mov Dl,shape
        Mov Dh,color
        Int 33H
    }
    return;
}

/****************************************************************************/

// Sets mouse cursor to be character of shape argument with color of the
// screen segments byte where cursor position is ( transparent background ).
// By default mouse forground is white.
void set_chameleon_mouse_shape( const char shape )
    {
    int x = mc.x;
    int y = mc.y;

    // Set the shape of the mouse with current back ground.
    asm {
        // Get back ground.
        Push Ds
        Mov Bx,0B800H
        Mov Ds,Bx
        Mov Ax,y
        Mov Bx,160
        Mul Bl
        Mov Cx,x
        Shl Cx,1
        Add Cx,1
        Add Ax,Cx
        Mov Si,Ax
        LodSB

        Mov Dh,Al
        And Dh,7FH               // Set of blinking.
        Or  Dh,0FH               // White is default.
        Mov Dl,shape

        Mov Ax,0AH
        Xor Bx,Bx
        Xor Cx,Cx
        Int 33H
        Pop Ds
    }
    return;
}

/****************************************************************************/

// Sets aparatus ( harware ) blinking mouse cursor.
void set_mouse_blink( int y_up, int y_dw )
    {
    asm {
        Mov Ax,0AH
        Mov Bx,1
        Mov Cx,y_up
        Mov Dx,y_dw
        Int 33H
    }
    return;
}

/****************************************************************************/

// Checks if mouse is installed, Loads its basic parameters and make it
// appear in Mashitup's shape. Returns true if function could perform those
// tasks.
// Parameters   : None.
// Side affects : md.r, md.l, md.c are set to False.
// modules Calls : All initial modules, set_mouse_event_handler().
boolean new_mouse(void)
    {
    boolean mouse = is_mouse_installed();
    if (mouse){
       reset_mouse();
       set_mouse_page(0);

       m_goto_xy( mc.x, mc.y );
       md.r = md.l = md.c = FALSE;

       focus_mouse();
       set_mouse_speed(30);

       // Activate Mashitup interrupt for mouse handling.
       set_mouse_event_handler();
    }
    else {
       // Mouse is not installed.
    }
    return mouse;
}

/****************************************************************************/

// Makes the mouse to appear on top of all the Mashitup's components.
// Parameters   : None.
// Side affects : is_mouse_focused is set to True.
// modules Calls : is_mouse_pressed(), set_chameleon_mouse_shape(),
//                mouse_on().
void focus_mouse(void)
    {
    // If already focused do nothing.
    if (is_mouse_focused){
       return;
    }
    else {
       is_mouse_focused = TRUE;
       // Set the shape of the mouse with current back ground.
       if (is_mouse_pressed())
          set_chameleon_mouse_shape(MOUSE_P_SHAPE);
       else
          set_chameleon_mouse_shape(MOUSE_R_SHAPE);
       mouse_on();
    }
    return;
}

/****************************************************************************/

// Take from the mouse its focus.
void defocus_mouse(void)
    {
    // If already defocused do nothing.
    if (!is_mouse_focused){
       return;
    }
    else {
       is_mouse_focused = FALSE;
       asm {
           Mov Ax,2H
           Int 33H
       }
    }
    return;
}

/****************************************************************************/

// Hides the mouse cursor and disables all the work with it.
void free_mouse(void)
    {
    is_mouse_focused = FALSE;
    set_mouse_page(0);
    distruct_event_handler();
    mouse_off();
    return;
}
/****************************************************************************/

// Turns val from VGA ( 80 * 50 ) coordinate to mouse standard coordinate
// in X pivot.
void VGA_to_mstd_x( int *val )
    {
    *val *= TEXT_X_RATION;
    return;
}

/****************************************************************************/

// Turns val from VGA ( 80 * 50 ) coordinate to mouse standard coordinate
// in Y pivot.
void VGA_to_mstd_y( int *val )
    {
    *val *= TEXT_Y_RATION;
    return;
}

/****************************************************************************/

// Turns val from mouse standard coordinate to VGA ( 80 * 50 ) coordinate
// in X pivot.
void mstd_to_VGA_x( int *val )
    {
    *val /= TEXT_X_RATION;
    return;
}

/****************************************************************************/

// Turns val from mouse standard coordinate to VGA ( 80 * 50 ) coordinate
// in Y pivot.
void mstd_to_VGA_y( int *val )
    {
    *val /= TEXT_Y_RATION;
    return;
}

/****************************************************************************/

// Returns mouse cursor position in VGA ( 80 * 50 ) coordinates.
void m_where_xy( int *x, int *y )
    {
    asm {
        Mov Ax,3H
        Int 33H
    }
    *x = _CX;
    *y = _DX;
    mstd_to_VGA_x(x);
    mstd_to_VGA_y(y);
    return;
}

/****************************************************************************/

// Returns mouse cursor X position in VGA ( 80 * 50 ) coordinates.
int m_where_x(void)
    {
    int x;
    asm {
        Mov Ax,3H
        Int 33H
        Mov x,Cx
    }
    mstd_to_VGA_x(&x);
    return x;
}

/****************************************************************************/

// Returns mouse cursor Y position in VGA ( 80 * 50 ) coordinates.
int m_where_y(void)
    {
    int y;
    asm {
        Mov Ax,3H
        Int 33H
        Mov y,Dx
    }
    mstd_to_VGA_y(&y);
    return y;
}

/****************************************************************************/

// Returns the position of mouse pointer in (x,y) arguments and the state of
// mouse buttons in butt argument.
void get_mouse_state( int *butt, int *x, int *y )
    {
    asm {
        Mov Ax,3H
        Int 33H
    }
    *butt = _BX;
    *x    = _CX;
    *y    = _DX;
    mstd_to_VGA_x(x);
    mstd_to_VGA_y(y);
    return;
}

/****************************************************************************/

// Returns True if any button is pressed. Otherwise returns False.
// Uses third interrup.
boolean is_mouse_pressed(void)
    {
    boolean pressed;

    asm {
        Mov Ax,3H
        Int 33H
	Mov pressed,Bx
    }
    return pressed;
}

/****************************************************************************/

// Returns true if left button is pressed.
// Doesn't activate any interrupt, instead uses "ax" button status argument.
boolean lb_pressed( int ax )
    {
    return ( ax & E_L_PRESSED );
}

/****************************************************************************/

// Returns true if right button is pressed.
// Doesn't activate any interrupt, instead uses "ax" button status argument.
boolean rb_pressed( int ax )
    {
    return ( ax & E_R_PRESSED );
}

/****************************************************************************/

// Returns true if center button is pressed.
// Doesn't activate any interrupt, instead uses "ax" button status argument.
boolean cb_pressed( int ax )
    {
    return ( ax & E_C_PRESSED );
}

/****************************************************************************/

// Returns true if currently any button on mouse is pressed.
// Doesn't activate any interrupt, instead uses "ax" button status argument.
boolean is_press_event( int ax )
    {
    return ( ax & E_L_PRESSED )||
           ( ax & E_R_PRESSED )||
           ( ax & E_C_PRESSED );
}

/****************************************************************************/

// Returns true if left button is released.
// Doesn't activate any interrupt, instead uses "ax" button status argument.
boolean lb_released( int ax )
    {
    return !( ax & E_L_PRESSED );
}

/****************************************************************************/

// Returns true if right button is released.
// Doesn't activate any interrupt, instead uses "ax" button status argument.
boolean rb_released( int ax )
    {
    return !( ax & E_R_PRESSED );
}

/****************************************************************************/

// Returns true if center button is released.
// Doesn't activate any interrupt, instead uses "ax" button status argument.
boolean cb_released( int ax )
    {
    return !( ax & E_C_PRESSED );
}

/****************************************************************************/

// Returns true if currently any button on mouse is released.
// Doesn't activate any interrupt, instead uses "ax" button status argument.
boolean is_release_event( int ax )
    {
    return !( ax & E_L_PRESSED )||
           !( ax & E_R_PRESSED )||
           !( ax & E_C_PRESSED );
}

/****************************************************************************/

// Gets the button (L_BUTT,R_BUTT) in butt argument and returns information
// about that button ( if it was pressed, where and how much times ).
void mouse_butt_pressed( int butt, button_event_t *event )
    {
    asm {
        Mov Ax,5H
        Mov Bx,butt
        Int 33H
    }
    event->butt_stat = _AX;
    event->count     = _BX;
    event->x         = _CX;
    event->y         = _DX;
    return;
}

/****************************************************************************/

// Gets the button (L_BUTT,R_BUTT) in butt argument and returns information
// about that button ( if it was released, where and how much times ).
void mouse_butt_released( int butt, button_event_t *event )
    {
    asm {
        Mov Ax,6H
        Mov Bx,butt
        Int 33H
    }
    event->butt_stat = _AX;
    event->count     = _BX;
    event->x         = _CX;
    event->y         = _DX;
    return;
}

/****************************************************************************/

// Moves mouse cursor to x,y coordinates.
void m_goto_xy( int x, int y )
    {
    // Turn VGA ( 80 * 50 ) coordinates to mouse coordinates.
    VGA_to_mstd_x(&x);
    VGA_to_mstd_y(&y);

    // Set coordinates.
    asm {
        Mov Ax,4H
        Mov Cx,x
        Mov Dx,y
        Int 33H
    }
    return;
}

/****************************************************************************/

// Sets mouse movement speed. This speed will be horizontal and vertical.
void set_mouse_speed( int speed )
    {
    asm {
	// Update the speed for horizontal movement.
	Mov Ax,speed
	Mov Cx,Ax
	And Cx,7FFFH

	// Update the speed for vertical   movement.
	Mov Ax,speed
	Mov Bl,TEXT_Y_RATION
	Mul Bl
	Mov Bl,TEXT_X_RATION
	Div Bl
	Xor Ah,Ah
	Mov Dx,Ax
	And Dx,7FFFH
	Mov Ax,0FH
	Int 33H
    }
    return;
}

/****************************************************************************/

// Make cage for mouse. It may be only between lx -> rx and ly -> ry.
void set_mouse_window( int lx, int ly, int rx, int ry )
    {
    // Turn VGA ( 80 * 50 ) coordinates to mouse coordinates.
    VGA_to_mstd_x(&lx);
    VGA_to_mstd_y(&ly);
    VGA_to_mstd_x(&rx);
    VGA_to_mstd_y(&ry);
    asm {
        // Set x pivot border.
        Mov Ax,7
        Mov Cx,lx
        Mov Dx,rx
        Int 33H

        // Set y pivot border.
        Mov Ax,8
        Mov Cx,ly
        Mov Dx,ry
        Int 33H
    }
    return;
}

/****************************************************************************/

// Returns True if mouse is inside window which coordinates are arguments.
// Uses third interrupt.
boolean intr_mouse_in( int lx, int ly, int rx, int ry )
    {
    // Turn VGA ( 80 * 50 ) coordinates to mouse coordinates.
    VGA_to_mstd_x(&lx);
    VGA_to_mstd_y(&ly);
    VGA_to_mstd_x(&rx);
    VGA_to_mstd_y(&ry);
    asm {
        Mov Ax,3H
        Int 33H

        // Check if in.
        cmp Cx,lx
     jl not_in
        cmp Cx,rx
     jg not_in
        cmp Dx,ly
     jl not_in
        cmp Dx,ry
     jg not_in
        Mov Ax,1;
    jmp fend
    }
    not_in:
    asm Mov Ax,0;
    fend:
    return _AX;
}

/****************************************************************************/

// Returns True if mouse is inside window which coordinates are arguments.
// No calls for interrupts.
boolean mouse_in( int lx, int ly, int rx, int ry )
    {
    return  (( lx <= mc.x )&&( mc.x <= rx )&&
             ( ly <= mc.y )&&( mc.y <= ry ));
}

/****************************************************************************/

// Procedure which is specified by its argument address will be called each
// time that:
// Mouse is moved.
// Mouse button is clicked.
// Mouse button is released.
void inlay_mouse_event_handler( void far *handler_address )
    {
//  _ES = FP_SEG(handler_address);
//  _DX = FP_OFF(handler_address);
    _ES = (unsigned long)handler_address / 65536L;
    _DX = (unsigned long)handler_address % 65536L;
    asm {
	Mov Ax,0CH
	Mov Cx,7FH
	Int 33H
    }
    return;
}

/****************************************************************************/

// Event handler interrupt.
void interrupt mouse_event_handler(void)
    {
    handler_t h;

    // Get iterrupt arguments
    h.mask_status = _AX;
    h.butt_stat   = _BX;
    h.x		  = _CX;
    h.y		  = _DX;
    h.hor_count   = _SI;
    h.ver_count   = _DI;

    // Convert coordinates.
    h.x /= TEXT_X_RATION;
    h.y /= TEXT_Y_RATION;

    // Update mouse background
    if ( ( h.x != mc.x )||( h.y != mc.y )||
         is_press_event(h.mask_status)   ||
         is_release_event(h.mask_status) ){

       // Update mouse coordinates and has_mouse_moved variabes.
       has_mouse.moved_x = ( mc.x - h.x );
       has_mouse.moved_y = ( mc.y - h.y );
       has_mouse.moved = (has_mouse.moved_x)||(has_mouse.moved_y);

       _CX = mc.x = h.x;
       _DX = mc.y = h.y;

       // If mouse exits on one side it will appear on another.
       // Mouse walks on circular screen's x axis.
       // First new coordinates are set.
       if ( mc.x > 78 ){
          mc.x = 1;
          _CX = mc.x;
       }
       else
       if ( mc.x < 1 ){
          mc.x = 78;
          _CX = mc.x;
       }

       _DI = h.butt_stat;
       // Set the shape of the mouse with current back ground.
       asm {
           // Set screen segment.
           Push Ds
           Mov Bx,0B800H
           Mov Ds,Bx

           // Set offset [ y * 80 * 2 + x * 2 + 1 ].
           Mov Ax,Dx
           Mov Bx,160
           Mul Bl
           Shl Cx,1
           Add Cx,1
           Add Ax,Cx
           Mov Si,Ax

           // Get the background of the mouse cursor and set its shape.
           LodSB
           Mov Dh,Al
           And Dh,7FH               // Set of blinking.
           Or  Dh,0FH               // Set white color.
           Mov Dl,MOUSE_R_SHAPE

           Test Di,ANY_BUTTON
        Jz not_pressed
           Mov Dl,MOUSE_P_SHAPE
       }
       not_pressed:
       asm {
           Mov Ax,0AH
           Xor Bx,Bx
           Xor Cx,Cx
           Int 33H
           Pop Ds
       }
    }

    // When ( mc.x != h.x ) it means that mouse made a circular movement
    // ( around the screen ).
    if ( mc.x != h.x ){
          // Apply new coordinates.
          m_goto_xy( mc.x, mc.y );
    }
    // Mouse left button pressed.
    if (lb_pressed(h.butt_stat)){
       if (!md.l){                        // If not dragging.
          md.l = TRUE;
	  on_mouse_left_click();
       }
       else
          if (has_mouse.moved){           // If dragging.
             on_mouse_left_drag();
          }
    }
    // Mouse left button released.
    else {
          md.l = FALSE;                   // Not dragging.
    }

    // Mouse right button pressed.
    if (rb_pressed(h.butt_stat)){
       if (!md.r){                        // If not dragging.
          md.r = TRUE;
	  on_mouse_right_click();
       }
       else
          if (has_mouse.moved){          // If dragging.
             on_mouse_right_drag();
          }
    }
    // Mouse right button released.
    else {
       md.r = FALSE;                      // Not dragging.
    }

    // Mouse center button pressed.
    if (cb_pressed(h.butt_stat)){
       if (!md.c){                        // If not dragging.
          md.c = TRUE;
	  on_mouse_center_click();
       }
       else
          if (has_mouse.moved){           // If dragging.
             on_mouse_center_drag();
          }
    }
    // Mouse left button released.
    else {
          md.c = FALSE;                   // Not dragging.
    }

    return;
}

/****************************************************************************/

// Activates the handler for each mouse event.
void set_mouse_event_handler(void)
    {
    inlay_mouse_event_handler( &mouse_event_handler );
    return;
}

/****************************************************************************/

// Deactivates the mouse interrupt handler.
void distruct_event_handler(void)
    {
    asm {
        Xor Dx,Dx
        Mov Es,Dx
	Mov Ax,0CH
	Xor Cx,Cx
	Int 33H
    }
    return;
}

/****************************************************************************/
// What happens when left button is clicked.
void on_mouse_left_click(void)
    {
    // Each layer has distinctn methods.
    switch (M_layer) {

           case NO_MOUSE_LAYER :
                break;

           case MAIN_LAYER     :
                //--------------------------------------
                // Screen saver is activated by clicking on the bar.
                if ( mouse_in( 0, 0, 79, 0 )){
                   push_con_char(K_SCREEN_SAV);
                }
                else
                //--------------------------------------
                // Hammer effect.
                if ( mouse_in( CT_L_X + 3, CT_R_Y - 7,
                               CT_L_X + 5, CT_R_Y - 7 )){
                   push_con_char(SP_HAMMER_KEY);
                }
                else
                //--------------------------------------
                // Word up effect.
                if ( mouse_in( CT_L_X + 8 , CT_R_Y - 7,
                               CT_L_X + 16, CT_R_Y - 7 )){
                   push_con_char(SP_WORDUP_KEY);
                }
                else
                //--------------------------------------
                // Rewind some seconds back.
                if ( mouse_in( CT_L_X + 19, CT_R_Y - 7,
                               CT_L_X + 21, CT_R_Y - 7 )){
                   push_con_mextend_char(BACK_SEC_KEY);
                }
                else
                //--------------------------------------
                // Roll some seconds forward.
                if ( mouse_in( CT_L_X + 24, CT_R_Y - 7,
                               CT_L_X + 26, CT_R_Y - 7 )){
                   push_con_mextend_char(FORW_SEC_KEY);
                }
                else
                //--------------------------------------
                // One song back.
                if ( mouse_in( CT_L_X + 3, CT_R_Y - 4,
                               CT_L_X + 6, CT_R_Y - 4 )){
                   push_con_mextend_char(K_UP);
                }
                else
                //--------------------------------------
                // Play song.
                if ( mouse_in( CT_L_X + 9,  CT_R_Y - 4,
                               CT_L_X + 11, CT_R_Y - 4 )){
                   push_con_char(PLAY_BUTTON_KEY);
                }
                else
                //--------------------------------------
                // Pause song.
                if ( mouse_in( CT_L_X + 14, CT_R_Y - 4,
                               CT_L_X + 16, CT_R_Y - 4 )){
                   push_con_char(PAUSE_BUTTON_KEY);
                }
                else
                //--------------------------------------
                // Stop song.
                if ( mouse_in( CT_L_X + 19, CT_R_Y - 4,
                               CT_L_X + 21, CT_R_Y - 4 )){
                   push_con_char(STOP_BUTTON_KEY);
                }
                else
                //--------------------------------------
                // One song forward.
                if ( mouse_in( CT_L_X + 24, CT_R_Y - 4,
                               CT_L_X + 27, CT_R_Y - 4 )){
                   push_con_mextend_char(K_DOWN);
                }
                else
                //--------------------------------------
                // Play random button.
                if ( mouse_in( CT_R_X - 6, CT_L_Y + 3,
                               CT_R_X - 3, CT_L_Y + 3 )){
                   push_con_char(PLAY_RANDOM_KEY);
                }
                else
                //--------------------------------------
                // Repeat track button.
                if ( mouse_in( CT_R_X - 6, CT_L_Y + 6,
                               CT_R_X - 3, CT_L_Y + 6 )){
                   push_con_char(PLAY_REPEAT_KEY);
                }
                else
                //--------------------------------------
                // Lock CD-ROM button.
                if ( mouse_in( CT_R_X - 6, CT_L_Y + 9,
                               CT_R_X - 3, CT_L_Y + 9 )){
                   push_con_char(LOCK_CD_KEY);
                }
                else
                //--------------------------------------
                // Eject disk button.
                if ( mouse_in( CT_R_X - 6, CT_L_Y + 12,
                               CT_R_X - 3, CT_L_Y + 12 )){
                   push_con_char(EJECT_BUTTON_KEY);
                }
                else
                //--------------------------------------
                // Change the play time direction.
                if ( mouse_in( CT_L_X + 24, CT_L_Y + 5,
                               CT_L_X + 28, CT_L_Y + 7 )){
                   push_con_char(K_TIME_DIR);
                }
                else
                //--------------------------------------
                // Track is selected by mouse.
                if ( mouse_in( TT_L_X + 2, TT_L_Y + 7,
                               TT_R_X - 2, TT_R_Y - 5)){
                   // Check track existance.
                   if ( mc.y - ( TT_L_Y + 7 ) + 1 <= cd_data.max_track )
                      k_move_finger( mc.y - ( TT_L_Y + 7 ) + 1 );
                }
                else
                //--------------------------------------
                // If mouse in equlizer.
                if ( mouse_in( VT_L_X + 3 , VT_L_Y + 3,
                               VT_L_X + 35, VT_L_Y + 12)){

                   // First trail.
                   if ( mouse_in( VT_L_X + 3 , VT_L_Y + 3,
                                  VT_L_X + 35, VT_L_Y + 3)&&
                        (volhdr.aic >= 1) ){
                      volhdr.vol.volume0 =
                                   (( mc.x - VT_L_X - 3 ) * 256 - 1 ) / 32;
                      push_con_char(K_SOUND_REF);
                   }
                   else
                   // Second trail.
                   if ( mouse_in( VT_L_X + 3 , VT_L_Y + 6,
                                  VT_L_X + 35, VT_L_Y + 6)&&
                        (volhdr.aic >= 2) ){
                      volhdr.vol.volume1 =
                                   (( mc.x - VT_L_X - 3 ) * 256 - 1 ) / 32;
                      push_con_char(K_SOUND_REF);
                   }
                   else
                   // Third trail.
                   if ( mouse_in( VT_L_X + 3 , VT_L_Y + 9,
                                  VT_L_X + 35, VT_L_Y + 9)&&
                        (volhdr.aic >= 3) ){
                      volhdr.vol.volume2 =
                                   (( mc.x - VT_L_X - 3 ) * 256 - 1 ) / 32;
                      push_con_char(K_SOUND_REF);
                   }
                   else
                   // Forth sound trail.
                   if ( mouse_in( VT_L_X + 3 , VT_L_Y + 12,
                                  VT_L_X + 35, VT_L_Y + 12)&&
                        (volhdr.aic >= 4) ){
                      volhdr.vol.volume3 =
                                   (( mc.x - VT_L_X - 3 ) * 256 - 1 ) / 32;
                      push_con_char(K_SOUND_REF);
                   }
                }
                else
                //--------------------------------------
                // Progress bar is changed by mouse.
                if ( mouse_in( CT_L_X + 3, CT_R_Y - 2,
                               CT_R_X - 3, CT_R_Y - 2 )){
                   chg_pprog_add = ( mc.x - ( CT_L_X + 3 ) );
                   push_con_char(K_PPBAR_REF);
                }
                else
                //--------------------------------------
                // "Help" option on the buttom bar.
                if ( mouse_in( 3, 48, 9, 48 )){
                   push_con_mextend_char(F1);
                }
                else
                //--------------------------------------
                // "About" option on the buttom bar.
                if ( mouse_in( 13, 48, 20, 48 )){
                   push_con_mextend_char(F2);
                }
                else
                //--------------------------------------
                // "Register disk" option on the buttom bar.
                if ( mouse_in( 24, 48, 44, 48 )){
                   push_con_mextend_char(REG_MOUSE_DSK_O); // Imitation of
                }                                          // Alt + R.
                else
                //--------------------------------------
                // "Hotkeys" option on the buttom bar.
                if ( mouse_in( 48, 48, 62, 48 )){
                   push_con_mextend_char(ALT_H);
                }
                else
                //--------------------------------------
                // "Exit" option on the buttom bar.
                if ( mouse_in( 66, 48, 77, 48 )){
                   push_con_mextend_char(ALT_X);
                }
                break;

           case RUN_ERR_LAYER  :
                if ( mouse_in( MID_X - 2, MID_Y + 3, MID_X + 1, MID_Y + 3 )){
                   push_con_char(K_ENTER);
                }
                break;

           case LOGO_WINDOW_LAYER :
                if ( mouse_in( 24, 41, 33, 41 )){
                   push_con_char(K_ENTER);
                }
                else
                if ( mouse_in( 44, 41, 49, 41 )){
                   push_con_char(K_EXIT);
                }
                break;

           case NO_DISK_LAYER :
                if ( mouse_in( 7, 42, 12, 42 )){
                   push_con_char(K_ENTER);
                }
                else
                if ( mouse_in( 24, 42, 29, 42 )){
                   push_con_char(K_EXIT);
                }
                break;

           case STOP_A_CHOICE_LAYER :
                if ( mouse_in( 48, 40, 52, 40 )){
                   push_con_char('Y');
                }
                else
                if ( mouse_in( 65, 40, 68, 40 )){
                   push_con_char('N');
                }
                break;

           case EDIT_TAG_LAYER :
                break;

           case DELETE_TAG_LAYER :
                if ( mouse_in( RT_R_X - 12, RT_L_Y + 2,
                               RT_R_X - 3 , RT_L_Y + 2 )){
                   push_con_char(K_ENTER);
                }
                break;

           case HELP_WINDOW_LAYER :
                // "Backward" button.
                if ( mouse_in( 15, 41, 27, 41 )){
                   push_con_char('<');
                }
                else
                // "Quit help" button.
                if ( mouse_in( 35, 41, 45, 41 )){
                   push_con_char(K_EXIT);
                }
                else
                // "Forward" button.
                if ( mouse_in( 53, 41, 64, 41 )){
                   push_con_char('>');
                }
                break;

           case ABOUT_WINDOW_LAYER :
                if ( mouse_in( 38, 29, 41, 29 )){
                   push_con_char(K_ENTER);
                }
                break;

           case HOTKEYS_PAGE_LAYER :
                if ( mouse_in( 35, 41, 45, 41 )){
                   push_con_char(K_EXIT);
                }
                break;

    }
    return;
}

/****************************************************************************/

// What happens when right button is clicked.
void on_mouse_right_click(void)
    {
    // Each layer has distinctn methods.
    switch (M_layer) {
           case MAIN_LAYER     :
                // Track is blocked by mouse.
                if ( mouse_in( TT_L_X + 2, TT_L_Y + 7,
                               TT_R_X - 2, TT_R_Y - 5)){
                   // Check track existance.
                   if ( mc.y - ( TT_L_Y + 7 ) + 1 <= cd_data.max_track ){
                      k_move_finger( mc.y - ( TT_L_Y + 7 ) + 1 );
                      push_con_mextend_char(MARK_TRACK_KEY);
                   }
                }
                else
                //--------------------------------------
                // Change the play time direction.
                if ( mouse_in( CT_L_X + 24, CT_L_Y + 5,
                               CT_L_X + 28, CT_L_Y + 7 )){
                   push_con_char(K_TIME_DIR);
                }
                break;
    }
    return;
}

/****************************************************************************/

// What happens when center button is clicked.
void on_mouse_center_click(void)
    {
    return;
}

/****************************************************************************/

// What happens when left button is pressed and mouse drags something.
void on_mouse_left_drag(void)
    {
    // Each layer has distinctn methods.
    switch (M_layer) {

           case MAIN_LAYER     :
                //--------------------------------------
                // Track is selected by mouse.
                if ( mouse_in( TT_L_X + 2, TT_L_Y + 7,
                               TT_R_X - 2, TT_R_Y - 5)){
                   // Dragging occurs only when mc_y is changed.
                   if (has_mouse.moved_y){
                      // Check track existance.
                      if ( mc.y - ( TT_L_Y + 7 ) + 1 <= cd_data.max_track ){
                         k_move_finger( mc.y - ( TT_L_Y + 7 ) + 1 );
                      }
                   }
                }
                else
                //--------------------------------------
                // Dragging occurs not only when mc_x is changed, because
                // mouse can drag through different trail.
                // If mouse in equlizer.
                if ( mouse_in( VT_L_X + 3 , VT_L_Y + 3,
                               VT_L_X + 35, VT_L_Y + 12)){

                   // First trail.
                   if ( mouse_in( VT_L_X + 3 , VT_L_Y + 3,
                                  VT_L_X + 35, VT_L_Y + 3)&&
                        (volhdr.aic >= 1) ){
                      volhdr.vol.volume0 =
                                   (( mc.x - VT_L_X - 3 ) * 256 - 1 ) / 32;
                      push_con_char(K_SOUND_REF);
                   }
                   else
                   // Second trail.
                   if ( mouse_in( VT_L_X + 3 , VT_L_Y + 6,
                                  VT_L_X + 35, VT_L_Y + 6)&&
                        (volhdr.aic >= 2) ){
                      volhdr.vol.volume1 =
                                   (( mc.x - VT_L_X - 3 ) * 256 - 1 ) / 32;
                      push_con_char(K_SOUND_REF);
                   }
                   else
                   // Third trail.
                   if ( mouse_in( VT_L_X + 3 , VT_L_Y + 9,
                                  VT_L_X + 35, VT_L_Y + 9)&&
                        (volhdr.aic >= 3) ){
                      volhdr.vol.volume2 =
                                   (( mc.x - VT_L_X - 3 ) * 256 - 1 ) / 32;
                      push_con_char(K_SOUND_REF);
                   }
                   else
                   // Forth sound trail.
                   if ( mouse_in( VT_L_X + 3 , VT_L_Y + 12,
                                  VT_L_X + 35, VT_L_Y + 12)&&
                        (volhdr.aic >= 4) ){
                      volhdr.vol.volume3 =
                                   (( mc.x - VT_L_X - 3 ) * 256 - 1 ) / 32;
                      push_con_char(K_SOUND_REF);
                   }
                }

                break;

    }
    return;
}

/****************************************************************************/

// What happens when right button is pressed and mouse drags something.
void on_mouse_right_drag(void)
    {
    // Each layer has distinctn methods.
    switch (M_layer) {
           case MAIN_LAYER     :
                // Track is blocked by mouse.

                if ( mouse_in( TT_L_X + 2, TT_L_Y + 7,
                               TT_R_X - 2, TT_R_Y - 5)){
                   // Dragging occurs only when mc_y is changed.
                   if (has_mouse.moved_y){
                      // Check track existance.
                      if ( mc.y - ( TT_L_Y + 7 ) + 1 <= cd_data.max_track ){
                         k_move_finger( mc.y - ( TT_L_Y + 7 ) + 1 );
                         push_con_mextend_char(MARK_TRACK_KEY);
                      }
                   }
                }
                break;
    }
    return;
}

/****************************************************************************/

// What happens when center button is pressed and mouse drags something.
void on_mouse_center_drag(void)
    {
    // Each layer has distinctn methods.
    return;
}

/****************************************************************************/
/* EOF.*/