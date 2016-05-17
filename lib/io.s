.section .data
	_char:	.quad 0 # storage for printing
	_count:	.quad 0 # storage for printing numbers
	_argc:	.quad 0 # storage for printing numbers
	_idx:	.quad 0 # storage for printing numbers
	_read_buf:	.quad 0 # storage for reading numbers
	_bytesread:	.quad 0 # storage for reading numbers
	_read_retval:	.quad 0 # storage for reading numbers
	_read_mul:	.quad 0 # storage for reading numbers
	_read_idx:	.quad 0 # storage for reading numbers
	

.type print_char, @function
.type print_nbr, @function
.type print_str, @function
.type io.read, @function

# Prints the char in %rcx to STDOUT
print_char:
	movq $4, %rax
	movq $1, %rbx
	movq %rcx, _char
	movq $1, %rdx
	movq $_char, %rcx
	int  $0x80
	ret
	
# Prints a sequence of number
print_nbr:
	movq 8(%rsp), %rax
	movq %rax, _argc	# get argc
	addq $2, _argc	
	movq $2, _idx		# get the first argument
	args:
	movq _idx, %rax
	cmpq _argc, %rax	# Return if done
	jz return
	movq $8, %rax
	imulq _idx 
	addq %rsp, %rax 
	movq (%rax), %rax	# get the value of the first argument 
	movq $0, _count 	# initialize a char counter
	
	cmpq $0, %rax
	jge loop			# if %rax is positive, continue
	movq $45, %rcx		# otherwise, print a '-'
	pushq %rax			
	call print_char		# save rax, print, restore rax
	popq %rax
	negq %rax			# continue with the positive number
	
	
	
	
	loop:
	incq _count 
	movq $0, %rdx
	movq $10, %rbx
	idivq %rbx
	addq $48, %rdx
	pushq %rdx			# push the least significant digit as a char
	cmpq $0, %rax		# loop while !=0
	jnz loop
	prloop:
	decq _count
	popq %rcx
	call print_char		# pop the chars and print them
	cmpq $0, _count		# until all of them are done
	jnz prloop
	incq _idx
	movq _idx, %rax
	cmpq _argc, %rax
	jz return			# return if the last argument is done
	movq $32, %rcx
	call print_char		# otherwise print a space and jump to the next argument
	jmp args
	return:
	ret
print_str:
	movq 8(%rsp), %rax
	movq %rax, _argc		# get argc
	addq $2, _argc
	movq $2, _idx 			# set idx to the first arg
argsprintstr:
	movq _idx, %rax
	cmpq _argc, %rax
	jz return
	movq $8, %rax
	imulq _idx				# compute the offset of the current arg
	addq %rsp, %rax
	movq (%rax), %rax		# adress of the length
	movq (%rax), %rdx		# value of the length
	addq $8, %rax			# the string is stored right after its length
	movq %rax, %rcx			# pointer to the string
	movq $4, %rax
	movq $1, %rbx
	int  $0x80				# write
	incq _idx				# increment to go to the next arg 
	movq _idx, %rax
	cmpq _argc, %rax
	jz return				#return if we are done
	movq $32, %rcx			# print a space & go to the next arg to print
	call print_char
	jmp argsprintstr
io.read:
	movq $8, %rdx
	mov $_read_buf, %rcx
	movq $0, %rbx
	movq $3, %rax
	int $0x80				# read chars in the buffer
	movq %rax, _bytesread
	movq $0, _read_retval	# retval = 0
	movq $1, _read_mul		# mul = 1
	movq _bytesread, %rax
	movq %rax, _read_idx	# idx = strlen
	decq _read_idx			# idx--
	read_beg:
	decq _read_idx			# idx--
	movq $_read_buf, %rax
	addq _read_idx, %rax
	movq (%rax), %rax		
	andq $0xff, %rax		# rax = string[idx]
	
	cmpq $45, %rax			# if rax == '-' then retval = -retval, and skip the current char
	jnz read_skip_sign
	movq _read_retval, %rax
	negq %rax				
	movq %rax, _read_retval
	
	jmp read_end			# return the current retval
	
	read_skip_sign:
	subq $48, %rax			# rax = intval(rax)
	imulq _read_mul, %rax	# rax*=mul
	addq %rax, _read_retval	# retval+=rax
	movq _read_mul, %rcx
	imulq $10, %rcx
	movq %rcx, _read_mul	# mul*=10
	
	
	cmpq $0, _read_idx		# continue while idx>0
	jnz read_beg
	
	read_end:
	movq _read_retval, %rax # return
	ret
