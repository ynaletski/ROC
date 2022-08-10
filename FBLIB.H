/****************************************************************************/
/* fblib.h        ROC407 library function prototypes                        */
/*                                                                          */
/* Copyright (c)  1997  Fisher Controls International Incorporated          */
/*                                                                          */
/*   CONFIDENTIAL                                                           */
/* This program is the poperty of Fisher Controls.  Its configuration and   */
/* algorithms are confidential and may not be used, reproduced, or revealed */
/* to others except in accord with contract or other written consent of     */
/* Fisher Controls. Any permitted reproduction, in whole or in part, must   */
/* bear this legend.                                                        */
/*                                                                          */
/****************************************************************************/
/* This module contains prototypes for functions available in the ROC407    */
/* library (fblib.lib).                                                     */
/****************************************************************************/
/*
 * $Workfile:$
 * $Header:$
 * $Log:$
*/
/****************************************************************************/

#ifndef FBLIB_H
#define FBLIB_H

extern void get_config(void);                 
extern void get_pt_type_pos(void);
extern void log_param(UINT8 *, UINT8 *, INT16, INT16, INT16, INT16, UINT8);
extern void log_event(INT16, INT16, UINT8 *);
extern void log_alarm(INT16, INT16, UINT8 *);
extern UINT8 *get_tlp (UINT8 *, UINT8 *, UINT8 *);
extern void a_euout(INT16, INT16, FLOAT);
extern void do_euout(INT16, FLOAT);
extern void ee_restore(UINT8);
extern void set_time(UINT8 *);
extern void get_time(UINT8 *);
extern INT16 get_param(UINT8, UINT8, UINT8, UINT8 *);
extern INT16 set_param(UINT8, UINT8, UINT8, UINT8 *);
extern INT16 set_log_param(UINT8, UINT8, UINT8, UINT8 *);
extern struct db_history_data_def *db_300_hist_addr (UINT8);
extern struct db_history_data_def *db_407_hist_addr (UINT8);
extern INT16 new_point_type(UINT8, UINT8, UINT16, UINT8 *);
extern INT16 set_template(UINT8, UINT8, UINT8 *, UINT8 *);

#endif
