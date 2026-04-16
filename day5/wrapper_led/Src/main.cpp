#include <cstdint>
//#include "STM32F405xx.h"

// =======================================================
// 🔹 1. Generic Register Wrapper
// =======================================================
template<std::uintptr_t address>
struct Register
{
    static volatile uint32_t& value()
    {
        return *reinterpret_cast<volatile uint32_t*>(address);
    }
};

// =======================================================
// 🔹 2. Peripheral Base Addresses
// =======================================================
constexpr std::uintptr_t RCC_BASE   = 0x40023800;
constexpr std::uintptr_t GPIOD_BASE = 0x40020C00;
constexpr std::uintptr_t GPIOG_BASE = 0x40021800;


// =======================================================
// 🔹 3. RCC Wrapper
// =======================================================
struct RCC
{
    static auto& AHB1ENR()
    {
        return Register<RCC_BASE + 0x30>::value();
    }
};

// =======================================================
// 🔹 4. GPIO Port Wrapper
// =======================================================
template<std::uintptr_t base>
struct GPIO_Port
{
    static auto& MODER()  { return Register<base + 0x00>::value(); }
    static auto& OTYPER() { return Register<base + 0x04>::value(); }
    static auto& OSPEEDR(){ return Register<base + 0x08>::value(); }
    static auto& PUPDR()  { return Register<base + 0x0C>::value(); }
    static auto& IDR()    { return Register<base + 0x10>::value(); }
    static auto& ODR()    { return Register<base + 0x14>::value(); }
    static auto& BSRR()   { return Register<base + 0x18>::value(); }
};

// =======================================================
// 🔹 5. GPIO Driver
// =======================================================
template<typename Port, uint8_t pin>
class GPIO
{
public:
    static_assert(pin < 16, "Invalid GPIO pin");

    static void enable_clock()
    {
        RCC::AHB1ENR() |= (1U << 6); // GPIOG
    }

    static void set_output()
    {
        Port::MODER() &= ~(3U << (pin * 2));
        Port::MODER() |=  (1U << (pin * 2));
    }

    static void set()
    {
        Port::BSRR() = (1U << pin);
    }

    static void reset()
    {
        Port::BSRR() = (1U << (pin + 16));
    }

    static bool read()
    {
        return (Port::IDR() & (1U << pin));
    }

    static void toggle()
    {
        if (read())
            reset();
        else
            set();
    }
};

// =======================================================
// 🔹 6. Define Port
// =======================================================
using GPIOG = GPIO_Port<GPIOG_BASE>;


// =======================================================
// 🔹 7. LED Abstraction
// =======================================================
template<typename GPIO_PIN>
class LED
{
public:
    LED()
    {
        GPIO_PIN::set_output();
    }

    void on()
    {
        GPIO_PIN::set();
    }

    void off()
    {
        GPIO_PIN::reset();
    }

    void toggle()
    {
        GPIO_PIN::toggle();
    }
};

// =======================================================
// 🔹 8. Delay
// =======================================================
void delay()
{
    for (volatile uint32_t i = 0; i < 200000; ++i)
    {
        __asm volatile("nop");
    }
}

// =======================================================
// 🔹 9. Main
// =======================================================
int main()
{
    using LED_PIN1 = GPIO<GPIOG, 8>;
    using LED_PIN2 = GPIO<GPIOG, 10>;

    LED_PIN1::enable_clock();

    LED<LED_PIN1> led1;
    LED<LED_PIN2> led2;

    while (true)
    {
        led1.on();
        delay();
        led1.off();

        led2.on();
        delay();
        led2.off();

        delay();
        delay();

        led1.off();
        led2.off();
    }
}
