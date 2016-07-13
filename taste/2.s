	.file	"2.c"
	.section	.rodata
.LC0:
	.string	"lv is %d\n"
.LC1:
	.string	"gv is %d\n"
	.text
.globl main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	movq	%rsp, %rbp
	.cfi_offset 6, -16
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$1, -4(%rbp)
	movl	$4, %edi
	call	malloc
	movq	%rax, -16(%rbp)
	movq	-16(%rbp), %rax
	movl	$2, (%rax)
	movl	$.LC0, %eax
	movl	-4(%rbp), %edx
	movl	%edx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	movq	-16(%rbp), %rax
	movl	(%rax), %edx
	movl	$.LC1, %eax
	movl	%edx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	movl	$0, %eax
	leave
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.4.7-8ubuntu1) 4.4.7"
	.section	.note.GNU-stack,"",@progbits
