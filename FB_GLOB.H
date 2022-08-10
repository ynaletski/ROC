/****************************************************************************/
/*  fb_glob.h      ROC407 global structure defintions                       */
/*  Copyright (c)  1995  Fisher Controls International Incorporated         */
/*                                                                          */
/*  CONFIDENTIAL                                                            */
/*  This program is the property of Fisher Controls. Its configuration and  */
/*  algorithms are confidential and may not be used, reproduced, or         */
/*  revealed to others except in accord with contract or other written      */
/*  consent of Fisher Controls. Any permitted  reproduction, in whole or in */
/*  part, must bear this legend.                                            */
/****************************************************************************/
/*  This file contains the declaration of global structures in the ROC407   */
/****************************************************************************/
/*
 *  10/24/95  GSW  Created new - fb407.h
 */
/****************************************************************************/

#ifndef FB407_H
#define FB407_H

#define MAX_PT_TYPES         60

#define NUM_TASKS            10
#define IO_TASK               0
#define SYS_TASK              1
#define COMM_TASK             2
#define DB_TASK               3
#define FST_TASK              4
#define PID_TASK              5
#define AGA_TASK              6
#define USER_TASK             7
#define USER2_TASK            8
#define WAIT_TASK             9

#define OPCODE_TYPE    (UINT8) 0
#define DI_TYPE        (UINT8) 1
#define DO_TYPE        (UINT8) 2
#define AI_TYPE        (UINT8) 3
#define AO_TYPE        (UINT8) 4
#define PI_TYPE        (UINT8) 5
#define PID_TYPE       (UINT8) 6
#define AGA_TYPE       (UINT8) 7
#define LCD_TYPE       (UINT8) 9
#define FLW_TYPE       (UINT8) 10
#define TANK_TYPE      (UINT8) 11
#define CLK_TYPE       (UINT8) 12
#define FLG_TYPE       (UINT8) 13
#define COM_TYPE       (UINT8) 14
#define SYS_TYPE       (UINT8) 15
#define FST_TYPE       (UINT8) 16
#define SPT_TYPE       (UINT8) 17
#define FST_INFO       (UINT8) 18
#define DB_TYPE        (UINT8) 19
#define TASK_TYPE      (UINT8) 20
#define USER_TYPE      (UINT8) 21
#define STEAM_TYPE     (UINT8) 27
#define HOST_TYPE      (UINT8) 36
#define RUN_TYPE       (UINT8) 41
#define EXTRARUN_TYPE  (UINT8) 42
#define ULIST_TYPE     (UINT8) 43
#define PWR_CTL_TYPE   (UINT8) 44

#define CAL_TYPE       (UINT8) 56
#define LOGON_TYPE     (UINT8) 57
#define FLASH_TYPE     (UINT8) 59

#define NUM_EVENTS         240    /* no of events in events log */
#define NUM_ALARMS         240    /* no of alarms in alarms log */

#define NUM_300_DB_POINTS   87
#define NUM_407_DB_POINTS   50
#define NUM_HIST_DAYS       35    /* no of days of historical data saved */
#define NUM_HIST_PERIODS   840    /* no of historical data periods, 840 = 35 days * 24 hrs/day */

#define  AC_TYPE             0   /*    ascii character */
#define  SC_TYPE             1   /*   signed character */
#define  UC_TYPE             2   /* unsigned character */
#define  SI_TYPE             3   /*   signed int       */
#define  UI_TYPE             4   /* unsigned int       */
#define  SL_TYPE             5   /*   signed long      */
#define  UL_TYPE             6   /* unsigned long      */
#define  FL_TYPE             7   /*          float     */
#define  BN_TYPE             8   /*          binary    */
#define  AC10_TYPE           9   /* 10 character array */
#define  AC20_TYPE          10   /* 20 character array */
#define  AC30_TYPE          11   /* 30 character array */ /* FST */
#define  AC40_TYPE          12   /* 40 character array */ /* FST */
#define  TLP_TYPE           13   /* type, logical num, parameter */
#define  AC12_TYPE          14   /* 12 character array */
#define  AC3_TYPE           15   /* 3 character array  */ 
#define  HEX_TYPE           16   /* single char hex    */
#define  HX4_TYPE           17   /* 4 digit hex        */
#define  HX2_TYPE           18   /* 2 digit hex        */
#define  TM_TYPE            19   /* 6 digit time       */

#define CHECK_CRC               Misc_flag[0]    /*  check/ignore crc (0/1)       */
#define DI_PI_SELECT            Misc_flag[1]    /*  select on-board DI or PIs    */
#define USER_LCD_ENABLE         Misc_flag[2]    /*  user LCD enable              */
#define OP_PORT_ENABLE          Misc_flag[3]    /*  Oper port enable             */
#define CLEAR_MEMORY            Misc_flag[4]    /*  memory clear                 */
#define USER_COM1_ENABLE        Misc_flag[5]    /*  user com1 enable             */
#define USER_COM2_ENABLE        Misc_flag[6]    /*  user com2 enable             */
#define USER_PROGRAM_ENABLE     Misc_flag[7]    /*  user prog enable             */
#define RTS_OP_PORT             Misc_flag[8]    /*  RTS Oper Port                */
#define RTS_COM1_PORT           Misc_flag[9]    /*  RTS COM1                     */
#define RTS_COM2_PORT           Misc_flag[10]   /*  RTS COM2                     */
#define CLEAR_EEPROM            Misc_flag[11]   /*  clear EEPROM                 */
#define IO_SCAN_ENABLE          Misc_flag[12]   /*  enable i/o scan              */
#define AUX2_OUT                Misc_flag[13]   /*  aux out #2 ON                */
#define AUX1_OUT                Misc_flag[14]   /*  aux out #1 ON                */
#define COLD_START              Misc_flag[15]   /*  1 = total initialization     */
#define WARM_START              Misc_flag[16]   /*  1 = cold start               */
#define READ_IO                 Misc_flag[17]   /*  1 = warm start               */
#define EEPROM_WR_ENABLE        Misc_flag[18]   /*  1 = write to eeprom          */
#define EEPROM_WR_COMPLETE      Misc_flag[19]   /*  1 = write to eeprom complete */
#define INIT_HISTORY            Misc_flag[20]   /*  init history                 */
#define UPDATE_ENABLE           Misc_flag[22]   /*  update enable(ao,do,pid)     */
#define LCD_ENABLE              Misc_flag[26]   /*  lcd disable                  */
#define EE_VALID                Misc_flag[27]   /*  eeprom data valid            */
#define INIT_DISPLAYS           Misc_flag[28]   /*  initialize displays          */
#define INIT_FST                Misc_flag[29]
#define FULL_INIT               Misc_flag[30]   /*  Full(1)/Partial(0) initialization */
#define POWER_UP                Misc_flag[31]   /*  Power up flag, 1 = powerup */
#define FLASH_FAIL              Misc_flag[32]
#define COM0_CONTINUE           Misc_flag[37]   /*  Oper port continue  */
#define COM1_CONTINUE           Misc_flag[38]   /*  Com1 port continue  */
#define COM2_CONTINUE           Misc_flag[39]   /*  Com2 port continue  */


EXTERN UINT8 *Misc_flag;

EXTERN struct roc_io_pt 
  {
  UINT8 type[70];
  UINT8 pos[70];
  UINT8 logicnum[70];
  UINT8 DI_base;
  UINT8 DO_base;
  UINT8 PI_base;
  UINT8 AO_base;
  UINT8 AI_base;
  UINT8 io_base[6];
  } Io;

struct roc_cfg_def 
  {
  UINT8   manual;                 /* config[0] manual mode flag */
  UINT8   alarm;                  /* config[1] alarm flag */
  UINT8   task_ticks;             /* config[2]*/
  UINT8   exp_brd;                /* not used */
  UINT8   ao_brd;                 /* not used */       
  UINT8   io_unused;              /* not used */
  UINT8   num_dis;                /* config[6]  num_dis; */ 
  UINT8   num_dos;                /* config[7]  num_dos;  */
  UINT8   num_ais;                /* config[8]  num_ais;  */
  UINT8   num_aos;                /* config[9]  num_aos;  */
  UINT8   num_pids;               /* config[10] num_pids  */
  UINT8   num_agas;               /* config[11] num_agas  */
  UINT8   num_logs;               /* config[12] num of items in database */
  UINT8   num_history_days;       /* config[13] */
  UINT8   num_min_per_period;     /* config[14] */
  UINT8   num_pis;                /* config[15] */
  UINT8   num_esd;                /* config[16] */
  UINT8   num_tanks;              /* config[17] */
  UINT8   num_fsts;               /* config[18] */
  UINT8   cfg_spare;              /* config[19] */

  UINT8   *time_date_addr;        /* config[20] */
  UINT8   *event_ptr;             /* config[24] */
  UINT8   *alarm_ptr;             /* config[28] */
  UINT16  *hist_period;           /* config[32] */
  UINT8   *hist_day;              /* config[36] */
  UINT8   *num_hist_days;         /* config[40] */
  UINT8   *db_address;            /* config[44] */
  UINT8   *num_archived_periods;  /* config[48] */
  UINT8   *db_time_start;         /* config[52] */
  UINT8   *hist_db_org;           /* config[56] */
  void    *user_title;            /* config[60] */
  UINT8   *struct_start;          /* config[64] */
  UINT8   *min_start;             /* config[68] */        

  UINT8   *roc_address;           /* config[72] */        
  UINT8   *daily;                 /* config[76] */
  void    *user_ticks;            /* config[80] */

  void    *pid_addr;              /* config[84] */
  void    *aga_add;               /* config[88] */
  UINT8   *com1_delay;            /* config[92] */
  UINT8   *com1_tmout;            /* config[96] */
  UINT8   *com2_delay;            /* config[100] */
  UINT8   *com2_tmout;            /* config[104] */
  void    *check_task_add;        /* config[108] */
  void    *get_tlp_addr;          /* config[112] */

  UINT16  *ten_ms_ticks;          /* config[116] */
  void    *table_start;           /* config[120] */
  void    *table;                 /* config[124] */
  UINT8   *evt;                   /* config[128] */
  void    *task_addr;             /* config[132] */
  void    *soft_pt_addr;          /* config[136] */
  UINT8   *misc_flag;             /* config[140] */
  void    *sensor_addr;           /* config[144] */
  void    *io_addr;               /* config[148] */
  void    *pt_type_pos_addr;      /* config[152] */
  void    *comm_init;             /* config[156] */
  void    *hhd_add_addr;          /* config[160] */
  void    *roc_display;           /* config[164] */
  void    *alarm_pt_array_addr;   /* config[168] */
  void    *log_event_addr;        /* config[172] */
  void    *log_alarm_addr;        /* config[176] */
  void    *log_param_addr;        /* config[180] */
  void    *a_euout_addr;          /* config[184] */

  void    *do_euout_addr;         /* config[188] */
  void    *set_time_addr;         /* config[192] */
  UINT8   *flt_buf_addr;          /* config[196] */
  void    *com_process_addr;      /* config[200] */
  UINT8   *Umem_alloc;            /* config[204] */
  UINT16  *crc_table;             /* config[208] */
  UINT8   *keypad;                /* config[212] */
										                                                                                                                                                                                              
  void    *lcd_pt;                /* config[216] */
  void    *sio_addr;              /* config[220] */
  UINT8   *pt_base_addr;          /* config[224] */
  UINT8   *number_parms_addr;     /* config[228] */
  void    *log_io_alarm_addr;     /* config[232] */
  void    *ee_restore_addr;       /* config[236] */
  void    *log_mtlp_addr;         /* config[240] */
  void    *check_events_addr;     /* config[244] */
  void    *eemask_addr;           /* config[248] */
  void    *extra_io_addr;         /* config[252] */
  };

EXTERN struct roc_cfg_def *Config;


struct roc_sys_def
  {
  UINT8    roc_address;
  UINT8    roc_group;
  UINT8    station_name[20];
  UINT8    cfg_pid;
  UINT8    cfg_aga;
  UINT8    cfg_tanks;
  UINT8    hist[3];
  UINT8    frc_cntrt_day;
  UINT8    contract_hour;
  UINT8    part_number[20];
  UINT8    fisher_id[20];
  UINT8    burntime[20];
  UINT8    serial_num[12];
  UINT8    customer_name[20];
  UINT8    activated_pid;
  UINT8    activated_aga;
  UINT8    activated_tanks;
  UINT8    activated_fsts;
  UINT8    ram;
  UINT8    rom;
  FLOAT    mpu_load;
  UINT8    utilities;
  UINT16   type_of_roc;
  };

EXTERN struct roc_sys_def *Roc_sys;


struct table_start_def
  {
  UINT16  start;
  UINT8   cfg_num;
  UINT16  size;
  UINT8   *address;
  };

EXTERN struct table_start_def *Table_start;


struct table_407_def
  {
  UINT8  type;       
  UINT8  start_byte;
  UINT8  param_num;  
  UINT8  wr_en;      
  UINT8  data_def;   
  UINT8  label[18];   
  };

EXTERN struct table_407_def *Table407;


struct table_300_def
  {
  UINT8  type;       
  UINT8  start_byte;
  UINT8  param_num;  
  UINT8  wr_en;      
  UINT8  data_def;   
  };

EXTERN struct table_300_def *Table300;


struct comm_def
  {
  UINT8  tag[10];       
  UINT16 baud_rate;     
  UINT8  stop_bits;     
  UINT8  num_bits;      
  UINT8  parity;        
  UINT8  status;        
  UINT8  mode;          
  UINT8  keyon_delay;   
  UINT8  keyoff_delay;  
  UINT8  num_retries;   
  UINT16 extra_keyon;   
  UINT16 alarm_index;     
  UINT16 recv_ctr_copy; 
  UINT16 retry_idx;     
  UINT16 valid_rcv_ctr; 

  UINT8  modem_status;                       
  UINT8  modem_type;                       
  FLOAT  connect_time;    

  UINT8  modem_init[40];  
  UINT8  phone_number[40];

  FLOAT  disconnect_time; 
  FLOAT  inactivity_time; 

  struct rbx_retry_def
    {
    FLOAT  time;   
    UINT8  max_tries;
    } rbx_retry[3]; 

  UINT8  rbx_address;     
  UINT8  rbx_group;       

  UINT8  saf_address1;    
  UINT8  saf_group1;      
  UINT8  saf_address2;    
  UINT8  saf_group2;      
  UINT8  saf_address3;    
  UINT8  saf_group3;      
  };

EXTERN struct comm_def *Comm_init;


struct siotype
  {
  UINT8  rx_buff[1024];
  UINT8  *headp;
  UINT8  *tailp;
  UINT8  *byte2_of_msg;
  UINT8  *start_of_msg;
  UINT8  *eorb;
  UINT16 crc_rcvd;
  INT16  index;
  INT16  databyte_ctr;
  INT16  rx_mode;
  INT16  timeout;
  INT16  rts;
  INT16  delay;
  INT16  task_ptr;
  UINT8  tx_buff[1024];
  UINT8  *tx_ptr;
  UINT8  *eotb;
  INT16  tx_mode;
  INT16  tx_nchars;
  UINT16 port_addr;
  INT16  port_type; 
  UINT8  temp_buff[265];
  };

EXTERN struct siotype *Sio;


struct user_title_def
  {
  UINT8  tag[20];
  UINT8  *template_ptr;
  UINT8  num_params;
  UINT8  spare;
  };

EXTERN struct user_title_def *User_title;


struct history_300_def
  {
  UINT8  validation[6]; /*"Fisher"*/
  UINT16 hourly_ptr;
  UINT8  daily_ptr;
  UINT8  num_hist_days;
  UINT16 num_archived_periods;
  UINT8  periods_per_day;
  UINT8  cfg_point;
  UINT8  arc_type;
  UINT8  type;
  UINT8  log_num;
  UINT8  parm;
  UINT8  port;
  UINT8  spare[5];                /* 24 bytes */

  struct db_300_point_def
    {
    UINT8  *tag;
    UINT8  arc_type;
    UINT8  type;
    UINT8  log_num;
    UINT8  parm;
    FLOAT  yesterday_daily;
    FLOAT  cur_val;
    FLOAT  min_val;
    FLOAT  max_val;
    UINT8  min_time[5];
    UINT8  max_time[5];
    UINT8  min_max_yesday[18];
    FLOAT  last_hour_val;        /* ave/sum for last completed hour  */
    UINT16 cur_hourly_ptr;       /* pointer to hourly value archived */
    FLOAT  minute_sum;        
    FLOAT  period_sum;           /* current period (60 mins by default) */
    FLOAT  daily_sum;            /* current day */
    UINT16 minute_sum_ctr;
    UINT16 period_sum_ctr;
    UINT16 daily_sum_ctr;
    UINT8  min_max_reset_flag;   /* when flag = 0, min = max = cur value */
    UINT8  cfg_flag;          
    } point300[NUM_300_DB_POINTS]; 
  
  struct db_300_time_def
    {
    UINT8  minute;               /* 0 - 59 */
    UINT8  hour;                 /* 0 - 23 */
    UINT8  day;                  /* 1 - 31 */
    UINT8  month;                /* 1 - 12 */
    } time_stamp300[NUM_HIST_PERIODS+35];

  struct timestamp_300_ptr_def
    {
    INT16  hourly_ptr;
    UINT8  daily_ptr;
    } ts_ptr300[12*32];
  };

EXTERN struct history_300_def *Hist300;


struct history_407_def
  {
  UINT8  validation[6]; /* revision number */
  UINT16 hourly_ptr;
  UINT8  daily_ptr;
  UINT8  num_hist_days;
  UINT16 num_archived_periods;
  UINT8  periods_per_day;
  UINT8  cfg_point;
  UINT8  arc_type;
  UINT8  type;
  UINT8  log_num;
  UINT8  parm;
  UINT8  port;
  UINT8  spare[5];                /* 24 bytes */

  struct db_407_point_def                          /* offset 0x018 */
    {
    UINT8    *tag;
    UINT8    arc_type;
    UINT8    type;
    UINT8    log_num;
    UINT8    parm;
    FLOAT    yesterday_daily;
    FLOAT    cur_val;
    FLOAT    min_val;
    FLOAT    max_val;
    UINT8    min_time[5];
    UINT8    max_time[5];
    UINT8    min_max_yesday[18];
    FLOAT    last_hour_val;        /* ave/sum for last completed hour  */
    UINT16   cur_hourly_ptr;       /* pointer to hourly value archived */
    FLOAT    minute_sum;        
    FLOAT    hour_sum;             /* current hour */
    FLOAT    period_sum;           /* current period (60 mins by default) */
    FLOAT    daily_sum;            /* current day */
    UINT16   minute_sum_ctr;
    UINT16   hour_sum_ctr;
    UINT16   period_sum_ctr;
    UINT16   daily_sum_ctr;
    UINT8    min_max_reset_flag;   /* when flag = 0, min = max = cur value */
    UINT8    cfg_flag;             
    } point407[NUM_407_DB_POINTS];
  
  struct db_407_time_def                           /* offset 0x0fb8 */
    {
    UINT8    minute;               /* 0 - 59 */
    UINT8    hour;                 /* 0 - 23 */
    UINT8    day;                  /* 1 - 31 */
    UINT8    month;                /* 1 - 12 */
    } time_stamp407[NUM_HIST_PERIODS+35];   /* 4 bytes * 875 */

  struct timestamp_407_ptr_def
   {
   UINT8     daily_ptr;
   INT16     hourly_ptr;
   } ts_ptr407[12*32];
  };

EXTERN struct history_407_def *Hist407;


struct db_history_data_def
  {
  FLOAT   min_avg[60];
  FLOAT   hourly[840];
  FLOAT   daily[35];
  };                    /* 3740 bytes */

EXTERN struct db_history_data_def *Db_300_ptr;
EXTERN struct db_history_data_def *Db_407_ptr;

#endif
