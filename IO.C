/****************************************************************************/
/**********************************  IO.C  **********************************/
/****************************************************************************/
// This file includes modules for all Mashitup basic indirect functions.
// IO.C is included in the project.
// IO.C size is XX.X KB.

/****************************************************************************/

#include "basic.h"

/****************************************************************************/
/**************************  Modules declarations  **************************/

                        /* String handeling mosduls */

// Returns the length of the argument string "src".
// Parameters    : src.
// Side affects  : None.
// Modules Calls : None.
unsigned length_of( char *src );

/****************************************************************************/

// Copies "size" bytes from "src" array to "dest" array.
// Even if the "src" and "dest" blocks overlap, the copy direction is chosen
// so that the data is always moved correctly.
// Parameters    : dest, src, size.
// Side affects  : None.
// Modules Calls : None.
void copyn_mem( void *dest, void *src, unsigned size );

/****************************************************************************/

// Copies "size" bytes from source array to destination array.
// source arrray given as SEG_s:OFF_s argumets and destination array is given
// as SEG_d:OFF_d arguments.
// Even if the SEG_s:OFF_s and SEG_d:OFF_d blocks overlap, the copy direction
// is chosen so that the data is always moved correctly.
// Parameters    : SEG_d, OFF_d, SEG_s, OFF_s, size.
// Side affects  : None.
// Modules Calls : None.
void far_copyn_mem( unsigned SEG_d, unsigned OFF_d,
                    unsigned SEG_s, unsigned OFF_s, unsigned size );

/****************************************************************************/

// Copies "src" string to "dest" string ( Including the '\0' ).
// Even if the "src" and "dest" blocks overlap, the copy direction is chosen
// so that the data is always moved correctly.
// Parameters    : dest, src.
// Side affects  : None.
// Modules Calls : lenght_of(), copyn_mem().
void copy_string( void *dest, void *src );

/****************************************************************************/

// Sets first "size" characters of "s" buffer to "c".
// Parameters    : s, c, size.
// Side affects  : None.
// Modules Calls : None.
void set_mem( void *s, unsigned char c, unsigned size );

/****************************************************************************/

// Compares 2 strings "s1" with "s2". Works exactly like strcmp().
// Returns:
//         þ <  0  if s1 <  s2
//         þ == 0  if s1 == s2
//         þ >  0  if s1 >  s2
// Parameters    : s1, s2.
// Side affects  : None.
// Modules Calls : None.
int cmp_strings( char *s1, char *s2 );

/****************************************************************************/

// Returns true if ch is bold digit.
// Parameters    : ch.
// Side affects  : None.
// Modules Calls : None.
boolean is_bold_digit( char ch );

/****************************************************************************/

// Returns true if ch is thick digit.
// Parameters    : ch.
// Side affects  : None.
// Modules Calls : None.
boolean is_thick_digit( char ch );

/****************************************************************************/

// Turns digit to be bold.
// Preconditions: digit is a thick character.
// Parameters    : digit.
// Side affects  : None.
// Modules Calls : None.
char bold_digit( char digit );

/****************************************************************************/

// Turns digit to be thick.
// Preconditions: digit is a bold character.
// Parameters    : digit.
// Side affects  : None.
// Modules Calls : None.
char thick_digit( char digit );

/****************************************************************************/

// Sets all the digits in argument string to be bold. And returns the string.
// Parameters    : string.
// Side affects  : None.
// Modules Calls : is_thick_digit().
char* set_bold_digits( char *string );

/****************************************************************************/

// Sets all the digits in argument string to be thick. And returns the string.
// Parameters    : string.
// Side affects  : None.
// Modules Calls : is_bold_digit().
char* set_thick_digits( char *string );

/****************************************************************************/

                         /* PC speaker's mosduls */

// Makes PC speaker sound according to sound argument.
// Parameters    : sound.
// Side affects  : PC speaker continues to make the sound even after the
//                 modul complets its operation.
// Modules Calls : None.
void pc_sound( unsigned int sound );

/****************************************************************************/

// Shuts up the PC speaker.
// Parameters    : None.
// Side affects  : None.
// Modules Calls : None.
void pc_no_sound(void);

/****************************************************************************/

                        /* Console handeling modules */

// Checks if keystroke is currently ready. If does returns True.
// Parameters    : None.
// Side affects  : None.
// Modules Calls : None.
boolean kbhit(void);

/****************************************************************************/

// Reads character from console buffer without echo to the screen.
// Parameters    : None.
// Side affects  : Returned character is deleted from the console buffer.
// Modules Calls : None.
char getch(void);

/****************************************************************************/

// Pops all the characters from consoles buffer until it is emptied.
// Parameters    : None.
// Side affects  : All previous keystrokes are lost.
// Modules Calls : kbhit(), getch().
void clear_console_buffer(void);

/****************************************************************************/

// Pushes the "key" character to the consoles buffer, ( next kbhit() will
// recognize it and getch() will pull it out.
// Parameters    : key.
// Side affects  : None.
// Modules Calls : None.
void push_con_char( char key );

/****************************************************************************/

// Pushes the "key" character to the consoles buffer, ( next kbhit() will
// recognize it and getch() will pull it out.
// Key is mouse extended key ( for keys like arrows = 077 ).
// Parameters    : key.
// Side affects  : None.
// Modules Calls : None.
void push_con_mextend_char( char key );

/****************************************************************************/
/****************************************************************************/
/*************************  Modules implementation  *************************/

                       /* String handeling mosduls */

// Returns the length of the argument string "src".
unsigned length_of( char *src )
    {
    asm {
        Mov Dx,Ds
        Mov Es,Dx
        Mov Di,src                 // Es:Di  <= Ds:src

        Xor Ax,Ax                  // String ends with Zero.
        Cld                        // Go down.
        Mov Cx,0FFFFH              // Maximal length 0FFFFH Bytes.
        Repnz ScaSB                // Find the end of the string.

        Xchg Cx,Ax
        Not Ax                     // Cx was decreased in each iteration.
        Dec Ax                     // To cover on 0FFFFH ( = -1 ).
    }
    return _AX;
}

/****************************************************************************/

// Copies "size" bytes from "src" array to "dest" array.
// Even if the "src" and "dest" blocks overlap, the copy direction is chosen
// so that the data is always moved correctly.
void copyn_mem( void *dest, void *src, unsigned size )
    {
    asm {

        Mov Ax,src               // Load addresses to fast registers.
        Mov Bx,dest
        Cmp Ax,Bx                // Compare addresses to define direction.
    JNB _Go_Inc

        Std                      // Copy from the end to the beginning.
        Mov Ax,0001H             // ( Ax == 1 ) => copy will be reversed.
    Jmp _Assign_arguments

    }

    _Go_Inc:

    asm {
        Cld                      // Copy from the beginning to the end.
        Xor Ax,Ax                // ( Ax == 0 ) => copy will be normal.
    }

    _Assign_arguments:

    asm {
        Mov Dx,Ds
        Mov Es,Dx
        Mov Si,src               // Load addresses to copy registers.
        Mov Di,dest
        Mov Cx,size              // Cx <= Size to copy.

        Or Ax,Ax
     JE _Test_parity             // If Ax == 1 Then copy is reversed.
        Add Si,Cx                // Update copy registers.
        Dec Si
        Add Di,Cx
        Dec Di
    }

    _Test_parity:

    asm {
        Test Di,0001H            // If adress of dest is odd then copy
     JE _Do_main_copy            // one byte, Because the main copy will
                                 // be MovSW ( by words ).
        MovSB
        Dec Cx                   // Decrease the loop counter.
    }

    _Do_main_copy:

    asm {
        Sub Si,Ax                // If copy is normal Si and Di will not be
        Sub Di,Ax                // changed. In case of reversive copy Si, Di
        Shr Cx,1                 // Will be decreased by 1 in order to enable
                                 // full 2 wors copy. Cx = Cx/2 because each
        Rep MovSW                // iteration copies 2 bytes ( word ).
    JNB _No_reminder:
        Add Si,Ax                // Move back to byte copy and not word copy
        Add Di,Ax                // If it is reversed. Otherwise do nothing.
        MovSB                    // Pick up the reminder.
    }

    _No_reminder:

    asm {
        Cld                      // Set increment copy direction.

    }
    return;
}

/****************************************************************************/

// Copies "size" bytes from source array to destination array.
// source arrray given as SEG_s:OFF_s argumets and destination array is given
// as SEG_d:OFF_d arguments.
// Even if the SEG_s:OFF_s and SEG_d:OFF_d blocks overlap, the copy direction
// is chosen so that the data is always moved correctly.
void far_copyn_mem( unsigned SEG_d, unsigned OFF_d,
                    unsigned SEG_s, unsigned OFF_s, unsigned size )
    {
    asm {
        Push Sp
        Push Bp
        Push Ds

        Mov Cx,SEG_d             // Load addresses to fast registers.
        Mov Bx,OFF_d
        Mov Dx,SEG_s
        Mov Ax,OFF_s             // Cx:Bx <= dest, Dx:Ax <= src.

        Push Cx                  // Compare addresses to define direction.

        Mov Ch,Al                // <Ä¿
        Mov Cl,04H               //   ³
        Shr Ax,Cl                //   ÃÄÄÄÄÄ
        Add Dx,Ax                //   ³
        Mov Al,Cl                // <ÄÙ

        Mov Ah,Bl                // <Ä¿
        Shr Bx,Cl                //   ³
        Pop Cx                   //   ÃÄÄÄÄÄ
        Add Cx,Bx                //   ³
        Mov Bl,Ah                // <ÄÙ

        And Ax,000FH
        And Bx,000FH

        Cmp Dx,Cx                // Full Segments.
    JNE _Decide_where

        Cmp Ax,Bx                // One nibble of offsets.
    }

    _Decide_where:

    asm {
    JNB _Go_Inc

        Std                      // Copy from the end to the beginning.
        Mov Ax,0001H             // ( Ax == 1 ) => copy will be reversed.
    Jmp _Assign_arguments

    }

    _Go_Inc:

    asm {
        Cld                      // Copy from the beginning to the end.
        Xor Ax,Ax                // ( Ax == 0 ) => copy will be normal.
    }

    _Assign_arguments:

    asm {
        Push SEG_d               // Load addresses to copy registers.
        Pop Es
        Push SEG_s
        Pop Ds
        Mov Di,OFF_d
        Mov Si,OFF_s
        Mov Cx,size              // Cx <= Size to copy.

        Or Ax,Ax
     JE _Test_parity             // If Ax == 1 Then copy is reversed.
        Add Si,Cx                // Update copy registers.
        Dec Si
        Add Di,Cx
        Dec Di
    }

    _Test_parity:

    asm {
        Test Di,0001H            // If adress of dest is odd then copy
     JE _Do_main_copy            // one byte, Because the main copy will
                                 // be MovSW ( by words ).
        MovSB
        Dec Cx                   // Decrease the loop counter.
    }

    _Do_main_copy:

    asm {
        Sub Si,Ax                // If copy is normal Si and Di will not be
        Sub Di,Ax                // changed. In case of reversive copy Si, Di
        Shr Cx,1                 // Will be decreased by 1 in order to enable
                                 // full 2 wors copy. Cx = Cx/2 because each
        Rep MovSW                // iteration copies 2 bytes ( word ).
    JNB _No_reminder:
        Add Si,Ax                // Move back to byte copy and not word copy
        Add Di,Ax                // If it is reversed. Otherwise do nothing.
        MovSB                    // Pick up the reminder.
    }

    _No_reminder:

    asm {
        Cld                      // Set increment copy direction.

        Pop  Ds
        Pop  Bp
        Pop  Sp
    }
    return;
}

/****************************************************************************/

// Copies "src" string to "dest" string ( Including the '\0' ).
// Even if the "src" and "dest" blocks overlap, the copy direction is chosen
// so that the data is always moved correctly.
void copy_string( void *dest, void *src )
    {
    unsigned size = length_of(src) + 1;

    copyn_mem( dest, src, size );

    return;
}

/****************************************************************************/

// Sets first "size" characters of "s" buffer to "c".
void set_mem( void *s, unsigned char c, unsigned size )
    {
    asm {
        Mov Dx,Ds
        Mov Es,Dx
        Mov Di,s                 // Es:Di <= Ds:s.

        Mov Al,c
        Mov Ah,Al                // Ax    <= 'cc'.
        Mov Cx,size
        Cld
        Test Di,0001H
     JE _Do_set

        StoSB                    // Odd address. Copy one byte to make
        Dec Cx                   // it even, and decrease the counter.
     }

     _Do_set:

     asm {
         Shr Cx,1
         Rep StoSW               // Set words.
     JNB _No_reminder

         StoSB                  // Set the remaining byte.
     }
     _No_reminder:

    return;
}

/****************************************************************************/

// Compares 2 strings "s1" with "s2". Works exactly like strcmp().
// Returns:
//         þ <  0  if s1 <  s2
//         þ == 0  if s1 == s2
//         þ >  0  if s1 >  s2
int cmp_strings( char *s1, char *s2 )
    {
    asm {

        Mov Dx,Ds
        Mov Es,Dx               // Es <- Ds

        Cld                     // Calculate the length of s2.
        Xor Ax,Ax
        Xor Bx,Bx
        Mov Si,s2
        Mov Di,Si
        Mov Cx,0FFFFH
        RepNZ ScaSB
        Not Cx                  // Cx = length_of(s2) + 1.

        Mov Si,s1               // Ds:Si <- Ds:s1
        Mov Di,s2               // Es:Di <- Ds:s2
        Rep CmpSB

        Mov Al,[Si-1]           // Subtract the differences.
        Mov Bl,[Di-1]
        Sub Ax,Bx
    }
    return _AX;
}

/****************************************************************************/

// Returns true if ch is bold digit.
boolean is_bold_digit( char ch )
    {
    if (((unsigned)ch >= 0 + ZERO_ASCII )&&
        ((unsigned)ch <= 9 + ZERO_ASCII )){
        return TRUE;
    }
    return FALSE;
}

/****************************************************************************/

// Returns true if ch is thick digit.
boolean is_thick_digit( char ch )
    {
    if (( ch >= '0' )&&
        ( ch <= '9' )){
        return TRUE;
    }
    return FALSE;
}

/****************************************************************************/

// Turns digit to be bold.
// Preconditions: digit is a thick character.
char bold_digit( char digit )
    {
    digit += ZERO_ASCII - '0';
    return digit;
}

/****************************************************************************/

// Turns digit to be thick.
// Preconditions: digit is a bold character.
char thick_digit( char digit )
    {
    digit -= ZERO_ASCII + '0';
    return digit;
}

/****************************************************************************/

// Sets all the digits in argument string to be bold. And returns the string.
char* set_bold_digits( char *string )
    {
    int i;
    int len = length_of(string);
    for ( i = 0; i < len; i++ ){
        if (is_thick_digit(string[i])){
           string[i] += ZERO_ASCII - '0';
        }
    }
    return string;
}

/****************************************************************************/

// Sets all the digits in argument string to be thick. And returns the string.
char* set_thick_digits( char *string )
    {
    int i;
    int len = length_of(string);
    for ( i = 0; i < len; i++ ){
        if (is_bold_digit(string[i])){
           string[i] -= ZERO_ASCII + '0';
        }
    }
    return string;
}

/****************************************************************************/

                         /* PC speaker's mosduls */

// Makes PC speaker sound according to sound argument.
void pc_sound( unsigned int sound )
    {
    asm {
        Mov ax ,34DDH
        Mov Dx ,0012H
        Mov Bx ,sound
        Div Bx
        Mov Bx ,Ax
        In  Al ,61H
    Test    Al,03H
    JNE _NE

        Or  Al ,03H
        Out 61H,Al
        Mov Al ,0B6H
        Out 43H,Al
    }
    _NE:
    asm {
        Mov Al ,Bl
        Out 42H,Al
        Mov Al ,Bh
        Out 42H,Al
    }
    return;
}

/****************************************************************************/

// Shuts up the PC speaker.
void pc_no_sound(void)
    {
    asm {
        In  Al ,61H
        And Al ,0FCH
        Out 61H,Al
    }
    return;
}

/****************************************************************************/

                        /* Console handeling modules */

// Checks if keystroke is currently ready. If does returns True.
boolean kbhit(void)
    {
    boolean is_kbhit;
    asm {
        Mov Ah,01H
        Int 16H
        Mov Al,FALSE
     JZ do_nothing
        Mov Al,TRUE
    }
    do_nothing:
    return _AL;
}

/****************************************************************************/

// Reads character from console buffer without echo to the screen.
char getch(void)
    {
    asm {
        Mov Ah,08H
        Int 21H
    }
    return _AL;
}

/****************************************************************************/

// Pops all the characters from consoles buffer until it is emptied.
void clear_console_buffer(void)
    {
    while (kbhit())
          getch();
    return;
}

/****************************************************************************/

// Pushes the "key" character to the consoles buffer, ( next kbhit() will
// recognize it and getch() will pull it out.
void push_con_char( char key )
    {
    asm {
        Mov Ah,05H
        Mov Cl,key
        Xor Ch,Ch
        Int 16H
    }
    return;
}

/****************************************************************************/

// Pushes the "key" character to the consoles buffer, ( next kbhit() will
// recognize it and getch() will pull it out.
// Key is mouse extended key ( for keys like arrows = 077 ).
void push_con_mextend_char( char key )
    {
    asm {
        Mov Ah,05H
        Mov Cl,K_MOUSE_EXTENDED
        Xor Ch,Ch
        Int 16H
        Mov Ah,05H
        Mov Cl,key
        Xor Ch,Ch
        Int 16H
    }
    return;
}

/****************************************************************************/
/* EOF. */
