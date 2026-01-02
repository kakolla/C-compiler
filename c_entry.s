.globl _c_entry
_c_entry:
sub sp, sp, #64
mov w0, #42
str w0, [sp, #48]
mov w0, #67
ldr w1, [sp, #48]
add w0, w0, w1
add sp, sp, #64
ret
