#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringSerial.h>

#define GET_EULER "minimu9-ahrs -b /dev/i2c-1 --output euler"
#define THRSTICK 1
#define YAWSTICK 0
#define PITSTICK 2
#define ROLSTICK 3


struct axis
{
    float desired;
    float current;
    float output;
    float mapconstant;
};
typedef struct axis Axis;

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

float mapper(int from_max, int desired_max)
{
    return from_max / desired_max;
}

float sensor_read_yaw(FILE *sensor)
{
    char imu_output[85], imu_output_cut[26], *imu_yaw_str;

    if (fgets(imu_output, sizeof(imu_output)-1, sensor) != NULL)
    {
        // Grab the first 26 characters (first three values) from line
        strncpy(imu_output_cut,imu_output,26);

        // Separate the first value
        imu_yaw_str = strtok(imu_output_cut," ");

        return atof(imu_yaw_str);
    }
    else
    {
        return -1;
    }
}

float sensor_read_pit(FILE *sensor)
{
    char imu_output[85], imu_output_cut[26], *imu_pit_str;

    if (fgets(imu_output, sizeof(imu_output)-1, sensor) != NULL)
    {
        // Grab the first 26 characters (first three values) from line
        strncpy(imu_output_cut,imu_output,26);

        // Separate the pit value
        imu_pit_str = strtok(imu_output_cut," ");
        imu_pit_str = strtok(NULL," ");

        return atof(imu_pit_str);
    }
    else
    {
        return -1;
    }
}

float sensor_read_rol(FILE *sensor)
{
    char imu_output[85], imu_output_cut[26], *imu_rol_str;

    if (fgets(imu_output, sizeof(imu_output)-1, sensor) != NULL)
    {
        // Grab the first 26 characters (first three values) from line
        strncpy(imu_output_cut,imu_output,26);

        // Separate the rol value
        imu_rol_str = strtok(imu_output_cut," ");
        imu_rol_str = strtok(NULL," ");
        imu_rol_str = strtok(NULL," ");

        return atof(imu_rol_str);
    }
    else
    {
        return -1;
    }
}



int main()
{
    char from_rx[8];
    int js_axis, js_value;
    Axis thr, yaw, pit, rol;

    // Set up axis structures
    thr.mapconstant = 1; // No change (use pure js values)
    yaw.mapconstant = mapper(32767, 150); // Mapped to +-150
    pit.mapconstant = mapper(32767, 45);  // Mapped to +-45
    rol.mapconstant = mapper(32767, 45);  // Mapped to +-45


    /*********************/
    /** Open the sensor **/
    /*********************/
    FILE *sensor_fp;
    extern FILE *popen();

    // Run the AHRS
    sensor_fp = popen(GET_EULER, "r");

    if (sensor_fp == NULL)
    {
        printf("Failed to run AHRS.\n" );
        exit;
    }
    int i = 0;
    // Initial sensor settling time
    while (i < 250)
    {
        sensor_read_yaw(sensor_fp);
        sensor_read_pit(sensor_fp);
        sensor_read_rol(sensor_fp);
        i++;
    }
    thr.current = 0;

    /**********************/
    /** Open serial port **/
    /**********************/
    int uart_fd = serialOpen ("/dev/ttyAMA0", 9600);


    /**********************/
    /**     Run loop     **/
    /**********************/
    while (1)
    {
        // Read from serial port
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
                //printf("%i bytes read : %s\n", receive, from_rx);

                // Separate axis identifier from joystick positions
                js_axis = grab_axis(from_rx);
                js_value = grab_value(from_rx);

                if(js_axis == THRSTICK)
                {
                    thr.current = thr.desired;
                    thr.desired = fabs(js_value / thr.mapconstant);
                }
                if(js_axis == YAWSTICK)
                {
                    yaw.desired = js_value / yaw.mapconstant;
                }
                if(js_axis == PITSTICK)
                {
                    pit.desired = js_value / pit.mapconstant;
                }
                if(js_axis == ROLSTICK)
                {
                    rol.desired = js_value / rol.mapconstant;
                }

                serialFlush(uart_fd);
            }
        }

        // Read from sensor
        yaw.current = sensor_read_yaw(sensor_fp);
        pit.current = sensor_read_pit(sensor_fp);
        rol.current = sensor_read_rol(sensor_fp);

        printf("Thrust: Current:%f   Desired:%f\n", thr.current, thr.desired);
        printf("Yaw:    Current:%f   Desired:%f\n", yaw.current, yaw.desired);
        printf("Pitch:  Current:%f   Desired:%f\n", pit.current, pit.desired);
        printf("Roll:   Current:%f   Desired:%f\n", rol.current, rol.desired);
    }
    serialClose(uart_fd);
    return 0;
}
