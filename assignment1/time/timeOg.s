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
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subq	$48, %rsp
	.cfi_def_cfa_offset 64
	movl	$1, %esi
	leaq	32(%rsp), %rdi
	call	timespec_get
	movl	$0, %ecx
.L3:
	movl	$0, %ebx
	movl	$1, %eax
.L2:
	movl	%eax, %edx
	addq	%rdx, %rbx
	addl	$1, %eax
	cmpl	$1000000000, %eax
	jbe	.L2
	addl	$1, %ecx
	cmpl	$9, %ecx
	jbe	.L3
	movl	$1, %esi
	leaq	16(%rsp), %rdi
	call	timespec_get
	movq	16(%rsp), %rax
	subq	32(%rsp), %rax
	pxor	%xmm1, %xmm1
	cvtsi2sdq	%rax, %xmm1
	movsd	.LC0(%rip), %xmm2
	divsd	%xmm2, %xmm1
	movq	24(%rsp), %rax
	subq	40(%rsp), %rax
	pxor	%xmm0, %xmm0
	cvtsi2sdq	%rax, %xmm0
	divsd	.LC1(%rip), %xmm0
	divsd	%xmm2, %xmm0
	addsd	%xmm0, %xmm1
	movsd	%xmm1, 8(%rsp)
	movq	%rbx, %rsi
	movl	$.LC2, %edi
	movl	$0, %eax
	call	printf
	movsd	8(%rsp), %xmm0
	movl	$.LC3, %edi
	movl	$1, %eax
	call	printf
	addq	$48, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE22:
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	0
	.long	1076101120
	.align 8
.LC1:
	.long	0
	.long	1104006501
	.ident	"GCC: (GNU) 9.1.1 20190503 (Red Hat 9.1.1-1)"
	.section	.note.GNU-stack,"",@progbits
