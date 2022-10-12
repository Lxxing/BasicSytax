TITLE EXAMPLE
include irvine32.inc

.data
 str1 byte "Hello World!", 0

.code
	main proc
	mov  edx, offset str1
	call  writestring
	call  crlf
	exit
 	main endp	
end main

