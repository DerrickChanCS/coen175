insert:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$insert.size, %esp
	movl	8(%ebp), %eax
	cmpl	$0, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -4(%EBP)
	movl	-4(%EBP), %eax
	cmpl	$0, %eax
	je	.L1	#jump to else
	movl	$3, %eax
	imull	$4, %eax
	movl	%eax, -8(%EBP)
	pushl	-8(%EBP)
	call	malloc
	addl	$4, %esp
	movl	%eax, -12(%EBP)
	movl	-12(%EBP), %eax
	movl	%eax, 8(%ebp)
	movl	$0, %eax
	imull	$4, %eax
	movl	%eax, -16(%EBP)
	movl	8(%ebp), %eax
	addl	-16(%EBP), %eax
	movl	%eax, -20(%EBP)
	movl	12(%ebp), %eax
	movl	-20(%EBP), %ecx
	movl	%eax, (%ecx)
	movl	$1, %eax
	imull	$4, %eax
	movl	%eax, -24(%EBP)
	movl	8(%ebp), %eax
	addl	-24(%EBP), %eax
	movl	%eax, -28(%EBP)
	movl	null, %eax
	movl	-28(%EBP), %ecx
	movl	%eax, (%ecx)
	movl	$2, %eax
	imull	$4, %eax
	movl	%eax, -32(%EBP)
	movl	8(%ebp), %eax
	addl	-32(%EBP), %eax
	movl	%eax, -36(%EBP)
	movl	null, %eax
	movl	-36(%EBP), %ecx
	movl	%eax, (%ecx)
	jmp	.L2	#jump to end of if
.L1:	#else label
	movl	$0, %eax
	imull	$4, %eax
	movl	%eax, -40(%EBP)
	movl	8(%ebp), %eax
	addl	-40(%EBP), %eax
	movl	%eax, -44(%EBP)
	movl	-44(%EBP), %eax
	movl	(%eax), %eax
	movl	%eax, -48(%EBP)
	movl	12(%ebp), %eax
	cmpl	-48(%EBP), %eax
	setl	%al
	movzbl	%al, %eax
	movl	%eax, -52(%EBP)
	movl	-52(%EBP), %eax
	cmpl	$0, %eax
	je	.L3	#jump to else
	pushl	12(%ebp)
	movl	$1, %eax
	imull	$4, %eax
	movl	%eax, -56(%EBP)
	movl	8(%ebp), %eax
	addl	-56(%EBP), %eax
	movl	%eax, -60(%EBP)
	movl	-60(%EBP), %eax
	movl	(%eax), %eax
	movl	%eax, -64(%EBP)
	pushl	-64(%EBP)
	call	insert
	addl	$8, %esp
	movl	%eax, -68(%EBP)
	movl	$1, %eax
	imull	$4, %eax
	movl	%eax, -72(%EBP)
	movl	8(%ebp), %eax
	addl	-72(%EBP), %eax
	movl	%eax, -76(%EBP)
	movl	-68(%EBP), %eax
	movl	-76(%EBP), %ecx
	movl	%eax, (%ecx)
	jmp	.L4	#jump to end of if
.L3:	#else label
	movl	$0, %eax
	imull	$4, %eax
	movl	%eax, -80(%EBP)
	movl	8(%ebp), %eax
	addl	-80(%EBP), %eax
	movl	%eax, -84(%EBP)
	movl	-84(%EBP), %eax
	movl	(%eax), %eax
	movl	%eax, -88(%EBP)
	movl	12(%ebp), %eax
	cmpl	-88(%EBP), %eax
	setg	%al
	movzbl	%al, %eax
	movl	%eax, -92(%EBP)
	movl	-92(%EBP), %eax
	cmpl	$0, %eax
	je	.L5	#jump to else
	pushl	12(%ebp)
	movl	$2, %eax
	imull	$4, %eax
	movl	%eax, -96(%EBP)
	movl	8(%ebp), %eax
	addl	-96(%EBP), %eax
	movl	%eax, -100(%EBP)
	movl	-100(%EBP), %eax
	movl	(%eax), %eax
	movl	%eax, -104(%EBP)
	pushl	-104(%EBP)
	call	insert
	addl	$8, %esp
	movl	%eax, -108(%EBP)
	movl	$2, %eax
	imull	$4, %eax
	movl	%eax, -112(%EBP)
	movl	8(%ebp), %eax
	addl	-112(%EBP), %eax
	movl	%eax, -116(%EBP)
	movl	-108(%EBP), %eax
	movl	-116(%EBP), %ecx
	movl	%eax, (%ecx)
	jmp	.L6	#jump to end of if
.L5:	#else label
.L6:	# end of if
.L4:	# end of if
.L2:	# end of if
	movl	8(%ebp), %eax
	jmp	.L0
.L0:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	insert
	.set	insert.size, 116

search:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$search.size, %esp
	movl	8(%ebp), %eax
	cmpl	$0, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -4(%EBP)
	movl	-4(%EBP), %eax
	cmpl	$0, %eax
	je	.L8	#jump to else
	movl	$0, %eax
	jmp	.L7
	jmp	.L9	#jump to end of if
.L8:	#else label
.L9:	# end of if
	movl	$0, %eax
	imull	$4, %eax
	movl	%eax, -8(%EBP)
	movl	8(%ebp), %eax
	addl	-8(%EBP), %eax
	movl	%eax, -12(%EBP)
	movl	-12(%EBP), %eax
	movl	(%eax), %eax
	movl	%eax, -16(%EBP)
	movl	12(%ebp), %eax
	cmpl	-16(%EBP), %eax
	setl	%al
	movzbl	%al, %eax
	movl	%eax, -20(%EBP)
	movl	-20(%EBP), %eax
	cmpl	$0, %eax
	je	.L10	#jump to else
	pushl	12(%ebp)
	movl	$1, %eax
	imull	$4, %eax
	movl	%eax, -24(%EBP)
	movl	8(%ebp), %eax
	addl	-24(%EBP), %eax
	movl	%eax, -28(%EBP)
	movl	-28(%EBP), %eax
	movl	(%eax), %eax
	movl	%eax, -32(%EBP)
	pushl	-32(%EBP)
	call	search
	addl	$8, %esp
	movl	%eax, -36(%EBP)
	movl	-36(%EBP), %eax
	jmp	.L7
	jmp	.L11	#jump to end of if
.L10:	#else label
.L11:	# end of if
	movl	$0, %eax
	imull	$4, %eax
	movl	%eax, -40(%EBP)
	movl	8(%ebp), %eax
	addl	-40(%EBP), %eax
	movl	%eax, -44(%EBP)
	movl	-44(%EBP), %eax
	movl	(%eax), %eax
	movl	%eax, -48(%EBP)
	movl	12(%ebp), %eax
	cmpl	-48(%EBP), %eax
	setg	%al
	movzbl	%al, %eax
	movl	%eax, -52(%EBP)
	movl	-52(%EBP), %eax
	cmpl	$0, %eax
	je	.L12	#jump to else
	pushl	12(%ebp)
	movl	$2, %eax
	imull	$4, %eax
	movl	%eax, -56(%EBP)
	movl	8(%ebp), %eax
	addl	-56(%EBP), %eax
	movl	%eax, -60(%EBP)
	movl	-60(%EBP), %eax
	movl	(%eax), %eax
	movl	%eax, -64(%EBP)
	pushl	-64(%EBP)
	call	search
	addl	$8, %esp
	movl	%eax, -68(%EBP)
	movl	-68(%EBP), %eax
	jmp	.L7
	jmp	.L13	#jump to end of if
.L12:	#else label
.L13:	# end of if
	movl	$1, %eax
	jmp	.L7
.L7:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	search
	.set	search.size, 68

preorder:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$preorder.size, %esp
	movl	8(%ebp), %eax
	cmpl	$0, %eax
	je	.L15	#jump to else
	movl	$0, %eax
	imull	$4, %eax
	movl	%eax, -8(%EBP)
	movl	8(%ebp), %eax
	addl	-8(%EBP), %eax
	movl	%eax, -12(%EBP)
	movl	-12(%EBP), %eax
	movl	(%eax), %eax
	movl	%eax, -16(%EBP)
	movl	-16(%EBP), %eax
	movl	%eax, -4(%ebp)
	movl	-4(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -20(%EBP)
	pushl	-20(%EBP)
.data
.L17: .asciz "%d\n"
.text
	leal	.L17, %eax
	movl	%eax, -24(%EBP)
	pushl	-24(%EBP)
	call	printf
	addl	$8, %esp
	movl	%eax, -28(%EBP)
	movl	$1, %eax
	imull	$4, %eax
	movl	%eax, -32(%EBP)
	movl	8(%ebp), %eax
	addl	-32(%EBP), %eax
	movl	%eax, -36(%EBP)
	movl	-36(%EBP), %eax
	movl	(%eax), %eax
	movl	%eax, -40(%EBP)
	pushl	-40(%EBP)
	call	preorder
	addl	$4, %esp
	movl	%eax, -40(%EBP)
	movl	$2, %eax
	imull	$4, %eax
	movl	%eax, -44(%EBP)
	movl	8(%ebp), %eax
	addl	-44(%EBP), %eax
	movl	%eax, -48(%EBP)
	movl	-48(%EBP), %eax
	movl	(%eax), %eax
	movl	%eax, -52(%EBP)
	pushl	-52(%EBP)
	call	preorder
	addl	$4, %esp
	movl	%eax, -52(%EBP)
	jmp	.L16	#jump to end of if
.L15:	#else label
.L16:	# end of if
.L14:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	preorder
	.set	preorder.size, 52

inorder:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$inorder.size, %esp
	movl	8(%ebp), %eax
	cmpl	$0, %eax
	je	.L19	#jump to else
	movl	$0, %eax
	imull	$4, %eax
	movl	%eax, -8(%EBP)
	movl	8(%ebp), %eax
	addl	-8(%EBP), %eax
	movl	%eax, -12(%EBP)
	movl	-12(%EBP), %eax
	movl	(%eax), %eax
	movl	%eax, -16(%EBP)
	movl	-16(%EBP), %eax
	movl	%eax, -4(%ebp)
	movl	$1, %eax
	imull	$4, %eax
	movl	%eax, -20(%EBP)
	movl	8(%ebp), %eax
	addl	-20(%EBP), %eax
	movl	%eax, -24(%EBP)
	movl	-24(%EBP), %eax
	movl	(%eax), %eax
	movl	%eax, -28(%EBP)
	pushl	-28(%EBP)
	call	inorder
	addl	$4, %esp
	movl	%eax, -28(%EBP)
	movl	-4(%ebp), %eax
	movl	(%eax), %eax
	movl	%eax, -32(%EBP)
	pushl	-32(%EBP)
.data
.L21: .asciz "%d\n"
.text
	leal	.L21, %eax
	movl	%eax, -36(%EBP)
	pushl	-36(%EBP)
	call	printf
	addl	$8, %esp
	movl	%eax, -40(%EBP)
	movl	$2, %eax
	imull	$4, %eax
	movl	%eax, -44(%EBP)
	movl	8(%ebp), %eax
	addl	-44(%EBP), %eax
	movl	%eax, -48(%EBP)
	movl	-48(%EBP), %eax
	movl	(%eax), %eax
	movl	%eax, -52(%EBP)
	pushl	-52(%EBP)
	call	inorder
	addl	$4, %esp
	movl	%eax, -52(%EBP)
	jmp	.L20	#jump to end of if
.L19:	#else label
.L20:	# end of if
.L18:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	inorder
	.set	inorder.size, 52

main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$main.size, %esp
	movl	$0, %eax
	movl	%eax, -48(%ebp)
.L23:	#While loop start
	movl	-48(%ebp), %eax
	cmpl	$8, %eax
	setl	%al
	movzbl	%al, %eax
	movl	%eax, -52(%EBP)
	movl	-52(%EBP), %eax
	cmpl	$0, %eax
	je	.L24
	leal	-44(%ebp), %eax
	movl	%eax, -56(%EBP)
	movl	-48(%ebp), %eax
	imull	$4, %eax
	movl	%eax, -60(%EBP)
	movl	-56(%EBP), %eax
	addl	-60(%EBP), %eax
	movl	%eax, -64(%EBP)
	movl	-48(%ebp), %eax
	movl	-64(%EBP), %ecx
	movl	%eax, (%ecx)
	movl	-48(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -68(%EBP)
	movl	-68(%EBP), %eax
	movl	%eax, -48(%ebp)
	jmp	.L23
.L24:
	movl	null, %eax
	movl	%eax, -4(%ebp)
	leal	-44(%ebp), %eax
	movl	%eax, -72(%EBP)
	movl	$7, %eax
	imull	$4, %eax
	movl	%eax, -76(%EBP)
	movl	-72(%EBP), %eax
	addl	-76(%EBP), %eax
	movl	%eax, -80(%EBP)
	pushl	-80(%EBP)
	pushl	-4(%ebp)
	call	insert
	addl	$8, %esp
	movl	%eax, -88(%EBP)
	movl	-88(%EBP), %eax
	movl	%eax, -4(%ebp)
	leal	-44(%ebp), %eax
	movl	%eax, -92(%EBP)
	movl	$4, %eax
	imull	$4, %eax
	movl	%eax, -96(%EBP)
	movl	-92(%EBP), %eax
	addl	-96(%EBP), %eax
	movl	%eax, -100(%EBP)
	pushl	-100(%EBP)
	pushl	-4(%ebp)
	call	insert
	addl	$8, %esp
	movl	%eax, -108(%EBP)
	movl	-108(%EBP), %eax
	movl	%eax, -4(%ebp)
	leal	-44(%ebp), %eax
	movl	%eax, -112(%EBP)
	movl	$1, %eax
	imull	$4, %eax
	movl	%eax, -116(%EBP)
	movl	-112(%EBP), %eax
	addl	-116(%EBP), %eax
	movl	%eax, -120(%EBP)
	pushl	-120(%EBP)
	pushl	-4(%ebp)
	call	insert
	addl	$8, %esp
	movl	%eax, -128(%EBP)
	movl	-128(%EBP), %eax
	movl	%eax, -4(%ebp)
	leal	-44(%ebp), %eax
	movl	%eax, -132(%EBP)
	movl	$0, %eax
	imull	$4, %eax
	movl	%eax, -136(%EBP)
	movl	-132(%EBP), %eax
	addl	-136(%EBP), %eax
	movl	%eax, -140(%EBP)
	pushl	-140(%EBP)
	pushl	-4(%ebp)
	call	insert
	addl	$8, %esp
	movl	%eax, -148(%EBP)
	movl	-148(%EBP), %eax
	movl	%eax, -4(%ebp)
	leal	-44(%ebp), %eax
	movl	%eax, -152(%EBP)
	movl	$5, %eax
	imull	$4, %eax
	movl	%eax, -156(%EBP)
	movl	-152(%EBP), %eax
	addl	-156(%EBP), %eax
	movl	%eax, -160(%EBP)
	pushl	-160(%EBP)
	pushl	-4(%ebp)
	call	insert
	addl	$8, %esp
	movl	%eax, -168(%EBP)
	movl	-168(%EBP), %eax
	movl	%eax, -4(%ebp)
	leal	-44(%ebp), %eax
	movl	%eax, -172(%EBP)
	movl	$2, %eax
	imull	$4, %eax
	movl	%eax, -176(%EBP)
	movl	-172(%EBP), %eax
	addl	-176(%EBP), %eax
	movl	%eax, -180(%EBP)
	pushl	-180(%EBP)
	pushl	-4(%ebp)
	call	insert
	addl	$8, %esp
	movl	%eax, -188(%EBP)
	movl	-188(%EBP), %eax
	movl	%eax, -4(%ebp)
	leal	-44(%ebp), %eax
	movl	%eax, -192(%EBP)
	movl	$3, %eax
	imull	$4, %eax
	movl	%eax, -196(%EBP)
	movl	-192(%EBP), %eax
	addl	-196(%EBP), %eax
	movl	%eax, -200(%EBP)
	pushl	-200(%EBP)
	pushl	-4(%ebp)
	call	insert
	addl	$8, %esp
	movl	%eax, -208(%EBP)
	movl	-208(%EBP), %eax
	movl	%eax, -4(%ebp)
	leal	-44(%ebp), %eax
	movl	%eax, -212(%EBP)
	movl	$6, %eax
	imull	$4, %eax
	movl	%eax, -216(%EBP)
	movl	-212(%EBP), %eax
	addl	-216(%EBP), %eax
	movl	%eax, -220(%EBP)
	pushl	-220(%EBP)
	pushl	-4(%ebp)
	call	insert
	addl	$8, %esp
	movl	%eax, -228(%EBP)
	movl	-228(%EBP), %eax
	movl	%eax, -4(%ebp)
.data
.L25: .asciz "preorder traversal:\n"
.text
	leal	.L25, %eax
	movl	%eax, -232(%EBP)
	pushl	-232(%EBP)
	call	printf
	addl	$4, %esp
	movl	%eax, -236(%EBP)
	pushl	-4(%ebp)
	call	preorder
	addl	$4, %esp
	movl	%eax, -236(%EBP)
.data
.L26: .asciz "inorder traversal:\n"
.text
	leal	.L26, %eax
	movl	%eax, -240(%EBP)
	pushl	-240(%EBP)
	call	printf
	addl	$4, %esp
	movl	%eax, -244(%EBP)
	pushl	-4(%ebp)
	call	inorder
	addl	$4, %esp
	movl	%eax, -244(%EBP)
.L22:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.globl	main
	.set	main.size, 244

	.data
	.comm	null, 4, 4
