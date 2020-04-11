
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include "tools/main.h"

// Terminal attributes to restore on exit.
struct termios shivver_console_termios_save;


// Restore original terminal attributes.
void
shivver_console_reset()
{
        tcsetattr
                ( STDIN_FILENO, TCSANOW
                , &shivver_console_termios_save);
}


// Initialize the console interface.
//  We set it to character-at-a-time mode so we can do our own
//  console handling, such as up/down arrows.
void
shivver_console_init()
{
        struct termios tattr;

        // Check that stdin is a terminal.
        if (!isatty(STDIN_FILENO))
        {       fprintf ( stderr
                        , "shivver: not starting console on a non-terminal.");
                exit(EXIT_FAILURE);
        }

        // Save terminal attributes.
        tcgetattr(STDIN_FILENO, &shivver_console_termios_save);
        atexit(shivver_console_reset);

        // Set terminal modes for console.
        tcgetattr (STDIN_FILENO, &tattr);
        tattr.c_lflag     &= ~(ICANON|ECHO);
        tattr.c_cc[VMIN]   = 1;
        tattr.c_cc[VTIME]  = 0;
        tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
}


// Main console loop that handles individual characters.
void
shivver_console_start
        (obj_t* oModule)
{
        // Initialize the console interface.
        shivver_console_init();

        // Create an evaluator state and ingest the module declarations.
        eval_t* state_eval = shivver_eval_alloc();
        shivver_eval_ingest(state_eval, oModule);

        // Allocate a buffer to store the read line.
        size_t  lineMax = 1024;
        char*   line    = malloc(sizeof(char) * lineMax + 1);
        size_t  pos     = 0;

        char c;

  start:
        printf("> ");
        fflush(stdout);

        while(true)
        {
                // Read the next character.
                read(STDIN_FILENO, &c, 1);

                // Quit on control-D.
                if (c == '\004')
                {       printf("\n");
                        break;
                }

                // Newline.
                if (c == '\n')
                {       line[pos] = 0;
                        printf("\n");
                        fflush(stdout);
                        shivver_console_line (state_eval, line);
                        pos = 0;
                        goto start;
                }

                // Delete key.
                if (c == 0x7f)
                {       if (pos == 0) continue;
                        pos --;
                        printf("\b \b");
                        fflush(stdout);
                        continue;
                }

                // Echo printable chars to the line buffer,
                // and echo them back to the console.
                if (c >= 32 && c <= 126)
                {
                        if(pos >= lineMax) continue;
                        line[pos++] = c;

                        printf("%c", c);
                        fflush(stdout);
                        continue;
                }

                // Beep on other non-printable characters on the floor.
                printf("\007");
                fflush(stdout);
        }
}


// Check if this line is just all whitespace.
bool
shivver_console_isLineWhite(char* line)
{
        while(*line != 0)
        {       if (*line != ' ' && *line != '\t')
                        return false;
                line++;
        }
        return true;
}


// Handle an input line to the console.
void
shivver_console_line
        ( eval_t*       state_eval
        , char*         line)
{
        if (shivver_console_isLineWhite(line))
                return;

        char* arg;

        // Treat the line as a term and show its tokens.
        arg = shivver_string_stripPrefix(line, ":tokens");
        if (arg != 0)
        {       shivver_main_tokens(arg);
                return;
        }

        // Parse the line and show its physical description.
        arg = shivver_string_stripPrefix(line, ":printp");
        if (arg != 0)
        {       shivver_console_cmd_printp(arg);
                return;
        }

        arg = shivver_string_stripPrefix(line, ":");
        if(arg != 0)
        {       printf("Unknown command '%s'\n", arg);
                return;
        }

        // Treat line as a term to parse and evaluate.
        else    shivver_console_cmd_eval (state_eval, line);
}

