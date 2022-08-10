;****************************************************************************/
;* U_INTS.ASM     Assembly Language Routines for User Software              */
;*                                                                          */
;*  Copyright (c)  xxxx  Fisher Controls International Incorporated         */
;*                                                                          */
;*   CONFIDENTIAL                                                           */
;* This program is the poperty of Fisher Controls.  Its configuration and   */
;* algorithms are confidential and may not be used, reproduced, or revealed */
;* to others except in accord with contract or other written consent of     */
;* Fisher Controls.  Any permitted reproduction, in whole or in part, must  */
;* bear this legend.                                                        */
;*                                                                          */
;****************************************************************************/
;* (file description)    The file description section should contain a      */
;* brief description of the file and include enough information for someone */
;* to understand what the purpose of the file is and what functionality is  */
;* contained in it. It does not have to contain a detailed design           */
;* description although any information can be included. Authors should use */
;* their own judgement as to the type of information that will be useful.   */
;****************************************************************************/
;* (version control header and history)  If you are using a version control */
;* system, include the archive information and revision history. If you are */
;* not using a version control system, include a few comments along with    */
;* your initials and the date and time in this section each time you modify */
;* the file.                                                                */
;****************************************************************************/

codeseg segment para    public  'code'
codeseg ends

dataseg segment word    public  'data'
dataseg ends

     assume  cs:codeseg, ds:dataseg, es:dataseg, ss:dataseg

;    public  init_vector_

;    extrn   Com_buf_  : byte 
;    extrn   Com2_buf_ : byte 

;****************************************************************************

dataseg	segment	word    public  'data'

dataseg	ends
;****************************************************************************

codeseg	segment

;       extrn    com1_handler_ : far 
;       extrn    com2_handler_ : far 

;****************************************************************************
; 
; This data array will appear at the start of the code segment and must be
; placed before any assembly code in this file.  It is used by the ROC300
; operating system to check for an installed user program.  The first two
; lines are mandatory and the text must match exactly.  The third and forth
; lines are used for program description and version, and will be displayed
; using GV101 to view installed user programs.  An example for a COMM1,
; COMM2, and user program are shown below.

; for a COMM1 user program, code seg: B000, data seg: A000
;       db    'A','l','l','o','c','a','t','e','d',' ','L','o','a','d',' ',' '
;       db      2,  8,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
;       db    'U','s','e','r',' ','C',' ','E','x','a','m','p','l','e','1',' '
;       db    'A','l','p','h','a','0','1',' ',' ',' ','1','0','1','1','9','5'

; for a COMM2 user program, code seg: B800, data seg: A800
;       db    'A','l','l','o','c','a','t','e','d',' ','L','o','a','d',' ',' '
;       db      2, 10,  2,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
;       db    'U','s','e','r',' ','C',' ','E','x','a','m','p','l','e','1',' '
;       db    'A','l','p','h','a','0','1',' ',' ',' ','1','0','1','1','9','5'

; for a user program, code seg: c000, data seg: b000
        db    'A','l','l','o','c','a','t','e','d',' ','L','o','a','d',' ',' '
        db      1,  8,  1,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        db    'S','y','n','c','h','r','o','/','P','r','o','c',' ',' ',' ',' '
        db    'W','6','8','0','0','X','X','1','2',' ','V','e','r','1','.','0'

; The following lines are used to insert a copyright notice in the code.  You
; may use as many lines as needed.
;       db    'C','o','p','y','r','i','g','h','t',' ','(','C',')',' ',' ',' '
;       db    'F','i','s','h','e','r',' ','C','o','n','t','r','o','l','s',' '
;       db    '1','9','9','_',' ','F','i','e','l','d',' ','A','u','t','o','m'
;       db    'a','t','i','o','n',' ','S','y','s','t','e','m','s',' ',' ',' '

;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;Detailed Explanation/Description:
;
; For a user program, code seg: 7000, data seg: 6000
;
;       db    'A','l','l','o','c','a','t','e','d',' ','L','o','a','d',' ',' '
;
;             "Allocated Load  "
;
;       db     4,0,4,4,0,0,0,0,0,0,0,0,0,0,0,0
;
;              ^ ^ ^ ^ ^                     ^
;              | | | | |_____________________|
;              | | | |           |
;              | | | |           |__ Reserved (Always 0)
;              | | | |
;              | | | |__ Task Assignment (See below)
;              | | |____ Number of 16K data blocks 
;              | |______ Starting data block (See Chart Below)
;              |________ Number of 16K code blocks 
;
;     Starting Data Blocks                    Task Assignments
;     -------------------------------         ---------------------
;      0  6000:0000     12  B000:0000         0 COM1 Port
;      1  6400:0000     13  B400:0000         1 COM2 Port
;      2  6800:0000     14  B800:0000         2 Local Operator Port
;      3  6C00:0000     15  BC00:0000         3 LCD Display
;      4  7000:0000     16  C000:0000         4 User 1 Task
;      5  7400:0000     17  C400:0000         5 User 2 Task
;      6  7800:0000     18  C800:0000         6 User 3 Task
;      7  7C00:0000     19  CC00:0000         7 User 4 Task
;      8  A000:0000     20  D000:0000
;      9  A400:0000     21  D400:0000
;     10  A800:0000     22  D800:0000
;     11  AC00:0000     23  DC00:0000
;
;       db    'P','r','o','g','r','a','m',' ','N','a','m','e',' ',' ',' ',' '
;       db    'W','6','8','0','?','?','X','0','0','1','2',' ','1','.','0','0'
;
;              ^                                       ^       ^           ^
;              |_______________________________________|       |___________|
;                                |                               |    
;                                |__ Part Number                 |__ Version
;
;~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;Note:  You must also change the link script to match the code and data 
;       sections!  The HEX86 program must also match the number of blocks.
;****************************************************************************
;com1    proc    far                ; service routine for comm1 port interrupt
;
;        push    ax                 ; save registers
;        push    bx		
;        push    cx
;        push    dx
;        push    ds
;        push    es
;        push    bp
;        push    si
;        push    di
;
;        mov     ax, seg com_buf_   ;prepare the data segment for call
;        mov     ds, ax
;
;        call    com1_handler_      ;call c language interrupt handler      
;
;        pop     di                 ; restore registers
;        pop     si
;        pop     bp
;        pop     es
;        pop     ds
;        pop     dx
;        pop     cx
;        pop     bx
;        pop     ax
;
;        db      0fh                ;same as FINT
;        db      92h
;
;        iret
;
;com1    endp
;****************************************************************************
;com2    proc    far                ; service routine for comm1 port interrupt
;
;        push    ax                 ; save registers
;        push    bx		
;        push    cx
;        push    dx
;        push    ds
;        push    es
;        push    bp
;        push    si
;        push    di
;
;        mov     ax, seg com2_buf_  ;prepare the data segment for call
;        mov     ds, ax
;
;        call    com2_handler_      ;call c language interrupt handler      
;
;        pop     di                 ; restore registers
;        pop     si
;        pop     bp
;        pop     es
;        pop     ds
;        pop     dx
;        pop     cx
;        pop     bx
;        pop     ax
;
;        db      0fh                ;same as FINT
;        db      92h
;
;        iret
;
;com2    endp
;****************************************************************************
;init_vector_   proc   far 
;
;        push    ax                 ; save registers
;        push    bx		
;        push    cx
;        push    dx
;        push    ds
;        push    es
;        push    bp
;        push    si
;        push    di
;
;        cli                        ; disable interrupts
;
;
; INTERRUPT VECTORS INITIALIZATION:
;
;        cld                        ; clear direction flag
;        sub     ax, ax
;        mov     es, ax
;
;        mov     di, 24*4           ; address for int type 24 (INTP0, comm 1) 
;        mov     ax, offset com1
;        stosw                      ; move contents of ax to offset in di
;        mov     ax, seg com1
;        stosw
;
;        mov     di, 25*4           ; address for int type 25 (INTP1, comm 2) 
;        mov     ax, offset com2
;        stosw                      ; move contents of ax to offset in di
;        mov     ax, seg com2
;        stosw
;
;        pop     di                 ; restore registers
;        pop     si
;        pop     bp
;        pop     es
;        pop     ds
;        pop     dx
;        pop     cx
;        pop     bx
;        pop     ax
;
;        sti
;        ret
;init_vector_     endp
;****************************************************************************

codeseg ends
