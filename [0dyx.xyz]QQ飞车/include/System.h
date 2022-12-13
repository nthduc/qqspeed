/*
**  SYSTEM.H
**  system functions header.
**
**  ZJian(LiuZiJian), 2000.7.10.
**		Created.
**	Jim(LiuZiJian), 2003.6.24.
**		Modified for library "system".
**
*/
#ifndef _XSYSTEM_H_INCLUDE_
#define _XSYSTEM_H_INCLUDE_      1


//Jim, remark for MFC. [6/24/2003]
//#define TRACE(s)		{ sprintf((char *)print_rec,"TRACE:%s",#s); log_error(1,print_rec); }

#define char_upper(ch)  ( ((ch)>='a' && (ch)<='z') ? ('A'+(ch)-'a') : (ch) )
#define char_lower(ch)  ( ((ch)>='A' && (ch)<='Z') ? ('a'+(ch)-'A') : (ch) )


#define PACK_FILE   FILE
#define pack_fopen  fopen
#define pack_fclose fclose
#define pack_fseek  fseek
#define pack_fread  fread
#define pack_fwrite fwrite


//--- windows types ---------------------------------------
typedef enum    WINDOWS_TYPE_ENUMS
{ WINDOWS_NT        =   3,
WINDOWS_32          =   2,
WINDOWS_95          =   1,
} WINDOWS_TYPE;


//--- update_data_bits flags ------------------------------ 
typedef enum    UPDATE_DATA_BIT_FLAG_ENUMS
{   UDB_CHK_BIT     =   1,  
UDB_SET_BIT         =   2,  
UDB_GET_BIT         =   3,  
UDB_ADD_BIT         =   4,  
UDB_DEC_BIT         =   5,  
UDB_SWT_BIT         =   6   
} UPDATE_DATA_BIT_FLAG;



//==================================================================================
extern  USTR    print_rec[2048];
extern  USTR    game_path[_MAX_PATH];
extern  USTR    game_filename[_MAX_PATH+_MAX_FNAME];
extern  ULONG   game_now_time;
extern  ULONG   game_start_time;
//
extern  ULONG   timer_tick00;
extern  ULONG   timer_tick01;
extern  ULONG   timer_tick02;
extern  ULONG   timer_tick03;
extern  ULONG   timer_tick04;
extern  ULONG   timer_tick05;
extern  ULONG   timer_tick06;
extern  ULONG   timer_tick07;
extern  ULONG   timer_tick08;
extern  ULONG   timer_tick09;
extern  ULONG   cdrom_timer_tick;
extern  ULONG   cursor_timer_tick;
extern  ULONG   system_timer_tick;
//

//==================================================================================
EXPORT  int     FNBACK  init_system(void);
EXPORT  void    FNBACK  active_system(int active);
EXPORT  void    FNBACK  free_system(void);
//
EXPORT  void    FNBACK  log_error(int p, USTR *strMsg );
EXPORT  void    FNBACK  log_error(int p, char *strMsg, ...);
EXPORT  void    FNBACK  system_rand_init(void);
EXPORT  int     FNBACK  system_rand(void);
//
EXPORT  void    FNBACK  idle_loop(void);
EXPORT  SLONG   FNBACK  is_gb_windows(void);
EXPORT  SLONG   FNBACK  get_cdrom_drive(void);
EXPORT  SLONG   FNBACK  check_cdrom_volume(USTR *title);
EXPORT  USTR *  FNBACK  get_cdrom_volume(SLONG drive);
EXPORT  void    FNBACK  store_game_path(USTR *path);
EXPORT  void    FNBACK  capture_screen(void);
//
EXPORT  ULONG   FNBACK  get_fps(void);
EXPORT  USTR *  FNBACK  get_computer_name(void);
EXPORT  USTR *  FNBACK  get_user_name(void);
EXPORT  USTR *  FNBACK  get_windows_directory(void);
EXPORT  SLONG   FNBACK  get_windows_version(void);
EXPORT  void    FNBACK  get_memory_status(ULONG *total_phys,ULONG *avail_phys);
EXPORT  ULONG   FNBACK  get_disk_serial_no(void);
EXPORT  ULONG   FNBACK  get_cpu_clock(void);
EXPORT  char *  FNBACK  get_cpu_id(void);
EXPORT  void    FNBACK  analyst_system(void);
//
// Jim, found error... [7/23/2003]
//EXPORT	bool	FNBACK	is_cpu_mmx(void);
//
EXPORT  void    FNBACK  get_time(USTR *buffer);
//
//--- data compute functions --------------------------------------
EXPORT  void    FNBACK  run_random_init(void);
EXPORT  ULONG   FNBACK  checksum_ulong_table(ULONG *table, ULONG length);
EXPORT  void    FNBACK  switch_data_flag(ULONG *data,ULONG flag);
EXPORT  SLONG   FNBACK  update_data_bit(ULONG *data, ULONG mask, SLONG flag);
EXPORT  UCHR    FNBACK  compute_uchar_chksum(USTR *buffer, SLONG size);
EXPORT  void    FNBACK  pack_uchar(USTR *buffer, UCHR data, SLONG *index);
EXPORT  void    FNBACK  pack_uhint(USTR *buffer, UHINT data, SLONG *index);
EXPORT  void    FNBACK  pack_ulong(USTR *buffer, ULONG data, SLONG *index);
EXPORT  UCHR    FNBACK  unpack_uchar(USTR *buffer, SLONG *index);
EXPORT  UHINT   FNBACK  unpack_uhint(USTR *buffer, SLONG *index);
EXPORT  ULONG   FNBACK  unpack_ulong(USTR *buffer, SLONG *index);

//--- file & directory functions ----------------------------------
EXPORT  void    FNBACK  ChangeFileExt(CHAR *szOrigPath, CHAR *szDestPath, CHAR *szExt);
#define change_file_extension(a,b,c) ChangeFileExt((CHAR*)a,(CHAR*)b,(CHAR*)c)
EXPORT  SLONG   FNBACK  is_file_exist(USTR *filename);
EXPORT  SLONG   FNBACK  is_directory_exist(USTR *path);
EXPORT  SLONG   FNBACK  make_directory(USTR *path);
EXPORT  SLONG   FNBACK  remove_directory(USTR *path);
EXPORT  USTR *  FNBACK  get_pure_filename(USTR *filename);
EXPORT  USTR *  FNBACK  get_file_extension(USTR *filename);
EXPORT  USTR *  FNBACK  get_file_path(USTR *path_fname);
EXPORT  USTR *  FNBACK  get_nopath_filename(USTR *path_fname);
EXPORT  USTR *  FNBACK  make_full_filename(USTR *path,USTR *filename,USTR *fileext);
EXPORT  USTR *  FNBACK  adjust_file_path(USTR *path);
EXPORT  void    FNBACK  oper_whole_path(USTR *spath,USTR *tpath);

//--- string functions --------------------------------------------
EXPORT  char**  FNBACK  divide_chine_string(char *str, SLONG seg_len, SLONG *segments);
EXPORT  SLONG   FNBACK  find_word_clip_position(char *str, SLONG start, SLONG len);
EXPORT  SLONG   FNBACK  find_word_reserved_last_position(char *str, SLONG reserved_len);
EXPORT  char *  FNBACK  word_upper(char *str);
EXPORT  char *  FNBACK  word_lower(char *str);  
EXPORT  char *  FNBACK  delete_last_word(char *str);
EXPORT  SLONG   FNBACK  find_prev_word_position(char *str, SLONG now_position);
EXPORT  SLONG   FNBACK  find_next_word_position(char *str, SLONG now_position);
EXPORT  SLONG   FNBACK  delete_after_cursor_word(char *str, SLONG now_position);
EXPORT  SLONG   FNBACK  delete_before_cursor_word(char *str, SLONG now_position);
EXPORT  SLONG   FNBACK  insert_string_at_cursor(char *str, char *insert_str, SLONG now_position);
EXPORT  SLONG   FNBACK  find_before_cursor_position(char *str, SLONG now_position, SLONG before_length);
EXPORT  SLONG   FNBACK  find_after_cursor_position(char *str, SLONG now_position, SLONG after_length);
EXPORT  SLONG   FNBACK  get_buffer_line(char *buffer, SLONG *buffer_index, SLONG buffer_size, 
										char *line, SLONG line_size);
EXPORT  SLONG   FNBACK  get_string(USTR *out,SLONG out_max,USTR *in,SLONG *in_from,
                                   USTR *end_list,SLONG end_len);
EXPORT  void    FNBACK  skip_compartment(USTR *in,SLONG *in_from,USTR *skip_list,SLONG skip_len);

//--- misc functions ------------------------------------------------
EXPORT  int     FNBACK  string_2_number(char *str);
EXPORT  SLONG   FNBACK  load_file_to_buffer(USTR *filename, USTR **buffer);
EXPORT  SLONG   FNBACK  get_buffer_number(USTR *buffer, SLONG *index);
EXPORT  USTR *  FNBACK  get_buffer_string(USTR *buffer, SLONG *index);


#endif//_XSYSTEM_H_INCLUDE_

