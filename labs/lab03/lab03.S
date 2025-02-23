.syntax unified                 @ Specify unified assembly syntax
.cpu    cortex-m0plus           @ Specify CPU type is Cortex M0+
.thumb                          @ Specify thumb assembly for RP2040
.global main_asm                @ Provide program starting address to the linker
.align 4                        @ Specify code alignment

.equ    LED_GPIO_PIN, 25        @ Specifiy the physical GPIO pin that the LED is connected to
.equ    LED_GPIO_OUT, 1         @ Specify the direction that we want to set the GPIO pin to
.equ    LED_VALUE_ON, 1         @ Specify the value that turns the LED "on"
.equ    LED_VALUE_OFF, 0        @ Specify the value that turns the LED "off"
.equ    BUTTON_PIN, 21

@   the program continously polls the button state in a loop (no sleep)
@   when the button is pressed (active low), LED state is toggled
@   after toggling, the code waits for the button to be released before resuming polling
@   each button press will therefore only toggle the LED once

@ Entry point to the ASM portion of the program
main_asm:
    movs    r0, #LED_GPIO_PIN           @ This value is the GPIO LED pin on the PI PICO board
    bl      asm_gpio_init               @ Call the subroutine to initialise the GPIO pin specified by r0
    movs    r0, #LED_GPIO_PIN           @ This value is the GPIO LED pin on the PI PICO board
    movs    r1, #LED_GPIO_OUT           @ We want this GPIO pin to be setup as an output pin
    bl      asm_gpio_set_dir            @ Call the subroutine to set the GPIO pin specified by r0 to state specified by r1


    movs    r0, #BUTTON_PIN
    bl      asm_gpio_init               @ initialise pin 21 (button)
    movs    r0, #BUTTON_PIN
    movs    r1, #0
    bl      asm_gpio_set_dir            @ set direction of pin 21 (button) to input


loop:
    movs    r0, #BUTTON_PIN                    
    bl      asm_gpio_get                @ read button state at pin 21
    cmp     r0, #0
    bne     loop                        @ if not pressed (r0 != 0), continue polling

    bl      sub_toggle                  @ If pressed, toggle the LED


wait_release:                           @ subroutine entered after sub_toggle returns
    movs    r0, #BUTTON_PIN             @ subroutine to wait for button release
    bl      asm_gpio_get
    cmp     r0, #0
    beq     wait_release                @ stay here until button released
    b       loop


@ Subroutine to toggle the LED GPIO pin value
sub_toggle:
    push    {lr}                        @ Store the link register to the stack as we will call nested subroutines
    movs    r0, #LED_GPIO_PIN           @ Set the LED GPIO pin number to r0 for use by asm_gpio_get
    bl      asm_gpio_get                @ Get current the value of the LED GPIO pin (returns to r0)
    cmp     r0, #LED_VALUE_OFF          @ Check if the LED GPIO pin value is "off"
    beq     led_set_on                  @ If it is "off" then then jump code to to turn it on
led_set_off:
    movs    r1, #LED_VALUE_OFF          @ The LED is currently "on" so we want to turn it "off"
    b       led_set_state               @ Jump to portion of code where we set the state of the LED
led_set_on:
    movs    r1, #LED_VALUE_ON           @ The LED is currently "off" so we want to turn it "on"
led_set_state:
    movs    r0, #LED_GPIO_PIN           @ Set the LED GPIO pin number to r0 for use by asm_gpio_put
    bl      asm_gpio_put                @ Update the the value of the LED GPIO pin (based on value in r1)
    pop     {pc}                        @ Pop the link register from the stack to the program counter

@ Set data alignment
.data
    .align 4