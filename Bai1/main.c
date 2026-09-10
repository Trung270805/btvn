
#include <stdint.h>

/* =========================
   RCC
   ========================= */
#define RCC_BASE        0x40021000UL
#define RCC_APB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x18UL))

/* =========================
   GPIOC
   ========================= */
#define GPIOC_BASE      0x40011000UL
#define GPIOC_CRH       (*(volatile uint32_t *)(GPIOC_BASE + 0x04UL))
#define GPIOC_ODR       (*(volatile uint32_t *)(GPIOC_BASE + 0x0CUL))

/* =========================
   SysTick
   ========================= */
#define SYSTICK_BASE    0xE000E010UL
#define SYSTICK_CTRL    (*(volatile uint32_t *)(SYSTICK_BASE + 0x00UL))
#define SYSTICK_LOAD    (*(volatile uint32_t *)(SYSTICK_BASE + 0x04UL))
#define SYSTICK_VAL     (*(volatile uint32_t *)(SYSTICK_BASE + 0x08UL))

/* =========================
   THAM SỐ
   ========================= */

/* Đổi giá trị này để thay đổi tốc độ nhấp nháy */
#define BLINK_DELAY_MS  500UL

/* STM32F103 sau reset sử dụng HSI = 8 MHz */
#define CPU_CLOCK_HZ    8000000UL


/* =========================
   Delay 1 ms
   ========================= */
static void delay_1ms(void)
{
    /* 8 MHz / 1000 Hz = 8000 */
    SYSTICK_LOAD = (CPU_CLOCK_HZ / 1000UL) - 1UL;

    /* Xóa giá trị hiện tại */
    SYSTICK_VAL = 0;

    /* Bật SysTick
       bit 2: Clock source = processor clock
       bit 0: Enable
    */
    SYSTICK_CTRL = (1UL << 2) | (1UL << 0);

    /* Chờ COUNTFLAG (bit 16) */
    while ((SYSTICK_CTRL & (1UL << 16)) == 0)
    {
    }

    /* Tắt SysTick */
    SYSTICK_CTRL = 0;
}


/* =========================
   Delay nhiều ms
   ========================= */
static void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++)
    {
        delay_1ms();
    }
}


/* =========================
   Cấu hình PC13 OUTPUT
   ========================= */
static void GPIOC_PC13_Init(void)
{
    /* Bật clock cho GPIOC
       IOPCEN = bit 4
    */
    RCC_APB2ENR |= (1UL << 4);

    /* PC13 nằm trong CRH
       PC13 bắt đầu từ bit 20

       MODE13 = 01 -> Output 10 MHz
       CNF13  = 00 -> Push-pull
    */

    /* Xóa 4 bit cấu hình PC13 */
    GPIOC_CRH &= ~(0xFUL << 20);

    /* Cấu hình PC13:
       MODE13 = 01
       CNF13  = 00
    */
    GPIOC_CRH |= (0x1UL << 20);

    /* PC13 = 1 -> LED tắt
       (LED onboard Blue Pill active-low)
    */
    GPIOC_ODR |= (1UL << 13);
}


/* =========================
   MAIN
   ========================= */
int main(void)
{
    /* Khởi tạo PC13 */
    GPIOC_PC13_Init();

    while (1)
    {
        /* Đảo trạng thái PC13 */
        GPIOC_ODR ^= (1UL << 13);

        /* Chờ 1 giây */
        delay_ms(BLINK_DELAY_MS);
    }

    return 0;
}
