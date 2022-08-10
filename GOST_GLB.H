/****************************************************************************/
/* GOST_GLB.H      Global GOST Definitions File                             */
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
/*  Ver 3.02 , (Natural Gas+ Orifice/Annubar/Turbine) March 2001 Moscow     */
/****************************************************************************/

#ifndef GOST_GLB_H
#define GOST_GLB_H

/*==========================================================================
    Toggle this preprocessor directive in order to 
	enable messages using different ASCII code pages
 *=========================================================================*/

#define ASCII_ENGLISH
/*#define  ASCII_EXTENDED */

/***************************************************************************/
/* 
 * Jump_main must be the first initialized variable and provides the ROC 
 * operating system with the address of main().  The START.ASM code will 
 * place the address of main() into jump_main.
 */
/***************************************************************************/

#ifndef GLOBALS_DEFINED     /* must be the first initialized variable */
extern                        /* provides operating system with main () address */
unsigned char jump_main[5];
#else
unsigned char jump_main[5] = {"12345"}; /* will be replaced with address */
#endif                                  /* by start.asm */

/***************************************************************************/

EXTERN UINT8   Bit_bias[8]
#ifdef GLOBALS_DEFINED                 /* bit_bias, bit weight of a byte */
 = {1, 2, 4, 8, 16, 32, 64, 128}
#endif
;

#ifndef GLOBALS_DEFINED
extern                        /* check in start.asm for ram valid */
unsigned char ram_check[20];
#else
unsigned char ram_check[20] = {"Ram Check AA55aa55cc"};
#endif

/*
 *		Defines size of data defined by data definitions 
 */
EXTERN INT8 Data_size[20] = {1,1,1,2,2,4,4,4,1,10,20,30,40,3,12,6,0,0,0,0};

/****************************************************************************/

EXTERN UINT8   NUorg, NUend, first_time;

EXTERN UINT8 udp[];                 /* check in start.asm for User Def Point */

EXTERN UINT16 *Crc_table;

EXTERN struct Alarm_defs            /* single alarm log format */
{
  UINT16        code;
  UINT8         time[6];             /* seconds, mins, hour, day, month */
  UINT8         detail[14];
};

EXTERN struct Event_defs            /* single event log format */
{
  UINT16        code;
  UINT8         time[6];             /* seconds, mins, hour, day, month */
  UINT8         detail[14];
};

EXTERN struct t_l_p
{
  UINT8		type;
  UINT8		logicno;
  UINT8		parano;

}UP_TLP;

/*****************************************************************************
 *				System defined Memory Mapping
 ****************************************************************************/

#define System_Address			Table_start[15].address
#define ROC_Address				(System_Address +   0)
#define ROC_Group				(System_Address +   1)
#define Station_Name			(System_Address +   2)
#define Active_PIDs				(System_Address +  22)
#define Active_AGAs				(System_Address +  23)
#define Active_Tanks			(System_Address +  24)
#define Base_DB_Points			(System_Address +  25)
#define RAM1_DB_Points			(System_Address +  26)
#define RAM2_DB_Points			(System_Address +  27)
#define Force_EODay				(System_Address +  28)
#define Contract_Hour			(System_Address +  29)
#define Version_Name			(System_Address +  30)
#define Fisher_ID				(System_Address +  50)
#define AGA_Max					(System_Address + 123)
#define ROC312		((*AGA_Max & 0x01) && (*AGA_Max & 0x02))
#define ROC364		((*AGA_Max & 0x01) && (*AGA_Max & 0x04) && (!(*AGA_Max & 0x02)) )
#define FBOSS		((*AGA_Max & 0x04) && (!(*AGA_Max & 0x02)) && (!(*AGA_Max & 0x01)) )

#define ACTIVE_GOSTS		( *Active_AGAs)
#define AGA_MAX				( *AGA_Max)
#define CONTRACT_HOUR       ( *Contract_Hour)

/*****************************************************************************
 *				ROC Time Mapping
 ****************************************************************************/
 /* time map: sec, min, hour, day, month, year, leap year, day in week */ 

#define Now_Second				(*(Config->time_date_addr + 0))
#define Now_Minute				(*(Config->time_date_addr + 1))
#define Now_Hour				(*(Config->time_date_addr + 2))
#define Now_Day 				(*(Config->time_date_addr + 3))
#define Now_Month				(*(Config->time_date_addr + 4))
#define Now_Year				(*(Config->time_date_addr + 5))

/*----------------- global flags and counters ------------------------------*/
UINT8	flow_no;   					

/****************************************************************************
 *
 *             Fisher Point Types 7-10 Address Maps
 *
 ****************************************************************************/

#define   offset7    Table_start[7].address + flow_no*Table_start[7].size
#define   Tag7                ( offset7 + 0  ) /* point tag */
#define   Roughness           ( offset7 + 10 ) /* Roughness*/
#define   HVal_Sup            ( offset7 + 14 ) /* Sup H Val */
#define   GostOpt1            ( offset7 + 18 ) /* FIRST  OPTION MAP */
#define   GostOpt2            ( offset7 + 19 ) /* SECOND OPTION MAP */
#define   Entered_Dens_Base   ( offset7 + 20 ) /* Entered Base Density, kg/m3 */
#define   HVal_Inf            ( offset7 + 24 ) /* Heating value */
#define   Sup_EngRateDaily    ( offset7 + 28 ) /* Superior energy Flow */
#define   ScanPer             ( offset7 + 32 ) /* scan time */
#define   PipeDShop           ( offset7 + 34 ) /* Pipe Diameter */
#define   OrifDShop           ( offset7 + 38 ) /* Orifice diameter */
#define   OrifDMsTemp         ( offset7 + 42 ) /* Orifice measured temperature */
#define   OrifMatCode         ( offset7 + 46 ) /* Orifice material */
#define   MeterDescr		  ( offset7 + 47 ) /* point description */
#define   AlmCode			  ( offset7 + 77 ) /* Alarm code BIT MAP*/
#define   LowAlarmVal         ( offset7 + 78 ) /* low alarm */
#define   HiAlarmVal          ( offset7 + 82 ) /* high alarm */
#define   Viscosity           ( offset7 + 86 ) /* entered viscosity */
#define   IsentrExp           ( offset7 + 90 ) /* entered izentropic exponent */
#define   BasePress           ( offset7 + 94 ) /* contract pressure */
#define   BaseTemp            ( offset7 + 98 ) /* contract temperature */
#define   DPLowCutOff         ( offset7 + 102) /* DP low cutoff */
#define   Rd_OrifCorner       ( offset7 + 106) /* Average radius for Orifice corner */
#define   Nitrogen            ( offset7 + 110) /* mole fraction of N2 */
#define   CO2                 ( offset7 + 114) /* mole fraction of Co2 */
#define   H2S                 ( offset7 + 118) /* mole fraction of H2S */
#define   H2O                 ( offset7 + 122) /* mole fraction of H2O */
#define   He                  ( offset7 + 126) /* mole fraction of He */
#define   Methane             ( offset7 + 130) /* mole fraction of C1 */
#define   Ethane              ( offset7 + 134) /* mole fraction of C2 */
#define   Propane             ( offset7 + 138) /* mole fraction of C3 */
#define   NButane             ( offset7 + 142) /* mole fraction of NC4 */
#define   IButane             ( offset7 + 146) /* mole fraction of IC4 */
#define   NPentane            ( offset7 + 150) /* mole fraction of IC5 */
#define   IPentane            ( offset7 + 154) /* mole fraction of NC5 */
#define   Hexanes             ( offset7 + 158) /* mole fraction of C6 */
#define   Heptanes            ( offset7 + 162) /* mole fraction of C7 */
#define   Octanes             ( offset7 + 166) /* mole fraction of C8 */
#define   Nonanes             ( offset7 + 170) /* mole fraction of C9 */
#define   Decanes             ( offset7 + 174) /* mole fraction of C10 */
#define   Oxygen              ( offset7 + 178) /* mole fraction of O2 */
#define   CarbMono            ( offset7 + 182) /* mole fraction of CO */
#define   Hydrogen            ( offset7 + 186) /* mole fraction of H2 */
#define   NotUsedByte         ( offset7 + 190) /* Not used, read only */
#define   EnableStack         ( offset7 + 191) /* Enable stack */
#define   LowDPType           ( offset7 + 192) /* Low Dp address-point type */
#define   LowDPLogic          ( offset7 + 193) /* Low Dp address-logical nr.*/
#define   LowDPParam          ( offset7 + 194) /* Low Dp address-parameter */
#define   DPType              ( offset7 + 195) /* hw address-t */
#define   DPLogic             ( offset7 + 196) /* hw address-l */
#define   DPParam             ( offset7 + 197) /* hw address-p */
#define   P1Type		            ( offset7 + 198) /* pf-t  */
#define   P1Logic		           ( offset7 + 199) /* pf-l  */
#define   P1Param		           ( offset7 + 200) /* pf-ps */
#define   T1Type			           ( offset7 + 201) /* tf-t  */
#define   T1Logic			          ( offset7 + 202) /* tf-l */
#define   T1Param			          ( offset7 + 203) /* tf-p */
#define   LowSetpoint		       ( offset7 + 204) /* low dp setpoint */
#define   HiSetpoint		        ( offset7 + 208) /* high dp setpoint */
#define   DPReading			        ( offset7 + 212) /* diff press reading */
#define   P1Reading           ( offset7 + 216) /* pressure reading */
#define   T1Reading           ( offset7 + 220) /* temperature reading */
#define   FlowRateDaily		     ( offset7 + 224) /* flow */
#define   Dsp_EngRateDaily	   ( offset7 + 228) /* energy */
#define   VolToday			         ( offset7 + 232) /* accumulated flow since contract */
#define   Dsp_EngyToday		     ( offset7 + 236) /* accum energy since contract */
#define   VolYesday		         ( offset7 + 240) /* yesterdays prod */
#define   Dsp_EngyYesday	     ( offset7 + 244) /* yesterdays energy */
#define   MassFlowHr		        ( offset7 + 248) /* Mass flow rate  */
#define   DiffStaticProd      ( offset7 + 252) /* sqrt of diff times static */
#define   SampleTime          ( offset7 + 256) /* sample_time */
#define   ExpFactor           ( offset7 + 260) /* expansion factor - see Fisher Map*/
#define   Inf_EngRateDaily    ( offset7 + 264) /* Inferior energy Flow */
#define   NotDefined01        ( offset7 + 268) /*  */
#define   NotDefined02        ( offset7 + 272) /*  */
#define   Squared_N2          ( offset7 + 276) /*  */
#define   Squared_CO2         ( offset7 + 280) /*  */
#define   Prod_N2_CO2  	      ( offset7 + 284) /*  */
#define   NotDefined06	       ( offset7 + 288) /*  */
#define   NotDefined07		      ( offset7 + 292) /*  */

#define   NotDefined09_Inv	   ( offset7 + 296) /*  */
#define   NotDefined10_Inv	   ( offset7 + 300) /*  */

#define   FlowZ_FST   		      ( offset7 + 304) /* compressibility flow cond*/
#define   FlowDens_FST        ( offset7 + 308) /* real density, mol per l */
#define   AtmPrs_FST          ( offset7 + 312) /* atmospheric pressure */
/*=============================================================
    44 bytes hole, to be mapped later (316-360)
 *=============================================================*/
#define   ContrZ_FST          ( offset7 + 360) /* base compressibility */
#define   ContrDens_FST       ( offset7 + 364) /* base_density */
/*=============================================================
    18 bytes hole, to be mapped later (368-386)
 *=============================================================*/
#define   Ropk_Factor         ( offset7 + 386)
#define   Old_Ent_Dens        ( offset7 + 390)
#define   Rop_Factor          ( offset7 + 394)
#define   O_Factor            ( offset7 + 398)
#define   TempK_Squared       ( offset7 + 402)
#define   TempPsCritic        ( offset7 + 406)
#define   PressPsCritic       ( offset7 + 410)
#define   StaticOverTempK     ( offset7 + 414)

#define   Old_Method          ( offset7 + 418)
#define   No_Flow_Flag        ( offset7 + 419)
#define   Low_Flow_Flag       ( offset7 + 420)
#define   Hi_Flow_Flag        ( offset7 + 421)
#define   GOST_Alarm_Flag     ( offset7 + 422)
#define   Is_Flowing_Flag     ( offset7 + 423)
#define   Flow_Time_Accum_Y   ( offset7 + 424)   /*== Flow Time Ticks Yesday */ 
#define   Cubed_N2            ( offset7 + 426)
#define   Cubed_CO2           ( offset7 + 430)
#define   DP_Average          ( offset7 + 434)
#define   PF_Average          ( offset7 + 438)
#define   TF_Average          ( offset7 + 442)
#define   DP_Conv             ( offset7 + 446)
#define   PF_Conv             ( offset7 + 450)
#define   TF_Conv             ( offset7 + 454)
#define   DP_Accum            ( offset7 + 458)
#define   PF_Accum            ( offset7 + 462)
#define   TF_Accum            ( offset7 + 466)
#define   Comp_Changed_Flag   ( offset7 + 470)
#define   Stack_Status        ( offset7 + 471) 
#define   Expiry_Time         ( offset7 + 472) 
#define   Flow_Time_Accum     ( offset7 + 474) 
#define   Scan_Time_Accum     ( offset7 + 476) 
#define   FlowTimer           ( offset7 + 478) 
#define   RecalcDensityFlag	  ( offset7 + 480) /* recalc Z flag */
#define   CalcFlag			         ( offset7 + 481) /* calc_flag */

/****************************************************************************
 *				Convenient Definitions for Options 1
 ****************************************************************************/
/*#define Turbine        ((*GostOpt1 & 0x1B)==0x01)*/
/* xxx00x01 */

/*== bit 2 always set, it is AGA 1992->atm press ==*/

#define FlangeTaps     ((*GostOpt1 & 0x1A)==0x00)
/* xxx00x0x */ 
#define PipeTaps       ((*GostOpt1 & 0x1A)==0x08)
/* xxx01x0x */
#define CornerTaps     ((*GostOpt1 & 0x1A)==0x10)
/* xxx10x0x */
#define Annubar_10     ((*GostOpt1 & 0x1A)==0x18)
/* xxx11x0x */
#define Annubar_15     ((*GostOpt1 & 0x1A)==0x02)
/* xxx00x1x */
#define Annubar_25     ((*GostOpt1 & 0x1A)==0x0A)
/* xxx01x1x */
#define Annubar_35     ((*GostOpt1 & 0x1A)==0x12)
/* xxx10x1x */
#define Annubar_45     ((*GostOpt1 & 0x1A)==0x1A)
/* xxx11x1x */









#define DisableAlm         ( !( *GostOpt1 & 0x20))
#define EnableAlm          (    *GostOpt1 & 0x20 )
#define DisRBXonClear      ( !( *GostOpt1 & 0x40))
#define EnRBXonClear       (    *GostOpt1 & 0x40 )
#define DisRBXonSet        ( !( *GostOpt1 & 0x80))
#define EnRBXonSet         (    *GostOpt1 & 0x80 )
/****************************************************************************
 *				Convenient Definitions for Options 2
 ****************************************************************************/

#define DispInfHVal        ( !( *GostOpt2 & 0x01))
#define DispSupHVal        (    *GostOpt2 & 0x01 )
/*== bit 1 always set, it is SET SPGR ====*/
#define UpstrTaps          ( !( *GostOpt2 & 0x04))
#define DnstrTaps          (    *GostOpt2 & 0x04 )
#define AbsStatic          ( !( *GostOpt2 & 0x08))
#define GaugeStatic        (    *GostOpt2 & 0x08 )
/*== leave bit 4 (10) always set, it is SET HVAL ====*/
/*== leave bit 5 (20) always set, it is SET ACCEL ====*/

#define NX_19          ((!(*GostOpt2 & 0x40))&&(!( *GostOpt2 & 0x80)))
                                                /* xx00xxxx */
#define VNIC_SMV       (  (*GostOpt2 & 0x40)&&(!( *GostOpt2 & 0x80)))
                                                /* xx01xxxx */
#define GERG_91        ((!(*GostOpt2 & 0x40))&& ( *GostOpt2 & 0x80))
                                                /* xx10xxxx */
#define REILIH_K       (( (*GostOpt2 & 0x40))&& ( *GostOpt2 & 0x80))
                                                /* xx11xxxx */


/****************************************************************************
 *				Convenient Recast by Value
 ****************************************************************************/

#define   TAG7               ( *Tag7 ) /* point tag */
#define   ROUGHNESS          ( *(FLOAT *)Roughness)
#define   GOSTOPT1           ( *GostOpt1 )
#define   GOSTOPT2           ( *GostOpt2 )
#define   ENTERED_DENS_BASE  ( *(FLOAT *)Entered_Dens_Base)
#define   ALMCODE            ( *(UINT8 *)AlmCode)     /* Alarm code BIT MAP*/
#define   LOWALARMVAL        ( *(FLOAT *)LowAlarmVal) /* low alarm */
#define   HIALARMVAL         ( *(FLOAT *)HiAlarmVal)  /* high alarm */
#define   NO_FLOW_FLAG       ( *No_Flow_Flag)
#define   LOW_FLOW_FLAG      ( *Low_Flow_Flag)
#define   HI_FLOW_FLAG       ( *Hi_Flow_Flag)
#define   GOST_ALARM_FLAG    ( *GOST_Alarm_Flag)
#define   BASEPRESS          ( *(FLOAT *)BasePress)    /* contract pressure */
#define   BASETEMP           ( *(FLOAT *)BaseTemp)     /* contract temperature */
#define   OLD_ENT_DENS       ( *(FLOAT *)Old_Ent_Dens) /* prev entered density */
#define   SAMPLETIME         ( *(FLOAT *)SampleTime) 
#define   EXPIRY_TIME        ( *(UINT16 *)Expiry_Time) 
#define   FLOW_TIME_ACCUM    ( *(UINT16 *)Flow_Time_Accum) 
#define   SCAN_TIME_ACCUM    ( *(UINT16 *)Scan_Time_Accum) 
#define   FLOW_TIME_ACCUM_Y  ( *(UINT16 *)Flow_Time_Accum_Y)/* Flow Time Ticks Yesday */ 
#define   SCANPER            ( *(UINT16 *)ScanPer) 
#define   FLOWTIMER          ( *(UINT16 *)FlowTimer) 
#define   RECALCDENSITYFLAG  ( *RecalcDensityFlag)
#define   CALCFLAG			 ( *CalcFlag)
#define   IS_FLOWING_FLAG    ( *Is_Flowing_Flag)
#define   ENABLESTACK        ( *EnableStack)  /* Enable stack */
#define   LOWDPTYPE          ( *LowDPType)    /* Low Dp address-point type */
#define   LOWDPLOGIC         ( *LowDPLogic)   /* Low Dp address-logical nr.*/
#define   LOWDPPARAM         ( *LowDPParam)   /* Low Dp address-parameter */
#define   DPTYPE             ( *DPType)       /* hw address-t */
#define   DPLOGIC            ( *DPLogic)      /* hw address-l */
#define   DPPARAM            ( *DPParam)      /* hw address-p */
#define   P1TYPE             ( *P1Type)       /* pf-t  */
#define   P1LOGIC            ( *P1Logic)      /* pf-l  */
#define   P1PARAM            ( *P1Param)      /* pf-ps */
#define   T1TYPE             ( *T1Type)       /* tf-t  */
#define   T1LOGIC            ( *T1Logic)      /* tf-l */
#define   T1PARAM            ( *T1Param)      /* tf-p */
#define   LOWSETPOINT        ( *(FLOAT *)LowSetpoint) /* low dp setpoint */
#define   HISETPOINT         ( *(FLOAT *)HiSetpoint)  /* high dp setpoint */
#define   DPREADING          ( *(FLOAT *)DPReading)   /* diff press reading */
#define   P1READING          ( *(FLOAT *)P1Reading)   /* pressure reading */
#define   T1READING          ( *(FLOAT *)T1Reading)   /* temperature reading */
#define   STACK_STATUS       ( *Stack_Status)  /* Stack Status */
#define   DPLOWCUTOFF        ( *(FLOAT *)DPLowCutOff) /* DP low cutoff */
#define   DIFFSTATICPROD     ( *(FLOAT *)DiffStaticProd) /* sqrt of diff times static */
#define   DP_AVERAGE         ( *(FLOAT *)DP_Average)
#define   PF_AVERAGE         ( *(FLOAT *)PF_Average)
#define   TF_AVERAGE         ( *(FLOAT *)TF_Average)
#define   DP_CONV            ( *(FLOAT *)DP_Conv)
#define   PF_CONV            ( *(FLOAT *)PF_Conv)
#define   TF_CONV            ( *(FLOAT *)TF_Conv)
#define   DP_ACCUM           ( *(FLOAT *)DP_Accum)
#define   PF_ACCUM           ( *(FLOAT *)PF_Accum)
#define   TF_ACCUM           ( *(FLOAT *)TF_Accum)
#define   FLOWRATEDAILY      ( *(FLOAT *)FlowRateDaily)    /* flow */
#define   INF_ENGRATEDAILY   ( *(FLOAT *)Inf_EngRateDaily) /* inf energy */
#define   SUP_ENGRATEDAILY   ( *(FLOAT *)Sup_EngRateDaily) /* sup energy */
#define   DSP_ENGRATEDAILY   ( *(FLOAT *)Dsp_EngRateDaily) /* displayed energy */
#define   MASSFLOWHR         ( *(FLOAT *)MassFlowHr)       /* Mass flow rate */
#define   VOLTODAY           ( *(FLOAT *)VolToday)  /* accumulated flow since contract */
#define   DSP_ENGYTODAY      ( *(FLOAT *)Dsp_EngyToday) /* accum energy since contract */
#define   VOLYESDAY          ( *(FLOAT *)VolYesday) /* yesterdays prod */
#define   DSP_ENGYYESDAY     ( *(FLOAT *)Dsp_EngyYesday)/* yesterdays energy */
#define   METHANE            ( *(FLOAT *)Methane)  /* mole fraction methane */
#define   NITROGEN           ( *(FLOAT *)Nitrogen) /* mole fraction of N2 */
#define   CDIOX              ( *(FLOAT *)CO2)      /* mole fraction of Co2 */
#define   HSULFIDE           ( *(FLOAT *)H2S)      /* mole fraction of H2S */
#define   WATER              ( *(FLOAT *)H2O)      /* mole fraction of H2O */
#define   HELIUM             ( *(FLOAT *)He)       /* mole fraction of He */
#define   ETHANE             ( *(FLOAT *)Ethane)   /* mole fraction of C2 */
#define   PROPANE            ( *(FLOAT *)Propane)  /* mole fraction of C3 */
#define   NBUTANE            ( *(FLOAT *)NButane)  /* mole fraction of NC4 */
#define   IBUTANE            ( *(FLOAT *)IButane)  /* mole fraction of IC4 */
#define   NPENTANE           ( *(FLOAT *)NPentane) /* mole fraction of IC5 */
#define   IPENTANE           ( *(FLOAT *)IPentane) /* mole fraction of NC5 */
#define   HEXANES            ( *(FLOAT *)Hexanes)  /* mole fraction of C6 */
#define   HEPTANES           ( *(FLOAT *)Heptanes) /* mole fraction of C7 */
#define   OCTANES            ( *(FLOAT *)Octanes)  /* mole fraction of C8 */
#define   NONANES            ( *(FLOAT *)Nonanes)  /* mole fraction of C9 */
#define   DECANES            ( *(FLOAT *)Decanes)  /* mole fraction of C10 */
#define   OXYGEN             ( *(FLOAT *)Oxygen)   /* mole fraction of O2 */
#define   CARBMONO           ( *(FLOAT *)CarbMono) /* mole fraction of CO */
#define   HYDROGEN           ( *(FLOAT *)Hydrogen) /* mole fraction of H2 */
#define   SQUARED_N2         ( *(FLOAT *)Squared_N2)
#define   SQUARED_CO2        ( *(FLOAT *)Squared_N2)
#define   PROD_N2_CO2        ( *(FLOAT *)Prod_N2_CO2)
#define   CUBED_N2           ( *(FLOAT *)Cubed_N2)
#define   CUBED_CO2          ( *(FLOAT *)Cubed_CO2)
#define   ROPK_FACTOR        ( *(FLOAT *)Ropk_Factor)
#define   ROP_FACTOR         ( *(FLOAT *)Rop_Factor)
#define   O_FACTOR           ( *(FLOAT *)O_Factor)
#define   TEMPK_SQUARED      ( *(FLOAT *)TempK_Squared)
#define   TEMPPSCRITIC       ( *(FLOAT *)TempPsCritic)
#define   PRESSPSCRITIC      ( *(FLOAT *)PressPsCritic)
#define   STATICOVERTEMPK    ( *(FLOAT *)StaticOverTempK)
#define   OLD_METHOD         ( *Old_Method)
#define   HVAL_INF           ( *(FLOAT *)HVal_Inf) /*  */
#define   HVAL_SUP           ( *(FLOAT *)HVal_Sup) /*  */
#define   ISENTREXP          ( *(FLOAT *)IsentrExp) /*  */
#define   VISCOSITY          ( *(FLOAT *)Viscosity) /*  */
#define   COMP_CHANGED_FLAG  ( *Comp_Changed_Flag)
#define   PIPEDSHOP          ( *(FLOAT *)PipeDShop)    /* Pipe Diameter */
#define   ORIFDSHOP          ( *(FLOAT *)OrifDShop )   /* Orifice diameter */
#define   ORIFDMSTEMP        ( *(FLOAT *)OrifDMsTemp ) /* Orifice measured temperature */
#define   ORIFMATCODE        ( *OrifMatCode)           /* Orifice material */
#define   RD_ORIFCORNER      ( *(FLOAT *)Rd_OrifCorner)/* Average radius for Orifice corner */
#define   EXPFACTOR          ( *(FLOAT *)ExpFactor)    /* Expansion Factor */
#define   FLOWZ_FST          ( *(FLOAT *)FlowZ_FST)    /* compressibility flow cond*/
#define   FLOWDENS_FST       ( *(FLOAT *)FlowDens_FST) /* real density, mol per l */
#define   ATMPRS_FST         ( *(FLOAT *)AtmPrs_FST)   /* atmospheric pressure */
#define   CONTRZ_FST         ( *(FLOAT *)ContrZ_FST)   /* base compressibility */
#define   CONTRDENS_FST      ( *(FLOAT *)ContrDens_FST)/* base_density */

/****************************************************************************
 *
 *             Fisher Point Types 41-42 Address Maps
 *
 ****************************************************************************/

#define offset41  Table_start[41].address + flow_no * Table_start[41].size

#define	Tag41		   ( offset41 +  0 ) /* Tag                            0 */
#define	AtmPress	   ( offset41 + 10 ) /* Atmospheric Pressure          10 */
#define CalcMethod2    ( offset41 + 14 ) /* Calculation Method 2          14 */
#define	NotUsedTLP	   ( offset41 + 15 ) /* Spare                         15 */
#define	PipeDMsTemp	   ( offset41 + 18 ) /* Pipe Reference Temp           18 */
#define PipeMatCode    ( offset41 + 22 ) /* Pipe Material, carbon steel   22 */
#define	InspectionPer  ( offset41 + 23 ) /* Inspection period             23 */
#define	ExpFactor41    ( offset41 + 24 ) /* Expansion Factor              24 */
#define	CurrSpeed      ( offset41 + 28 ) /* Avg Velocity in pipe          32 */
#define SoundSpeed     ( offset41 + 32 ) /* Sound Speed in this gas       36 */
#define	FlowZ          ( offset41 + 36 ) /* Flow Compressibility          40 */
#define	ContractZ      ( offset41 + 40 ) /* Contract Compressibility      44 */
#define CoeffZ         ( offset41 + 44 ) /* Compressibility Coeff (X)     48 */
#define	FlowDensity    ( offset41 + 48 ) /* Real Density                  52 */
#define	BaseDensity    ( offset41 + 52 ) /* Base Density                  56 */
/*==========================================================
    History Assignment and Conversion Follows Here in Pt 41
	- We leave this for later on
 *=========================================================*/

#define	ATMPRESS       ( *(FLOAT *)AtmPress )   /* Atmospheric Pressure          10 */
#define	EXPFACTOR41    ( *(FLOAT *)ExpFactor41) /* Expansion Factor              24 */
#define CURRSPEED      ( *(FLOAT *)CurrSpeed )  /* Current Speed in the pipe     32 */
#define SOUNDSPEED     ( *(FLOAT *)SoundSpeed ) /* Sound Speed in this gas       36 */
#define	PIPEDMSTEMP    ( *(FLOAT *)PipeDMsTemp )/* Pipe Reference Temp           18 */
#define PIPEMATCODE    ( *PipeMatCode)          /* Pipe Material, carbon steel   22 */
#define	INSPECTIONPER  ( *InspectionPer)        /* Inspection period             23 */
#define	FLOWDENSITY    ( *(FLOAT *)FlowDensity) /* Real Density                  52 */
#define	BASEDENSITY    ( *(FLOAT *)BaseDensity) /* Base Density                  56 */
#define	FLOWZ          ( *(FLOAT *)FlowZ )      /* Flow Compressibility          40 */
#define	CONTRACTZ      ( *(FLOAT *)ContractZ )  /* Contract Compressibility      44 */
#define COEFFZ         ( *(FLOAT *)CoeffZ )     /* Compressibility Coeff (X)     48 */

/****************************************************************************
 *				Convenient Recast by Value
 ****************************************************************************/

#define CALCMETHOD2             ( *(UINT8 *)CalcMethod2)

#define CalcMethod2    ( offset41 + 14 ) /* Calculation Method 2          14 */
#define offset42  Table_start[42].address + flow_no * Table_start[42].size
									    /* Relative address */
#define Tag42		   ( offset42 +  0)	/* Tag                                 0 */
#define	FlwToday       ( offset42 + 10)	/* Accumulated Flow for Today         10 */
#define FlwYday	       ( offset42 + 14)	/* Accumulated Flow for Yesterday     14 */
#define	FlwMonth	   ( offset42 + 18)	/* Accumulated Flow for the Month     18 */
#define	FlwLastm	   ( offset42 + 22) /* Accumulated Flow for the Prev Month22 */
#define FlwThisYear    ( offset42 + 26) /* Accumulated Flow this year         26 */
#define	FlwLastYear	   ( offset42 + 30)	/* Accumulated Flow last year         30 */
#define	MinFlwToday    ( offset42 + 34)	/* Accumulated Minutes of Flow Today  34 */
#define	MinFlwYday	   ( offset42 + 38)	/* Accumulated Minutes of Flow Yesday 38 */
#define	I_EngToday	   ( offset42 + 42)	/* Accumulated Inf Energy Today       42 */
#define	I_EngYday	   ( offset42 + 46)	/* Accumulated Inf Energy Yesday      46 */
#define	I_EngMonth     ( offset42 + 50)	/* Accumulated Inf Energy This Month  50 */
#define	I_EngLastm	   ( offset42 + 54)	/* Accumulated Inf Energy Last Month  54 */
#define	I_EngThisYear  ( offset42 + 58)	/* Accumulated Inf Energy This Year   58 */
#define	I_EngLastYear  ( offset42 + 62)	/* Accumulated Inf Energy Last Year   62 */
#define	S_EngToday     ( offset42 + 66)	/* Accumulated Sup Energy Today       66 */
#define	S_EngYday      ( offset42 + 70)	/* Accumulated Sup Energy Yesday      70 */
#define	S_EngMonth 	   ( offset42 + 74)	/* Accumulated Sup Energy This Month  74 */
#define	S_EngLastm	   ( offset42 + 78)	/* Accumulated Sup Energy Last Month  78 */
#define	S_EngThisYear  ( offset42 + 82)	/* Accumulated Sup Energy This Year   82 */
#define	S_EngLastYear  ( offset42 + 86)	/* Accumulated Sup Energy Last Year   86 */
#define	OrifD_T 	   ( offset42 + 90) /* Orifice Plate Bore Diameter        90 */
#define	PipeD_T	       ( offset42 + 94) /* Meter Tube Internal Diameter       94 */
#define	Beta		   ( offset42 + 98)	/* Diameter Ratio                     98 */
#define	VelAppr 	   ( offset42 +102)	/* Velocity of Approach Factor       102 */
#define	CoeffDisch	   ( offset42 +106)	/* Coefficient of Discharge          106 */
#define	ReNumber	   ( offset42 +110)	/* Reynolds Number                   110 */
#define	UpstrAbsPrs    ( offset42 +114)	/* Upstream Static Pressure          114 */
#define	MolWeight	   ( offset42 +118)	/* Molecular Weight                  118 */


#define	UPSTRABSPRS    ( *(FLOAT *)UpstrAbsPrs)	/* Upstream Static Pressure               114 */
#define	MOLWEIGHT      ( *(FLOAT *)MolWeight)	/* Molecular Weight                       118 */
#define	BETA           ( *(FLOAT *)Beta)	    /* Diameter Ratio                          98 */
#define	VELAPPR        ( *(FLOAT *)VelAppr)	    /* Velocity of Approach Factor            102 */
#define	COEFFDISCH     ( *(FLOAT *)CoeffDisch)	/* Coefficient of Discharge               106 */
#define	RENUMBER       ( *(FLOAT *)ReNumber)	/* Reynolds Number                        110 */
#define	ORIFD_T        ( *(FLOAT *)OrifD_T)     /* Orifice Plate Bore Diameter             90 */
#define	PIPED_T        ( *(FLOAT *)PipeD_T)     /* Meter Tube Internal Diameter            94 */

#define	FLWTODAY       ( *(FLOAT *)FlwToday)    /* Accumulated Flow for Today         10 */
#define FLWYDAY        ( *(FLOAT *)FlwYday)	    /* Accumulated Flow for Yesterday     14 */
#define	FLWMONTH       ( *(FLOAT *)FlwMonth)    /* Accumulated Flow for the Month     18 */
#define	FLWLASTM       ( *(FLOAT *)FlwLastm)    /* Accumulated Flow for the Prev Month22 */
#define FLWTHISYEAR    ( *(FLOAT *)FlwThisYear) /* Accumulated Flow this year         26 */
#define	FLWLASTYEAR    ( *(FLOAT *)FlwLastYear)	/* Accumulated Flow last year         30 */
#define	MINFLWTODAY    ( *(FLOAT *)MinFlwToday)	/* Accumulated Minutes of Flow Today  34 */
#define	MINFLWYDAY     ( *(FLOAT *)MinFlwYday)	/* Accumulated Minutes of Flow Yesday 38 */
#define	I_ENGTODAY     ( *(FLOAT *)I_EngToday)	/* Accumulated Inf Energy Today       42 */
#define	I_ENGYDAY      ( *(FLOAT *)I_EngYday)	/* Accumulated Inf Energy Yesday      46 */
#define	I_ENGMONTH     ( *(FLOAT *)I_EngMonth)	/* Accumulated Inf Energy This Month  50 */
#define	I_ENGLASTM     ( *(FLOAT *)I_EngLastm)	/* Accumulated Inf Energy Last Month  54 */
#define	I_ENGTHISYEAR  ( *(FLOAT *)I_EngThisYear)/* Accumulated Inf Energy This Year   58 */
#define	I_ENGLASTYEAR  ( *(FLOAT *)I_EngLastYear)/* Accumulated Inf Energy Last Year   62 */
#define	S_ENGTODAY     ( *(FLOAT *)S_EngToday)	/* Accumulated Sup Energy Today       66 */
#define	S_ENGYDAY      ( *(FLOAT *)S_EngYday)	/* Accumulated Sup Energy Yesday      70 */
#define	S_ENGMONTH     ( *(FLOAT *)S_EngMonth)	/* Accumulated Sup Energy This Month  74 */
#define	S_ENGLASTM     ( *(FLOAT *)S_EngLastm)	/* Accumulated Sup Energy Last Month  78 */
#define	S_ENGTHISYEAR  ( *(FLOAT *)S_EngThisYear)/* Accumulated Sup Energy This Year   82 */
#define	S_ENGLASTYEAR  ( *(FLOAT *)S_EngLastYear)/* Accumulated Sup Energy Last Year   86 */
 
/*****************************************************************************
 *          User Defined Macros and Process Variables
 *****************************************************************************/

UINT8 Max_Flow_No = 0;		/* Maximum Liquid Flows Defined at start up */


#endif  