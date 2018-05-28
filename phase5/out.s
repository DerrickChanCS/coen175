.globl	foo
foo:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$0, %esp
	movl	$1, x
	movl	$2, y
	movl	$3, z
	movl	%ebp, %esp
	popl	%ebp
	ret
.data
.comm	x, 4
.comm	y, 4
.comm	z, 4

