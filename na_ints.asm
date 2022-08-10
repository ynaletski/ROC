;========================================================================
;
;     Memory allocation Specific for:  GOST Program
;
;	u_ints.asm Initialization File
;   gost_mn      = user program name (GOST main)
;   1.10         = revision 

;  FBRAM  = 7000 to 7FFF RAM target for code, 6800 to 6FFF for data
;  Makefile line:
;  FBRAM  = -C 7000 -D 6800 -t -o gost407a.exe gost_fb gost407a $(START)

;  FL3AB  = A800 to B3FF RAM target for code, A000 to A7FF for data
;  Makefile line:
;  FL3AB  = -C A800 -D A000 -t -o gost300a.exe gost_300 gost300a $(START)
;========================================================================


dataseg  segment  word    public  'data'

;============================================================================
;            User Defined Points and Ports
;=============================================================================
;public	udp_
;udp_	db 27,28             ; User Defined Points  
; public port_
; port_	 dw 1                ; port 0=OpPort, 1=COMM1, 2=COMM2, if necessary
;==============================================================================

dataseg  ends

codeseg  segment

ifdef GOST407A
	db    'A','l','l','o','c','a','t','e','d',' ','L','o','a','d',' ',' '
	db     4 ; ---> Number of 16K code blocks
	db     2 ; ---> Starting data block (See Chart Below)
	db     1 ; ---> Number of 16K data blocks
	db     6 ; ---> Task Assignment (User 3)
	db     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0   ; it should be 0
	db    'F','i','s','h','e','r',' ','M','o','s','c','o','w',' ',' ',' '
	db    'G','O','S','T',' ','3','.','0','2',' ','F','e','b','0','1',' '
endif

ifdef GOST300A
	db    'A','l','l','o','c','a','t','e','d',' ','L','o','a','d',' ',' '
	db     4 ; ---> Number of 16K code blocks
	db     8 ; ---> Starting data block (See Chart Below)
	db     1 ; ---> Number of 16K data blocks
	db     6 ; ---> Task Assignment (User 3)
	db     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0   ; it should be 0
	db    'F','i','s','h','e','r',' ','M','o','s','c','o','w',' ',' ',' '
	db    'G','O','S','T',' ','3','.','0','2',' ','F','e','b','0','1',' '
endif

ifdef GOST407B
	db    'A','l','l','o','c','a','t','e','d',' ','L','o','a','d',' ',' '
	db     4 ; ---> Number of 16K code blocks
	db     1 ; ---> Starting data block (See Chart Below)
	db     1 ; ---> Number of 16K data blocks
	db     6 ; ---> Task Assignment (User 3)
	db     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0   ; it should be 0
	db    'F','i','s','h','e','r',' ','M','o','s','c','o','w',' ',' ',' '
	db    'G','O','S','T',' ','3','.','0','2',' ','F','e','b','0','1',' '
endif

ifdef GOST300B
	db    'A','l','l','o','c','a','t','e','d',' ','L','o','a','d',' ',' '
	db     4 ; ---> Number of 16K code blocks
	db    14 ; ---> Starting data block (See Chart Below)
	db     1 ; ---> Number of 16K data blocks
	db     6 ; ---> Task Assignment (User 3)
	db     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0   ; it should be 0
	db    'F','i','s','h','e','r',' ','M','o','s','c','o','w',' ',' ',' '
	db    'G','O','S','T',' ','3','.','0','2',' ','F','e','b','0','1',' '
endif

ifdef GOST300C
	db    'A','l','l','o','c','a','t','e','d',' ','L','o','a','d',' ',' '
	db     4 ; ---> Number of 16K code blocks
	db    10 ; ---> Starting data block (See Chart Below)
	db     1 ; ---> Number of 16K data blocks
	db     6 ; ---> Task Assignment (User 3)
	db     0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 0   ; it should be 0
	db    'F','i','s','h','e','r',' ','M','o','s','c','o','w',' ',' ',' '
	db    'G','O','S','T',' ','3','.','0','2',' ','F','e','b','0','1',' '
endif

codeseg ends

;=========================================================================|
;																		                                                       |
;	MEMORY MAP & TASK ASSIGNEMENT CONVENTION 							                        |
;																		                                                       |
;     Code   Starting Data Blocks             Task Assignments            |
;            (segment:offset)											                                  |
;     ---------------------------------     ---------------------         |
;      0 = 6000:0000     12 = B000:0000       0 = COM1 Port               |
;      1 = 6400:0000     13 = B400:0000       1 = COM2 Port               |
;      2 = 6800:0000     14 = B800:0000       2 = Local Operator Port     |
;      3 = 6C00:0000     15 = BC00:0000       3 = LCD Display             |
;      4 = 7000:0000     16 = C000:0000       4 = User 1 Task             |
;      5 = 7400:0000     17 = C400:0000       5 = User 2 Task             |
;      6 = 7800:0000     18 = C800:0000       6 = User 3 Task             |
;      7 = 7C00:0000     19 = CC00:0000       7 = User 4 Task             |
;      8 = A000:0000     20 = D000:0000									                          |
;      9 = A400:0000     21 = D400:0000									                          |
;     10 = A800:0000     22 = D800:0000									                          |
;     11 = AC00:0000     23 = DC00:0000									                          |
;																		                                                       |	
;=========================================================================|
