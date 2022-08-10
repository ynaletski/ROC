; startup assembly file
; linker will use location of $begin as reset jump location
;
   include lmacros.h
;
;       Do not set the stack segment or stack pointer since the program
;       will use the stack maintained by the operating system. Do not use
;       the stack for arrays since overrunning the stack is possible.
;       Use globals variables as much as possible.
;
;stacksize equ   512
;
;       dataseg is the data segment address (as a paragraph #)
;       this is picked from the -D option of the linker
;
;Dseg   equ     040H   ;physical addr 0x400, just above the int vectors

dataseg segment word public 'data'
;       bss     cstack:byte,stacksize
        public  $MEMRY
$MEMRY  dw      -1
        dw      -1
        public  errno_
errno_  dw      0
        public  _dsval_,_csval_
_dsval_ dw      0
_csval_ dw      0
        public  _mbot_, _sbot_
_mbot_  dw      0
        dw      0
_sbot_  dw      0
        dw      0
;
; The variable is used with csav and cret only.  Therefore if you do not
; compile the the +c option or +b option you do not need this.
;
        public  _lowwater_
_lowwater_      dw   -1
        extrn   _Dorg_:byte,_Dend_:byte
        extrn   _Uorg_:byte,_Uend_:byte
        extrn   NUorg_:byte, NUend_:byte, ram_check_:byte, jump_main_:byte
        extrn   first_time_:byte
; ============================================================================
;            User Defined Points and Ports
;=============================================================================
;public	udp_
;udp_	db	22            ; USER DEFINED POINT FOR USER APPLICATION
;udp_	db 32,33          ; alternate locations JUST IN CASE of incompatibility  
;	public port_
; port_	 dw 1             ; port 0=OpPort, 1=COMM1, 2=COMM2, if necessary
;==============================================================================

dataseg ends
        extrn   _Corg_:byte,_Cend_:byte

ifdef   FARPROC
        extrn main_:far, $fltinit:far
else
        extrn main_:far, $fltinit:far
endif

        public  $begin
$begin  proc    far
;       cli                   ; don't disable interrupts
cld
        push    ax            ; save registers
        push    bx
        push    cx
        push    dx
        push    ds
        push    es
        push    bp
        push    si
        push    di
;
;       Compute where initialzed data starts (@ next para after code)
;
        mov     ax,offset _Cend_+15
        mov     cl,4
        shr     ax,cl
        add     ax,seg _Cend_  ;the init data starts at seg = ax, offset = 0
        mov     ds,ax          ;place where data is in rom (program)
        mov     bx,dataseg     ;place where data is to go in ram
        mov     es,bx
;**************
;      check 20-byte pattern at ram_check.
;      skip copying ram if it compares to eprom.
;      comment this part out, if all data is to be intialized
;      at every warm start and cold start
     mov     ax, offset es: ram_check_ ;ram data segment in es (see above)
     mov     si, ax
     mov     di, ax             ;rom data segment in ds (see above )
     mov     ah, 0

chkram0:
      mov     al, ds:[di]        ;mov byte from rom
      cmp     al, es:[si]        ;compare from rom (ds) to ram (es)
      jne     copyram
      inc     si
      inc     di
      inc     ah
      cmp     ah, 20             ;compare 20 bytes
      jne     chkram0
      jmp     clrpart
; **************
;            copy Init data from rom to ram
copyram:
	mov     di,0
	mov     cx,offset _Dend_
	inc     cx               ;no of init data bytes =
	shr     cx,1      ;1 + offset of Dend - offset (= 0) of Dorg
	jcxz    clrall
	mov     si,0
initloop:
	mov     ax,ds:WORD PTR[di]   ;copy init data from rom to ram
	mov     es:WORD PTR[di],ax   ;(beginning at ds:di, di = 0)
	inc     di                   ;to ram (beginning at es: si, si = 0)
	inc     di
	dec     cx
	jne     initloop
;            clear all or part of the uninitialized data area
clrall:
        mov     di,offset _Uorg_
        jmp     clr2
clrpart:
        mov     di,offset NUorg_
clr2:   mov     cx,offset _Uend_
        mov     bx,dataseg
        mov     es,bx
        sub     cx,di
        inc     cx
        shr     cx,1
        jcxz    clrdone
        sub     ax,ax
clrloop:
        mov     es:WORD PTR[di],ax
        inc     di
        inc     di
        dec     cx
        jne     clrloop

clrdone:

        assume  ds:dataseg,es:dataseg

        mov     ds,bx           ;set DS, now DS, SS, ES are equal
        mov     di,$MEMRY
        inc     di
        and     di,0fffeH       ;adjust to word boundary
        mov     $MEMRY,di       ;save memory allocation info for sbrk()
        mov     $MEMRY+2,ds
        mov     _mbot_,0f2bah   ;di
        mov     _mbot_+2,0a0h   ;ds
        mov     _sbot_,0ffffH   ;this is the heap limit for sbrk()
        mov     _sbot_+2,0fff0h
        mov     _dsval_,ds
        mov     _csval_,cs      ;this is of dubious value in large code
        call    $fltinit        ;setup floating point software/hardware
        jnc     flt_ok
        hlt                     ;program needs 8087 and one wasn't found
flt_ok:
        mov     bx,dataseg	;place jump to main instruction
        mov     es,bx           ;at bottom of user ram
        mov     ax, offset es: jump_main_
        mov     si, ax
        mov     es:BYTE PTR[si],0eah
        mov     es:WORD PTR[si+1],offset main_
        mov     es:WORD PTR[si+3],seg main_

        mov     ax, offset es: first_time_   ;set first time flag
        mov     si, ax
        mov     es:BYTE PTR[si],1

        pop     di               ; restore registers
        pop     si
        pop     bp
        pop     es
        pop     ds
        pop     dx
        pop     cx
        pop     bx
        pop     ax
        ret
$begin  endp
codeseg ends
        end     $begin


