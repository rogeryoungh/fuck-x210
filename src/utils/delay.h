void delay(volatile unsigned int t) {
  volatile unsigned int t2 = 0xFFFF;
  while (t--)
    for (; t2; t2--)
      ;
}
