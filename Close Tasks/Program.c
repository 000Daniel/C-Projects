#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>
#include <string.h>

int capacity = 2;  // Initial capacity of the array
int size = 0;      // Current number of strings in the array
char **strArray;   // Pointer to array of strings

void killProcessByName(const char *processName) {
            //    This function opens a hidden cmd and runs the taskkill command to kill the task.
    char command[256];
    snprintf(command, sizeof(command), "/C taskkill /f /im %s", processName);
    ShellExecute(NULL, "open", "cmd.exe", command, NULL, SW_HIDE);
}

void addString(const char *newStr) {
            // Increase the capacity if we run out of space in the array.
    if (size >= capacity) { 
        capacity *= 2;
        strArray = (char **)realloc(strArray, capacity * sizeof(char *));
        if (strArray == NULL)
        {
            perror("Failed to reallocate memory for string array");
            exit(1);
        }
    }

            // Allocate memory for the new string.
    strArray[size] = (char *)malloc(strlen(newStr) + 1);  // (+1 for null terminator).
    if (strArray[size] == NULL)
    {
        perror("Failed to allocate memory for string");
        exit(1);
    }

            // Copy the string into the newly allocated space
    strcpy(strArray[size], newStr);
    size++;
}

int main_prog() {
            // 'time_intervals' refers to how many seonds to wait between repetitions.
            // 'repeat' refers to how many time to repeat the process.
    int time_intervals = 1;
    int repeat = 12;
    
            // Allocate memory for 'strArray' that will contain our process' names.
    strArray = (char **)malloc(capacity * sizeof(char *));
    if (strArray == NULL)
    {
        perror("Failed to allocate memory for string array");
        return 1;
    }

            // This looks for the config.txt file in the same directory as the program.
    char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    char *dir = strrchr(path, '\\');
    *dir = '\0';
    strcat(path,"\\config.txt");

            // Try opening the config.txt file.
    FILE* file;
    fopen_s(&file, path, "r");

            // If the program couldn't open the file for some reason, create a basic config.txt file.
    if (!file) 
    {
                //    If the config.txt can't open, create a new config file that closes this program.
        printf("Error opening file...\nCreating a new config file...");
        
        file = fopen("config.txt", "w");
        fprintf(file,"1 / 12\n");
        fprintf(file,"CloseTasks.exe");
        fclose(file);
    }

    char buffer[256];       //  This buffer will contain the Line's string.
    int first_line = 0;
    while (fgets(buffer, sizeof(buffer), file))
    {
                // If its the first line, save the time intervals and the repeats.
        if (first_line < 1)
        {
            if (sscanf(buffer, "%d / %d", &time_intervals, &repeat) == 2) {
                printf("Time intervals between closing operations: %d\n", time_intervals);
                printf("Times to repeat process: %d\n", repeat);
            } else {
                printf("Failed to extract numbers.\n");
            }
            first_line++;
        }
        else
        {
                // For the rest of the lines, add their content into our 'strArray'.
            addString(buffer);
        }
    }
    fclose(file);

            // This while loops is responsible to sending kill requests.
    while (repeat > 0)
    {
        for (int i = 0; i < size; i++)
        {
            printf("strArray[%d] = %s\n", i, strArray[i]);
            killProcessByName(strArray[i]);
        }

        sleep(time_intervals);
        repeat--;
    }

            // Free allocated memory.
    for (int i = 0; i < size; i++)
    {
        free(strArray[i]);
    }
    free(strArray);

    return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
            // This function creates an empty/hidden window using the windows.h library.
    main_prog();

    return 0;
}