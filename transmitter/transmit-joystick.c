#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <fcntl.h>
#include <wiringSerial.h>

#define JS_EVENT_BUTTON 0x01
#define JS_EVENT_AXIS 0x02

int main()
{
    // Open joystick
    int js_fd = open("/dev/input/js0", O_RDONLY | O_NONBLOCK);
    struct js_event jse;

    // Open serial port
    int uart_fd = serialOpen ("/dev/ttyAMA0", 9600);

    char to_tx[8], from_rx[8], plusminus;
    int axis;
    unsigned int value;

    // Loop the event processing
    while (1)
    {
        while (read(js_fd, &jse, sizeof(jse)) > 0)
        {
            if ( jse.type == JS_EVENT_AXIS )
            {
                // Construct transmission string
                if (jse.value < 0)
                {
                    plusminus = 'n';
                }
                else
                {
                    plusminus = 'p';
                }
                axis = jse.number;
                value = abs(jse.value);
                snprintf(to_tx, 8, "%d%c%05d", axis, plusminus, value);
                printf("%s\n",to_tx);

                 if (uart_fd != -1)
                 {
                    // Write bytes and error if fail
                    int transmit = write(uart_fd, &to_tx, 8);
                    printf("Write\n");
                    if ( transmit < 0)
                    {
                        printf("UART TX error\n");
                    }
                    serialFlush(uart_fd);
                }
            }
        }
    }
    close(js_fd);
    serialClose(uart_fd);
    return 0;
}
