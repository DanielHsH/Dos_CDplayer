/****************************************************************************/
/*******************************  Mashitup.C  *******************************/
/****************************************************************************/
// This file is Mashitup main which activates the whole project.
// Mashitup.C is included in the project.
// Mashitup.C size is XX.X KB.

/****************************************************************************/

#include <stdlib.h>     // Needed for random function.
#include "basic.h"

/****************************************************************************/

// For Future Developments:
//  -  If needed, may store all the panels in one screen array and return it
//     to Mashitup main.
//  -  New skins may be done for Mashitup by user, when working. Use ASCII
//     editor and font functions.
//  -  Diffrent CD-ROM drives using is_cdrom_installed();
//     Use secondary in request structres to select the needed disk.


/***************************** Global variables *****************************/

// cd_data stores information about CD-ROM disk. It is defined as global out
// of 2 reasons: 1 - Mashitup uses only one cd_data each time so that is
//               more understandable to use global variable.
//               2 - A lot of functions may use cd_data variable so making
//               it global decreases the number of passed arguments.
cdrom_data_t cd_data;

// M_layer defines the number of current upper layer. Layers are windows and
// boxes. Each group of windows has distinct layer number, therfore Mushitup
// can recognize the current active box.
int M_layer = 0;

// Selected song variable holds the number of currently selected track.
// It is designed as global in order to make an easy access to mouse clicks.
// Mouse can select new track without arguments passed.
// Form the same reasons playing variable is global and the others..
int     selected_song = 1;
boolean playing;                        // Is song is being played.
volume_handler volhdr = {{0},0};        // Mashitup volume.
play_time_t time_direction = passed;    // Show the remaining or the passed
                                        // time.
// Variable "Change play progress bar address. When progress bar is changed
// with the mouse this variable stores the starting segment to play in the
// selected song in scope of 0..PROGRESS_LENGTH.
unsigned char chg_pprog_add;

/****************************************************************************/
/******************************* Mashitop Main ******************************/
/****************************************************************************/

int main(void)
    {

    musicinfo_t music_info;
    unsigned long cur_status = 0;           // CD status.
    unsigned char play_mode = PLAY_REGULAR; // Not repeated and not random.
    unsigned char submit;
    boolean stop_working, paused;
    boolean cd_locked = FALSE;              // CD-ROM in unlocked.
    char    disk_id[6];
    disk_tag_t tag;

    int skin_num = 0;                       // Number of current skin.

    // Prepeare for running.
    preparations();
    randomize();

    if (!is_cdrom_installed())
       terminate(2); // No CD drive.

    playing = (is_cd_busy())?TRUE:FALSE;
    stop_working = paused = FALSE;

    aic_calc(&volhdr);
    cur_status = new_disk( play_mode, volhdr, selected_song, &tag );
    clear_console_buffer();

    // If Mashitup was activated when track is already playing.
    if ((paused == FALSE)&&(playing)){
       // Get the curent track number.
       cd_tell_pos(&music_info);

       delete_tracks_pointer(selected_song);
       cd_data.current_track = music_info.track;
       selected_song = cd_data.current_track;
       play_mode = PLAY_REGULAR;

       // Make index of currently selected song.
       draw_tracks_pointer(selected_song);
       cd_locked = lock_disk();

       // Cannot continue to play blocked track.
       if ( !is_track_playable( selected_song, &tag )){
          playing = stop_music();
          click_stop_button();
          // Unlock CD-ROM.
          cd_locked = unlock_disk();
          run_time_errors(19);
       }
       else {
          // Change status to playing.
          write_string_xy( CT_L_X + 22, CT_L_Y + 3, RED_ON_CYAN, "Playing." );
       }

    }

    while ( !stop_working ){
          cd_status();
          if (cur_status != cd_data.status){
	     cur_status = display_cd_status_panel();
	     if (cd_data.status & 1){
	        /* Preserve previous DISK ID */
	        copy_string( disk_id , cd_data.diskid );

                no_disk();

	        /* See if disk was changed */
	        if (cmp_strings( disk_id , cd_data.diskid)){
                   selected_song = cd_data.min_track;;
	           cur_status = new_disk( play_mode, volhdr,
                                          selected_song, &tag );
                }
	     }
          }
          if (kbhit()){

             submit = toupper(getch());
             switch (submit) {

                    case K_MOUSE_EXTENDED :
                    case K_EXTENDED :
                               submit = toupper(getch());
                               switch (submit) {

                                      // One song up.
                                      case K_UP :
         selected_song = k_up_finger(selected_song);
         // When song was changed start playing it again.
         if (playing)
            push_con_char(PLAY_BUTTON_KEY);
	 break;
                                      // One song down.
                                      case K_DOWN :
         selected_song = k_down_finger(selected_song);
         // When song was changed start playing it again.
         if (playing)
            push_con_char(PLAY_BUTTON_KEY);
	 break;

                                      // Select the first song.
                                      case K_HOME :
         selected_song = k_home_finger(selected_song);
         // When song was changed start playing it again.
         if (playing)
            push_con_char(PLAY_BUTTON_KEY);
	 break;

                                      // Select the last song.
                                      case K_END :
         selected_song = k_end_finger(selected_song);
         // When song was changed start playing it again.
         if (playing)
            push_con_char(PLAY_BUTTON_KEY);
	 break;

                                      // 5 songs up.
                                      case K_PGUP :
         selected_song = k_pgup_finger(selected_song);
         // When song was changed start playing it again.
         if (playing)
            push_con_char(PLAY_BUTTON_KEY);
		                                  break;

                                      // 5 songs down.
                                      case K_PGDOWN :
         selected_song = k_pgdown_finger(selected_song);
         // When song was changed start playing it again.
         if (playing)
            push_con_char(PLAY_BUTTON_KEY);
		                                  break;

                                      // Block/Unblock track.
                                      case MARK_TRACK_KEY:
         if ((playing)&&( music_info.track == selected_song )){
            // Cannot block the track while playing it.
            run_time_errors(17);
         }
         else {
            block_track( selected_song, &tag );
            write_single_track( selected_song, &tag );
         }
                                                  break;

                                      // Some seconds back.
                                      case BACK_SEC_KEY:
         if ((playing)&&(!paused)){
            click_backsec_button();
            back_sec( 5, &music_info );
         }
                                                  break;

                                      // Some seconds forward.
                                      case FORW_SEC_KEY:
         if ((playing)&&(!paused)){
            click_forwsec_button();
            forward_sec( 5, &music_info );
         }
                                                  break;

                                      // Mashitup help.
                                      case F1 :
                                                  bar_help_opt_click();
                                                  Mashitup_help();
                                                  break;

                                      // About window.
                                      case F2 :   bar_about_opt_click();
                                                  Mashitup_about();
                                                  break;

                                      // Disk registration wizard.
                                      case REGISTER_DISK_O:
                                      case REG_MOUSE_DSK_O:
         bar_rdisk_opt_click();
         edit_disk_tag( &tag, selected_song );
         (void)v_register_disk( &tag, cd_data.diskid, DATA_FILE );
         display_album_panel( selected_song, &tag, FALSE );
                                                  break;

                                      // Disk deleting wizard.
                                      case DELETE_EDITOR_O:
                                                  delete_disk_tag();
                                                  break;

                                      // Hotkeys help.
       	                              case ALT_H :
                                                  bar_hot_keys_opt_click();
                                                  hot_keys_page();
		                                  break;

                                      // Exit.
       	                              case ALT_X :
                                                  bar_exit_opt_click();
                                                  stop_working = TRUE;
		                                  break;

                               } // Switch
                               break;

                    // Make screen saver.
                    case K_SCREEN_SAV :
                               screen_saver(skin_num);
                               clear_console_buffer();
                               break;

                    // Play the song.
                    case PLAY_BUTTON_KEY :
                               if (paused)
                                  paused = pause_song(paused);
                               else
                               if (playing)
                                  cd_stop_music();

                               clear_play_progress_bar();
                               if (!is_track_playable( selected_song, &tag )){
                                  // Cannot play locked track.
                                  run_time_errors(18);
                                  break;
                               }
                               playing = play_song( selected_song );
                               // Lock CD-ROM.
                               cd_locked = lock_disk();
                               cd_data.current_track = selected_song;
                               music_info.track      = selected_song;
                               click_play_button();
                               delay(P_TIME_DELAY);
                               paused = FALSE;
                               break;

                    // Pause the song.
	            case PAUSE_BUTTON_KEY :
                               if (playing)
                                  paused = pause_song(paused);
 		               break;

                    // Stop the song.
	            case STOP_BUTTON_KEY :
                               if (playing){
                                  if (paused){
                                     release_pause_button();
                                     paused = FALSE;
                                  }
                                  playing = stop_music();
                                  click_stop_button();
                                  // Unlock CD-ROM.
                                  cd_locked = unlock_disk();
                               }
                               clear_play_progress_bar();
		               break;

                    // Eject CD-ROM disk.
	            case EJECT_BUTTON_KEY :
                               // Eject if not forbidden.
                               click_eject_button();
                               if ((!playing)&&(!cd_locked)){
		                  move_tray(EJECT);
                               }
                               else
                                  // Unable to open locked CD-ROM.
                                  run_time_errors(5);
		               break;

                    // Maximal volume.
	            case MAX_VOLUME_KEY :
                               max_volume(&volhdr.vol);
                               write_all_sound_trails( volhdr.vol,
                                                       volhdr.aic );
		               break;

                    // Minimal volume.
	            case MIN_VOLUME_KEY :
                               mute_volume(&volhdr.vol);
                               write_all_sound_trails( volhdr.vol,
                                                       volhdr.aic );
		               break;

                    // Increase volume.
	            case INC_VOLUME_KEY :
                               inc_volume(&volhdr.vol);
                               write_all_sound_trails( volhdr.vol,
                                                       volhdr.aic );
		               break;

                    // Decrease volume.
	            case DEC_VOLUME_KEY :
                               dec_volume(&volhdr.vol);
                               write_all_sound_trails( volhdr.vol,
                                                       volhdr.aic );
		               break;

                    // Refresh equalizer.
                    // Not useful, and not for user.
                    case K_SOUND_REF :
                               cd_set_volume(&volhdr.vol);
                               write_all_sound_trails( volhdr.vol,
                                                       volhdr.aic );
                               break;

                    // Refresh play progress bar.
                    // Not useful, and not for user.
                    case K_PPBAR_REF :
                               if (playing){
                                  jump_inside_song(chg_pprog_add);
                                  if (paused)
                                     paused = pause_song(!paused);
                               }
                               break;

                    // Play the songs randomally.
                    case PLAY_RANDOM_KEY :
                               if ( play_mode & PLAY_RANDOM )
                                  click_play_mode( PLAY_STOP + PLAY_RANDOM );
                               else
                                  click_play_mode(             PLAY_RANDOM );
                               play_mode ^= PLAY_RANDOM;
                               break;

                    // Repeat one song.
                    case PLAY_REPEAT_KEY :
                               if ( play_mode & PLAY_REPEAT )
                                  click_play_mode( PLAY_STOP + PLAY_REPEAT );
                               else
                                  click_play_mode(             PLAY_REPEAT );
                               play_mode ^= PLAY_REPEAT;
                               break;

                    // Change playing time direction.
                    case K_TIME_DIR :
                               if ( time_direction == passed ){
                                  time_direction = remaining;
                                  write_string_xy( CT_L_X + 9, CT_L_Y + 5,
                                                   BLACK_ON_CYAN, "resting" );
                                  write_string_xy( CT_L_X + 9, CT_L_Y + 7,
                                                   BLACK_ON_CYAN, "resting" );
                               }
                               else {
                                  time_direction = passed;
                                  write_string_xy( CT_L_X + 9, CT_L_Y + 5,
                                                   BLACK_ON_CYAN, "playing" );
                                  write_string_xy( CT_L_X + 9, CT_L_Y + 7,
                                                   BLACK_ON_CYAN, "playing" );
                               }
                               break;

                    // Lock/Unlock the CD drive.
	            case LOCK_CD_KEY :
                               if (!cd_locked){
                                  cd_locked = lock_disk();
                               }
                               else {
                                  if (!playing)
		                     cd_locked = unlock_disk();
                                  else
                                     // Unlocking while playing is forbidden.
                                     run_time_errors(4);
                               }
		               break;

                    // Hammer effect.
                    case SP_HAMMER_KEY:
                               if ((playing)&&(!paused)){
                                  click_sphammer_button();
                                  hammer_effect();
                               }
                               break;

                    // Word up effect.
                    case SP_WORDUP_KEY:
                               if ((playing)&&(!paused)){
                                  click_spwordup_button();
                                  wordup_effect(&volhdr.vol);
                               }
                               break;

                    // Set original skin.
                    case SET_ORIG_SKIN_O:
                               {
                               unsigned char pal[256][3];
                               save_pal(pal);

                               skin_num = 0;
                               screen_fade_out();
                               free_mouse();
                               load_font(FONT_FILE);
                               cursor_off();
                               new_mouse();
                               screen_fade_in(pal);
                               }
                               break;

                    // ----------------- Different skins ---------------.
                    case '1':
                               skin_num = 1;
                               change_skin(skin_num);
                               break;

                    case '2':
                               skin_num = 2;
                               change_skin(skin_num);
                               break;

                    case '3':
                               skin_num = 3;
                               change_skin(skin_num);
                               break;

                    case '4':
                               skin_num = 4;
                               change_skin(skin_num);
                               break;

                    case '5':
                               skin_num = 5;
                               change_skin(skin_num);
                               break;

                    case '6':
                               skin_num = 6;
                               change_skin(skin_num);
                               break;

                    case '7':
                               skin_num = 7;
                               change_skin(skin_num);
                               break;

                    case '8':
                               skin_num = 8;
                               change_skin(skin_num);
                               break;

                    case '9':
                               skin_num = 9;
                               change_skin(skin_num);
                               break;
                    // -------------------------------------------------.

             }
          }
         if (playing)
         {
	   if ((paused == FALSE)&&(!is_cd_busy())){
              // Song is over.
              // Check if have to repeat that song.
              if (( play_mode & PLAY_REPEAT )||
                  ( play_mode & PLAY_RANDOM )){
                 int i = next_track( selected_song, play_mode, &tag );

                 if ( selected_song != i ){
                    delete_tracks_pointer(selected_song);
                    selected_song = i;
                    draw_tracks_pointer(selected_song);
                 }
                 playing = play_song( selected_song );
                 delay(P_TIME_DELAY);
              }
              else
              if ( play_mode == PLAY_REGULAR ){
                 int i = next_track( selected_song, play_mode, &tag );

                 // If music will be continued.
                 if ( i != -1 ){
                    delete_tracks_pointer(selected_song);
                    selected_song = i;
                    draw_tracks_pointer(selected_song);
                    playing = play_song( selected_song );
                    delay(P_TIME_DELAY);
                 }
                 else {
                    // Stop the music.
                    if (paused)
                       paused = pause_song(paused);
                    playing = stop_music();
                    click_stop_button();
                    // Unlock CD-ROM.
                    cd_locked = unlock_disk();
                 }

              }
              else {
	         playing = FALSE;
	         cd_stop_music();
	         cd_locked = unlock_disk();
              }

	   }
	   else {
                print_play_time( &music_info, time_direction, &tag );
	   }
         }
    } // While.
    if ((playing)&&(!paused)){
       stop_audio_choice();
    }
    terminate(0);
    return 0;
}

/****************************************************************************/
/* EOF. */