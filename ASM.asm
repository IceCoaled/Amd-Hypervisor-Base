

.code

extern HandleVmExit : proc


PUSHAQ macro
        push    rax
        push    rcx
        push    rdx
        push    rbx
        push    -1      ; Dummy for rsp.
        push    rbp
        push    rsi
        push    rdi
        push    r8
        push    r9
        push    r10
        push    r11
        push    r12
        push    r13
        push    r14
        push    r15
        endm


POPAQ macro
        pop     r15
        pop     r14
        pop     r13
        pop     r12
        pop     r11
        pop     r10
        pop     r9
        pop     r8
        pop     rdi
        pop     rsi
        pop     rbp
        pop     rbx    ; Dummy for rsp (this value is destroyed by the next pop).
        pop     rbx
        pop     rdx
        pop     rcx
        pop     rax
        endm


LaunchVm proc frame
       
        mov rsp, rcx    ; Rsp <= HostRsp

MainLoop:

        mov rax, [rsp]  ; RAX <= VpData->HostStackLayout.GuestVmcbPa
        vmload rax      ; load previously saved guest state from VMCB
        vmrun rax       ; Switch to the guest until #VMEXIT
        vmsave rax      ; Save current guest state to VMCB

       .pushframe
        PUSHAQ      ;rsp <<--- guestRegisters

        mov rdx, rsp                                ; Rdx <= GuestRegisters
        mov rcx, [rsp + 8 * 18]  ; Rcx <= VpData


        sub rsp, 80h
        movaps xmmword ptr [rsp + 20h], xmm0
        movaps xmmword ptr [rsp + 30h], xmm1
        movaps xmmword ptr [rsp + 40h], xmm2
        movaps xmmword ptr [rsp + 50h], xmm3
        movaps xmmword ptr [rsp + 60h], xmm4
        movaps xmmword ptr [rsp + 70h], xmm5
        .endprolog

      
        call HandleVmExit

        movaps xmm5, xmmword ptr [rsp + 70h]
        movaps xmm4, xmmword ptr [rsp + 60h]
        movaps xmm3, xmmword ptr [rsp + 50h]
        movaps xmm2, xmmword ptr [rsp + 40h]
        movaps xmm1, xmmword ptr [rsp + 30h]
        movaps xmm0, xmmword ptr [rsp + 20h]
        add rsp, 80h

        test al, al
        POPAQ       ;rsp <<--- HostRsp
       
        jnz Exit                 ; if (ExitVm != 0) jmp Exit
        jmp MainLoop             ; jmp MainLoop

Exit:

        mov rsp, rcx
        mov ecx, 'Ice'
        jmp rbx
LaunchVm endp


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


ReadEFlags proc
	pushfq
	pop rax
    ret
ReadEFlags endp


ReadRsp proc
	mov rax, rsp
    add rax, 8h
	ret
ReadRsp endp


ReadRip proc
	mov rax, [rsp]
	ret
ReadRip endp

        end
