#include <stdint.h>

#define RCC_BASE        0x40021000UL
#define RCC_APB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x18UL))

#define GPIOA_BASE      0x40010800UL
#define GPIOA_CRL       (*(volatile uint32_t *)(GPIOA_BASE + 0x00UL))
#define GPIOA_IDR       (*(volatile uint32_t *)(GPIOA_BASE + 0x08UL))
#define GPIOA_ODR       (*(volatile uint32_t *)(GPIOA_BASE + 0x0CUL))

#define BUTTON_PIN      0
#define LED_PIN         1

static void GPIOA_Init(void)
{
    /* Bat clock GPIOA */
    RCC_APB2ENR |= (1UL << 2);

    /*
     * PA0: Input pull-up
     * MODE = 00
     * CNF  = 10
     * => 0x8
     *
     * PA1: Output push-pull 50 MHz
     * MODE = 11
     * CNF  = 00
     * => 0x3
     */
    GPIOA_CRL &= ~0xFFUL;
    GPIOA_CRL |= 0x38UL;

    /* PA0 pull-up */
    GPIOA_ODR |= (1UL << BUTTON_PIN);

    /* PA1 initially LOW */
    GPIOA_ODR &= ~(1UL << LED_PIN);
}

int main(void)
{
    GPIOA_Init();

    uint32_t button_was_pressed = 0;

    while (1)
    {
        uint32_t button_pressed =
            ((GPIOA_IDR & (1UL << BUTTON_PIN)) == 0);

        if (button_pressed)
        {
            button_was_pressed = 1;
        }
        else
        {
            if (button_was_pressed)
            {
                GPIOA_ODR ^= (1UL << LED_PIN);
                button_was_pressed = 0;
            }
        }
    }

    return 0;
}
