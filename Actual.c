/****************************************************************************/
/********************************  Actual.C  ********************************/
/****************************************************************************/
// This file includes modules for all Mashitup playing and basic functions.
// Actual.C is included in the project.
// Actual.C size is XX.X KB.

/****************************************************************************/

#include <dos.h>        // Needed for REGS union and int86x().
#include <stdio.h>      // Needed for FILE declaration.
#include "basic.h"

/***************************** Global variables *****************************/

extern cdrom_data_t cd_data;
static union REGS registers;

/****************************************************************************/
/**************************  Modules declarations  **************************/

// Check if CD-ROM drive is installed on computer, returns True if does.
// Parameters    : None.
// Side affects  : Fills the cd_data variable with appropriate information.
// Modules Calls : cd_get_audio_info();
boolean is_cdrom_installed(void);

/****************************************************************************/

// Basic function of Mashitup. Gets block "where", fills it with
// appropriate information and performs hardware functions.
// This module is called "talk to MicroSoft CDrom EXtension ( MSCDEX )".
// Parameters    : where.
// Side affects  : None.
// Modules Calls : None.
void cdrom_request( void *where );

/****************************************************************************/

// Convert CD-ROM position to minutes and seconds using Red Book addressing.
// Red Book returns dword as following -> "00"- nibble, minutes nibble,
// seconds nibble, frames nibble: [00MMSSFF].
// Returns       : minutes in min argument, seconds in sec and frames in
//                 frames.
// Parameters    : value, min, sec, frames.
// Side affects  : None.
// Modules Calls : None.
void val_to_time( unsigned long value, unsigned char *min ,
		  unsigned char *sec , unsigned char *frame );

/****************************************************************************/

// Convert CD-ROM position to frames using HSG addressing and returns it.
// HSG addressing retuns the physical frame address according to dword value.
// It is converted to min:sec:frames using Red Book, afterwards address is
// calculated: frames + ( sec * 75 ) * 1 + ( min * 75 ) * 60 - 150.
// Parameters    : value.
// Side affects  : None.
// Modules Calls : val_to_time().
unsigned long val_to_frames( unsigned long value );

/****************************************************************************/

// Returns the position of CD-ROM head as frames offset from current song,
// according to musicinfo_t structure "m" variable. The method is HSG.
// ( m->track_min * 60 ) + m->track_sec ) * 75 + m->track_frames;
// Parameters    : m.
// Side affects  : None.
// Modules Calls : None.
unsigned long cd_head_position( musicinfo_t *m );

/****************************************************************************/

// Get current status of CD-ROM ( to cd_data global variable ).
// Parameters    : None.
// Side affects  : cd_data.status and cd_data.error are changed.
// Modules Calls : cdrom_request().
void cd_status(void);

/****************************************************************************/

// Get information about current track and CD error.
// Parameters    : None.
// Side affects  : Fills cd_data variable with appropriate information.
// Modules Calls : cdrom_request().
void cd_get_audio_info(void);

/****************************************************************************/

// Locks and unlocks CD-ROM drive according to "action" parameter.
// Parameters    : action ( LOCK / UNLOCK ).
// Side affects  : cd_data.error is changed.
// modules Calls : cdrom_request().
void cd_lock( unsigned char action );

/****************************************************************************/

// Opens and closes CD-ROM door according to "action" parameter.
// Parameters    : action ( EJECT / CLOSE ).
// Side affects  : cd_data.error is changed.
// modules Calls : cdrom_request().
void move_tray( unsigned char action );

/****************************************************************************/

// Check if CD-ROM is currently busy, and returns True if does.
// Parameters    : None.
// Side affects  : cd_data.status and cd_data.error are changed.
// modules Calls : cd_status(), move_tray(), make_rdelay_message().
boolean is_cd_busy(void);

/****************************************************************************/

// Set CD-ROM head to parameter location.
// Parameters    : location.
// Side affects  : cd_data.error is changed.
// modules Calls : cdrom_request().
void cd_seek( unsigned long location );

/****************************************************************************/

// Return information of current position ( like disk playing time, current
// track number, e.t.c. ) in info argument.
// Parameters    : info.
// Side affects  : cd_data.error is changed.
// modules Calls : cdrom_request().
void cd_tell_pos( musicinfo_t *info );

/****************************************************************************/

// Set track to be current track on CD disk.
// Parameters    : tracknum.
// Side affects  : Updates appropriately the cd_data variable.
// modules Calls : cdrom_request().
void cd_set_track( int tracknum );

/****************************************************************************/

// Count the length of single track in bytes and returns it.
// Parameters    : tracknum.
// Side affects  : Updates appropriately the cd_data variable.
// modules Calls : cd_set_track().
unsigned long get_track_byte_length( int tracknum );

/****************************************************************************/

// Count the time length of single track and returns it in min:sec:frames.
// Parameters    : tracknum, min, sec, frames.
// Side affects  : Updates appropriately the cd_data variable.
// modules Calls : get_track_byte_length().
void get_track_time_length( int tracknum, unsigned char *min,
			    unsigned char *sec, unsigned char *frame );

/****************************************************************************/

// Gets the current 4 channels of sound into "vol" structure.
// Parameters    : vol.
// Side affects  : cd_data.error is changed.
// modules Calls : cdrom_request().
void cd_get_volume( volumeinfo_t *vol );

/****************************************************************************/

// calculates the number of active sound channels ( out of 4 supported ).
// Parameters    : volhdr.
// Side affects  : cd_data.error is changed.
// modules Calls : max_volume(), cd_get_volume().
void aic_calc( volume_handler *volhdr );

/****************************************************************************/

// Sets the current 4 channels of sound from "vol" structure.
// Parameters    : vol.
// Side affects  : cd_data.error is changed.
// modules Calls : cdrom_request().
void cd_set_volume( volumeinfo_t *vol );

/****************************************************************************/

// Decrease the volume by 8.
// Parameters    : volume.
// Side affects  : cd_data.error is changed.
// modules Calls : cd_set_volume().
void dec_volume( volumeinfo_t *volume );

/****************************************************************************/

// Increase the volume by 8.
// Parameters    : volume.
// Side affects  : cd_data.error is changed.
// modules Calls : cd_set_volume().
void inc_volume( volumeinfo_t *volume );

/****************************************************************************/

// Set the current 4 channels of sound to maximum ( 255 ).
// Parameters    : volume.
// Side affects  : cd_data.error is changed.
// modules Calls : cd_set_volume().
void max_volume( volumeinfo_t *volume );

/****************************************************************************/

// Mute the sound.
// Parameters    : volume.
// Side affects  : cd_data.error is changed.
// modules Calls : cd_set_volume().
void mute_volume( volumeinfo_t *volume );

/****************************************************************************/

// Play disk between start and finish position.
// Parameters    : start, finish.
// Side affects  : cd_data.error is changed.
// modules Calls : cdrom_request().
void cd_play_music( unsigned long start, unsigned long finish );

/****************************************************************************/

// Stop playing.
// Parameters    : None.
// Side affects  : cd_data.error is changed.
// modules Calls : cdrom_request().
void cd_stop_music(void);

/****************************************************************************/

// Start playing after pause.
// Parameters    : None.
// Side affects  : cd_data.error is changed.
// modules Calls : cdrom_request().
void cd_resume_music(void);

/****************************************************************************/

// Returns the position of end of track - specified by tracknum argument.
// position is absolute ( offset from beginning of the disk ).
// Parameters    : tracknum.
// Side affects  : Updates appropriately the cd_data variable.
// modules Calls : cd_set_track().
unsigned long end_of_track( int tracknum );

/****************************************************************************/

// Plays the selected_song. Returns True if managed to play the specified
// "selected song". Otherwise returns False.
// Parameters    : selected song.
// Side affects  : Updates appropriately the cd_data variable.
// modules Calls : run_time_errors(), cd_set_track(), end_of_track(),
//                cd_play_music().
boolean play_song( int selected_song );

/****************************************************************************/

// Performs hammer effect ( like hammer hit the track back while playing ).
// Parameters    : None.
// Side affects  : None.
// modules Calls : cd_stop_music(), cd_resume_music().
void hammer_effect(void);

/****************************************************************************/

// Performs word up effect ( track goes back one word ).
// Parameters    : volume.
// Side affects  : cd_data.error is changed.
// modules Calls : cd_set_volume().
void wordup_effect( volumeinfo_t *volume );

/****************************************************************************/

// While song is playing it jumps "sec" seconds back. "m" argument is needed
// to determine CD head position.
// Parameters    : m.
// Side affects  : None.
// modules Calls : cd_head_position(), cd_stop_music(), cd_resume_music().
void back_sec( int sec, musicinfo_t *m );

/****************************************************************************/

// While song is playing it jumps "sec" seconds forward. "m" argument is
// needed to determine CD head position.
// Parameters    : m.
// Side affects  : None.
// modules Calls : cd_head_position(), end_of_track(), cd_stop_music(),
//                cd_resume_music().
void forward_sec( int sec, musicinfo_t *m );

/****************************************************************************/

// Jumps to starting segment to play inside the song in scope of
// 0..PROGRESS_LENGTH.
// Parameters    : segment.
// Side affects  : None.
// modules Calls : end_of_track(), cd_stop_music(), cd_resume_music().
void jump_inside_song( unsigned char segment );

/****************************************************************************/
			 /* Disks database modules.*/

// Opens index of disks database and reports errors. If function fails to
// open INDEX_FILE it will return NULL.
// Parameters    : None.
// Side affects  : INDEX_FILE File becomes opened at succesful
//                 function termination.
// modules Calls : run_time_errors().
FILE* open_index_file(void);

/****************************************************************************/

// Returns the first index node of "fi" file. If node could not be read
// function will return node.pointer = -1. If end of file, function will
// return               node.pointer = -2.
// Parameters    : fi.
// Side affects  : None.
// modules Calls : run_time_errors().
index_t index_first( FILE *fi );

/****************************************************************************/

// Returns the next index node of "fi" file. If node could not be read
// function will return negative node.pointer.
// Parameters    : fi.
// Side affects  : None.
// modules Calls : run_time_errors().
index_t index_next( FILE *fi );

/****************************************************************************/

// Finds the node in "fl" file according to "disk_id" key, and returns its
// pointer field. Searching scope is:    set <=  file position <= end.
// Parameters    : fi, disk_id, set, end.
// Side affects  : None.
// modules Calls : binary_search(). Performs recursive calls.
static
int binary_search( FILE* fl, char *disk_id, long set, long end );

/****************************************************************************/

// Searches node in index file with key == search_id. If not found - negative
// value will be returned. If found - function will return the serial place
// of the needed disk tag in data file.
// Index file is selected by open_index_file().
// Parameters    : search_id.
// Side affects  : None.
// modules Calls : open_index_file(), file_size(), binary_search().
int find_in_index( char *search_id );

/****************************************************************************/

// Opens file of disks database and reports errors. If function fails to
// open DATA_FILE it will return NULL.
// Parameters    : source_file - path + file name.
// Side affects  : source_file File becomes opened at successful
//                 function termination.
// modules Calls : run_time_errors().
FILE* open_disk_data_file( char *source_file );

/****************************************************************************/

// Retrieves the tag with serial number in the file == pointer. On success
// Returns True, otherwise False.
// For example: if pointer == 5 then retrieve fifth tag from source_file.
// Parameters    : tag, pointer, source file.
// Side affects  : 1. source_file File becomes opened at successfull
//                    function termination.
//                 2. If Index is not matching to the data file all the
//                    database will be deleted.
// modules Calls : run_time_errors().
boolean retrieve_data_tag( disk_tag_t *tag, int pointer, char *source_file );

/****************************************************************************/

// Inits tag of disk with default values and returns it.
// Parameters    : None.
// Side affects  : None.
// modules Calls : get_track_time_length(),
//                Not crucial modules.
disk_tag_t init_disk_tag(void);

/****************************************************************************/

// Loads disk's tag according to its cd_data.disk_id. In case of success tag
// will be returned in "tag" argument and function will return True. In case
// Of failure ( matching tag couldn't be found ) function will return False.
// Locating process of disk tag:
// 1. - Open index file.
// 2. - Using binary search find the index node which points on needed tag.
// 3. - If not found
//    3.1 - tag <- default values.
//    3.2 - Exit.
// 4. - Else ( if found )
//    4.1 - Open data file.
//    4.2 - Locate the needed tag by index node.pointer.
//    4.2 - If located
//           4.21 - tag <- located tag.
//           4.22 - Exit.
//    4.3 - Else ( if not located )
//           4.31 - Report external error ( wrong index ).
//           4.32 - Delete all database files.
//           4.33 - tag <- default values.
//           4.34 - Exit.
//
// When index does not match the data file,  Mashitup can rebuild the data
// file, reindex the index file => and the problem is solved.
// But this error happens only when ( external program / user manually )
// changes the data file. Mushitup prefers not to deal with external
// problems because it may lead to corrupted tag information and therefore
// it deletes the whole database.
// Parameters    : disk_id, tag, source_file.
// Side affects  : If Index is not mathcing to the data file all the database
//                 will be deleted.
// modules Calls : find_in_index(), init_disk_tag(), retrieve_data_tag
boolean load_disk_tag( char *disk_id, disk_tag_t *tag, char *source_file );

/****************************************************************************/

// Returns the status from disk tag "tag".
// Parameters    : tag.
// Side affects  : None.
// modules Calls : None.
int extract_status( disk_tag_t *tag );

/****************************************************************************/

// Returns the artists name from disk tag "tag".
// Parameters    : tag.
// Side affects  : None.
// modules Calls : None.
char* extract_artist( disk_tag_t *tag );

/****************************************************************************/

// Returns the disk genre from disk tag "tag".
// Parameters    : tag.
// Side affects  : None.
// modules Calls : None.
char* extract_genre( disk_tag_t *tag );

/****************************************************************************/

// Returns the album name from disk tag "tag".
// Parameters    : tag.
// Side affects  : None.
// modules Calls : None.
char* extract_album( disk_tag_t *tag );

/****************************************************************************/

// Returns the album year from disk tag "tag".
// Parameters    : tag.
// Side affects  : None.
// modules Calls : None.
char* extract_year( disk_tag_t *tag );

/****************************************************************************/

// Returns the name of "track_num" track from disk tag "tag".
// Attention: Tag has track numbers from 0..31 while CD-Disk from 1..32.
//            Therefore -1 calculation must be done in the current function.
// Parameters    : tag.
// Side affects  : None.
// modules Calls : None.
char* extract_track_name( disk_tag_t *tag, int track_num );

/****************************************************************************/

// Returns the size of opened fl file in bytes.
// Parameters    : fl - opened file.
// Side affects  : None.
// modules Calls : None.
long file_size( FILE* fl );

/****************************************************************************/

// Saves tag to dest_file file.
// Parameters    : tag, disk_id, dest_file.
// Side affects  : None.
// modules Calls : run_time_errors(), find_in_index(), file_size().
boolean save_disk_tag( disk_tag_t *tag, char *disk_id, char *dest_file );

/****************************************************************************/

// Registers the new disk, buckups it and updates the indexes
// Parameters    : tag, disk_id, dest_file.
// Side affects  : None.
// modules Calls : save_disk_tag(), back_up_data(), reindex().
boolean register_disk( disk_tag_t *tag, char *disk_id, char *dest_file );

/****************************************************************************/

// Returns true if file with "file_path" path is empty. Otherwise false.
// It is used on closed files, without reading from it ( unlike feof() ).
// Preconditions : "file_path" file exists and may be accessed.
// Parameters    : file_path.
// Side affects  : None.
// modules Calls : file_size().
boolean is_file_empty( char *file_path );

/****************************************************************************/

// Splits the file with "file_path" path to 2 files "fnew" and "fbreak",
// ( fnew_path and fbreak_path accordingly ).
// If file that was split is fully sorted ( fbreak is empty ) then function
// returns True. If file in unsorted, function returns False.
// Preconditions : "file_path" file exists and may be accessed.
// Parameters    : file_path, fnew_path, fbreak_path ).
// Side affects  : 2 Files are created on hard drive.
// modules Calls : file_size().
static
boolean file_split( char *file_path, char* fnew_path, char* fbreak_path );

/****************************************************************************/

// Merges two files "fnew" and "fbreak" ( f1_path and f2_path accordingly ),
// to the file with "file_path" path.
// Preconditions : "file_path" file exists and may be accessed.
// Parameters    : file_path, f1_path, f2_path.
// Side affects  : None.
// modules Calls : None.
static
void file_merge( char *file_path, char* f1_path, char* f2_path );

/****************************************************************************/

// Algorithm of serial merge sort:
// A loop finds each time series where a[n] < a[n+1]. Each seria is copied
// into fnew file and the breakersof seria are copied to fbreak file.
// Then both files ( fnew and fbreak ) are merged back to main file when
// each time node is being read from fnew or fbreak file and the smaller one
// goes to main file.
// Example:
//  Main file -  A B C      P      K L M      I        E F       .
//              ÀSeriaÙ ÀBreakerÙ ÀSeriaÙ ÀBreakerÙ À Seria Ù

// Sorts the file with "file_path"
// Preconditions: "file_path" file exists and may be accessed.
// Function sorts only index files.
// Parameters    : file_path.
// Side affects  : None.
// modules Calls : is_file_empty(), file_split(), file_merge().
void file_merge_sort( char *file_path );

/****************************************************************************/

// Returns True if index file with "file_path" path is sorted.
// Parameters    : file_path.
// Side affects  : None.
// modules Calls : file_size().
boolean is_file_sorted( char *file_path );

/****************************************************************************/

// Creates unsorted index on data_path file into index_path file.
// Parameters    : data_path, index_path.
// Side affects  : Creates index file on hard drive.
// modules Calls : file_size().
void make_index( char *data_path, char *index_path );

/****************************************************************************/

// Creates sorted index on data_path file into index_path file.
// Parameters    : data_path, index_path.
// Side affects  : Creates index file on hard drive.
// modules Calls : make_index(), is_file_sorted(), file_merge_sort().
void reindex( char *data_path, char *index_path );

/****************************************************************************/

// Reorganizes the disk data file. It is used to clean deleted disks.
// Parameters    : data_path.
// Side affects  : Creates reorg file on hard drive.
// modules Calls : file_size().
void reorganize( char *data_path );

/****************************************************************************/

// Backups the sf file to df file. On succes returns 0;
// Parameters    : sf, df.
// Side affects  : Creates file with df path on hard drive.
// modules Calls : None.
int back_up_data( char *sf, char *df );

/****************************************************************************/

// Gets track, specified by tracknum and changes in the tag variable its
// status. If it was blocked, becomes playable and from playable to blocked.
// Data tracks cannot be blocked or unblocked.
// Parameters    : tracknum, tag.
// Side affects  : None.
// modules Calls : None.
void block_track( int tracknum, disk_tag_t *tag );

/****************************************************************************/

// Return True if track, specified by tracknum is blocked.
// Parameters    : tracknum, tag.
// Side affects  : None.
// modules Calls : None.
boolean is_track_blocked( int tracknum, disk_tag_t *tag );

/****************************************************************************/

// Return True if track, specified by tracknum may be played. It is when track
// is not blocked by the user and it is not DATA track.
// Parameters    : tracknum, tag.
// Side affects  : None.
// modules Calls : is_track_data(), is_track_blocked().
boolean is_track_playable( int tracknum, disk_tag_t *tag );

/****************************************************************************/

// Return True if track, specified by tracknum is data track.
// Parameters    : tracknum, tag.
// Side affects  : None.
// modules Calls : None.
boolean is_track_data( int tracknum, disk_tag_t *tag );

/****************************************************************************/

// Gets current track, specified by tracknum and returns the next track that
// Would be played according to paly_mode and tracks status'.
// If disk nothing has to be played next -1 will be returned.
// Parameters    : tracknum, play_mode, tag.
// Side affects  : None.
// modules Calls : None.
int next_track( int tracknum, int play_mode, disk_tag_t *tag );

/****************************************************************************/
			 /* Help database modules.*/

// Finds the appropriate position of topic in the "fl" file.
// Parameters    : fl, topic.
// Side affects  : None.
// modules Calls : run_time_errors.
boolean find_help_topic( FILE *fl, int topic );

/****************************************************************************/
/****************************************************************************/
/*************************  modules implementation  *************************/

// Check if CD-ROM drive is installed on computer.
boolean is_cdrom_installed(void)
     {
     registers.x.ax = 0x1500;
     registers.x.bx = 0;
     int86( 0x2F, &registers, &registers );
     if (!registers.x.bx)
	return FALSE;
     cd_data.num_drives = registers.x.bx;
     cd_data.first_drive = registers.x.cx;
     cd_get_audio_info();
     return TRUE;
}

/****************************************************************************/

// Basic function of Mashitup. Gets block "where", fills it with
// appropriate information and performes hardware functions.
void cdrom_request( void *where )
    {
    static struct SREGS sregs;

    registers.x.ax = 0x1510;
    registers.x.bx = FP_OFF(where);
    sregs.es = FP_SEG(where);
    registers.x.cx = cd_data.first_drive;
    int86x( 0x2F , &registers , &registers , &sregs );
    return;
}

/****************************************************************************/

// Convert CD-ROM position to minutes and seconds using Red Book addressing.
void val_to_time( unsigned long value, unsigned char *min ,
		  unsigned char *sec , unsigned char *frame )
    {
    *frame = ( value & 0x000000FF );
    *sec   = ( value & 0x0000FF00 ) >> 8;
    *min   = ( value & 0x00FF0000 ) >> 16;
    return;
}

/****************************************************************************/

// Convert CD-ROM position to frames using HSG addressing.
unsigned long val_to_frames( unsigned long value )
    {
    unsigned char min , sec , frame;

    val_to_time( value , &min , &sec , &frame );
    return (unsigned long)min * 4500 + (int)sec * 75 + frame - 150;
}

/****************************************************************************/

// Returns the position of CD-ROM head as frames offset from current song,
// according to musicinfo_t structure "m" variable. The method is HSG.
// ( m->track_min * 60 ) + m->track_sec ) * 75 + m->track_frames;
unsigned long cd_head_position( musicinfo_t *m )
    {
    return (( m->track_min * 60 ) + m->track_sec ) * 75 + m->track_frames;
}

/****************************************************************************/

// Get current status of CD-ROM ( to cd_data global variable ).
void cd_status(void)
    {
    struct {
	   unsigned char ssize;
	   unsigned char secondary;
	   unsigned char executer;
	   unsigned int status;
	   char ununsed[8];
	   unsigned char media;
	   unsigned long address;
	   unsigned int count;
	   unsigned int sector;
	   unsigned long  vol_id;
    } request;

    struct {
	   unsigned char mode;
	   unsigned long status;
    } cd_a_data;

    request.ssize = sizeof(request);
    request.secondary = 0;
    request.executer = GET_INFORMATION;
    request.media = request.sector = request.vol_id = 0;
    request.address = (unsigned long)&cd_a_data;
    request.count = 5;

    cd_a_data.mode = 0x06;
    cdrom_request(&request);
    cd_data.status = cd_a_data.status;
    cd_data.error = request.status;
    return;
}

/****************************************************************************/

// Get information about current track and CD error.
void cd_get_audio_info(void)
    {
    struct {
	   unsigned char ssize;
	   unsigned char secondary;
	   unsigned char executer;
	   unsigned int status;
	   char ununsed[8];
	   unsigned char media;
	   long address;
	   int count;
	   int sector;
	   long vol_id;
    } ioctli;

    struct {
	   unsigned char mode;
	   unsigned char min;           // Track number.
	   unsigned char max;           // Track number.
	   unsigned long address;
    } track_data;

    ioctli.ssize = sizeof(ioctli);
    ioctli.secondary = 0;
    ioctli.executer = GET_INFORMATION;
    ioctli.media = 0;
    ioctli.sector = 0;
    ioctli.vol_id = 0;
    ioctli.address = (long)&track_data;
    ioctli.count = sizeof(track_data);
    track_data.mode = 0x0A;
    cdrom_request(&ioctli);
    copyn_mem( &cd_data.diskid , &track_data.min , 6 );
    cd_data.min_track = track_data.min;
    cd_data.max_track = track_data.max;
    val_to_time( track_data.address,       &cd_data.disk_length_min,
		 &cd_data.disk_length_sec, &cd_data.disk_length_frames );
    cd_data.end_of_disk = val_to_frames(track_data.address);
    cd_data.error = ioctli.status;
    return;
}

/****************************************************************************/

// Locks and unlocks CD-ROM drive according to "action" parameter.
void cd_lock( unsigned char action )
    {
    struct {
	   unsigned char ssize;
	   unsigned char secondary;
	   unsigned char executer;
	   unsigned int status;
	   char ununsed[8];
	   unsigned char media;
	   unsigned long address;
	   unsigned int count;
	   unsigned char unused[4];
    } request;

    struct {
	   unsigned char mode;
	   unsigned char media;
    } cd_a_data;

    request.ssize = sizeof(request);
    request.secondary = 0;
    request.executer = SET_INFORMATION;
    request.media = 0;
    request.address = (unsigned long)&cd_a_data;
    request.count = 2;
    cd_a_data.mode = 1;
    cd_a_data.media = action;
    cdrom_request(&request);
    cd_data.error = request.status;
    return;
}

/****************************************************************************/

// Opens and closes CD-ROM door according to "action" parameter.
void move_tray( unsigned char action )
    {
    struct {
	   unsigned char ssize;
	   unsigned char secondary;
	   unsigned char executer;
	   unsigned int status;
	   char ununsed[8];
	   unsigned char media;
	   unsigned long address;
	   unsigned int count;
	   unsigned char unused[4];
    } request;

    unsigned char tray_mode = action;

    request.ssize = sizeof(request);
    request.secondary = 0;
    request.executer = SET_INFORMATION;
    request.media = 0;
    request.address = (unsigned long)&tray_mode;
    request.count = 1;
    cdrom_request(&request);
    cd_data.error = request.status;
    return;
}

/****************************************************************************/

// Check if CD-ROM is currently busy, and returns True if does.
boolean is_cd_busy(void)
     {
     cd_status();
     if (cd_data.status&1){
	move_tray(CLOSE);
        make_rdelay_message();
     }
     return (( cd_data.error & BUSY ));   /* if cd rom is busy. */
}

/****************************************************************************/

// Set CD-ROM head to parameter location.
void cd_seek( unsigned long location )
    {
    struct {
	   unsigned char ssize;
	   unsigned char secondary;
	   unsigned char executer;
	   unsigned int status;
	   char ununsed[8];
	   unsigned char address;
	   unsigned long transfer;
	   unsigned int sectors;
	   unsigned long seekpos;
    } request;

    request.ssize = sizeof(request);
    request.secondary = 0;
    request.executer = SEEK;
    request.address = request.transfer = request.sectors = 0;
    request.seekpos = location;
    cdrom_request (&request);
    cd_data.error = request.status;
    return;
}

/****************************************************************************/

// Return information of current position ( like disk playing time e.t.c. ).
void cd_tell_pos( musicinfo_t *info )
    {
    struct {
	   unsigned char ssize;
	   unsigned char secondary;
	   unsigned char executer;
	   unsigned int  status;
	   char          ununsed[8];
	   unsigned char media;
	   unsigned long address;
	   unsigned int  count;
	   unsigned int  sector;
	   unsigned long vol_id;
    } request;

    request.ssize = sizeof(request);
    request.secondary = 0;
    request.executer = GET_INFORMATION;
    request.media = request.sector = request.vol_id = 0;
    request.address = (unsigned long)info;
    request.count = 6;
    info->control = 12;
    cdrom_request(&request);
    cd_data.error = request.status;

    // Turn from hexa to decimal.
    info->track -= ( info->track / 16 ) * ( 16 - 10 );
    return;
}

/****************************************************************************/

// Set track to be current track on CD disk.
void cd_set_track( int tracknum )
    {
    struct {
	   unsigned char ssize;
	   unsigned char secondary;
	   unsigned char executer;
	   unsigned int status;
	   char ununsed[8];
	   unsigned char media;
	   unsigned long address;
	   unsigned int count;
	   unsigned int sector;
	   unsigned long  vol_id;
    } request;

    struct {
	   unsigned char mode;
	   unsigned char track;
	   unsigned long address;
	   unsigned char control;
    } track_data;

    request.ssize = sizeof(request);
    request.secondary = 0;
    request.executer = GET_INFORMATION;
    request.media = request.sector = request.vol_id = 0;
    request.address = (unsigned long)&track_data;
    request.count = 7;
    track_data.mode = 0x0B;
    track_data.track = tracknum;
    cdrom_request(&request);
    cd_data.error = request.status;
    cd_data.track_position = val_to_frames(track_data.address);
    cd_data.current_track = tracknum;
    cd_data.track_type = track_data.control & TRACK_MASK;
    return;
}

/****************************************************************************/

// Count the length of single track in bytes and returns it.
unsigned long get_track_byte_length( int tracknum )
    {
    unsigned long start, finish;
    unsigned int old_track = cd_data.current_track;

    cd_set_track(tracknum);
    start = cd_data.track_position;
    if ( tracknum < cd_data.max_track ){
       cd_set_track( tracknum + 1 );
       finish = cd_data.track_position;
    }
    else
	finish = cd_data.end_of_disk;

    cd_set_track(old_track);
    return finish - start;
}

/****************************************************************************/

// Count the time length of single track.
void get_track_time_length( int tracknum, unsigned char *min,
			    unsigned char *sec, unsigned char *frame )
    {
    unsigned long value = get_track_byte_length(tracknum);

    *frame = value % 75;
    value -= *frame;
    value /= 75;
    *sec = value % 60;
    value -= *sec;
    value /= 60;
    *min = value;
    return;
}

/****************************************************************************/

// Gets the current 4 channels of sound into "vol" structure.
void cd_get_volume( volumeinfo_t *vol )
    {
    struct {
	   unsigned char ssize;
	   unsigned char secondary;
	   unsigned char executer;
	   unsigned int status;
	   char           unused[8];
	   unsigned char media;
	   unsigned long address;
	   unsigned int count;
	   unsigned int sector;
	   unsigned long  vol_id;
    } request;

    vol->mode = 0x04;
    request.ssize = sizeof(request);
    request.secondary = 0;
    request.executer = GET_INFORMATION;
    request.media = request.sector = request.vol_id = 0;
    request.address = (unsigned long)vol;
    request.count = 9;
    cdrom_request(&request);
    cd_data.error = request.status;
    return;
}

/****************************************************************************/

// calculates the number of active sound channels ( out of 4 supported ).
void aic_calc( volume_handler *volhdr )
    {
    max_volume(&(volhdr->vol));
    cd_get_volume(&(volhdr->vol));

    volhdr->aic = ( volhdr->vol.volume0 / 255 ) +
                  ( volhdr->vol.volume1 / 255 ) +
                  ( volhdr->vol.volume2 / 255 ) +
                  ( volhdr->vol.volume3 / 255 ) ;

    return;
}

/****************************************************************************/

// Sets the current 4 channels of sound from "vol" structure.
void cd_set_volume( volumeinfo_t *vol )
    {
    struct {
	   unsigned char ssize;
	   unsigned char secondary;
	   unsigned char executer;
	   unsigned int status;
	   char ununsed[8];
	   unsigned char media;
	   unsigned long address;
	   unsigned int count;
	   unsigned char unused[4];
    } cd_request;

    vol->mode = 0x03;
    cd_request.ssize = sizeof(cd_request);
    cd_request.secondary = 0;
    cd_request.executer = SET_INFORMATION;
    cd_request.media =   0;
    cd_request.address = (unsigned long)vol;
    cd_request.count = 9;
    cdrom_request(&cd_request);
    cd_data.error = cd_request.status;
    return;
}

/****************************************************************************/

// Decrease the volume by 8.
void dec_volume( volumeinfo_t *volume )
    {
    // Get the current volume.
    // cd_get_volume(volume);

    if ( volume->volume0 > 8 )
       volume->volume0 -= 8;
    else
       volume->volume0  = 0;

    if ( volume->volume1 > 8 )
       volume->volume1 -= 8;
    else
       volume->volume1  = 0;

    if ( volume->volume2 > 8 )
       volume->volume2 -= 8;
    else
       volume->volume2  = 0;

    if ( volume->volume3 > 8 )
       volume->volume3 -= 8;
    else
       volume->volume3  = 0;

    // Set the current volume.
    cd_set_volume(volume);
    return;
}

/****************************************************************************/

// Increase the volume by 8.
void inc_volume( volumeinfo_t *volume )
    {
    // Get the current volume.
    //cd_get_volume(volume);

    if ( volume->volume0 < 255 - 8 )
       volume->volume0 += 8;
    else
       volume->volume0 = 255;

    if ( volume->volume1 < 255 - 8 )
       volume->volume1 += 8;
    else
       volume->volume1 = 255;

    if ( volume->volume2 < 255 - 8 )
       volume->volume2 += 8;
    else
       volume->volume2 = 255;

    if ( volume->volume3 < 255 - 8 )
       volume->volume3 += 8;
    else
       volume->volume3 = 255;

    // Set the current volume.
    cd_set_volume(volume);
    return;
}

/****************************************************************************/

// Set the current 4 channels of sound to maximum to ( 255 ).
void max_volume( volumeinfo_t *volume )
    {
    // Get the current volume.
    //cd_get_volume(volume);

    volume->volume0 = 255;
    volume->volume1 = 255;
    volume->volume2 = 255;
    volume->volume3 = 255;

    // Set the current volume.
    cd_set_volume(volume);
    return;
}

/****************************************************************************/

// Mute the sound.
void mute_volume( volumeinfo_t *volume )
    {
    // Get the current volume.
    //cd_get_volume(volume);

    volume->volume0 = 0;
    volume->volume1 = 0;
    volume->volume2 = 0;
    volume->volume3 = 0;

    // Set the current volume.
    cd_set_volume(volume);
    return;
}

/****************************************************************************/

// Play disk between start and finish position.
void cd_play_music( unsigned long start, unsigned long finish )
    {
    struct {
	   unsigned char ssize;
	   unsigned char secondary;
	   unsigned char executer;
	   unsigned int status;
	   char ununsed[8];
	   unsigned char address;
	   unsigned long start;
	   unsigned long music_length;
    } request;

    request.ssize = sizeof(request);
    request.secondary = 0;
    request.executer = PLAY;
    request.address = 0;
    request.start = start;
    request.music_length = finish - start;
    cdrom_request(&request);
    cd_data.error = request.status;
    return;
}

/****************************************************************************/

// Stop playing.
void cd_stop_music(void)
    {
    struct {
	   unsigned char ssize;
	   unsigned char secondary;
	   unsigned char executer;
	   unsigned int status;
	   char ununsed[8];
    } request;

    request.ssize = sizeof(request);
    request.secondary = 0;
    request.executer = STOP;
    cdrom_request(&request);
    cd_data.error = request.status;
    return;
}

/****************************************************************************/

// Start playing after pause.
void cd_resume_music(void)
    {
    struct {
	   unsigned char ssize;
	   unsigned char secondary;
	   unsigned char executer;
	   unsigned int status;
	   char ununsed[8];
    } request;

    request.ssize = sizeof(request);
    request.secondary = 0;
    request.executer = RESUME;
    cdrom_request(&request);
    cd_data.error = request.status;
    return;
}

/****************************************************************************/

// Returns the position of end of track - specified by tracknum argument.
// position is absolute ( offset from beginning of the disk ).
unsigned long end_of_track( int tracknum )
    {
    unsigned long end_pos;

    // Calculate the end of track.
    if (( tracknum + 1 ) > cd_data.max_track )
       end_pos = cd_data.end_of_disk;
    else {
       cd_set_track( tracknum + 1 );
       end_pos = cd_data.track_position;
       cd_set_track(tracknum);
    }
    return end_pos;
}

/****************************************************************************/

// Plays the selected_song. Returns True if managed to play the specified
// "selected song". Otherwise returns False.
boolean play_song( selected_song )
    {
    unsigned long start_pos, end_pos;

    if (( selected_song < cd_data.min_track)||
        ( selected_song > cd_data.max_track)){
       run_time_errors(2);   // Specified track does not exists.
       return FALSE;
    }

    // Select the song.
    cd_set_track(selected_song);

    // Check the type of track.
    if (cd_data.track_type == DATA_TRACK){
       run_time_errors(1);      // Cannot play data track.
       return FALSE;
    }

    // Set playing boundries.
    start_pos = cd_data.track_position + 8;

    // For playing up to the end insert:
    // if (play_mode == PLAY_REGULAR)
    //    end_pos = cd_data.end_of_disk;
    end_pos    = end_of_track(cd_data.current_track);

    // Unused "if", but may prevent a lot of bugs in future development.
    if ( start_pos > end_pos ){
       run_time_errors(3);               //   Disk is still not accessible
       return FALSE;                     //        because of rotary delay.
    }

    // Prepare for playing.
    // cd_seek(cd_data.track_position);
    cd_play_music( start_pos, end_pos );

    // Change status to playing.
    write_string_xy( CT_L_X + 22, CT_L_Y + 3, RED_ON_CYAN, "Playing." );

    return TRUE;
}

/****************************************************************************/

// Performs hammer effect ( like hammer hit the track back while playing ).
void hammer_effect(void)
    {
    cd_stop_music();
    cd_resume_music();
    return;
}

/****************************************************************************/

// Performs word up effect ( track goes back one word ).
void wordup_effect( volumeinfo_t *volume )
    {
    int i;
    volumeinfo_t temp;
    temp.volume0 = volume->volume0;
    temp.volume1 = volume->volume1;
    temp.volume2 = volume->volume2;
    temp.volume3 = volume->volume3;
    for ( i = 0; i < 6; i++ ){
        cd_stop_music();
        cd_resume_music();
        delay(120);
        volume->volume0 = 255 *  (i%2);
        volume->volume1 = 255 * !(i%2);
        cd_set_volume(volume);
    }
    volume->volume0 = temp.volume0;
    volume->volume1 = temp.volume1;
    volume->volume2 = temp.volume2;
    volume->volume3 = temp.volume3;
    cd_set_volume(volume);
    return;
}

/****************************************************************************/

// While song is playing it jumps "sec" seconds back. "m" argument is needed
// to determine CD head position.
void back_sec( int sec, musicinfo_t *m )
    {
    unsigned long start_pos, end_pos;
    int i;

    // Count current place and subtract number of seconds.
    start_pos = cd_head_position(m);
    if ( start_pos > sec * 75 )
       start_pos -= sec * 75;
    else
       start_pos = 8;      // Cannot jump to previous song.

    // Make the start_pos to be offset of current song.

    start_pos += cd_data.track_position;
    end_pos    = end_of_track(cd_data.current_track);

    // Stop the music and start playing from the new place.
    cd_stop_music();
    cd_play_music( start_pos, end_pos );

    return;
}

/****************************************************************************/

// While song is playing it jumps "sec" seconds forward. "m" argument is
// needed to determine CD head position.
void forward_sec( int sec, musicinfo_t *m )
    {
    unsigned long start_pos, end_pos;
    int i;

    // Count current place and add number of seconds.
    start_pos = cd_head_position(m);
    start_pos += sec * 75;

    // Make the start_pos to be absolute address, and calculate the end pos.
    end_pos    = end_of_track(cd_data.current_track);
    start_pos += cd_data.track_position;

    // Stop the music and start playing from the new place.
    cd_stop_music();
    if ( start_pos > end_pos )
       start_pos = end_pos;    // Cannot jump to the next song.
    cd_play_music( start_pos, end_pos );
    return;
}

/****************************************************************************/

// Jumps to starting segment to play inside the song in scope of
// 0..PROGRESS_LENGTH.
void jump_inside_song( unsigned char segment )
    {
    unsigned long start_pos, end_pos;
    int i;

    cd_stop_music();
    // Count the needed address.
    end_pos    = end_of_track(cd_data.current_track);
    start_pos  = cd_data.track_position + 8;
    start_pos += ( end_pos - start_pos ) * segment / PROGRESS_LENGTH;

    // Stop the music and start playing from the new place.
    cd_play_music( start_pos, end_pos );
    delay(P_TIME_DELAY);
    return;
}

/****************************************************************************/
			 /* Disks database modules.*/

// Opens index of disks database and reports errors. If function fails to
// open INDEX_FILE it will return NULL.
FILE* open_index_file(void)
    {
    FILE *index = fopen( INDEX_FILE, "rb");

    // Check if found.
    if ( index == NULL ){
       run_time_errors(6);          // Disks' index file not found.

       // Create file if empty.
       if ( ( index = fopen( INDEX_FILE, "wb+")) == NULL )
          {
          run_time_errors(7);       // Could not create disks' index file.
          return NULL;              // Could not proceed registration.
       }
    }
    return index;
}

/****************************************************************************/

// Returns the first index node of "fi" file. If node could not be read
// function will return node.pointer = -1. If end of file, function will
// return               node.pointer = -2.
index_t index_first( FILE *fi )
    {
    index_t node;

    // Go to the beginning of the file.
    fseek( fi, 0L, SEEK_SET );
    clearerr(fi);

    if ( fread( &node, sizeof(node), 1, fi ) != 1 ){
       // If file is empty?
       if (feof(fi)){
          node.pointer    =   -2;
          node.disk_id[0] = '\0';
       }
       else {
          run_time_errors(8);       // Index file is corrupted.
          node.pointer    =   -1;
          node.disk_id[0] = '\0';
       }
    }
    return node;
}

/****************************************************************************/

// Returns the next index node of "fi" file. If node could not be read
// function will return negative node.pointer.
index_t index_next( FILE *fi )
    {
    index_t node;

    clearerr(fi);
    if ( fread( &node, sizeof(node), 1, fi ) != 1 ){
       if (!feof(fi))
          run_time_errors(8);       // Index file is corrupted.
       node.pointer    =   -1;
       node.disk_id[0] = '\0';
    }
    return node;
}

/****************************************************************************/

// Finds the node in "fl" file, and returns its pointer field. Searching
// scope is:       set <=  file position <= end.
static
int binary_search( FILE* fl, char *disk_id, long set, long end )
    {
    index_t node;
    long mid = ( set + end ) / 2;
    int p;

    // Stopping condition;
    if ( set > end ){
       return - 1;                // Not found.
    }
    // Seek halfway and read the node.
    fseek( fl, mid * sizeof(index_t), SEEK_SET );
    fread( &node, sizeof(index_t), 1, fl );

    p = ( cmp_strings( node.disk_id, disk_id ));
    if (!p)
       return node.pointer;
    else
    if ( p > 0 )
       return binary_search( fl, disk_id, set    , mid - 1 );
    else
       return binary_search( fl, disk_id, mid + 1, end     );
}

/****************************************************************************/

// Searches node in index file with key == search_id. If not found negative
// number will be returned. If found function will return the serial place of
// the disk in data file.
int find_in_index( char *search_id )
    {
    FILE *index = open_index_file();
    index_t node;
    long set, end;

    // Check if can search.
    if ( index == NULL ){
       return -1;                    // Not found.
    }

    // Binary search.
    if (!file_size(index)){
       fclose(index);
       return -1;                    // Empty index - Not found.
    }
    set = 0;
    end = file_size(index) / sizeof(index_t) - 1;
    node.pointer = binary_search( index, search_id, set, end );

    fclose(index);
    return node.pointer;

/*
    // Serial Search

    // Read first node.
    node = index_first(index);
    if ( node.pointer < 0 ){
       fclose(index);
       return -1;                    // Not found.
    }


    // While not end of file and key is not found.
    while ( (node.pointer >= 0)&&( cmp_strings( node.disk_id, search_id )
            < 0 )){
          node = index_next(index);
    }

    fclose(index);

    // Check if found.
    if (cmp_strings( node.disk_id, search_id ))
       return -1;                    // Not found.
    else
       return node.pointer;
*/
}

/****************************************************************************/

// Opens file of disks database and reports errors. If function fails to
// open DATA_FILE it will return NULL.
FILE* open_disk_data_file( char *source_file )
    {
    FILE *data = fopen( source_file, "rb");

    // Check if found.
    if ( data == NULL ){
       run_time_errors(9);          // Disks' data file not found.

       // Create file if empty.
       if ( ( data = fopen( DATA_FILE, "wb+")) == NULL )
          {
          run_time_errors(10);      // Could not create disks' data file.
          return NULL;              // Could not proceed registration.
       }
    }
    return data;
}

/****************************************************************************/

// Retrieves the tag with serial number in the file == pointer. On success
// Returns True, otherwise False.
boolean retrieve_data_tag( disk_tag_t *tag, int pointer, char *source_file )
    {
    FILE* data = open_disk_data_file(source_file);

    if ( data == NULL ){
       return FALSE;
    }

    // Seek the right tag.
    if (fseek( data, pointer * sizeof(*tag), SEEK_SET )){
       if (feof(data)){
          // Index file and data file are not matching!!!
          // Someone manually modified the database.
          fclose(data);
          // External curruption in disks database cannot be repared.
          run_time_errors(14);
          // Database must be deleted.
          run_time_errors(15);
          unlink(INDEX_FILE);
          unlink(DATA_FILE);
          return FALSE;
       }
       else {
          run_time_errors(11);         // Data file is corrupted.
          fclose(data);
       }
       return FALSE;
    }

    // Read the right tag.
    if ( fread( tag, sizeof(*tag), 1, data ) != 1 ){
       if (feof(data)){
          // Index file and data file are not matching!!!
          // Someone manually modified the database.
          fclose(data);
          // External curruption in disks database cannot be repared.
          run_time_errors(14);
          // Database must be deleted.
          run_time_errors(15);
          unlink(INDEX_FILE);
          unlink(DATA_FILE);
       }
       else {
          run_time_errors(11);      // Data file is corrupted.
          fclose(data);
       }
       return FALSE;
    }
    fclose(data);
    return TRUE;
}

/****************************************************************************/

// Inits tag of disk with default values.
disk_tag_t init_disk_tag(void)
    {
    disk_tag_t tag;
    int i;

    // Fill tag inforation.
    tag.status = S_NORMAL;
    copy_string( tag.disk_id, cd_data.diskid );
    copy_string( tag.artist,  "Unknown Artist");
    set_bold_digits(tag.artist);
    copy_string( tag.genre,   "Unknown" );
    set_bold_digits(tag.genre);
    copy_string( tag.album,   "Unknown album" );
    set_bold_digits(tag.album);
    copy_string( tag.year,    "0000" );
    set_bold_digits(tag.year);
    copy_string( tag.unused, "");
    set_bold_digits(tag.unused );

    // Fill tracks information.
    for ( i = 0; i <= MAX_TRACKS; i++ ){
        char name[3];
        itoa( i + 1, name, 10 );
        if ( i < 10 - 1 ){
           tag.tracks[i].name[0] = '0';
           copy_string( tag.tracks[i].name + 1, name );

        }
        else {
          copy_string( tag.tracks[i].name, name );
        }
        set_bold_digits(tag.tracks[i].name);
        cd_set_track( i + 1 );

        // Check the type of track.
        if (cd_data.track_type == DATA_TRACK)
           tag.tracks[i].status = T_DATA;
        else
           tag.tracks[i].status = T_PLAYABLE;
        get_track_time_length( i + 1, &(tag.tracks[i].track_min),
			              &(tag.tracks[i].track_sec),
                                      &(tag.tracks[i].track_frame));

    }
    tag.songs_num = cd_data.max_track;
    if ( tag.songs_num > MAX_TRACKS )
      tag.songs_num = MAX_TRACKS;

    return tag;
}

/****************************************************************************/

// Loads disk's tag according to its cd_data.disk_id. In case of success tag
// will be returned in "tag" argument and function will return True. In case
// Of failure ( matching tag couldn't be found ) function will return False.
// Locating process of disk tag:
// 1. - Open index file.
// 2. - Using binary search find the index node which points on needed tag.
// 3. - If not found
//    3.1 - tag <- default values.
//    3.2 - Exit.
// 4. - Else ( if found )
//    4.1 - Open data file.
//    4.2 - Locate the needed tag by index node.pointer.
//    4.2 - If located
//           4.21 - tag <- located tag.
//           4.22 - Exit.
//    4.3 - Else ( if not located )
//           4.31 - Report external error ( wrong index ).
//           4.32 - Delete all database files.
//           4.33 - tag <- default values.
//           4.34 - Exit.
//
// When index does not match the data file,  Mashitup can rebuild the data
// file, reindex the index file => and the problem is solved.
// But this error happens only when ( external program / user manually )
// changes the data file. Mushitup prefers not to deal with external
// problems because it may lead to corrupted tag information and therefore
// it deletes the whole database.
boolean load_disk_tag( char *disk_id, disk_tag_t *tag, char *source_file )
    {
    int pointer = find_in_index( disk_id );

    // If not found fill the tag with default values and return False.
    if ( pointer < 0 ){
       *tag = init_disk_tag();
       return FALSE;
    }

    // Retrieve the tag.
    if ( !retrieve_data_tag( tag, pointer, source_file )){
       *tag = init_disk_tag();
       return FALSE;
    }

    return TRUE;
}

/****************************************************************************/

// Returns the status from disk tag "tag".
int extract_status( disk_tag_t *tag )
    {
    return tag->status;
}

/****************************************************************************/

// Returns the artists name from disk tag "tag".
char* extract_artist( disk_tag_t *tag )
    {
    return tag->artist;
}
/****************************************************************************/

// Returns the disk genre from disk tag "tag".
char* extract_genre( disk_tag_t *tag )
    {
    return tag->genre;
}
/****************************************************************************/

// Returns the album name from disk tag "tag".
char* extract_album( disk_tag_t *tag )
    {
    return tag->album;
}
/****************************************************************************/

// Returns the album year from disk tag "tag".
char* extract_year( disk_tag_t *tag )
    {
    return tag->year;
}
/****************************************************************************/

// Returns the name of "track_num" track from disk tag "tag".
// Attention: Tag has track numbers from 0..31 while CD-Disk from 1..32.
//            Therefore -1 calculation must be done in the current function.
char* extract_track_name( disk_tag_t *tag, int track_num )
    {
    return tag->tracks[ track_num - 1 ].name;
}

/****************************************************************************/

// Returns the size of opened fl file in bytes.
long file_size( FILE* fl )
     {
     long size;
     long cur_pos = ftell(fl);

     // Count the size.
     fseek( fl, 0, SEEK_END);
     size = ftell(fl);
     fseek( fl, cur_pos, SEEK_SET);

     return size;
}

/****************************************************************************/

// Saves tag to data file.
boolean save_disk_tag( disk_tag_t *tag, char *disk_id, char *dest_file )
    {
//    FILE *index;
    FILE *data;

    index_t node;

    // Open data file.
    data = fopen( dest_file, "rb+");
    if ( data == NULL ){
       run_time_errors(9);                // Data file not found.
       data = fopen( dest_file, "wb+");
       if ( data == NULL ){
          run_time_errors(12);            // Could not save disk information.
          return FALSE;
       }
    }

    // Fill index node.
    copy_string( node.disk_id, disk_id );
    node.pointer = find_in_index( disk_id );
    // Open index file.
//    index = fopen( INDEX_FILE, "ab+");
  //  if ( index == NULL ){
    //   run_time_errors(12);      // Could not save disk information.
     //  fclose(data);
     //  return FALSE;
    //}

    if ( node.pointer < 0 ){
       node.pointer = file_size(data) / sizeof(*tag);
    }

    // Get location for the new tag.
    fseek( data, (long)sizeof(*tag) * node.pointer, SEEK_SET);

    // Write tag.
    if ( fwrite( tag, sizeof(*tag), 1, data ) != 1 ){
       run_time_errors(12);      // Could not save disk information.
//       fclose(index);
       fclose(data);
       return FALSE;
    }
 //   fclose(index);
    fclose(data);

    return TRUE;
}

/****************************************************************************/

// Registers the new disk, buckups it and updates the indexes
boolean register_disk( disk_tag_t *tag, char *disk_id, char *dest_file )
    {
    boolean saved = save_disk_tag( tag, disk_id, dest_file );

    if (saved){
       // Backup the file.
       back_up_data( dest_file, BU_DAT_FILE );

       // Reindex the index and backup it.
       reindex( dest_file, INDEX_FILE );
       reindex( BU_DAT_FILE, BU_NDX_FILE );
    }
    return saved;
}

/****************************************************************************/

// Returns true if file with "file_path" path is empty. Otherwise false.
// It is used on closed files, without reading from it ( unlike feof() ).
// Preconditions: "file_path" file exists and may be accessed.
boolean is_file_empty( char *file_path )
    {
    boolean empty;

    FILE* fl = fopen( file_path, "rb" );
    empty = !file_size(fl);
    fclose(fl);

    return empty;
}

/****************************************************************************/

// Splits the file with "file_path" path to 2 files "fnew" and "fbreak",
// ( fnew_path and fbreak_path accordingly ).
// If file that was split is fully sorted ( fbreak is empty ) then function
// returns True. If file in unsorted, function returns False.
// Preconditions: "file_path" file exists and may be accessed.
static
boolean file_split( char *file_path, char* fnew_path, char* fbreak_path )
    {
    index_t node;
    char last_key[6];
    long count;

    // Open files.
    FILE* fl      = fopen( file_path,   "rb"  );
    FILE* fnew    = fopen( fnew_path,   "wb+" );
    FILE* fbreak  = fopen( fbreak_path, "wb+" );

    count = file_size(fl) / sizeof(index_t);

    // Deal with first node.
    fread ( &node, sizeof(node), 1, fl     );
    fwrite( &node, sizeof(node), 1, fnew   );
    copy_string( last_key, node.disk_id );
    count--;
    fread ( &node, sizeof(node), 1, fl     );

    // Split the file.
    while (!feof(fl)){
          if ( cmp_strings( last_key, node.disk_id ) <= 0 ){
             // If seria progresses.
             fwrite( &node, sizeof(node), 1, fnew   );
             copy_string( last_key, node.disk_id );
             count--;
          }
          else {
             // Start new seria.
             fwrite( &node, sizeof(node), 1, fbreak   );
             copy_string( last_key, "\0\0\0\0\0\0x0" );
          }
          fread( &node, sizeof(node), 1, fl );
    }

    // Close all files.
    fclose(fl);
    fclose(fnew);
    fclose(fbreak);
    return (boolean)( count == 0 );     // File is sorted.
}

/****************************************************************************/

// Merges two files "fnew" and "fbreak" ( f1_path and f2_path accordingly ),
// to the file with "file_path" path.
// Preconditions: "file_path" file exists and may be accessed.
static
void file_merge( char *file_path, char* f1_path, char* f2_path )
    {
    index_t node1, node2;
    boolean choose_1;

    // Open files.
    FILE* fl      = fopen( file_path, "wb+" );
    FILE* fnew    = fopen( f1_path,   "rb"  );
    FILE* fbreak  = fopen( f2_path,   "rb"  );

    // Deal with first nodes.
    fread( &node1, sizeof(index_t), 1, fnew   );
    fread( &node2, sizeof(index_t), 1, fbreak );

    // Merge to the file.
    while ( !feof(fnew) && !feof(fbreak) ){
          // Write.
          if ( cmp_strings( node2.disk_id, node1.disk_id ) >= 0 ){
             fwrite( &node1, sizeof(index_t), 1, fl );
             choose_1 = TRUE;
          }
          else {
             fwrite( &node2, sizeof(index_t), 1, fl );
             choose_1 = FALSE;
          }
          // Read.
          if (choose_1){
             fread( &node1, sizeof(index_t), 1, fnew   );
          }
          else {
             fread( &node2, sizeof(index_t), 1, fbreak   );
          }
    }

    // Pick up reminders.
    while (!feof(fnew)){
          fwrite( &node1, sizeof(index_t), 1, fl     );
          fread ( &node1, sizeof(index_t), 1, fnew   );
    }
    while (!feof(fbreak)){
          fwrite( &node2, sizeof(index_t), 1, fl     );
          fread ( &node2, sizeof(index_t), 1, fbreak );
    }

    // Close all files.
    fclose(fl);
    fclose(fnew);
    fclose(fbreak);
    return;
}

/****************************************************************************/

// Algorithm of serial merge sort:
// A loop finds each time series where a[n] < a[n+1]. Each seria is copied
// into fnew file and the breakersof seria are copied to fbreak file.
// Then both files ( fnew and fbreak ) are merged back to main file when
// each time node is being read from fnew or fbreak file and the smaller one
// goes to main file.
// Example:
//  Main file -  A B C      P      K L M      I        E F       .
//              ÀSeriaÙ ÀBreakerÙ ÀSeriaÙ ÀBreakerÙ À Seria Ù

// Sorts the file with "file_path"
// Preconditions: "file_path" file exists and may be accessed.
// Function sorts only index files.
void file_merge_sort( char *file_path )
    {
    boolean sorted;

    if ( is_file_empty(file_path) ){
       return;
    }

    sorted = file_split( file_path, NEW_FILE, BREAK_FILE );
    while (!sorted){
                   file_merge( file_path, NEW_FILE, BREAK_FILE );
          sorted = file_split( file_path, NEW_FILE, BREAK_FILE );
    }

    // Delete assistant files.
    unlink(NEW_FILE);
    unlink(BREAK_FILE);
    return;

}

/****************************************************************************/

// Returns True if index file with "file_path" path is sorted.
boolean is_file_sorted( char *file_path )
    {
    index_t node;
    char key[6];

    // Open file.
    FILE* fl = fopen( file_path, "rb" );
    if (!file_size(fl)){
       fclose(fl);
       return TRUE;                  // File is empty - Sorted.
    }

    fread( &node, sizeof(index_t), 1, fl );
    copy_string( key, node.disk_id );

    clearerr(fl);
    // Check the whole file.
    while ( !feof(fl) && ( cmp_strings( key, node.disk_id ) <= 0 )){
          copy_string( key, node.disk_id );
          fread( &node, sizeof(index_t), 1, fl );
    }

    fclose(fl);
    if ( cmp_strings( key, node.disk_id ) <= 0 )
       return TRUE;
    return FALSE;
}

/****************************************************************************/

// Creates unsorted index on data_path file into index_path file.
void make_index( char *data_path, char *index_path )
    {
    disk_tag_t tag;
    index_t    node;
    int i;

    // Open all files.
    FILE* data  = fopen( data_path,  "rb"  );
    FILE* index = fopen( index_path, "wb+" );

    if (!file_size(data)){
       fclose(data);
       fclose(index);
       return;
    }

    clearerr(data);
    i = 0;
    // Make index.
    fread( &tag, sizeof(disk_tag_t), 1, data );
    while (!feof(data)){
          if ( tag.status == S_NORMAL ){
             copy_string( node.disk_id, tag.disk_id );
             node.pointer = i;
             fwrite( &node, sizeof(index_t), 1, index );
             i++;
          }
          fread( &tag, sizeof(disk_tag_t), 1, data );
    }

    // Close all files and rename org file to data file.
    fclose(data);
    fclose(index);
    return;
}

/****************************************************************************/

// Creates sorted index on data_path file into index_path file.
void reindex( char *data_path, char *index_path )
    {
    make_index( data_path, index_path );
    if (is_file_sorted(index_path))
       return;
    file_merge_sort(index_path);
    return;
}

/****************************************************************************/

// Reorganizes the disk data file. It is used to clean deleted disks.
void reorganize( char *data_path )
    {
    disk_tag_t tag;

    // Open all files.
    FILE* data = fopen( data_path,  "rb" );
    FILE* forg = fopen( REORG_FILE, "wb+" );

    if (!file_size(data)){
       fclose(data);
       fclose(forg);
       unlink(REORG_FILE);
       return;
    }
    clearerr(data);

    fread( &tag, sizeof(disk_tag_t), 1, data );
    // Reorganize.
    while (!feof(data)){
          if ( tag.status == S_NORMAL ){
             fwrite( &tag, sizeof(disk_tag_t), 1, forg );
          }
          fread( &tag, sizeof(disk_tag_t), 1, data );
    }

    // Close all files and rename org file to data file.
    fclose(data);
    fclose(forg);
    unlink(data_path);
    rename( REORG_FILE, data_path );
    return;
}

/****************************************************************************/

// Backups the sf file to df file. On succes returns 0;
int back_up_data( char *sf, char *df )
    {
    disk_tag_t tag;
    FILE* source;
    FILE* dest;

    // Open both files.
    source = fopen( sf, "rb");
    if ( source == NULL ){
       return -1;
    }
    dest = fopen( df, "wb+");
    if ( dest == NULL ){
       fclose(source);
       return - 1;
    }

    // Copy the source into dest.
    clearerr(source);
    fread ( &tag, sizeof(tag), 1, source );
    while (!feof(source)){
          fwrite( &tag, sizeof(tag), 1, dest   );
          fread ( &tag, sizeof(tag), 1, source );
    }
    fclose(source);
    fclose(dest);
    return 0;
}

/****************************************************************************/

// Gets track, specified by tracknum and changes in the tag variable its
// status. If it was blocked, becomes playable and from playable to blocked.
// Data tracks cannot be blocked or unblocked.
void block_track( int tracknum, disk_tag_t *tag )
    {
    if ( tag->tracks[ tracknum - 1 ].status == T_DATA )
       return;     // Cannot unblock data track.
    else
    if ( tag->tracks[ tracknum - 1 ].status == T_BLOCKED )
       tag->tracks  [ tracknum - 1 ].status  = T_PLAYABLE;
    else
       tag->tracks  [ tracknum - 1 ].status  = T_BLOCKED;
   return;
}

/****************************************************************************/

// Return True if track, specified by tracknum is blocked.
boolean is_track_blocked( int tracknum, disk_tag_t *tag )
    {
    // If blocked track.
    if ( tag->tracks[ tracknum - 1 ].status == T_BLOCKED )
       return TRUE;

    // Track may be played.
    return FALSE;
}

/****************************************************************************/

// Return True if track, specified by tracknum may be played. It is when track
// is not blocked by the user and it is not DATA track.
boolean is_track_playable( int tracknum, disk_tag_t *tag )
    {
    // If data track.
    if ( is_track_data( tracknum, tag ) )
       return FALSE;

    // If blocked track.
    if ( is_track_blocked( tracknum, tag ) )
       return FALSE;

    // Track may be played.
    return TRUE;
}

/****************************************************************************/

// Return True if track, specified by tracknum is data track.
boolean is_track_data( int tracknum, disk_tag_t *tag )
    {
    // If data track.
    if ( tag->tracks[ tracknum - 1 ].status == T_DATA )
       return TRUE;

    // Track may be played.
    return FALSE;
}

/****************************************************************************/

// Gets current track, specified by tracknum and returns the next track that
// Would be played according to paly_mode and tracks status'.
// If disk nothing has to be played next -1 will be returned.
int next_track( int tracknum, int play_mode, disk_tag_t *tag )
    {
    // If play mode is REPEAT.
    if ( play_mode & PLAY_REPEAT )
       return tracknum;

    // If play mode is RANDOM.
    if ( play_mode & PLAY_RANDOM ){
       tracknum = ( rand() % cd_data.max_track ) + 1;
       // Make sure that blocked tracks will not be played.
       while ( tag->tracks[ tracknum - 1 ].status != T_PLAYABLE ){
             tracknum = ( rand() % cd_data.max_track ) + 1;
       }
       return tracknum;
    }

    // If play mode is REGULAR.
    if ( play_mode == PLAY_REGULAR ){
       if ( tracknum == cd_data.max_track )
          return -1;
       tracknum++;
       // Make sure that blocked tracks will not be played.
       while ( tag->tracks[ tracknum - 1 ].status != T_PLAYABLE ){
             if ( tracknum == cd_data.max_track )
                return -1;
             tracknum++;
       }
    }
    return tracknum;
}

/****************************************************************************/
			 /* Help database modules.*/

// Finds the appropriate position of topic in the "fl" file.
boolean find_help_topic( FILE *fl, int topic )
    {
    char line[81] = {0};

    // Find the appropriate topic in the file.
    while ( topic > 0 ){

         // Check if line can be read.
         if ( fgets( line, 81, fl ) == NULL ){
            run_time_errors(28);                // Help file is corrupted.
            return FALSE;
         }
         else {
            if ( !strncmp( line, NEW_HELP_TOPIC, length_of(NEW_HELP_TOPIC))){
               topic--;
            }
         }
    }
    return TRUE;
}

/****************************************************************************/
/* EOF. */