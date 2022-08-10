/***
*fb407.h - definitions for fblib.lib
*
*
****************************************************************************/

#if  ! defined (ROCLIB)  
extern                               
#endif     
struct ROC_IO_PT {
    unsigned char type[70];
    unsigned char pos[70];
    unsigned char logicnum[70];
    unsigned char DI_base;
    unsigned char DO_base;
    unsigned char PI_base;
    unsigned char AO_base;
    unsigned char AI_base;
    unsigned char io_base[6];
    } Io;

#if ! defined (ROCLIB)
extern
#endif
struct roc_cfg_def {
  unsigned char manual;                 /* config[0] manual mode flag */
  unsigned char alarm;                  /* config[1] alarm flag */
  unsigned char task_ticks;             /* config[2]*/
  unsigned char exp_brd;                /* not used */
  unsigned char ao_brd;                 /* not used */       
  unsigned char io_unused;              /* not used */
  unsigned char num_dis;                /* config[6]  num_dis; */ 
  unsigned char num_dos;                /* config[7]  num_dos;  */
  unsigned char num_ais;                /* config[8]  num_ais;  */
  unsigned char num_aos;                /* config[9]  num_aos;  */
  unsigned char num_pids;               /* config[10] num_pids  */
  unsigned char num_agas;               /* config[11] num_agas  */
  unsigned char num_logs;               /* config[12] num of items in database */
  unsigned char num_history_days;       /* config[13] */
  unsigned char num_min_per_period;     /* config[14] */
  unsigned char num_pis;                /* config[15] */
  unsigned char num_esd;                /* config[16] */
  unsigned char num_tanks;              /* config[17] */
  unsigned char num_fsts;               /* config[18] */
  unsigned char cfg_spare;              /* config[19] */

  unsigned char *time_date_addr;        /* config[20] */
  unsigned char *event_ptr;             /* config[24] */
  unsigned char *alarm_ptr;             /* config[28] */
  unsigned int  *hist_period;           /* config[32] */
  unsigned char *hist_day;              /* config[36] */
  unsigned char *num_hist_days;         /* config[40] */
  unsigned char *db_address;            /* config[44] */
  unsigned char *num_archived_periods;  /* config[48] */
  unsigned char *db_time_start;         /* config[52] */
  unsigned char *hist_db_org;           /* config[56] */
  void          *user_title;            /* config[60] */
  unsigned char *struct_start;          /* config[64] */
  unsigned char *min_start;             /* config[68] */        

  unsigned char *roc_address;           /* config[72] */        
  unsigned char *daily;                 /* config[76] */
  void          *user_ticks;            /* config[80] */

  void          *pid_addr;              /* config[84] */
  void          *aga_add;               /* config[88] */
  unsigned char *com1_delay;            /* config[92] */
  unsigned char *com1_tmout;            /* config[96] */
  unsigned char *com2_delay;            /* config[100] */
  unsigned char *com2_tmout;            /* config[104] */
  void          *check_task_add;        /* config[108] */
  void          *get_tlp_addr;          /* config[112] */

  unsigned int  *ten_ms_ticks;          /* config[116] */
  void          *table_start;           /* config[120] */
  void          *table;                 /* config[124] */
  unsigned char *evt;                   /* config[128] */
  void          *task_addr;             /* config[132] */
  void          *soft_pt_addr;          /* config[136] */
  unsigned char *misc_flag;             /* config[140] */
  void          *sensor_addr;           /* config[144] */
  void          *io_addr;               /* config[148] */
  void          *pt_type_pos_addr;      /* config[152] */
  void          *comm_init;             /* config[156] */
  void          *hhd_add_addr;          /* config[160] */
  void          *roc_display;           /* config[164] */
  void          *alarm_pt_array_addr;   /* config[168] */
  void          *log_event_addr;        /* config[172] */
  void          *log_alarm_addr;        /* config[176] */
  void          *log_param_addr;        /* config[180] */
  void          *a_euout_addr;          /* config[184] */

  void          *do_euout_addr;         /* config[188] */
  void          *set_time_addr;         /* config[192] */
  unsigned char *flt_buf_addr;          /* config[196] */
  void          *com_process_addr;      /* config[200] */
  unsigned char *Umem_alloc;            /* config[204] */
  unsigned int  *crc_table;             /* config[208] */
  unsigned char *keypad;                /* config[212] */
										                                                                                                                                                                                              
  void          *lcd_pt;                /* config[216] */
  void          *Sio_addr;              /* config[220] */
  unsigned char *Pt_base_addr;          /* config[224] */
  unsigned char *Number_parms_addr;     /* config[228] */
  void          *log_io_alarm_addr;     /* config[232] */
  void          *ee_restore_addr;       /* config[236] */
  void          *log_mtlp_addr;         /* config[240] */
  void          *check_events_addr;     /* config[244] */
  void          *EEmask_addr;           /* config[248] */
  void          *extra_io_addr;         /* config[252] */
  };

#if ! defined (ROCLIB)
extern
#endif
struct roc_cfg_def *Config;

#ifndef ROC_SYS_DEF
#define ROC_SYS_DEF   1

#if ! defined (ROCLIB)
extern
#endif
struct roc_sys_def
{
 unsigned char    roc_address;
 unsigned char    roc_group;
 unsigned char    station_name[20];
 unsigned char    cfg_pid;
 unsigned char    cfg_aga;
 unsigned char    cfg_tanks;
 unsigned char    hist[3];
 unsigned char    frc_cntrt_day;
 unsigned char    contract_hour;
 unsigned char    part_number[20];
 unsigned char    fisher_id[20];
 unsigned char    burntime[20];
 unsigned char    serial_num[12];
 unsigned char    customer_name[20];
 unsigned char    activated_pid;
 unsigned char    activated_aga;
 unsigned char    activated_tanks;
 unsigned char    activated_fsts;
 unsigned char    ram;
 unsigned char    rom;
 float            mpu_load;
 unsigned char    utilities;
 unsigned char    extra;
 unsigned char    code_size[4];
 unsigned char    eprom_crc[2];
};

#if ! defined (ROCLIB)
extern
#endif
struct roc_sys_def *Roc_sys;

#endif

#if ! defined (ROCLIB)
extern
#endif
struct comm_def
{
 unsigned char tag[10];
 int           baud_rate;
 unsigned char stop_bits;
 unsigned char num_bits;   
 unsigned char parity;   
 unsigned char status; 
 unsigned char mode; 
 unsigned char keyon_delay; 
 unsigned char turnaround_delay; 
 unsigned char num_retry; 
 unsigned int  retry_time; 
 unsigned int  alarm_ptr;
 unsigned int  recv_ctr_copy;
 unsigned int  retry_ctr; 
 unsigned int  valid_rcv_ctr; /* added JGK */
 unsigned char spare[2];
 unsigned int  delay_task; 
 unsigned int  chan_clear; 
};

#if ! defined (ROCLIB)
extern
#endif
struct comm_def *Comm_init;


#if ! defined (ROCLIB)
extern
#endif
struct table_start_def
{
 unsigned int  start;
 unsigned char cfg_num;
 unsigned int  size;
 unsigned char *address;
};

#if ! defined (ROCLIB)
extern
#endif
struct table_start_def *Table_start;


#if ! defined (ROCLIB)
extern
#endif
struct table_def
{
 unsigned char  type;       
 unsigned char  start_byte;
 unsigned char  param_num;  
 unsigned char  wr_en;      
 unsigned char  data_def;   
 unsigned char  label[18];   
};

#if ! defined (ROCLIB)
extern
#endif
struct table_def *Table;


#if ! defined (ROCLIB)
extern
#endif
struct user_title_def
{
 unsigned char  tag[20];
 unsigned char  *template_ptr;
 unsigned char  num_params;
 unsigned char  spare;

};

#if ! defined (ROCLIB)
extern
#endif
struct user_title_def *User_title;


#if ! defined (ROCLIB)
extern
#endif
struct task_def
{
  unsigned int  sp;
  unsigned int  ss;
  unsigned char priority;
  unsigned char status;
  unsigned char task_name[10];
  unsigned int  child;
  unsigned int  entry_time;
  unsigned int  exit_time;
  unsigned char timer;
  unsigned char set_time;
  unsigned int  pass_counter;
  void          *task_addr;
  unsigned int  data_seg;      
  unsigned int  flt_buf[27];
  unsigned int  stack[512];
};

#if ! defined (ROCLIB)
extern
#endif
struct task_def *Task;

#if defined(FLASHPAC)
#define NUM_DB_POINTS     87
#else
#define NUM_DB_POINTS     50
#endif
#define NUM_HIST_DAYS     35    /* no of days of historical data saved */
#define NUM_HIST_PERIODS 840    /* no of historical data periods, 840 = 35 days * 24 hrs/day */

#if ! defined (ROCLIB)
extern
#endif
struct history_def
  {
  unsigned char validation[6]; /*"Fisher"*/
  unsigned int  hourly_ptr;
  unsigned char daily_ptr;
  unsigned char num_hist_days;
  unsigned int  num_archived_periods;
  unsigned char periods_per_day;
  unsigned char cfg_point;
  unsigned char arc_type;
  unsigned char type;
  unsigned char log_num;
  unsigned char parm;
  unsigned char port;
  unsigned char spare[5];                /* 24 bytes */

  struct db_point_def
    {
    unsigned char *tag;
    unsigned char arc_type;
    unsigned char type;
    unsigned char log_num;
    unsigned char parm;
    float         cur_val;
    float         min_val;
    float         max_val;
    unsigned char min_time[5];
    unsigned char max_time[5];
    unsigned char min_max_yesday[18];
    float         last_hour_val;        /* ave/sum for last completed hour  */
    unsigned int  cur_hourly_ptr;       /* pointer to hourly value archived */
    float         minute_sum;        
    float         hour_sum;             /* current hour */
    float         period_sum;           /* current period (60 mins by default) */
    float         daily_sum;            /* current day */
    unsigned int  minute_sum_ctr;
    unsigned int  hour_sum_ctr;
    unsigned int  period_sum_ctr;
    unsigned int  daily_sum_ctr;
    unsigned char min_max_reset_flag;   /* when flag = 0, min = max = cur value */
    unsigned char cfg_flag;          
    }point[NUM_DB_POINTS];      /* 80 bytes times 50 */
  
  struct db_time_def
    {
    unsigned char minute;               /* 0 - 59 */
    unsigned char hour;                 /* 0 - 23 */
    unsigned char day;                  /* 1 - 31 */
    unsigned char month;                /* 1 - 12 */
    } time_stamp[NUM_HIST_PERIODS+35];

  struct timestamp_ptr_def
    {
    unsigned char daily_ptr;
    int           hourly_ptr;
    }ts_ptr[12*32];
  } *Hist;                      /* 4 x 875 bytes */

#if ! defined (ROCLIB)
extern
#endif
struct db_history_data_def
  {
  float min_avg[60];
  float hourly[840];
  float daily[35];
  } *Db_ptr;                    /* 3740 bytes */
