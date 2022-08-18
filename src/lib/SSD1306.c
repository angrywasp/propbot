#include "./SSD1306.h"
#include "./refs.h"

static void _ssd1306_allocate_buffer(ssd1306_context_t *cxt)
{
    if (cxt->buffer != 0)
        return;

    cxt->buffer = (byte *)malloc((sizeof(byte) * cxt->buffer_size));
}

static bool _ssd1306_send_cmd(ssd1306_context_t *cxt, byte cmd)
{
    i2c_start(cxt->i2c_cxt);

    byte data[3] = {
        SSD1306_WRITE_ADDRESS,
        SSD1306_CONTROL,
        cmd};

    int bytes_sent = i2c_writeData(cxt->i2c_cxt, data, 3);
    i2c_stop(cxt->i2c_cxt);
    return bytes_sent == 3;
}

static void _ssd1306_send_init_commands(SSD1306_GEOMETRY geom, ssd1306_context_t *cxt)
{
    _ssd1306_send_cmd(cxt, SSD1306_DISPLAYOFF);
    _ssd1306_send_cmd(cxt, SSD1306_SETDISPLAYCLOCKDIV);
    _ssd1306_send_cmd(cxt, 0x80); // Increase speed of the display max ~96Hz
    _ssd1306_send_cmd(cxt, SSD1306_SETMULTIPLEX);
    _ssd1306_send_cmd(cxt, cxt->height - 1);
    _ssd1306_send_cmd(cxt, SSD1306_SETDISPLAYOFFSET);
    _ssd1306_send_cmd(cxt, 0x00);
    _ssd1306_send_cmd(cxt, SSD1306_SETSTARTLINE);
    _ssd1306_send_cmd(cxt, SSD1306_CHARGEPUMP);
    _ssd1306_send_cmd(cxt, 0x14);
    _ssd1306_send_cmd(cxt, SSD1306_MEMORYMODE);
    _ssd1306_send_cmd(cxt, 0x00);
    _ssd1306_send_cmd(cxt, SSD1306_SEGREMAP);
    _ssd1306_send_cmd(cxt, SSD1306_COMSCANINC);
    _ssd1306_send_cmd(cxt, SSD1306_SETCOMPINS);

    if (geom == SSD1306_GEOMETRY_128_64)
        _ssd1306_send_cmd(cxt, 0x12);
    else if (geom == SSD1306_GEOMETRY_128_32)
        _ssd1306_send_cmd(cxt, 0x02);

    _ssd1306_send_cmd(cxt, SSD1306_SETCONTRAST);

    if (geom == SSD1306_GEOMETRY_128_64)
        _ssd1306_send_cmd(cxt, 0xCF);
    else if (geom == SSD1306_GEOMETRY_128_32)
        _ssd1306_send_cmd(cxt, 0x8F);

    _ssd1306_send_cmd(cxt, SSD1306_SETPRECHARGE);
    _ssd1306_send_cmd(cxt, 0xF1);
    _ssd1306_send_cmd(cxt, SSD1306_SETVCOMDETECT); // 0xDB, (additionally needed to lower the contrast)
    _ssd1306_send_cmd(cxt, 0x40);                  // 0x40 default, to lower the contrast, put 0
    _ssd1306_send_cmd(cxt, SSD1306_DISPLAYALLON_RESUME);
    _ssd1306_send_cmd(cxt, SSD1306_NORMALDISPLAY);
    _ssd1306_send_cmd(cxt, 0x2e); // stop scroll
    _ssd1306_send_cmd(cxt, SSD1306_DISPLAYON);
}

static inline void _ssd1306_draw_internal(ssd1306_context_t *cxt, ushort x, ushort y, ushort width, ushort height, const byte *data, ushort offset, ushort bytesInData)
{
    if (width < 0 || height < 0)
        return;
    if (y + height < 0 || y > cxt->height)
        return;
    if (x + width < 0 || x > cxt->width)
        return;

    ushort rasterHeight = 1 + ((height - 1) >> 3); // fast ceil(height / 8.0)
    byte yOffset = y & 7;

    bytesInData = bytesInData == 0 ? width * rasterHeight : bytesInData;

    ushort initYMove = y;
    byte initYOffset = yOffset;

    for (ushort i = 0; i < bytesInData; i++)
    {

        // Reset if next horizontal drawing phase is started.
        if (i % rasterHeight == 0)
        {
            y = initYMove;
            yOffset = initYOffset;
        }

        byte currentByte = data[offset + i];

        ushort xPos = x + (i / rasterHeight);
        ushort yPos = ((y >> 3) + (i % rasterHeight)) * cxt->width;

        //    int16_t yScreenPos = yMove + yOffset;
        ushort dataPos = xPos + yPos;

        if (dataPos >= 0 && dataPos < cxt->buffer_size &&
            xPos >= 0 && xPos < cxt->width)
        {

            if (yOffset >= 0)
            {
                switch (cxt->color)
                {
                case SSD1306_COLOR_WHITE:
                    cxt->buffer[dataPos] |= currentByte << yOffset;
                    break;
                case SSD1306_COLOR_BLACK:
                    cxt->buffer[dataPos] &= ~(currentByte << yOffset);
                    break;
                case SSD1306_COLOR_INVERSE:
                    cxt->buffer[dataPos] ^= currentByte << yOffset;
                    break;
                }

                if (dataPos < (cxt->buffer_size - cxt->width))
                {
                    switch (cxt->color)
                    {
                    case SSD1306_COLOR_WHITE:
                        cxt->buffer[dataPos + cxt->width] |= currentByte >> (8 - yOffset);
                        break;
                    case SSD1306_COLOR_BLACK:
                        cxt->buffer[dataPos + cxt->width] &= ~(currentByte >> (8 - yOffset));
                        break;
                    case SSD1306_COLOR_INVERSE:
                        cxt->buffer[dataPos + cxt->width] ^= currentByte >> (8 - yOffset);
                        break;
                    }
                }
            }
            else
            {
                // Make new offset position
                yOffset = -yOffset;

                switch (cxt->color)
                {
                case SSD1306_COLOR_WHITE:
                    cxt->buffer[dataPos] |= currentByte >> yOffset;
                    break;
                case SSD1306_COLOR_BLACK:
                    cxt->buffer[dataPos] &= ~(currentByte >> yOffset);
                    break;
                case SSD1306_COLOR_INVERSE:
                    cxt->buffer[dataPos] ^= currentByte >> yOffset;
                    break;
                }

                // Prepare for next iteration by moving one block up
                y -= 8;

                // and setting the new yOffset
                yOffset = 8 - yOffset;
            }
        }
    }
}

static ushort _ssd1306_draw_string_internal(ssd1306_context_t *cxt, int x, int y, char *text, ushort text_length, ushort text_width)
{
    ushort textHeight = font_data[HEIGHT_POS];
    ushort firstChar = font_data[FIRST_CHAR_POS];
    ushort sizeOfJumpTable = font_data[CHAR_NUM_POS] * JUMPTABLE_BYTES;

    ushort cursorX = 0;
    ushort cursorY = 0;
    ushort charCount = 0;

    // Don't draw anything if it is not on the screen.
    if (x + text_width < 0 || x >= cxt->width)
        return 0;

    if (y + textHeight < 0 || y >= cxt->height)
        return 0;

    for (ushort j = 0; j < text_length; j++)
    {
        ushort xPos = x + cursorX;
        ushort yPos = y + cursorY;
        if (xPos > cxt->width)
            break; // no need to continue
        charCount++;

        byte code = text[j];
        if (code >= firstChar)
        {
            byte charCode = code - firstChar;

            // 4 Bytes per char code
            ushort msbJumpToChar = font_data[JUMPTABLE_START + charCode * JUMPTABLE_BYTES];                      // MSB  \ JumpAddress
            ushort lsbJumpToChar = font_data[JUMPTABLE_START + charCode * JUMPTABLE_BYTES + JUMPTABLE_LSB];      // LSB /
            ushort charByteSize = font_data[JUMPTABLE_START + charCode * JUMPTABLE_BYTES + JUMPTABLE_SIZE];      // Size
            ushort currentCharWidth = font_data[JUMPTABLE_START + charCode * JUMPTABLE_BYTES + JUMPTABLE_WIDTH]; // Width
            // Test if the char is drawable
            if (!(msbJumpToChar == 255 && lsbJumpToChar == 255))
            {
                // Get the position of the char data
                ushort charDataPosition = JUMPTABLE_START + sizeOfJumpTable + ((msbJumpToChar << 8) + lsbJumpToChar);
                _ssd1306_draw_internal(cxt, xPos, yPos, currentCharWidth, textHeight, font_data, charDataPosition, charByteSize);
            }

            cursorX += currentCharWidth;
        }
    }
    return charCount;
}

ssd1306_context_t *ssd1306_init(SSD1306_GEOMETRY geom, int scl, int sda)
{
    ssd1306_context_t *cxt = (ssd1306_context_t *)malloc(sizeof(ssd1306_context_t));
    cxt->geometry = geom;
    cxt->color = SSD1306_COLOR_WHITE;

    switch (geom)
    {
    case SSD1306_GEOMETRY_128_64:
        cxt->width = 128;
        cxt->height = 64;
        break;
    case SSD1306_GEOMETRY_128_32:
        cxt->width = 128;
        cxt->height = 32;
        break;
    }

    cxt->buffer_size = cxt->width * cxt->height / 8;

    cxt->i2c_cxt = (i2c *)malloc(sizeof(i2c));
    i2c_open(cxt->i2c_cxt, scl, sda, 1);

    _ssd1306_allocate_buffer(cxt);

    _ssd1306_send_init_commands(geom, cxt);
    ssd1306_reset_display(cxt);

    return cxt;
}

void ssd1306_clear(ssd1306_context_t *cxt)
{
    memset(cxt->buffer, 0, cxt->buffer_size);
}

void ssd1306_reset_display(ssd1306_context_t *cxt)
{
    ssd1306_clear(cxt);
    ssd1306_display(cxt);
}

void ssd1306_display_on(ssd1306_context_t *cxt)
{
    _ssd1306_send_cmd(cxt, SSD1306_DISPLAYON);
}

void ssd1306_display_off(ssd1306_context_t *cxt)
{
    _ssd1306_send_cmd(cxt, SSD1306_DISPLAYOFF);
}

void ssd1306_set_pixel(ssd1306_context_t *cxt, ushort x, ushort y)
{
    ssd1306_set_pixel_color(cxt, x, y, cxt->color);
}

void ssd1306_set_pixel_color(ssd1306_context_t *cxt, ushort x, ushort y, SSD1306_COLOR color)
{
    if (x >= 0 && x < cxt->width && y >= 0 && y < cxt->height)
    {
        switch (color)
        {
        case SSD1306_COLOR_WHITE:
            cxt->buffer[x + (y / 8) * cxt->width] |= (1 << (y & 7));
            break;
        case SSD1306_COLOR_BLACK:
            cxt->buffer[x + (y / 8) * cxt->width] &= ~(1 << (y & 7));
            break;
        case SSD1306_COLOR_INVERSE:
            cxt->buffer[x + (y / 8) * cxt->width] ^= (1 << (y & 7));
            break;
        }
    }
}

void ssd1306_clear_pixel(ssd1306_context_t *cxt, ushort x, ushort y)
{
    if (x >= 0 && x < cxt->width && y >= 0 && y < cxt->height)
    {
        switch (cxt->color)
        {
        case SSD1306_COLOR_BLACK:
            cxt->buffer[x + (y >> 3) * cxt->width] |= (1 << (y & 7));
            break;
        case SSD1306_COLOR_WHITE:
            cxt->buffer[x + (y >> 3) * cxt->width] &= ~(1 << (y & 7));
            break;
        case SSD1306_COLOR_INVERSE:
            cxt->buffer[x + (y >> 3) * cxt->width] ^= (1 << (y & 7));
            break;
        }
    }
}

void ssd1306_display(ssd1306_context_t *cxt)
{
    _ssd1306_send_cmd(cxt, SSD1306_COLUMNADDR);
    _ssd1306_send_cmd(cxt, 0);                // column start address (0 = reset)
    _ssd1306_send_cmd(cxt, (cxt->width - 1)); // column end address (127 = reset)

    _ssd1306_send_cmd(cxt, SSD1306_PAGEADDR);
    _ssd1306_send_cmd(cxt, 0x00);

    if (cxt->geometry == SSD1306_GEOMETRY_128_32)
        _ssd1306_send_cmd(cxt, 0x03);
    else if (cxt->geometry == SSD1306_GEOMETRY_128_64)
        _ssd1306_send_cmd(cxt, 0x07);

    i2c_start(cxt->i2c_cxt);
    i2c_writeByte(cxt->i2c_cxt, SSD1306_WRITE_ADDRESS);
    i2c_writeByte(cxt->i2c_cxt, 0x40);
    i2c_writeData(cxt->i2c_cxt, cxt->buffer, cxt->buffer_size);
    i2c_stop(cxt->i2c_cxt);
}

bool ssd1306_detect(ssd1306_context_t *cxt)
{
    int ack = 0;
    i2c_start(cxt->i2c_cxt);
    ack = i2c_writeByte(cxt->i2c_cxt, SSD1306_WRITE_ADDRESS);
    i2c_stop(cxt->i2c_cxt);
    return ack == 0;
}

ushort ssd1306_string_width(char *text, ushort text_length)
{
    byte first_char = font_data[FIRST_CHAR_POS];
    ushort string_width = 0;
    ushort max_width = 0;
    for (int i = 0; i < text_length; i++)
    {
        char c = text[i];
        string_width += font_data[JUMPTABLE_START + (c - first_char) * JUMPTABLE_BYTES + JUMPTABLE_WIDTH];
        if (c == 10)
        {
            max_width = max(max_width, string_width);
            string_width = 0;
        }
    }

    return max(max_width, string_width);
}

void ssd1306_txt(ssd1306_context_t *cxt, int x, int y, char *text)
{
    byte lineHeight = font_data[HEIGHT_POS];

    ushort charDrawn = 0;
    ushort line = 0;
    char *textPart = strtok(text, "\n");
    while (textPart != NULL)
    {
        ushort length = strlen(textPart);
        charDrawn += _ssd1306_draw_string_internal(cxt, x, y + (line++) * lineHeight, textPart, length, ssd1306_string_width(textPart, length));
        textPart = strtok(NULL, "\n");
    }
}

void ssd1306_draw_horizontal_line(ssd1306_context_t *cxt, ushort x, ushort y, ushort length)
{
    if (y < 0 || y >= cxt->height)
        return;

    if (x < 0)
    {
        length += x;
        x = 0;
    }

    if ((x + length) > cxt->width)
        length = (cxt->width - x);

    if (length <= 0)
        return;

    uint8_t *bufferPtr = cxt->buffer;
    bufferPtr += (y >> 3) * cxt->width;
    bufferPtr += x;

    uint8_t drawBit = 1 << (y & 7);

    switch (cxt->color)
    {
    case SSD1306_COLOR_WHITE:
        while (length--)
        {
            *bufferPtr++ |= drawBit;
        };
        break;
    case SSD1306_COLOR_BLACK:
        drawBit = ~drawBit;
        while (length--)
        {
            *bufferPtr++ &= drawBit;
        };
        break;
    case SSD1306_COLOR_INVERSE:
        while (length--)
        {
            *bufferPtr++ ^= drawBit;
        };
        break;
    }
}

void ssd1306_draw_vertical_line(ssd1306_context_t *cxt, ushort x, ushort y, ushort length)
{
    if (x < 0 || x >= cxt->width)
        return;

    if (y < 0)
    {
        length += y;
        y = 0;
    }

    if ((y + length) > cxt->height)
        length = (cxt->height - y);

    if (length <= 0)
        return;

    uint8_t yOffset = y & 7;
    uint8_t drawBit;
    uint8_t *bufferPtr = cxt->buffer;

    bufferPtr += (y >> 3) * cxt->width;
    bufferPtr += x;

    if (yOffset)
    {
        yOffset = 8 - yOffset;
        drawBit = ~(0xFF >> (yOffset));

        if (length < yOffset)
            drawBit &= (0xFF >> (yOffset - length));

        switch (cxt->color)
        {
        case SSD1306_COLOR_WHITE:
            *bufferPtr |= drawBit;
            break;
        case SSD1306_COLOR_BLACK:
            *bufferPtr &= ~drawBit;
            break;
        case SSD1306_COLOR_INVERSE:
            *bufferPtr ^= drawBit;
            break;
        }

        if (length < yOffset)
            return;

        length -= yOffset;
        bufferPtr += cxt->width;
    }

    if (length >= 8)
    {
        switch (cxt->color)
        {
        case SSD1306_COLOR_WHITE:
        case SSD1306_COLOR_BLACK:
            drawBit = (cxt->color == SSD1306_COLOR_WHITE) ? 0xFF : 0x00;
            do
            {
                *bufferPtr = drawBit;
                bufferPtr += cxt->width;
                length -= 8;
            } while (length >= 8);
            break;
        case SSD1306_COLOR_INVERSE:
            do
            {
                *bufferPtr = ~(*bufferPtr);
                bufferPtr += cxt->width;
                length -= 8;
            } while (length >= 8);
            break;
        }
    }

    if (length > 0)
    {
        drawBit = (1 << (length & 7)) - 1;
        switch (cxt->color)
        {
        case SSD1306_COLOR_WHITE:
            *bufferPtr |= drawBit;
            break;
        case SSD1306_COLOR_BLACK:
            *bufferPtr &= ~drawBit;
            break;
        case SSD1306_COLOR_INVERSE:
            *bufferPtr ^= drawBit;
            break;
        }
    }
}

void ssd1306_draw_rect(ssd1306_context_t *cxt, ushort x, ushort y, ushort width, ushort height)
{
    ssd1306_draw_horizontal_line(cxt, x, y, width);
    ssd1306_draw_vertical_line(cxt, x, y, height);
    ssd1306_draw_vertical_line(cxt, x + width - 1, y, height);
    ssd1306_draw_horizontal_line(cxt, x, y + height - 1, width);
}

void ssd1306_fill_rect(ssd1306_context_t *cxt, ushort x, ushort y, ushort width, ushort height)
{
    for (ushort i = x; i < x + width; x++)
        ssd1306_draw_vertical_line(cxt, x, y, height);
}
