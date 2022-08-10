/****************************************************************************/
/* NG_AN_MN.C      GOST Program Main File                                    */
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
/*  Ver 3.01 Feb 2001(NG + Annubar 25/26)                                   */
/*  Ver 3.02 , (Natural Gas+ Orifice/Annubar/Turbine) March 2001 Moscow     */
/****************************************************************************/
#define  GLOBALS_DEFINED

#include <math.h>         /*== include IDE libs first ==*/
#include <string.h>
#include <Dtypes.h>       /*== data type defs next  ==*/

#include <fb_glob.h>
#include <fblib.h>    

#include "gost_glb.h"     /*= at the end include project headers */
#include "gost_hd.h"
#include "GOSTVals.h"


/****************************************************************************/
/*      SYNTAX: main ()                                                     */
/* DESCRIPTION: GOST Program main control                                   */
/* CALLING SEQ: main ();                                                    */
/*   ARGUMENTS: NAME        TYPE      DESCRIPTION                           */
/*              ---------   -------   ------------------------------------  */
/*              arg         None      no arguments                          */
/*     RETURNS: None                                                        */
/*       NOTES: None                                                        */
/****************************************************************************/

main ()
{
  #asm
  push  ds
  push  es
  mov   ax, seg first_time_
  mov   ds,ax
  #endasm

  if (first_time)
  {
	   initialize  ();
    first_time = 0;
  }
  gost_schedule();

  #asm
  pop   es
  pop   ds
  #endasm

} /* end main () */

/****************************************************************************/
/*      SYNTAX: void AGACleanUp (void)                                      */
/* DESCRIPTION: Cleans Up AGA registers for 1 flow                          */
/* CALLING SEQ: initialize ();                                              */
/*   ARGUMENTS: NAME        TYPE      DESCRIPTION                           */
/*              ---------   -------   ------------------------------------  */
/*              arg         None      no arguments                          */
/*     RETURNS: None                                                        */
/*       NOTES: None                                                        */
/****************************************************************************/
void AGACleanUp(void)
{
  GOSTOPT1    |= 0x04;       /* Set AGA extended */
  GOSTOPT2    |= 0x02;       /* Set Enter Sp Gravity */
  GOSTOPT2    |= 0x10;       /* Set Enter HVal  */
  GOSTOPT2    |= 0x20;       /* Set Enter Acceleration */
  CALCMETHOD2 |= 0x04;       /* Set Enter atm pressure */
  /*== DO NOT CHANGE ANYTHING HERE - DANGEROUS !! ====*/
  memset(Ropk_Factor,0,96);  /* Clean Up AGA registers */ 
  memset(FlowRateDaily,0,16);
  memset(ExpFactor41,0,32);
  memset(OrifD_T,0,32);
  if ( FULL_INIT )           /* clear protected values at cold start */
      memset(&PFlow[flow_no],0,80);  
  else
      DisplayFlow ( );       /*== restore acc flow values ==*/

}
/****************************************************************************/
/*      SYNTAX: void initialize (void)                                      */
/* DESCRIPTION: Initializes any required globals. Identifies the ROC        */
/* CALLING SEQ: initialize ();                                              */
/*   ARGUMENTS: NAME        TYPE      DESCRIPTION                           */
/*              ---------   -------   ------------------------------------  */
/*              arg         None      no arguments                          */
/*     RETURNS: None                                                        */
/*       NOTES: None                                                        */
/****************************************************************************/

void  initialize (void)
{
  UINT8 *chptr;
  UINT8	index, stopAGA;
  FLOAT zero_elapsed;
    
  chptr = (UINT8 *) &NUorg;

  /* get segment offset for beginning of float scratch area and */
  /* initialize offsets to primary and secondary float scratch areas */
  #asm
  EXTRN flterr_:word
  EXTRN flprm:word
  EXTRN flsec:word
  mov   ax, offset flterr_
  add   ax, 20
  mov   flprm, ax
  add   ax, 14
  mov   flsec, ax
  #endasm

  get_config      ();
  get_pt_type_pos ();
	
	 /* get number of flows */
	 get_param (SYS_TYPE, 0, 4,&FlowsRunning);

	 /* disable first AGA !! */
	 stopAGA = (UINT8)NO_FLOW;
	 set_param (TASK_TYPE, 3/*AGA_TASK*/, 3,&stopAGA);    

  /* get pointer to CRC table in ROC firmware */
  Crc_table = Config->crc_table; 
  
		/* fill up VNIC tables, make it a function */
	
	 FillUpVNICTables();

	 /* get tick counter */
	 OneScanTick     = system_ticks();
	 OneSecTickAccum = 0;
	
	 /* initialize calendar */
	 PreviousMonth = Now_Month;
	 PreviousYear  = Now_Year;
	 PreviousDay   = Now_Day;
	 PreviousHour  = Now_Hour;
	 /* don't trigger a day transfer the first scan */


	 for (flow_no=0; flow_no<ACTIVE_GOSTS;flow_no++)
			{
		  AGACleanUp(); /* Clean Up AGA registers */
		  FLOWTIMER    = OneScanTick + SCANPER * 5;
    FLOW_TIME_ACCUM_Y = 0;
			}

	 flow_no = 0;  /*= pass control to first flow =*/
    
} /* end of initialize () */

/****************************************************************************/
/*                                                                          */
/*      SYNTAX: void FillUpVNICTables (void)                                */
/* DESCRIPTION: initializes vnic volatile values                            */
/* CALLING SEQ: FillUpVNICTables ();                                        */
/*   ARGUMENTS: NAME        TYPE      DESCRIPTION                           */
/*              ---------   -------   -----------------------------------   */
/*     RETURNS: None                                                        */
/*                                                                          */
/*       NOTES: This function controls the real time schedule for gas       */
/*              metering calculations.                                      */
/*                                                                          */
/****************************************************************************/

void FillUpVNICTables (void)
{
  FLOAT tempr1, tempr2;
  FLOAT *pt1, *pt2, *pt3;  
  UINT8 i,j;

	 OneThird = 0.3333333;
    
	 for(i = 0; i < 8; i++)
			{
     tempr1 = GOST_Mi[i]/Roki[i];
		   pt1    = &Vkij[i][0];
		   pt2    = &Oij[i][0];
		   pt3    = &Tkij_2[i][0];
		   for(j = 0; j < 8; j++, pt1++,pt2++,pt3++)
      {
 	      tempr2 = GOST_Mi[j]/Roki[j];

        /*Vkij[i][j]*/
		      *pt1 = (1-Lij[i][j])*pow( (pow(tempr1,OneThird)+pow(tempr2,OneThird)) /2.0,3); /*  (65)*/
        *pt2 = (Oi[i]*tempr1+Oi[j]*tempr2)/(tempr1 + tempr2);         /*  (70)*/
        *pt3 = (1-Xij[i][j])*(1-Xij[i][j])*(Tki[i]*Tki[j]);  /*  (68)*/
      }  
			}
}

/****************************************************************************/
/*                                                                          */
/*      SYNTAX: void CheckDateAndTime (void)                                */
/* DESCRIPTION: Checks if there is a new contract day, month, or year       */
/* CALLING SEQ: gost_schedule ();                                           */
/*   ARGUMENTS: NAME        TYPE      DESCRIPTION                           */
/*              ---------   -------   -----------------------------------   */
/*     RETURNS: None                                                        */
/*                                                                          */
/*       NOTES: This function sets the flags for new hour, day, and year    */
/*              transfers                                                   */
/*                                                                          */
/****************************************************************************/
void CheckDateAndTime(void)
{
	UINT8 ind;
	
	if( Now_Hour != PreviousHour )
	 {
   for(ind=0; ind< FlowsRunning; ++ind)
		   ContrTransferFlag[ind] |= 0x01;      /*== set new Hour bit ==*/
   
			PreviousHour = Now_Hour;
		 
			if(Now_Hour == CONTRACT_HOUR)
				{
			   for(ind=0; ind< FlowsRunning; ++ind)
		       ContrTransferFlag[ind] |= 0x02;  /*== set new Contract day bit 1 ==*/
			   
						if( Now_Month != PreviousMonth)
							{
				     for(ind=0; ind< FlowsRunning; ++ind)
				        ContrTransferFlag[ind] |= 0x04;     /*== set new Month bit 2 ==*/
         
									PreviousMonth = Now_Month;

				     if (Now_Year != PreviousYear)
										{
					       for(ind=0; ind< FlowsRunning; ++ind)
				          ContrTransferFlag[ind] |= 0x08; /*== set new Year bit 3 ==*/
            PreviousYear = Now_Year;
										}
						 }
			 }
	 }
}
/****************************************************************************/
/*                                                                          */
/*      SYNTAX: void gost_schedule (void)                                   */
/* DESCRIPTION: Controls the overall operation of the user program.         */
/* CALLING SEQ: gost_schedule ();                                           */
/*   ARGUMENTS: NAME        TYPE      DESCRIPTION                           */
/*              ---------   -------   -----------------------------------   */
/*     RETURNS: None                                                        */
/*                                                                          */
/*       NOTES: This function controls the real time schedule for gas       */
/*              metering calculations.                                      */
/*                                                                          */
/****************************************************************************/

void gost_schedule (void)
{
 UINT8 ind, res;
 UINT16 elTicks, currTick;
 FLOAT locTempP, locPressP;
 
/* if( ACTIVE_GOSTS > AGA_MAX )     checked out by Roclink
           ACTIVE_GOSTS = AGA_MAX;  de-Comment if using gv101  */
 
/* initialize new flows if no flows changed */
   if ( ACTIVE_GOSTS > FlowsRunning )
   {
     ind = flow_no;
	    for (flow_no = FlowsRunning; flow_no < ACTIVE_GOSTS; ++flow_no)
						{
        AGACleanUp();
		      FLOWTIMER    = system_ticks() + SCANPER * 5;
		      FLOW_TIME_ACCUM_Y = 0;
						}
	    FlowsRunning = ACTIVE_GOSTS;
	    flow_no = ind;
   }  /*  flow no has changed */

   /* every system scan update ticks in a second */
   currTick         = system_ticks();
   elTicks          = currTick-OneScanTick;
   OneScanTick      = currTick;
   OneSecTickAccum += elTicks;
   
   if(OneSecTickAccum >= 100)
   {
      /*=== check date and time ==*/
      CheckDateAndTime();
      /*=== save current index ===*/
      ind = flow_no;
      for (flow_no = 0; flow_no < FlowsRunning; ++flow_no)
							{
         /*=== set calculation flags ===*/
	        res = IsTimerExpired(FLOWTIMER,currTick);
	        if(res)
										{
			         EXPIRY_TIME = currTick;
		          CALCFLAG = 1;
										}
 	        /*=== accumulate all flow inputs ===*/
		        ReadInputs(OneSecTickAccum);
   		     /*=== update scan time ==*/
		        SCAN_TIME_ACCUM += OneSecTickAccum;
							}
      /*=== restore the index ==*/
	     flow_no = ind;
	     OneSecTickAccum  = 0;
    }

   switch(GlobalCalcStatus)
				{
	    case IDDLE:
		     GlobalCalcStatus = PROCESSING;

		     /*=== process the flow flow_no ==*/
		   if ( CALCFLAG )
						{
		      /*=== average inputs at sacan time ==*/
        AverageInputs();
		      /*=== check composition ==*/
		      COMP_CHANGED_FLAG = CheckCompAndBaseDens();
		      locPressP = PF_AVERAGE / PRESSPSCRITIC;
        		locTempP  = TF_AVERAGE / TEMPPSCRITIC;

		      /*=== call appropiate compressibility equation ==*/
		      if (OLD_METHOD == PREV_VNIC)
			      VNIC_SMVRoutine(locPressP, locTempP);			        					
		      else 
				{
				if (OLD_METHOD == PREV_GERG)
				        GERG_91Routine(locPressP, locTempP);

				else if (OLD_METHOD == PREV_NX19)
			      	  NX_19Routine(locPressP, locTempP);

				else 
					  Reilih_Kvong_Routine(locPressP, locTempP); 
				RunTimeViscGRNX(locPressP, locTempP);
				}

		      /*=== calculate flow rate ===*/
		      CalculateFlowRate();
		      /*=== check alarms ===*/
		      if(EnableAlm)
		       CheckGOSTAlarms();
		      else
									{
			        if(ALMCODE)
												{
				          ALMCODE = 0;
 			          memset(No_Flow_Flag,ALM_CLEAR,4);
												}
									}
		      
								SAMPLETIME = ((FLOAT)SCAN_TIME_ACCUM)/100.0;
		      /*=== integrate =============*/
		      AccVolAndEnergy();

		      FLOWTIMER  = EXPIRY_TIME + SCANPER * 5;
		      SCAN_TIME_ACCUM = 0;
		   
		      CALCFLAG = 0;
						}
		
 		  flow_no++;
		   if (flow_no >= FlowsRunning)
			    flow_no = 0;

		   GlobalCalcStatus = IDDLE;

		  break;
	
			  case PROCESSING:
		  break;
			 }

} /* end of gost_schedule() */

/******************************************************************************/
/*                                                                            */
/* SYNTAX:      void DisplayFlow ( void )                                     */
/*                                                                            */
/* DESCRIPTION: utility, displays point 7 flow registers                      */
/*                                                                            */
/* ARGUMENTS:   NAME       TYPE   DESCRIPTION                                 */
/*              __________ ______ __________________________________________  */
/*                                                                            */
/* RETURNS:     Nothing                                                       */
/*                                                                            */
/* NOTES:       This function will run when a scan period completes, and      */                               
/*              after a new flow Rate is calculated                           */
/******************************************************************************/
void DisplayFlow ( void )
{
/*============*
float  x;
UINT32 y;
 *============*/
    VOLTODAY       = PFlow[flow_no].AccVolToday;
	   VOLYESDAY      = PFlow[flow_no].AccVolYday;
	   if( DispInfHVal )
					{
      DSP_ENGYTODAY  = PFlow[flow_no].AccI_EngToday;
	     DSP_ENGYYESDAY = PFlow[flow_no].AccI_EngYday;
					}
	   else
					{
      DSP_ENGYTODAY  = PFlow[flow_no].AccS_EngToday;
	     DSP_ENGYYESDAY = PFlow[flow_no].AccS_EngYday;
					}

    memcpy(FlwToday, &PFlow[flow_no], 80);
/*=================*
	y=(UINT32)(void *)&PFlow[flow_no];
	x=(FLOAT)y;

	set_param(17,flow_no, 3, (UINT8 *)&x);
 *=================*/
}
/******************************************************************************/
/*                                                                            */
/* SYNTAX:      void UnitAccVolAndEnergy (FLOAT volIncr, FLOAT, i_engIncr,    */
/*                                        FLOAT s_engIncr )                   */
/*                                                                            */
/* DESCRIPTION: The function Accumulates Unit Volumes and Energy              */
/*              scan period                                                   */
/*                                                                            */
/* ARGUMENTS:   NAME       TYPE   DESCRIPTION                                 */
/*              __________ ______ __________________________________________  */
/*                                                                            */
/* RETURNS:     Nothing                                                       */
/*                                                                            */
/* NOTES:       This function will run when a scan period completes, and      */                               
/*              after a new flow Rate is calculated                           */
/******************************************************************************/
void UnitAccVolAndEnergy (FLOAT volIncr, FLOAT i_engIncr,
						  FLOAT s_engIncr, FLOAT flwSecs)
{
		 PFlow[flow_no].AccVolToday     += volIncr;
		 PFlow[flow_no].AccVolMonth     += volIncr;
		 PFlow[flow_no].AccVolThisYear  += volIncr;
   PFlow[flow_no].AccI_EngToday   += i_engIncr;
   PFlow[flow_no].AccI_EngMonth   += i_engIncr;
   PFlow[flow_no].AccI_EngThisYear+= i_engIncr;
   PFlow[flow_no].AccS_EngToday   += s_engIncr;
   PFlow[flow_no].AccS_EngMonth   += s_engIncr;
   PFlow[flow_no].AccS_EngThisYear+= s_engIncr;
   PFlow[flow_no].AccMinFlwToday  += flwSecs/60.0;
}
/******************************************************************************/
/*                                                                            */
/* SYNTAX:      void AccVolAndEnergy ( void )                                 */
/*                                                                            */
/* DESCRIPTION: The function Accumulates Volumes and Energy for the previous  */
/*              scan period                                                   */
/*                                                                            */
/* ARGUMENTS:   NAME       TYPE   DESCRIPTION                                 */
/*              __________ ______ __________________________________________  */
/*                                                                            */
/* RETURNS:     Nothing                                                       */
/*                                                                            */
/* NOTES:       This function will run when a scan period completes, and      */                               
/*              after a new flow Rate is calculated                           */
/******************************************************************************/
void AccVolAndEnergy (void)
{
	 FLOAT volIncremTday, IengIncremTday, SengIncremTday; 
	 FLOAT volIncremYday, IengIncremYday, SengIncremYday;
	 FLOAT flowTimeTotal, flowTimeYday, locFltBuff;
   
  flowTimeTotal   = ((FLOAT)FLOW_TIME_ACCUM)/100.0;
  FLOW_TIME_ACCUM = 0;	
    
	 volIncremTday  = (FLOWRATEDAILY * flowTimeTotal) / 86400.0;
	 IengIncremTday = (INF_ENGRATEDAILY * flowTimeTotal) / 86400.0;
	 SengIncremTday = (SUP_ENGRATEDAILY * flowTimeTotal) / 86400.0;

	 /*== a new hour has arrived ==*/
	 if( ContrTransferFlag[flow_no] & 0x01 )  /*== new Hour =*/
			{
      ContrTransferFlag[flow_no] &= 0xFE;  /*== clear bit 0 =*/
			}

	 /*== a new contract day has arrived ==*/
	 if( ContrTransferFlag[flow_no] & 0x02 )   /*== new contract day =*/
			{
     ContrTransferFlag[flow_no] &= 0xFD;   /*== clear bit 1 =*/
     flowTimeYday  = ((FLOAT)FLOW_TIME_ACCUM_Y)/100.0; 
     FLOW_TIME_ACCUM_Y = 0;

		   locFltBuff      = flowTimeYday / flowTimeTotal;
		   volIncremYday   = volIncremTday * locFltBuff;
     volIncremTday  -= volIncremYday;
		   IengIncremYday  = IengIncremTday * locFltBuff;
     IengIncremTday -= IengIncremYday;
		   SengIncremYday  = SengIncremTday * locFltBuff;
     SengIncremTday -= SengIncremYday;
		   flowTimeTotal  -= flowTimeYday;
	
		   UnitAccVolAndEnergy(volIncremYday,IengIncremYday,
			                 SengIncremYday, flowTimeYday);
     /*== Transfer to Yesterday ==*/
		   PFlow[flow_no].AccVolYday    = PFlow[flow_no].AccVolToday;
     PFlow[flow_no].AccI_EngYday  = PFlow[flow_no].AccI_EngToday;
     PFlow[flow_no].AccS_EngYday  = PFlow[flow_no].AccS_EngToday;
     PFlow[flow_no].AccMinFlwYday = PFlow[flow_no].AccMinFlwToday;
		   /*== Clear Today Registers ==*/
	    PFlow[flow_no].AccVolToday    = 0.0;
		   PFlow[flow_no].AccI_EngToday  = 0.0;
		   PFlow[flow_no].AccS_EngToday  = 0.0;
	   	PFlow[flow_no].AccMinFlwToday = 0.0;
			}
	 /*== a new month has arrived ==*/
	 if( ContrTransferFlag[flow_no] & 0x04 )  /*== new Month =*/
			{
     ContrTransferFlag[flow_no] &= 0xFB;  /*== clear bit 2 =*/
     /*== Transfer to Last Month ==*/
		   PFlow[flow_no].AccVolLastm    = PFlow[flow_no].AccVolMonth;
     PFlow[flow_no].AccI_EngLastm  = PFlow[flow_no].AccI_EngMonth;
     PFlow[flow_no].AccS_EngLastm  = PFlow[flow_no].AccS_EngMonth;
		   /*== Clear This Month Registers ==*/
	    PFlow[flow_no].AccVolMonth    = 0.0;
		   PFlow[flow_no].AccI_EngMonth  = 0.0;
	    PFlow[flow_no].AccS_EngMonth  = 0.0;
			}
	 /*== a new year has arrived ==*/

	 if( ContrTransferFlag[flow_no] & 0x08 ) /*== new Year =*/
			{
     ContrTransferFlag[flow_no] &= 0xF7;   /*== clear bit 3 =*/
	
     /*== Transfer to Last Year ==*/
		   PFlow[flow_no].AccVolLastYear   = PFlow[flow_no].AccVolThisYear;
     PFlow[flow_no].AccI_EngLastYear = PFlow[flow_no].AccI_EngThisYear;
     PFlow[flow_no].AccS_EngLastYear = PFlow[flow_no].AccS_EngThisYear;
		   /*== Clear This Year Registers ==*/
	    PFlow[flow_no].AccVolThisYear    = 0.0;
		   PFlow[flow_no].AccI_EngThisYear  = 0.0;
	    PFlow[flow_no].AccS_EngThisYear  = 0.0;
			}

	 UnitAccVolAndEnergy(volIncremTday,IengIncremTday,
		                 SengIncremTday, flowTimeTotal);
  
  /*== hard copy protected registers into display
	      NOTE: it might flicker!!         ==========*/

	 DisplayFlow ();

}
/******************************************************************************/
/*                                                                            */
/* SYNTAX:      void CalculateFlowRate ( void )                               */
/*                                                                            */
/* DESCRIPTION: The function Calculates the Flow Rate during previous         */
/*              scan period                                                   */
/*                                                                            */
/* ARGUMENTS:   NAME       TYPE   DESCRIPTION                                 */
/*              __________ ______ __________________________________________  */
/*                                                                            */
/* RETURNS:     Nothing                                                       */
/*                                                                            */
/* NOTES:                                                                     */                               
/*                                                                            */
/******************************************************************************/

void CalculateFlowRate (void)
{
  FLOAT locRe, Cib, avgRadius, musor, Kp, tempC;
  FLOAT Ypi, Yor, Beta4, r0, Qcb, Qc, Qmb, Qm, B, Are, Ksh, Kre;
  UINT8 j,cond_Ksh;
  FLOAT L1 = 0.0;
  FLOAT L2 = 0.0;




  tempC = TF_AVERAGE - 273.15; 
     /* (B.1)..(B.5) GOST 8.563.1-97 */

  Ypi=ST[(UINT8)PIPEMATCODE].A + 
		  tempC * ST[(UINT8)PIPEMATCODE].B - 
		    tempC * tempC * ST[(UINT8)PIPEMATCODE].C;

  Yor=ST[(UINT8)ORIFMATCODE].A + 
		 tempC * ST[(UINT8)ORIFMATCODE].B-
		   tempC * tempC * ST[(UINT8)ORIFMATCODE].C;


  if (Annubar_10||Annubar_15||Annubar_25||Annubar_35||Annubar_45)
	{
	Yor=14.51e-6;
	if (Annubar_10) Ypi=14.51e-6;
	if ( (Annubar_10)&&(fabs(ORIFDSHOP- 4.3942)>1) ) ORIFDSHOP =  4.3942;
	if ( (Annubar_15)&&(fabs(ORIFDSHOP- 9.2710)>2) ) ORIFDSHOP =  9.2710;
	if ( (Annubar_25)&&(fabs(ORIFDSHOP-21.7424)>3) ) ORIFDSHOP = 21.7424;
	if ( (Annubar_35)&&(fabs(ORIFDSHOP-14.9860)>3) ) ORIFDSHOP = 14.9860;
	if ( (Annubar_45)&&(fabs(ORIFDSHOP-26.9240)>4) ) ORIFDSHOP = 26.9240;
	};
    
  PIPED_T = PIPEDSHOP * (1+Ypi*(tempC-20.0))/ (1+Ypi*(PIPEDMSTEMP - 20.0));
  ORIFD_T = ORIFDSHOP * (1+Yor*(tempC-20.0))/ (1+Yor*(ORIFDMSTEMP - 20.0));

 /*=== Stop here if no flow was occuring last scan period ==*/
	 
	 if (IS_FLOWING_FLAG == NO_FLOW)
	  {
     VELAPPR          = 1.0;
		   EXPFACTOR        = 1.0;
		   FLOWRATEDAILY    = 0.0; 
     INF_ENGRATEDAILY = 0.0;
     SUP_ENGRATEDAILY = 0.0;
     DSP_ENGRATEDAILY = 0.0;
     COEFFDISCH       = 0.0;
	    MASSFLOWHR       = 0.0;
	    RENUMBER         = 0.0;
	    CURRSPEED        = 0.0;
		   return;
	  }

if (Annubar_10||Annubar_15||Annubar_25||Annubar_35||Annubar_45)
{     
	B =(4.0*ORIFD_T) / (3.1415927*PIPED_T);
	
	if (Annubar_10) 								/*Annubar koeffitsient for 10*/
		{ROUGHNESS = -0.8212*B+0.7269;};    				
	
	if (Annubar_15)								/*Annubar koeffitsient for 15/16*/
		{musor = (1.0-0.92*B); ROUGHNESS = musor / sqrt(1.0+1.3452*musor*musor);}; 
	
	if (Annubar_25)								/*Annubar koeffitsient for 25/26*/
		{musor = (1.0-1.265*B); ROUGHNESS = musor / sqrt(1.0+1.43*musor*musor);}; 
	
	if (Annubar_35)								/*Annubar koeffitsient for 35/36*/
		{musor = (1.0-1.4229*B); ROUGHNESS = musor / sqrt(1.0+1.515*musor*musor);}; 
	
	if (Annubar_45)								/*Annubar koeffitsient for 45/46*/
		{musor = (1.0-1.4179*B); ROUGHNESS = musor / sqrt(1.0+1.492*musor*musor);}; 
	

	EXPFACTOR = 1.0 -((1.0 - B)*(1.0 - B)*0.31424-0.09484)*DP_AVERAGE/
		           (ISENTREXP * PF_AVERAGE * 1000.0);  		/* Algoritm (22) */

		
	
      Qm = 0.12645*ROUGHNESS*EXPFACTOR*PIPED_T*PIPED_T*sqrt(DP_AVERAGE*FLOWDENSITY);  /* Algoritm (23) */

};

if (FlangeTaps||PipeTaps||CornerTaps)
{/*Orifice*/

 	BETA  = (ORIFD_T / PIPED_T); Beta4 = BETA * BETA * BETA * BETA;


 	if( FlangeTaps ) /*   Taps pipe or flange */
	  { L1 = 25.4/PIPED_T;  L2 = 25.4/PIPED_T; };
	 
	if( PipeTaps ) /*== pipe taps D&D/2? ==*/
	  { L1 = 1.0;  L2 = 0.47;};
	
	if( CornerTaps ) /*== ==*/
	  { L1 = 0.0;  L2 = 0.0;};


	/*(8.8) GOST 8.563.1-97 */
   Cib = 0.5959 + 0.0312 * pow(BETA,2.1) +
            Beta4 * (-0.1840 * Beta4 + 0.090 * L1/(1 - Beta4) - 0.0337 * L2/BETA);
     
   VELAPPR = 1.0F / sqrt(1 - Beta4); /*(5.1) GOST 8.563.1-97 */

   /*  eps  (8.10) GOST 8.563.1-97 */
   EXPFACTOR = 1.0 -((0.41e-3)+(0.35e-3)*Beta4) * DP_AVERAGE
				         / (ISENTREXP * PF_AVERAGE);

   if (INSPECTIONPER/*Tpp*/)                            /* (B.40) GOST 8.563.1-97 */
	{
      musor     = ((FLOAT)INSPECTIONPER ) /3.0F;
      avgRadius = 0.195F - (0.195F - RD_ORIFCORNER/*rn*/) * (1.0F - exp(-musor)) / musor;
	}
   else
	avgRadius = RD_ORIFCORNER/*rn*/;

   /*== another condition Kp, Feb 22, 2000 */
   if( (0.0004 * ORIFD_T) > RD_ORIFCORNER )
		Kp = 1.0;
   else
     		Kp = 1.0547-0.0575*exp(-149.0 * avgRadius/ORIFD_T); /* (B.37) GOST 8.563.1-97 */

   /*  d20*Ko (5.8) GOST 8.563.2-97 */
   Qmb = 0.12645 * VELAPPR * Kp * Cib * ORIFD_T * ORIFD_T * EXPFACTOR * 
			sqrt(DP_AVERAGE * FLOWDENSITY);

	  /*Rsh - ROUGHNESS */
   r0 = 0.07 * log10(ROUGHNESS * 10000.0/PIPED_T) - 0.04;

   /* =1 (8.6) GOST 8.563.1-97 */
   cond_Ksh=(  (10000.0*ROUGHNESS/PIPED_T) <= pow(10.0,(8.0 + 0.1/Beta4)/14.0 )  );

   /*== start iterating with Qcb, use tempC which hangs on the
	       stack for the exit criteria                     ======*/
   Qm    = Qmb;
	  tempC = Qm * EPS;

   for( j = 0; j < 10; j++)
    {
     /* ?0.35... а Re  (5.8) GOST 8.563.2-97 */
     locRe = 353680.0 * Qm /(VISCOSITY * PIPED_T);

     	if( cond_Ksh || locRe <= 10000.0)
       	Ksh = 1;
     	else 
		{   	
		if ( locRe < 1000000.0F ) 
			{
			L1  = (log10(locRe)-6.0);
		 	Are = 1.0F - 0.25F * L1 * L1;
			        /*pow((log10(locRe)-6.0),2.0);  *(B.28) GOST 8.563.1-97 */
			}
        	else
			Are = 1.0F;
      Ksh = 1.0F + Beta4 * r0 * Are;             /*(B.27) GOST 8.563.1-97 */
						}
     /* Kre на  Re(8.9) GOST 8.563.1-97 */
     Kre   = 1.0 + (0.0029 * pow(BETA, 2.5) / Cib) * pow(1000000/locRe,0.75);
     
	/* == the musor guy seems to be hanging too at this moment ==*/
	musor = Qmb * Kre * Ksh;
	    
	if ( fabs(musor - Qm) < tempC )
		{
	      Qm = musor;
	      break;
		}
      Qm = musor;
    } /*end for */
}; /*end Orifice */ 
 
/*
set_param(17,flow_no, 5,(UINT8 *)&ISENTREXP);
set_param(17,flow_no, 6,(UINT8 *)&Beta4);
set_param(17,flow_no, 7,(UINT8 *)&EXPFACTOR);

set_param(17,flow_no, 9,(UINT8 *)&Kp);

set_param(17,flow_no,11,(UINT8 *)&VELAPPR);
set_param(17,flow_no,12,(UINT8 *)&Cib);
set_param(17,flow_no,13,(UINT8 *)&VISCOSITY);
set_param(17,flow_no,14,(UINT8 *)&Qmb);
set_param(17,flow_no,15,(UINT8 *)&PIPED_T);
set_param(17,flow_no,16,(UINT8 *)&COEFFZ);
set_param(17,flow_no,17,(UINT8 *)&ENTERED_DENS_BASE);
set_param(17,flow_no,18,(UINT8 *)&Qm);
set_param(17,flow_no,19,(UINT8 *)&TF_AVERAGE);
set_param(17,flow_no,20,(UINT8 *)&PF_AVERAGE);
set_param(17,flow_no,21,(UINT8 *)&DP_AVERAGE);
*/
   MASSFLOWHR        = Qm;
   Qc = Qm / ENTERED_DENS_BASE;
   FLOWRATEDAILY     = Qc * 0.024F; /*== 10^3 m3/day for maximum sensitivity ==*/

   INF_ENGRATEDAILY  = FLOWRATEDAILY * HVAL_INF;
   SUP_ENGRATEDAILY  = FLOWRATEDAILY * HVAL_SUP;
   COEFFDISCH        = Cib;
   RENUMBER=353680.0 * Qm /(VISCOSITY*PIPED_T);
   CURRSPEED         = 353.67764609F * (MASSFLOWHR/FLOWDENSITY)/(PIPED_T * PIPED_T);
	  
			if(DispInfHVal)
      DSP_ENGRATEDAILY = INF_ENGRATEDAILY;
   else
      DSP_ENGRATEDAILY = SUP_ENGRATEDAILY;
 
}
/****************************************************************************
 *		Function: IsTimerExpired (UINT16 timer, UINT16 currTime)
 *	  Parameters: - UINT16 timer, UINT16 currTime
 *       Returns: - 1 if expired, 0 if not
 *   Description: - updates timers
 *
 *	        Note: 
 ****************************************************************************/
UINT8 IsTimerExpired ( UINT16 timer, UINT16 currTime)
{
  if( (timer - currTime) > 0x7fff )    
  	  /*== overflow, 0x7fff is half of max of a 16 bit int ==*/
	  return (1);
  else if (( timer <= currTime ) && (( currTime - timer) < 600))/*= 1 min max =*/
  	  return (1);
  else
  	  return (0);

}
/******************************************************************************/
/*                                                                            */
/* SYNTAX:      void AverageInputs ( void )                                   */
/*                                                                            */
/* DESCRIPTION: This function averages Static Pressure, Differential          */
/*              Pressure, and Temperature of the gas being measured for       */
/*              calculations                                                  */
/*                                                                            */
/* ARGUMENTS:   NAME       TYPE   DESCRIPTION                                 */
/*              __________ ______ __________________________________________  */
/*                                                                            */
/* RETURNS:     Nothing                                                       */
/*                                                                            */
/* NOTES:       This function will run when a scan period completes           */                               
/*                                                                            */
/******************************************************************************/

void AverageInputs ( void )
{
   if( FLOW_TIME_ACCUM != 0 )
   {
       DP_AVERAGE = DP_ACCUM / (FLOAT)FLOW_TIME_ACCUM;
       PF_AVERAGE = PF_ACCUM / (FLOAT)FLOW_TIME_ACCUM /1000.0;  /* MPa */
       TF_AVERAGE      = TF_ACCUM / (FLOAT)FLOW_TIME_ACCUM + 273.15;
       TEMPK_SQUARED   = TF_AVERAGE * TF_AVERAGE;
       STATICOVERTEMPK = PF_AVERAGE / TF_AVERAGE;
   }

   if (DP_ACCUM > 0.0 && PF_ACCUM > 0.0)
       IS_FLOWING_FLAG = FLOWING;
   else
       IS_FLOWING_FLAG = NO_FLOW;

   DP_ACCUM = 0.0;
   PF_ACCUM = 0.0;
   TF_ACCUM = 0.0;

}
/******************************************************************************/
/*                                                                            */
/* SYNTAX:      void ReadInputs ( UINT16 ElapsedTicks )                       */
/*                                                                            */
/* DESCRIPTION: This function updates the Static Pressure, Differential       */
/*              Pressure, and Temperature of the gas being measured.  It then */
/*              decides if the sample was flowing or not and accumulates in   */
/*              the appropriate manner.                                       */
/*                                                                            */
/* ARGUMENTS:   NAME       TYPE   DESCRIPTION                                 */
/*              __________ ______ __________________________________________  */
/*            ElapsedTicks UINT16 elapsed ticks since last inputs read        */
/*                                                                            */
/* RETURNS:     Nothing                                                       */
/*                                                                            */
/* NOTES:       This function will run  every 100 ticks (once per second)     */                               
/*                                                                            */
/******************************************************************************/

void ReadInputs ( UINT16 ElapsedTicks )
{
  INT8   valid_flag;
/*UINT8  flow_at_sample;*/
  FLOAT  dpLow, dpHigh;

 /****************** Get Current Inputs for Calculation *******************/
  
  /* get differential pressure */
  /* if point Type of hw_input_pt is not in manual */
  if (DPTYPE != 0)
    {
    valid_flag = get_param (DPTYPE, DPLOGIC, DPPARAM, (UINT8 *)&dpHigh);
    
    /* check for invalid point type, logic number, parameter number, or type */
    if ( valid_flag != 0) 
      {
      DPTYPE =  0;   /* if invalid then set to manual (zero) */
      DPLOGIC = 0;
      DPPARAM = 0;
      } /* end if */
    
    if ( ENABLESTACK == 1)        /* if diff pressure stack is enabled */
     {
      if (STACK_STATUS == LOW)     /* if status is low */
       {
        if ( LOWDPTYPE != 0)
         {
           valid_flag = get_param (LOWDPTYPE, LOWDPLOGIC, 
			                       LOWDPPARAM, (UINT8 *)&dpLow);
    
      	    DPREADING = dpLow;
	          /* check for invalid point type, logic number, parameter number, or type */
           if (valid_flag != 0)
											 {
               LOWDPTYPE  = 0; /* if invalid then set to manual (zero) */
               LOWDPLOGIC = 0;
               LOWDPPARAM = 0;
											 } 
									} /* end if */
        
        if ( DPREADING >= HISETPOINT)
         {
           STACK_STATUS = HIGH;  /* read from high range next time */
         } /* end if */
							}
      else                                  /* else status is high */
       {
	        DPREADING = dpHigh;

		       if ( DPREADING <= LOWSETPOINT)
          {
            STACK_STATUS = LOW;   /* read from low range next time */
          } /* end if */
        
							} /* end if .. stack is low */
      
					}     /* end if .. stack enabled */
	   else  /* stack disabled */
    
					DPREADING = dpHigh; 
			} /* end if .. dp not in manual */

   /* get flowing static pressure */
   /* if point Type of Pf_input_pt is not in manual */
   if ( P1TYPE != 0)
    {
    valid_flag = get_param(P1TYPE, P1LOGIC, P1PARAM, P1Reading);
    
    /* check for invalid point type, logic number, parameter number, or type */
    if ( valid_flag != 0 )
      {
      P1TYPE  = 0;   /* if invalid then set to manual (zero) */
      P1LOGIC = 0;
      P1PARAM = 0;
      }  /* end if */
    } /* end if */

  /* get flowing temperature */
  /* if point Type of Tf_input_pt is not in manual */
  if (T1TYPE != 0)
    {
    valid_flag = get_param (T1TYPE,T1LOGIC,T1PARAM, T1Reading);
    
    /* check for invalid point type, logic number, parameter number, or type */
    if (valid_flag != 0)
      {
      T1TYPE  = 0;   /* if invalid then set to manual (zero) */
      T1LOGIC = 0;
      T1PARAM = 0;
      } /* end if */
    } /* end if */
  
    /* temp in Celsius */
    TF_CONV = T1READING;    
    /* diff pressure in kPa - convert to ?? */
    DP_CONV = DPREADING;
    /* static pressure in kPa - convert to ?? */
    PF_CONV = P1READING;
    /* if Gauge then change to Absolute  */

    if (GaugeStatic)
	{
      PF_CONV += ATMPRESS;

    } /* end if */

    /* if downstream then change to upstream */
    if (DnstrTaps)
    {
      PF_CONV += DPREADING;
    } /* end if */
  
    /* update Upstream Static Pressure */
    UPSTRABSPRS = PF_CONV;
    
  /************* Check for Flowing Conditions *****************/
  
  /* if flowing differential pressure is lower than or equal to the set cutoff
     or the converted static pressure is less than or equal to 0     */
  if ((DPREADING <= DPLOWCUTOFF) || (PF_CONV <= 0.0))
    {
    /* No Flow is occuring    *
    flow_at_sample = NO_FLOW; */

    /* set Differential Pressure to zero if lower than Diff Press cutoff */
    if (DPREADING <= DPLOWCUTOFF)
      {
        DP_CONV = 0.0;
      } /* end if */
    
    /* set converted Static Pressure to zero if it is less than 0.0 */
    if (PF_CONV < 0.0)
      {  
         PF_CONV = 0.0;
      } /* end if */
    
    }
  else
    {
     /* Flow is occuring  */
     /* Do a transfer to yesterday if necessary */
	  if( (ContrTransferFlag[flow_no] & 0x02) && (FLOW_TIME_ACCUM_Y == 0))
			FLOW_TIME_ACCUM_Y = FLOW_TIME_ACCUM;
     /* accumulate the flow time since last input reading in ticks */
      FLOW_TIME_ACCUM += ElapsedTicks;
     /* accumulate inputs */
	  DP_ACCUM += DP_CONV * (FLOAT)ElapsedTicks;
	  TF_ACCUM += TF_CONV * (FLOAT)ElapsedTicks;
	  PF_ACCUM += PF_CONV * (FLOAT)ElapsedTicks;
    
    } /* end if */  
    
  
  /*************** Calculate Instantaneous Values ****************/
  
  /* Pressure extension */
  DIFFSTATICPROD = sqrt(DP_CONV * PF_CONV);
  
}

/****************************************************************************/
/*      SYNTAX: FLOAT calc_Z_for_R_K(FLOAT b, FLOAT c, FLOAT Z)             */
/* DESCRIPTION: calculates point of cube equation                           */
/* CALLING SEQ: calc_Z_for_R_K(FLOAT b, FLOAT c, FLOAT Z)                   */
/*   ARGUMENTS: NAME        TYPE      DESCRIPTION                           */
/*              ---------   -------   -----------------------------------   */
/*     RETURNS:                                                             */
/*       NOTES:	                                                          */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

FLOAT calc_Z_for_R_K(FLOAT b, FLOAT c, FLOAT baseZ)             
{
FLOAT  H,Z;
UINT8  i;

Z=baseZ;
for( i = 0; i < 55; i++)
   {
   H = (Z*(Z*Z-Z+b)+c) / (3.0*Z-2.0*Z+b);
   if ( fabs(H/Z<0.0001) ) {break;} ;
   Z = Z - H;
   };
return (Z);
};


/****************************************************************************/
/*      SYNTAX: void GERG_91Routine( FLOAT Pp, FLOAT Tp )           		*/
/* DESCRIPTION: applies GERG 91                                             */
/* CALLING SEQ: GERG_91Routine( FLOAT Pp, FLOAT Tp )                        */
/*   ARGUMENTS: NAME        TYPE      DESCRIPTION                           */
/*              ---------   -------   -----------------------------------   */
/*               Pp         FLOAT     modified process press                */
/*               Tp         FLOAT     modified process temp                 */
/*     RETURNS: none                                                        */
/*       NOTES:	     */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

void GERG_91Routine (FLOAT Pp, FLOAT Tp)  
{
	 FLOAT C1, C2, C3, C223, C233, Me, ResComp, ResComp2;
  FLOAT H, Czv, Bzv, B, B1, B2, B3, B23, Bm;
	 FLOAT Cm,A0,A1,A2;
	 FLOAT pw1, pw2, pw3;

  /*if( COMP_CHANGED_FLAG == 0)
		  COMP_CHANGED_FLAG = 1;  */ /*== clear the reversed flag ==*/

  /*== Xy - fraction CO2 GOST_Comp_Adj[flow_no][6],
	      Xa - fraction N2 GOST_Comp_Adj[flow_no][5]
       Tpk - pseudo critical temperature
	      Ppk - pseudo critical pressure
	      Zc - critical Z
   *==================================================*/
	 
		ResComp  = (1.0 - GOST_Comp_Adj[flow_no][5] - GOST_Comp_Adj[flow_no][6]);
	 ResComp2 = ResComp * ResComp;

  Me = (24.05525 * CONTRACTZ * ENTERED_DENS_BASE 
			  -  28.0135 * GOST_Comp_Adj[flow_no][5] 
					-  44.01 * GOST_Comp_Adj[flow_no][6]) / ResComp;
    
	 H = 128.64F + 47.479F * Me;

  Czv = 0.0013F * TF_AVERAGE + 0.569F;
/*Bzv = (1.875e-5) * (320.0 - TF_AVERAGE) * (320.0-TF_AVERAGE) + 0.72;*/
  Bzv = 1.875e-5 * TEMPK_SQUARED - 0.012 * TF_AVERAGE + 2.64;
  C233= (3.58783e-3)+(TF_AVERAGE*8.06674e-6)-(TEMPK_SQUARED*3.25798e-8);
  C223= (5.52066e-3)-(TF_AVERAGE*1.68609e-5)+(TEMPK_SQUARED*1.57169e-8);
  C3  = ( 2.0513e-3)+(TF_AVERAGE* 3.4888e-5)-(TEMPK_SQUARED* 8.3703e-8);
  C2  = ( 7.8498e-3)-(TF_AVERAGE* 3.9895e-5)+(TEMPK_SQUARED* 6.1187e-8);

  C1  = -0.302488   +(1.95861e-3)*TF_AVERAGE-(3.16302e-6 )*TEMPK_SQUARED
      + H * (( 6.46422e-4)-(4.22876e-6)*TF_AVERAGE+(6.88157e-9 )*TEMPK_SQUARED)
      + H *H * ((-3.32805e-7)+( 2.2316e-9)*TF_AVERAGE-(3.67713e-12)*TEMPK_SQUARED);

  B3  = -0.86834 +( 4.0376e-3)*TF_AVERAGE-( 5.1657e-6)*TEMPK_SQUARED;
  B23 = -0.339693+(1.61176e-3)*TF_AVERAGE-(2.04429e-6)*TEMPK_SQUARED;
  B2  = -0.1446  +( 7.4091e-4)*TF_AVERAGE-( 9.1195e-7)*TEMPK_SQUARED;

  B1  = -0.425468 +(  2.865e-3)*TF_AVERAGE-(4.62073e-6 )*TEMPK_SQUARED
      + H * (( 8.77118e-4)-(5.56281e-6)*TF_AVERAGE+(8.81514e-9 )*TEMPK_SQUARED)
      + H *H *((-8.24747e-7)+(4.31436e-9)*TF_AVERAGE-(6.08319e-12)*TEMPK_SQUARED);

  Bm  = B1 * ResComp2 
			   + ResComp *GOST_Comp_Adj[flow_no][5]*Bzv*(B1+B2) 
						- 1.73F * ResComp *GOST_Comp_Adj[flow_no][6]*sqrt(B1*B3) 
						+ SQUARED_N2 * B2 
						+ 2.0F * PROD_N2_CO2 * B23
						+ SQUARED_CO2 * B3;

     
	 pw1 = pow(C1,OneThird);
  pw2 = pow(C2,OneThird);
  pw3 = pow(C3,OneThird);
  Cm  = C1  * ResComp2 * ResComp + pw1 * 
		     (3.0F   * ResComp2 * GOST_Comp_Adj[flow_no][5] * Czv * pw1 * pw2 + 
        2.76F* ResComp2 * GOST_Comp_Adj[flow_no][6] * pw1 * pw3 + 
        3.0F * ResComp * SQUARED_N2 * Czv * pw2 * pw2 + 
        6.6F *ResComp * PROD_N2_CO2 * pw2 * pw3 +  
        2.76F*ResComp * SQUARED_CO2 * pw3 * pw3) + 
        CUBED_N2 * C2 +
		      3.0F * SQUARED_N2 * GOST_Comp_Adj[flow_no][6] * C223 +
		      3.0F * GOST_Comp_Adj[flow_no][5] * SQUARED_CO2 * C233 +
		      CUBED_CO2 * C3;

  B  = PF_AVERAGE / TF_AVERAGE / 2.7715e-3;
  A1 = 1.0F + B * Bm;
  A0 = 1.0F + 1.5F * ( B * Bm + B * B * Cm);
  A2 = pow(A0-sqrt(A0*A0-A1*A1*A1),OneThird);

  FLOWZ  = FLOWZ_FST = (1.0F + A2 + A1/A2) / 3.0F;
  COEFFZ = FLOWZ / CONTRACTZ;

  FLOWDENSITY = FLOWDENS_FST = ENTERED_DENS_BASE * 1000.0F * 
         STATICOVERTEMPK /BASEPRESS/COEFFZ * (BASETEMP+273.15F);
                     /* (6), GOST 30319.1-96*/


}

/****************************************************************************/
/*      SYNTAX: void NX_19Routine( FLOAT Pp, FLOAT Tp )               		    */
/* DESCRIPTION: applies NX_19                                               */
/* CALLING SEQ: NX_19Routine( FLOAT Pp, FLOAT Tp )                          */
/*   ARGUMENTS: NAME        TYPE      DESCRIPTION                           */
/*              ---------   -------   -----------------------------------   */
/*               Pp         FLOAT     modified process press                */
/*               Tp         FLOAT     modified process temp                 */
/*     RETURNS: none                                                        */
/*       NOTES:	                                                            */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

void NX_19Routine ( FLOAT Pp, FLOAT Tp)  
{
	DOUBLE Ta, Ta2, Pa,dTa, dTa2;
    DOUBLE musor, mus_i, mus_j,F, Q1,Q0,fB1,fB0,fB2 ;

    /*if( COMP_CHANGED_FLAG == 0)
		    COMP_CHANGED_FLAG = 1;*/  /*== clear the reversed flag ==*/


    Ta   = (DOUBLE)0.71892 * Tp + (DOUBLE)0.0007;
    Pa   = (DOUBLE)0.6714 * Pp + (DOUBLE)0.0147;
	   Ta2  = Ta * Ta;
    dTa  = Ta - (DOUBLE)1.09;
	   dTa2 = dTa * dTa;
    musor= (75e-5)*pow(Pa,(DOUBLE)2.3);

    if((Pa >= (DOUBLE)0 && Pa <= (DOUBLE)2) && 
		   (dTa >= (DOUBLE)0 && dTa <= (DOUBLE)0.3))
					{
		      Q0 = Pa * ((DOUBLE)2.17 - Pa + (DOUBLE)1.4 * sqrt(dTa));
        F  = musor/exp((DOUBLE)20 * dTa) + (11e-4) * sqrt(dTa) * Q0 *Q0;
					}
    
	   if((Pa>=(DOUBLE)0 && Pa<(DOUBLE)1.3) && 
		     (dTa >= (DOUBLE)(-0.25) && dTa < (DOUBLE)0))
					{
        F = musor * ((DOUBLE)2 - exp((DOUBLE)20 * dTa)) + 
			         (DOUBLE)1.317 * Pa * ((DOUBLE)1.69 - Pa * Pa) * dTa2 * dTa2;
					}
    
	   if((Pa >= (DOUBLE)1.3 && Pa < (DOUBLE)2) && 
		    (dTa >= (DOUBLE)-0.21 && dTa < (DOUBLE)0))
     {
        mus_i = dTa * ((DOUBLE)0.03249 + (DOUBLE)18.028 * dTa2) +
			             dTa2 * ((DOUBLE)2.0167 + dTa2 * ((DOUBLE)42.844 + (DOUBLE)200 * dTa2));
        F = musor * ((DOUBLE)2 - exp((DOUBLE)20*dTa)) + 
			        (DOUBLE)0.455 * ((DOUBLE)1.3-Pa)*((DOUBLE)4.0195200487-Pa*Pa)*mus_i;
     }

    Q1 = Ta2 * Ta2 * Ta / (Ta2 * ((DOUBLE)6.60756 * Ta - (DOUBLE)4.42646)+(DOUBLE)3.22706);
    Q0 = (Ta2 * ((DOUBLE)1.77218 - (DOUBLE)0.8879 * Ta) + (DOUBLE)0.305131) *
		       Q1/Ta2/Ta2;

    fB1 = (DOUBLE)2.0 * Q1 /(DOUBLE)3.0 - Q0 * Q0;
    fB0 = Q0 * (Q1 - Q0 * Q0) + (DOUBLE)0.1 * Q1 * Pa * (F - (DOUBLE)1);
    fB2 = pow(fB0 + sqrt(fB0*fB0 + fB1*fB1*fB1),OneThird);

	   dTa2   = (DOUBLE)1 + (DOUBLE)0.00132/pow(Ta,3.25);
  	 FLOWZ  = FLOWZ_FST = dTa2 * dTa2 * Pa/(DOUBLE)10.0/(fB1/fB2-fB2+Q0);
	   COEFFZ = FLOWZ / CONTRACTZ;

  FLOWDENSITY = FLOWDENS_FST = ENTERED_DENS_BASE * 1000.0F * 
         STATICOVERTEMPK /BASEPRESS/COEFFZ * (BASETEMP+273.15F);
                     /* (6), GOST 30319.1-96*/

}

/****************************************************************************/
/*      SYNTAX: void RunTimeViscGRNX( FLOAT Pp, FLOAT Tp )            		    */
/* DESCRIPTION: calculates viscosity, isentropic exp and flow speeds at     */
/*              runtime for GERG-91 and NX-19 methods; Calculates flow      */
/*              density too!!                                               */
/* CALLING SEQ: RunTimeViscGRNX ()                                          */
/*   ARGUMENTS: NAME        TYPE      DESCRIPTION                           */
/*              ---------   -------   -----------------------------------   */
/*               Pp         FLOAT     modified process press                */
/*               Tp         FLOAT     modified process temp                 */
/*     RETURNS: none                                                        */
/*       NOTES:	                                                            */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
void RunTimeViscGRNX( FLOAT Pp, FLOAT Tp)
{
  FLOAT Mut, Mu, Cmu;

  /* Dinamic viscosity calculation */
  Mut = 3.24 * (sqrt(TF_AVERAGE) + 1.37 - 9.09 * pow(ENTERED_DENS_BASE,0.125));
  Mut = Mut/(sqrt(ENTERED_DENS_BASE) + 2.08 -
	         1.5 * (GOST_Comp_Adj[flow_no][5] + GOST_Comp_Adj[flow_no][6]));
                                 /* (44), GOST 30319.1-96*/
  Cmu = 1.0 + Pp * Pp / (30.0 * (Tp-1));
  VISCOSITY = Mut * Cmu;         /* (45), GOST 30319.1-96*/
                                 /* (28), GOST 30319.1-96*/
  
  SOUNDSPEED = 18.591 * sqrt(TF_AVERAGE * ISENTREXP * COEFFZ / ENTERED_DENS_BASE); 
               /* (31), GOST 30319.1-96*/

  /*== NOTE: Set BaseTemp to 20 and BasePress to 101.325 
             The advantage is, it still calculates right if 
			 base conditions are changed ==*/
}



/****************************************************************************/
/*      SYNTAX: void VNIC_SMVRoutine( FLOAT Pp, FLOAT Tp )            		    */
/* DESCRIPTION: applies VNIC_SMV                                            */
/* CALLING SEQ: VNIC_SMVRoutine (FLOAT, FLOAT)                              */
/*   ARGUMENTS: NAME        TYPE      DESCRIPTION                           */
/*              ---------   -------   -----------------------------------   */
/*               Pp         FLOAT     modified process press                */
/*               Tp         FLOAT     modified process temp                 */
/*     RETURNS: none                                                        */
/*     RETURNS: none                                                        */
/*       NOTES:	     */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

void VNIC_SMVRoutine (FLOAT Pp, FLOAT Tp)  
{
  UINT8  j,k,l;
  DOUBLE R_T;
  FLOAT  locZ,locK;
  FLOAT  Rom, dRom;
  FLOAT  Rom0;
		FLOAT  Ro2,Ro3,Ro4,Ro5,Ro6,Ro7,Ro8,Ro9,Ro10;
  FLOAT  A1vnic, A2vnic, A3vnic;
  FLOAT  musor, mus_i, mus_j;
  FLOAT  Roc_i;
  FLOAT  Cpom_R;
  FLOAT  CpCv1A1;
  FLOAT *pt1, *pt2, *pt3;

  FLOAT  ksi;
  FLOAT  Mu0;
  FLOAT  Tp2, Rop2, Rop4;
 
  R_T = GAS_CONST * TF_AVERAGE;

 /******************************* O,Ropk,Tpk,Ppk,Tp,Pp */

  memset(sumBase,0,40);   /* Reset all sums */ 
  memset(sumBase1,0,40);  /* there is no guarantee          */ 
  memset(sumBase2,0,40);  /* the memory would be successive */ 

		for( l = 0, mus_i = 1.0; l < 8; l++, mus_i *= Tp )
			{
    for(k=0, mus_j = ROPK_FACTOR; k < Sl[l]; k++, mus_j *= ROPK_FACTOR)
					{
					   musor        = (Alk[l][k] + Blk[l][k]*O_FACTOR)/mus_j/mus_i;
					   sumBase[k]  += musor;
        sumBase1[k] += (1-l) * musor;
        sumBase2[k] += (1-l) * l * musor;
						}
			}

		/*
		for(l = 0, mus_i = 1;  l < 8; l++, mus_i *= Tp  )
    for(k = 1,mus_j = ROPK_FACTOR; k <= Sl[l]; k++,mus_j *= ROPK_FACTOR)
        Base[l][k] = (Alk[l][k-1] + Blk[l][k-1] * O_FACTOR)/mus_j/mus_i;  *(63+...) *
   */
  Rom0 = 9000.0F * PF_AVERAGE / TF_AVERAGE / (GAS_CONST * (1.1F * Pp+0.7F));  /*  (75)    */
  Rom  = Rom0;
  
		/*=== iterate max 20 times for flow Z ====*/
  
		for (l = 0; l < 20; l++)
   {
		
     Ro2 = Rom * Rom;
     Ro3 = Rom * Ro2;
     Ro4 = Rom * Ro3;
     Ro5 = Rom * Ro4;
     Ro6 = Rom * Ro5;
     Ro7 = Rom * Ro6;
     Ro8 = Rom * Ro7;
     Ro9 = Rom * Ro8;
     Ro10 = Rom * Ro9;
     /*
     A1vnic = 0.0;
	    locZ   = 1.0;
      
					for(l=0; l < 8; l++)
						{
        for(k=1,mus_i=Rom;k<=Sl[l];k++,mus_i*=Rom)
									{
           musor = Base[l][k]*mus_i;
           locZ += musor;
           A1vnic += (k+1)*musor;
										} * for k,l*
					 }
		    */
					
     locZ = 1.0 + Rom * sumBase[0] + Ro2 * sumBase[1] + Ro3 * sumBase[2] 
			 + Ro4 * sumBase[3] + Ro5 * sumBase[4] + Ro6 * sumBase[5] 
  			+ Ro7 * sumBase[6] + Ro8 * sumBase[7] + Ro9 * sumBase[8] 
																+ Ro10 * sumBase[9];
										
			  A1vnic = 2.0 * Rom * sumBase[0] + 3.0 * Ro2 * sumBase[1] + 4.0 * Ro3 * sumBase[2] 
						      + 5.0 * Ro4 * sumBase[3] + 6.0 * Ro5 * sumBase[4] + 7.0 * Ro6 * sumBase[5]
		          + 8.0 * Ro7 * sumBase[6] + 9.0 * Ro8 * sumBase[7] + 10.0 * Ro9 * sumBase[8] 
												+ 11.0 * Ro10 * sumBase[9];
     
					dRom = (1000.0F * PF_AVERAGE - (R_T*locZ*Rom))/(R_T * (1.0F + A1vnic));    /*  (78) */
     Rom += dRom;	                                                              /*  (79) */
      
			  if(fabs(dRom/Rom) < 0.000001)
					  break;
				
			} 
/*		if(++i > 20)
		     break;
   
			}while( (( mus_i = dRom/Rom ) >= 0 ? mus_i : -mus_i) > 0.000001 );*/

   
   ROP_FACTOR = Rom/ROPK_FACTOR;
   /* **********************************   Z,Rom,Rop,A1vnic */

   FLOWZ = FLOWZ_FST = locZ;

  

   COEFFZ = FLOWZ / CONTRACTZ;
    
	  FLOWDENSITY = FLOWDENS_FST = Rom * MOLWEIGHT;           /*(2)  G3 */
   /* **********************************   K,Ro,*/



/*
   A2vnic = 0.0;
   A3vnic = 0.0;
   
			for(l = 0; l < 8; l++)
    {
      for(k = 1; k <= Sl[l]; k++)
							{
         for(k = 1, mus_i = Rom; k <= Sl[l]; k++, mus_i *= Rom)
										{
            musor = (1-l) * Base[l][k] * mus_i;
            A2vnic += musor;                               * (9)  G3 *
            A3vnic += musor * l / k;                       * (10) G3 *
										} * for k,l *
							}
				}

  */
   A2vnic  = Rom * sumBase1[0] + Ro2 * sumBase1[1] + Ro3 * sumBase1[2] + Ro4 * sumBase1[3]
           + Ro5 * sumBase1[4] + Ro6 * sumBase1[5] + Ro7 * sumBase1[6] + Ro8 * sumBase1[7]
           + Ro9 * sumBase1[8] + Ro10 * sumBase1[9];

   A3vnic  = Rom * sumBase2[0]     + Ro2 * sumBase2[1]/2.0
	     + Ro3 * sumBase2[2]/3.0 + Ro4 * sumBase2[3]/4.0
           + Ro5 * sumBase2[4]/5.0 + Ro6 * sumBase2[5]/6.0
           + Ro7 * sumBase2[6]/7.0 + Ro8 * sumBase2[7]/8.0
           + Ro9 * sumBase2[8]/9.0 + Ro10 * sumBase2[9]/10.0;

    /* **********************************   A2vnic, A3vnic */

			
			Cpom_R = 0.0;
    
	  for (l = 0; l < 8; l++)
    {
      Cpoi_R[l] = 0.0;
      mus_i     = TF_AVERAGE / (FLOAT)Tni[l];
   
						for (j = 0, mus_j = 1; j <= N1i[l];)             /*(13)/R G3*/
       {
          Cpoi_R[l] += Alpha_ij[l][j++] * mus_j;
          mus_j     *= mus_i;
      
										if( j <= N2i[l])
              Cpoi_R[l] += Betta_ij[l][j-1]/mus_j;
							}/* for j*/
       
						Cpom_R += GOST_Comp_Adj[flow_no][l] * Cpoi_R[l];       /*(12)/R G3*/
				}  /* end   for i=0;i<8 */
  
			CpCv1A1 = 1.0F + A1vnic + (1.0F + A2vnic) * (1.0F + A2vnic)
				       / ( Cpom_R - 1.0F + A3vnic );
   
			/*********************************   Cp, Cv */
   ISENTREXP  = CpCv1A1/locZ;                              /*(5)  G3*/
   /* ********************************   adiab  */
   SOUNDSPEED = sqrt(1000.0 * R_T * CpCv1A1 / MOLWEIGHT);  /*(14) G3*/
   /* ********************************   Velos  */



  Rop2 = ROP_FACTOR * ROP_FACTOR;
  Rop4 = Rop2 * Rop2;

  Tp2 = Tp * Tp;             /*  (77)  */

  /* Dinamic viscosity calculation */
  ksi  = MOLWEIGHT * PRESSPSCRITIC * PRESSPSCRITIC;
  ksi *= ksi;
  ksi  = TEMPPSCRITIC /( ksi * MOLWEIGHT );
  ksi  = pow ( ksi, 0.16666667);                        /*(17) G3*/
  
  Mu0  = 78.037 + 3.85612 * O_FACTOR - 29.0053 * O_FACTOR * O_FACTOR 
			    + (-156.728+145.519/Tp-51.1082/Tp2 )/Tp
							+ ROP_FACTOR*( 6.57895+(11.7452-95.7215*O_FACTOR*O_FACTOR/Tp)*ROP_FACTOR
							+ 17.1027 * Rop2 *O_FACTOR+0.519623/Tp2 * Rop4 );
  
  VISCOSITY = Mu0/(10.0 * ksi);                                    /*(15) G3*/


}

/****************************************************************************/
/*      SYNTAX: void Reilih_Kvong_Routine( FLOAT Pp, FLOAT Tp )  		    */
/* DESCRIPTION: applies Reilih Kvong						    */
/* CALLING SEQ: NX_19Routine( FLOAT Pp, FLOAT Tp )                          */
/*   ARGUMENTS: NAME        TYPE      DESCRIPTION                           */
/*              ---------   -------   -----------------------------------   */
/*               Pp         FLOAT     modified process press                */
/*               Tp         FLOAT     modified process temp                 */
/*     RETURNS: none                                                        */
/*       NOTES:	                                                          */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

void Reilih_Kvong_Routine( FLOAT Pp, FLOAT Tp)
{
UINT8 i;
FLOAT  musor, mus_i, mus_j;

FLOAT    base_A,base_B,hvost_Aj,hvost_Ajc,hvost_Bj,hvost_Bjc,A,B,Ac,Bc;  

FLOAT  tempC;
FLOAT dry_dens,wet_dens,fi,P_sat_steam;
FLOAT RO_sat_steam,P_sat_steam_z,RO_sat_steam_z;
FLOAT P_z,T_z,fi_z;	

get_param(17,flow_no,3,(UINT8 *)&musor);  
get_param(17,flow_no,4,(UINT8 *)&P_z);  
get_param(17,flow_no,5,(UINT8 *)&fi_z);  
T_z=musor+273.15;
	
  tempC = TF_AVERAGE - 273.15; 

 
A=0;B=0;Ac=0;Bc=0;

/*Algoritm (3),(4),(5),(6),(7),(8),(21)...*/
hvost_Aj  = PF_AVERAGE / pow(TF_AVERAGE,2.5);
hvost_Ajc = 0.001 * BASEPRESS  / pow((BASETEMP+273.15),2.5);
hvost_Bj  = PF_AVERAGE / TF_AVERAGE;
hvost_Bjc = 0.001 * BASEPRESS  / (BASETEMP+273.15);

 for (i = 0; i < 18; i++)
 {
 base_A = 0.4274802327 * pow(Tki_kvong[i],2.5) / Pki_kvong[i];
 base_B = 0.08664035 * Tki_kvong[i] / Pki_kvong[i];
 A  += GOST_Comp[flow_no][i]*sqrt(base_A * hvost_Aj);
 Ac += GOST_Comp[flow_no][i]*sqrt(base_A * hvost_Ajc);
 B  += GOST_Comp[flow_no][i]*base_B * hvost_Bj; /*Algoritm (8)*/
 Bc += GOST_Comp[flow_no][i]*base_B * hvost_Bjc; /*Algoritm (8)*/
 };
 A = A*A; /*Algoritm (7)*/
 Ac = Ac*Ac; /*Algoritm (7)*/
 /* ********************************   adiab, A,B,Ac,Bc  */


/*set_param(17,flow_no,21,(UINT8 *)&ISENTREXP);

set_param(17,flow_no,12,(UINT8 *)&A);  
set_param(17,flow_no,13,(UINT8 *)&Ac);  
set_param(17,flow_no,14,(UINT8 *)&B);  
set_param(17,flow_no,15,(UINT8 *)&Bc);  
*/

mus_i = calc_Z_for_R_K(A-B*B-B /***b***/, -A*B /***c***/, 1.00 /***Z***/); /*Algoritm B.1*/         
mus_j = calc_Z_for_R_K(A-B*B-B /***b***/, -A*B /***c***/, 0.01 /***Z***/); /*Algoritm B.2*/                     
musor = 1 - mus_i - mus_j;		/*Algoritm B.3*/         

FLOWZ = MAX(MAX(mus_i,mus_j),musor);   /*Algoritm 4.2.3.5*/         						
/*set_param(17,flow_no,17,(UINT8 *)&FLOWZ);  */

mus_i = calc_Z_for_R_K(Ac-Bc*Bc-Bc /***b***/, -Ac*Bc /***c***/, 1.00 /***Z***/); /*Algoritm B.1*/         
mus_j = calc_Z_for_R_K(Ac-Bc*Bc-Bc /***b***/, -Ac*Bc /***c***/, 0.01 /***Z***/); /*Algoritm B.2*/                     
musor = 1 - mus_i - mus_j;		/*Algoritm B.3*/         

CONTRACTZ = MAX(MAX(mus_i,mus_j),musor);   /*Algoritm 4.2.3.5*/         						
/*set_param(17,flow_no,18,(UINT8 *)&CONTRACTZ);  */
 
COEFFZ = FLOWZ / CONTRACTZ;    


	if ( /*(243.15<=TF_AVERAGE)&&*/  (TF_AVERAGE<273.15))   
		{mus_i=TF_AVERAGE/273.15;

		musor=-9.09685*(1-mus_i)/mus_i-3.56654*log10(1/mus_i)+0.87682*(1-mus_i)+0.78614;
		P_sat_steam=100*pow(10,musor);             		/* Algoritm (11) */

		RO_sat_steam=(2.167e-3)*P_sat_steam/TF_AVERAGE;		/* Algoritm (13) */
		};
	if ( /*(243.15<=T_z)&&*/ (T_z<273.15))   
		{mus_i=T_z/273.15;

		musor=-9.09685*(1-mus_i)/mus_i-3.56654*log10(1/mus_i)+0.87682*(1-mus_i)+0.78614;
		P_sat_steam_z=100*pow(10,musor);				/* Algoritm (11) */

		RO_sat_steam_z=(2.167e-3)*P_sat_steam/TF_AVERAGE;	/* Algoritm (13) */
		};
	if ((273.15<=TF_AVERAGE) /*&&(TF_AVERAGE<340)*/)   
		{mus_j=1-TF_AVERAGE/647.14;

		musor=-7.85823*(mus_j)+1.8399*pow(mus_j,1.5)-11.7811*pow(mus_j,3)+
		  22.6705*pow(mus_j,3.5)-15.9393*pow(mus_j,4)+1.77516*pow(mus_j,7.5);
		P_sat_steam=(22.064e+6)*exp(musor/(1-mus_j)); 		/* Algoritm (12) */

		musor=2.02957*pow(mus_j,0.3333333333333)+2.68781*pow(mus_j,0.666666666666666)+
		      5.38107*pow(mus_j,1.3333333333333)+17.3151*mus_j*mus_j*mus_j +
		      44.6384*pow(mus_j,6.1666666666666)+64.3486*pow(mus_j,11.83333333333333);	
		RO_sat_steam=322*exp(-musor);           /* Algoritm (14) */
		};
	if ((273.15<=T_z) /*&&(T_z<340)*/)   
		{mus_j=1-T_z/647.14;

		musor=-7.85823*(mus_j)+1.8399*pow(mus_j,1.5)-11.7811*pow(mus_j,3)+
		  22.6705*pow(mus_j,3.5)-15.9393*pow(mus_j,4)+1.77516*pow(mus_j,7.5);
		P_sat_steam_z=(22.064e+6)*exp(musor/(1-mus_j)); 	/* Algoritm (12) */

		musor=2.02957*pow(mus_j,0.3333333333333)+2.68781*pow(mus_j,0.666666666666666)+
		      5.38107*pow(mus_j,1.3333333333333)+17.3151*mus_j*mus_j*mus_j +
		      44.6384*pow(mus_j,6.1666666666666)+64.3486*pow(mus_j,11.83333333333333);
		RO_sat_steam_z=322*exp(-musor);           		/* Algoritm (14) */
		};


/*
set_param(17,flow_no,6,(UINT8 *)&P_sat_steam); 
set_param(17,flow_no,7,(UINT8 *)&RO_sat_steam);  

set_param(17,flow_no,8,(UINT8 *)&P_sat_steam_z);  
set_param(17,flow_no,9,(UINT8 *)&RO_sat_steam_z);  
*/
	fi=fi_z*(PF_AVERAGE*1000*T_z*RO_sat_steam_z)/(TF_AVERAGE*P_z*RO_sat_steam);/* Algoritm (15) */
	set_param(17,flow_no,2,(UINT8 *)&fi);  
	if(fi>1){fi=1;};	

	dry_dens = ENTERED_DENS_BASE * (BASETEMP + 273.15)*
	 (PF_AVERAGE*1000- fi*P_sat_steam/1000)/(BASEPRESS * TF_AVERAGE * COEFFZ);/* Algoritm (16) */
	set_param(17,flow_no,3,(UINT8 *)&dry_dens );
	wet_dens = dry_dens + fi * RO_sat_steam ;				/* Algoritm (17) */
	set_param(17,flow_no,4,(UINT8 *)&wet_dens );

FLOWDENSITY = FLOWDENS_FST = dry_dens*dry_dens/wet_dens;  
}


/****************************************************************************/
/*      SYNTAX: UINT8 CheckCompAndBaseDens( void )                          */
/* DESCRIPTION: checks and deals with composition changes                   */
/* CALLING SEQ: CheckComposition ()                                         */
/*   ARGUMENTS: NAME        TYPE      DESCRIPTION                           */
/*              ---------   -------   -----------------------------------   */
/*     RETURNS: sets 0 if changed, 1 if unchanged                           */
/*       NOTES:	-reversed flags allow the function to fire at first run     */
/*                                                                          */
/*                                                                          */
/****************************************************************************/

UINT8 CheckCompAndBaseDens (void)  
{
	 UINT8 i,j, result,ret_value;
	 UINT16 curr_crc;
	 FLOAT rough_sum, locZBase, Vcr, locFltReg, locTpk, *act_comp_flt;
	 FLOAT  locZ,Roc_i,musor;	
/*	 actual_comp = &NITROGEN;*/

	 /* Calc only crc 
		stored_comp = &StoredComp[flow_no][0]; == Fisher sequence ==*/

	 ret_value = 1;
  
	 curr_crc = calc_crc((UINT8 *)&NITROGEN, 80);
		if ( curr_crc != StoredCompCRC[flow_no] )
			{
			 /* composition has changed */
			  StoredCompCRC[flow_no] = curr_crc;
     ret_value = 0;
			}
	 /* == BaseDensity holds the old value for Roc == */
	 result = compare_floats(OLD_ENT_DENS, ENTERED_DENS_BASE);
	 if(result)
			{
       OLD_ENT_DENS = ENTERED_DENS_BASE;
       ret_value = 0;
			}
	 /* == and finally we check if the customer changed his mind
	      and wants a different type of calculation ==*/

	 if ((VNIC_SMV) && (OLD_METHOD != PREV_VNIC))
			{
		   OLD_METHOD = PREV_VNIC;
		   ret_value  = 0;
			}

	 if ((GERG_91) && (OLD_METHOD != PREV_GERG))
			{
		   OLD_METHOD = PREV_GERG;
		   ret_value  = 0;
			}

	 
       if ((NX_19) && (OLD_METHOD != PREV_NX19))
			{
		   OLD_METHOD = PREV_NX19;
		   ret_value  = 0;
			}
	
	 if ((REILIH_K) && (OLD_METHOD != PREV_R_KV))
			{
		   OLD_METHOD = PREV_R_KV;
		   ret_value  = 0;
			}


  /* return if nothing happened since last scan */
	 if ( ret_value )
		 return (ret_value);

	 act_comp_flt = &NITROGEN;
	 rough_sum   = 0.0F;

	 for ( i = 0; i < NO_COMP_ROC; i++, act_comp_flt++ )
			 rough_sum +=  *act_comp_flt;
	 if ( compare_floats( rough_sum, 100.0F) )
			{
		  /* discard to methane */
		  METHANE += (100.0F - rough_sum);
				/* recalc crc to avoid an unnecesary iteration */
				StoredCompCRC[flow_no] = calc_crc((UINT8 *)&NITROGEN,80);
			}
	
		/*=======================================================
	    map Fisher components into GOST 18 components sequence
   *=======================================================*/
		 /* we may save some code memory by using pointers
		 act_comp_flt = &GOST_Comp[flow_no][0];
			
			*act_comp_flt++ = METHANE / 100.0F;
		 *act_comp_flt++ = ETHANE / 100.0F;
		 *act_comp_flt++ = PROPANE / 100.0F;
		 *act_comp_flt++ = NBUTANE / 100.0F;
		 *act_comp_flt++ = IBUTANE / 100.0F;
		 
			*/
		 GOST_Comp[flow_no][0] = METHANE / 100.0F;
		 GOST_Comp[flow_no][1] = ETHANE / 100.0F;
		 GOST_Comp[flow_no][2] = PROPANE / 100.0F;
		 GOST_Comp[flow_no][3] = NBUTANE / 100.0F;
		 GOST_Comp[flow_no][4] = IBUTANE / 100.0F;
		 GOST_Comp[flow_no][5] = NITROGEN / 100.0F;
		 GOST_Comp[flow_no][6] = CDIOX / 100.0F;
		 GOST_Comp[flow_no][7] = HSULFIDE / 100.0F;
		 GOST_Comp[flow_no][8] = NPENTANE / 100.0F;
		 GOST_Comp[flow_no][9] = IPENTANE / 100.0F;
		 GOST_Comp[flow_no][10] = HEXANES / 100.0F;
		 GOST_Comp[flow_no][11] = (HEPTANES + DECANES + NONANES) / 100.0F;
		 GOST_Comp[flow_no][12] = OCTANES / 100.0F;
		 GOST_Comp[flow_no][13] = WATER / 100.0F;
		 GOST_Comp[flow_no][14] = HELIUM / 100.0F;
		 GOST_Comp[flow_no][15] = OXYGEN / 100.0F;
		 GOST_Comp[flow_no][16] = CARBMONO / 100.0F;
		 GOST_Comp[flow_no][17] = HYDROGEN / 100.0F;

  /*=================================================
    map Fisher components into GOST adjusted sequence
   *================================================*/

			GOST_Comp_Adj[flow_no][8] = GOST_Comp[flow_no][13] + GOST_Comp[flow_no][14] +
			                            GOST_Comp[flow_no][15] + GOST_Comp[flow_no][16] +
																															GOST_Comp[flow_no][17];
	/* the ninth slot holds the fraction sum for minors */
	  GOST_Comp_Adj[flow_no][0] = GOST_Comp[flow_no][0];
	  GOST_Comp_Adj[flow_no][1] = GOST_Comp[flow_no][1];
	  GOST_Comp_Adj[flow_no][2] = GOST_Comp[flow_no][2];
	  GOST_Comp_Adj[flow_no][3] = GOST_Comp[flow_no][3] + GOST_Comp[flow_no][8] +
			                            GOST_Comp[flow_no][9] + GOST_Comp[flow_no][10] + 
																															GOST_Comp[flow_no][11];
	  GOST_Comp_Adj[flow_no][4] = GOST_Comp[flow_no][4];
	  GOST_Comp_Adj[flow_no][5] = GOST_Comp[flow_no][5] + GOST_Comp_Adj[flow_no][8];
	  GOST_Comp_Adj[flow_no][6] = GOST_Comp[flow_no][6];
	  GOST_Comp_Adj[flow_no][7] = GOST_Comp[flow_no][7];
	 
  /*== refresh Molecular Weight ==*/
	 locFltReg = 0.0;
	 act_comp_flt = &GOST_Comp[flow_no][0];

  for (i=0; i < NO_COMP_GOST; i++, act_comp_flt++)
			{
     locFltReg += *act_comp_flt * GOST_Mi[i];   /*(3)  G3 */
			}

	 MOLWEIGHT = locFltReg;

/*=====================================================
  each GOST method calculates viscosity and 
  isentropic exponent at flow conditions        
 *=====================================================*/

  if( (OLD_METHOD == PREV_VNIC )||(OLD_METHOD == PREV_R_KV))
   {
      

  if(ret_value == 0)
    { musor = 0.0;
      locZ   = 0.0;
	Roc_i  = 0.0;
      rough_sum = 0.0; /*== used to iterate sup hval ==*/
	locFltReg = 0.0; /*== used to iterate inf hval ==*/
	
	act_comp_flt = &GOST_Comp[flow_no][0];


      for (i=0; i < 18; i++, act_comp_flt++)
	     {
	     rough_sum += *act_comp_flt * hv_sup[i];
           locFltReg += *act_comp_flt * hv_inf[i];

           musor  += *act_comp_flt * GOST_isentr_i[i]; /*Algoritm (21)*/

           locZ   += *act_comp_flt * Bi_05[i];             /*(18) G1 */
           Roc_i  += *act_comp_flt * Roc_i_i[i];           /*(17) G1 */
           }
      HVAL_SUP = rough_sum;
      HVAL_INF = locFltReg;

	ISENTREXP = musor; /*will be owerwritten by VNIC-SMV*/

      CONTRACTZ = CONTRZ_FST = 1.0 - locZ * locZ; /*(18) G1 */
      /*===== VNIC method overrides input gas density base ===*/
	BASEDENSITY = ENTERED_DENS_BASE = Roc_i/CONTRACTZ;      /* (16) G1 */

	} 

   

	     /*== I need to save a Ropk and O factors, beside the
	          pseudo critic values ======*/
      locFltReg  = 0.0; 
	rough_sum  = 0.0;
	locTpk     = 0.0;
       
						for(i = 0; i < 8; i++)
							{
		       if(GOST_Comp_Adj[flow_no][i] <= 0.000001F)
			         continue;
         for(j = 0; j < 8; j++)
										{
		          if(GOST_Comp_Adj[flow_no][j] <= 0.000001F)
			            continue;
            Vcr         = GOST_Comp_Adj[flow_no][i] * 
		                        GOST_Comp_Adj[flow_no][j] * Vkij[i][j];
            locFltReg  += Vcr;                     /* (..64)  */
            rough_sum  += Vcr*Oij[i][j];           /* (..69)  */
            locTpk     += Vcr*Tkij_2[i][j];        /* (67)    */
										}
							}
      
	     ROPK_FACTOR    = 1.0F/locFltReg;              /*  (64)  */
      O_FACTOR       = ROPK_FACTOR * rough_sum;    /*  (69)  */
      TEMPPSCRITIC   = sqrt(locTpk * ROPK_FACTOR); /*  (66)  */
      PRESSPSCRITIC  = (8.31451e-3) * (0.28707 - 0.05559 * O_FACTOR)*
		                     ROPK_FACTOR * TEMPPSCRITIC; 

				}
   if( (OLD_METHOD == PREV_GERG)||(OLD_METHOD == PREV_NX19))
    {
    /*== Xy - fraction CO2 GOST_Comp_Adj[flow_no][6],
	        Xa - fraction N2 GOST_Comp_Adj[flow_no][5]
         Tpk - pseudo critical temperature
	        Ppk - pseudo critical pressure
	        Zc - critical Z
     *==================================================*/
       TEMPPSCRITIC = 88.25F * (0.9915F + 1.759F * ENTERED_DENS_BASE -
					                 GOST_Comp_Adj[flow_no][6] - 1.681F * GOST_Comp_Adj[flow_no][5]);
      
	      PRESSPSCRITIC = 2.9585F * (1.608F - 0.05994F * ENTERED_DENS_BASE +
					                  GOST_Comp_Adj[flow_no][6] - 0.392F * GOST_Comp_Adj[flow_no][5]);

       locZBase      = -0.006F + 0.0741F * ENTERED_DENS_BASE - 
		                     0.0575F * GOST_Comp_Adj[flow_no][6] - 0.063F * GOST_Comp_Adj[flow_no][5];
      
	      /* (24), GOST 30319.1-96 */
	      CONTRACTZ     = CONTRZ_FST = 1.0F - locZBase * locZBase;  
	      /*== calculate the Contract Density ?== the user relies on
	           his input, but we have an extra field that can be used ==*/
	      CalcBaseDensity();     
       /* (52), GOST 30319.1-96 */
	      HVAL_SUP  = 92.819F * (0.51447F * ENTERED_DENS_BASE + 
		                 0.05603F - 0.65689F * GOST_Comp_Adj[flow_no][6] -
						             GOST_Comp_Adj[flow_no][5]);
	      /* (53), GOST 30319.1-96 */
       HVAL_INF  = 85.453F * (0.5219F * ENTERED_DENS_BASE + 
		                 0.04242F - 0.65197F * GOST_Comp_Adj[flow_no][6] - 
						             GOST_Comp_Adj[flow_no][5]);
       SQUARED_N2  = GOST_Comp_Adj[flow_no][5] * GOST_Comp_Adj[flow_no][5];
	      CUBED_N2    = SQUARED_N2 * GOST_Comp_Adj[flow_no][5];
	      SQUARED_CO2 = GOST_Comp_Adj[flow_no][6] * GOST_Comp_Adj[flow_no][6];
							CUBED_CO2   = SQUARED_CO2 * GOST_Comp_Adj[flow_no][6];
							PROD_N2_CO2 = GOST_Comp_Adj[flow_no][5] * GOST_Comp_Adj[flow_no][6];

        ISENTREXP = 1.556 * (1+0.074 * GOST_Comp_Adj[flow_no][5]) -
	               /*(3.9/10000.0)*/ 
				          0.00039 * TF_AVERAGE * (1.0-0.68 * GOST_Comp_Adj[flow_no][5]) - 
				          0.208 * ENTERED_DENS_BASE +
              pow(STATICOVERTEMPK,1.43) * 
				         (384.0 * (1 - GOST_Comp_Adj[flow_no][5]) * 
				          pow(STATICOVERTEMPK,0.8)+26.4 * GOST_Comp_Adj[flow_no][5]);

   }
  /*== a return value still needed, VNIC has more to initialize ==*/	 
  return (ret_value);

}
/****************************************************************************/
/*      SYNTAX: void CalcBaseDensity( void )                        		*/
/* DESCRIPTION: calculates an informal base density, checks also user input */
/* CALLING SEQ: CalcBaseDensity ()                                          */
/*   ARGUMENTS: NAME        TYPE      DESCRIPTION                           */
/*              ---------   -------   -----------------------------------   */
/*     RETURNS: none                                                        */
/*       NOTES:	- sets or clears appropiate bits                            */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
void CalcBaseDensity ( void )
{
	FLOAT baseTinK;

	baseTinK = BASETEMP + 273.15; 
	/*== nobody ever can stop an user to do stupid things ==*/
	if(ENTERED_DENS_BASE <= 0.1 )
		ENTERED_DENS_BASE = 0.666;
		
    if(baseTinK >0.0 && CONTRACTZ > 0.0)
       BASEDENSITY = BASEPRESS * MOLWEIGHT / 
		             CONTRACTZ / GAS_CONST/ baseTinK;
	else 
	   BASEDENSITY = ENTERED_DENS_BASE;

	CONTRDENS_FST = BASEDENSITY;
}
/****************************************************************************/
/*      SYNTAX: void CheckGOSTAlarms( void )                        		      */
/* DESCRIPTION: checks alarms in a flow calculation, including GOST alarms  */
/* CALLING SEQ: CheckGOSTAlarms ()                                          */
/*   ARGUMENTS: NAME        TYPE      DESCRIPTION                           */
/*              ---------   -------   -----------------------------------   */
/*     RETURNS: none                                                        */
/*       NOTES:	- sets or clears appropiate bits                            */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
void CheckGOSTAlarms ( void )
{
	UINT8 alarm_no[2];
	UINT8 detail[14];
 UINT8 GOSTAlmCondition, i;
/*	FLOAT last12, *GOSTcomp;*/

    memcpy(&detail[0], (INT8 *)&TAG7, 10 );               /* Point ID */
    memcpy( (UINT8*)&detail[10],(UINT8*)&FLOWRATEDAILY,4);/* Flow Rate */
/*-----------------------------------------------------------------------
 *		MANUAL Mode Alarm.
 *		Checks if any of the inputs for meter, pressure, density
 *		or temperature are set for manual entry.  If any is set to manual,
 *		switchs the alarm mode  bit 7 to a 1.
 *
 *		Note: Specific logging to alarm database is NOT done
 *----------------------------------------------------------------------*/

		if (( DPTYPE == 0) || (P1TYPE == 0) || (T1TYPE ==0 ) )
			 ALMCODE |= 0x80; /* Set bit7 to 1 */
		else  
			 ALMCODE &= 0x7F; /* Set bit7 to 0 */

/* ----------------------------------------------------------------------
 *	LOW FLOW Alarm ( the current flow is less than low flow limit), then:
 *		1) Entry into LOW Alarm Status procedure
 *		2) Clear LOW Alarm Status procedure
 *----------------------------------------------------------------------*/
		if( (FLOWRATEDAILY < LOWALARMVAL ) && 
			  ( FLOWRATEDAILY != 0.0) && 
			    ( !(ALMCODE & 0x01)) && 
			      (LOW_FLOW_FLAG == ALM_CLEAR ))
		{
/*-----------------------------------------------------------------------  
 *      LOW alarm occurs, entry alarm status procedure  
 *----------------------------------------------------------------------*/
			if (EnRBXonSet)            /* if bit 6 is set for RBX on set */
				alarm_no[0] = 0x65;    /* RBX and log to database*/
			else
				alarm_no[0] = 0x61;    /* log to Alarm log database */
	
			alarm_no[1] = 0x00;                 /* "Low Alarm" */
		    log_alarm(alarm_no[0],alarm_no[1], &detail[0]);
			ALMCODE |= 0x01;           /* set low flow alarm, bit 6 to 1 */
			LOW_FLOW_FLAG = ALM_LOGGED;/* Set Low flow flag */
		}
/*-----------------------------------------------------------------------
 *   LOW flow alarm condition clears, exit alarm status procedure 
 *----------------------------------------------------------------------*/
		if ((FLOWRATEDAILY > LOWALARMVAL ) && ( ALMCODE & 0x01))
		{
			if(EnRBXonClear)                 /* bit 5 set for RBX on clear */
				alarm_no[0] = 0x64;          /* RBX and log to database*/
			else
				alarm_no[0] = 0x60;          /* log to database */
	
			alarm_no[1] = 0x00;              /* "Low Alarm" */
		    log_alarm(alarm_no[0],alarm_no[1], &detail[0]);
			ALMCODE &= 0xFE;                 /* Clear low flow alarm, bit 0 */
			LOW_FLOW_FLAG = ALM_CLEAR;       /* Clear low flow flag */

		}

/* ------------------------------------------------------------------------
 *	HIGH Flow Alarm ( the current flow is higher the high alarm limit), then:
 *		1) entry HIGH Alarm Status procedure
 *		2) Clear HIGH alarm Status procedure
 *-------------------------------------------------------------------------*/
		if (( FLOWRATEDAILY > HIALARMVAL ) &&  
			  ( !(ALMCODE & 0x04)) &&
			    ( HI_FLOW_FLAG == ALM_CLEAR) )
		{
/*--------------------------------------------------------------------------
 *  HIGH Alarm condition occurs, entry Alarm Status Procedure
 *-------------------------------------------------------------------------*/
			if (EnRBXonSet)          /* if bit 6 is set for RBX on set */
				alarm_no[0] = 0x65;  /* RBX and log to database */
			else
				alarm_no[0] = 0x61;  /* Set for log to database */
	
			alarm_no[1] = 0x02;                   /* "High Alarm" */
		    log_alarm(alarm_no[0],alarm_no[1], &detail[0]);
			ALMCODE |= 0x04;          /* Set high flow alarm, bit 2 */
			HI_FLOW_FLAG = ALM_LOGGED;/* Set high flow flag */
		}
/*--------------------------------------------------------------------------
 *    HIGH alarm condition clears, exit alarm status procedure
 *-------------------------------------------------------------------------*/
		if ( ( FLOWRATEDAILY < HIALARMVAL) && ( ALMCODE & 0x04) )
		{
			if(EnRBXonClear)  /* bit 5 set for RBX on clear */
				alarm_no[0] = 0x64;           /* RBX and log to database*/
			else
				alarm_no[0] = 0x60;           /* log to database */
	
			alarm_no[1] = 0x02;               /* "High Alarm" */
		    log_alarm(alarm_no[0],alarm_no[1], &detail[0]);
			ALMCODE &= 0xFB;                  /* Clear high flow alarm, bit 2 */
			HI_FLOW_FLAG = ALM_CLEAR;         /* Clear high flow flag */
		}

}
	

/****************************************************************************/
/*                                                                          */
/* SYNTAX:      void calc_crc (UINT8 *, UINT16)                             */
/*                                                                          */
/* DESCRIPTION: Calculates CRC16 for message passed and returns CRC value   */
/*              port.                                                       */
/*                                                                          */
/* CALLING SEQ: crc = calc_crc (message_ptr, message_len);                  */
/*                                                                          */
/* ARGUMENTS:   NAME         TYPE      DESCRIPTION                          */
/*              ----         ----      --------------------------------     */
/*              message_ptr  UINT8 *   Pointer to beginning of message      */
/*              num_bytes    UINT16    Number of bytes in message           */
/*                                                                          */
/* RETURNS:                                                                 */
/*                                                                          */
/* NOTES:                                                                   */
/*                                                                          */
/****************************************************************************/

UINT16 calc_crc (UINT8 *message_ptr, UINT16 num_bytes)
  {
  UINT16 i;
  UINT16 temp_crc;
  
  temp_crc = 0;

  for (i=0; i<num_bytes; i++)
    {
    temp_crc = (temp_crc >> 8) ^ 
      *(UINT16 *)(Crc_table + ((temp_crc ^ (UINT16)(*message_ptr++)) & 0x00ff));
    }

  return temp_crc;
  }  /* end of calc_check () */

/****************************************************************************/
/*      SYNTAX: UINT8 compare_floats( FLOAT a, FLOAT b )              		*/
/* DESCRIPTION: compares two floats because of the floating point processor */
/* CALLING SEQ: compare_floats ( a, b)                                      */
/*   ARGUMENTS: NAME        TYPE      DESCRIPTION                           */
/*              ---------   -------   -----------------------------------   */
/*              a           FLOAT     first FLOAT                           */
/*              b           FLOAT     second FLOAT                          */
/*     RETURNS: 0 if equals, 1 if different                                 */
/*       NOTES:	according to memory-speed optimum it's used alternate with  */
/*              COMPARE_FLOATS macro                                        */
/*                                                                          */
/****************************************************************************/

UINT8 compare_floats(FLOAT a, FLOAT b)
{
	if( fabs(a-b)<0.0001 ) 
		return (0);
	else
		return (1);
}

/****************************************************************************/
/*      SYNTAX: unsigned int system_ticks (void)                            */
/* DESCRIPTION: Returns User Ticks according to wich ROC is defined         */
/* CALLING SEQ: system_ticks ();                                            */
/*     RETURNS: None                                                        */
/****************************************************************************/
UINT16 system_ticks (void)
{
	#if defined (ROC3FLS)
		return( *(unsigned int *)Config->ten_ms_ticks);
	#endif
	#if defined (FLOBOSS)
		return( *(unsigned int *)Config->user_ticks);
	#endif

}

/*======================= END OF PROGRAM ==================================*/