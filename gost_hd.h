/****************************************************************************/
/* GOST_HD.H      GOST Program Header File                                  */
/*                                                                          */
/*  Copyright (c)  2000  Fisher Controls International Incorporated         */
/*                                                                          */
/*   CONFIDENTIAL                                                           */
/* This program is the poperty of Fisher Controls.  Its configuration and   */
/* algorithms are confidential and may not be used, reproduced, or revealed */
/* to others except in accord with contract or other written consent of     */
/* Fisher Controls. Any permitted reproduction, in whole or in part, must   */
/* bear this legend.                                                        */
/*                                                                          */
/****************************************************************************/
/* (file description)    The file description section should contain a      */
/* brief description of the file and include enough information for someone */
/* to understand what the purpose of the file is and what functionality is  */
/* contained in it. It does not have to contain a detailed design           */
/* description although any information can be included. Authors should use */
/* their own judgement as to the type of information that will be useful.   */
/****************************************************************************/
/* (version control header and history)  If you are using a version control */
/* system, include the archive information and revision history. If you are */
/* not using a version control system, include a few comments along with    */
/* your initials and the date and time in this section each time you modify */
/* the file.                                                                */    
/*  Ver 1.10 (1st for 1.07 & 2.11), Aug 2000                                */                                                                
/****************************************************************************/
/****************************************************************************/
/* Specific GOST Structures, Macros, Prototypes and constants             */
/****************************************************************************/

#ifndef GOST_HD_H
#define GOST_HD_H

/****************************************************************************/
/*			Structures, Macros and	Function Prototypes                     */
/****************************************************************************/
/*----------------- global definitions        ------------------------------*/

#define NO_FLOW         (UINT8)0
#define FLOWING         (UINT8)1

#define IDDLE           (UINT8)0
#define PROCESSING      (UINT8)1

#define LOW             (UINT8)0
#define HIGH            (UINT8)1

#define ALM_CLEAR       (UINT8)0
#define ALM_LOGGED      (UINT8)1
/*======================== Time related Data Defs =========================*/
EXTERN UINT16 OneScanTick;
EXTERN UINT16 OneSecTickAccum;
EXTERN UINT8  PreviousHour;
EXTERN UINT8  PreviousDay;
EXTERN UINT8  PreviousMonth;
EXTERN UINT8  PreviousYear;
EXTERN UINT8  ContrTransferFlag[5];
  
/*======================== Process Variables ==============================*/
EXTERN UINT8 FlowsRunning;
EXTERN UINT8 GlobalCalcStatus;

/*=================== Stored Composition and Options ======================*/
#define NO_COMP_ROC         20
#define NO_COMP_GOST        18 
#define NO_COMP_GOST_ADJ     9 /* 8 majors + sum of last 12 */

EXTERN  UINT16  StoredCompCRC[5];

EXTERN  FLOAT  GOST_Comp[5][NO_COMP_GOST];
EXTERN  FLOAT  GOST_Comp_Adj[5][NO_COMP_GOST_ADJ];

#define PREV_VNIC       (UINT8)1
#define PREV_GERG       (UINT8)2
#define PREV_NX19       (UINT8)3
#define PREV_R_KV       (UINT8)4


/*=================== Stored Volumes and Energies =========================*/
EXTERN struct ProtFlow_def
{
	FLOAT   AccVolToday;
	FLOAT   AccVolYday;
	FLOAT   AccVolMonth;
	FLOAT   AccVolLastm;
	FLOAT   AccVolThisYear;
	FLOAT   AccVolLastYear;
	FLOAT   AccMinFlwToday;
	FLOAT   AccMinFlwYday;
	FLOAT   AccI_EngToday;
	FLOAT   AccI_EngYday;
	FLOAT   AccI_EngMonth;
	FLOAT   AccI_EngLastm;
	FLOAT   AccI_EngThisYear;
	FLOAT   AccI_EngLastYear;
	FLOAT   AccS_EngToday;
	FLOAT   AccS_EngYday;
	FLOAT   AccS_EngMonth;
	FLOAT   AccS_EngLastm;
	FLOAT   AccS_EngThisYear;
	FLOAT   AccS_EngLastYear;

} PFlow[5] = { {0.0, }, };

/*===================  Uninitialized storage ==============================*/

EXTERN FLOAT OneThird;
EXTERN FLOAT Vkij[8][8],Tkij_2[8][8],Oij[8][8];
EXTERN FLOAT sumBase[10];
EXTERN FLOAT sumBase1[10];
EXTERN FLOAT sumBase2[10];

EXTERN FLOAT Cpoi_R[8];

/*==========================================================================*/
/*        Prototypes - logical groups rather than alphabetical              */
/*        Some or All of this functions are needed in a metering program    */
/*==========================================================================*/

/*===================  ROC OS specific =====================================*/
void   initialize           ( void );
UINT16 system_ticks         ( void );
void   integrate            ( void ); 
void   gost_schedule        ( void );

/*===================  timing ==============================================*/
UINT8  IsTimerExpired       ( UINT16 timer, UINT16 currTime);
void   CheckdateAndTime     ( void );
/*===================  Program I/O =========================================*/

void  ReadInputs            ( UINT16 );
void  AverageInputs         ( void );

/*===================  Flow Calculations ===================================*/

void  CalculateFlowRate     ( void );
void  AccVolAndEnergy       ( void );
UINT8 CheckCompAndBaseDens  ( void );  
void  FillUpVNICTables      ( void );
FLOAT calc_Z_for_R_K		(FLOAT, FLOAT, FLOAT);             
void  Reilih_Kvong_Routine    ( FLOAT, FLOAT);
void  VNIC_SMVRoutine       ( FLOAT, FLOAT);
void  GERG_91Routine        ( FLOAT, FLOAT);
void  NX_19Routine          ( FLOAT, FLOAT);
void  CheckGOSTAlarms       ( void );
void  CalcBaseDensity       ( void );
void  RunTimeViscGRNX       ( FLOAT, FLOAT);
void  RunTimeViscVNIC       ( FLOAT );
void  DisplayFlow           ( void );
void  UnitAccVolAndEnergy   ( FLOAT, FLOAT,FLOAT,FLOAT);
 
/*======================  Utilities ========================================*/

UINT16 calc_crc (UINT8 *, UINT16);
UINT8 compare_floats(FLOAT, FLOAT );

/*=================  END OF GOST HEADER FILE ================================*/

#endif

