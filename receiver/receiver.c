#include <stdio.h>
#include <string.h>
#include <wiringSerial.h>

int grab_axis(char *input)
{
    char *axis_id_str, input_copy[8];
    char *delim = "x";
    int axis_id;

    strcpy(input_copy, input);

    axis_id_str = strtok(input_copy, delim);

    axis_id = atoi(axis_id_str);

    return axis_id;
}

int grab_value(char *input)
{
    char *value_str, input_copy[8];
    char *delim = "x";
    int value;

    strcpy(input_copy, input);

    value_str = strtok(input_copy, delim);
    value_str = strtok(NULL, delim);

    value = atoi(value_str);

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
                /*js_axis = grab_axis(from_rx);
                js_value = grab_value(from_rx);
                printf("A: %i  V: %i\n",js_axis, js_value);*/
                //serialFlush(uart_fd);
            }
        }
    }

    serialClose(uart_fd);
    return 0;
}
