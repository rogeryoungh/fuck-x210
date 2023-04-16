.global _start 
.global key_isr                   
_start:
      	@设置栈，以调用c函数      
	ldr sp, =0x40000000             
	@ 开总中断      
	mrs r0, cpsr      
	@ 读取cpsr寄存器中的值到r0
	bic r0, r0, #0x00000080     
	@ 清除第7位，IRQ中断禁止位，写0使能IRQ      
	msr cpsr, r0  	       
	@把修改好的r0的值重新赋会cpsr 
	bl main                    
	@  跳转到C函数去执行  
halt:
	b halt    
key_isr:      
@; 计算返回地址:PC的值等于当前执行的地址+8，当CPU正要执行某条指令时（还未执行），被中断， 这时这条刚要执行的指令的地址刚好=PC-4   
     sub lr, lr, #4     
     stmfd sp!, {r0-r12, lr}     
@; 保护现场    
     bl key_handle    
@; 恢复现场    
     ldmfd sp!, {r0-r12, pc}^    
@; ^表示把spsr恢复到cpsr

