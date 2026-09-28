	.build_version macos, 26, 0	sdk_version 26, 5
	.section	__TEXT,__text,regular,pure_instructions
	.globl	__Z4GEMMRNSt3__16vectorIfNS_9allocatorIfEEEERKNS0_IxNS1_IxEEEES4_S8_S4_ ; -- Begin function _Z4GEMMRNSt3__16vectorIfNS_9allocatorIfEEEERKNS0_IxNS1_IxEEEES4_S8_S4_
	.p2align	2
__Z4GEMMRNSt3__16vectorIfNS_9allocatorIfEEEERKNS0_IxNS1_IxEEEES4_S8_S4_: ; @_Z4GEMMRNSt3__16vectorIfNS_9allocatorIfEEEERKNS0_IxNS1_IxEEEES4_S8_S4_
	.cfi_startproc
; %bb.0:
	stp	x28, x27, [sp, #-96]!           ; 16-byte Folded Spill
	stp	x26, x25, [sp, #16]             ; 16-byte Folded Spill
	stp	x24, x23, [sp, #32]             ; 16-byte Folded Spill
	stp	x22, x21, [sp, #48]             ; 16-byte Folded Spill
	stp	x20, x19, [sp, #64]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #80]             ; 16-byte Folded Spill
	add	x29, sp, #80
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w24, -64
	.cfi_offset w25, -72
	.cfi_offset w26, -80
	.cfi_offset w27, -88
	.cfi_offset w28, -96
	mov	x22, x4
	mov	x20, x2
	mov	x21, x0
	mov	x23, x8
	ldr	x8, [x1]
	ldp	x9, x26, [x8]
	ldr	x8, [x3]
	ldr	x25, [x8]
	lsl	x27, x9, #32
	mul	x8, x27, x25
	stp	xzr, xzr, [x23, #8]
	str	xzr, [x23]
	cbz	x8, LBB0_3
; %bb.1:
	asr	x28, x8, #32
	lsr	x9, x28, #62
	cbnz	x9, LBB0_49
; %bb.2:
	asr	x24, x8, #30
	mov	x0, x24
	bl	__Znwm
	mov	x19, x0
	str	x0, [x23]
	add	x8, x0, x28, lsl #2
	str	x8, [x23, #16]
	mov	x1, x24
	bl	_bzero
	add	x8, x19, x24
	str	x8, [x23, #8]
	asr	x8, x27, #32
	cmp	x8, #1
	b.ge	LBB0_4
	b	LBB0_48
LBB0_3:
	mov	x19, #0                         ; =0x0
	asr	x8, x27, #32
	cmp	x8, #1
	b.lt	LBB0_48
LBB0_4:
	sxtw	x11, w26
	lsl	x15, x25, #32
	sxtw	x10, w25
	ldr	x9, [x22]
	cmp	x11, #1
	b.lt	LBB0_24
; %bb.5:
	cmp	x10, #1
	b.lt	LBB0_48
; %bb.6:
	mov	x12, #0                         ; =0x0
	ldr	x13, [x21]
	ldr	x14, [x20]
	smull	x16, w8, w10
	add	x16, x19, x16, lsl #2
	add	x15, x9, x15, lsr #30
	cmp	x10, #4
	cset	w1, lo
	cmp	x19, x15
	ccmp	x9, x16, #2, lo
	cset	w2, lo
	and	x15, x25, #0xf
	sub	x16, x10, x15
	and	x17, x25, #0x3
	sub	x0, x10, x17
	orr	w1, w1, w2
	lsl	x2, x10, #2
	lsl	x3, x11, #2
	add	x4, x9, #32
	add	x5, x19, #32
	sub	x6, x17, x10
	b	LBB0_8
LBB0_7:                                 ;   in Loop: Header=BB0_8 Depth=1
	add	x12, x12, #1
	add	x19, x19, x2
	add	x5, x5, x2
	cmp	x12, x8
	b.eq	LBB0_48
LBB0_8:                                 ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB0_9 Depth 2
                                        ;       Child Loop BB0_10 Depth 3
                                        ;     Child Loop BB0_17 Depth 2
                                        ;     Child Loop BB0_21 Depth 2
                                        ;     Child Loop BB0_23 Depth 2
	mov	x7, #0                          ; =0x0
	mul	x20, x12, x11
	add	x20, x13, x20, lsl #2
	mov	x21, x14
LBB0_9:                                 ;   Parent Loop BB0_8 Depth=1
                                        ; =>  This Loop Header: Depth=2
                                        ;       Child Loop BB0_10 Depth 3
	mov	x22, x21
	mov	x23, x19
	mov	x24, x10
LBB0_10:                                ;   Parent Loop BB0_8 Depth=1
                                        ;     Parent Loop BB0_9 Depth=2
                                        ; =>    This Inner Loop Header: Depth=3
	ldr	s0, [x20, x7, lsl #2]
	ldr	s1, [x22]
	ldr	s2, [x23]
	fmadd	s0, s0, s1, s2
	str	s0, [x23], #4
	add	x22, x22, x3
	subs	x24, x24, #1
	b.ne	LBB0_10
; %bb.11:                               ;   in Loop: Header=BB0_9 Depth=2
	add	x7, x7, #1
	add	x21, x21, #4
	cmp	x7, x11
	b.ne	LBB0_9
; %bb.12:                               ;   in Loop: Header=BB0_8 Depth=1
	tbz	w1, #0, LBB0_14
; %bb.13:                               ;   in Loop: Header=BB0_8 Depth=1
	mov	x7, #0                          ; =0x0
	b	LBB0_23
LBB0_14:                                ;   in Loop: Header=BB0_8 Depth=1
	cmp	x10, #16
	b.hs	LBB0_16
; %bb.15:                               ;   in Loop: Header=BB0_8 Depth=1
	mov	x20, #0                         ; =0x0
	b	LBB0_20
LBB0_16:                                ;   in Loop: Header=BB0_8 Depth=1
	mov	x7, x5
	mov	x20, x4
	mov	x21, x16
LBB0_17:                                ;   Parent Loop BB0_8 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldp	q0, q1, [x20, #-32]
	ldp	q2, q3, [x20], #64
	ldp	q4, q5, [x7, #-32]
	ldp	q6, q7, [x7]
	fadd.4s	v0, v0, v4
	fadd.4s	v1, v1, v5
	fadd.4s	v2, v2, v6
	fadd.4s	v3, v3, v7
	stp	q0, q1, [x7, #-32]
	stp	q2, q3, [x7], #64
	subs	x21, x21, #16
	b.ne	LBB0_17
; %bb.18:                               ;   in Loop: Header=BB0_8 Depth=1
	cbz	x15, LBB0_7
; %bb.19:                               ;   in Loop: Header=BB0_8 Depth=1
	mov	x20, x16
	mov	x7, x16
	cmp	x15, #4
	b.lo	LBB0_23
LBB0_20:                                ;   in Loop: Header=BB0_8 Depth=1
	add	x7, x6, x20
	lsl	x20, x20, #2
LBB0_21:                                ;   Parent Loop BB0_8 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldr	q0, [x9, x20]
	ldr	q1, [x19, x20]
	fadd.4s	v0, v0, v1
	str	q0, [x19, x20]
	add	x20, x20, #16
	adds	x7, x7, #4
	b.ne	LBB0_21
; %bb.22:                               ;   in Loop: Header=BB0_8 Depth=1
	mov	x7, x0
	cbz	x17, LBB0_7
LBB0_23:                                ;   Parent Loop BB0_8 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldr	s0, [x9, x7, lsl #2]
	ldr	s1, [x19, x7, lsl #2]
	fadd	s0, s0, s1
	str	s0, [x19, x7, lsl #2]
	add	x7, x7, #1
	cmp	x10, x7
	b.ne	LBB0_23
	b	LBB0_7
LBB0_24:
	cmp	x10, #1
	b.lt	LBB0_48
; %bb.25:
	cmp	x10, #3
	b.hi	LBB0_31
; %bb.26:
	add	x11, x19, #8
	lsl	x10, x10, #2
	mov	x12, #4294967296                ; =0x100000000
	mov	x13, #8589934592                ; =0x200000000
	b	LBB0_28
LBB0_27:                                ;   in Loop: Header=BB0_28 Depth=1
	add	x11, x11, x10
	subs	x8, x8, #1
	b.eq	LBB0_48
LBB0_28:                                ; =>This Inner Loop Header: Depth=1
	ldr	s0, [x9]
	ldur	s1, [x11, #-8]
	fadd	s0, s0, s1
	stur	s0, [x11, #-8]
	cmp	x15, x12
	b.eq	LBB0_27
; %bb.29:                               ;   in Loop: Header=BB0_28 Depth=1
	ldr	s0, [x9, #4]
	ldur	s1, [x11, #-4]
	fadd	s0, s0, s1
	stur	s0, [x11, #-4]
	cmp	x15, x13
	b.eq	LBB0_27
; %bb.30:                               ;   in Loop: Header=BB0_28 Depth=1
	ldr	s0, [x9, #8]
	ldr	s1, [x11]
	fadd	s0, s0, s1
	str	s0, [x11]
	b	LBB0_27
LBB0_31:
	add	x11, x9, x15, lsr #30
	smull	x12, w8, w10
	add	x12, x19, x12, lsl #2
	cmp	x19, x11
	mov	x11, #0                         ; =0x0
	ccmp	x9, x12, #2, lo
	b.lo	LBB0_44
; %bb.32:
	and	x12, x25, #0xf
	sub	x13, x10, x12
	and	x14, x25, #0x3
	sub	x15, x10, x14
	add	x16, x9, #32
	add	x17, x19, #32
	lsl	x0, x10, #2
	sub	x1, x14, x10
	b	LBB0_34
LBB0_33:                                ;   in Loop: Header=BB0_34 Depth=1
	add	x11, x11, #1
	add	x17, x17, x0
	add	x19, x19, x0
	cmp	x11, x8
	b.eq	LBB0_48
LBB0_34:                                ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB0_37 Depth 2
                                        ;     Child Loop BB0_41 Depth 2
                                        ;     Child Loop BB0_43 Depth 2
	cmp	x10, #16
	b.hs	LBB0_36
; %bb.35:                               ;   in Loop: Header=BB0_34 Depth=1
	mov	x3, #0                          ; =0x0
	b	LBB0_40
LBB0_36:                                ;   in Loop: Header=BB0_34 Depth=1
	mov	x2, x17
	mov	x3, x16
	mov	x4, x13
LBB0_37:                                ;   Parent Loop BB0_34 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldp	q0, q1, [x3, #-32]
	ldp	q2, q3, [x3], #64
	ldp	q4, q5, [x2, #-32]
	ldp	q6, q7, [x2]
	fadd.4s	v0, v0, v4
	fadd.4s	v1, v1, v5
	fadd.4s	v2, v2, v6
	fadd.4s	v3, v3, v7
	stp	q0, q1, [x2, #-32]
	stp	q2, q3, [x2], #64
	subs	x4, x4, #16
	b.ne	LBB0_37
; %bb.38:                               ;   in Loop: Header=BB0_34 Depth=1
	cbz	x12, LBB0_33
; %bb.39:                               ;   in Loop: Header=BB0_34 Depth=1
	mov	x3, x13
	mov	x2, x13
	cmp	x12, #4
	b.lo	LBB0_43
LBB0_40:                                ;   in Loop: Header=BB0_34 Depth=1
	add	x2, x1, x3
	lsl	x4, x3, #2
	add	x3, x19, x4
	add	x4, x9, x4
LBB0_41:                                ;   Parent Loop BB0_34 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldr	q0, [x4], #16
	ldr	q1, [x3]
	fadd.4s	v0, v0, v1
	str	q0, [x3], #16
	adds	x2, x2, #4
	b.ne	LBB0_41
; %bb.42:                               ;   in Loop: Header=BB0_34 Depth=1
	mov	x2, x15
	cbz	x14, LBB0_33
LBB0_43:                                ;   Parent Loop BB0_34 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldr	s0, [x9, x2, lsl #2]
	ldr	s1, [x19, x2, lsl #2]
	fadd	s0, s0, s1
	str	s0, [x19, x2, lsl #2]
	add	x2, x2, #1
	cmp	x10, x2
	b.ne	LBB0_43
	b	LBB0_33
LBB0_44:
	lsl	x12, x10, #2
LBB0_45:                                ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB0_46 Depth 2
	mov	x13, x9
	mov	x14, x19
	mov	x15, x10
LBB0_46:                                ;   Parent Loop BB0_45 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldr	s0, [x13], #4
	ldr	s1, [x14]
	fadd	s0, s0, s1
	str	s0, [x14], #4
	subs	x15, x15, #1
	b.ne	LBB0_46
; %bb.47:                               ;   in Loop: Header=BB0_45 Depth=1
	add	x11, x11, #1
	add	x19, x19, x12
	cmp	x11, x8
	b.ne	LBB0_45
LBB0_48:
	ldp	x29, x30, [sp, #80]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #64]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #48]             ; 16-byte Folded Reload
	ldp	x24, x23, [sp, #32]             ; 16-byte Folded Reload
	ldp	x26, x25, [sp, #16]             ; 16-byte Folded Reload
	ldp	x28, x27, [sp], #96             ; 16-byte Folded Reload
	ret
LBB0_49:
	bl	__ZNSt3__16vectorIfNS_9allocatorIfEEE20__throw_length_errorB9nqe210106Ev
	.cfi_endproc
                                        ; -- End function
	.globl	__Z4ReLURNSt3__16vectorIfNS_9allocatorIfEEEE ; -- Begin function _Z4ReLURNSt3__16vectorIfNS_9allocatorIfEEEE
	.p2align	2
__Z4ReLURNSt3__16vectorIfNS_9allocatorIfEEEE: ; @_Z4ReLURNSt3__16vectorIfNS_9allocatorIfEEEE
	.cfi_startproc
; %bb.0:
	ldp	x8, x9, [x0]
	subs	x11, x9, x8
	b.eq	LBB1_52
; %bb.1:
	asr	x9, x11, #2
	cmp	x9, #3
	b.hi	LBB1_3
; %bb.2:
	mov	x10, #0                         ; =0x0
	b	LBB1_53
LBB1_3:
	cmp	x9, #16
	b.hs	LBB1_5
; %bb.4:
	mov	x10, #0                         ; =0x0
	b	LBB1_41
LBB1_5:
	and	x10, x9, #0xfffffffffffffff0
	add	x12, x8, #32
	mov	x13, x10
	b	LBB1_7
LBB1_6:                                 ;   in Loop: Header=BB1_7 Depth=1
	add	x12, x12, #64
	subs	x13, x13, #16
	b.eq	LBB1_39
LBB1_7:                                 ; =>This Inner Loop Header: Depth=1
	ldur	q0, [x12, #-32]
	fcmp	s0, #0.0
	b.mi	LBB1_23
; %bb.8:                                ;   in Loop: Header=BB1_7 Depth=1
	mov	s1, v0[1]
	fcmp	s1, #0.0
	b.mi	LBB1_24
LBB1_9:                                 ;   in Loop: Header=BB1_7 Depth=1
	mov	s1, v0[2]
	fcmp	s1, #0.0
	b.mi	LBB1_25
LBB1_10:                                ;   in Loop: Header=BB1_7 Depth=1
	mov	s0, v0[3]
	fcmp	s0, #0.0
	b.mi	LBB1_26
LBB1_11:                                ;   in Loop: Header=BB1_7 Depth=1
	ldur	q0, [x12, #-16]
	fcmp	s0, #0.0
	b.mi	LBB1_27
LBB1_12:                                ;   in Loop: Header=BB1_7 Depth=1
	mov	s1, v0[1]
	fcmp	s1, #0.0
	b.mi	LBB1_28
LBB1_13:                                ;   in Loop: Header=BB1_7 Depth=1
	mov	s1, v0[2]
	fcmp	s1, #0.0
	b.mi	LBB1_29
LBB1_14:                                ;   in Loop: Header=BB1_7 Depth=1
	mov	s0, v0[3]
	fcmp	s0, #0.0
	b.mi	LBB1_30
LBB1_15:                                ;   in Loop: Header=BB1_7 Depth=1
	ldr	q1, [x12]
	fcmp	s1, #0.0
	b.mi	LBB1_31
LBB1_16:                                ;   in Loop: Header=BB1_7 Depth=1
	mov	s0, v1[1]
	fcmp	s0, #0.0
	b.mi	LBB1_32
LBB1_17:                                ;   in Loop: Header=BB1_7 Depth=1
	ldr	q0, [x12, #16]
	mov	s2, v1[2]
	fcmp	s2, #0.0
	b.mi	LBB1_33
LBB1_18:                                ;   in Loop: Header=BB1_7 Depth=1
	mov	s1, v1[3]
	fcmp	s1, #0.0
	b.mi	LBB1_34
LBB1_19:                                ;   in Loop: Header=BB1_7 Depth=1
	fcmp	s0, #0.0
	b.mi	LBB1_35
LBB1_20:                                ;   in Loop: Header=BB1_7 Depth=1
	mov	s1, v0[1]
	fcmp	s1, #0.0
	b.mi	LBB1_36
LBB1_21:                                ;   in Loop: Header=BB1_7 Depth=1
	mov	s1, v0[2]
	fcmp	s1, #0.0
	b.mi	LBB1_37
LBB1_22:                                ;   in Loop: Header=BB1_7 Depth=1
	mov	s0, v0[3]
	fcmp	s0, #0.0
	b.pl	LBB1_6
	b	LBB1_38
LBB1_23:                                ;   in Loop: Header=BB1_7 Depth=1
	stur	wzr, [x12, #-32]
	mov	s1, v0[1]
	fcmp	s1, #0.0
	b.pl	LBB1_9
LBB1_24:                                ;   in Loop: Header=BB1_7 Depth=1
	stur	wzr, [x12, #-28]
	mov	s1, v0[2]
	fcmp	s1, #0.0
	b.pl	LBB1_10
LBB1_25:                                ;   in Loop: Header=BB1_7 Depth=1
	stur	wzr, [x12, #-24]
	mov	s0, v0[3]
	fcmp	s0, #0.0
	b.pl	LBB1_11
LBB1_26:                                ;   in Loop: Header=BB1_7 Depth=1
	stur	wzr, [x12, #-20]
	ldur	q0, [x12, #-16]
	fcmp	s0, #0.0
	b.pl	LBB1_12
LBB1_27:                                ;   in Loop: Header=BB1_7 Depth=1
	stur	wzr, [x12, #-16]
	mov	s1, v0[1]
	fcmp	s1, #0.0
	b.pl	LBB1_13
LBB1_28:                                ;   in Loop: Header=BB1_7 Depth=1
	stur	wzr, [x12, #-12]
	mov	s1, v0[2]
	fcmp	s1, #0.0
	b.pl	LBB1_14
LBB1_29:                                ;   in Loop: Header=BB1_7 Depth=1
	stur	wzr, [x12, #-8]
	mov	s0, v0[3]
	fcmp	s0, #0.0
	b.pl	LBB1_15
LBB1_30:                                ;   in Loop: Header=BB1_7 Depth=1
	stur	wzr, [x12, #-4]
	ldr	q1, [x12]
	fcmp	s1, #0.0
	b.pl	LBB1_16
LBB1_31:                                ;   in Loop: Header=BB1_7 Depth=1
	str	wzr, [x12]
	mov	s0, v1[1]
	fcmp	s0, #0.0
	b.pl	LBB1_17
LBB1_32:                                ;   in Loop: Header=BB1_7 Depth=1
	str	wzr, [x12, #4]
	ldr	q0, [x12, #16]
	mov	s2, v1[2]
	fcmp	s2, #0.0
	b.pl	LBB1_18
LBB1_33:                                ;   in Loop: Header=BB1_7 Depth=1
	str	wzr, [x12, #8]
	mov	s1, v1[3]
	fcmp	s1, #0.0
	b.pl	LBB1_19
LBB1_34:                                ;   in Loop: Header=BB1_7 Depth=1
	str	wzr, [x12, #12]
	fcmp	s0, #0.0
	b.pl	LBB1_20
LBB1_35:                                ;   in Loop: Header=BB1_7 Depth=1
	str	wzr, [x12, #16]
	mov	s1, v0[1]
	fcmp	s1, #0.0
	b.pl	LBB1_21
LBB1_36:                                ;   in Loop: Header=BB1_7 Depth=1
	str	wzr, [x12, #20]
	mov	s1, v0[2]
	fcmp	s1, #0.0
	b.pl	LBB1_22
LBB1_37:                                ;   in Loop: Header=BB1_7 Depth=1
	str	wzr, [x12, #24]
	mov	s0, v0[3]
	fcmp	s0, #0.0
	b.pl	LBB1_6
LBB1_38:                                ;   in Loop: Header=BB1_7 Depth=1
	str	wzr, [x12, #28]
	b	LBB1_6
LBB1_39:
	cmp	x9, x10
	b.eq	LBB1_52
; %bb.40:
	tst	x11, #0x30
	b.eq	LBB1_53
LBB1_41:
	mov	x12, x10
	and	x10, x9, #0xfffffffffffffffc
	sub	x11, x12, x10
	add	x12, x8, x12, lsl #2
	b	LBB1_43
LBB1_42:                                ;   in Loop: Header=BB1_43 Depth=1
	add	x12, x12, #16
	adds	x11, x11, #4
	b.eq	LBB1_51
LBB1_43:                                ; =>This Inner Loop Header: Depth=1
	ldr	q0, [x12]
	fcmp	s0, #0.0
	b.mi	LBB1_47
; %bb.44:                               ;   in Loop: Header=BB1_43 Depth=1
	mov	s1, v0[1]
	fcmp	s1, #0.0
	b.mi	LBB1_48
LBB1_45:                                ;   in Loop: Header=BB1_43 Depth=1
	mov	s1, v0[2]
	fcmp	s1, #0.0
	b.mi	LBB1_49
LBB1_46:                                ;   in Loop: Header=BB1_43 Depth=1
	mov	s0, v0[3]
	fcmp	s0, #0.0
	b.pl	LBB1_42
	b	LBB1_50
LBB1_47:                                ;   in Loop: Header=BB1_43 Depth=1
	str	wzr, [x12]
	mov	s1, v0[1]
	fcmp	s1, #0.0
	b.pl	LBB1_45
LBB1_48:                                ;   in Loop: Header=BB1_43 Depth=1
	str	wzr, [x12, #4]
	mov	s1, v0[2]
	fcmp	s1, #0.0
	b.pl	LBB1_46
LBB1_49:                                ;   in Loop: Header=BB1_43 Depth=1
	str	wzr, [x12, #8]
	mov	s0, v0[3]
	fcmp	s0, #0.0
	b.pl	LBB1_42
LBB1_50:                                ;   in Loop: Header=BB1_43 Depth=1
	str	wzr, [x12, #12]
	b	LBB1_42
LBB1_51:
	cmp	x9, x10
	b.ne	LBB1_53
LBB1_52:
	ret
LBB1_53:
	add	x8, x8, x10, lsl #2
	sub	x9, x9, x10
	b	LBB1_55
LBB1_54:                                ;   in Loop: Header=BB1_55 Depth=1
	add	x8, x8, #4
	subs	x9, x9, #1
	b.eq	LBB1_52
LBB1_55:                                ; =>This Inner Loop Header: Depth=1
	ldr	s0, [x8]
	fcmp	s0, #0.0
	b.pl	LBB1_54
; %bb.56:                               ;   in Loop: Header=BB1_55 Depth=1
	str	wzr, [x8]
	b	LBB1_54
	.cfi_endproc
                                        ; -- End function
	.globl	__Z7SoftmaxRNSt3__16vectorIfNS_9allocatorIfEEEERKNS0_IxNS1_IxEEEE ; -- Begin function _Z7SoftmaxRNSt3__16vectorIfNS_9allocatorIfEEEERKNS0_IxNS1_IxEEEE
	.p2align	2
__Z7SoftmaxRNSt3__16vectorIfNS_9allocatorIfEEEERKNS0_IxNS1_IxEEEE: ; @_Z7SoftmaxRNSt3__16vectorIfNS_9allocatorIfEEEERKNS0_IxNS1_IxEEEE
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #384
	stp	d9, d8, [sp, #272]              ; 16-byte Folded Spill
	stp	x28, x27, [sp, #288]            ; 16-byte Folded Spill
	stp	x26, x25, [sp, #304]            ; 16-byte Folded Spill
	stp	x24, x23, [sp, #320]            ; 16-byte Folded Spill
	stp	x22, x21, [sp, #336]            ; 16-byte Folded Spill
	stp	x20, x19, [sp, #352]            ; 16-byte Folded Spill
	stp	x29, x30, [sp, #368]            ; 16-byte Folded Spill
	add	x29, sp, #368
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	.cfi_offset w23, -56
	.cfi_offset w24, -64
	.cfi_offset w25, -72
	.cfi_offset w26, -80
	.cfi_offset w27, -88
	.cfi_offset w28, -96
	.cfi_offset b8, -104
	.cfi_offset b9, -112
	ldr	x8, [x1]
	ldr	x28, [x8]
	cmp	x28, #1
	b.lt	LBB2_24
; %bb.1:
	ldr	x20, [x8, #8]
	cmp	x20, #1
	b.lt	LBB2_24
; %bb.2:
	mov	x21, #0                         ; =0x0
	and	x9, x20, #0x7ffffffffffffff8
	ldr	x19, [x0]
	and	x8, x20, #0x7ffffffffffffff0
	stp	x8, x9, [sp, #32]               ; 16-byte Folded Spill
	and	x9, x20, #0xc
	and	x8, x20, #0x7ffffffffffffffc
	lsl	x27, x20, #2
	add	x24, x19, #16
	add	x25, x19, #32
	str	x8, [sp, #24]                   ; 8-byte Folded Spill
	neg	x8, x8
	stp	x9, x8, [sp, #8]                ; 16-byte Folded Spill
	mov	x23, x19
	b	LBB2_4
LBB2_3:                                 ;   in Loop: Header=BB2_4 Depth=1
	add	x21, x21, #1
	add	x23, x23, x27
	add	x24, x24, x27
	add	x25, x25, x27
	cmp	x21, x28
	b.eq	LBB2_24
LBB2_4:                                 ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB2_5 Depth 2
                                        ;     Child Loop BB2_9 Depth 2
                                        ;     Child Loop BB2_11 Depth 2
                                        ;     Child Loop BB2_17 Depth 2
                                        ;     Child Loop BB2_21 Depth 2
                                        ;     Child Loop BB2_23 Depth 2
	mov	x8, #0                          ; =0x0
	mul	x9, x21, x20
	ldr	s1, [x19, x9, lsl #2]
LBB2_5:                                 ;   Parent Loop BB2_4 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldr	s0, [x23, x8, lsl #2]
	fcmp	s0, s1
	fcsel	s1, s0, s1, gt
	add	x8, x8, #1
	cmp	x20, x8
	b.ne	LBB2_5
; %bb.6:                                ;   in Loop: Header=BB2_4 Depth=1
	cmp	x20, #8
	str	q1, [sp, #48]                   ; 16-byte Folded Spill
	b.hs	LBB2_8
; %bb.7:                                ;   in Loop: Header=BB2_4 Depth=1
	mov	x22, #0                         ; =0x0
	movi.2d	v5, #0000000000000000
	b	LBB2_11
LBB2_8:                                 ;   in Loop: Header=BB2_4 Depth=1
	mov	x26, x28
	dup.2s	v8, v1[0]
	movi.2d	v5, #0000000000000000
	mov	x28, x24
	ldr	x22, [sp, #40]                  ; 8-byte Folded Reload
LBB2_9:                                 ;   Parent Loop BB2_4 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	stur	q5, [x29, #-128]                ; 16-byte Folded Spill
	ldp	d0, d1, [x28, #-16]
	ldp	d2, d3, [x28]
	fsub.2s	v4, v0, v8
	stur	q4, [x29, #-176]                ; 16-byte Folded Spill
	fsub.2s	v0, v1, v8
	str	q0, [sp, #160]                  ; 16-byte Folded Spill
	fsub.2s	v0, v2, v8
	str	q0, [sp, #176]                  ; 16-byte Folded Spill
	fsub.2s	v0, v3, v8
	stur	q0, [x29, #-160]                ; 16-byte Folded Spill
	mov	s0, v4[1]
	bl	_expf
                                        ; kill: def $s0 killed $s0 def $q0
	stur	q0, [x29, #-144]                ; 16-byte Folded Spill
	ldur	q0, [x29, #-176]                ; 16-byte Folded Reload
                                        ; kill: def $s0 killed $s0 killed $q0
	bl	_expf
                                        ; kill: def $s0 killed $s0 def $q0
	str	q0, [sp, #128]                  ; 16-byte Folded Spill
	mov.16b	v1, v0
	ldur	q0, [x29, #-144]                ; 16-byte Folded Reload
	mov.s	v1[1], v0[0]
	str	q1, [sp, #144]                  ; 16-byte Folded Spill
	ldr	q0, [sp, #160]                  ; 16-byte Folded Reload
	mov	s0, v0[1]
	bl	_expf
                                        ; kill: def $s0 killed $s0 def $q0
	stur	q0, [x29, #-176]                ; 16-byte Folded Spill
	ldr	q0, [sp, #160]                  ; 16-byte Folded Reload
                                        ; kill: def $s0 killed $s0 killed $q0
	bl	_expf
                                        ; kill: def $s0 killed $s0 def $q0
	str	q0, [sp, #96]                   ; 16-byte Folded Spill
	mov.16b	v1, v0
	ldur	q0, [x29, #-176]                ; 16-byte Folded Reload
	mov.s	v1[1], v0[0]
	str	q1, [sp, #112]                  ; 16-byte Folded Spill
	ldr	q0, [sp, #176]                  ; 16-byte Folded Reload
	mov	s0, v0[1]
	bl	_expf
                                        ; kill: def $s0 killed $s0 def $q0
	str	q0, [sp, #160]                  ; 16-byte Folded Spill
	ldr	q0, [sp, #176]                  ; 16-byte Folded Reload
                                        ; kill: def $s0 killed $s0 killed $q0
	bl	_expf
                                        ; kill: def $s0 killed $s0 def $q0
	str	q0, [sp, #80]                   ; 16-byte Folded Spill
	mov.16b	v1, v0
	ldr	q0, [sp, #160]                  ; 16-byte Folded Reload
	mov.s	v1[1], v0[0]
	str	q1, [sp, #176]                  ; 16-byte Folded Spill
	ldur	q0, [x29, #-160]                ; 16-byte Folded Reload
	mov	s0, v0[1]
	bl	_expf
                                        ; kill: def $s0 killed $s0 def $q0
	str	q0, [sp, #64]                   ; 16-byte Folded Spill
	ldur	q0, [x29, #-160]                ; 16-byte Folded Reload
                                        ; kill: def $s0 killed $s0 killed $q0
	bl	_expf
                                        ; kill: def $s0 killed $s0 def $q0
	ldur	q1, [x29, #-128]                ; 16-byte Folded Reload
	ldr	q2, [sp, #128]                  ; 16-byte Folded Reload
	fadd	s1, s1, s2
	ldur	q2, [x29, #-144]                ; 16-byte Folded Reload
	fadd	s1, s1, s2
	ldr	q2, [sp, #96]                   ; 16-byte Folded Reload
	fadd	s1, s1, s2
	ldur	q2, [x29, #-176]                ; 16-byte Folded Reload
	fadd	s1, s1, s2
	ldp	q3, q2, [sp, #64]               ; 32-byte Folded Reload
	fadd	s1, s1, s2
	ldp	q5, q4, [sp, #144]              ; 32-byte Folded Reload
	fadd	s1, s1, s4
	fadd	s1, s1, s0
	mov.s	v0[1], v3[0]
	ldr	q2, [sp, #112]                  ; 16-byte Folded Reload
	stp	d5, d2, [x28, #-16]
	ldr	q2, [sp, #176]                  ; 16-byte Folded Reload
	stp	d2, d0, [x28], #32
	fadd	s5, s1, s3
	subs	x22, x22, #8
	b.ne	LBB2_9
; %bb.10:                               ;   in Loop: Header=BB2_4 Depth=1
	ldr	x8, [sp, #40]                   ; 8-byte Folded Reload
	mov	x22, x8
	cmp	x20, x8
	mov	x28, x26
	ldr	q1, [sp, #48]                   ; 16-byte Folded Reload
	b.eq	LBB2_12
LBB2_11:                                ;   Parent Loop BB2_4 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	stur	q5, [x29, #-128]                ; 16-byte Folded Spill
	ldr	s0, [x23, x22, lsl #2]
	fsub	s0, s0, s1
	bl	_expf
	ldr	q1, [sp, #48]                   ; 16-byte Folded Reload
	ldur	q5, [x29, #-128]                ; 16-byte Folded Reload
	str	s0, [x23, x22, lsl #2]
	fadd	s5, s5, s0
	add	x22, x22, #1
	cmp	x20, x22
	b.ne	LBB2_11
LBB2_12:                                ;   in Loop: Header=BB2_4 Depth=1
	cmp	x20, #3
	b.hi	LBB2_14
; %bb.13:                               ;   in Loop: Header=BB2_4 Depth=1
	mov	x8, #0                          ; =0x0
	b	LBB2_23
LBB2_14:                                ;   in Loop: Header=BB2_4 Depth=1
	cmp	x20, #16
	b.hs	LBB2_16
; %bb.15:                               ;   in Loop: Header=BB2_4 Depth=1
	mov	x9, #0                          ; =0x0
	b	LBB2_20
LBB2_16:                                ;   in Loop: Header=BB2_4 Depth=1
	dup.4s	v0, v5[0]
	mov	x8, x25
	ldr	x9, [sp, #32]                   ; 8-byte Folded Reload
LBB2_17:                                ;   Parent Loop BB2_4 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldp	q1, q2, [x8, #-32]
	ldp	q3, q4, [x8]
	fdiv.4s	v1, v1, v0
	fdiv.4s	v2, v2, v0
	fdiv.4s	v3, v3, v0
	fdiv.4s	v4, v4, v0
	stp	q1, q2, [x8, #-32]
	stp	q3, q4, [x8], #64
	subs	x9, x9, #16
	b.ne	LBB2_17
; %bb.18:                               ;   in Loop: Header=BB2_4 Depth=1
	ldr	x8, [sp, #32]                   ; 8-byte Folded Reload
	cmp	x20, x8
	b.eq	LBB2_3
; %bb.19:                               ;   in Loop: Header=BB2_4 Depth=1
	ldr	x9, [sp, #32]                   ; 8-byte Folded Reload
	mov	x8, x9
	ldr	x10, [sp, #8]                   ; 8-byte Folded Reload
	cbz	x10, LBB2_23
LBB2_20:                                ;   in Loop: Header=BB2_4 Depth=1
	dup.4s	v0, v5[0]
	ldr	x8, [sp, #16]                   ; 8-byte Folded Reload
	add	x8, x8, x9
	lsl	x9, x9, #2
LBB2_21:                                ;   Parent Loop BB2_4 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldr	q1, [x23, x9]
	fdiv.4s	v1, v1, v0
	str	q1, [x23, x9]
	add	x9, x9, #16
	adds	x8, x8, #4
	b.ne	LBB2_21
; %bb.22:                               ;   in Loop: Header=BB2_4 Depth=1
	ldr	x9, [sp, #24]                   ; 8-byte Folded Reload
	mov	x8, x9
	cmp	x20, x9
	b.eq	LBB2_3
LBB2_23:                                ;   Parent Loop BB2_4 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	ldr	s0, [x23, x8, lsl #2]
	fdiv	s0, s0, s5
	str	s0, [x23, x8, lsl #2]
	add	x8, x8, #1
	cmp	x20, x8
	b.ne	LBB2_23
	b	LBB2_3
LBB2_24:
	ldp	x29, x30, [sp, #368]            ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #352]            ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #336]            ; 16-byte Folded Reload
	ldp	x24, x23, [sp, #320]            ; 16-byte Folded Reload
	ldp	x26, x25, [sp, #304]            ; 16-byte Folded Reload
	ldp	x28, x27, [sp, #288]            ; 16-byte Folded Reload
	ldp	d9, d8, [sp, #272]              ; 16-byte Folded Reload
	add	sp, sp, #384
	ret
	.cfi_endproc
                                        ; -- End function
	.private_extern	__ZNSt3__16vectorIfNS_9allocatorIfEEE20__throw_length_errorB9nqe210106Ev ; -- Begin function _ZNSt3__16vectorIfNS_9allocatorIfEEE20__throw_length_errorB9nqe210106Ev
	.globl	__ZNSt3__16vectorIfNS_9allocatorIfEEE20__throw_length_errorB9nqe210106Ev
	.weak_def_can_be_hidden	__ZNSt3__16vectorIfNS_9allocatorIfEEE20__throw_length_errorB9nqe210106Ev
	.p2align	2
__ZNSt3__16vectorIfNS_9allocatorIfEEE20__throw_length_errorB9nqe210106Ev: ; @_ZNSt3__16vectorIfNS_9allocatorIfEEE20__throw_length_errorB9nqe210106Ev
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
Lloh0:
	adrp	x0, l_.str@PAGE
Lloh1:
	add	x0, x0, l_.str@PAGEOFF
	bl	__ZNSt3__120__throw_length_errorB9nqe210106EPKc
	.loh AdrpAdd	Lloh0, Lloh1
	.cfi_endproc
                                        ; -- End function
	.private_extern	__ZNSt3__120__throw_length_errorB9nqe210106EPKc ; -- Begin function _ZNSt3__120__throw_length_errorB9nqe210106EPKc
	.globl	__ZNSt3__120__throw_length_errorB9nqe210106EPKc
	.weak_def_can_be_hidden	__ZNSt3__120__throw_length_errorB9nqe210106EPKc
	.p2align	2
__ZNSt3__120__throw_length_errorB9nqe210106EPKc: ; @_ZNSt3__120__throw_length_errorB9nqe210106EPKc
Lfunc_begin0:
	.cfi_startproc
	.cfi_personality 155, ___gxx_personality_v0
	.cfi_lsda 16, Lexception0
; %bb.0:
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	add	x29, sp, #16
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x20, x0
	mov	w0, #16                         ; =0x10
	bl	___cxa_allocate_exception
	mov	x19, x0
Ltmp0:
	mov	x1, x20
	bl	__ZNSt12length_errorC1B9nqe210106EPKc
Ltmp1:
; %bb.1:
Lloh2:
	adrp	x1, __ZTISt12length_error@GOTPAGE
Lloh3:
	ldr	x1, [x1, __ZTISt12length_error@GOTPAGEOFF]
Lloh4:
	adrp	x2, __ZNSt12length_errorD1Ev@GOTPAGE
Lloh5:
	ldr	x2, [x2, __ZNSt12length_errorD1Ev@GOTPAGEOFF]
	mov	x0, x19
	bl	___cxa_throw
LBB4_2:
Ltmp2:
	mov	x20, x0
	mov	x0, x19
	bl	___cxa_free_exception
	mov	x0, x20
	bl	__Unwind_Resume
	.loh AdrpLdrGot	Lloh4, Lloh5
	.loh AdrpLdrGot	Lloh2, Lloh3
Lfunc_end0:
	.cfi_endproc
	.section	__TEXT,__gcc_except_tab
	.p2align	2, 0x0
GCC_except_table4:
Lexception0:
	.byte	255                             ; @LPStart Encoding = omit
	.byte	255                             ; @TType Encoding = omit
	.byte	1                               ; Call site Encoding = uleb128
	.uleb128 Lcst_end0-Lcst_begin0
Lcst_begin0:
	.uleb128 Lfunc_begin0-Lfunc_begin0      ; >> Call Site 1 <<
	.uleb128 Ltmp0-Lfunc_begin0             ;   Call between Lfunc_begin0 and Ltmp0
	.byte	0                               ;     has no landing pad
	.byte	0                               ;   On action: cleanup
	.uleb128 Ltmp0-Lfunc_begin0             ; >> Call Site 2 <<
	.uleb128 Ltmp1-Ltmp0                    ;   Call between Ltmp0 and Ltmp1
	.uleb128 Ltmp2-Lfunc_begin0             ;     jumps to Ltmp2
	.byte	0                               ;   On action: cleanup
	.uleb128 Ltmp1-Lfunc_begin0             ; >> Call Site 3 <<
	.uleb128 Lfunc_end0-Ltmp1               ;   Call between Ltmp1 and Lfunc_end0
	.byte	0                               ;     has no landing pad
	.byte	0                               ;   On action: cleanup
Lcst_end0:
	.p2align	2, 0x0
                                        ; -- End function
	.section	__TEXT,__text,regular,pure_instructions
	.private_extern	__ZNSt12length_errorC1B9nqe210106EPKc ; -- Begin function _ZNSt12length_errorC1B9nqe210106EPKc
	.globl	__ZNSt12length_errorC1B9nqe210106EPKc
	.weak_def_can_be_hidden	__ZNSt12length_errorC1B9nqe210106EPKc
	.p2align	2
__ZNSt12length_errorC1B9nqe210106EPKc:  ; @_ZNSt12length_errorC1B9nqe210106EPKc
	.cfi_startproc
; %bb.0:
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	mov	x29, sp
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	bl	__ZNSt11logic_errorC2EPKc
Lloh6:
	adrp	x8, __ZTVSt12length_error@GOTPAGE
Lloh7:
	ldr	x8, [x8, __ZTVSt12length_error@GOTPAGEOFF]
	add	x8, x8, #16
	str	x8, [x0]
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.loh AdrpLdrGot	Lloh6, Lloh7
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__cstring,cstring_literals
l_.str:                                 ; @.str
	.asciz	"vector"

.subsections_via_symbols
