/****************************************************************************/
/********************************  Visual.C  ********************************/
/****************************************************************************/
// This file includes modules for Mashitup interface.
// Visual.C is included in the project.
// Visual.C size is XX.X KB.

/****************************************************************************/

#include <stdlib.h>           // Needed for malloc() function.
#include <dos.h>              // Needed for gettime() function.
#include <stdio.h>            // Needed for FILE structure.
#include <stdarg.h>           // Needed for write_strings..() function.
#include "basic.h"

/***************************** Global variables *****************************/

extern cdrom_data_t cd_data;

extern int M_layer;
// Side affects : All functions that use the mouse sets M_layer variable to
// their layer. For example: MAIN_LAYER, HELP_WINDOW_LAYER, NO_DISK_LAYER...

extern int     selected_song;
extern boolean playing;

/****************************************************************************/
/**************************  modules declarations  ***************************/

// Terminates Mashitup. If code != 0 reports appropriate error.
// Parameters    : code.
// Side affects  : None.
// modules Calls : set_80_25_text_mode(), cursor_on(), free_mouse().
//                 Not crucial graphic functions.
void terminate( int code );

/****************************************************************************/
			  /* Font handle modules */

// Sets the ASCII table to the font from the file file_path.
// If file not found or font could not be read terminate() will be called.
// Parameters    : file_path.
// Side affects  : Turns screen into VGA ( 80X50 ) mode.
// modules Calls : terminate(), cursor_off().
void load_font( char *file_path );

/****************************************************************************/

// Copies the current ASCII table to font array.
// Parameters    : font.
// Side affects  : None.
// modules Calls : None.
void get_font( char font[ASCII_TABLE] );

/****************************************************************************/

// Sets the ASCII table to the font variable.
// Parameters    : font.
// Side affects  : Turns screen into VGA ( 80X50 ) mode.
// modules Calls : None.
void set_font( char font[ASCII_TABLE] );

/****************************************************************************/

// Changes the skin of Mashituip according to chosen skin number.
// 0            = Basic skin.
// 1..SKINS_NUM = Other skins.
// Parameters    : skin_num.
// Side affects  : 1. Turns screen into VGA ( 80X50 ) mode.
//                 2. Makes new mouse.
// modules Calls : run_time_errors(), free_mouse(), cursor_off(), new_mouse().
//                 Not crucial graphic functions.
void change_skin( int skin_num );

/****************************************************************************/

// Set normal text mode ( 80x25 ), defined as C80 in C language.
// Parameters    : None.
// Side affects  : None.
// modules Calls : None.
void set_80_25_text_mode(void);

/****************************************************************************/
			/* Indirect interface modules.*/

// Hides the cursor.
// Parameters    : None.
// Side affects  : None.
// modules Calls : None.
void cursor_off(void);

/****************************************************************************/

// Reveals the cursor.
// Parameters    : None.
// Side affects  : None.
// modules Calls : None.
void cursor_on(void);

/****************************************************************************/

// Saves the rectangle of screen segment which is represented by "c" to
// screen array. It is essential for restoration of the screen.
// If malloc of *screen fails terminate() will be called.
// Parameters    : c, screen.
// Side affects  : Heap memory is decreased.
// modules Calls : terminate() - on failure.
void get_screen( coords_t c, pscreen_array *screen );

/****************************************************************************/

// Restores the rectangle of screen segment which is represented by "c" from
// screen array.
// Parameters    : c, screen.
// Side affects  : Heap memory is increased back.
// modules Calls : None.
void put_screen( coords_t c, pscreen_array *screen );

/****************************************************************************/

// Performs fast character output using screen segment.
// Writes the character 'ch' with color 'color' on (x,y) coordinates.
// Parameters    : x, y, color, ch.
// Side affects  : None.
// modules Calls : None.
void write_char_xy( int x, int y, char color, unsigned char ch );

/****************************************************************************/

// Performs fast string output using screen segment.
// Writes the character 'ch' with color 'color' on (x,y) coordinates.
// Parameters    : x, y, color, ch.
// Side affects  : None.
// modules Calls : None.
void write_string_xy( int x, int y, char color, char *s );

/****************************************************************************/

// Performs fast strings output using screen segment. ... = List of strings
// That ends with NULL. for example ( x, y, col, "a", "123", "tc", NULL );
// Parameters    : x, y, color, ..., NULL.
// Side affects  : None.
// modules Calls : write_string_xy().
void write_strings_xy( int x , int y , char color, ... );

/****************************************************************************/

// Draws box with shadows. Return the screen part that is under the box.
// argument "c" includes the coordinates of the box, "color" its color and
// "caption" - its name that is displayed in the top of the box.
// Parameters    : c, color, caption.
// Side affects  : 1. Defocuses the mouse ( User see no mouse ).
//                 2. Heap memory is decreased ( see get_screen() ).
// modules Calls : defocus_mouse(), get_screen().
//                 Not crucial graphic functions.
pscreen_array draw_box( coords_t c, char color, char *caption );

/****************************************************************************/

// Restores the visual program state property of draw_box() preconditions.
// Cleans the box, returns the mouse and frees heap memory,
// argument "c" includes the coordinates of the box, and "screen" - the screen
// data that was covered by the box.
// Parameters    : c, screen.
// Side affects  : 1. Focuses the mouse ( User can use the mouse again ).
//                 2. Heap memory is increased back ( see put_screen() ).
// modules Calls : put_screen(), focus_mouse().
void destruct_box( coords_t c, pscreen_array *screen );

/****************************************************************************/

// Frees the box's screen array but not restores the visual preconditions
// of draw_box(). It is used in Mashitup but should be replaced by function
// that returns the screen argument to Mashitup main().
// Parameters    : screen.
// Side affects  : 1. Focuses the mouse ( User can use the mouse again ).
//                 2. Heap memory is increased back ( using stdlib.free() ).
// modules Calls : focus_mouse().
void destruct_box_vpc_psp( pscreen_array *screen );

/****************************************************************************/

// Draws button, and returns his hotkey.
// "c" means button coordinates, "color" - its color, "caption" - its caption.
// if "selected" is True then button is drawn as selected.
char draw_button( coord_t c, char color, char *caption, boolean selected );

/****************************************************************************/

// Draws pressable button, which indicates switch mode ( on/off ).
// "c" means button coordinates, "color" - its color, "caption" - its caption.
// if "pressed" is True then button is drawn without shadow and +1 to X
// button coordinate.
void draw_control_button( coord_t c, char color, char *caption,
                                                 boolean pressed );

/****************************************************************************/

// Makes sound for clicked button.
void click_sound(void);

/****************************************************************************/

// Imitates click on the selected button.
// "c" means button coordinates, "color" - its color, "caption" - its caption.
// "bg" is background of the button. This moduel draws the button as pressed
// and makes a click_sound().
void button_click( coord_t c, char color, char* caption, char bg );

/****************************************************************************/

// Draws box with message to the user in the center of the screen.
// argument "color" is color of the box, "caption" is the caption and
// "message" will be written inside the box. Also " OK " button will wait for
// user to click it and close the box.
// Box's coordinates are calculated according to the length of the message.
// Parameters    : color, caption, message.
// Side affects  : None.
// modules Calls : draw_box(), draw_button(), focus_mouse(), getch(),
//                 button_click(), defocus_mouse(), destruct_box().
//                 Not crucial graphic functions.
void message_box( char color, char *caption, char *message );

/****************************************************************************/

// Makes error sound.
void error_sound(void);

/****************************************************************************/

// Handles user's error that may be overcame by Mashitup, And warnings to
// the user ( with code > 100 ).
// Parameters    : code.
// Side affects  : 1. On unknown error may terminate Mashitup.
//                 2. Action which caused the error is stopped.
// modules Calls : message_box(), terminate()  - if unknown error occurs.
//                 Not crucial functions.
// Error codes table:
//         1 : Track is not audio.
//         2 : Specified track does not exists.
//         3 : Disk is still not accessible because of rotary delay.
//         4 : Unlocking while playing is forbidden.
//         5 : Unable to open locked CD-ROM.
//         6 : Disks' index file not found.
//         7 : Could not create disks' index file.
//         8 : Index file is corrupted.
//         9 : Registration file not found.
//        10 : Could not create registration file.
//        11 : Registration file is corrupted.
//        12 : Could not save disk information.
//        13 : Too much disks in registration database.
//        14 : External corruption in disk's database.
//        15 : Database has to be deleted! Use back up files.
//        16 : Could not backup the new registration.
//        17 : Cannot lock the track while playing it.
//        18 : Cannot play blocked track.
//        19 : Cannot continue to play blocked track.
//        21 : Not enough memory to load new skin.
//	  22 : Could not open the skin file.
//        23 : Skin file is corrupted.
//        27 : Help file not found.
//        28 : Help file is corrupted.
//
//    Others - Unknown Error.
//
// Warnings codes table:
//       101 : Nothing to delete - Registration file is empty.
//
void run_time_errors( int code );

/****************************************************************************/

// Makes the back ground of Mashitup, Including upper and bottom bars.
void make_back_ground(void);

/****************************************************************************/

// Imitates click on help option.
void bar_help_opt_click(void);

/****************************************************************************/

// Imitates click on about option.
void bar_about_opt_click(void);

/****************************************************************************/

// Imitates click on register disk option.
void bar_rdisk_opt_click(void);

/****************************************************************************/

// Imitates click on hotkeys help option.
void bar_hot_keys_opt_click(void);

/****************************************************************************/

// Imitates click on exit option on.
void bar_exit_opt_click(void);

/****************************************************************************/

// Creates 2 buttons for users decision. The second button is "exit"
// because it is used like this in the whole Mashitup.
// "c" stores coordinates of 2 buttons, "col1" & "col2" are colors of 2
// buttons, "capt1" & "capt2" are captions of 2 buttons respectively.
// "selected" defines which button is selected ( 1 or 2 ).
// Parameters    : c, col1, col2, capt1, capt2, selected.
// Side affects  : None.
// modules Calls : draw_button(), focus_mouse(), defocus_mouse(), getch().
int choose_button( coords_t c, char col1, char col2,
                               char *capt1, char *capt2, int selected );

/****************************************************************************/

// Performs the Mashitup's logo window.
void make_logo_window(void);

/****************************************************************************/

// Performs Mashitup "about" window.
void Mashitup_about(void);

/****************************************************************************/

// Gets the tinge of basic colors ( red, green, blue ) in col_no color code.
// Parameters    : col_no, r, g, b.
// Side affects  : None.
// modules Calls : None.
void get_pal( unsigned char col_no, char* r, char* g, char* b );

/****************************************************************************/

// Save the current colors palette. Colors palette has 256 colors and each
// one consists of 3 basic colors ( r, g, b ).
// Parameters    : pal.
// Side affects  : None.
// modules Calls : get_pal() - for each 0..255 colors.
void save_pal( unsigned char pal[256][3] );

/****************************************************************************/

// Sets the tinge of basic colors ( red, green, blue ) in col_no color code.
// Parameters    : col_no, r, g, b.
// Side affects  : None.
// modules Calls : None.
void set_pal( char col_no, char r, char g, char b );

/****************************************************************************/

// Returns True if electrons canon is in delay.
boolean is_ecanon_in_delay(void);

/****************************************************************************/

// Returns True if electrons cannon is currently painting.
boolean is_ecanon_busy(void);

/****************************************************************************/

// Waits until electron cannon will in the begining of its delay.
// If screen segment memory is suplied with new data while electron cannon is
// painting the monitor the picture becomes a little deformed. That is why
// Mashitup waits until cannon stops and then inserts the data into memory.
// When the cannon starts to paint again, it makes no deformations.
// Parameters    : None.
// Side affects  : A little delay in program execution.
// modules Calls : is_canon_in_delay(), is_canon_busy().
void retrace_ecanon(void);

/****************************************************************************/

// Creates the effect of screen enblacking ( fading out ).
// Max brightness is 63. This modules makes loop for all the colors ( 256 )
// and decreases its brightness by 1. After 64 Loops for each color, the
// color becomes black ( fade out ). ( 0..63 ) X ( 0.255 ) = 16384 iterations.
// Parameters    : None.
// Side affects  : None.
// modules Calls : retrace_ecanon(), get_pal(), set_pal().
void screen_fade_out(void);

/****************************************************************************/

// Zeros the ting of all basic colors. It turns screen black ( works like
// screen_fade_out but makes it immediately ).
void screen_im_out(void);

/****************************************************************************/

// Creates the effect of screen fading in. The pal argument is used to define
// the maximal brightness.
// Warks as screen_fade_out() but it increases the brightness each iteration.
// Parameters    : pal.
// Side affects  : None.
// modules Calls : retrace_ecanon(), get_pal(), set_pal().
void screen_fade_in( unsigned char pal[256][3] );

/****************************************************************************/

// Restores the palette to pal argument. ( works like screen_fade_in
// but makes it immediately ).
void screen_im_in( unsigned char pal[256][3] );

/****************************************************************************/

// Scroll the screen y pixels down ( text goes up ).
// Parameters    : y.
// Side affects  : All other programs ( like operation system ) will scroll
//                 their screen to because of the change of angle of ecanon.
// modules Calls : retrace_ecanon(), get_pal(), set_pal().
void scroll_p( char y );

/****************************************************************************/

// Scroll the screen one line down. All text goes up.
// How does it works? Its returns the canon to normal position, copies the
// screen segment data 1 line up and then moves the canon up 8 times for 8
// text pixels ( scroll down effect ).
// Parameters    : None.
// Side affects  : All other programs ( like operation system ) will scroll
//                 their screen to because of the change of angle of ecanon.
// modules Calls : retrace_ecanon(), scroll_p().
void scroll_screen_down(void);

/****************************************************************************/

// Scroll the screen one line up. All text goes down.
// How does it works? Its moves the canon up 8 times for 8 text pixels down
// ( scroll down effect ), returns the canon to normal position and then
// copies the screen segment data 1 line down and then.
// Parameters    : None.
// Side affects  : All other programs ( like operation system ) will scroll
//                 their screen to because of the change of angle of ecanon.
// modules Calls : retrace_ecanon(), scroll_p().
void scroll_screen_up(void);

/****************************************************************************/

// Creates screen saver of flowing water. After finishing restores the current
// skin back.
// Parameters    : Number of current skin.
// Side affects  : None.
// modules Calls : free_mouse(), get_screen(), put_screen(), load_font(),
//                 cursor_off(), new_mouse(), change_skin().
void screen_saver( int skin_num );

/****************************************************************************/

// Prepares the Mashitup interface.
void preparations(void);

/****************************************************************************/

// Changes the colors of y's screen line between c.l.x to c.r.x.
void mark_line( coords_t c, char color );

/****************************************************************************/

// Same as mark_line() but marks bar.
void mark_bar( coords_t c, char color );

/****************************************************************************/

// Fills c.l.y's screen line between c.l.x to c.r.x with ch characters.
void clear_line( coords_t c, char color, char ch );

/****************************************************************************/

// Same as clear_line() but clears bar.
void clear_bar( coords_t c, char color, char ch );

/****************************************************************************/

// Sets the cursors position to (x,y) coordinates.
void goto_xy( char x, char y );

/****************************************************************************/

// Imitates the scanf function but has more options. Starting line is "line",
// maximal line length is "bound" and key that made get_string to stop is
// returned by function.
// String is inputted starting at coordinates ( x, y ).
// The background of input field is drawn with bg_char.
// Parameters    : x, y, color, line, bound, bg_char.
// Side affects  : None.
// modules Calls : getch(), goto_xy(), write_char_xy(), e.t.c.
//                 Not crucial functions.
char get_string_xy( char x, char y, char color, char *line, char bound,
                    char bg_char );

/****************************************************************************/
			 /* Direct interface modules.*/

// Draw the finger which points on current song on album panel.
void draw_tracks_pointer( int track_num );

/****************************************************************************/

// Delete the finger which points on previous song on album panel.
void delete_tracks_pointer( int track_num );

/****************************************************************************/

// Returns the previous track and moves the finger to it.
int k_up_finger( int tracknum );

/****************************************************************************/

// Returns the next track and moves the finger to it.
int k_down_finger( int tracknum );

/****************************************************************************/

// Returns the first track and moves the finger to it.
int k_home_finger( int tracknum );

/****************************************************************************/

// Returns the last track and moves the finger to it.
int k_end_finger( int tracknum );

/****************************************************************************/

// Returns the track lower by 5 if exists and moves the finger to it.
// Otherwise returns the first track.
int k_pgup_finger( int tracknum );

/****************************************************************************/

// Returns the track higher by 5 if exists and moves the finger to it.
// Otherwise returns the maximal place.
int k_pgdown_finger( int tracknum );

/****************************************************************************/

// Sets track to tracknum and moves the finger to it. If that track does not
// exists, it does nothing.
// It is used by mouse procedures.
// If music is playing then module push_con_char() is called to force Mashitup
// to start playing the new selected song.
void k_move_finger( int tracknum );

/****************************************************************************/

// Write track, specified by tracknum to album panel.
void write_single_track( int tracknum, disk_tag_t *tag );

/****************************************************************************/

// Fills the information of album panel from disk tag.
void fill_album_panel( disk_tag_t *tag );

/****************************************************************************/

// Create the tracks panel. If "focused" argument is True album panel will
// not be defocused.
void display_album_panel( unsigned char selected_song, disk_tag_t *tag,
                          boolean focused );

/****************************************************************************/

// Makes album panel to be under the other panels in mutual coordinates.
void defocus_album_panel(void);

/****************************************************************************/

// Creates CD-ROM status panel. Returns cd_data.status.
// Parameters    : where.
// Side affects  : cd_data variable is appropriatly changed.
// modules Calls : cd_status(), draw_box(), destruct_box_vpc_psp().
//                 Not crucial graphic functions.
unsigned long display_cd_status_panel(void);

/****************************************************************************/

// Writes single sound trail specified by trailnum where its modifier is
// place according to trail_sound out of 0 - 255.
void write_sound_trail( int trailnum, unsigned trail_sound, boolean active );

/****************************************************************************/

// Writes all the four sound trails. Unimplemented trails will not be active.
// "aic" variable indicates the number of active channels.
void write_all_sound_trails( volumeinfo_t vol, int aic );

/****************************************************************************/

// Create volume control panel.
void display_equalizer( volume_handler volhdr );

/****************************************************************************/

// Make an empty playing progress bar.
void clear_play_progress_bar(void);

/****************************************************************************/

// Interprets the whole information of new disk, creates all the panels.
// "play_mode" defines the state of play_mode buttons, "volhdr" has
// information for equalizer. "tag" is used to extract tracks name e.t.c.
// "selected song" is currently selected_song ( With finger on it ).
// Parameters    : play_mode, volhdr, selected_song, tag.
// Side affects  : cd_data variable is appropriatly changed.
// modules Calls : cd_get_audio_info(), no_disk(), v_load_disk_tag(),
//                 display_album_panel(), display_equalizer(), draw_box(),
//                 display_cd_status_panel(),
//                 draw_control_button(), destruct_box_vpc_psp().
//                 Not crucial graphic functions.
unsigned long new_disk( unsigned char play_mode, volume_handler volhdr,
                        unsigned char selected_song, disk_tag_t *tag );

/****************************************************************************/

// Writes the progress bar according to passed time ( taken form music_info
// parameter ) out of total time ( taken from tag argument ).
// Parameters    : music_info, tag.
// Side affects  : None.
// modules Calls : write_char_xy().
void write_play_progress_bar( musicinfo_t *music_info, disk_tag_t *tag );

/****************************************************************************/

// Prints the track's play time and disk's play time.
// "time_direction" argument defines whether to write the elapsed or remaining
// time.
// Parameters    : music_info, time_direction, tag.
// Side affects  : cd_data.error is changed.
// modules Calls : cd_tell_pos().
//                 Not crucial graphic functions.
void print_play_time( musicinfo_t *music_info, play_time_t time_direction,
                      disk_tag_t *tag );

/****************************************************************************/

// Makes message that disk is still revolving.
void make_rdelay_message(void);

/****************************************************************************/

// Makes warning message that there is no disk in CD drive.
// Parameters    : None.
// Side affects  : cd_data variable is chenged.
// modules Calls : draw_box(), move_tray(), choose_button(), defocus_mouse(),
//                 destruct_box(), terminate() - if user wants to quit.
//                 cd_get_audio_info().
//                 Not crucial graphic functions.
void no_disk(void);

/****************************************************************************/

// Locks the CD-ROM drive and draws the "Lock" button as pressed.
// Returns True ( = cd is locked ).
// Parameters    : None.
// Side affects  : CD-Drive becomes locked.
// modules Calls : draw_control_button(), cd_lock().
boolean lock_disk(void);

/****************************************************************************/

// Unlocks the CD-ROM drive and draws the "Lock" button as released.
// Returns False ( = cd is not locked ).
// Parameters    : None.
// Side affects  : CD-Drive becomes unlocked.
// modules Calls : draw_control_button(), cd_lock().
boolean unlock_disk(void);

/****************************************************************************/

// Imitates click on "Rewind" button. This button cannot be pressed, only
// clicked for short period of time.
void click_rewind_button(void);

/****************************************************************************/

// Imitates click on "Play" button. This button cannot be pressed, only
// clicked for short period of time.
void click_play_button(void);

/****************************************************************************/

// Imitates click on "Stop" button. This button cannot be pressed, only
// clicked for short period of time.
void click_stop_button(void);

/****************************************************************************/

// Imitates click on "Forward" button. This button cannot be pressed, only
// clicked for short period of time.
void click_forward_button(void);

/****************************************************************************/

// Imitates click on "Eject" button. This button cannot be pressed, only
// clicked for short period of time.
void click_eject_button(void);

/****************************************************************************/

// Imitates clicks on "Repeat" and "Random" button.
void click_play_mode( int button );

/****************************************************************************/

// Imitates click on special hammer effect button. This button cannot be
// pressed, only clicked for short period of time.
void click_sphammer_button(void);

/****************************************************************************/

// Imitates click on special word up effect button. This button cannot be
// pressed, only clicked for short period of time.
void click_spwordup_button(void);

/****************************************************************************/

// Imitates click on backward 5 seconds button. This button cannot be
// pressed, only clicked for short period of time.
void click_backsec_button(void);

/****************************************************************************/

// Imitates click on forward 5 seconds button. This button cannot be
// pressed, only clicked for short period of time.
void click_forwsec_button(void);

/****************************************************************************/

// Draws released pause button.
void release_pause_button(void);

/****************************************************************************/

// Pauses and unpauses the music according to "paused" parameter. Draws the
// "Pause" button accordingly.
// Returns True if paused or False if unpaused.
// Parameters    : paused.
// Side affects  : cd_data variable is changed.
// modules Calls : cd_stop_music(), draw_control_button(), defocus_mouse().
//                 focus_mouse(), cd_resume_music().
//                 Not crucial modules.
boolean pause_song( boolean paused );

/****************************************************************************/

// Stops the audio. Works like pause but ends the current song, and returns
// FALSE to indicate that Mashitup is not playing.
// Parameters    : None.
// Side affects  : cd_data variable is changed.
// modules Calls : cd_stop_music().
//                 Not crucial graphic modules.
boolean stop_music(void);

/****************************************************************************/

// Asks user if he wills to stop the audio or not at exit.
void stop_audio_choice(void);

/****************************************************************************/
			 /* Disks database modules.*/

// Loads the disk tag with appropriate message to the user.
// "disk_id" argument is Id of the current disk, "tag" is where to load the
// information, "source_file" is from which file path to load.
// Returns True if loaded, otherwise - False.
// Parameters    : disk_id, tag, source_file.
// Side affects  : None.
// modules Calls : draw_box(), focus_mouse(), load_disk_tag(),
//                 defocus_mouse(), destruct_box().
//                 Not crucial graphic modules.
boolean v_load_disk_tag( char *disk_id, disk_tag_t *tag, char *source_file );

/****************************************************************************/

// Draw artist, album, year and genre input options from tag variable.
// It is draw from ( x, y ) coordinates.
void draw_album_info_for_editor( int x, int y, disk_tag_t *tag );

/****************************************************************************/

// Returns the color of track specified by "tracknum", according to track
// status ( T_BLOCKED, T_PLAYABLE, e.t.c ).
char edit_track_color( int tracknum, disk_tag_t *tag );

/****************************************************************************/

// Performs the disk tag editor.
// All the changes are updated in *tag identifier and first editing line
// is curopt wich represents the number of track.
// curopt table:
//               -4 : Edit disk's artist name.
//               -3 : Edit disk's album name.
//               -2 : Edit disk's production year.
//               -1 : Edit disk's music genre.
//    0.. max_track : Edit track with number of curopt + 1.
// Parameters    : tag, curopt.
// Side affects  : None.
// modules Calls : draw_box(), draw_album_info_for_editor(),
//                 edit_track_color(), get_string_xy(), destruct_box().
//                 Not crucial graphic modules.
void edit_disk_tag( disk_tag_t *tag, int curopt );

/****************************************************************************/

// Registers the disk with appropriate message to the user.
// "disk_id" argument is Id of the current disk, "tag" is the source of the
// information, "dest_file" is path for saving file.
// Returns True if saved, otherwise - False.
// Parameters    : disk_id, tag, source_file.
// Side affects  : None.
// modules Calls : draw_box(), focus_mouse(), register_disk(),
//                 defocus_mouse(), destruct_box().
//                 Not crucial graphic modules.
boolean v_register_disk( disk_tag_t *tag, char *disk_id, char *dest_file );

/****************************************************************************/

// Adds the "Deleted" mark to disk tag in the deleting editor.
// Argument ( x, y ) are coordinates of the mark.
void add_deleted_mark( char x, char y );

/****************************************************************************/

// Removes the "Deleted" mark in the deleting editor.
// Argument ( x, y ) are coordinates of the mark.
void remove_deleted_mark( char x, char y );

/****************************************************************************/

// Clears all tag's information in deleting editor for the next tag.
// Argument c defines a rectangle to clean.
void clear_tag_window( coords_t c );

/****************************************************************************/

// Inits tag of disk with default values, And returns it.
// Implementation is in Actual.C siurce file.
disk_tag_t init_disk_tag(void);

/****************************************************************************/

// Makes deleting editor for disk tags, and return True if it worked
// successfuly.
// Parameters    : None.
// Side affects  : None.
// modules Calls : draw_box(), run_time_errors(), destruct_box(), file_size(),
//                 focus_mouse(), defocus_mouse(), edit_track_color(),
//                 draw_album_info_for_editor(), draw_button()
//                 add_deleted_mark(), remove_deleted_mark(), getch(),
//                 clear_tag_window(),
//                 reorganize(), back_up_data(), reindex().
//                 Not crucial modules.
boolean delete_disk_tag(void);

/****************************************************************************/
			 /* Help database modules.*/

// Converts Help "topic" number to its literal topic -> "caption" argument.
void topic_to_caption( int topic, char *caption );

/****************************************************************************/

// Output information from Mashitup help file according to argument topic.
// Return True if loading and writing process terminated successfully.
boolean write_topic_info( int topic, char color );

/****************************************************************************/

// Deletes the information from help window.
// Argument "c" is rectangle to delete, color is with which color.
void delete_topic_info( coords_t c, char color );

/****************************************************************************/

// Adds page number in the right top according to the topic.
// Arguments ( x, y ) are ccordinates of page number and "color" is its color.
void add_help_page_num( char x, char y, int topic, char color );

/****************************************************************************/

// Performs the Mashitup's help window on argument topic.
// Parameters    : topic.
// Side affects  : None.
// modules Calls : topic_to_caption(), draw_box(), write_topic_info(),
//                 destruct_box(), add_help_page_num(), delete_topic_info(),
//                 focus_mouse(), defocus_mouse(), getch(), draw_button(),
//                 Not crucial modules.
void make_help_window( int topic );

/****************************************************************************/

// Make mashitup Help.
void Mashitup_help(void);

/****************************************************************************/

// Makes hotkeys separate page.
// Works almost as make_help_windwo(), but shows only one hotkeys page.
void hot_keys_page(void);

/****************************************************************************/
/****************************************************************************/
/*************************  modules implementation  *************************/

// Terminates Mashitup. If code != 0 reports appropriate error.
// Error code tables:
//         1 - Not enough heap memory.
//         2 - No CD-ROM drive.
//         3 - Font file not found.
//         4 - Font file is corrupted.
//    Others - Unknown Error.
void terminate( int code )
    {

    #define FERR "Fatal Error"
    #define _END 1                    // 0 = Fade out, 1 = Scrall Down.

#if   _END == 1
    char i;

#elif _END == 0
    unsigned char pal[256][3];
    save_pal(pal);
#endif

    if (code){
       error_sound();
    switch (code) {

	   case 0 : break;


	   case 1 : /*
                    message_box( ERR_COLS, FERR, "Not Enough Heap Memory!" );
                    */
		    break;


	   case 2 : message_box( ERR_COLS, FERR, "No CD-ROM Drive!" );
		    break;

	   case 3 : message_box( ERR_COLS, FERR,
				 "File: "FONT_FILE" Not Found!" );
		    break;

	   case 4 : message_box( ERR_COLS, FERR,
				 "File: "FONT_FILE" Is Corrupted!" );
		    break;

	   default: message_box( ERR_COLS, FERR, "Unknown Error!" );

    }
    } // if (code).

#if _END == 1
    free_mouse();
    for ( i = 0; i < 8; i++ ){
        retrace_ecanon();
        scroll_p(i);
    }
    for ( i = 0; i < 50; i++ ){
        scroll_screen_down();
    }
    set_80_25_text_mode();
    cursor_on();

#elif _END == 0
    screen_fade_out();
    set_80_25_text_mode();
    cursor_on();
    free_mouse();
    screen_im_in(pal);
#endif

    if ( code == 1 ){
       write_string_xy( 25, 0, ERR_COLS, "Not Enough Heap Memory!" );
       goto_xy( 0, 1 );
    }
    exit(code);
    return;
    #undef FERR
    #undef _END
}

/****************************************************************************/
			  /* Font handle modules */

// Sets the ASCII table to the font from the file file_path.
// If file not found or font could not be read terminate() will be called.
void load_font( char *file_path )
    {
    FILE *fl = fopen( file_path, "rb" );
    char ascii[ASCII_TABLE];

    // Read font from file if possible and report errors.
    if ( fl == NULL ){
       // Set vga screen.
       asm {
	   Mov Ax,1112H
	   Mov Bx,0800H
	   Int 10H
       }
       cursor_off();
       terminate(3);  // File not found.
    }

    if ( fread( ascii, ASCII_TABLE, 1, fl ) != 1 ){
       // Set vga screen.
       asm {
	   Mov Ax,1112H
	   Mov Bx,0800H
	   Int 10H
       }
       cursor_off();
       fclose(fl);
       terminate(4);  // File is corrupted - font could not be read.
    }

    // Set the font.
    asm {
	Push Bp
	Mov Ax,1110H           // Al = 10 => ( 1 = Set Mode, 0 = Set Font ).
	Mov Bx,0800H           // Bl = 8 Bytes * 8 Bits = 8x8 Fnt.
	Lea Bp,ascii
	Mov Dx,Ds              // Es:[Bp] => ascii
	Mov Es,Dx
	Mov Cx,256             // Cx = Ascii characters.
	Xor Dx,Dx              // Change from first character.
	Int 10H
	Pop Bp
    }

    if (fclose(fl))
       terminate(4);  // File is corrupted - could not be closed.
    return;
}

/****************************************************************************/

// Copies the current ASCII table to font array.
void get_font( char font[ASCII_TABLE] )
    {
    set_mem( font, 0, sizeof(font) );
    asm {
	Mov Ax,1130H
	Mov BH,6H
	Int 10H
	Lea Si,font
	Mov Cx,ASCII_TABLE
    }
    Cpy:
    asm {
	Mov DL     ,Es:[Bp]
	Mov Ds:[Si],DL
	Inc Bp
	Inc Si
     Loop Cpy
    }
    return;
}

/****************************************************************************/

// Sets the ASCII table to the font variable.
void set_font( char font[ASCII_TABLE] )
    {
    asm {
	Push Bp
	Mov Ax,1110H
	Mov Bx,0800H
	Lea Bp,font
	Mov Dx,Ds
	Mov Es,Dx
	Mov Cx,256
	Xor Dx,Dx
	Int 10H
	Pop Bp
    }
    return;
}

/****************************************************************************/

// Changes the skin of Mashituip according to chosen skin number.
// 0            = Basic skin.
// 1..SKINS_NUM = Other skins.
void change_skin( int skin_num )
    {
 // char *skin;
    char skin[ASCII_TABLE];
    FILE *fl;
    char file_path[40] = SKIN_FILE;

    unsigned char pal[256][3];

    if ( skin_num > SKINS_MAX_NUM ){
       // That skin does not exists.
       return;
    }

    save_pal(pal);

    // skin = malloc( ASCII_TABLE * sizeof(char));
    // Check if memory allocation was successful.
    if ( skin == NULL ){
       run_time_errors(21);    //  Not enough heap memory.
       return;
    }

    // Generate skins filename.
    //  "Skins\Mp0.Skn"
    file_path   [8] += skin_num;

    // Read skin from file if possible and report errors.
    fl = fopen( file_path, "rb" );
    if ( fl == NULL ){
       run_time_errors(22);    // Could not open the skin file.
    // free(skin);
       return;
    }
    if ( fread( skin, ASCII_TABLE, 1, fl ) != 1 ){
       run_time_errors(23);  // Skin file is corrupted.
    // free(skin);
       fclose(fl);
       return;
    }

    screen_fade_out();
    free_mouse();
    // Set the new skin.
    asm {
	Push Bp
	Mov Ax,1110H
	Mov Bx,0800H
	Lea Bp,skin
	Mov Dx,Ds
	Mov Es,Dx
	Mov Cx,256
	Xor Dx,Dx
	Int 10H
	Pop Bp
    }
 // free(skin);
    cursor_off();
    new_mouse();
    screen_fade_in(pal);

    // Close the file.
    if (fclose(fl))
       run_time_errors(23);  // Skin file is corrupted.
    return;
}

/****************************************************************************/

// Set normal text mode ( 80x25 ), defined as C80 in C language.
void set_80_25_text_mode(void)
    {
    asm {
        Push Bp
        Mov Ax,0003H
        Mov Bx,0000H
        Int 10H
        Pop Bp
    }
    return;
}

/****************************************************************************/
			/* Indirect interface modules.*/

// Hides the cursor.
void cursor_off(void)
    {
    asm {
	Mov Ah,1;
	Mov Ch,20H;
	Int 10H
    }
    return;
}

/****************************************************************************/

// Reveals the cursor.
void cursor_on(void)
    {
    asm {
	Mov Ah,1
	Mov Cx,0D0EH
	Int 10H
    }
    return;
}

/****************************************************************************/

// Saves the rectangle of screen segment which is represented by "c" to
// screen array. It is essential for restoration of the screen.
void get_screen( coords_t c, pscreen_array *screen )
    {
    int i, j;
    int dx = c.r.x - c.l.x + 1,
        dy = c.r.y - c.l.y + 1;

    *screen = malloc( dy * dx * sizeof(unsigned));
    if ( *screen == NULL )
       terminate(1);    //  Not enough heap memory.

    for ( j = 0; j < dy; j++ )
        for ( i = 0; i < dx; i++ )
            *( *screen + j * dx + i ) =
                                 *(unsigned far *)(SCREEN_SEG
                                 + ( i + c.l.x ) * 2 + ( j + c.l.y ) * 160 );

//  For each window allocate 8KB of memory.
//
//  *screen = malloc( MAX_X * MAX_Y * sizeof(unsigned));
//  if ( *screen == NULL )
//     terminate(1);    //  Not enough heap memory.

//  for ( i = c.l.x; i <= c.r.x; i++ )
//	for ( j = c.l.y; j<= c.r.y; j++ )
//	    (**screen)[i][j] = *(unsigned far *)(SCREEN_SEG +
//                                               i * 2 + j * 160 );

    return;
}

/****************************************************************************/

// Restores the rectangle of screen segment which is represented by "c" from
// screen array.
void put_screen( coords_t c, pscreen_array *screen )
    {
    int i, j;
    int dx = c.r.x - c.l.x + 1,
        dy = c.r.y - c.l.y + 1;

    for ( j = 0; j < dy; j++ )
        for ( i = 0; i < dx; i++ )
	    *(unsigned far *)(SCREEN_SEG
            + ( i + c.l.x ) * 2 + ( j + c.l.y ) * 160 ) =
            *( *screen + j * dx + i );

//  For each window allocate 8KB of memory.
//
//    for ( i = c.l.x; i <= c.r.x; i++ )
//	for ( j = c.l.y; j<= c.r.y; j++ )
//	    *(unsigned far *)(SCREEN_SEG + i*2 + j*160 ) = (**screen)[i][j];

    free(*screen);
    return;
}

/****************************************************************************/

// Performs fast character output using screen segment.
// Writes the character 'ch' with color 'color' on (x,y) coordinates.
void write_char_xy( int x, int y, char color, unsigned char ch )
    {
    *(unsigned far *)(SCREEN_SEG + x * 2 + y * 160 ) =
     ( color << 8 ) + (unsigned char)ch;
    return;
}

/****************************************************************************/

// Performs fast string output using screen segment.
// Writes the string 's' with color 'color' on (x,y) coordinates.
void write_string_xy( int x, int y, char color, char *s )
    {
    int i;

    for ( i = 0; i < length_of(s); i++ ){
	*(unsigned far *)(SCREEN_SEG + ( x + i ) * 2 + y * 160 ) =
	( color << 8 ) + (unsigned char)s[i];

    }
    return;
}

/****************************************************************************/

// Performs fast strings output using screen segment. ... = List of strings
// That ends with NULL. for example ( x, y, col, "a", "123", "tc", NULL );
void write_strings_xy( int x , int y , char color, ... )
    {
    va_list list;
    char *arg;
    int  new_x = x;

    va_start( list, color );
    while (( arg = va_arg( list, char* )) != NULL ){
	  write_string_xy( new_x , y , color , arg );
	  new_x += length_of(arg);
    }
    va_end(list);
    return;
}

/****************************************************************************/

// Draws box with shadows. Return the screen part that is under the box.
// argument "c" includes the coordinates of the box, "color" its color and
// "caption" - its name that is displayed in the top of the box.
pscreen_array draw_box( coords_t c, char color, char *caption )
    {
    int i, j, len = length_of(caption);
    pscreen_array psa = NULL;

    // Visual preconditions should not include the mouse cursor.
    defocus_mouse();

    // Catch the screen part under the box and its shadow.
    c.r.x += 2;
    c.r.y += 2;
    get_screen( c, &psa );
    c.r.x -= 2;
    c.r.y -= 2;

    // Draw the colored rectangle.
    for ( i = c.l.x; i <= c.r.x; i++ )
	for ( j = c.l.y; j<= c.r.y; j++ )
	    write_char_xy( i, j, color, K_SPACE );

    // Draw horizontal frames.
    for ( i = c.l.x + 1; i < c.r.x; i++ ){
	write_char_xy( i, c.l.y, color, HD_BAR );
	write_char_xy( i, c.r.y, color, HD_BAR );
    }

    // Draw vertical frames.
    for ( j = c.l.y + 1; j < c.r.y; j++ ){
	write_char_xy( c.l.x, j, color, VD_BAR );
	write_char_xy( c.r.x, j, color, VD_BAR );
    }

    // Draw corners.
    write_char_xy( c.l.x, c.l.y, color, LU_DCR );
    write_char_xy( c.l.x, c.r.y, color, LD_DCR );
    write_char_xy( c.r.x, c.l.y, color, RU_DCR );
    write_char_xy( c.r.x, c.r.y, color, RD_DCR );

    // Insert caption.
    i = ( c.r.x + c.l.x - len - 2 )/2;
    write_char_xy(   i++, c.l.y, color, LD_SEP );
    write_char_xy(   i++, c.l.y, color, K_SPACE );
    write_string_xy( i  , c.l.y, color, caption);
    i = ( c.r.x + c.l.x + len + 2 )/2;
    write_char_xy(   i++, c.l.y, color, K_SPACE );
    write_char_xy(   i++, c.l.y, color, RD_SEP );

    // Draw horizontal shadow.
    for ( i = c.l.x + 1; (i < c.r.x + 2)&&( i < MAX_X - 1 ); i++ ){
        if (( c.r.y + 4 ) < MAX_Y ){
	   *(unsigned far *)(SCREEN_SEG + (c.r.y+1) * 160 + (i+1) * 2 ) =
	   *(unsigned far *)(SCREEN_SEG + (c.r.y+1) * 160 + (i+1) * 2 )&
	    (0x00FF)|(0x0800);
	if (( c.r.y + 5 ) < MAX_Y )
	   *(unsigned far *)(SCREEN_SEG + (c.r.y+2) * 160 + (i+1) * 2 ) =
	   *(unsigned far *)(SCREEN_SEG + (c.r.y+2) * 160 + (i+1) * 2 )&
	    (0x00FF)|(0x0800);
        }

    }

    // Draw verical shadow.
    for ( j = c.l.y + 2; j < c.r.y+1; j++ ){
	if (( c.r.x + 1 ) < MAX_X ){
	   *(unsigned far *)(SCREEN_SEG + (c.r.x+1) * 2 + j * 160 ) =
	   *(unsigned far *)(SCREEN_SEG + (c.r.x+1) * 2 + j * 160 )&
	    (0x00FF)|(0x0800);
	if (( c.r.x + 2 ) < MAX_X )
	   *(unsigned far *)(SCREEN_SEG + (c.r.x+2) * 2 + j * 160 ) =
	   *(unsigned far *)(SCREEN_SEG + (c.r.x+2) * 2 + j * 160 )&
	    (0x00FF)|(0x0800);
	}
    }
    return psa;
}

/****************************************************************************/

// Restores the visual program state property of draw_box() preconditions.
// Cleans the box, returns the mouse and frees heap memory,
// argument "c" includes the coordinates of the box, and "screen" - the screen
// data that was covered by the box.
void destruct_box( coords_t c, pscreen_array *screen )
    {
    c.r.x += 2;
    c.r.y += 2;
    put_screen( c, screen );

    // Visual precondition of put_screen() in draw_box() included the mouse.
    focus_mouse();
    return;
}

/****************************************************************************/

// Frees the box's screen array but not restores the visual preconditions
// of draw_box(). It is used in Mashitup but should be replaced by function
// that returns the screen argument to Mashitup main().
void destruct_box_vpc_psp( pscreen_array *screen )
    {
    free(*screen);

    // Visual of put_screen() in draw_box() included the mouse cursor.
    focus_mouse();
    return;
}

/****************************************************************************/

// Draws button, and returns his hotkey.
// "c" means button coordinates, "color" - its color, "caption" - its caption.
// if "selected" is True then button is drawn as selected.
char draw_button( coord_t c, char color, char *caption, boolean selected )
    {
    int hot_key = (char*)strchr( caption, HOTKEY ) - (char*)caption;
    int i, len = length_of(caption);

    // Selected = white, Not selected = black.
    if (selected)
       color |= 0x0F;
    else
       color &= 0xF0;

    // Write caption until HOTKEY mark.
    for ( i = 0; i < hot_key; i++ )
	write_char_xy( c.x + i, c.y, color, caption[i] );

    // Write hotkey letter encolored with yellow.
    write_char_xy( c.x + i, c.y, (color&0xF0|0x0E), caption[ i + 1 ] );

    // Write the remainings of caption.
    for ( ++i; i < len - 1; i++ )
	write_char_xy( c.x + i, c.y, color, caption[ i + 1 ] );

    // Make shadow.
    for ( i = 1; i <= len - 1; i++)
	*(unsigned far *)(SCREEN_SEG + ( c.x + i ) * 2 + (c.y+1) * 160 ) =
	*(unsigned far *)(SCREEN_SEG + ( c.x + i ) * 2 + (c.y+1) * 160 )&
	(0xF000)|(unsigned char)UP_SHADOW;
    *(unsigned far *)(SCREEN_SEG + ( c.x + len - 1 ) * 2 + (c.y) * 160 ) =
    *(unsigned far *)(SCREEN_SEG + ( c.x + len - 1 ) * 2 + (c.y) * 160 )&
    (0xF000)|(unsigned char)DN_SHADOW;

    // Return the hotkey.
    return caption[ hot_key + 1 ];
}

/****************************************************************************/

// Draws pressable button, which indicates switch mode ( on/off ).
// "c" means button coordinates, "color" - its color, "caption" - its caption.
// if "pressed" is True then button is drawn without shadow and +1 to X
// button coordinate.
void draw_control_button( coord_t c, char color, char *caption,
                                                 boolean pressed )
    {
    int i, len = length_of(caption);

    // 2 different modes.
    if (pressed){
       c.x++;                             // Pressed to right.
       for ( i = 0; i < len; i++ )
	   write_char_xy( c.x + i, c.y, color, caption[i] );
    }
    else {
       for ( i = 0; i < len; i++ )
	   write_char_xy( c.x + i, c.y, color, caption[i] );

       // Make shadow.
       for ( i = 1; i <= len; i++)
	   *(unsigned far *)(SCREEN_SEG + ( c.x + i ) * 2 + (c.y+1) * 160 ) =
	   *(unsigned far *)(SCREEN_SEG + ( c.x + i ) * 2 + (c.y+1) * 160 )&
	   (0xF000)|(unsigned char)UP_SHADOW;
       *(unsigned far *)(SCREEN_SEG + ( c.x + len ) * 2 + (c.y) * 160 ) =
       *(unsigned far *)(SCREEN_SEG + ( c.x + len ) * 2 + (c.y) * 160 )&
       (0xF000)|(unsigned char)DN_SHADOW;
    }

    return;
}

/****************************************************************************/

// Makes sound for clicked button.
void click_sound(void)
    {
    int i;
    for ( i = 0; i < 30; i++ ){
	pc_sound(350);
	delay(2);
	pc_sound(6000);
	delay(2);
    }
    pc_no_sound();
}

/****************************************************************************/

// Imitates click on the selected button.
// "c" means button coordinates, "color" - its color, "caption" - its caption.
// "bg" is background of the button. This module draws the button as pressed
// and makes a click_sound().
void button_click( coord_t c, char color, char* caption, char bg )
    {
    int hot_key = (char*)strchr( caption, HOTKEY ) - (char*)caption;
    int i, len = length_of(caption);

    color |= 0x0F;  // White color.

    // Delete the button.
    for ( i = 0; i < len; i++ ){
	write_char_xy( c.x + i, c.y   ,  bg, K_SPACE );
	write_char_xy( c.x + i, c.y + 1, bg, K_SPACE );

    }
    c.x++;          // Clicked to right.

    // Write caption until HOTKEY mark.
    for ( i = 0; i < hot_key; i++ )
	write_char_xy( c.x + i, c.y, color, caption[i] );

    // Write hotkey letter encolored with yellow.
    write_char_xy( c.x + i, c.y, (color&0xF0|0x0E), caption[ i + 1 ] );

    // Write the remainings of caption.
    for ( ++i; i < len - 1; i++ )
	write_char_xy( c.x + i, c.y, color, caption[ i + 1 ] );
    click_sound();
    return;
}

/****************************************************************************/

// Draws box with message to the user in the center of the screen.
// argument "color" is color of the box, "caption" is the caption and
// "message" will be written inside the box. Also " OK " button will wait for
// user to click it and close the box.
// Box's coordinates are calculated according to the length of the message.
void message_box( char color, char *caption, char *message )
    {
    coords_t c;
    int len = length_of(message) + 2*5;
    char submit, hotkey;
    pscreen_array psa;
    char but_ok[] = " ^Ok ";

    // Calculate coordinates.
    c.l.x = MID_X - len / 2;
    c.r.x = MID_X + len / 2 - 1;
    c.l.y = MID_Y - 4;
    c.r.y = MID_Y + 6;

    // Draw thw message box.
    psa = draw_box( c, color, caption );
    write_string_xy( c.l.x + 5, c.l.y + 4, color, message );

    // Draw the Ok button.
    c.l.x = MID_X - ( length_of(but_ok) - 1 ) / 2;
    c.l.y += 4 + 3;
    hotkey = draw_button( c.l, BUTTON_COLS, but_ok, TRUE );

    focus_mouse();
    for ( submit = toupper(getch());
	  ( submit != hotkey )&&( submit != K_ENTER );
	  submit = toupper(getch()))
	;
    button_click( c.l, BUTTON_COLS, but_ok, color );

    // Restore the screen that is under the box.
    c.l.x = MID_X - len / 2;
    c.l.y = MID_Y - 4;

    defocus_mouse();
    destruct_box( c, &psa );

    return;
}


/****************************************************************************/

// Makes error sound.
void error_sound(void)
    {
    pc_sound(900);
    delay(300);
    pc_no_sound();
    return;
}

/****************************************************************************/

// Handles user's error that may be overcomed by Mashitup, And wornings to
// the user ( with code > 100 ).
void run_time_errors( int code )
    {
    // Set error layer.
    int M_prev = M_layer;
                 M_layer = RUN_ERR_LAYER;
    error_sound();

    switch (code) {

	   case 1  : message_box( ERR_COLS, "Run Time Error",
			          "Cannot play data track!" );
		     break;

	   case 2  : message_box( ERR_COLS, "Run Time Error",
                                  "Specified track does not exists!" );
		     break;

	   case 3  : message_box( ERR_COLS, "Run Time Error",
                                  "Disk is still not accessible!" );
		     break;

	   case 4  : message_box( ERR_COLS, "Run Time Error",
                                  "Unlocking While Playing Is Forbidden!" );
		     break;

	   case 5  : message_box( ERR_COLS, "Run Time Error",
                                  "Unable To Open Locked CD-ROM!" );
		     break;

	   case 6  : message_box( ERR_COLS, "Run Time Error",
                                  "Disks\' Index File Not Found!" );
		     break;

           // Occurs only after 6'th error.
	   case 7  : message_box( ERR_COLS, "Run Time Error",
                                  "Could Not Create Disks' Index File!" );
		     break;

	   case 8  : message_box( ERR_COLS, "Run Time Error",
                                  "Index File Is Corrupted!" );
		     break;

	   case 9  : message_box( ERR_COLS, "Run Time Error",
                                  "Registration File Not Found!" );
		     break;

           // Occurs only after 9'th error.
	   case 10 : message_box( ERR_COLS, "Run Time Error",
                                  "Could Not Create Registration File!" );
		     break;

	   case 11 : message_box( ERR_COLS, "Run Time Error",
                                  "Registration File Is Corrupted!" );
		     break;

	   case 12 : message_box( ERR_COLS, "Run Time Error",
                                  "Could Not Save Disk Information!" );
		     break;

	   case 13 : message_box( ERR_COLS, "Run Time Error",
                                 "Too Much Disks In Registration Database!" );
		     break;

	   case 14 : message_box( ERR_COLS, "Run Time Error",
			 	 "External Corruption In Disk's Database!" );
		     break;

	   case 15 : message_box( ERR_COLS, "Run Time Error",
                                  "Database Has To Be Deleted!  "
                                  "Use Back Up Files!" );
		     break;

	   case 16 : message_box( ERR_COLS, "Run Time Error",
                                  "Could Not Backup The New Registration!" );
		     break;

	   case 17 : message_box( ERR_COLS, "Run Time Error",
                                  "Cannot Lock The Track While Playing It!" );
		     break;

	   case 18 : message_box( ERR_COLS, "Run Time Error",
                                  "Cannot Play Blocked Track!" );
		     break;

	   case 19 : message_box( ERR_COLS, "Run Time Error",
                                  "Cannot Continue To Play Blocked Track!" );
		     break;

	   case 21 : message_box( ERR_COLS, "Run Time Error",
                                  "Not Enough Memory To Load New Skin!" );
		     break;

	   case 22 : message_box( ERR_COLS, "Run Time Error",
                                  "Could Not Open The Skin File!" );
		     break;

	   case 23 : message_box( ERR_COLS, "Run Time Error",
                                  "Skin File Is Corrupted!" );
		     break;

	   case 27 : message_box( ERR_COLS, "Run Time Error",
                                  "Help File Not Found!" );
		     break;

	   case 28 : message_box( ERR_COLS, "Run Time Error",
                                  "Help File Is Corrupted!" );
		     break;

           // Warnings:

           case 101: message_box( WARN_COLS, "Operation Canceled",
                                  "Registration File Is Empty!" );
		     break;

           default: terminate(-1);  // Unknown error.

    }

    // Set main layer.
    M_layer = M_prev;
    return;
}

/****************************************************************************/

// Makes the back ground of Mashitup, Including upper and bottom bars.
void make_back_ground(void)
    {
    int i, j;
    coords_t c;

    for ( i = 0; i < MAX_X; i++ )
	for ( j = 0; j < MAX_Y; j++ )
	    *(unsigned far *)(SCREEN_SEG + i * 2 + j * 160 ) =
	     ( MBG_COLS << 8 ) + (unsigned char)M_BGROUND;

    // Make top bar.
    write_strings_xy( MIN_X, MIN_Y, BLACK_ON_CYAN,
		     "<<<     Mashitup ",set_bold_digits(VERSION),
                     " - CD Player, Coded By Shmulyan Daniel (V.I.P),",
		     set_bold_digits(" 2001"),
                     "     >>>", NULL );  // Year, 2001 in big numbers.

    // Make down bar.
    c.l.x = MIN_X;
    c.r.x = MAX_X;
    c.l.y = MAX_Y - 1;
    clear_line( c, BLACK_ON_CYAN, HS_BAR );
    c.l.y = MAX_Y - 2;
    clear_line( c, BLACK_ON_CYAN, K_SPACE );
    c.l.y = MAX_Y - 3;
    clear_line( c, BLACK_ON_CYAN, HS_BAR );

    // Make corners.
    write_char_xy( MIN_X    , MAX_Y - 3, BLACK_ON_CYAN, LU_SCR );
    write_char_xy( MIN_X    , MAX_Y - 1, BLACK_ON_CYAN, LD_SCR );
    write_char_xy( MAX_X - 1, MAX_Y - 3, BLACK_ON_CYAN, RU_SCR );
    write_char_xy( MAX_X - 1, MAX_Y - 1, BLACK_ON_CYAN, RD_SCR );
    write_strings_xy( MIN_X, MAX_Y - 2, BLACK_ON_CYAN,
		     set_bold_digits("³  F¡-Help,"),
                     set_bold_digits("  F¢-About,"),
                     "  Alt+R - Register Disk,",
		     "  Alt+H - Hotkeys,  Alt+X - Exit ³", NULL);
    return;
}

/****************************************************************************/

// Imitates click on help option.
void bar_help_opt_click(void)
    {
    coords_t c;
    c.l.x = 3;
    c.r.x = 9;
    c.l.y = 48;
    c.r.y = 48;
    defocus_mouse();
    mark_line( c, RED_ON_GREEN );
    focus_mouse();
    click_sound();
    defocus_mouse();
    mark_line( c, BLACK_ON_CYAN );
    focus_mouse();
    return;
}

/****************************************************************************/

// Imitates click on about option.
void bar_about_opt_click(void)
    {
    coords_t c;
    c.l.x = 13;
    c.r.x = 20;
    c.l.y = 48;
    c.r.y = 48;
    defocus_mouse();
    mark_line( c, RED_ON_GREEN );
    focus_mouse();
    click_sound();
    defocus_mouse();
    mark_line( c, BLACK_ON_CYAN );
    focus_mouse();
    return;
}

/****************************************************************************/

// Imitates click on register disk option.
void bar_rdisk_opt_click(void)
    {
    coords_t c;
    c.l.x = 24;
    c.r.x = 44;
    c.l.y = 48;
    c.r.y = 48;
    defocus_mouse();
    mark_line( c, RED_ON_GREEN );
    focus_mouse();
    click_sound();
    defocus_mouse();
    mark_line( c, BLACK_ON_CYAN );
    focus_mouse();
    return;
}

/****************************************************************************/

// Imitates click on hotkeys help option.
void bar_hot_keys_opt_click(void)
    {
    coords_t c;
    c.l.x = 48;
    c.r.x = 62;
    c.l.y = 48;
    c.r.y = 48;
    defocus_mouse();
    mark_line( c, RED_ON_GREEN );
    focus_mouse();
    click_sound();
    defocus_mouse();
    mark_line( c, BLACK_ON_CYAN );
    focus_mouse();
    return;
}

/****************************************************************************/

// Imitates click on exit option.
void bar_exit_opt_click(void)
    {
    coords_t c;
    c.l.x = 66;
    c.r.x = 77;
    c.l.y = 48;
    c.r.y = 48;
    defocus_mouse();
    mark_line( c, RED_ON_GREEN );
    focus_mouse();
    click_sound();
    defocus_mouse();
    mark_line( c, BLACK_ON_CYAN );
    focus_mouse();
    return;
}

/****************************************************************************/

// Creates 2 buttons for users decision. The second button is "exit"
// because it is used like this in the whole Mashitup.
// "c" stores coordinates of 2 buttons, "col1" & "col2" are colors of 2
// buttons, "capt1" & "capt2" are captions of 2 buttons respectively.
// "selected" defines which button is selected ( 1 or 2 ).
int choose_button( coords_t c, char col1, char col2,
                               char *capt1, char *capt2, int selected )
   {
   int i;
   char key_1, key_2, submit;

   key_1 = draw_button( c.l, col1, capt1, !(selected - 1));
   key_2 = draw_button( c.r, col2, capt2,  (selected - 1));
   i = selected;

   focus_mouse();
   while ( i < 10 ){
	 submit = toupper(getch());
	 if ( submit == key_1 )
	    i = 11;
	 else
	 if ( submit == key_2 )
	    i = 12;
	 else
	 switch (submit) {

	        case K_EXIT      : i =  12;   // Exit.
				   break;

	        case K_ENTER     : i += 10;
				   break;

	        case K_EXTENDED  : submit = toupper(getch());
				   if (!((submit == K_RIGHT )||
				        (submit == K_LEFT   )||
				        (submit == K_SH_TAB )))
				      break;
	        case K_TAB       : i = !(i - 1) + 1; // 1 -> 2, 2 -> 1.
                                      break;
	 }
         defocus_mouse();
         if ( i < 10 ){
	    key_1 = draw_button( c.l, col1, capt1, !(i - 1));
	    key_2 = draw_button( c.r, col2, capt2, (i - 1));
         }
         else{
	    key_1 = draw_button( c.l, col1, capt1, !(i - 11));
	    key_2 = draw_button( c.r, col2, capt2, (i - 11));
         }
         focus_mouse();
   }
   return i - 10;
}

/****************************************************************************/

// Performs the Mashitup's logo window.
void make_logo_window(void)
    {
    pscreen_array screen;
    coords_t c;
    char color = WHITE_ON_CYAN ;
    int i = 2;

    // Set logo window layer.
    int M_prev = M_layer;
                 M_layer = LOGO_WINDOW_LAYER;

    // Draw the window
    c.l.x = 14;
    c.l.y = 3;
    c.r.x = 66;
    c.r.y = 44;
    screen = draw_box( c, color, "Welcome To Mashitop" );

    // Write the logo information.
    write_strings_xy( c.l.x + 13, c.l.y + i++, RED_ON_CYAN ,
		      "Mashitup ", set_bold_digits(VERSION), " - CD player.",
                      NULL );
    write_string_xy( c.l.x + 12, c.l.y + i++, RED_ON_CYAN ,
		     "~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    i++;
    write_string_xy( c.l.x + 3, c.l.y + i++, BLACK_ON_CYAN,
		     "Coded and designed By: Shmulyan Daniel (V.I.P). ");
    i++;
    write_strings_xy( c.l.x + 3, c.l.y + i++, BLACK_ON_CYAN,
		     "                       ", set_bold_digits(DATE), NULL );
    i++;
    write_string_xy( c.l.x + 3, c.l.y + i++, BLACK_ON_CYAN,
		     "EMail                : V.I.P@WriteMe.com");
    i++;
    write_string_xy( c.l.x + 3, c.l.y + i++, BLACK_ON_CYAN,
		     "This is a demonstration version of Mashitup,");
    write_string_xy( c.l.x + 3, c.l.y + i++, BLACK_ON_CYAN,
		     "presented as ¥ points final project.");
    i++;
    write_string_xy( c.l.x + 3, c.l.y + i++, BLUE_ON_CYAN,
		     "Feel free to destribute this copy of software.");

    i += 2;
    write_string_xy( c.l.x + 3, c.l.y + i++, BLACK_ON_CYAN,
		     "/^^\\       /^~/^~\\ /^~~~~\\      ,        ,");
    write_string_xy( c.l.x + 2, c.l.y + i++, BLACK_ON_CYAN,
		    "{    \\     /  |    Y       )    /(        )`");
    write_string_xy( c.l.x + 3, c.l.y + i++, BLACK_ON_CYAN,
		     "\\    \\   /   |    |  (~)   }   \\ \\___   / |");
    write_string_xy( c.l.x + 4, c.l.y + i++, BLACK_ON_CYAN,
		     "\\    \\ /    |    |       )    /- _  `-/  '");
    write_string_xy( c.l.x + 5, c.l.y + i++, BLACK_ON_CYAN,
		     "\\    Y    /|    |    Ú-'    (/\\/ \\ \\   /\\");
    write_string_xy( c.l.x + 6, c.l.y + i++, BLACK_ON_CYAN,
		     "\\       / |    A    |      / /   \\ `    \\");
    write_string_xy( c.l.x + 7, c.l.y + i++, BLACK_ON_CYAN,
		     "\\_____/   \\__/ \\__/       á á   / /   `)");

    write_string_xy( c.l.x +33, c.l.y + i++, BLACK_ON_CYAN,
		     "`-^--'`<     '");
    write_string_xy( c.l.x +32, c.l.y + i++, BLACK_ON_CYAN,
		     "(_.)  _  )   /");
    write_string_xy( c.l.x +33, c.l.y + i++, BLACK_ON_CYAN,
		     "`.___/`    /");
    write_string_xy( c.l.x + 2, c.l.y + i++, BLACK_ON_CYAN,
		     "/=================\\              `-----' /");
    write_string_xy( c.l.x + 2, c.l.y + i++, BLACK_ON_CYAN,
		     "}                 { <----.     __ / ,_   \\");
    write_strings_xy( c.l.x + 2, c.l.y + i++, BLACK_ON_CYAN,
		     "}  Mashitup ", set_bold_digits(VERSION),
                     "   { <----|====O)))==) \\) /====O", NULL );
    write_string_xy( c.l.x + 2, c.l.y + i++, BLACK_ON_CYAN,
		     "}                 { <----'    `--' `.__,' \\");
    write_string_xy( c.l.x + 2, c.l.y + i++, BLACK_ON_CYAN,
		     "\\=================/              |        |");
    write_string_xy( c.l.x +35, c.l.y + i++, BLACK_ON_CYAN,
		     "\\~~~~~~~~/");
    write_string_xy( c.l.x +31, c.l.y + i++, BLACK_ON_CYAN,
		     "  ___(  (   /\\____");
    write_string_xy( c.l.x +29, c.l.y + i++, BLACK_ON_CYAN,
		     "  ,'  ,---'   |     \\");
    write_string_xy( c.l.x +29, c.l.y + i++, BLACK_ON_CYAN,
		     "  `--{________)     \\/");

    // Make buttons and wait for user to proceed.
    {
     coords_t cb;
     char but_ok[] = " ^Activate ";
     char but_exit[] = " ^Exit ";

     cb.l.x = ( (c.r.x + c.l.x - length_of(but_ok) - 1) / 2) - 10;
     cb.l.y = c.r.y - 3;
     cb.r.x = ( (c.r.x + c.l.x - length_of(but_ok) - 1) / 2) + 10;
     cb.r.y = c.r.y - 3;
     i = 1;         // But_ok is selected.
     i = choose_button( cb, BUTTON_COLS, BUTTON_COLS, but_ok, but_exit, i );
     if ( i == 2 )
	button_click( cb.r, BUTTON_COLS, but_exit, color );
     else
	button_click( cb.l, BUTTON_COLS, but_ok, color );
    } // Sub block.

    // Restore the screen.
    defocus_mouse();
    destruct_box( c, &screen );

    M_layer = M_prev;
    if ( i == 2 )               // User wants to exit.
       terminate(0);
    return;
}

/****************************************************************************/

// Performs Mashitup "about" window.
void Mashitup_about(void)
    {
    pscreen_array screen;
    coords_t c;
    char submit, hotkey;
    char but_ok[] = " ^Ok ";

    // Set Mashitup about layer.
    int M_prev = M_layer;
                 M_layer = ABOUT_WINDOW_LAYER;

    // Draw the window
    c.l.x = 24;
    c.l.y = 13;
    c.r.x = 56;
    c.r.y = 32;
    screen = draw_box( c, WHITE_ON_BLUE, "About" );

    // Draw the information.
    write_string_xy( c.l.x + 12, c.l.y + 3, YELLOW_ON_BLUE, "Mashitup");
    write_strings_xy( c.l.x + 9, c.l.y + 6, WHITE_ON_BLUE,
		      "Version - ", set_bold_digits(VERSION), NULL );
    write_strings_xy( c.l.x + 5, c.l.y + 9, WHITE_ON_BLUE, "CD Player (c), ",
                      set_bold_digits("2001"), ", by", NULL );
    write_string_xy( c.l.x + 5, c.l.y + 12, WHITE_ON_BLUE,
                     "Shmulyan Daniel - V.I.P");

    // Draw the Ok button.
    c.l.x = MID_X - ( length_of(but_ok) - 1 ) / 2;
    c.l.y = 29;
    hotkey = draw_button( c.l, BUTTON_COLS, but_ok, TRUE );

    focus_mouse();
    for ( submit = toupper(getch());
	  ( submit != hotkey )&&( submit != K_ENTER );
	  submit = toupper(getch()))
	;
    button_click( c.l, BUTTON_COLS, but_ok, WHITE_ON_BLUE );

    // Restore the screen that is under the box.
    // Restore the screen.
    defocus_mouse();
    c.l.x = 24;
    c.l.y = 13;
    destruct_box( c, &screen );

    M_layer = M_prev;
    return;
}


/****************************************************************************/

// Gets the tinge of basic colors ( red, green, blue ) in col_no color code.
void get_pal( unsigned char col_no, char* r, char* g, char* b )
    {
    asm {
        // Output col_no
        Mov Al,col_no
        Mov Dx,03C7H
        Out Dx,Al

        // Get the tinge of red color.
        Xor Al,Al
        Mov Dx,03C9H
        In  Al,Dx
        Mov Bx,r
        Mov [Bx],Al

        // Get the tinge of green color.
        Xor Al,Al
        Mov Dx,03C9H
        In  Al,Dx
        Mov Bx,g
        Mov [Bx],Al

        // Get the tinge of blue color.
        Xor Al,Al
        Mov Dx,03C9H
        In  Al,Dx
        Mov Bx,b
        Mov [Bx],Al
    }
    return;
}

/****************************************************************************/

// Save the current colors palette. Colors palette has 256 colors and each
// one consists of 3 basic colors ( r, g, b ).
void save_pal( unsigned char pal[256][3] )
    {                      // save palette.
    int i;

    for ( i = 0; i < 256; i++ )
        get_pal( i, &pal[i][0], &pal[i][1], &pal[i][2] );

    return;
}


/****************************************************************************/

// Sets the tinge of basic colors ( red, green, blue ) in col_no color code.
void set_pal( char col_no, char r, char g, char b )
    {
    asm {
        // Output col_no
        Mov Al,col_no
        Mov Dx,03C8H
        Out Dx,Al

        // Set the tinge of red color.
        Mov Al,r
        Mov Dx,03C9H
        Out Dx,Al

        // Set the tinge of green color.
        Mov Al,g
        Mov Dx,03C9H
        Out Dx,Al

        // Set the tinge of blue color.
        Mov Al,b
        Mov Dx,03C9H
        Out Dx,Al
    }
    return;
}

/****************************************************************************/

// Returns True if electrons canon is in delay.
boolean is_ecanon_in_delay(void)
    {
    asm {
        Xor Al,Al
        Mov Dx,03DAH
        In  Al,Dx
        Xor Ah,Ah
        And Ax,0008H
    }
    return _AX;
}

/****************************************************************************/

// Returns True if electrons cannon is currently painting.
boolean is_ecanon_busy(void)
    {
    asm {
        Xor Al,Al
        Mov Dx,03DAH
        In  Al,Dx
        Xor Ah,Ah
        And Ax,0008H
    }
    return !_AX;
}

/****************************************************************************/

// Waits until electron cannon will in the begining of its delay.
// If screen segment memory is suplied with new data while electron cannon is
// painting the monitor the picture becomes a little deformed. That is why
// Mashitup waits until cannon stops and then inserts the data into memory.
// When the cannon starts to paint again, it makes no deformations.
void retrace_ecanon(void)
    {
    // Wait until the delay is over.
    while (is_ecanon_in_delay())
          ;

    // Wait until cannon stops.
    while (is_ecanon_busy())
          ;

    return;
}

/****************************************************************************/

// Creates the effect of screen enblacking ( fading out ).
// Max brightness is 63. This modules makes loop for all the colors ( 256 )
// and decreases its brightness by 1. After 64 Loops for each color, the
// color becomes black ( fade out ). ( 0..63 ) X ( 0.255 ) = 16384 iterations.
void screen_fade_out(void)
    {
    char b_colors[3];                                 // 3 Basic colors.
    int i,j;

    for ( i = 0; i < 64; i++ ){                       // Max brightness = 63.
        delay(FADE_DELAY);
        retrace_ecanon();

        for ( j = 0; j < 256; j++ ){                     // 256 Colors cells.
            get_pal( j, &b_colors[0], &b_colors[1], &b_colors[2] );

            // Decrease the tinge for all basic colors.
            if ( b_colors[0] > 0 )
               b_colors[0]--;
            if ( b_colors[1] > 0 )
               b_colors[1]--;
            if ( b_colors[2] > 0 )
               b_colors[2]--;

            set_pal( j, b_colors[0], b_colors[1], b_colors[2] );
        }
    }
    return;
}

/****************************************************************************/

// Zeros the ting of all basic colors. It turns screen black ( works like
// screen_fade_out but makes it immediately ).
void screen_im_out(void)
    {
    char b_colors[3];                                 // 3 Basic colors.
    int j;

    retrace_ecanon();
    for ( j = 0; j < 256; j++ ){                     // 256 Colors cells.
        get_pal( j, &b_colors[0], &b_colors[1], &b_colors[2] );

        // Zeroes the tinge for all basic colors.
        b_colors[0] = 0;
        b_colors[1] = 0;
        b_colors[2] = 0;

        set_pal( j, b_colors[0], b_colors[1], b_colors[2] );
    }
    return;
}

/****************************************************************************/

// Creates the effect of screen fading in. The pal argument is used to define
// the maximal brightness.
// Warks as screen_fade_out() but it increases the brightness each iteration.
void screen_fade_in( unsigned char pal[256][3] )
    {
    char b_colors[3];                                 // 3 Basic colors.
    int i,j;

    for ( i = 0; i < 64; i++ ){                       // Max brightness = 63.
        delay( FADE_DELAY * 4 );
        retrace_ecanon();

        for ( j = 0; j < 256; j++ ){                     // 256 Colors cells.
            get_pal( j, &b_colors[0], &b_colors[1], &b_colors[2] );

            // Increase the tinge for all basic colors.
            if ( b_colors[0] < pal[j][0] )
               b_colors[0]++;
            if ( b_colors[1] < pal[j][1] )
               b_colors[1]++;
            if ( b_colors[2] < pal[j][2] )
               b_colors[2]++;

            set_pal( j, b_colors[0], b_colors[1], b_colors[2] );
        }
    }
    return;
}

/****************************************************************************/

// Restores the palette to pal argument. ( works like screen_fade_in
// but makes it immediately ).
void screen_im_in( unsigned char pal[256][3] )
    {
    char b_colors[3];                                 // 3 Basic colors.
    int j;

    retrace_ecanon();
    for ( j = 0; j < 256; j++ ){                     // 256 Colors cells.
        get_pal( j, &b_colors[0], &b_colors[1], &b_colors[2] );

        // Restore the tinge for all basic colors.
        b_colors[0] = pal[j][0];
        b_colors[1] = pal[j][1];
        b_colors[2] = pal[j][2];

        set_pal( j, b_colors[0], b_colors[1], b_colors[2] );
    }
    return;
}

/****************************************************************************/

// Scroll the screen y pixels down ( text goes up ).
void scroll_p( char y )
    {
    asm {
        Mov  Dx,03D4H
        Mov  Ah,y
        Mov  Al,8
        Out  Dx,Ax
    }
    return;
}

/****************************************************************************/

// Scroll the screen one line down. All text goes up.
// How does it works? Its returns the canon to normal position, copies the
// screen segment data 1 line up and then moves the canon up 8 times for 8
// text pixels ( scroll down effect ).
void scroll_screen_down(void)
    {
    char i;

    retrace_ecanon();
    scroll_p(0);
    far_copyn_mem( 0xB800, 0, 0xB800, 160, 4160 * 2 );
    for ( i = 0; i < 8; i++ ){
        retrace_ecanon();
        scroll_p(i);
    }

    return;
}

/****************************************************************************/

// Scroll the screen one line up. All text goes down.
// How does it works? Its moves the canon up 8 times for 8 text pixels down
// ( scroll down effect ), returns the canon to normal position and then
// copies the screen segment data 1 line down and then.
void scroll_screen_up(void)
    {
    char i;

    for ( i = 8; i >= 0; i-- ){
        retrace_ecanon();
        scroll_p(i);
    }
    retrace_ecanon();
    scroll_p(8);
    far_copyn_mem( 0xB800, 160, 0xB800, 0, 4160 * 2 );
    return;
}

/****************************************************************************/

// Creates screen saver of flowing water. After finishing restores the current
// skin back.
void screen_saver( skin_num )
    int skin_num;
    {
    coords_t c;
    pscreen_array *screen = NULL;

    int  weather_sensor = 0;        // Defines whether waves will be made
                                    // or disappear.
    char wave_sensor    = 0;        // Makes the waves depth change.

    free_mouse();

    c.l.x = 0;
    c.l.y = 0;
    c.r.x = 79;
    c.r.y = 49;
    get_screen( c, screen );
    asm {
        Push Ds
        Push Bp

        Mov Dx,0A000H
        Mov Es,Dx         // Es <= Vga segment.
        Add Dx,Dx
        Mov Ds,Dx         // Ds <= 4000H

        Mov Ax,13H        // Set 13th mode.
        Int 10H

        Mov Dx,3C9H       // Palette changing port.

        Xor Bx,Bx         // Zero color as first palette color.
    }

    Set_Palette:          // Palette loop.

    asm {

        Xor Ax,Ax
        Out Dx,Al         // Output 0  as Red   color to the port.
        Out Dx,Al         // Output 0  as Green color to the port.

        Mov Al,Bl
        Mov Cl,2
        Shr Al,Cl
        Out Dx,Al         // Output Al as Blue color to the port.
        Inc Bx            // Set all the palette.

        Cmp Bx,0100H      // Palette = 256 Bytes = 0100H
     JB Set_Palette

        Xor Di,Di
        Xor Cx,Cx

    }

    Main_Loop:

    asm {
        Xor Bx,Bx
        Mov Dx,3H
        Mov Si,Di
    }

    SUX:

    asm {
        Inc Si
        Mov Al,Byte Ptr[ Si - 322 ]  // Matrix Calculations
        Add Bx,Ax
        Mov Al,Byte Ptr[ Si + 318 ]
        Add Bx,Ax
        Mov Al,Byte Ptr[ Si -   2 ]
        Add Bx,Ax

        Dec Dx
    JNZ SUX                          // Make 3x3 matrix size.

        Mov Al,Byte Ptr[ Di       ]

        Sub Bx,Ax

        Push Cx
        Mov Cl,2
        Shl Ax,Cl                    // Calculations for making the water.
        Sub Bx,Ax
        Shr Bx,Cl
        Mov Ax,Bx
        Mov Cl,7
        Shr Ax,cl
        Sub Bx,Ax
        Add Bl,Cs:wave_sensor
        Pop Cx

        Mov Ds:[Di],Bl               // Moving Point to the memory.
        Inc Di

   Loop Main_Loop                    // Loop for full screen.

        Mov Cx,0
        Dec Cx
        Rep MovSB                    // Moving data to screen.

        Add Cs:weather_sensor,1
        Cmp Cs:weather_sensor,070H
    JLE Check_key_pressed

        Mov Cs:weather_sensor,0
        Xor Cs:wave_sensor,01H
    }

    Check_key_pressed:

    asm {

  //    In Al,60H                    // Reading port 60H.
  //    Dec Al                       // ( Port 60H != 1 ) => Exit.

        Mov Ah,01H
        Int 16H
    JZ Main_Loop

        Mov Ax,03H                   // Text mode.
        Int 10H
        Pop Bp
        Pop Ds

    }

    put_screen( c, screen );
    load_font(FONT_FILE);
    if ( skin_num ){
       cursor_off();
       change_skin(skin_num);
    }
    cursor_off();
    new_mouse();
    return;
}
/****************************************************************************/

// Prepares the Mashitup interface.
void preparations(void)
    {
    unsigned char pal[256][3];
    save_pal(pal);

    load_font(FONT_FILE);
    cursor_off();
    screen_im_out();
    make_back_ground();
    new_mouse();
    screen_fade_in(pal);
    make_logo_window();
    return;
}

/****************************************************************************/

// Changes the colors of c.l.y's screen line between c.l.x to c.r.x.
void mark_line( coords_t c, char color )
    {
    int i;
    for ( i = c.l.x; i <= c.r.x; i++ ){
	*(unsigned far *)(SCREEN_SEG + (c.l.y) * 160 + (i) * 2 ) =
	*(unsigned far *)(SCREEN_SEG + (c.l.y) * 160 + (i) * 2 )&
	 (0x00FF)|(color<<8);
    }
    return;
}

/****************************************************************************/

// Same as mark_line() but marks bar.
void mark_bar( coords_t c, char color )
    {
    int i,j;
    for ( i = c.l.x; i <= c.r.x; i++ ){
        for ( j = c.l.y; j <= c.r.y; j++ ){
	    *(unsigned far *)(SCREEN_SEG + (j) * 160 + (i) * 2 ) =
	    *(unsigned far *)(SCREEN_SEG + (j) * 160 + (i) * 2 )&
	     (0x00FF)|(color<<8);
        }
    }
    return;
}


/****************************************************************************/

// Clears c.l.y's screen line between c.l.x to c.r.x.
void clear_line( coords_t c, char color, char ch )
    {
    int i;
    for ( i = c.l.x; i < c.r.x; i++ ){
	*(unsigned far *)(SCREEN_SEG + (c.l.y) * 160 + (i) * 2 ) =
	 (color<<8) + (unsigned char)ch;
    }
    return;
}

/****************************************************************************/

// Same as clear_line() but works on bar.
void clear_bar( coords_t c, char color, char ch )
    {
    int i,j;
    for ( j = c.l.y; j < c.r.y; j++ )
        for ( i = c.l.x; i < c.r.x; i++ ){
	    *(unsigned far *)(SCREEN_SEG + (j) * 160 + (i) * 2 ) =
	    (color<<8) + (unsigned char)ch;
    }
    return;
}

/****************************************************************************/

// Sets the cursors position to (x,y) coordinates.
void goto_xy( char x, char y )
    {
    asm {
        Mov Ah,2H
        Xor Bx,Bx
        Mov Dl,x
        Mov Dh,y
        Int 10H
    }
    return;
}

/****************************************************************************/

// Imitates the scanf function but has more options. Starting line is "line",
// maximal line length is "bound" and key that made get_string to stop is
// returned.
// String is inputted starting at coordinates ( x, y ).
// The background of input field is drawn with bg_char.
char get_string_xy( char x, char y, char color, char *line, char bound,
                    char bg_char )
    {
    char key;
    char curp;
    char i;
    char len = length_of(line);
    boolean extended;

    // Reveal the cursor.
    cursor_on();

    // Print parameter line and prepare the background.
    write_string_xy( x, y, color, line );
    for ( i = len; i < bound; i++ )
        write_char_xy( x + i, y, color, bg_char );

    // Init preconditions.
    curp = ( len < bound ) ? len : bound - 1;
    goto_xy( x + curp, y );
    key = getch();
    if ( key == K_EXTENDED ){
       extended = TRUE;
       key = getch();
    }
    else
       extended = FALSE;

    // Start editing loop.
    while ((key != K_EXIT)&&(key != K_ENTER )){

          len = length_of(line);    // For making less calculations.

          // Special exit conditions:
          if ( (extended)&&
               (( key == K_UP)||( key == K_DOWN ))){
             break; // Out of while.
          }

          if (extended){
             // Special key was pressed.
             switch (key){

                    case K_HOME :

                         curp = 0;
                         goto_xy( x + curp, y );
                         break;

                    case K_END :

                         if ( len < bound ){
                            curp = len;
                         }
                         else {
                            curp = bound - 1;
                         }
                         goto_xy( x + curp, y );
                         break;

                    case K_LEFT :

                         if ( curp > 0 ){
                            curp--;
                            goto_xy( x + curp, y );
                         }
                         break;

                    case K_RIGHT :

                         // Prevent stepping out of zone.
                         if (!(( curp == len        )||
                               ( curp == bound - 1 ))){
                            curp++;
                            goto_xy( x + curp, y );
                         }
                         break;

                    case K_DELETE :

                         if ( curp < len ){
                            for ( i = curp; i < len; i++ )
                                line[i] = line[i+1];
                            write_string_xy( x + curp, y, color,
                                             line + curp );
                            write_char_xy( x + len - 1, y, color, bg_char );
                            goto_xy( x + curp, y );
                         }
                         break;
             }
          }
          else

          switch (key){

                 case K_BACK_SPACE :

                          if ( curp > 0 ){
                             curp--;
                             for ( i = curp; i < len; i++ )
                                 line[i] = line[ i + 1 ];
                                 write_string_xy( x + curp, y, color,
                                                  line + curp );
                                 write_char_xy( x + len - 1, y, color,
                                                  bg_char );
                                 goto_xy( x + curp, y );
                          }
                          break;

                 case CTRL_Y:

                          // Clear line.
                          curp = 0;
                          goto_xy( x + curp, y );
                          for ( i = curp; i < len; i++ ){
                              write_char_xy( x + i, y, color, ' ');
                          }
                          line[0] = '\0';
                          goto_xy( x + curp, y );
                          break;

                 default :
                          if (( key !=K_EXIT )&&( key !=K_ENTER )&&
                              ( key !=K_TAB  )&&( key != CTRL_Y )&&
                              ( curp < bound )&&( len < bound   )&&
                              ( key > 20 )                      ){
                             for ( i = len + 1; i > curp; i-- )
                                 line[i] = line[i-1];

                             // Check if key is thick digit:
                             if (is_thick_digit(key))
                                key = bold_digit(key);
                             line[curp] = key;
                             write_string_xy( x + curp, y, color,
                                              line + curp );
                             if ( curp < bound - 1 )
                                curp++;
                             goto_xy( x + curp, y );
                          }
                          else {
                             if (( key == K_TAB )&&( curp < bound - 2 )&&
                                 ( len <bound-2 )                    ){
                                for ( i = len + 3; i >= curp; i-- )
                                    line[i] = line[i-3];
                                line[curp]   = ' ';
                                line[curp+1] = ' ';
                                line[curp+2] = ' ';
                                write_string_xy( x + curp, y, color,
                                          line + curp );

                                if ( curp < bound - 3 )
                                   curp += 3;
                                goto_xy( x + curp, y );
                             }
                          }
          }

          key = getch();
          if ( key == K_EXTENDED ){
             extended = TRUE;
             key = getch();
          }
          else
             extended = FALSE;

    } // While.

    // Hide the cursor and delete background chars.
    cursor_off();
    for ( i = length_of(line); i < bound; i++ )
        write_char_xy( x + i, y, color, K_SPACE );

    return key;
}

/****************************************************************************/
			 /* Direct interface modules.*/

// Draw the finger which points on current song on album panel.
void draw_tracks_pointer( int track_num )
    {
    write_string_xy( TT_L_X, TT_L_Y + track_num + 6, WHITE_ON_CYAN ,
                     TRACK_FINGER );
    return;
}

/****************************************************************************/

// Delete the finger which points on previous song on album panel.
void delete_tracks_pointer( int track_num )
    {
    write_string_xy( TT_L_X, TT_L_Y + track_num + 6, WHITE_ON_CYAN ,
                     EMPTY_FINGER );
    return;
}

/****************************************************************************/

// Returns the previous track and moves the finger to it.
int k_up_finger( int tracknum )
    {

    delete_tracks_pointer(tracknum);

    // Calculate tracknum.
    tracknum--;
    if ( tracknum < cd_data.min_track )
       tracknum = cd_data.max_track;

    draw_tracks_pointer(tracknum);
    click_rewind_button();

    return tracknum;
}

/****************************************************************************/

// Returns the next track and moves the finger to it.
int k_down_finger( int tracknum )
    {

    delete_tracks_pointer(tracknum);

    // Calculate tracknum.
    tracknum++;
    if ( tracknum > cd_data.max_track )
       tracknum = cd_data.min_track;

    draw_tracks_pointer(tracknum);
    click_forward_button();

    return tracknum;
}

/****************************************************************************/

// Returns the first track and moves the finger to it.
int k_home_finger( int tracknum )
    {

    delete_tracks_pointer(tracknum);

    // Calculate tracknum.
    tracknum = cd_data.min_track;

    draw_tracks_pointer(tracknum);
    click_rewind_button();

    return tracknum;
}

/****************************************************************************/

// Returns the last track and moves the finger to it.
int k_end_finger( int tracknum )
    {

    delete_tracks_pointer(tracknum);

    // Calculate tracknum.
    tracknum = cd_data.max_track;

    draw_tracks_pointer(tracknum);
    click_forward_button();

    return tracknum;
}

/****************************************************************************/

// Returns the track lower by 5 if exists and moves the finger to it.
// Otherwise returns the first track.
int k_pgup_finger( int tracknum )
    {

    delete_tracks_pointer(tracknum);

    // Calculate tracknum.
    tracknum -= 5;
    if ( tracknum < cd_data.min_track )
       tracknum = cd_data.min_track;

    draw_tracks_pointer(tracknum);
    click_rewind_button();

    return tracknum;
}

/****************************************************************************/

// Returns the track higher by 5 if exists and moves the finger to it.
// Otherwise returns the maximal place.
int k_pgdown_finger( int tracknum )
    {

    delete_tracks_pointer(tracknum);

    // Calculate tracknum.
    tracknum += 5;
    if ( tracknum > cd_data.max_track )
       tracknum = cd_data.max_track;

    draw_tracks_pointer(tracknum);
    click_forward_button();

    return tracknum;
}

/****************************************************************************/

// Sets track to tracknum and moves the finger to it. If that track does not
// exists, it does nothing.
// It is used by mouse procedures.
void k_move_finger( int tracknum )
    {
    if (( tracknum < cd_data.min_track )||
        ( tracknum > cd_data.max_track )){
       // No such track.
       return;
    }
    else {
       delete_tracks_pointer(selected_song);
       draw_tracks_pointer(tracknum);
       selected_song = tracknum;
    }
    // When song was changed start playing it again.
    if (playing)
       push_con_char(PLAY_BUTTON_KEY);

    return;
}

/****************************************************************************/

// Write track, specified by tracknum to album panel.
void write_single_track( int tracknum, disk_tag_t *tag )
    {
    coords_t c;
    unsigned char minutes, seconds, frames;
    char track_color;
    char track_name[T_NAME_LENGTH];
    int i;

    // Panel coordinates.
    c.l.x = TT_L_X;
    c.l.y = TT_L_Y + 6;
    c.r.x = TT_R_X;
    c.r.y = TT_R_Y;

    defocus_mouse();
    get_track_time_length( tracknum , &minutes, &seconds, &frames );
    if (is_track_playable( tracknum, tag )){
       track_color = BLACK_ON_CYAN;       // Playable.
       write_char_xy( c.r.x, c.l.y + tracknum, WHITE_ON_CYAN , VD_BAR );
    }
    else
    if (is_track_blocked( tracknum, tag )){
       track_color = BROWN_ON_CYAN;       // blocked.
       write_char_xy( c.r.x, c.l.y + tracknum, WHITE_ON_CYAN ,
                      BLOCKED_POINT );
    }
    else {
       track_color = DGRAY_ON_CYAN;       // data.
       write_char_xy( c.r.x, c.l.y + tracknum, WHITE_ON_CYAN , DATA_POINT );
    }

    // Write the name, time e.t.c.
    copy_string( track_name, extract_track_name( tag, tracknum ));
    write_string_xy( c.l.x + 2, c.l.y + tracknum, track_color, track_name );
    {
     char tim[6];
     tim[0] = ('0' + (minutes / 10 ));
     tim[1] = ('0' + (minutes % 10 )); tim[2] = ':';
     tim[3] = ('0' + (seconds / 10 ));
     tim[4] = ('0' + (seconds % 10 )); tim[5] = '\0';
     write_strings_xy( c.l.x +  T_NAME_LENGTH,
		      c.l.y + tracknum, track_color, " - ", tim, NULL );
    }
    if ( cd_data.track_type == DATA_TRACK )
       write_string_xy( c.l.x + 8 + T_NAME_LENGTH,
			c.l.y + tracknum, track_color, " -Data" );
    else
       write_string_xy( c.l.x + 8 + T_NAME_LENGTH,
			c.l.y + tracknum, track_color, " -Audio" );
    focus_mouse();
    return;
}

/****************************************************************************/

// Fills the information of album panel from disk tag.
void fill_album_panel( disk_tag_t *tag )
    {
    coords_t c;
    int i, j;

    // Init album panel coordinates and change the caption.
    {
     char caption[40] = {0};
     int len;

     // Make caption of "artist - (year)" token.
     strcat( caption , extract_album(tag));
     strcat( caption , " - (" );
     strcat( caption , extract_year(tag));
     strcat( caption , ")" );

     c.l.x = TT_L_X;
     c.l.y = TT_L_Y;
     c.r.x = TT_R_X;
     c.r.y = TT_R_Y;

     // Delete the remainigs of previous caption and write a new one.
     len = length_of(caption);
     for ( j = 0; j < len; j++ )
         write_char_xy( j + ( c.l.x + c.r.x )/2 - len/2 + 1, c.l.y,
                        WHITE_ON_CYAN , K_SPACE + 4 );
     write_string_xy( ( c.l.x + c.r.x )/2 - len/2 + 1, c.l.y,
                      WHITE_ON_CYAN , caption );
    }

    // Write artist name.
    {
     char temp[A_NAME_LENGTH];
     copy_string( temp, extract_artist(tag));
     write_string_xy( c.l.x + 2, c.l.y + 2, RED_ON_CYAN , "Artist : " );

     write_string_xy( c.l.x + 11, c.l.y + 2, BLACK_ON_CYAN, temp );
     copy_string( temp, extract_genre(tag));
     write_string_xy( c.l.x + 2, c.l.y + 4, BLACK_ON_CYAN, "Genre  : " );
     write_string_xy( c.l.x + 11, c.l.y + 4, BLACK_ON_CYAN, temp );
    }

    write_char_xy( c.l.x, c.l.y + 5, WHITE_ON_CYAN , RM_SEP );
    write_char_xy( c.r.x, c.l.y + 5, WHITE_ON_CYAN , LM_SEP );

    for ( i = c.l.x + 1; i < c.r.x; i++ )
	write_char_xy( i, c.l.y + 5, WHITE_ON_CYAN , HS_BAR );

    // Write track's information.
    if ( cd_data.max_track > MAX_TRACKS )
       cd_data.max_track = MAX_TRACKS;
    c.l.y += 6;
    for ( i = cd_data.min_track; i <= cd_data.max_track; i++ ){
        write_single_track( i, tag );
        // Delete the remainigs of previous name.
        for ( j = c.l.x +2+ length_of(tag->tracks[i-1].name);
              j < T_NAME_LENGTH - 1; j++ )
            write_char_xy( j , c.l.y + i, BLACK_ON_CYAN, K_SPACE );
    }
    c.l.y -= 4;

    // Write disk length.
    {
     char tim[6];
     tim[0] = ('0' + (cd_data.disk_length_min / 10 ));
     tim[1] = ('0' + (cd_data.disk_length_min % 10 )); tim[2] = ':';
     tim[3] = ('0' + (cd_data.disk_length_sec / 10 ));
     tim[4] = ('0' + (cd_data.disk_length_sec % 10 )); tim[5] = '\0';
     write_string_xy( c.l.x + 2, c.r.y - 2, BLACK_ON_CYAN, "Total :" );
     write_char_xy( c.l.x, c.r.y - 3, WHITE_ON_CYAN , RM_SEP );
     write_char_xy( c.r.x, c.r.y - 3, WHITE_ON_CYAN , LM_SEP );
     for ( i = c.l.x + 1; i < c.r.x; i++ )
	 write_char_xy( i, c.r.y - 3, WHITE_ON_CYAN , HS_BAR );
     write_string_xy( c.l.x + 2, c.r.y - 2, RED_ON_CYAN , "Total  :" );
     write_string_xy( c.l.x + 3 + T_NAME_LENGTH, c.r.y - 2, BLACK_ON_CYAN,
                      tim );
    }
    return;
}

/****************************************************************************/

// Create the tracks panel. If "focused" argument is True album panel will
// not be defocused.
void display_album_panel( unsigned char selected_song, disk_tag_t *tag,
                          boolean focused )
    {
    coords_t c;
    pscreen_array psa;
    int i;

    {
     char caption[40] = {0};

     // Make caption of "artist - (year)" token.
     strcat( caption , extract_album(tag));
     strcat( caption , " - (" );
     strcat( caption , extract_year(tag));
     strcat( caption , ")" );

     c.l.x = TT_L_X;
     c.l.y = TT_L_Y;
     c.r.x = TT_R_X;
     c.r.y = TT_R_Y;
     psa = (pscreen_array)draw_box( c , WHITE_ON_CYAN  , caption );
    }
    if (!focused)
       defocus_album_panel();

    // Write tag information.
    // Write artist name.
    {
     char temp[A_NAME_LENGTH];
     copy_string( temp, extract_artist(tag));
     write_string_xy( c.l.x + 2, c.l.y + 2, RED_ON_CYAN , "Artist : " );
     write_string_xy( c.l.x + 11, c.l.y + 2, BLACK_ON_CYAN, temp );
     copy_string( temp, extract_genre(tag));
     write_string_xy( c.l.x + 2, c.l.y + 4, BLACK_ON_CYAN, "Genre  : " );
     write_string_xy( c.l.x + 11, c.l.y + 4, BLACK_ON_CYAN, temp );
    }

    write_char_xy( c.l.x, c.l.y + 5, WHITE_ON_CYAN , RM_SEP );
    write_char_xy( c.r.x, c.l.y + 5, WHITE_ON_CYAN , LM_SEP );

    for ( i = c.l.x + 1; i < c.r.x; i++ )
	write_char_xy( i, c.l.y + 5, WHITE_ON_CYAN , HS_BAR );

    // Write track's information.
    if ( cd_data.max_track > MAX_TRACKS )
       cd_data.max_track = MAX_TRACKS;
    c.l.y += 6;
    for ( i = cd_data.min_track; i <= cd_data.max_track; i++ ){
        write_single_track( i, tag );
    }
    c.l.y -= 4;

    // Write disk length.
    {
     char tim[6];
     tim[0] = ('0' + (cd_data.disk_length_min / 10 ));
     tim[1] = ('0' + (cd_data.disk_length_min % 10 )); tim[2] = ':';
     tim[3] = ('0' + (cd_data.disk_length_sec / 10 ));
     tim[4] = ('0' + (cd_data.disk_length_sec % 10 )); tim[5] = '\0';
     write_string_xy( c.l.x + 2, c.r.y - 2, BLACK_ON_CYAN, "Total :" );
     write_char_xy( c.l.x, c.r.y - 3, WHITE_ON_CYAN , RM_SEP );
     write_char_xy( c.r.x, c.r.y - 3, WHITE_ON_CYAN , LM_SEP );
     for ( i = c.l.x + 1; i < c.r.x; i++ )
	 write_char_xy( i, c.r.y - 3, WHITE_ON_CYAN , HS_BAR );
     write_string_xy( c.l.x + 2, c.r.y - 2, RED_ON_CYAN , "Total  :" );
     write_string_xy( c.l.x + 3 + T_NAME_LENGTH, c.r.y - 2, BLACK_ON_CYAN,
                      tim );
    }
    // Make index of currently selected song.
    draw_tracks_pointer(selected_song);

    // Mashitup Does not need the psa array any more.
    // For future developments: new_disk() returns psa, instead of free it.
    destruct_box_vpc_psp(&psa);
    return;
}

/****************************************************************************/

// Makes album panel to be under the other panels in mutual coordinates.
void defocus_album_panel(void)
    {
    int i;

    // Make volume panel on top.
    defocus_mouse();
    for ( i = VT_L_Y; i <= VT_R_Y; i++ ){
	*(unsigned far *)(SCREEN_SEG + VT_L_X * 2 + i * 160 ) =
	*(unsigned far *)(SCREEN_SEG + VT_L_X * 2 + i * 160 )&
	 (0x00FF)|(0x3F00);
        ;
    }

    // Make control panel on top.
    for ( i = CT_L_Y; i <= CT_R_Y; i++ ){
	*(unsigned far *)(SCREEN_SEG + CT_L_X * 2 + i * 160 ) =
	*(unsigned far *)(SCREEN_SEG + CT_L_X * 2 + i * 160 )&
	 (0x00FF)|(0x3F00);
        ;
    }
    focus_mouse();
    return;
}

/****************************************************************************/

// Creates CD-ROM status panel. Returns cd_data.status.
unsigned long display_cd_status_panel(void)
    {
    unsigned char diskid[6];
    pscreen_array psa;
    coords_t c;

    // Get cd status.
    cd_status();

    c.l.x = ST_L_X;
    c.l.y = ST_L_Y;
    c.r.x = ST_R_X;
    c.r.y = ST_R_Y;
    psa = (pscreen_array)draw_box( c, WHITE_ON_CYAN , "Status" );

    // Fill status window with information.
    // Look at the cd_data.status documentation table.
    write_strings_xy( c.l.x + 2, c.l.y + 2, BLACK_ON_CYAN,"CD         : ",
		      (cd_data.status & 8)?"Writable":"Read Only", NULL );
    write_strings_xy( c.l.x + 2, c.l.y + 3, BLACK_ON_CYAN,"Door       : ",
		      (cd_data.status & 1)?"Open":"Closed", NULL );
    write_strings_xy( c.l.x + 2, c.l.y + 4, BLACK_ON_CYAN,"Audio      : ",
		      (cd_data.status & 256)?"Channel control":
					     "No channel control", NULL );
    write_strings_xy( c.l.x + 2, c.l.y + 5, BLACK_ON_CYAN,"Addressing : ",
		      (cd_data.status & 512)?"Red Book & HSG":
					     "HSG only", NULL );

    // Mashitup Does not need the psa array any more.
    // For future developments: function will return psa, instead of free it.
    destruct_box_vpc_psp(&psa);
    return cd_data.status;
}

/****************************************************************************/

// Writes single sound trail specified by trailnum where its modifier is
// place according to trail_sound out of 0 - 255.
void write_sound_trail( int trailnum, unsigned trail_sound, boolean active )
    {
    int i;

    int size = VT_R_X - VT_L_X - 7;                // 32;
    int modif = ( size * ( trail_sound + 1 )) / 256;
    char num = bold_digit(trailnum + '0');
    char color;

     if (active)
        color = BLACK_ON_CYAN;
     else
        color = DGRAY_ON_CYAN;

    // Write caption.
    write_char_xy( VT_L_X + 2, VT_L_Y + trailnum * 3 - 1, BLACK_ON_CYAN,
                   num );
    if (active)
       write_string_xy( VT_L_X + 3, VT_L_Y + trailnum * 3 - 1, color,
                        "- Sound Channel" );
    else
       write_string_xy( VT_L_X + 3, VT_L_Y + trailnum * 3 - 1, color,
                        "- N/A" );
//                        "- Sound Channel ( Not active )" );

    // Make right border.
    write_char_xy( VT_L_X + 2, VT_L_Y+ trailnum * 3, color, R_T_BORDER );

    // Write trail up to modifier.
    for ( i = VT_L_X + 3; i < VT_L_X + 3 + modif; i++ ){
        write_char_xy( i, VT_L_Y+ trailnum * 3, color, T_SCALE );
    }

    // Write the modifier.
    if (active)
       write_char_xy( i, VT_L_Y+ trailnum * 3, color, T_MODIF );
    else
       write_char_xy( i, VT_L_Y+ trailnum * 3, color, T_SCALE );

    // Write the rest of trail.
    for ( i++; i < VT_R_X - 3; i++ ){
        write_char_xy( i, VT_L_Y+ trailnum * 3, color, T_SCALE );
    }

    // Make left border                                              */
    write_char_xy( i, VT_L_Y + trailnum * 3, color, L_T_BORDER );

    return;
}

/****************************************************************************/

// Writes all the four sound trails. Unimplemented trails will not be active.
// "aic" variable indicates the number of active channels.
void write_all_sound_trails( volumeinfo_t vol, int aic )
    {
    defocus_mouse();
    write_sound_trail( 1, vol.volume0, aic >= 1 );
    write_sound_trail( 2, vol.volume1, aic >= 2 );
    write_sound_trail( 3, vol.volume2, aic >= 3 );
    write_sound_trail( 4, vol.volume3, aic >= 4 );
    focus_mouse();
    return;
}

/****************************************************************************/

// Create volume control panel.
void display_equalizer( volume_handler volhdr )
    {
    coords_t c;
    pscreen_array psa;

    // Create volume control window.
    c.l.x = VT_L_X;
    c.l.y = VT_L_Y;
    c.r.x = VT_R_X;
    c.r.y = VT_R_Y;

    psa = (pscreen_array)draw_box( c, WHITE_ON_CYAN , "Equalizer" );

    // Writes the sound trails.
    write_all_sound_trails( volhdr.vol, volhdr.aic );

    // Mashitup Does not need the psa array any more.
    destruct_box_vpc_psp(&psa);
    return;
}

/****************************************************************************/

// Make an empty playing progress bar.
void clear_play_progress_bar(void)
    {
    coords_t c;
    c.l.x = CT_L_X + 3;
    c.l.y = CT_R_Y - 2;
    c.r.x = CT_R_X - 2;
    c.r.y = CT_R_Y - 2;
    clear_line( c, PROG_BAR_COLS, K_SPACE );
}

/****************************************************************************/

// Interprets the whole information of new disk, creates all the panels.
// "play_mode" defines the state of play_mode buttons, "volhdr" has
// information for equalizer. "tag" is used to extract tracks name e.t.c.
// "selected song" is currently selected_song ( With finger on it ).
unsigned long new_disk( unsigned char play_mode, volume_handler volhdr,
                        unsigned char selected_song, disk_tag_t *tag )
    {
    coords_t c;
    pscreen_array psa;
    unsigned long cd_status_pnl_ret;
    int i;
    char *names[] = { REWIND_BUT, PLAY_BUT, PAUSE_BUT, STOP_BUT, FORWARD_BUT,
		      RANDOM_BUT, REPEAT_BUT,
		      UNLOCK_BUT, EJECT_BUT,
                      SP_HAMMER_BUT, SP_WORDUP_BUT,
                      BACK_SEC_BUT, FORW_SEC_BUT };

    unsigned char pal[256][3];
    save_pal(pal);

    // Get CD information.
    cd_get_audio_info();
    // If no disk then wait until disk will be insereted.
    while (cd_data.error == NO_CD_ERROR){
          no_disk();
    }

    // Create tracks panel.
    (void)v_load_disk_tag( cd_data.diskid, tag, DATA_FILE );
    screen_fade_out();
    display_album_panel( selected_song, tag, TRUE );

    // Create volume control panel.
    display_equalizer(volhdr);

    // Create Mashitups main window.
    c.l.x = CT_L_X;
    c.l.y = CT_L_Y;
    c.r.x = CT_R_X;
    c.r.y = CT_R_Y;
    psa = (pscreen_array)draw_box( c , WHITE_ON_CYAN  , "Remote Control" );

    // Create control's components.
    // Track controlers.
    c.l.x += 3;
    c.l.y = c.r.y - 4;
    for ( i = 0; i < 5; i++ ){
	draw_control_button( c.l, BUTTON_COLS, names[i], FALSE );
	c.l.x += length_of(names[i]) + 2;
    }
    // Play mode controlers.
    c.l.x = CT_R_X - 6;
    c.l.y = CT_R_Y - 13;
    draw_control_button( c.l, BUTTON_COLS, names[i++],
                                           play_mode & PLAY_RANDOM );
    c.l.y += 3;
    draw_control_button( c.l, BUTTON_COLS, names[i++],
                                           play_mode & PLAY_REPEAT );
    c.l.y += 3;

    // CD controlers.
    for ( ; i < CONTROL_KEYS_NUM; i++ ){
	draw_control_button( c.l, BUTTON_COLS, names[i], FALSE );
	c.l.y += 3;
    }

    //  Special effect buttons, and seconds control buttons.
    c.l.x = CT_L_X + 3;
    c.l.y = CT_R_Y - 7;
    draw_control_button( c.l, WHITE_ON_BLUE, names[i], FALSE );
    i++;
    c.l.x += 5;
    draw_control_button( c.l, WHITE_ON_BLUE, names[i], FALSE );
    i++;
    c.l.x += 11;
    draw_control_button( c.l, WHITE_ON_BLUE, names[i], FALSE );
    i++;
    c.l.x += 5;
    draw_control_button( c.l, WHITE_ON_BLUE, names[i], FALSE );
    i++;

    // Mashitup status.
    c.l.x = CT_L_X + 3;
    c.l.y = CT_L_Y + 3;
    c.r.x = CT_R_X;
    c.r.y = CT_R_Y;
    write_string_xy( c.l.x, c.l.y, RED_ON_CYAN ,
                     "Mashitup status  : Working." );

    // Write time statistics.
    c.l.y += 2;
    write_string_xy( c.l.x, c.l.y, BLACK_ON_CYAN,
                     "Track playing time : 00:00." );
    c.l.y += 2;
    write_string_xy( c.l.x, c.l.y, BLACK_ON_CYAN,
                     "Disk  playing time : 00:00." );

    // Make an empty playing progress bar and frames near him.
    clear_play_progress_bar();
    write_string_xy( c.l.x - 3, c.r.y - 2, WHITE_ON_CYAN, "ãíî" );
    write_string_xy( c.r.x - 2, c.r.y - 2, WHITE_ON_CYAN, "ïíä" );


    // Mashitup Does not need the psa array any more.
    destruct_box_vpc_psp(&psa);
    cd_status_pnl_ret = display_cd_status_panel();
    screen_fade_in(pal);
    return cd_status_pnl_ret;
}

/****************************************************************************/

// Writes the progress bar according to passed time ( taken form music_info
// parameter ) out of total time ( taken from tag argument ).
void write_play_progress_bar( musicinfo_t *music_info, disk_tag_t *tag )
    {
    int i;
    int color = PROG_BAR_COLS;
    coords_t c;

    long total_track_tim ;
    long passed_track_tim;
    long progress;

    total_track_tim = ( tag->tracks[music_info->track-1].track_min * 60   +
                        tag->tracks[music_info->track-1].track_sec ) * 75 +
                        tag->tracks[music_info->track-1].track_frame;

    passed_track_tim= ( music_info->track_min * 60 +
                        music_info->track_sec ) * 75 +
                        music_info->track_frames;

    progress = PROGRESS_LENGTH * 8 * passed_track_tim / total_track_tim;

    // If progress == 0 then new song has started.
    if (progress){
       c.l.x = CT_L_X + 3;
       c.l.y = CT_R_Y - 2;
       c.r.x = CT_R_X - 2;
       c.r.y = CT_R_Y - 2;

       for ( i = 0; i < progress / 8; i++ )
           write_char_xy( c.l.x + i, c.l.y, color, FULL_CELL );

       if ( progress % 8 )
          write_char_xy( c.l.x + i++, c.l.y, color, FIRST_CELL + progress % 8
                                                               - 1 );

       for ( ; i < PROGRESS_LENGTH; i++ )
           write_char_xy( c.l.x + i, c.l.y, color, K_SPACE );
    }
    else
       clear_play_progress_bar();

    if ( mouse_in( c.l.x, c.l.y, c.r.x, c.r.y )){
       defocus_mouse();
         focus_mouse();
    }

    return;
}

/****************************************************************************/

// Prints the track's play time and disk's play time.
// "time_direction" argument defines whether to write the elapsed or remaining
// time.
void print_play_time( musicinfo_t *music_info, play_time_t time_direction,
                      disk_tag_t *tag )
    {
    char tim[7];

    cd_tell_pos(music_info);

    // Write track playing time.
    if ( time_direction == passed ){
       tim[0] = ' ';
       tim[1] = '0' + music_info->track_min / 10;
       tim[2] = '0' + music_info->track_min % 10;
       tim[3] = ':';
       tim[4] = '0' + music_info->track_sec / 10;
       tim[5] = '0' + music_info->track_sec % 10;
       tim[6] = '\0';
    }
    else {
       long rem = ( tag->tracks[music_info->track-1].track_min -
                    music_info->track_min                      ) * 60 +
                    tag->tracks[music_info->track-1].track_sec -
                    music_info->track_sec;
       tim[0] = '-';
       tim[1] = '0' + ( rem / 60 ) / 10;
       tim[2] = '0' + ( rem / 60 ) % 10;
       tim[3] = ':';
       tim[4] = '0' + ( rem % 60 ) / 10;
       tim[5] = '0' + ( rem % 60 ) % 10;
       tim[6] = '\0';
    }

    if ( mouse_in( CT_L_X + 24, CT_L_Y + 5, CT_L_X + 29, CT_L_Y + 7 ))
       defocus_mouse();
    write_string_xy( CT_L_X + 23, CT_L_Y + 5, BLACK_ON_CYAN, tim );

    // Overcome the "2 seconds tag" in the first track.
    if ( music_info->track == 1 ){
       if (( music_info->disk_sec > 2 )||( music_info->disk_min != 0 )){
          music_info->disk_sec -= 2;        // 2 Tag seconds.
          if ( music_info->disk_sec < 0 ){
             music_info->disk_sec += 60;
             music_info->disk_min--;
          }
       }
       else {
            music_info->disk_sec = 0;
            music_info->disk_min = 0;
       }
    }
    if ( time_direction == passed ){
       // Write disk playing time.
       tim[0] = ' ';
       tim[1] = '0' + music_info->disk_min / 10;
       tim[2] = '0' + music_info->disk_min % 10;
       tim[3] = ':';
       tim[4] = '0' + music_info->disk_sec / 10;
       tim[5] = '0' + music_info->disk_sec % 10;
       tim[6] = '\0';
    }
    else {
       long rem = ( cd_data.disk_length_min - music_info->disk_min ) * 60 +
                  ( cd_data.disk_length_sec - music_info->disk_sec );

       tim[0] = '-';
       tim[1] = '0' + ( rem / 60 ) / 10;
       tim[2] = '0' + ( rem / 60 ) % 10;
       tim[3] = ':';
       tim[4] = '0' + ( rem % 60 ) / 10;
       tim[5] = '0' + ( rem % 60 ) % 10;
       tim[6] = '\0';
    }
    write_string_xy( CT_L_X + 23, CT_L_Y + 7, BLACK_ON_CYAN, tim );

    //write play progress bar.
    write_play_progress_bar( music_info, tag );

    if ( mouse_in( CT_L_X + 24, CT_L_Y + 5, CT_L_X + 29, CT_L_Y + 7 ))
       focus_mouse();

    return;
}

/****************************************************************************/

// Makes message that disk is still revolving.
void make_rdelay_message(void)
    {
    pscreen_array psa;
    coords_t c;
    struct time t, rev_t;
    long int  i, rev_i;

    // Set no mouse layer.
    int M_prev = M_layer;
                 M_layer = NO_MOUSE_LAYER;

    // Make message window.
    c.l.x = 25;
    c.l.y = 22;
    c.r.x = 55;
    c.r.y = 28;
    psa = (pscreen_array)draw_box( c , ERR_COLS , "Please wait" );
    write_string_xy( c.l.x + 3, c.l.y + 3, ERR_COLS,
                     "Disk Is Being Revolved..." );
    focus_mouse();
    // Give CD-ROM time to revolve.
    gettime(&t);
    gettime(&rev_t);
    i     =     t.ti_sec * 100L +     t.ti_hund;
    rev_i = rev_t.ti_sec * 100L + rev_t.ti_hund;
    cd_get_audio_info();
    while ((cd_data.error != CD_OK_ERROR)&&( rev_i - i < ROTARY_DELAY )){
          cd_get_audio_info();
          gettime(&rev_t);
          rev_i = rev_t.ti_sec * 100L + rev_t.ti_hund;
          if (rev_i < i)
             rev_i += 60 * 100L;    // One minute.
          if (rev_i < i)
             rev_i += 59 * 60 * 100L;    // One hour.
    }

    // Closing the window.
    defocus_mouse();
    destruct_box( c, &psa );
    M_layer = M_prev;
    return;
}

/****************************************************************************/

// Makes warning message that there is no disk in CD drive.
void no_disk(void)
    {
    pscreen_array psa;
    coords_t c;
    int i = 2;
    char cd_drive[2];

    // Set no disk layer.
    int M_prev = M_layer;
                 M_layer = NO_DISK_LAYER;

    // Create dialog box.
    error_sound();
    c.l.x = TT_L_X;
    c.l.y = TT_L_Y;
    c.r.x = TT_R_X;
    c.r.y = TT_R_Y;
    psa = (pscreen_array)draw_box( c , ERR_COLS , "No CD Disk To Play!" );
    cd_drive[0] = cd_data.first_drive + 'A';
    cd_drive[1] = '\0';
    write_string_xy( c.l.x + 6, c.r.y - 12, ERR_COLS,
                     "Please insert disk into your");
    write_strings_xy( c.l.x + 6, c.r.y - 10, ERR_COLS,
                      "    CD-ROM drive (", cd_drive, ").", NULL);
    // Eject the tray for new disk to be inserted.
    move_tray(EJECT);

    // Make buttons and wait for user to proceed.
    {
     coords_t cb;
     char but_ok[]   = " ^Play ";
     char but_exit[] = " ^Exit ";

     cb.l.x = ( (c.r.x + c.l.x - length_of(but_ok) - 1) / 2) - 7;
     cb.l.y = c.r.y - 3;
     cb.r.x = ( (c.r.x + c.l.x - length_of(but_ok) - 1) / 2) + 10;
     cb.r.y = c.r.y - 3;
     i = 1;         // But_ok is selected.
     i = choose_button( cb, BUTTON_COLS, BUTTON_COLS, but_ok, but_exit, i );
     if ( i == 2 )
	button_click( cb.r, BUTTON_COLS, but_exit, ERR_COLS );
     else
	button_click( cb.l, BUTTON_COLS, but_ok, ERR_COLS );
    } // Sub block.

    move_tray(CLOSE);

    // Restore the screen.
    defocus_mouse();
    destruct_box( c, &psa );
    if ( i == 2 )                // User wants to exit.
       terminate(0);

    make_rdelay_message();
    cd_get_audio_info();
    M_layer = M_prev;
    return;
}

/****************************************************************************/

// Locks the CD-ROM drive and draws the "Lock" button as pressed.
// Returns True ( = cd is locked ).
boolean lock_disk(void)
    {
    coord_t c = { CT_L_X + 33, CT_R_Y - 7 };


    // Delete the shadow.
    write_char_xy( c.x, c.y, BLACK_ON_CYAN, K_SPACE );
    write_string_xy( c.x + 1, c.y + 1, BLACK_ON_CYAN, "    " );

    // Drow pressed "Lock" button.
    draw_control_button( c, BUTTON_COLS, LOCK_BUT, TRUE );

/*  Not needed because cd_status_panel is focusing the mouse.
    if ( mouse_in( c.x, c.y, c.x + 5, c.y + 1 )){
       defocus_mouse();
       focus_mouse();
    } */

    // Lock the CD-ROM.
    cd_lock(LOCK);
    return TRUE;
}

/****************************************************************************/

// Unlocks the CD-ROM drive and draws the "Lock" button as released.
// Returns False ( = cd is not locked ).
boolean unlock_disk(void)
    {
    coord_t c = { CT_L_X + 33, CT_R_Y - 7 };

    // Delete pressed button last cell.
    write_char_xy( c.x + 4, c.y, BLACK_ON_CYAN, K_SPACE );

    // Drow released "Lock" button.
    draw_control_button( c, BUTTON_COLS, UNLOCK_BUT, FALSE );

/*  Not needed because cd_status_panel is focusing the mouse.
    if ( mouse_in( c.x, c.y, c.x + 5, c.y + 1 )){
       defocus_mouse();
       focus_mouse();
    } */

    // Unlock the CD-ROM.
    cd_lock(UNLOCK);
    return FALSE;
}

/****************************************************************************/

// Imitates click on "Rewind" button. This button cannot be pressed, only
// clicked for short period of time.
void click_rewind_button(void)
    {
    coord_t c = { CT_L_X + 3, CT_R_Y - 4 };

    // Delete the shadow.
    write_char_xy( c.x, c.y, BLACK_ON_CYAN, K_SPACE );
    write_string_xy( c.x + 1, c.y + 1, BLACK_ON_CYAN, "    " );

    // Drow pressed "Rewind" button.
    draw_control_button( c, BUTTON_COLS, REWIND_BUT, TRUE );
    if ( mouse_in( c.x, c.y, c.x + 5, c.y + 1 )){
       defocus_mouse();
       focus_mouse();
    }
    delay(CONT_BUT_DELAY);

    // Delete pressed button last cell.
    write_char_xy( c.x + 4, c.y, BLACK_ON_CYAN, K_SPACE );

    // Drow released "Rewind" button.
    draw_control_button( c, BUTTON_COLS, REWIND_BUT, FALSE );
    if ( mouse_in( c.x, c.y, c.x + 5, c.y + 1 )){
       defocus_mouse();
       focus_mouse();
    }
    return;
}

/****************************************************************************/

// Imitates click on "Play" button. This button cannot be pressed, only
// clicked for short period of time.
void click_play_button(void)
    {
    coord_t c = { CT_L_X + 9, CT_R_Y - 4 };

    // Delete the shadow.
    write_char_xy( c.x, c.y, BLACK_ON_CYAN, K_SPACE );
    write_string_xy( c.x + 1, c.y + 1, BLACK_ON_CYAN, "   " );

    // Drow pressed "Play" button.
    draw_control_button( c, BUTTON_COLS, PLAY_BUT, TRUE );
    if ( mouse_in( c.x, c.y, c.x + 5, c.y + 1 )){
       defocus_mouse();
       focus_mouse();
    }
    delay(CONT_BUT_DELAY);

    // Delete pressed button last cell.
    write_char_xy( c.x + 3, c.y, BLACK_ON_CYAN, K_SPACE );

    // Drow released "Play" button.
    draw_control_button( c, BUTTON_COLS, PLAY_BUT, FALSE );
    if ( mouse_in( c.x, c.y, c.x + 5, c.y + 1 )){
       defocus_mouse();
       focus_mouse();
    }
    return;
}

/****************************************************************************/

// Imitates click on "Stop" button. This button cannot be pressed, only
// clicked for short period of time.
void click_stop_button(void)
    {
    coord_t c = { CT_L_X + 19, CT_R_Y - 4 };

    // Delete the shadow.
    write_char_xy( c.x, c.y, BLACK_ON_CYAN, K_SPACE );
    write_string_xy( c.x + 1, c.y + 1, BLACK_ON_CYAN, "   " );

    // Drow pressed "Stop" button.
    draw_control_button( c, BUTTON_COLS, STOP_BUT, TRUE );
    if ( mouse_in( c.x, c.y, c.x + 5, c.y + 1 )){
       defocus_mouse();
       focus_mouse();
    }
    delay(CONT_BUT_DELAY);

    // Delete pressed button last cell.
    write_char_xy( c.x + 3, c.y, BLACK_ON_CYAN, K_SPACE );

    // Drow released "Stop" button.
    draw_control_button( c, BUTTON_COLS, STOP_BUT, FALSE );
/*  Not needed because cd_status_panel is focusing the mouse.
    if ( mouse_in( c.x, c.y, c.x + 5, c.y + 1 )){
       defocus_mouse();
       focus_mouse();
    } */
    return;
}

/****************************************************************************/

// Imitates click on "Forward" button. This button cannot be pressed, only
// clicked for short period of time.
void click_forward_button(void)
    {
    coord_t c = { CT_L_X + 24, CT_R_Y - 4 };

    // Delete the shadow.
    write_char_xy( c.x, c.y, BLACK_ON_CYAN, K_SPACE );
    write_string_xy( c.x + 1, c.y + 1, BLACK_ON_CYAN, "    " );

    // Drow pressed "Forward" button.
    draw_control_button( c, BUTTON_COLS, FORWARD_BUT, TRUE );
    if ( mouse_in( c.x, c.y, c.x + 5, c.y + 1 )){
       defocus_mouse();
       focus_mouse();
    }
    delay(CONT_BUT_DELAY);

    // Delete pressed button last cell.
    write_char_xy( c.x + 4, c.y, BLACK_ON_CYAN, K_SPACE );

    // Drow released "Forward" button.
    draw_control_button( c, BUTTON_COLS, FORWARD_BUT, FALSE );
    if ( mouse_in( c.x, c.y, c.x + 5, c.y + 1 )){
       defocus_mouse();
       focus_mouse();
    }
    return;
}

/****************************************************************************/

// Imitates click on "Eject" button. This button cannot be pressed, only
// clicked for short period of time.
void click_eject_button(void)
    {
    coord_t c = { CT_L_X + 33, CT_R_Y - 4 };

    // Delete the shadow.
    write_char_xy( c.x, c.y, BLACK_ON_CYAN, K_SPACE );
    write_string_xy( c.x + 1, c.y + 1, BLACK_ON_CYAN, "    " );

    // Drow pressed "Eject" button.
    draw_control_button( c, BUTTON_COLS, EJECT_BUT, TRUE );
    if ( mouse_in( c.x, c.y, c.x + 5, c.y + 1 )){
       defocus_mouse();
       focus_mouse();
    }
    delay(CONT_BUT_DELAY);

    // Delete pressed button last cell.
    write_char_xy( c.x + 4, c.y, BLACK_ON_CYAN, K_SPACE );

    // Drow released "Eject" button.
    draw_control_button( c, BUTTON_COLS, EJECT_BUT, FALSE );
    if ( mouse_in( c.x, c.y, c.x + 5, c.y + 1 )){
       defocus_mouse();
       focus_mouse();
    }
    return;
}

/****************************************************************************/

// Imitates clicks on "Repeat" and "Random" button.
void click_play_mode( int button )
    {
    coord_t c = { CT_R_X - 6, CT_R_Y - 13 };

    if ( button & PLAY_RANDOM ){
       if ( button & PLAY_STOP ){
       // Delete pressed button last cell.
       write_char_xy( c.x + 4, c.y, BLACK_ON_CYAN, K_SPACE );

       // Drow released button.
       draw_control_button( c, BUTTON_COLS, RANDOM_BUT, FALSE );
       }
       else {
       // Delete the shadow.
       write_char_xy( c.x, c.y, BLACK_ON_CYAN, K_SPACE );
       write_string_xy( c.x + 1, c.y + 1, BLACK_ON_CYAN, "    " );

       // Drow pressed button.
       draw_control_button( c, BUTTON_COLS, RANDOM_BUT, TRUE );
       }
    }
    else {
       c.y += 3;
       if ( button & PLAY_STOP ){
       // Delete pressed button last cell.
       write_char_xy( c.x + 4, c.y, BLACK_ON_CYAN, K_SPACE );

       // Drow released button.
       draw_control_button( c, BUTTON_COLS, REPEAT_BUT, FALSE );
       }
       else {
       // Delete the shadow.
       write_char_xy( c.x, c.y, BLACK_ON_CYAN, K_SPACE );
       write_string_xy( c.x + 1, c.y + 1, BLACK_ON_CYAN, "    " );

       // Drow pressed button.
       draw_control_button( c, BUTTON_COLS, REPEAT_BUT, TRUE );
       }
    }
    if ( mouse_in( c.x, c.y, c.x + 5, c.y + 1 )){
       defocus_mouse();
       focus_mouse();
    }
    return;
}

/****************************************************************************/

// Imitates click on special hammer effect button. This button cannot be
// pressed, only clicked for short period of time.
void click_sphammer_button(void)
    {
    coord_t c = { CT_L_X + 3, CT_R_Y - 7 };

    // Delete the shadow.
    write_char_xy( c.x, c.y, BLACK_ON_CYAN, K_SPACE );
    write_string_xy( c.x + 1, c.y + 1, BLACK_ON_CYAN, "   " );

    // Drow pressed special hammer effect button.
    draw_control_button( c, WHITE_ON_BLUE, SP_HAMMER_BUT, TRUE );
    if ( mouse_in( c.x, c.y, c.x + 5, c.y + 1 )){
       defocus_mouse();
       focus_mouse();
    }
    delay(CONT_BUT_DELAY);

    // Delete pressed button last cell.
    write_char_xy( c.x + 3, c.y, BLACK_ON_CYAN, K_SPACE );

    // Drow released special hammer effect button.
    draw_control_button( c, WHITE_ON_BLUE, SP_HAMMER_BUT, FALSE );
    if ( mouse_in( c.x, c.y, c.x + 5, c.y + 1 )){
       defocus_mouse();
       focus_mouse();
    }

    return;
}

/****************************************************************************/

// Imitates click on special word up effect button. This button cannot be
// pressed, only clicked for short period of time.
void click_spwordup_button(void)
    {
    coord_t c = { CT_L_X + 8, CT_R_Y - 7 };

    // Delete the shadow.
    write_char_xy( c.x, c.y, BLACK_ON_CYAN, K_SPACE );
    write_string_xy( c.x + 1, c.y + 1, BLACK_ON_CYAN, "         " );

    // Drow pressed special word up effect button.
    draw_control_button( c, WHITE_ON_BLUE, SP_WORDUP_BUT, TRUE );
    if ( mouse_in( c.x, c.y, c.x + 10, c.y + 1 )){
       defocus_mouse();
       focus_mouse();
    }
    delay(CONT_BUT_DELAY);

    // Delete pressed button last cell.
    write_char_xy( c.x + 9, c.y, BLACK_ON_CYAN, K_SPACE );

    // Drow released special word up effect button.
    draw_control_button( c, WHITE_ON_BLUE, SP_WORDUP_BUT, FALSE );
    if ( mouse_in( c.x, c.y, c.x + 10, c.y + 1 )){
       defocus_mouse();
       focus_mouse();
    }

    return;
}

/****************************************************************************/

// Imitates click on backward 5 seconds button. This button cannot be
// pressed, only clicked for short period of time.
void click_backsec_button(void)
    {
    coord_t c = { CT_L_X + 19, CT_R_Y - 7 };

    // Delete the shadow.
    write_char_xy( c.x, c.y, BLACK_ON_CYAN, K_SPACE );
    write_string_xy( c.x + 1, c.y + 1, BLACK_ON_CYAN, "   " );

    // Drow pressed back 5 seconds button.
    draw_control_button( c, WHITE_ON_BLUE, BACK_SEC_BUT, TRUE );
    if ( mouse_in( c.x, c.y, c.x + 5, c.y + 1 )){
       defocus_mouse();
       focus_mouse();
    }
    delay(CONT_BUT_DELAY);

    // Delete pressed button last cell.
    write_char_xy( c.x + 3, c.y, BLACK_ON_CYAN, K_SPACE );

    // Drow released back 5 seconds button.
    draw_control_button( c, WHITE_ON_BLUE, BACK_SEC_BUT, FALSE );
    if ( mouse_in( c.x, c.y, c.x + 5, c.y + 1 )){
       defocus_mouse();
       focus_mouse();
    }

    return;
}

/****************************************************************************/

// Imitates click on forward 5 seconds button. This button cannot be
// pressed, only clicked for short period of time.
void click_forwsec_button(void)
    {
    coord_t c = { CT_L_X + 24, CT_R_Y - 7 };

    // Delete the shadow.
    write_char_xy( c.x, c.y, BLACK_ON_CYAN, K_SPACE );
    write_string_xy( c.x + 1, c.y + 1, BLACK_ON_CYAN, "   " );

    // Drow pressed forward 5 seconds button.
    draw_control_button( c, WHITE_ON_BLUE, FORW_SEC_BUT, TRUE );
    if ( mouse_in( c.x, c.y, c.x + 5, c.y + 1 )){
       defocus_mouse();
       focus_mouse();
    }
    delay(CONT_BUT_DELAY);

    // Delete pressed button last cell.
    write_char_xy( c.x + 3, c.y, BLACK_ON_CYAN, K_SPACE );

    // Drow released forward 5 seconds button.
    draw_control_button( c, WHITE_ON_BLUE, FORW_SEC_BUT, FALSE );
    if ( mouse_in( c.x, c.y, c.x + 5, c.y + 1 )){
       defocus_mouse();
       focus_mouse();
    }

    return;
}

/****************************************************************************/

// Draws released pause button.
void release_pause_button(void)
    {
    coord_t c = { CT_L_X + 14, CT_R_Y - 4 };

    // Change control status to playing.
    write_string_xy( CT_L_X + 22, CT_L_Y + 3, RED_ON_CYAN , "Playing." );

    // Delete pressed button last cell.
    write_char_xy( c.x + 3, c.y, BLACK_ON_CYAN, K_SPACE );

    // Drow released "paused" button.
    draw_control_button( c, BUTTON_COLS, " „ ", FALSE );
    if ( mouse_in( c.x, c.y, c.x + 5, c.y + 1 )){
       defocus_mouse();
       focus_mouse();
    }
    return;
}

/****************************************************************************/

// Pauses and unpauses the music according to "paused" parameter. Draws the
// "Pause" button accordingly.
// Returns True if paused or False if unpaused.
boolean pause_song( boolean paused )
    {
    coord_t c = { CT_L_X + 14, CT_R_Y - 4 };

    if (!paused)
       {
       cd_stop_music();
       paused = TRUE;

       // Change control status to paused.
       write_string_xy( CT_L_X + 22, CT_L_Y + 3, RED_ON_CYAN , "Paused. " );

       // Delete the shadow.
       write_char_xy( c.x, c.y, BLACK_ON_CYAN, K_SPACE );
       write_string_xy( c.x + 1, c.y + 1, BLACK_ON_CYAN, "   " );

       // Drow pressed "paused" button.
       draw_control_button( c, BUTTON_COLS, PAUSE_BUT, TRUE );
       if ( mouse_in( c.x, c.y, c.x + 5, c.y + 1 )){
          defocus_mouse();
          focus_mouse();
       }

    }
    else {
       cd_resume_music();
       paused = FALSE;
       release_pause_button();
    }
    return paused;
}

/****************************************************************************/

// Stops the audio. Works like pause but ends the current song, and retrns
// FALSE to indicate that Mashitup is not playing.
boolean stop_music(void)
    {
    cd_stop_music();
    write_string_xy( CT_L_X+22, CT_L_Y+3, RED_ON_CYAN , "Working." );

    // Zero time statistics.
    write_string_xy( CT_L_X + 24, CT_L_Y + 5, BLACK_ON_CYAN, "00:00." );
    write_string_xy( CT_L_X + 24, CT_L_Y + 7, BLACK_ON_CYAN, "00:00." );

    return FALSE;
}

/****************************************************************************/

// Asks user if he wills to stop the audio or not at exit.
void stop_audio_choice(void)
    {
    pscreen_array psa;
    coords_t c;
    int i = 2;

    // Set stop audio choice layer.
    int M_prev = M_layer;
                 M_layer = STOP_A_CHOICE_LAYER;

    // Create dialog box.
    c.l.x = CT_L_X;
    c.l.y = CT_L_Y;
    c.r.x = CT_R_X;
    c.r.y = CT_R_Y;
    psa = (pscreen_array)draw_box( c , ERR_COLS , "Exit Mashitup!" );
    write_string_xy( c.l.x + 3, c.l.y + 3, ERR_COLS,
                     "After exiting Mashitup");
    write_string_xy( c.l.x + 3, c.l.y + 5, ERR_COLS,
                      "Audio will continue to play?");

    // Make buttons and wait for user to proceed.
    {
     coords_t cb;
     char but_ok[]   = " ^Yes ";
     char but_exit[] = " ^No ";

     cb.l.x = ( (c.r.x + c.l.x - length_of(but_ok) - 1) / 2) - 7;
     cb.l.y = c.r.y - 3 - 2;
     cb.r.x = ( (c.r.x + c.l.x - length_of(but_ok) - 1) / 2) + 10;
     cb.r.y = c.r.y - 3 - 2;
     i = 1;         // But_ok is selected.
     i = choose_button( cb, BUTTON_COLS, BUTTON_COLS, but_ok, but_exit, i );
     if ( i == 2 )
	button_click( cb.r, BUTTON_COLS, but_exit, ERR_COLS );
     else
	button_click( cb.l, BUTTON_COLS, but_ok, ERR_COLS );
    } // Sub block.

    defocus_mouse();
    // Restore the screen.
    destruct_box( c, &psa );
    if ( i == 2 ){                // User wants to stop audio.
       cd_stop_music();
    }
    M_layer = M_prev;
    return;
}

/****************************************************************************/
			 /* Disks database modules.*/

// Loads the disk tag with appropriate message to the user.
// "disk_id" argument is Id of the current disk, "tag" is where to load the
// information, "source_file" is from which file path to load.
// Returns True if loaded, otherwise - False.
boolean v_load_disk_tag( char *disk_id, disk_tag_t *tag, char *source_file )
    {
    pscreen_array psa;
    coords_t c;
    boolean loaded;

    // Make message window.
    c.l.x = 20;
    c.l.y = 22;
    c.r.x = 60;
    c.r.y = 28;
    psa = (pscreen_array)draw_box( c , WHITE_ON_CYAN  , "Please wait" );
    write_string_xy( c.l.x + 3, c.l.y + 3, BLACK_ON_CYAN,
                     " Loading disk\'s tag information..." );
    focus_mouse();

    // Load the tag.
    loaded = load_disk_tag( disk_id, tag, source_file );

    defocus_mouse();
    // Closing the window.
    destruct_box( c, &psa );
    return loaded;
}

/****************************************************************************/

// Draw artist, album, year and genre input options from tag variable.
// It is draw from ( x, y ) coordinates.
void draw_album_info_for_editor( int x, int y, disk_tag_t *tag )
    {
    // Make artist input option.
    write_string_xy( x, y, RED_ON_CYAN , "Artist :"         );
    write_string_xy( x + 9, y, BLACK_ON_CYAN, tag->artist    );
    y += 2;

    // Make album input option.
    write_string_xy( x, y, BLACK_ON_CYAN, "Album  :"         );
    write_string_xy( x + 9, y, BLACK_ON_CYAN, tag->album     );
    y += 2;

    // Make year input option.
    write_string_xy( x, y, BLACK_ON_CYAN, "Year   :"         );
    write_string_xy( x + 9, y, BLACK_ON_CYAN, tag->year      );
    y += 2;

    // Make genre and year input option.
    write_string_xy( x, y, BLACK_ON_CYAN, "Genre  :" 	);
    write_string_xy( x + 9, y, BLACK_ON_CYAN, tag->genre     );
    y += 2;
    return;
}

/****************************************************************************/

// Returns the color of track specified by "tracknum", according to track
// status ( T_BLOCKED, T_PLAYABLE, e.t.c ).
char edit_track_color( int tracknum, disk_tag_t *tag )
    {
    char color;

    if ( is_track_data   ( tracknum, tag ))
       color = DGRAY_ON_CYAN;                      // Data     track.
    else
    if ( is_track_blocked( tracknum, tag ))
       color = BROWN_ON_CYAN;                      // Blocked  track.
    else
       color = BLACK_ON_CYAN;                      // Playable track.
    return color;
}

/****************************************************************************/

// Performs the disk tag editor.
// All the changes are updated in *tag identifier and first editing line
// is curopt wich represents the number of track.
// curopt table:
//               -4 : Edit disk's artist name.
//               -3 : Edit disk's album name.
//               -2 : Edit disk's production year.
//               -1 : Edit disk's music genre.
//    0.. max_track : Edit track with number of curopt + 1.
void edit_disk_tag( disk_tag_t *tag, int curopt )
    {
    pscreen_array psa;
    coords_t      c;
    coord_t       i;
    int           j;
    char          akey;
    char          color;

    // Set edit disk tag layer.
    int M_prev = M_layer;
                 M_layer = EDIT_TAG_LAYER;

    // Make disk registration pannel.
    c.l.x = RT_L_X;
    c.l.y = RT_L_Y;
    c.r.x = RT_R_X;
    c.r.y = RT_R_Y;
    psa = (pscreen_array)draw_box( c , WHITE_ON_CYAN  ,
                                   "Registration Wizard" );

    i.x = c.l.x + 2;
    i.y = c.l.y + 2;

    // Draw artist, album, year and genre input options.
    draw_album_info_for_editor( i.x, i.y, tag );

    // Make tracks list.
    i.y += 7;
    write_string_xy( 37, i.y, RED_ON_CYAN , "Tracks" 	);
    i.x += 2;
    i.y += 2;
    for ( j = 0; j < cd_data.max_track; j++ ){

//      Make caption far each track.
//      char num[3];
//      num[0] = ( j + 1 ) / 10 + '0';
//      num[1] = ( j + 1 ) % 10 + '0';
//      num[2] = '\0';
//	write_string_xy( i.x + j%2 * 29, i.y + j / 2 * 2 - 1, BLACK_ON_CYAN,
//                       num );

        color = edit_track_color( j + 1, tag );
	write_string_xy( i.x + j%2 * 29, i.y + j / 2 * 2, color,
                         tag->tracks[j].name );
    }

    // Read all the information.
    i.x = c.l.x + 4;
    i.y = c.l.y + 11;
    curopt--;
    color = edit_track_color( curopt + 1, tag );
    akey = get_string_xy( i.x + curopt%2 * 29, i.y + curopt / 2 * 2, color,
                          tag->tracks[curopt].name, T_NAME_LENGTH - 1,
                          EDIT_BG_CHAR );

    while ( akey != K_EXIT ){

          // Check if user wants to edit other item.
          switch (akey){

                 case K_UP    :
                             curopt--;
                             if ( curopt < -4 )
                                curopt = cd_data.max_track - 1;
                             break;

                 case K_DOWN  :
                 case K_ENTER :
                             curopt++;
                             if ( curopt > cd_data.max_track- 1 )
                                curopt = -4;
                             break;
          }

          // Get general item.
          if ( curopt < 0 ){
             // Edit disk info item.
             switch (curopt){

		    case -1 :  // Edit disk's music genre..
             akey = get_string_xy( i.x + 7, i.y + curopt * 2 - 1,
                                   BLACK_ON_CYAN,
                                   tag->genre, G_NAME_LENGTH - 1,
                                   EDIT_BG_CHAR );
                               break;

                    case -2 :  // Edit disk's production year.
             akey = get_string_xy( i.x + 7, i.y + curopt * 2 - 1,
                                   BLACK_ON_CYAN,
                                   tag->year, 4, EDIT_BG_CHAR );
                               break;

                    case -3 :  // Edit disk's album name.
             akey = get_string_xy( i.x + 7, i.y + curopt * 2 - 1,
                                   BLACK_ON_CYAN,
                                   tag->album, A_NAME_LENGTH - 1,
                                   EDIT_BG_CHAR );
                               break;

                    case -4 :  // Edit disk's artist name.
             akey = get_string_xy( i.x + 7, i.y + curopt * 2 - 1,
                                   BLACK_ON_CYAN,
                                   tag->artist, A_NAME_LENGTH - 1,
				   EDIT_BG_CHAR );
                               break;
             }
          }
          else {
             // Edit track item.
             color = edit_track_color( curopt + 1, tag );
             akey = get_string_xy( i.x + curopt%2 * 29, i.y + curopt / 2 * 2,
                                   color, tag->tracks[curopt].name,
                                   T_NAME_LENGTH - 1, EDIT_BG_CHAR );
          }
    }

    // Closing the pannel.
    destruct_box( c, &psa );
    M_layer = M_prev;
    return;
}

/****************************************************************************/

// Registers the disk with appropriate message to the user.
// "disk_id" argument is Id of the current disk, "tag" is the source of the
// information, "dest_file" is path for saving file.
// Returns True if saved, otherwise - False.
boolean v_register_disk( disk_tag_t *tag, char *disk_id, char *dest_file )
    {
    pscreen_array psa;
    coords_t c;
    boolean saved;

    // Make message window.
    c.l.x = 20;
    c.l.y = 22;
    c.r.x = 60;
    c.r.y = 28;
    psa = (pscreen_array)draw_box( c , WHITE_ON_CYAN  , "Please wait" );
    write_string_xy( c.l.x + 3, c.l.y + 3, BLACK_ON_CYAN,
                     " Saving disk\'s tag information..." );
    focus_mouse();

    // Save the tag.
    saved = register_disk( tag, disk_id, dest_file );

    defocus_mouse();
    // Closing the window.
    destruct_box( c, &psa );
    return saved;
}

/****************************************************************************/

// Adds the "Deleted" mark to disk tag in the deleting editor.
// Argument ( x, y ) are coordinates of the mark.
void add_deleted_mark( char x, char y )
    {
    write_string_xy( x, y    , YELLOW_ON_CYAN, "ÚÄ  ÄÄ  Ä¿" );
    write_string_xy( x, y + 1, BRED_ON_CYAN  , " Deleted! " );
    write_string_xy( x, y + 2, YELLOW_ON_CYAN, "ÀÄ  ÄÄ  ÄÙ" );
    return;
}

/****************************************************************************/

// Removes the "Deleted" mark in the deleting editor.
// Argument ( x, y ) are coordinates of the mark.
void remove_deleted_mark( char x, char y )
    {
    write_string_xy( x, y    , BLACK_ON_CYAN, "            " );
    write_string_xy( x, y + 1, BLACK_ON_CYAN, "            " );
    write_string_xy( x, y + 2, BLACK_ON_CYAN, "            " );
    return;
}

/****************************************************************************/

// Clears all tag's information in deleting editor for the next tag.
// Argument c defines a rectangle to clean.
void clear_tag_window( coords_t c )
    {
    coord_t i;

    for ( i.x = c.l.x + 11; i.x <= c.r.x - 30; i.x++ )
	for ( i.y = c.l.y + 2; i.y<= c.l.y + 8; i.y++ )
	    write_char_xy( i.x, i.y, BLACK_ON_CYAN, K_SPACE );
    for ( i.x = c.l.x + 4; i.x <= c.r.x - 8; i.x++ )
	for ( i.y = c.l.y + 11; i.y<= c.r.y - 2; i.y++ )
	    write_char_xy( i.x, i.y, BLACK_ON_CYAN, K_SPACE );

    return;
}

/****************************************************************************/

// Makes deleting editor for disk tags, and return True if it worked
// successfuly.
boolean delete_disk_tag(void)
    {
    pscreen_array psa;
    coords_t c;
    coord_t  i;
    int      j;
    char     color;
    disk_tag_t tag;
    int        tag_num = 0;
    int        tag_total_num = 0;
    FILE*   data;
    char    hotkey, key = 0;
    char but_del[13];

    // Set delete disk tag layer.
    int M_prev = M_layer;
                 M_layer = DELETE_TAG_LAYER;

    // Make deleting editor window.
    c.l.x = RT_L_X;
    c.l.y = RT_L_Y;
    c.r.x = RT_R_X;
    c.r.y = RT_R_Y;
    psa = (pscreen_array)draw_box( c , WHITE_ON_CYAN,
                                   " Disk Deleting Wizard" );

    //-------------
    // Open data file.
    data = fopen( DATA_FILE, "rb+");
    if ( data == NULL ){
       run_time_errors(9);       // Data file not found.
       // Closing the window.
       destruct_box( c, &psa );
       M_layer = M_prev;
       return FALSE;
    }

    // Count the number of tags.
    tag_total_num = 1L * file_size(data) / sizeof(tag);
    if ( tag_total_num == 0 ){
       focus_mouse();
       run_time_errors(101);       // Warning - nothing to delete.
       fclose(data);
       defocus_mouse();
       destruct_box( c, &psa );
       M_layer = M_prev;
       return TRUE;
    }

    key = K_RIGHT;
    while ( key != K_EXIT ){
          //-------------
          focus_mouse();
          if (( key == K_RIGHT )||( key == K_LEFT )){
             // Get location for the tag.
             fseek( data, (long)sizeof(tag) * tag_num, SEEK_SET);

             // Read the tag.
             if ( fread( &tag, sizeof(tag), 1, data ) != 1 ){
                run_time_errors(11);      // Data file is corrupted.
                fclose(data);
                // defocus_mouse();   - Is inside the last loop iteration.
                // Closing the window.
                destruct_box( c, &psa );
                M_layer = M_prev;
                return FALSE;
             }
             //-------------

             i.x = c.l.x + 2;
             i.y = c.l.y + 2;

             // Draw artist, album, year and genre input options.
             draw_album_info_for_editor( i.x, i.y, &tag );

             // Make tracks list.
             i.y += 7;
             write_string_xy( 37, i.y, RED_ON_CYAN , "Tracks" 	);
             i.x += 2;
             i.y += 2;
             for ( j = 0; j < tag.songs_num; j++ ){
                 color = edit_track_color( j + 1, &tag );
	         write_string_xy( i.x + j%2 * 29, i.y + j / 2 * 2, color,
                                  tag.tracks[j].name );
             }

             {
             char nums[20];

             i.x = c.r.x - 12;
             i.y = c.l.y + 7;
             itoa( tag_num + 1, nums, 10 );
             set_bold_digits(nums);
             write_strings_xy( i.x, i.y, BLACK_ON_CYAN, "Disk  : ", nums,
                               NULL );
             itoa( tag_total_num, nums, 10 );
             set_bold_digits(nums);
             write_strings_xy( i.x, i.y + 2, BLACK_ON_CYAN, "Total : ", nums,
                               NULL );
             }
          }

          i.x = c.l.x + 35;
          i.y = c.l.y + 2;
          if ( tag.status == S_DELETED ){
             add_deleted_mark( i.x, i.y );
             copy_string( but_del, " ^UnDelete " );
          }
          else {
             remove_deleted_mark( i.x, i.y );
             copy_string( but_del, "  ^Delete  " );
          }

          // Draw the Delete button.
          i.x = RT_R_X - 12;
          i.y = RT_L_Y + 2;

          hotkey = draw_button( i, BUTTON_COLS, but_del, TRUE );

          defocus_mouse();
          focus_mouse();
          for ( key = toupper(getch());
	      ( key != hotkey )&&( key != K_ENTER )&&( key != K_EXIT )&&
              ( key != K_RIGHT )&&( key != K_LEFT );
	      key = toupper(getch()))
	      ;
          defocus_mouse();
          focus_mouse();
          if (( key == hotkey )||( key == K_ENTER )){
             button_click( i, BUTTON_COLS, but_del, color );
             if ( tag.status == S_DELETED ){
                tag.status = S_NORMAL;
             }
             else{
                tag.status = S_DELETED;
             }
             write_char_xy( c.r.x - 2, c.l.y + 2, BLACK_ON_CYAN, K_SPACE );

             //-------------
             // Get location for the tag.
             fseek( data, (long)sizeof(tag) * tag_num, SEEK_SET);

             // Write tag.
             if ( fwrite( &tag, sizeof(tag), 1, data ) != 1 ){
                run_time_errors(12);      // Could not save disk information.
                fclose(data);
                defocus_mouse();
                // Closing the window.
                destruct_box( c, &psa );
                M_layer = M_prev;
                return FALSE;
             }
          }
          else
          if ( key == K_RIGHT ){
             tag_num++;
             if ( tag_num == tag_total_num ) // tag_nums = 0..total - 1.
                tag_num = 0;

             // Delete the old tag.
             clear_tag_window(c);

          }
          else
          if ( key == K_LEFT  ){
             tag_num--;
             if ( tag_num < 0 )
                tag_num = tag_total_num - 1;

             // Delete the old tag.
             clear_tag_window(c);
          }
    //-------------
    }
    fclose(data);

    // Reorganize the data file and backup it.
    reorganize(DATA_FILE);
    back_up_data( DATA_FILE, BU_DAT_FILE );

    // Reindex the index and backup it.
    reindex( DATA_FILE, INDEX_FILE );
    reindex( DATA_FILE, BU_NDX_FILE );

    // defocus_mouse();   - Is inside the last loop iteration.
    // Closing the window.
    destruct_box( c, &psa );
    M_layer = M_prev;
    return TRUE;
}

/****************************************************************************/
			 /* Help database modules.*/

// Converts Help "topic" number to its litteral topic -> "caption" argument.
void topic_to_caption( int topic, char *caption )
    {
    switch (topic) {

           case 1  : copy_string( caption, "    Hot Keys    " );
                     break;

           case 2  : copy_string( caption, "Mashitup Guide ¡" );
                     break;

           case 3  : copy_string( caption, "Mashitup Guide ¢" );
                     break;

           case 4  : copy_string( caption, "Mashitup Guide £" );
                     break;
           case 5  : copy_string( caption, "Mashitup Guide ¤" );
                     break;

           case 6  : copy_string( caption, "Mashitup Wizards" );
                     break;

           case 7  : copy_string( caption, "   Mouse Help   " );
                     break;

           case 8  : copy_string( caption, "Mashitup Credits" );
                     break;

           default : copy_string( caption, "  General Help  " );
                     break;
    }
    return;
}

/****************************************************************************/

// Output information from Mashitup help file according to argument topic.
// Return True if loading and writing process terminated successfully.
boolean write_topic_info( int topic, char color )
    {
    char line[81] = {0};
    coord_t c;
    FILE *fl = fopen( HELP_FILE, "rt" );

    // Check if file exists.
    if ( fl == NULL ){
       run_time_errors(27);                     // Help file not found.
       return FALSE;
    }

    // Find the appropriate topic in the file.
    if (!find_help_topic( fl, topic )){
       fclose(fl);
       return FALSE;
    }

    // Write the topic to the screen.
    c.x = 7;
    c.y = 6;
    // Read first info line.
    if ( fgets( line, 81, fl ) == NULL ){
       run_time_errors(28);                // Help file is corrupted.
       fclose(fl);
       return FALSE;
    }

    // Write the help caption.
    if (strncmp( line, NEW_HELP_TOPIC, length_of(NEW_HELP_TOPIC))&&
        strncmp( line, EOF_HELP_TOPIC, length_of(EOF_HELP_TOPIC))){
       // Write the line, without carriage return.
       line[length_of(line) - 1 ] = '\0';
       set_bold_digits(line);
       write_string_xy( c.x, c.y, YELLOW_ON_BLUE, line );
       c.y++;
       // Read next info line.
       if ( fgets( line, 81, fl ) == NULL ){
          run_time_errors(28);
          fclose(fl);
          return FALSE;
       }
    }

    // While not new topic.
    while (strncmp( line, NEW_HELP_TOPIC, length_of(NEW_HELP_TOPIC))&&
           strncmp( line, EOF_HELP_TOPIC, length_of(EOF_HELP_TOPIC))){
          // Write the line, without carriage return.
          line[length_of(line) - 1 ] = '\0';
          set_bold_digits(line);
          write_string_xy( c.x, c.y, color, line );
          c.y++;
          // Read next info line.
          if ( fgets( line, 81, fl ) == NULL ){
             run_time_errors(28);
             fclose(fl);
             return FALSE;
          }
    }
    fclose(fl);
    return TRUE;
}

/****************************************************************************/

// Deletes the information from help window.
// Argument "c" is rectangle to delete, color is with which color.
void delete_topic_info( coords_t c, char color )
    {
    int i,j;

    // Fill the window with spaces.
    c.l.x += 3;
    c.r.x -= 2;
    c.l.y += 3;
    c.r.y -= 4;
    clear_bar( c, color, K_SPACE );
    return;
}

/****************************************************************************/

// Adds page number in the right top according to the topic.
// Arguments ( x, y ) are ccordinates of page number and "color" is its color.
void add_help_page_num( char x, char y, int topic, char color )
    {
    write_char_xy(   x - 16, y,     color, 'Ë'                             );
    write_string_xy( x - 16, y + 1, color, "º Page    Of    "              );
    write_string_xy( x - 16, y + 2, color, "ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍµ"             );
    write_char_xy(   x -  9, y + 1, color, ZERO_ASCII+( topic/10          ));
    write_char_xy(   x -  8, y + 1, color, ZERO_ASCII+( topic%10          ));
    write_char_xy(   x -  3, y + 1, color, ZERO_ASCII+( TOPICS_MAX_NUM/10 ));
    write_char_xy(   x -  2, y + 1, color, ZERO_ASCII+( TOPICS_MAX_NUM%10 ));
    return;
}

/****************************************************************************/

// Performs the Mashitup's help window on argument topic.
// Returns the selected button. 1 = Backward, 2 = Quit help, 3 = Forward.
void make_help_window( int topic )
    {
    pscreen_array screen;
    coords_t c;
    int i = 3;
    char color = WHITE_ON_BLUE;
    char caption[17] = {0};

    // Set help window layer.
    int M_prev = M_layer;
                 M_layer = HELP_WINDOW_LAYER;

    // Draw the window
    c.l.x = 4;
    c.l.y = 3;
    c.r.x = 76;
    c.r.y = 44;
    topic_to_caption( topic, caption );
    screen = draw_box( c, color, caption );

    // Do the help loop.    While user wants to read prev or next page.
    while ( i != 2 ){

          // Make caption on the box frame.
          topic_to_caption( topic, caption );
          write_string_xy( c.l.x + 29, c.l.y, YELLOW_ON_BLUE, caption );
          // Write the help.
          if (!write_topic_info( topic, color )){
             // Restore the screen.
             destruct_box( c, &screen );

             M_layer = M_prev;
             return;
          }

          // Add page number
          add_help_page_num( c.r.x, c.l.y, topic, color );

          // Make buttons and wait for user to proceed.
          {
           coord_t cb[3];
           char but_backwrd[] = " ^<- Backward ";
           char but_exit[]    = " ^Quit Help ";
           char but_forward[] = " Forward -^> ";
           char key_1, key_2, key_3, submit;

           cb[0].x = c.l.x + 11;      cb[0].y = c.r.y - 3;
           cb[1].x = c.l.x + 31;      cb[1].y = c.r.y - 3;
           cb[2].x = c.l.x + 49;      cb[2].y = c.r.y - 3;

           // Draw 3 buttons.
           key_1 = draw_button( cb[0], BUTTON_COLS, but_backwrd, (i == 1));
           key_2 = draw_button( cb[1], BUTTON_COLS, but_exit   , (i == 2));
           key_3 = draw_button( cb[2], BUTTON_COLS, but_forward, (i == 3));

           while ( i < 10 ){
                 focus_mouse();
	         submit = toupper(getch());
	         if ( submit == key_1 )
	            i = 11;
	         else
	         if ( submit == key_2 )
	            i = 12;
	         else
	         if ( submit == key_3 )
	            i = 13;
   	         else
	         switch (submit) {

	                case K_EXIT      : i =  12;   // Exit.
				           break;

	                case K_ENTER     : i += 10;
				           break;

	                case K_EXTENDED  : submit = toupper(getch());
                                           if (( submit == K_LEFT   )||
                                               ( submit == K_SH_TAB )){
                                              i = ( i - 1 ) ? ( i - 1 ) : 3;
                                           }
                                           else
				           if ( submit == K_RIGHT ){
                                              i = ( i % 3 ) + 1;
                                           }
                                           break;

	              case K_TAB   : i = ( i % 3 ) + 1; // 1->2, 2->3, 3->1.
                                           break;
	         }

                 defocus_mouse();

                 if ( i < 10 ){
                    key_1 = draw_button( cb[0], BUTTON_COLS, but_backwrd,
                                         ( i == 1 ));
                    key_2 = draw_button( cb[1], BUTTON_COLS, but_exit   ,
                                         ( i == 2 ));
                    key_3 = draw_button( cb[2], BUTTON_COLS, but_forward,
                                         ( i == 3 ));
                 }
                 else{
                    key_1 = draw_button( cb[0], BUTTON_COLS, but_backwrd,
                                         ( i == 11 ));
                    key_2 = draw_button( cb[1], BUTTON_COLS, but_exit   ,
                                         ( i == 12 ));
                    key_3 = draw_button( cb[2], BUTTON_COLS, but_forward,
                                         ( i == 13 ));
                 }
           } // While.
           i -= 10;

           focus_mouse();
           if ( i == 1 ){
	      button_click( cb[0], BUTTON_COLS, but_backwrd, color );
              write_char_xy( cb[0].x + length_of(but_backwrd) - 1, cb[0].y,
                             BLACK_ON_BLUE, DN_SHADOW );
              // Clear window for next topic.
              delete_topic_info( c, color );

              // Previous topic.
              topic = ( topic - 1 ) ? ( topic - 1 ) : TOPICS_MAX_NUM;
           }
           else
           if ( i == 2 ){
	      button_click( cb[1], BUTTON_COLS, but_exit,    color );
           }
           else{
	      button_click( cb[2], BUTTON_COLS, but_forward, color );
              write_char_xy( cb[2].x + length_of(but_forward) - 1, cb[2].y,
                             BLACK_ON_BLUE, DN_SHADOW );
              // Clear window for next topic.
              delete_topic_info( c, color );

              // Next topic.
              topic = topic % TOPICS_MAX_NUM + 1;
           }
          }
          defocus_mouse();

    } // Help loop.

    // Restore the screen.
    defocus_mouse();
    destruct_box( c, &screen );

    M_layer = M_prev;
    return;
}

/****************************************************************************/

// Make mashitup Help.
void Mashitup_help(void)
    {
    int topic = 2;

    // Do help.
    make_help_window(topic);
    return;
}

/****************************************************************************/

// Makes hotkeys separate page.
// Works almost as make_help_windwo(), but shows only one hotkeys page.
void hot_keys_page(void)
    {
    pscreen_array screen;
    coords_t c;
    char color = WHITE_ON_BLUE;
    char caption[17] = {0};

    coord_t cb;
    char but_exit[]    = " ^Quit Help ";
    char key_2, submit;

    // Set hot keys page layer.
    int M_prev = M_layer;
                 M_layer = HOTKEYS_PAGE_LAYER;

    // Draw the window
    c.l.x = 4;
    c.l.y = 3;
    c.r.x = 76;
    c.r.y = 44;
    topic_to_caption( HOT_KEYS_TOPIC, caption );
    screen = draw_box( c, color, caption );

    // Make caption on the box frame.
    write_string_xy( c.l.x + 29, c.l.y, YELLOW_ON_BLUE, caption );

    // Write the help.
    if (!write_topic_info( HOT_KEYS_TOPIC, color )){
       // Restore the screen.
       destruct_box( c, &screen );

       M_layer = M_prev;
       return;
    }

    // Make buttons and wait for user to proceed.
    cb.x = c.l.x + 31;
    cb.y = c.r.y - 3;
    // Draw the buttons, and wait for it being clicked.
    key_2 = draw_button( cb, BUTTON_COLS, but_exit, TRUE );

    focus_mouse();
    for ( submit = toupper(getch());
	  ( submit != key_2 )&&( submit != K_ENTER )&&( submit != K_EXIT );
	  submit = toupper(getch()))
	;
    button_click( cb, BUTTON_COLS, but_exit, color );

    // Restore the screen.
    defocus_mouse();
    destruct_box( c, &screen );

    M_layer = M_prev;
    return;
}

/****************************************************************************/
/* EOF. */
