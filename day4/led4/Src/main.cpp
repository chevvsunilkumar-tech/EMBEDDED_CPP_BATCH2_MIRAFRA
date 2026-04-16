#include <cstdint>


constexpr std::uint32_t RCC_AHB1ENR_ADDR = 0x40023830;
constexpr std::uint32_t GPIOD_MODER_ADDR = 0x40020C00;

constexpr std::uint32_t GPIOD_ODR_ADDR   = 0x40020C14;
constexpr std::uint32_t GPIOA_MODER_ADDR = 0x40020000;

constexpr std::uint32_t GPIOA_IDR_ADDR   = 0x40020010;


/*inline volatile std::uint32_t& RCC_AHB1ENR = *reinterpret_cast<volatile std::uint32_t*>(RCC_AHB1ENR_ADDR);

inline volatile std::uint32_t& GPIOD_MODER = *reinterpret_cast<volatile std::uint32_t*>(GPIOD_MODER_ADDR);

inline volatile std::uint32_t& GPIOD_ODR =*reinterpret_cast<volatile std::uint32_t*>(GPIOD_ODR_ADDR);

inline volatile std::uint32_t& GPIOA_IDR = *reinterpret_cast<volatile std::uint32_t*>(GPIOA_IDR_ADDR);

*/


template<std::uint32_t address>
struct reg
{
    static volatile std::uint32_t& value()
    {
        return *reinterpret_cast<volatile std::uint32_t*>(address);
    }
};



class GPIO
{
public:


    static void enable_clock() noexcept
    {
        reg<RCC_AHB1ENR_ADDR>::value() |= (1U << 3); // GPIOD
        reg<RCC_AHB1ENR_ADDR>::value() |= (1U << 0); // GPIOA
    }

    static void set_output(std::uint32_t pin) noexcept
    {
         reg<GPIOD_MODER_ADDR>::value() &= ~(3U << (pin * 2)); // clear
        reg<GPIOD_MODER_ADDR>::value() |=  (1U << (pin * 2)); // output
    }


    static void set_input(std::uint32_t pin) noexcept
    {
        reg<GPIOA_MODER_ADDR>::value() &= ~(3U << (pin * 2));
    }


    static void toggle(std::uint32_t pin) noexcept
    {
        reg<GPIOD_ODR_ADDR>::value() ^= (1U << pin);

    }


    static bool sw_status() noexcept
    {
        return (reg<GPIOA_IDR_ADDR>::value() & (1U << 0));
    }
};



void delay()
{
    for (volatile std::uint32_t i = 0; i < 5000000; ++i)
    {
    }
}



int main(void)
{
    constexpr std::uint32_t LED_PIN = 14;

       GPIO::enable_clock();

    GPIO::set_output(LED_PIN);
    GPIO::set_input(0);

    while (true)
    {
        if (GPIO::sw_status()) // if button pressed
          {
            GPIO::toggle(LED_PIN);
          delay();
        }
    }
}
