#include <stdio.h>
#include "js.h"

#define JSRANGE 32767

#define THRSTICK 0 /*****************/
#define YAWSTICK 1 // These values
#define PITSTICK 2 // need changing
#define ROLSTICK 3 /*****************/

int main() {

    Pinval rcthr, rcyaw, rcpit, rcrol;

    // rcthr does not need initializing as we can use the unmodified jse.value

    rcyaw.range = 150;
    rcyaw.neutral = 0;
    rcyaw.constant = mapper(JSRANGE, rcyaw.range, rcyaw.neutral); // Mapped to +-150

    rcpit.range = 45;
    rcpit.neutral = 0;
    rcpit.constant = mapper(JSRANGE, rcpit.range, rcpit.neutral); // Mapped to +-45

    rcrol.range = 45;
    rcrol.neutral = 0;
    rcrol.constant = mapper(JSRANGE, rcrol.range, rcrol.neutral); // Mapped to +-45

    int fd;
    struct js_event jse; // Create joystick event structure

    // Open the joystick
    fd = open_joystick("/dev/input/js0");

    // Event processing loop
    while (1) {
        while (read(fd, &jse, sizeof(jse)) > 0) {

            if ( jse.type == JS_EVENT_AXIS ) {

                 if (jse.number == THRSTICK) {
                     rcpit.value = abs(jse.value); // abs() is important to prevent negative throttle (no reverse)
                     printf("pit%d\n",rcpit.value);
                     fflush(stdout);
                 }

                 if (jse.number == YAWSTICK) {
                     rcyaw.change = jse.value / rcyaw.constant;
                     rcyaw.value = rcyaw.neutral + rcyaw.change;
                     printf("yaw%d\n",rcyaw.value);
                     fflush(stdout);
                 }

                 if (jse.number == PITSTICK) {
                     rcpit.change = jse.value / rcpit.constant;
                     rcpit.value = rcpit.neutral + rcpit.change;
                     printf("pit%d\n",rcpit.value);
                     fflush(stdout);
                 }

                 if (jse.number == ROLSTICK) {
                     rcrol.change = jse.value / rcrol.constant;
                     rcrol.value = rcrol.neutral + rcpit.change;
                     printf("rol%d\n",rcrol.value);
                     fflush(stdout);
                 }
            }
        }
    }
    return 0;
}
