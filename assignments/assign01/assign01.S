#include "hardware/regs/addressmap.h"
#include "hardware/regs/io_bank0.h"
#include "hardware/regs/timer.h"
#include "hardware/regs/m0plus.h"

.syntax unified
.cpu cortex-m0plus
.thumb
.global main_asm
.align 4

.equ DFLT_STATE_STRT, 1                     @ Specify the value to start flashing
.equ DFLT_STATE_STOP, 0                     @ Specify the value to stop flashing
.equ DFLT_ALARM_TIME, 1000000               @ Specify the default alarm timeout (1 sec)

.equ GPIO_BTN_DN_MSK, 0x00040000            @ Bit-18 for falling-edge event on GP20
.equ GPIO_BTN_EN_MSK, 0x00400000            @ Bit-22 for falling-edge event on GP21
.equ GPIO_BTN_UP_MSK, 0x04000000            @ Bit-26 for falling-edge event on GP22

.equ GPIO_BTN_DN, 20                        @ Specify pin for the "down" button
.equ GPIO_BTN_EN, 21                        @ Specify pin for the "enter" button
.equ GPIO_BTN_UP, 22                        @ Specify pin for the "up" button
.equ GPIO_LED_PIN, 25                       @ Specify pin for the built-in LED

.equ GPIO_DIR_IN, 0                         @ Specify input direction for a GPIO pin
.equ GPIO_DIR_OUT, 1                        @ Specify output direction for a GPIO pin

.equ LED_VAL_ON, 1                          @ Specify value that turns the LED "on"
.equ LED_VAL_OFF, 0                         @ Specify value that turns the LED "off"

.equ GPIO_ISR_OFFSET, 0x74                  @ GPIO is int #13 (vector table entry 29)
.equ ALRM_ISR_OFFSET, 0x40                  @ ALARM0 is int #0 (vector table entry 16)


@ Entry point to the ASM portion of the program
main_asm:
    ldr r0, =200000000                      @ small wait to allow time to connect to serial output
    delay_loop:
        subs r0, #1
        bne delay_loop

    ldr r0, =hello_msg
    bl printf

    bl init_leds                            @ Initialise LED pins
    bl install_alarm_isr                    @ Install the alarm ISR handler
    bl install_gpio_isr
    bl init_btns
    

@ main loop toggles LED for testing purposes
main_loop:
    
    bl set_alarm                            @ Set a new alarm
    wfi                                     @ Wait for interrupt
    b main_loop                             @ Return to main loop

@ subroutine to install the GPIO ISR handler
install_alarm_isr:
    push {lr}
    @ get the address of RAM vector table
    ldr r2, =(PPB_BASE + M0PLUS_VTOR_OFFSET) @ private peripheral bus + vector table offset register
    ldr r1, [r2]
    movs r2, #ALRM_ISR_OFFSET
    add r2, r1
    ldr r0, =alrm_isr

    @ store the address of the alrm_isr handler to the vector table
    str r0, [r2]

    @ disable the ALARM0 IRQ by writing to ICPR
    ldr r2, =(PPB_BASE + M0PLUS_NVIC_ICPR_OFFSET) @ PPB + interrupt clear pending register offset
    movs r0, #1
    str r0, [r2]

    @ enable the ALARM0 IRQ by writing to ISER
    ldr r2, =(PPB_BASE + M0PLUS_NVIC_ISER_OFFSET)
    movs r0, #1
    str r0, [r2]

    pop {pc}

@ subroutine to install the GPIO ISR handler
install_gpio_isr:
    push {lr}
    @ Get the address of the RAM vector table
    ldr r2, (PPB_BASE + M0PLUS_VTOR_OFFSET)
    ldr r1, [r2]
    ldr r2, =GPIO_ISR_OFFSET
    add r2, r1
    ldr r0, =gpio_isr

    @ store the address of the gpio_isr handler to the vector table
    str r0, [r2]
    
    @ disable the GPIO IRQ (int #13)
    ldr r0, =0x1
    lsls r0, #13
    ldr r1, =(PPB_BASE + M0PLUS_NVIC_ICPR_OFFSET)
    str r0, [r1]

    @ enable the GPIO IRQ (int #13)
    ldr r1, =(PPB_BASE + M0PLUS_NVIC_ISER_OFFSET)
    str r0, [r1]

    pop {pc}


@ Subroutine to set up a new alarm
set_alarm:
    push {lr}

    @ Check if LED is in flashing state
    ldr r2, =lstate
    ldr r0, [r2]
    cmp r0, #DFLT_STATE_STRT
    bne skip_alarm @ If LED is not flashing, skip setting alarm

    @ Load current time
    ldr r2, =(TIMER_BASE + TIMER_TIMELR_OFFSET)
    ldr r0, [r2]

    @ Load alarm interval
    ldr r2, =ltimer
    ldr r1, [r2]

    @ Add interval to current time
    add r0, r1

    @ Set alarm
    ldr r2, =(TIMER_BASE + TIMER_ALARM0_OFFSET)
    str r0, [r2]

    @ Enable the alarm interrupt
    ldr r2, =(TIMER_BASE + TIMER_INTE_OFFSET)
    ldr r1, [r2]
    movs r0, #1
    orrs r1, r0
    str r1, [r2]

skip_alarm:
    pop {pc}


@ Subroutine to toggle the LED GPIO pin value based on lab 3
toggle_led:
    push {lr}                               @ Store the link register to the stack
    movs r0, #GPIO_LED_PIN                  @ Set the LED GPIO pin number to r0
    bl asm_gpio_get                         @ Get current value of the LED GPIO pin
    cmp r0, #LED_VAL_OFF                    @ Check if the LED GPIO pin value is "off"
    beq led_set_on                          @ If it is "off" then jump to code to turn it on
led_set_off:
    movs r1, #LED_VAL_OFF                   @ The LED is currently "on" so we want to turn it "off"
    b led_set_state                         @ Jump to portion of code where we set the state
led_set_on:
    movs r1, #LED_VAL_ON                    @ The LED is currently "off" so we want to turn it "on"
led_set_state:
    movs r0, #GPIO_LED_PIN                  @ Set the LED GPIO pin number to r0
    bl asm_gpio_put                         @ Update the value of the LED GPIO pin (based on r1)
    pop {pc}                                @ Pop the link register from the stack to the program counter

@ Funtion to initialise LED pins
init_leds:
    push {lr}
    
    movs r0, #GPIO_LED_PIN
    bl asm_gpio_init
    movs r0, #GPIO_LED_PIN
    movs r1, #GPIO_DIR_OUT
    bl asm_gpio_set_dir

    pop {pc}

init_btns:
    push {lr}

    @ initialise "down" button (GP20)
    movs r0, #GPIO_BTN_DN
    bl asm_gpio_init
    movs r0, #GPIO_BTN_DN
    movs r1, #GPIO_DIR_IN
    bl asm_gpio_set_dir
    movs r0, #GPIO_BTN_DN
    bl asm_gpio_set_irq

    @ initialise "enter" button (GP20)
    movs r0, #GPIO_BTN_EN
    bl asm_gpio_init
    movs r0, #GPIO_BTN_EN
    movs r1, #GPIO_DIR_IN
    bl asm_gpio_set_dir
    movs r0, #GPIO_BTN_EN
    bl asm_gpio_set_irq

    @ initialise the "up" button (GP22)
    movs r0, #GPIO_BTN_UP
    bl asm_gpio_init
    movs r0, #GPIO_BTN_UP
    movs r1, #GPIO_DIR_IN
    bl asm_gpio_set_dir
    movs r0, #GPIO_BTN_UP
    bl asm_gpio_set_irq

    pop {pc}


@ Alarm ISR Handler
.thumb_func
alrm_isr:
    push {lr}

    @ clear the interrupt
    ldr r2, =(TIMER_BASE + TIMER_INTR_OFFSET)
    movs r1, #1
    str r1, [r2]

    @ get current LED state
    movs r0, #GPIO_LED_PIN
    bl asm_gpio_get

    @ toggle LED
    bl toggle_led
    pop {pc}

@ GPIO ISR Handler
.thumb_func
gpio_isr:
    push {lr}

    @ load interrupt status
    ldr r2, =(IO_BANK0_BASE + IO_BANK0_PROC0_INTS2_OFFSET)
    ldr r1, [r2]

    @ check if "down" button (GP20) was pressed
    ldr r3, =GPIO_BTN_DN_MSK
    ands r4, r1, r3
    cmp r4, #0
    beq not_dn

    @ clear the interrupt for "down" button
    ldr r2, =(IO_BANK0_BASE + IO_BANK0_INTR2_OFFSET)
    str r3, [r2]

    @ check if LED is in a flashing state
    ldr r4, =lstate
    ldr r5, [r4]
    cmp r5, #DFLT_STATE_STRT
    beq not_paused

    @ LED is not flashing, reset timer to default
    ldr r4, =ltimer
    ldr r5, =DFLT_ALARM_TIME
    str r5, [r4]

    b gpio_isr_end

not_paused:
    @ LED is flashing, halve the flashing rate (double the interval)
    ldr r1, =ltimer
    ldr r3, [r1]
    lsls r3, #1 @ Multiply by 2
    str r3, [r1]

    b gpio_isr_end

not_dn:
    @ check the interrupt for "enter" button
    ldr r2, =(IO_BANK0_BASE + IO_BANK0_INTR2_OFFSET)
    str r3, [r2]

    @ load the current LED state
    ldr r2, =lstate
    ldr r3, [r2]

    @ toggle the LED flashing state
    cmp r3, #DFLT_STATE_STRT
    bne start_timer

    @ stop flashing
    movs r1, #DFLT_STATE_STOP
    str r1, [r2]

    b gpio_isr_end

start_timer:
    @ start flashing
    movs r1, #DFLT_STATE_STRT
    str r1, [r2]

not_en:
    @ check if "up" button (GP22) was pressed
    ldr r3, =GPIO_BTN_UP_MSK
    ands r4, r1, r3
    cmp r4, #0
    beq gpio_isr_end

    @ clear the interrupt for "up" button
    ldr r2, =(IO_BANK0_BASE + IO_BANK0_INTR2_OFFSET)
    str r3, [r2]

    @ check if LED is in flashing state
    ldr r4, =lstate
    ldr r5, [r4]
    cmp r5, #DFLT_STATE_STRT
    beq not_paused_up

    @ LED is not flashing, reset timer to default
    ldr r4, =ltimer
    ldr r5, =DFLT_ALARM_TIME
    str r5, [r4]

not_paused_up:
    @ LED is flashing, double the flashing rate (halve the interval)
    ldr r1, =ltimer
    ldr r3, [r1]
    asrs r3, #1 @ Divide by 2
    str r3, [r1]

gpio_isr_end:
    pop {pc}
    

.align 4
hello_msg: .asciz "Hello World!\n"

.data
lstate: .word DFLT_STATE_STRT               @ Initial LED state (flashing)
ltimer: .word DFLT_ALARM_TIME               @ Initial timer interval (1 sec)