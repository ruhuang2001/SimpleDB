#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

typedef struct 
{
    char* buffer;
    size_t buffer_length;
    size_t input_length;
} InputBuffer;

InputBuffer* new_input_buffer(); // init
void print_prompt(); // print prompt to user
void read_input(InputBuffer* input_buffer); // read a line input
void close_input_buffer(InputBuffer* input_buffer); // free the memory

int main(int argc, char* argv[]) {
    InputBuffer* input_buffer = new_input_buffer();
    /* infinite loop that prints the prompt,
    gets a line of input, then processes that line of input */
    while (1)
    {
        print_prompt();
        read_input(input_buffer);

        // when input ".exit", exit program
        if (strcmp(input_buffer->buffer, ".exit") == 0) {
            close_input_buffer(input_buffer);
            exit(EXIT_SUCCESS);
        } else {
            printf("Unrecognized command '%s' .\n", input_buffer->buffer);
        }
    }
}

InputBuffer* new_input_buffer() {
    InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;

    return input_buffer;
}

void print_prompt() { printf("db > "); }

void read_input(InputBuffer* input_buffer) {
    ssize_t bytes_read = getline(&(input_buffer->buffer),
                                &(input_buffer->buffer_length),
                                stdin);

    if (bytes_read <= 0) {
        printf("Error reading input");
        exit(EXIT_FAILURE);
    }   

    // Ignore trailing newline
    input_buffer->input_length = bytes_read - 1;
    input_buffer->buffer[bytes_read - 1] = 0;
}

void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}


