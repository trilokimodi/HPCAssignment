	.file	"time.c"
	.text
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC2:
	.string	"The sum of billion integers = %lu\n"
	.align 8
.LC3:
	.string	"Time taken for one single iteration by timespec_get is %lf\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB22:
	.cfi_startproc
	subq	$56, %rsp
	.cfi_def_cfa_offset 64
	movl	$1, %esi
	leaq	32(%rsp), %rdi
	call	timespec_get
	movl	$10, %edx
.L3:
	movl	$1000000000, %eax
.L2:
	subl	$1, %eax
	jne	.L2
	subl	$1, %edx
	jne	.L3
	movl	$1, %esi
	leaq	16(%rsp), %rdi
	call	timespec_get
	movq	24(%rsp), %rax
	subq	40(%rsp), %rax
	pxor	%xmm0, %xmm0
	cvtsi2sdq	%rax, %xmm0
	divsd	.LC0(%rip), %xmm0
	movsd	.LC1(%rip), %xmm2
	divsd	%xmm2, %xmm0
	movq	16(%rsp), %rax
	subq	32(%rsp), %rax
	pxor	%xmm1, %xmm1
	cvtsi2sdq	%rax, %xmm1
	divsd	%xmm2, %xmm1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, 8(%rsp)
	movabsq	$500000000500000000, %rsi
	movl	$.LC2, %edi
	movl	$0, %eax
	call	printf
	movsd	8(%rsp), %xmm0
	movl	$.LC3, %edi
	movl	$1, %eax
	call	printf
	addq	$56, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE22:
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
