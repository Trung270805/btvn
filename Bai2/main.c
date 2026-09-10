#include <stdint.h>

/* =========================
 * RCC
 * ========================= */
#define RCC_BASE        0x40021000UL
#define RCC_APB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x18UL))

/* =========================
 * GPIOA
 * ========================= */
#define GPIOA_BASE      0x40010800UL
#define GPIOA_CRL       (*(volatile uint32_t *)(GPIOA_BASE + 0x00UL))
#define GPIOA_ODR       (*(volatile uint32_t *)(GPIOA_BASE + 0x0CUL))

/* =========================
 * SysTick
 * ========================= */
#define SYSTICK_BASE    0xE000E010UL
#define SYSTICK_CTRL    (*(volatile uint32_t *)(SYSTICK_BASE + 0x00UL))
#define SYSTICK_LOAD    (*(volatile uint32_t *)(SYSTICK_BASE + 0x04UL))
#define SYSTICK_VAL     (*(volatile uint32_t *)(SYSTICK_BASE + 0x08UL))

/* =========================
 * Tham số
 * ========================= */
#define DELAY_MS        300UL
#define CPU_CLOCK_HZ    8000000UL


/* =========================
 * Delay 1 ms
 * ========================= */
static void delay_1ms(void)
{
    SYSTICK_LOAD = (CPU_CLOCK_HZ / 1000UL) - 1UL;
    SYSTICK_VAL = 0;

    SYSTICK_CTRL = (1UL << 2) | (1UL << 0);

    while ((SYSTICK_CTRL & (1UL << 16)) == 0)
    {
    }

    SYSTICK_CTRL = 0;
}


/* =========================
 * Delay nhiều ms
 * ========================= */
static void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++)
    {
        delay_1ms();
    }
}


/* =========================
 * Cấu hình PA0 - PA7
 * Output Push-Pull
 * ========================= */
static void GPIOA_Init(void)
{
    /* Bật clock cho GPIOA */
    RCC_APB2ENR |= (1UL << 2);

    /*
     * PA0 - PA7:
     * Output Push-Pull
     * Maximum speed 50 MHz
     *
     * Mỗi chân dùng 4 bit:
     * 0011 = 0x3
     *
     * 8 chân -> 0x33333333
     */
    GPIOA_CRL = 0x33333333UL;

    /* Tắt tất cả LED ban đầu */
    GPIOA_ODR = 0x00000000UL;
}


/* =========================
 * Chương trình chính
 * ========================= */
int main(void)
{
    GPIOA_Init();

    while (1)
    {
        /* =====================
         * Chạy từ trái sang phải
         * PA0 -> PA7
         * ===================== */
        for (uint32_t i = 0; i < 8; i++)
        {
            GPIOA_ODR = (1UL << i);

            delay_ms(DELAY_MS);
        }

        /* =====================
         * Chạy từ phải sang trái
         * PA7 -> PA0
         *
         * PA7 đã sáng ở vòng trước
         * nên bắt đầu từ PA6
         * ===================== */
        for (int i = 6; i >= 0; i--)
        {
            GPIOA_ODR = (1UL << i);

            delay_ms(DELAY_MS);
        }
    }

    return 0;
}
