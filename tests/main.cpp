#include <iostream>

#include "window_manager.hpp"
#include "VT100.hpp"

int key_press();

std::vector<Window> wins;

void voltage_ref_changed() { }

Bar Voltage("Voltage", "V", true, voltage_ref_changed, 10, 0, 35);
Bar Current("Current", "V", true, voltage_ref_changed, 10, 0, 35);
Slider cv("Current", "V", true, voltage_ref_changed, 10, 0, 35);
Label label("Current", "V", true, voltage_ref_changed, 10, 20, 55);

void usart_put_char(char ch) { std::cout << ch; }

int main() {
    system(" "); // enable VT100 in windows

    wins.emplace_back("ADC1", 1, 7, 40, 19, true);
    wins.emplace_back("ADC2", 43, 7, 40, 19, true);


//    wins[0].addBox(&text);
    wins[0].addBox(&Voltage);
    wins[0].addBox(&Current);
    wins[0].addBox(&cv);
    wins[0].addBox(&label);
//

    VT::init(usart_put_char);

    window_manager::init(&wins);
    window_manager::refreshAll();


    while (true) {
        const int key = key_press(); // blocks until a key is pressed

        // Arrows
        if (key == -37 || key == -38 || key == -39 || key == -40) {
            uint8_t tab[] = {'B', 'C', 'A', 'D'};
            window_manager::in_RX_callback((uint8_t)VT::esc);
            window_manager::in_RX_callback('[');
            window_manager::in_RX_callback(tab[key + 40]);

            for(int i = 0 ; i < 5; i++) {
                window_manager::run();
            }
        } else {
            window_manager::in_RX_callback(static_cast<uint8_t >(key));
            window_manager::run();
        }



//        println("Input is: "+std::to_string(key)+", \""+(char)key+"\"");
//        std::cout << "\x1b[31mThis text has a red foreground using SGR.31.\r\n" << std::endl;

    }

    return 0;
}
