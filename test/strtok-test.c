#include <stdio.h>
#include <string.h>


int grab_value(char *input)
{
    char *value_str, input_copy[32];
    char *delim = "x";
    int value;

    strcpy(input_copy, input);

    value_str = strtok(input_copy, delim);
    value_str = strtok(NULL, delim);

    value = atoi(value_str);

    return value;
}

int grab_axis(char *input)
{
    char *axis_id_str, input_copy[32];
    char *delim = "x";
    int axis_id;

    strcpy(input_copy, input);

    axis_id_str = strtok(input_copy, delim);

    axis_id = atoi(axis_id_str);

    return axis_id;
}

int main()
{
    char from_rx[] = "1x-32767";
    int js_value, js_axis;

    js_value = grab_value(from_rx);
    js_axis = grab_axis(from_rx);
    printf("Ident %i\n Val %i\n\n",js_axis, js_value);
    return 0;
}
