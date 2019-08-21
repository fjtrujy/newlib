/*
# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0
# Review ps2sdk README & LICENSE files for further details.
#
# This is a simple version of setjmp and longjmp.
# Floating point support in.
*/

#define O_S0	0x00
#define O_S1	0x10
#define O_S2	0x20
#define O_S3	0x30
#define O_S4	0x40
#define O_S5	0x50
#define O_S6	0x60
#define O_S7	0x70
#define O_FP	0x80
#define O_SP	0x90
#define O_RA	0xa0
#define O_F20   0xb0
#define O_F21   0xb4
#define O_F22   0xb8
#define O_F23   0xbc
#define O_F24   0xc0
#define O_F25   0xc4
#define O_F26   0xc8
#define O_F27   0xcc
#define O_F28   0xd0
#define O_F29   0xd4
#define O_F30   0xd8
#define O_F31   0xdc

/* int setjmp (jmp_buf);  */
	.globl	setjmp
	.ent	setjmp
setjmp:
	.frame	$sp,0,$31

	sq	$16, O_S0($4)
	sq	$17, O_S1($4)
	sq	$18, O_S2($4)
	sq	$19, O_S3($4)
	sq	$20, O_S4($4)
	sq	$21, O_S5($4)
	sq	$22, O_S6($4)
	sq	$23, O_S7($4)
	sq	$30, O_FP($4)
	sq	$29, O_SP($4)
	sq	$31, O_RA($4)

	swc1    $f20, O_F20($4)
	swc1    $f21, O_F21($4)
	swc1    $f22, O_F22($4)
	swc1    $f23, O_F23($4)
	swc1    $f24, O_F24($4)
	swc1    $f25, O_F25($4)
	swc1    $f26, O_F26($4)
	swc1    $f27, O_F27($4)
	swc1    $f28, O_F28($4)
	swc1    $f29, O_F29($4)
	swc1    $f30, O_F30($4)
	swc1    $f31, O_F31($4)


	move	$2, $0

	jr	$31

	.end	setjmp

/* volatile void longjmp (jmp_buf, int);  */
	.globl	longjmp
	.ent	longjmp
longjmp:
	.frame	$29,0,$31

	lq	$16, O_S0($4)
	lq	$17, O_S1($4)
	lq	$18, O_S2($4)
	lq	$19, O_S3($4)
	lq	$20, O_S4($4)
	lq	$21, O_S5($4)
	lq	$22, O_S6($4)
	lq	$23, O_S7($4)
	lq	$30, O_FP($4)
	lq	$29, O_SP($4)
	lq	$31, O_RA($4)

	lwc1    $f20, O_F20($4)
	lwc1    $f21, O_F21($4)
	lwc1    $f22, O_F22($4)
	lwc1    $f23, O_F23($4)
	lwc1    $f24, O_F24($4)
	lwc1    $f25, O_F25($4)
	lwc1    $f26, O_F26($4)
	lwc1    $f27, O_F27($4)
	lwc1    $f28, O_F28($4)
	lwc1    $f29, O_F29($4)
	lwc1    $f30, O_F30($4)
	lwc1    $f31, O_F31($4)

	bne	$4, $0, 1f
	li	$4, 1
1:
	move	$2, $4

	jr	$31

	.end longjmp
