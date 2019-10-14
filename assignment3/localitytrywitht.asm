	.file	"localitytry.c"
	.text
	.section	.rodata
	.align 8
.LC1:
	.string	"Time taken for one single iteration by timespec_get is %lf\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$120096, %rsp
	movl	$1000, -4(%rbp)
	leaq	-120064(%rbp), %rax
	movl	$120000, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	call	memset
	movl	$1, -120064(%rbp)
	leaq	-120080(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	timespec_get
.L6:
	movq	$0, -16(%rbp)
	jmp	.L2
.L5:
	movq	-16(%rbp), %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$2, %rax
	addq	%rbp, %rax
	subq	$120064, %rax
	movl	(%rax), %eax
	movl	%eax, -36(%rbp)
	movq	-16(%rbp), %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$2, %rax
	addq	%rbp, %rax
	subq	$120056, %rax
	movl	(%rax), %eax
	movl	%eax, -40(%rbp)
	movq	-16(%rbp), %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$2, %rax
	addq	%rbp, %rax
	subq	$120056, %rax
	movl	(%rax), %eax
	movl	%eax, -44(%rbp)
	movq	-16(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -24(%rbp)
	jmp	.L3
.L4:
	movq	-24(%rbp), %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$2, %rax
	addq	%rbp, %rax
	subq	$120064, %rax
	movl	(%rax), %eax
	movl	-36(%rbp), %edx
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, -48(%rbp)
	movq	-24(%rbp), %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$2, %rax
	addq	%rbp, %rax
	subq	$120060, %rax
	movl	(%rax), %eax
	movl	-40(%rbp), %edx
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, -52(%rbp)
	movq	-24(%rbp), %rdx
	movq	%rdx, %rax
	addq	%rax, %rax
	addq	%rdx, %rax
	salq	$2, %rax
	addq	%rbp, %rax
	subq	$120056, %rax
	movl	(%rax), %eax
	movl	-44(%rbp), %edx
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, -56(%rbp)
	movl	-48(%rbp), %eax
	imull	%eax, %eax
	movl	%eax, %edx
	movl	-52(%rbp), %eax
	imull	%eax, %eax
	addl	%eax, %edx
	movl	-56(%rbp), %eax
	imull	%eax, %eax
	addl	%edx, %eax
	cvtsi2ssl	%eax, %xmm0
	call	sqrtf
	movd	%xmm0, %eax
	movl	%eax, -60(%rbp)
	addq	$1, -24(%rbp)
.L3:
	cmpq	$9999, -24(%rbp)
	jbe	.L4
	addq	$1, -16(%rbp)
.L2:
	cmpq	$9999, -16(%rbp)
	jbe	.L5
	subl	$1, -4(%rbp)
	cmpl	$0, -4(%rbp)
	jg	.L6
	leaq	-120096(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	timespec_get
	movq	-120096(%rbp), %rdx
	movq	-120080(%rbp), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	cvtsi2sdq	%rax, %xmm1
	movq	-120088(%rbp), %rdx
	movq	-120072(%rbp), %rax
	subq	%rax, %rdx
	movq	%rdx, %rax
	cvtsi2sdq	%rax, %xmm0
	movsd	.LC0(%rip), %xmm2
	divsd	%xmm2, %xmm0
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -32(%rbp)
	movq	-32(%rbp), %rax
	movq	%rax, %xmm0
	movl	$.LC1, %edi
	movl	$1, %eax
	call	printf
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC0:
	.long	0
	.long	1104006501
	.ident	"GCC: (GNU) 9.1.1 20190503 (Red Hat 9.1.1-1)"
	.section	.note.GNU-stack,"",@progbits
