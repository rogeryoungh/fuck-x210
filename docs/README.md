# fuck-x210

original 下是给的代码，src 下是我写的。

由于我调通之后也不会回来改，有些代码可能要你自己改。

## 没啥用

失败的尝试。

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
