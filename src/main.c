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

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECONGNIZED_STATEMENT
} PrepareResult;

typedef enum { 
    STATEMENT_INSERT, 
    STATEMENT_SELECT 
} StatementType;

typedef struct {
  StatementType type;
} Statement;

InputBuffer* new_input_buffer(); // init
void print_prompt(); // print prompt to user
void read_input(InputBuffer* input_buffer); // read a line input
void close_input_buffer(InputBuffer* input_buffer); // free the memory
MetaCommandResult do_meta_command(InputBuffer* input_buffer); // handle meta_command
PrepareResult prepare_statement(InputBuffer* input_buffer,
                                 Statement* statement); // SQL Complier
void execute_statement(Statement* statement); // finally become virtual machine 

int main(int argc, char* argv[]) {
    InputBuffer* input_buffer = new_input_buffer();
    /* infinite loop that prints the prompt,
    gets a line of input, then processes that line of input */
    while (1)
    {
        print_prompt();
        read_input(input_buffer);

        /*
        // when input ".exit", exit program
        if (strcmp(input_buffer->buffer, ".exit") == 0) {
            close_input_buffer(input_buffer);
            exit(EXIT_SUCCESS);
        } else {
            printf("Unrecognized command '%s' .\n", input_buffer->buffer);
        }
        */

        // check "meta-commands" : Non-SQL statements like .exit 
        if (input_buffer->buffer[0] == '.') {
            // use special function to handle
            switch (do_meta_command(input_buffer)) {
                case (META_COMMAND_SUCCESS): 
                    continue;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized command '%s' .\n", input_buffer->buffer);
                    continue;
            }
        }
    
        Statement statement;
        switch (prepare_statement(input_buffer, &statement)) {
            case (PREPARE_SUCCESS):
                break;
            case (PREPARE_UNRECONGNIZED_STATEMENT):
                printf("Unrecognized keyword at start of '%s'.\n",
                        input_buffer->buffer);
                continue;
        }

        execute_statement(&statement);
        printf("Executed.\n");
    }
}

InputBuffer* new_input_buffer() {
    InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_length = 0;
    input_buffer->input_length = 0;

    return input_buffer;
}

void print_prompt() { printf("Simpledb > "); }

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

MetaCommandResult do_meta_command(InputBuffer* input_buffer) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

PrepareResult prepare_statement(InputBuffer* input_buffer,
                                 Statement* statement) {
    if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }
    if (strcmp(input_buffer->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECONGNIZED_STATEMENT;
}

void execute_statement(Statement* statement) {
    switch (statement->type) {
        case (STATEMENT_INSERT):
            printf("This is where we can do insert.\n");
            break;
        case (STATEMENT_SELECT):
            printf("This is where we can do select.\n");
            break;
    }
}



