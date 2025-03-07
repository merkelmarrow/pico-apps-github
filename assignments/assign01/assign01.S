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
    ldr r0, =hello_msg
    bl printf
    bl init_leds                            @ Initialise LED pins

@ main loop toggles LED for testing purposes
main_loop:
    movs r0, #GPIO_LED_PIN
    bl asm_gpio_get
    cmp r0, #LED_VAL_OFF
    beq turn_off

turn_on:
    movs r0, #GPIO_LED_PIN
    movs r1, #LED_VAL_ON
    bl asm_gpio_put
    b delay

turn_off:
    movs r0, #GPIO_LED_PIN
    movs r1, #LED_VAL_OFF
    bl asm_gpio_put

delay:
    @ delay function for testing only
    ldr r0, =500000
    delay_loop:
        subs r0, #1
        bne delay_loop

    b main_loop                             @ Return to main loop once cycles have been iterated through


@ Funtion to initialise LED pins
init_leds:
    push {lr}
    movs r0, #GPIO_LED_PIN
    bl asm_gpio_init
    movs r0, #GPIO_LED_PIN
    movs r1, #GPIO_DIR_OUT
    bl asm_gpio_set_dir
    pop {pc}

.align 4
hello_msg: .asciz "Hello World!\n"

.data
lstate: .word DFLT_STATE_STRT               @ Initial LED state (flashing)
ltimer: .word DFLT_ALARM_TIME               @ Initial timer interval (1 sec)