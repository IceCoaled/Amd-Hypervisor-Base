include Macros.inc

.code

extern HandleVmExit : proc


; __stdcall uses RCX, RDX, R8, R9 in this order
VmEnter proc
                        ;          guestRegisters
        mov rsp, rcx    ;rsp <<--- guestVmcbPa 
                        ;          hostVmcbPa
EnterLoop:              
           
        mov rax, [rsp]  
        vmload rax          
        vmrun rax          
        vmsave rax   

        mov rax, [rsp + 8h]
        vmload rax
           
        PUSH_XMM       ;rsp <<--- guestRegisters->xmm0
        PUSH_GPRS      ;rsp <<--- guestRegisters->rax
         
        mov rcx, [rsp + 190h]   ;rsp + 190h = hvProcessor    
        call HandleVmExit   
         
        POP_GPRS        ;rsp <<--- guestRegisters-> xmm0
        POP_XMM         ;rsp <<--- guestVmcbPa
         
           
        jmp EnterLoop   
vmenter endp


ReadRsp proc
        mov rax, [rsp]
        ret
ReadRsp endp


ReadRip proc
        mov rax, rsp
        add rax, 8h
        ret
ReadRip endp


ReadCs proc
        mov ax, cs
        ret
ReadCs endp


ReadSs proc
	mov ax, ss
	ret
ReadSs endp


ReadDs proc
	mov ax, ds
	ret
ReadDs endp


ReadEs proc
	mov ax, es
	ret
ReadEs endp


end