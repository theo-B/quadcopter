#include <stdio.h>

struct axis {
    float axis.desired;
    float axis.actual;
    float axis.output;
    float axis.p;
    float axis.Kp;
    float axis.i;
    float axis.Ki;
    float axis.d;
    float axis.Kd;
};
typedef struct axis Axis;

int main()
{
    Axis yaw, pit, rol;

    // Set the proportional (p) gain constants (Kp) (currently a sample number)
    yaw.Kp = 4.5;
    pit.Kp = 0.7;
    rol.Kp = 0.7;

    // Get current (actual) yaw, pitch and roll from gyro
    // Get desired yaw, pitch and roll from transmitter

    yaw.p = yaw.desired - yaw.actual;
    pit.p = pit.desired - pit.actual;
    rol.p = rol.desired - rol.actual;

    yaw.output = yaw.p * yaw.Kp;
    pit.output = pit.p * pit.Kp;
    rol.output = rol.p * rol.Kp;

    return 0;
}
