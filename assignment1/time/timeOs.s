	.file	"time.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC2:
	.string	"The sum of billion integers = %lu\n"
.LC3:
	.string	"Time taken for one single iteration by timespec_get is %lf\n"
	.section	.text.startup,"ax",@progbits
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	subq	$56, %rsp
	.cfi_def_cfa_offset 64
	movl	$1, %esi
	leaq	16(%rsp), %rdi
	call	timespec_get
	leaq	32(%rsp), %rdi
	movl	$1, %esi
	call	timespec_get
	movq	40(%rsp), %rax
	subq	24(%rsp), %rax
	movabsq	$500000000500000000, %rsi
	cvtsi2sdq	%rax, %xmm0
	movq	32(%rsp), %rax
	subq	16(%rsp), %rax
	divsd	.LC0(%rip), %xmm0
	cvtsi2sdq	%rax, %xmm1
	movsd	.LC1(%rip), %xmm2
	movl	$.LC2, %edi
	xorl	%eax, %eax
	divsd	%xmm2, %xmm0
	divsd	%xmm2, %xmm1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, 8(%rsp)
	call	printf
	movsd	8(%rsp), %xmm0
	movl	$.LC3, %edi
	movb	$1, %al
	call	printf
	addq	$56, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	0
	.long	1104006501
	.align 8
.LC1:
	.long	0
	.long	1076101120
	.ident	"GCC: (GNU) 9.1.1 20190503 (Red Hat 9.1.1-1)"
	.section	.note.GNU-stack,"",@progbits
