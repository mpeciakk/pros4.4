global _loader
extern kmain

MULTIBOOT_PAGE_ALIGN equ 1 << 0
MULTIBOOT_MEMINFO    equ 1 << 1
MULTIBOOT_MAGIC      equ 0x1BADB002
MULTIBOOT_FLAGS	     equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMINFO
MULTIBOOT_CHECKSUM   equ -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)

KERNEL_VIRTUAL_BASE  equ 0xC0000000
KERNEL_PAGE_NUMBER   equ (KERNEL_VIRTUAL_BASE >> 22)

section .multiboot
align 4
dd MULTIBOOT_MAGIC
dd MULTIBOOT_FLAGS
dd MULTIBOOT_CHECKSUM

section .data
align 0x1000
bootPageDirectory:
    ; identity map first page (4MB)
    dd 0x00000083

    ; zero pages before kernel
    times (KERNEL_PAGE_NUMBER - 1) dd 0

    ; identity map kernel page (4MB)
    dd 0x00000083

    ; pages after kernel
    times (1024 - KERNEL_PAGE_NUMBER - 1) dd 0

section .text
align 4

_loader:
    ; load our boot page directory
    mov ecx, (bootPageDirectory - KERNEL_VIRTUAL_BASE)
    mov cr3, ecx

    ; enable 4 mb pages
    mov ecx, cr4
    or ecx, 0x00000010
    mov cr4, ecx

    ; enable paging
    mov ecx, cr0
    or ecx, 0x80000001
    mov cr0, ecx

    ; jump to higher half code
    lea ecx, [startInHigherHalf]
    jmp ecx

startInHigherHalf:
    ; unmap first 4MB of physical address space
    mov dword [bootPageDirectory], 0
    invlpg [0]

    mov esp, stack_top
    push eax ; multiboot magic number

    push ebx ; multiboot info structure
    call kmain

    cli

loopb:
    hlt
    jmp loopb

section .bss
align 32
stack_bottom:
    resb 16384 ; 16KB
stack_top:
