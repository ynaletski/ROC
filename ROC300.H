/***
*roc300.h - definitions for roclib.lib
*
*
****************************************************************************/

#if ! defined (ROCLIB)
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
struct Roc_cfg {
    unsigned char       manual;                 /* config[0] manual mode flag */
    unsigned char       alarm;                  /* config[1] alarm flag */
    unsigned char       main_brd;               /* not used */
    unsigned char       exp_brd;                /* not used */
    unsigned char       ao_brd;                 /* not used */
    unsigned char       io_unused;              /* not used */
    unsigned char       num_dis;                /* config[6]  num_dis; */ 
    unsigned char       num_dos;                /* config[7]  num_dos;  */
    unsigned char       num_ais;                /* config[8]  num_ais;  */
    unsigned char       num_aos;                /* config[9]  num_aos;  */
    unsigned char       num_pids;               /* config[10] num_pids  */
    unsigned char       num_agas;               /* config[11] num_agas  */
    unsigned char       num_logs;               /* config[12] num of items in database */
    unsigned char       num_history_days;       /* config[13] */
    unsigned char       num_min_per_period;     /* config[14] */
    unsigned char       num_pis;                /* config[15] */
    unsigned char       num_esd;                /* config[16] */
    unsigned char       num_tanks;              /* config[17] */
    unsigned char       num_fsts;               /* config[18] */

    unsigned char       user_c_active;          /* config[19] */

    unsigned char       *time_date_add;          /* config[20] */
    unsigned char       *event_ptr;              /* config[24] */
    unsigned char       *alarm_ptr;              /* config[28] */
    unsigned int        *hist_period;            /* config[32] */
    unsigned char       *hist_day;               /* config[36] */
    unsigned char       *num_hist_days;          /* config[40] */
    unsigned char       *db_address;             /* config[44] */
    unsigned char       *num_archived_periods;   /* config[48] */
    unsigned char       *db_time_start;          /* config[52] */
    unsigned char       *hist_db_org;            /* config[56] */
    unsigned char       *user_title;             /* config[60] */
    unsigned char       *struct_start;           /* config[64] */
    unsigned char       *min_start;              /* config[68] */

    unsigned char       *roc_address;            /* config[72] */
    unsigned char       *not_daily;              /* config[76] */
    unsigned char	      *user_ticks;             /* config[80] */

    unsigned int        pid_add;                 /* config[84] */
    unsigned int        pid_add_seg;
    unsigned int        aga_add;                 /* config[88] */
    unsigned int        aga_add_seg;
    unsigned char       *com1_delay;             /* config[92] */
    unsigned char       *com1_tmout;             /* config[96] */
    unsigned char       *com2_delay;             /* config[100] */
    unsigned char       *com2_tmout;             /* config[104] */
    unsigned int        check_task_add;          /* config[108] */
    unsigned int        check_task_seg;
    unsigned int        spare[2];                /* config[112] */
    unsigned char	      *ten_ms_ticks;           /* config[116] */
    unsigned char       *table_start;            /* config[120] */
    unsigned char       *table;                  /* config[124] */
    unsigned char       *evt;                    /* config[128] */
    unsigned int        task_add;                /* config[132] */
    unsigned int        task_add_seg;
    unsigned char       *soft_pt_add;            /* config[136] */
    unsigned char       *misc_flag;              /* config[140] */
    unsigned int        tank_add;                /* config[144] */
    unsigned int        tank_add_seg;
    unsigned char       *io_add;                 /* config[148] */
    unsigned char       *pt_type_pos_add;        /* config[152] */
    unsigned char       *comm_init;              /* config[156] */
    unsigned int        hhd_add;                 /* config[160] */
    unsigned int        hhd_add_seg;
    unsigned char       *roc_display;            /* config[164] */
    unsigned char	      *task_addr;              /* config[168] */
    unsigned int        log_event_add;           /* config[172] */
    unsigned int        log_event_seg;
    unsigned int        log_alarm_add;           /* config[176] */
    unsigned int        log_alarm_seg;
    unsigned int        log_param_add;           /* config[180] */
    unsigned int        log_param_seg;
    unsigned int        a_euout_add;             /* config[184] */
    unsigned int        a_euout_seg;
    unsigned int        do_euout_add;            /* config[188] */
    unsigned int        do_euout_seg;
    unsigned int        set_time_add;            /* config[192] */
    unsigned int        set_time_seg;
    unsigned char	      *flt_buf_addr;           /* config[196] */
    unsigned int        p_comR_add;              /* config[200] */
    unsigned int        p_comR_seg;
  }config;

#if ! defined (ROCLIB)
extern
#endif
struct Start_table_def {
  unsigned int  start;
  unsigned char cfg_num;
  unsigned int  size;
  unsigned char *address;
  } Table_start[40];
