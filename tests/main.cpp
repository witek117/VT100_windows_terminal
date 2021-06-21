#include <iostream>

#include "window_manager.hpp"
#include "VT100.hpp"

int key_press();

//embenet::static_vector<Window, 10> windows;

//std::vector<Window> wins;
//std::array<Window*, 10> windows;

void voltage_ref_changed() { }

Bar Voltage("Voltage", "V", true, voltage_ref_changed, 10, 0, 35);
Bar Current("Current", "V", true, voltage_ref_changed, 10, 0, 35);
Slider cv("Current", "V", true, voltage_ref_changed, 10, 0, 35);
Label label("Current", "V", true, voltage_ref_changed, 10, 20, 55);

Window adc1("ADC1", 1, 7, 40, 19, true);
Window adc2("ADC2", 43, 7, 40, 19, true);

window_manager<3> windowManager;

void usart_put_char(char ch) { std::cout << ch; }

int main() {
    system(" "); // enable VT100 in windows

    windowManager.add(&adc1);
    windowManager.add(&adc2);

    adc1.addBox(&Voltage);
    adc1.addBox(&Current);
    adc1.addBox(&cv);
    adc1.addBox(&label);
//

    VT::init(usart_put_char);

    windowManager.init();
    windowManager.refreshAll();


    while (true) {
        const int key = key_press(); // blocks until a key is pressed

        // Arrows
        if (key == -37 || key == -38 || key == -39 || key == -40) {
            uint8_t tab[] = {'B', 'C', 'A', 'D'};
            windowManager.in_RX_callback((uint8_t)VT::esc);
            windowManager.in_RX_callback('[');
            windowManager.in_RX_callback(tab[key + 40]);

            for(int i = 0 ; i < 5; i++) {
                windowManager.run();
            }
        } else {
            windowManager.in_RX_callback(static_cast<uint8_t >(key));
            windowManager.run();
        }



//        println("Input is: "+std::to_string(key)+", \""+(char)key+"\"");
//        std::cout << "\x1b[31mThis text has a red foreground using SGR.31.\r\n" << std::endl;

    }

    return 0;
}
