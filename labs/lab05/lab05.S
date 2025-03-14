#include "hardware/regs/addressmap.h"
#include "hardware/regs/m0plus.h"

.syntax unified                 @ Specify unified assembly syntax
.cpu cortex-m0plus              @ Specify CPU type is Cortex M0+
.thumb                          @ Specify thumb assembly for RP2040
.global main_asm                @ Provide program starting address to the linker
.align 4                        @ Specify code alignment

.equ SLEEP_TIME, 500            @ Specify the sleep time (in ms)
.equ LED_GPIO_PIN, 25           @ Specify the pin that the LED is connected to
.equ LED_GPIO_OUT, 1            @ Specify the direction of the GPIO pin
.equ LED_VALUE_ON, 1            @ Specify the value that turns the LED "on"
.equ LED_VALUE_OFF, 0           @ Specify the value that turns the LED "off"
.equ SVC_ISR_OFFSET, 0x2C       @ The SVC is entry 11 in the vector table
.equ SVC_MAX_INSTRS, 0x01       @ Maximum allowed SVC subroutines

@ Entry point to the ASM portion of the program
main_asm:
    bl init_gpio_led            @ Initialise the GPIO LED pin
    bl install_svc_isr          @ Install the SVC interrupt service routine

loop:
    svc #0                      @ Call the SVC ISR with value 0 (turns on LED)
    nop                         @ Add a no-op instruction for alignment after SVC
    bl do_sleep                 @ Short pause before proceeding
    svc #1                      @ Call the SVC ISR with value 1 (turns off LED)
    nop                         @ Add a no-op instruction for alignment after SVC
    bl do_sleep                 @ Add a short pause before proceeding
    b loop                      @ Always jump back to the start of the loop

@ Subroutine used to introduce a short delay in the application
do_sleep:
    push {lr}
    ldr r0, =SLEEP_TIME         @ Load the sleep time
    bl sleep_ms                 @ Call the sleep_ms function
    pop {pc}

@ Subroutine to initialise the LED
init_gpio_led:
    push {lr}
    movs r0, #LED_GPIO_PIN      @ Load the LED GPIO No.
    bl asm_gpio_init            @ Initialise
    movs r0, #LED_GPIO_PIN      @ Load the LED GPIO Number again
    movs r1, #LED_GPIO_OUT      @ Set direction to output
    bl asm_gpio_set_dir
    pop {pc}

install_svc_isr:
    ldr r2, =(PPB_BASE + M0PLUS_VTOR_OFFSET) @ Get address of vector table register
    ldr r1, [r2]                @ Load the base address of the vector table into r1
    movs r2, #SVC_ISR_OFFSET    @ Load the SVC ISR offset in the vector table
    add r2, r1                  @ Add base address and offset to get address of SVC ISR entry in vector table
    ldr r0, =svc_isr            @ Load the address of the ISR handler
    str r0, [r2]                @ Store the ISR handler address in the vector table
    bx lr                       @ Return to caller

@ SVC interrupt service handler routine
.thumb_func                     @ Required for all interrupt service routines
svc_isr:
    push {lr}                   @ <TODO – add comment>
    ldr r0, [sp, #0x1C]         @ <TODO – add comment>
    subs r0, #0x2               @ <TODO – add comment>
    ldr r0, [r0]                @ <TODO – add comment>
    ldr r1, =#0xFF              @ <TODO – add comment>
    ands r0, r1                 @ <TODO – add comment>
    cmp r0, #SVC_MAX_INSTRS     @ <TODO – add comment>
    bgt svc_done                @ <TODO – add comment>
    adr r1, svc_jmptbl          @ <TODO – add comment>
    lsls r0, #2                 @ <TODO – add comment>
    ldr r1, [r1, r0]            @ <TODO – add comment>
    mov pc, r1                  @ <TODO – add comment>

svc_done:
    pop {pc}                    @ <TODO – add comment>

@ First function of SVC subroutine - turn on the LED
svc_num0:
    <TODO – add assembly code to turn on the LED>
    b svc_done                  @ Branch back to the main ISR when done

 @ Second function of SVC subroutine - turn off the LED
svc_num1:
    <TODO – add assembly code to turn off the LED>
    b svc_done                  @ Branch back to the main ISR when done


@ SVC function entry jump table.
.align 2
svc_jmptbl:
    .word svc_num0              @ Entry zero goes to SVC function #0.
    .word svc_num1              @ Entry one goes to SVC function #1.
    .word 0                     @ Null termination of the jump table.

@ Set data alignment
.data
    .align 4