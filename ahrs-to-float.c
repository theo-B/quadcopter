#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define GET_EULER "minimu9-ahrs -b /dev/i2c-1 --output euler"

int main()
{
    FILE *fp;
    extern FILE *popen();
    int i=0;
    float imu_rol, imu_pit, imu_yaw;
    char imu_input[85], imu_input_cut[26], imu_cut_copy[26], *imu_rol_str, *imu_pit_str, *imu_yaw_str;

    // Run the AHRS
    fp = popen(GET_EULER, "r");

    if (fp == NULL) {
    printf("Failed to run AHRS.\n" );
    exit;
    }

    while (fgets(imu_input, sizeof(imu_input)-1, fp) != NULL)
    {
            // Grab the first 26 characters (first three values) from line
            strncpy(imu_input_cut,imu_input,26);
            strncpy(imu_cut_copy,imu_input,26);

            // Separate the first three values
            imu_yaw_str = strtok(imu_cut_copy," ");
            imu_pit_str = strtok(NULL," ");
            imu_rol_str = strtok(NULL," ");

            // Convert to float
            imu_yaw = atof(imu_yaw_str);
            imu_rol = atof(imu_rol_str);
            imu_pit = atof(imu_pit_str);

            //printf("Full: %s\n",imu_input);
            //printf("Cut: %s\n",imu_input_cut);
            //printf("Yaw: %s    | Pit: %s    | Rol: %s    \n-----------------\n",imu_yaw_str, imu_pit_str, imu_rol_str);
            printf("Y: %f | P: %f | R: %f\n",imu_yaw, imu_pit, imu_rol);

            sleep(0.05);

            // Loop limiter
            /*
            i++;
            if (i>1000)
            {
                break;
            }
            */
    }
    pclose(fp);
    return 0;
}
