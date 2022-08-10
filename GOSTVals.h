/****************************************************************************/
/* GOSTVALS.H      GOST Values Header File                                  */
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
/*  Ver 1.03 ( alpha), Jan 2000                                             */                                                                
/****************************************************************************/
/* file modified for Annubar and wet gas                                    */    
/*  Ver 1.07  ( alpha), July 2000                                            */                                                                
/****************************************************************************/

#ifndef GOSTVALS_H
#define GOSTVALS_H

/*===================  Macros, messages, units ==============================*/

#define EPS             (FLOAT)0.00001
#define COMPARE_FLOATS(A,B) (( (((A)-(B)) <  0.0001) && \
                               (((A)-(B)) > -0.0001) )? (0):(1))
/* output = 0 for two (almost) equal floats, = 1 for two different floats */
#define MAX(A,B)			(( (A) >= (B) ) ? (A) : (B))
#define MAX_INDEX(A,B)		(( (A) >= (B) ) ? (0) : (1))
/* Reynolds Calc */
#define RE_CALC(A,B,C)		( (A)/((0.785398163)*(B)*(C)) )
/* constants */
const FLOAT qPI			=   0.785398163;
const FLOAT GAS_CONST   =   8.31451;
/*== we can set Ntpl to 18 because the other components don't
     show up in the interface; by hardcoding we save a jump  =====*
const UINT8 Ntpl        =  18;                        *29;*/  

const UINT8 Sl[8]       = {10,10,10,9,6,5,3,1};
/*const INT8  GOSTStr[4]  = "GOST";*/

/*===================  Initialized constants ==============================*/

FLOAT  Xij[8][8]=  {
       {    0, 0.036, 0.076, 0.121, 0.129,  0.06, 0.074, 0.089},
       {0.036,     0,     0,     0,     0, 0.106, 0.093, 0.079},
       {0.076,     0,     0,     0,     0,     0,     0,     0},
       {0.121,     0,     0,     0,     0,     0,     0,     0},
       {0.129,     0,     0,     0,     0,     0,     0,     0},
       { 0.06, 0.106,     0,     0,     0,     0, 0.022, 0.211},
       {0.074, 0.093,     0,     0,     0, 0.022,     0, 0.089},
       {0.089, 0.079,     0,     0,     0, 0.211, 0.089,     0}};

          /* 4 binar relations table (p.23) */
FLOAT  Lij[8][8]=   {
       {     0,-0.074,-0.146,-0.258,-0.222,-0.023,-0.086,     0},
       {-0.074,     0,     0,     0,     0,     0,     0,     0},
       {-0.146,     0,     0,     0,     0,     0,     0,     0},
       {-0.258,     0,     0,     0,     0,     0,     0,     0},
       {-0.222,     0,     0,     0,     0,     0,     0,     0},
       {-0.023,     0,     0,     0,     0,     0,-0.064,     0},
       {-0.086,     0,     0,     0,     0,-0.064,     0,-0.062},
       {     0,     0,     0,     0,     0,     0,-0.062,     0}};

/*===========
const FLOAT Mr[20] = * Molar Mass, Fisher ROC order *
  {
   28.0135,   *  Nitrogen          N2    0 *
   44.0100,   *  Carbon dioxide   CO2    1 *
   34.0820,   *  Hydrogen sulfide H2S    2 *
   18.0153,   *  Water            H2O    3 *
    4.0026,   *  Helium            He    4 *
   16.0430,   *  Methane          CH4    5 *
   30.0700,   *  Ethane          C2H6    6 *
   44.0970,   *  Propane         C3H8    7 *
   58.1230,   *  n-butane       C4H10    8 *
   58.1230,   *  iso-butane     C4H10    9 *
   72.1500,   *  n-pentane      C5H12   10 *
   72.1500,   *  iso-pentane    C5H12   11 *
   86.1770,   *  n-hexane       C6H14   12 *
  100.2040,   *  n-heptane      C7H16   13 *
  114.2310,   *  n-octane       C8H18   14 *
  128.2580,   *  n-nonane       C9H20   15 *
  142.2850,   *  n-decane      C10H22   16 *
   31.9988,   *  Oxygen            O2   17 *
   28.0100,   *  Carbon monoxide   CO   18 *
    2.0159    *  Hydrogen          H2   19 *
  };
 */
/*=== Fisher Template and GOST list have 18 common
      components. All internal calcs refer to GOST
	  list =======================================*/

const FLOAT GOST_Mi[18]=         
  {
	16.043,    /* methane rank  ( 5 Fisher)   0 */ 
    30.07,     /* ethane rank   ( 6 Fisher)   1 */
    44.097,    /* propane rank  ( 7 Fisher)   2 */
    58.123,    /* n_butane rank ( 8 Fisher)   3 */
    58.123,    /* i_butane rank ( 9 Fisher)   4 */
    28.0135,   /* nitrogen rank ( 0 Fisher)   5 */
    44.010,    /* Co2 rank      ( 1 Fisher)   6 */
    34.082,    /* H2s rank      ( 2 Fisher)   7 */
    72.15,     /* n_pentane rank(10 Fisher)   8 */
    72.15,     /* i_pentane rank(11 Fisher)   9 */
    86.177,    /* hexane rank   (12 Fisher)  10 */
   100.204,    /* heptanes rank (13 Fisher)  11 */
   114.231,    /* octanes rank  (14 Fisher)  12 */
    18.0153,   /* water rank    ( 3 Fisher)  13 */
     4.0026,   /* helium rank   ( 4 Fisher)  14 */
    31.9988,   /* oxygen rank   (17 Fisher)  15 */
    28.01,     /* crbn mono rank(18 Fisher)  16 */
     2.0159    /* hydrogen rank (19 Fisher)  17 */
};
/*
   26.038,
   28.054, 
   42.081, 
   72.114,
   92.141,
   17.0306,  
   34.042, 
   48.109,
   64.065,
   20.1797,
   39.948,
   28.9626
  };
  */

const FLOAT GOST_isentr_i[18]=         
  {
    1.31,    /* methane rank  ( 5 Fisher)   0 */ 
    1.19,    /* ethane rank   ( 6 Fisher)   1 */
    1.33,    /* propane rank  ( 7 Fisher)   2 */
    1.09,    /* n_butane rank ( 8 Fisher)   3 */
  1.1,          /* i_butane rank ( 9 Fisher)   4 */
    1.40,    /* nitrogen rank ( 0 Fisher)   5 */
    1.28,    /* Co2 rank      ( 1 Fisher)   6 */
    1.32,    /* H2s rank      ( 2 Fisher)   7 */
    1.09,    /* n_pentane rank(10 Fisher)   8 */
    1.06,    /* i_pentane rank(11 Fisher)   9 */
    1.08,    /* hexane rank   (12 Fisher)  10 */
 1.07,          /* heptanes rank (13 Fisher)  11 */
 1.07,          /* octanes rank  (14 Fisher)  12 */
 1.40,          /* water rank    ( 3 Fisher)  13 */
    1.66,    /* helium rank   ( 4 Fisher)  14 */
    1.40,    /* oxygen rank   (17 Fisher)  15 */
    1.41,    /* crbn mono rank(18 Fisher)  16 */
    1.40     /* hydrogen rank (19 Fisher)  17 */
};
const FLOAT hv_inf[18] =

{ /* Note: gas composition is listed in the  sequence 
     the values are stored in memory.
	 Source: GOST
  */
/* 6*/           33.37,         /* Methane C1           GOST  0*/
/* 7*/           59.39,         /* Ethane  C2           GOST  1*/
/* 8*/           84.94,         /* Propane C3           GOST  2*/
/* 9*/          110.50,         /* N-Butane  NC4        GOST  3*/
/*10*/          110.10,         /* IsoButane IC4        GOST  4*/
/* 1*/            0.,           /* Nitrogen  N2         GOST  5*/        
/* 2*/            0.,           /* Carbon Dioxide Co2   GOST  6*/  
/* 3*/           21.53,         /* Hydrogen Sulfide H2S GOST  7*/
/*11*/          136.00,         /* N-Pentane  NC5       GOST  8*/
/*12*/          135.70,         /* IsoPentane IC5       GOST  9*/
/*13*/          161.60,         /* Hexane  C6           GOST 10*/
/*14*/          187.10,         /* Heptane C7           GOST 11*/
/*15*/          212.70,         /* octane               GOST 12*/
/* 4*/            0.,           /* Water H2O            GOST 13*/
/* 5*/            0.,           /* Helium He            GOST 14*/
/*18*/            0.,           /* oxygen               GOST 15*/
/*19*/           11.76,         /* carb mono            GOST 16*/
/*20*/           10.05          /* hydrog               GOST 17*/
/*16*           212.70,          * nonane               GOST --*/
/*17*           212.70,          * decane               GOST --*/

};

const FLOAT hv_sup[18] =

{ /* Note: gas composition is listed in the  sequence 
     the values are stored in memory.
	 Source: GOST
  */
/* 6*/           37.04,         /* Methane C1 */
/* 7*/           64.91,         /* Ethane  C2 */
/* 8*/           92.29,         /* Propane C3 */
/* 9*/          119.70,         /* N-Butane  NC4 */
/*10*/          119.30,         /* IsoButane IC4 */
/* 1*/            0.,           /* Nitrogen  N2 */        
/* 2*/            0.,           /* Carbon Dioxide Co2 */  
/* 3*/           23.37,         /* Hydrogen Sulfide H2S */
/*11*/          147.00,         /* N-Pentane  NC5 */
/*12*/          146.80,         /* IsoPentane IC5 */
/*13*/          174.50,         /* Hexane  C6 */
/*14*/          201.80,         /* Heptane C7 */
/*15*/          229.20,         /* octane  */
/* 4*/            0.,           /* Water H2O */
/* 5*/            0.,           /* Helium He */
/*18*/            0.,           /* oxygen*/
/*19*/           11.76,         /* carb mono */
/*20*/           11.89          /* hydrog*/
/*16*           229.20,          * nonane*/
/*17*           229.20,          * decane */

};

const FLOAT Roki[8]=        /* */
  {163.03, 205.53, 218.54, 226.69, 225.64, 315.36, 466.74, 349.37 };

const FLOAT Tki[8]=         /*, K */
  {190.67, 305.57, 369.96, 425.40, 407.96, 125.65, 304.11, 373.18 };

const FLOAT Roci[8]=        /* */
  {0.6682, 1.2601, 1.8641, 2.4956, 2.488, 1.1649, 1.8393, 1.4311};

const FLOAT Oi[8]=          /* */
  {0.0006467, 0.1103, 0.1764, 0.2213, 0.2162, 0.04185, 0.2203, 0.042686};


const FLOAT Tki_kvong[18]=        /*1 GOST 303119.1-96  */
  {190.555, 305.83, 369.82, 425.14, 408.13,    126.2, 304.2, 373.2,
   469.69,  460.39, 506.4,  539.2,  568.4,
   647.14,    5.19, 154.58, 132.85,  33.2
   };

const FLOAT Pki_kvong[18]=        /*1 GOST 303119.1-96  */
  {4.5988, 4.88,  4.25,  3.784, 3.648,    3.39, 7.386, 8.940,
   3.364,  3.381, 3.03,  2.74,  2.49,
   22.064, 0.227, 5.043, 3.494, 1.297
   };

const FLOAT Roc_i_i[30]=        /*1 GOST 303119.1-96  */
  {0.66692, 1.25004, 1.83315, 2.41623, 2.41623,    1.16455, 1.82954, 1.41682,
   2.99934, 2.99934, 3.58246, 4.16558, 4.74869,
     0.74891, 0.16639, 1.33022, 1.1644, 0.083803,
   1.08243, 1.16623, 1.74935, 3.24727, 3.83039,
   0.70798, 1.41516,          1.99994, 2.66324,    0.83889, 1.66068, 1.204
  };

const FLOAT Bi_05[29]=      /* Bi^0.5  í1 GOST 303119.1-96 */
  {0.0436, 0.0894, 0.1288, 0.1783, 0.1703,        0.0173, 0.0728, 0.1,
   0.2345, 0.2168, 0.2846, 0.3521, 0.4278,
     0.2191,      0, 0.0265,   0.02, -0.0051,
   0.0837, 0.0775, 0.1225, 0.2530, 0.3286,
   0.1049, 0.3286,         0.1483, 0.1414,             0, 0.0265
  };

const UINT8    N1i[8] = { 10,  6,  6,  6,  5,  6,  6,  5};
const UINT8    N2i[8] = {  6,  5,  4,  5,  2,  6,  4,  5};
const UINT16   Tni[8] = {100,100,100,100,300,100,300,100};


struct material
{
 UINT8  name[10];
 FLOAT  A,B,C;

} ST[3/*25 - all posible materials, 3 only implemented */] = 

{
/*	{"8         ",10.9e-6,  7.7e-9,  2.4e-12},
	{"10        ",10.8e-6,  9.0e-9,  4.2e-12},
	{"15        ",11.1e-6,  7.9e-9,  3.9e-12},
	{"15M       ",10.7e-6, 13.0e-9, 13.0e-12},
	{"16M       ",11.1e-6,  8.4e-9,  3.7e-12},
	{"20M       ",10.7e-6, 13.0e-9, 13.0e-12},
	{"25        ",12.0e-6,  0.0e-9,  0.0e-12},
	{"30        ",10.2e-6, 10.4e-9,  5.6e-12},
	{"35        ",10.2e-6, 10.4e-9,  5.6e-12},
	{"X6CM      ",10.1e-6,  2.7e-9,  0.0e-12},
	{"X7CM      ",10.1e-6,  2.7e-9,  0.0e-12},
	{"12MX      ",11.3e-6,  3.8e-9,  0.0e-12},
	{"12X1M     ",10.0e-6,  9.6e-9,  6.0e-12}, */
	{"12X17     ", 9.4e-6,  7.4e-9,  6.0e-12},
	{"12X18H9T  ",15.6e-6,  8.3e-9,  6.5e-12},
	{"20        ",11.1e-6,  7.7e-9,  3.4e-12}
/*	{"12X18H10T ",15.6e-6,  8.3e-9,  6.5e-12},
	{"14X17H2   ", 9.4e-6,  7.5e-9,  7.8e-12},
	{"15XMA     ",11.1e-6,  8.5e-9,  5.2e-12},
	{"15X1M1    ",10.4e-6,  8.1e-9,  4.4e-12},
	{"15X5M     ",10.1e-6,  2.7e-9,  0.0e-12},
	{"15X12EHM  ", 9.8e-6,  3.0e-9,  0.0e-12},
	{"17X18H9   ",15.7e-6,  5.7e-9,  0.0e-12},
	{"20X23H13  ",15.5e-6,  1.0e-9,  0.0e-12},
	{"36X18H25C2",12.0e-6, 10.0e-9,  5.4e-12}  */
 };

const DOUBLE Alpha_ij[8][11]=
{
/*0*/	{1.46696186E+02,   -6.56744186E+01,  2.02698132E+01,
        -4.20931845E+00,    6.06743008E-01, -6.12623969E-02,
         4.30969226E-03,   -2.06597572E-04,  6.42615810E-06,
        -1.16805630E-07,    9.40958930E-10}  ,

/*1*/	{6.81209760E+01,   -3.06340580E+01,  9.52750290E+00,
        -1.69471020E+00,    1.76305850E-01, -9.95454020E-03,
         2.35364300E-04,    0.0,             0.0,
		 0.0,               0.0} ,

/*2*/	{-9.209726737E+01,  3.070930782E+01,-4.924017995E+00,
          5.045358836E-01, -3.140446759E-02, 1.076680079E-03,
         -1.556890669E-05,  0.0,             0.0,  
		  0.0,              0.0}  ,

/*3*/	{-2.096096482E+02,  6.877783535E+01,-1.228650555E+01,
          1.413691547E+00, -1.002920638E-01, 3.985571861E-03,
         -6.786460870E-05,  0.0,             0.0,
		  0.0,              0.0},

/*4*/	{-3.871419306E+01,  4.711104578E+01,-1.758225423E+01,
          4.183494309E+00, -5.520042474E-01, 3.034658409E-02,
		  0.0,              0.0,               0.0,
		  0.0,              0.0}     ,

/*5*/	{1.131290000E+01,  -2.159600000E+00, 3.527610000E-01,
        -3.217050000E-02,   1.676900000E-03,-4.679650000E-05,
         5.426030000E-07,   0.0,             0.0,
		 0.0,               0.0}     ,

/*6*/	{-9.508041394E-01,  7.008743711E+00,-3.505801670E+00,
          1.096778000E+00, -2.016835088E-01, 1.971024237E-02,
         -7.860765734E-04,  0.0,             0.0,
		  0.0,              0.0} ,

/*7*/	{3.913550000E+00,  -6.848510000E-02, 5.644240000E-02,
        -4.837450000E-03,  1.717820000E-04, -2.27537000E-06,
		 0.0,              0.0,              0.0,       
		 0.0,              0.0},
};

const DOUBLE Betta_ij[8][6]=
{
/*0*/
	{-2.09233731E+02, 2.06925203E+02, -1.35704831E+02,
      5.64368924E+01,-1.34496111E+01,  1.39664152E+00},
/*1*/
	{-8.74070840E+01, 7.84813740E+01, -4.48658590E+01,
      1.46543460E+01,-2.05183930E+00,  0.0},
/*2*/
	{1.748671280E+02,-1.756054503E+02, 8.874920732E+01,
    -1.720610207E+01, 0.0,             0.0},
/*3*/
	{4.055272850E+02,-4.457015773E+02, 2.743667350E+02,
    -8.643867287E+01, 1.070428636E+01, 0.0},
/*4*/
	{2.171601450E+01,-4.492603200E+00, 0.0, 
	 0.0,             0.0,             0.0},
/*5*/
	{-1.746540000E+01, 2.462050000E+01, -2.177310000E+01,
      1.164180000E+01,-3.421220000E+00, 4.222960000E-01},
/*6*/
	{ 1.087462263E+00,-7.976765747E-02,-2.837014896E-03,
      1.479612229E-04, 0.0,             0.0},
/*7*/
	{0.000000000E+00,  0.000000000E+00, 1.186580000E+00,
    -1.907470000E+00,  8.28520000E-01,  0.0}
} ;

      /* T-Å.1 GOST 30319.2-96*/

const FLOAT Alk[8][10]=
{ 
	{ 0.6087766, -0.4596885, 1.14934,  -0.607501,  -0.894094,
      1.144404,  -0.34579,  -0.1235682, 0.1098875, -0.0219306 } ,

	{-1.832916,   4.175759, -9.404549, 10.62713,   -3.080591,
     -2.122525,   1.781466, -0.4303578,-0.04963321, 0.0347496 },

	{ 1.317145, -10.73657,  23.95808, -31.47929,   18.42846,
     -4.092685,  -0.1906595, 0.4015072,-0.1016264, -0.009129047 },

	{-2.837908,  15.34274, -27.71885,  35.11413,  -23.485,
      7.767802,  -1.677977,  0.3157961, 0.004008579, 0.0 },

	{ 2.606878, -11.06722,  12.79987, -12.11554,   7.580666,
     -1.894086,   0.0,       0.0,       0.0,       0.0 },

	{-1.15575,    3.601316, -0.7326041,-1.151685,  0.5403439, },

	{ 0.09060572,-0.5151915,0.07622076, 0.0,       0.0,}        ,

	{ 0.04507142,}
};

const FLOAT Blk[8][10]=
{
	{  -0.7187864,  10.67179,  -25.7687,   17.13395,  16.17303,
      -24.38953,     7.156029,   3.350294, -2.806204,  0.5728541},

	{   6.057018,  -79.47685,  216.7887, -244.732,    78.04753,
       48.70601,   -41.92715,   10.00706,   1.237872, -0.8610273 },

	{ -12.95347,   220.839,   -586.4596,  744.4021, -447.0704,
       99.6537,      5.136013,  -9.5769,    2.41965,  -0.2275036},

	{  15.71955,  -302.0599,   684.5968, -828.1484,  560.0892,
     -185.9581,     39.91057,   -7.567516, -0.1062596, },

	{ -13.75957,   205.541,   -325.2751,  284.6518, -180.8168,
       46.05637, },

	{ 6.466081,    -57.3922,    36.94793,  20.77675, -12.56783, },

	{-0.9775244,     2.612338,  -0.4059629,  },

	{ -0.2298833, }
};

#endif