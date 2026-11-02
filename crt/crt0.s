; Startup code for AmigaOS programs built with clang.
;
; LoadSeg() starts a program at the first byte of its first hunk, with the
; argument string in a0 and its length in d0 (CLI), or with nothing (started
; from Workbench, which then sends a startup message to the process port).

	.text
	.globl _start
	.type _start,@function
_start:
	lea (-44,%sp),%sp
	movem.l %d2-%d7/%a2-%a6,(%sp)
	move.l %sp,__amiga_initial_sp
	move.l %a0,__amiga_argstr
	move.l %d0,__amiga_arglen
	move.l 4,%a6
	move.l %a6,SysBase
	; FindTask(NULL)
	move.l #0,%a1
	jsr (-294,%a6)
	move.l %d0,%a2
	; Started from the CLI if pr_CLI is set; otherwise wait for the
	; Workbench startup message.
	move.l (172,%a2),%d1
	bne .Lcli
	lea (92,%a2),%a0
	jsr (-384,%a6)		; WaitPort(&pr_MsgPort)
	lea (92,%a2),%a0
	jsr (-372,%a6)		; GetMsg(&pr_MsgPort)
	move.l %d0,_WBenchMsg
.Lcli:
	jsr __amiga_main
	move.l %d0,-(%sp)
	jsr _exit
	.size _start,.-_start

; void _exit(int status): return to the CLI or Workbench with the stack and
; registers _start received.
	.globl _exit
	.type _exit,@function
_exit:
	move.l (4,%sp),%d2
	move.l _WBenchMsg,%d0
	beq .Lret
	move.l SysBase,%a6
	jsr (-132,%a6)		; Forbid(); Workbench unloads us after the reply
	move.l _WBenchMsg,%a1
	jsr (-378,%a6)		; ReplyMsg(msg)
.Lret:
	move.l %d2,%d0
	move.l __amiga_initial_sp,%sp
	movem.l (%sp),%d2-%d7/%a2-%a6
	lea (44,%sp),%sp
	rts
	.size _exit,.-_exit

	.bss
	.globl SysBase
	.globl _WBenchMsg
	.p2align 2
SysBase:		.zero 4
_WBenchMsg:		.zero 4
__amiga_initial_sp:	.zero 4
	.globl __amiga_argstr
	.globl __amiga_arglen
__amiga_argstr:		.zero 4
__amiga_arglen:		.zero 4
