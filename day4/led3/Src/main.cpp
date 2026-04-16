#include <cstdint>


constexpr std::uint32_t RCC_AHB1ENR_ADDR = 0x40023830;
constexpr std::uint32_t GPIOD_MODER_ADDR = 0x40020C00;
constexpr std::uint32_t GPIOD_ODR_ADDR   = 0x40020C14;
constexpr std::uint32_t GPIOA_IDR_ADDR   = 0x40020010;




inline volatile std::uint32_t& RCC_AHB1ENR = *reinterpret_cast<volatile std::uint32_t*>(RCC_AHB1ENR_ADDR);

inline volatile std::uint32_t& GPIOD_MODER = *reinterpret_cast<volatile std::uint32_t*>(GPIOD_MODER_ADDR);

inline volatile std::uint32_t& GPIOD_ODR =*reinterpret_cast<volatile std::uint32_t*>(GPIOD_ODR_ADDR);

inline volatile std::uint32_t& GPIOA_IDR = *reinterpret_cast<volatile std::uint32_t*>(GPIOA_IDR_ADDR);


class GPIO
{
public:
    static void enable_clock() noexcept
    {
        RCC_AHB1ENR |= (1U << 3);  // Enable GPIOD clock
        RCC_AHB1ENR |= (1U << 0); //enable gpioa clock
    }

    static void set_output(std::uint32_t pin) noexcept
    {
        // Clear mode bits (2 bits per pin)
        GPIOD_MODER &= ~(3U << (pin * 2));

        // Set as output mode (01)
        GPIOD_MODER |=  (1U << (pin * 2));
    }

    static void toggle(std::uint32_t pin) noexcept
    {
        GPIOD_ODR ^= (1U << pin);  // Toggle pin
    }
};


void delay()
{
    for (volatile std::uint32_t i = 0; i < 100000; ++i)
    {

    }
}


int main(void)
{
    constexpr std::uint32_t LED_PIN = 12; // PD12 (Green LED)


    GPIO::enable_clock();


    GPIO::set_output(LED_PIN);

    while (true)
    {
       // GPIO::toggle(LED_PIN);


        if(GPIOA_IDR &1) //checking whether switch is on
        	  GPIO::toggle(LED_PIN);


        }



    }
}
