/****************************************************************************/
/********************************  Basic.H  *********************************/
/****************************************************************************/
// This file includes precompiler definitions abstract data types for
// Mashitup.
// Basic.H is not included in the project, it is used through "include".
// Basic.H size is XX.X KB.

#ifndef BASIC_H
#define BASIC_H

/*******************  Mashitup information definitions **********************/

#define VERSION          "2.3"         // Mashitup's version.
#define DATE      "06.15.2001"         // Mashitup's completion date.

/***************************  Mashitup files ********************************/

#define FONT_FILE          "Data\\Mashitup.Fnt"  // Path to Font  file.

#define INDEX_FILE         "Data\\DiskList.Ndx"  // Path to Index file.
#define DATA_FILE          "Data\\DiskList.Dat"  // Path to Data  file.

#define HELP_FILE          "Data\\Mashitup.Hlp"  // Path to Help  file.

// Backup files:

#define BU_NDX_FILE     "Data\\BackUp\\DiskList.Ndx"   // Index backup file.
#define BU_DAT_FILE     "Data\\BackUp\\DiskList.Dat"   // Data  backup file.

// Macro that checks if file is for backup or not:

#define MAC_is_backup_file(name) if ( !(strcmp((name),BU_NDX_FILE)) || \
                                      !(strcmp((name),BU_DAT_FILE)))

// Reindex assistant files:

#define NEW_FILE           "Data\\___New__.New"  // Path to New   file.
#define BREAK_FILE         "Data\\___BREAK.Brk"  // Path to Break file.

// Reorganization assistant files:

#define REORG_FILE         "Data\\___Reorg.Org"  // Path to org   file.

// Skins file name token.

#define SKIN_FILE          "Skins\\Mp0.Skn"      // Path to skin file.

/*********************  Defines for actual procedures  **********************/

// CD-ROM status errors

#define NO_CD_ERROR   (0x810C)         // Error code when no cd in drive.
#define CD_OK_ERROR   (0x0100)         // Error code when cd is ok.
#define BUSY             (512)         // Error when CD is busy.

// Comands to CD-ROM tray.

#define EJECT	 	   (0)
#define CLOSE	           (5)

// Different delays.

#define ROTARY_DELAY     (800)         // Disk read time = 8[sec].

// Sometimes ( mostly in tracks with high address ) CD-ROM reads the 00:00
// time of current track as total playing time of the previous track.
// The error occurs because the address of end of track and begining of the
// next track is the same byte. To prevent this error I use a small delay
// Which allows the CD head to read next bytes in order to recognize the
// true time counter.
// On fast computers this delay does not needed.
#define P_TIME_DELAY       (0)         // Time counters update = 0.0[sec].


#define CONT_BUT_DELAY   (120)         // Delay of control button's click.

// Fade delay determins the rate of screen fade in/out. (0) Is the fastest
// fade.
#define FADE_DELAY        (0)         // Screen fading out/in speed.

// Constants that are used to determine the type of track.

#define TRACK_MASK       (208)         // Mask for track's attributes.
#define DATA_TRACK        (64)         // Attribute of data track.

// Door lock system attributes.

#define LOCK               (1)
#define UNLOCK             (0)

// Playing modes ( Logical property ).

#define PLAY_REGULAR       (0)
#define PLAY_REPEAT        (1)
#define PLAY_RANDOM        (2)
#define PLAY_STOP          (8)         // Disactivates repeat or random.

// Values of executer in request. Sometimes executer called "command code"
// In IOCTL talk.

#define GET_INFORMATION    (3)
#define SET_INFORMATION   (12)         // 3 - IOCTL input, 12 - IOCTL output.
#define SEEK             (131)
#define PLAY             (132)
#define STOP             (133)
#define RESUME           (136)

// Defines for disk tag.

#define MAX_TRACKS        (32)         // Maximal number of tracks.
#define T_NAME_LENGTH     (21)         // Maximal length of tracks name.
#define A_NAME_LENGTH     (21)         // Maximal length of artist name
#define G_NAME_LENGTH     (11)         // Maximal length of disk genre.

// Defines for disk tag status.

#define S_NORMAL          (0)         // Normal tag.
#define S_DELETED         (1)         // Deleted tag.

#define T_BLOCKED         (0)         // Track cannot be played.
#define T_PLAYABLE        (1)         // Track can be played.
#define T_DATA            (2)         // Track cannot be played.

// Mashitup help defines.
// NEW_HELP_TOPIC, EOF_HELP_TOPIC are strings that appear in help file.

#define NEW_HELP_TOPIC    "### New Topic ###"    // Sign of new help topic.
#define EOF_HELP_TOPIC    "<< End of file >>"    // Sign of help EOF.
#define TOPICS_MAX_NUM        (8)                // Number of help topics.

#define HOT_KEYS_TOPIC        (1)                // Hotkeys topic number.


/*********************  Defines for visual procedures  **********************/

// ASCII constants.
// 256 characters * (8*8 Font size ) = 16384 Bits = 2048 Bytes.

#define ASCII_TABLE        (2048)      // Size of ascii table.
#define ZERO_ASCII          (160)      // Ascii of zero digit(0).

#define SKINS_MAX_NUM         (3)      // Number of skins.

// Mouse layers constants.
// Each layer has uniqe properties as buttons, for example, or equalizer
// sound trails.

#define NO_MOUSE_LAYER       (-1)      // Layer without mouse methods.
#define MAIN_LAYER            (0)      // Mashitup main layer.
#define RUN_ERR_LAYER         (1)      // Run time errors layer.
#define LOGO_WINDOW_LAYER     (3)      // Layer of logo window.
#define NO_DISK_LAYER         (4)      // Layer of no disk window.
#define STOP_A_CHOICE_LAYER   (5)      // Stop audio choice layer.
#define EDIT_TAG_LAYER        (6)      // Edit tag wizard layer.
#define DELETE_TAG_LAYER      (7)      // Delete tag wizard layer.
#define HELP_WINDOW_LAYER     (8)      // Mashitup help window layer.
#define ABOUT_WINDOW_LAYER    (9)      // About window layer.
#define HOTKEYS_PAGE_LAYER   (10)      // Hotkeys help page layer.

// Screen constants.
#define SCREEN_SEG   (0xB8000000)      // Hardware screen segment address.
#define VGASCR_SEG   (0xA0000000)      // VGA      screen segment address.
#define MAX_X                (80)      // Screen size values.
#define MID_X                (40)
#define MIN_X                 (0)
#define MAX_Y                (50)
#define MID_Y                (25)
#define MIN_Y                 (0)

// Main color defines :

   // Colors of error windows.
   #define ERR_COLS        (0x4E)                // Yellow on Red.
   // Colors of warning windows.
   #define WARN_COLS       (0x5E)                // White on Magenta.
   // Colors of Mashitup background.
   #define MBG_COLS        (0x17)                // Grey   on Blue.
   // Colors of standard buttons.
   #define BUTTON_COLS     (0x20)                // Black  on Green.
   // Colors of playing progress bar
   #define PROG_BAR_COLS   (0x30)                // Black  on Cyan.


// Color palettes.

#define BLACK_ON_BLUE      (0x10)
#define RED_ON_BLUE        (0x14)
#define YELLOW_ON_BLUE     (0x1E)
#define WHITE_ON_BLUE      (0x1F)
#define RED_ON_GREEN       (0x24)
#define BLACK_ON_CYAN      (0x30)
#define BLUE_ON_CYAN       (0x31)
#define RED_ON_CYAN        (0x34)
#define BROWN_ON_CYAN      (0x36)
#define LGRAY_ON_CYAN      (0x37)
#define DGRAY_ON_CYAN      (0x38)
#define YELLOW_ON_CYAN     (0x3E)
#define WHITE_ON_CYAN      (0x3F)
#define BRED_ON_CYAN       (0xB4)                // Blinking Red on Cyan.

// Hotkeys for play mode.
// This hotkeys allows to activate Mashitup using keyboard. Their organized
// list may be viewed in "Alt+H" Mashitup option.

#define DEC_VOLUME_KEY        '-'      // Key for decreasing the volume.
#define INC_VOLUME_KEY        '+'      // Key for increasing the volume.
#define MAX_VOLUME_KEY        '*'      // Key for maximizing the volume.
#define MIN_VOLUME_KEY        '/'      // Key for minimizing the volume.

#define PLAY_RANDOM_KEY       '?'      // Hotkey for "Play Random" key.
#define PLAY_REPEAT_KEY   (char)8      // Hotkey for "Play Repeat" key.

#define EJECT_BUTTON_KEY      'E'      // Key for ejecting CD-ROM tray.

#define PLAY_BUTTON_KEY  (char)13      // Key for playing  the music.
#define PAUSE_BUTTON_KEY      'P'      // Key for pausing  the music.
#define STOP_BUTTON_KEY  (char)32      // Key for stopping the music.

#define MARK_TRACK_KEY   (char)83      // Key for (un/ ) blocking the track.

#define LOCK_CD_KEY           'L'      // Key for (un/ ) locking the CD-ROM.

#define SP_HAMMER_KEY    (char)91      // Key for special hammer  effect.
#define SP_WORDUP_KEY    (char)93      // Key for special word up effect.

#define BACK_SEC_KEY     (char)75      // Key for 5 seconds back.
#define FORW_SEC_KEY     (char)77      // Key for 5 seconds forward.

#define K_TIME_DIR       (char)96      // Key for changing time direction.
#define K_SOUND_REF      (char)125     // Key for refreshing equalizer.
#define K_PPBAR_REF      (char)126     // Key for refreshing playing
                                       // progress bar.

#define K_SCREEN_SAV          'S'      // Key for screen saver.

// Bar options.

#define SET_ORIG_SKIN_O  (char)48      // Key for setting the original font.
#define REGISTER_DISK_O  (char)19      // Disk registration option.
#define REG_MOUSE_DSK_O  (char)(19-1)  // Mouse cannot send Alt+R character
                                       // because it is system reserved.
#define DELETE_EDITOR_O  (char)32      // Disk tag deleting option.

// Frame characters.

#define HD_BAR    'Õ'                  // Bold frame characters.
#define VD_BAR    '∫'
#define LU_DCR    '…'
#define LD_DCR    '»'
#define RU_DCR    'ª'
#define RD_DCR    'º'
#define LD_SEP    'π'
#define RD_SEP    'Ã'
#define LM_SEP    '∂'
#define RM_SEP    '«'
#define HS_BAR    'ƒ'                  // Thick frame characters.
#define VS_BAR    '≥'
#define LU_SCR    '⁄'
#define LD_SCR    '¿'
#define RU_SCR    'ø'
#define RD_SCR    'Ÿ'
#define LS_SEP    '¥'
#define RS_SEP    '√'

// Special characters.

#define UP_SHADOW             'ﬂ'      // Up shadow.
#define DN_SHADOW             '‹'      // Down shadow.
#define HOTKEY                '^'      // Hotkey character.
#define M_BGROUND             (char)15 // Mashitup background character.

// While editing disk tag EDIT_BG_CHAR fills the pattern where you type.
#define EDIT_BG_CHAR          '˘'      // Char that symbolizes an empty char.

#define TRACK_FINGER         "èê"      // Finger which points on tracks.
#define EMPTY_FINGER         "∫ "      // Deleted tracks finger.
#define BLOCKED_POINT         'ë'      // Blocked track indication point.
#define DATA_POINT            'ô'      // Data    track indication point.

#define R_T_BORDER            'í'      // Sound trail right border.
#define   T_SCALE             'î'      // Sound trail inside scale.
#define   T_MODIF             'ï'      // Sound trail scale modifier.
#define L_T_BORDER            'ì'      // Sound trail left  border.

#define MOUSE_R_SHAPE         170      // Shape of Mashitup's released mouse.
#define MOUSE_P_SHAPE         172      // Shape of Mashitup's pressed  mouse.

// Play progress bar definitions.
#define PROGRESS_LENGTH      (34)      // Length of play progress bar.
#define FIRST_CELL          (229)      // ASCII code of first cell.
#define FULL_CELL           (236)      // ASCII code of full cell.

// Mouse click cannot send regular extended character so it immitates, by
// sending K_MOUSE_EXTENDED character.
#define K_EXTENDED       (char)0      // Action keys.
#define K_MOUSE_EXTENDED       1

#define K_TAB             (char)9
#define K_SH_TAB         (char)15     // Shift+Tab.

#define K_ENTER          (char)13
#define K_EXIT           (char)27
#define K_SPACE          (char)32

#define K_BACK_SPACE      (char)8
#define K_DELETE         (char)83

#define K_UP             (char)72     // Arrows.
#define K_LEFT           (char)75
#define K_RIGHT          (char)77
#define K_DOWN           (char)80
#define K_PGUP           (char)73
#define K_PGDOWN         (char)81
#define K_HOME           (char)71
#define K_END            (char)79

#define ALT_R            (char)17
#define ALT_D            (char)32
#define ALT_H            (char)35
#define ALT_X            (char)45

#define CTRL_Y           (char)25

#define F1               (char)59
#define F2               (char)60

/********************************/

// Coordinates of Mashitup windows.

#define TT_L_X                (0)      // Coordinates of Tracks panel.
#define TT_L_Y                (2)
#define TT_R_X               (37)
#define TT_R_Y               (45)

#define ST_L_X               (41)      // Coordinates of Status panel.
#define ST_L_Y                (2)
#define ST_R_X               (76)
#define ST_R_Y                (9)

#define VT_L_X               (39)      // Coordinates of Volume panel.
#define VT_L_Y               (12)
#define VT_R_X               (78)
#define VT_R_Y               (26)

#define CT_L_X               (39)      // Coordinates of Control panel.
#define CT_L_Y               (29)
#define CT_R_X               (78)
#define CT_R_Y               (45)

#define RT_L_X               (10)      // Coordinates of Register panel.
#define RT_L_Y               (2)
#define RT_R_X               (70)
#define RT_R_Y               (45)

// Appearence of control panel buttons.

#define CONTROL_KEYS_NUM   (9)         // Number of buttons on control panel.
#define REWIND_BUT      " ÄÅ "         // Control buttons captions.
#define PLAY_BUT         " Ö "
#define PAUSE_BUT        " Ñ "
#define STOP_BUT         " Ü "
#define FORWARD_BUT     " ÇÉ "

#define RANDOM_BUT      " ?? "
#define REPEAT_BUT      " áà "
#define UNLOCK_BUT      " çé "
#define LOCK_BUT        " ãå "
#define EJECT_BUT       " Üâ "

#define SP_HAMMER_BUT   " ä "          // Special hammer effect  button.
#define SP_WORDUP_BUT   " word up "    // Special word up effect button.

#define BACK_SEC_BUT    " ñ "          // Backward 5 seconds button.
#define FORW_SEC_BUT    " ó "          // Forward  5 seconds button.

/****************************************************************************/
/***************************  Types declarations  ***************************/

// Used for storing information about current track.
//  - musicinfo_t;

/****************************************************************************/

// Used to store information about volume.
//  - volumeinfo_t;

/****************************************************************************/

// Used to handle all volume functions.
//  - volume_handler;

/****************************************************************************/

// Used to store information about CD-ROM drive and CD disk.
//  - cdrom_data_t;

/****************************************************************************/

// Used for ambivalent variables.
//  - boolean;

/****************************************************************************/
// Used for ambivalent track time counter.
// - play_time_t;

/****************************************************************************/

// Type which represents coordinates of point.
//  - coord_t;

/****************************************************************************/

// Type which represents coordinates of rectangle.
//  - coords_t;

/****************************************************************************/

// Types which is representing the screen segment.
//  - screen_array[MAX_X][MAX_Y];
//  - pscreen_array;
//  - VGA_t;
//  - c_palette_t;

/****************************************************************************/

// Type which is representing disks database index single node.
//  - index_t;

/****************************************************************************/

// Type which is representing single song tag.
//  - track_id;
/****************************************************************************/

// Type which is representing disk tag ( single record in disks data file ).
//  - disk_tag;
/****************************************************************************/

// Type which is representing the information about mouse pressed or released
// button.
//  - button_event_t;

/****************************************************************************/

// Type which is representing the information of that is passed to mouse
// hanler procedure.
//  - handler_t;

/****************************************************************************/
// Type which is representing mouse movements. ( moved_x == TRUE ) means that
// mouse has changed it x coordinate. "moved" means x or y changes.
//  - m_move_t;

/****************************************************************************/

// Type which is representing mouse buttons dragging. ( r == TRUE ) means that
// currently right button is dragging an item.
//  - m_drag_t;

/****************************************************************************/
/****************************************************************************/
/**************************  Types implementation  **************************/

// Used for storing information about current track.
typedef struct {
               unsigned char control;       // Media descriptor ( 12 ).
               unsigned char unused1;
               unsigned char track;         // Number of current track.
               unsigned char unused2;
               unsigned char track_min;     // Relative position in min:sec:
               unsigned char track_sec;     // frames - which current track
               unsigned char track_frames;  // was played up to.
               unsigned char unused4;
                        char disk_min;      // Absolute position of CD-ROM
                        char disk_sec;      // head in min:sec:frames.
               unsigned char disk_frames;
        } musicinfo_t;

/****************************************************************************/

// Used to store information about volume.
typedef struct {
               unsigned char mode;      // ( 4 = get, 3 = set ) the sound.
               unsigned char input0;    // Index  of sound channel.
               unsigned char volume0;   // Volume of sound channel.
               unsigned char input1;    // Index  of sound channel.
               unsigned char volume1;   // Volume of sound channel.
               unsigned char input2;    // Index  of sound channel.
               unsigned char volume2;   // Volume of sound channel.
               unsigned char input3;    // Index  of sound channel.
               unsigned char volume3;   // Volume of sound channel.
        } volumeinfo_t;

/****************************************************************************/

// Used to handle all volume functions.
typedef struct {
               volumeinfo_t vol;    // Volume data record.
               int          aic;    // Number of active channels ( out of 4 ).
        } volume_handler;

/****************************************************************************/

// Used to store information about CD-ROM drive and CD disk.
typedef struct {
               unsigned short num_drives;         // Number of CD-ROM drives.
               unsigned char  first_drive;        // Drive which I use.
               unsigned char  current_track;      // Currently celected track.
               unsigned long  track_position;     // Offset of the track.
               unsigned char  track_type;         // ( Audio / Data ).
               unsigned char  min_track;          // Minimal track (0).
               unsigned char  max_track;          // Index of maximal track.
               unsigned char  disk_length_min;    // Length of the whole disk
               unsigned char  disk_length_sec;    // in min:sec:frames format.
               unsigned char  disk_length_frames;
               unsigned long  end_of_disk;        // Absolute address.
               unsigned char  unused[7];
               unsigned char  diskid[6];          // ID string of the disk.
               unsigned long  status;             // Status of disk and drive,
               unsigned short error;              // Like ( CD disk is in but
        } cdrom_data_t;                           // it is not played ).

/****************************************************************************/

// Used for ambivalent variables.
typedef enum boolean { FALSE , TRUE } boolean;

/****************************************************************************/

// Used for ambivalent track time counter.
typedef enum play_time_t { passed , remaining } play_time_t;

/****************************************************************************/

// Type which represents coordinates of point.
typedef struct {
	       int x;              // X coordinate.
	       int y;              // Y coordinate.
	} coord_t;

/****************************************************************************/

// Type which represents coordinates of rectangle.
typedef struct {
	       coord_t l;          // left.
	       coord_t r;          // right.
	} coords_t;

/****************************************************************************/

// Types which is representing the screen segment and colors palette.
typedef unsigned screen_array;
typedef screen_array* pscreen_array;

typedef unsigned char far *VGA_t;
typedef unsigned char **c_palette_t;

// For allocating 8KB for each window, use that typedef fo screen_array:
// typedef unsigned screen_array[MAX_X][MAX_Y];

/****************************************************************************/

// Type which is representing disks database index single node.
typedef struct {
               char disk_id[6];          // Index key.
               int  pointer;             // Pointer to data file.
        } index_t;

/****************************************************************************/

// Type which is representing single song tag.
typedef struct {
               char name[T_NAME_LENGTH];
               int  status;
               unsigned char track_min;
               unsigned char track_sec;
               unsigned char track_frame;
        } track_id;

/****************************************************************************/

// Type which is representing disk tag ( single record in disks data file ).
typedef struct {
               int status;
               char disk_id[6];
               char artist[A_NAME_LENGTH];    // Name of artist.
               char genre[G_NAME_LENGTH];     // Disk genre.
               char album[A_NAME_LENGTH];     // Album name.
               char year[5];                  // Year of album.
               track_id tracks[MAX_TRACKS];   // Tracks list.
               char songs_num;                // Number of songs.
               char unused[29];               // For future development
        } disk_tag_t;

/****************************************************************************/

// Type which is representing the information about mouse pressed or released
// button.
typedef struct {
	       int butt_stat;    // Status of all buttons.
	       int count;        // Number of times that button event occured.
	       int x;            // X coordinate where   button event occured.
	       int y;            // X coordinate where   button event occured.
	} button_event_t;

/****************************************************************************/

// Type which is representing the information of that is passed to mouse
// hanler procedure.
typedef struct {
	       int mask_status;
	       int butt_stat;    // Status of all buttons.
	       int x;            // X coordinate where button event occured.
	       int y;            // X coordinate where button event occured.
	       int hor_count;    // horizontal delta mickeys.
	       int ver_count;    // vertical   delta mickeys.
	} handler_t;


/****************************************************************************/

// Type which is representing mouse movements. ( moved_x == TRUE ) means that
// mouse has changed it x coordinate. "moved" means x or y changes.
typedef struct {
               boolean moved_x;  // Has X   coordinate changed.
               boolean moved_y;  // Has Y   coordinate changed.
               boolean moved  ;  // Has Any coordinate changed.
        } m_move_t;

/****************************************************************************/

// Type which is representing mouse buttons dragging. ( r == TRUE ) means that
// currently right button is dragging an item.
typedef struct {
               boolean l;        // Is left   button dragging.
               boolean r;        // Is right  button dragging.
               boolean c;        // Is center button dragging.
        } m_drag_t;

/****************************************************************************/

#endif

/****************************************************************************/
/* EOF. */