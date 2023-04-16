```bash
$ qemu-system-arm -M vexpress-a9 -kernel ./3rd/u-boot/u-boot -nographic -append "console=ttyAMA0,115200"

$ sudo uftpd -n -o ftp=0,tftp=69  .
```


```bash
=> dhcp
=> setenv serverip 192.168.225.215
=> tftp 0x60020010 uart.bin
=> go 0x60020010
```