.global _start
_start:
  @ bl clock_init
  bl uart_init
  bl main

halt:
  b halt
