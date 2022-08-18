#include "simpletools.h"

#include "./lib/SSD1306.h"
#include "./lib/refs.h"

#define SDA 1
#define SCL 3

void display_text(ssd1306_context_t* oled, ushort y, char* str)
{
    int len = ssd1306_string_width(str, strlen(str));
    ssd1306_txt(oled, (oled->width - len) / 2, y, str); 
}

int main()
{
    pause(1000);

    ssd1306_context_t* oled = ssd1306_init(SSD1306_GEOMETRY_128_32, SCL, SDA);
    printf(ssd1306_detect(oled) ? "OLED found\n" : "OLED not found\n");

    memset(oled->buffer, 0x00, oled->buffer_size);

    display_text(oled, 0, "This is a test");
    display_text(oled, 16, "Also a test");
        
    ssd1306_display(oled);

    while (true)
    {
        pause(1000);
    }
}
