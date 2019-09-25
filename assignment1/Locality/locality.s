	.file	"locality.c"
	.text
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC3:
	.string	"Time taken for row sums by timespec_get is %lf\n"
	.align 8
.LC4:
	.string	"Time taken for col sums by  timespec_get is %lf\n"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC5:
	.string	"first 5 Row sums array"
.LC6:
	.string	"%.2lf\t"
.LC7:
	.string	"first 5 column sum array"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB22:
	.cfi_startproc
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	movl	$8000, %edi
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$56, %rsp
	.cfi_def_cfa_offset 112
	call	malloc
	movl	$8000000, %edi
	movq	%rax, %rbp
	call	malloc
	movq	%rbp, %rdx
	movq	%rax, 8(%rsp)
	leaq	8000000(%rax), %rcx
.L2:
	movq	%rax, (%rdx)
	addq	$8000, %rax
	addq	$8, %rdx
	cmpq	%rax, %rcx
	jne	.L2
	movl	$8000, %edi
	leaq	8000(%rbp), %rbx
	movq	%rbp, %r14
	call	malloc
	movl	$8000, %edi
	movq	%rax, %r13
	call	malloc
	movq	%rax, %r12
.L3:
	xorl	%r15d, %r15d
	.p2align 4,,10
	.p2align 3
.L4:
	call	rand
	pxor	%xmm0, %xmm0
	movq	(%r14), %rsi
	movslq	%eax, %rdx
	movl	%eax, %edi
	imulq	$1374389535, %rdx, %rdx
	sarl	$31, %edi
	sarq	$37, %rdx
	subl	%edi, %edx
	imull	$100, %edx, %edx
	subl	%edx, %eax
	addl	$1, %eax
	cvtsi2sdl	%eax, %xmm0
	movsd	%xmm0, (%rsi,%r15)
	addq	$8, %r15
	cmpq	$8000, %r15
	jne	.L4
	addq	$8, %r14
	cmpq	%r14, %rbx
	jne	.L3
	movl	$1, %esi
	leaq	16(%rsp), %rdi
	call	timespec_get
	movl	$10000, %esi
	pxor	%xmm1, %xmm1
	.p2align 4,,10
	.p2align 3
.L6:
	xorl	%ecx, %ecx
	.p2align 4,,10
	.p2align 3
.L10:
	movq	0(%rbp,%rcx,8), %rax
	movapd	%xmm1, %xmm0
	leaq	8000(%rax), %rdx
	.p2align 4,,10
	.p2align 3
.L7:
	addsd	(%rax), %xmm0
	addq	$8, %rax
	cmpq	%rax, %rdx
	jne	.L7
	movsd	%xmm0, 0(%r13,%rcx,8)
	addq	$1, %rcx
	cmpq	$1000, %rcx
	jne	.L10
	subl	$1, %esi
	jne	.L6
	movl	$1, %esi
	leaq	32(%rsp), %rdi
	call	timespec_get
	movq	40(%rsp), %rax
	pxor	%xmm0, %xmm0
	subq	24(%rsp), %rax
	cvtsi2sdq	%rax, %xmm0
	pxor	%xmm2, %xmm2
	movq	32(%rsp), %rax
	subq	16(%rsp), %rax
	cvtsi2sdq	%rax, %xmm2
	movl	$.LC3, %edi
	movl	$1, %eax
	divsd	.LC2(%rip), %xmm2
	divsd	.LC1(%rip), %xmm0
	divsd	.LC2(%rip), %xmm0
	addsd	%xmm2, %xmm0
	call	printf
	movl	$1, %esi
	leaq	16(%rsp), %rdi
	call	timespec_get
	movl	$10000, %esi
	pxor	%xmm1, %xmm1
	.p2align 4,,10
	.p2align 3
.L11:
	xorl	%ecx, %ecx
	.p2align 4,,10
	.p2align 3
.L15:
	movq	%rbp, %rax
	movapd	%xmm1, %xmm0
	.p2align 4,,10
	.p2align 3
.L12:
	movq	(%rax), %rdx
	addq	$8, %rax
	addsd	(%rdx,%rcx), %xmm0
	cmpq	%rax, %rbx
	jne	.L12
	movsd	%xmm0, (%r12,%rcx)
	addq	$8, %rcx
	cmpq	$8000, %rcx
	jne	.L15
	subl	$1, %esi
	jne	.L11
	movl	$1, %esi
	leaq	32(%rsp), %rdi
	leaq	40(%r13), %r15
	movq	%r13, %rbx
	call	timespec_get
	movq	40(%rsp), %rax
	pxor	%xmm0, %xmm0
	subq	24(%rsp), %rax
	cvtsi2sdq	%rax, %xmm0
	pxor	%xmm1, %xmm1
	movq	32(%rsp), %rax
	subq	16(%rsp), %rax
	cvtsi2sdq	%rax, %xmm1
	movl	$.LC4, %edi
	movl	$1, %eax
	divsd	.LC2(%rip), %xmm1
	divsd	.LC1(%rip), %xmm0
	divsd	.LC2(%rip), %xmm0
	addsd	%xmm1, %xmm0
	call	printf
	movl	$.LC5, %edi
	call	puts
.L16:
	movsd	(%rbx), %xmm0
	movl	$.LC6, %edi
	movl	$1, %eax
	addq	$8, %rbx
	call	printf
	cmpq	%r15, %rbx
	jne	.L16
	movl	$10, %edi
	movq	%r12, %rbx
	leaq	40(%r12), %r15
	call	putchar
	movl	$.LC7, %edi
	call	puts
.L17:
	movsd	(%rbx), %xmm0
	movl	$.LC6, %edi
	movl	$1, %eax
	addq	$8, %rbx
	call	printf
	cmpq	%rbx, %r15
	jne	.L17
	movl	$10, %edi
	call	putchar
	movq	%rbp, %rdi
	call	free
	movq	%r13, %rdi
	call	free
	movq	%r12, %rdi
	call	free
	movq	8(%rsp), %rdi
	addq	$56, %rsp
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	jmp	free
	.cfi_endproc
.LFE22:
	.size	main, .-main
	.text
	.p2align 4
	.globl	row_sums
	.type	row_sums, @function
row_sums:
.LFB23:
	.cfi_startproc
	movq	%rdi, %r10
	testq	%rdx, %rdx
	je	.L27
	leaq	0(,%rcx,8), %rdi
	xorl	%r9d, %r9d
	pxor	%xmm1, %xmm1
	.p2align 4,,10
	.p2align 3
.L28:
	movapd	%xmm1, %xmm0
	testq	%rcx, %rcx
	je	.L31
	movq	(%rsi,%r9,8), %rax
	movapd	%xmm1, %xmm0
	leaq	(%rdi,%rax), %r8
	.p2align 4,,10
	.p2align 3
.L29:
	addsd	(%rax), %xmm0
	addq	$8, %rax
	cmpq	%r8, %rax
	jne	.L29
.L31:
	movsd	%xmm0, (%r10,%r9,8)
	addq	$1, %r9
	cmpq	%r9, %rdx
	jne	.L28
.L27:
	movq	%r10, %rax
	ret
	.cfi_endproc
.LFE23:
	.size	row_sums, .-row_sums
	.p2align 4
	.globl	col_sums
	.type	col_sums, @function
col_sums:
.LFB24:
	.cfi_startproc
	movq	%rdi, %r11
	testq	%rcx, %rcx
	je	.L37
	leaq	0(,%rcx,8), %r10
	leaq	(%rsi,%rdx,8), %r9
	pxor	%xmm1, %xmm1
	xorl	%r8d, %r8d
	.p2align 4,,10
	.p2align 3
.L38:
	movq	%rsi, %rax
	movapd	%xmm1, %xmm0
	testq	%rdx, %rdx
	je	.L40
	.p2align 4,,10
	.p2align 3
.L39:
	movq	(%rax), %rcx
	addq	$8, %rax
	addsd	(%rcx,%r8), %xmm0
	cmpq	%rax, %r9
	jne	.L39
.L40:
	movsd	%xmm0, (%r11,%r8)
	addq	$8, %r8
	cmpq	%r10, %r8
	jne	.L38
.L37:
	movq	%r11, %rax
	ret
	.cfi_endproc
.LFE24:
	.size	col_sums, .-col_sums
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC1:
	.long	0
	.long	1104006501
	.align 8
.LC2:
	.long	0
	.long	1086556160
	.ident	"GCC: (GNU) 9.1.1 20190503 (Red Hat 9.1.1-1)"
	.section	.note.GNU-stack,"",@progbits
