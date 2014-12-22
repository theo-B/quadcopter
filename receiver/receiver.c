#include <stdio.h>
#include <string.h>
#include <wiringSerial.h>

int grab_axis(char *input)
{
    char input_copy[8];
    int axis_id;

    strcpy(input_copy, input);

    char axis_id_char = input_copy[0];

    //Convert to int
    axis_id = axis_id_char - '0';

    return axis_id;
}

int grab_value(char *input)
{
    char *value_str, input_copy[8];
    signed int value;

    strcpy(input_copy, input);

    // Strip last 5 charactes of string
    int len = strlen(input_copy);
    value_str = &input_copy[len-5];

    value = atoi(value_str);

    // If identifier (2nd char of string) is n, make value negative
    if( input_copy[1] == 'n' )
    {
        value = -value;
    }

    return value;
}

int main()
{
    char from_rx[8];
    int js_axis, js_value;

    // Open serial port
    int uart_fd = serialOpen ("/dev/ttyAMA0", 9600);

    while (1)
    {
        if (uart_fd != -1)
        {
            int receive = read(uart_fd, from_rx, 8);

            if (receive < 0)
            {
                // Error/No bytes waiting
            }
            else
            {
                // Bytes received
                printf("%i bytes read : %s\n", receive, from_rx);

                // Separate axis identifier from joystick positions
                js_axis = grab_axis(from_rx);
                js_value = grab_value(from_rx);
                if(js_axis < 0 || js_axis > 3)
                {
                    // Invalid axis
                    continue;
                }
                else
                {
                    printf("A: %i  V: %i\n", js_axis, js_value);
                }
                serialFlush(uart_fd);
            }
        }
    }
    serialClose(uart_fd);
    return 0;
}
