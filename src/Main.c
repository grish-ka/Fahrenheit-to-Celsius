#include <stdio.h>
#include <stdlib.h> // For sscanf/atof

// REMOVED: #include "cargs_api.h"
// ADDED: Include the main cargs header
#include "cargs.h" 
#include "Util.h" // Include the header for our DLL

// Define the options cargs will look for
// This part is unchanged
static struct cag_option options[] = {
    {.identifier = 'f',
        .access_letters = "f",
        .access_name = "fahrenheit",
        .value_name = "TEMP",
        .description = "Convert Fahrenheit to Celsius"},

    {.identifier = 'c',
        .access_letters = "c",
        .access_name = "celsius",
        .value_name = "TEMP",
        .description = "Convert Celsius to Fahrenheit"},

    {.identifier = 'h',
        .access_letters = "h",
        .access_name = "help",
        .value_name = NULL,
        .description = "Show this help message"}
};

int main(int argc, char **argv) {
    printf("--- Temp Converter Started ---\n");

    cag_option_context context;
    cag_option_init(&context, options, CAG_ARRAY_SIZE(options), argc, argv);

    // Loop over all arguments
    while (cag_option_fetch(&context)) {
        char identifier = cag_option_get_identifier(&context);
        const char *value_str = cag_option_get_value(&context);

        switch (identifier) {
        case 'f': {
            double temp_f;
            sscanf(value_str, "%lf", &temp_f); // Parse the double from the string
            double temp_c = ftc(temp_f);
            printf("%.1f Fahrenheit is %.1f Celsius\n", temp_f, temp_c);
            break;
        }

        case 'c': {
            double temp_c;
            sscanf(value_str, "%lf", &temp_c); // Parse the double from the string
            double temp_f = ctf(temp_c);
            printf("%.1f Celsius is %.1f Fahrenheit\n", temp_c, temp_f);
            break;
        }

        case 'h':
            printf("Usage: Main.exe [options]\n");
            cag_option_print_help(&context, stdout);
            break;

        case '?':
            printf("Error: Unknown option '-%c'\n",
                   cag_option_get_identifier(&context));
            cag_option_print_help(&context, stdout);
            break;
        case ':':
            printf("Error: Option '-%c' requires a value.\n",
                   cag_option_get_identifier(&context));
            break;
        }
    }

    printf("--- Temp Converter Exiting ---\n");
    return 0;
}