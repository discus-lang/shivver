
#include "tools/main.h"

#include "shivver/codec/ascii.h"

int main(int argc, char** argv)
{
        shivver_heapInit(1024 * 1024);

        if (argc == 2) {
                char* sFileName = argv[1];

                // Read the source file.
                FILE* file = fopen(sFileName, "r");
                assert (file != 0);

                assert(fseek(file, 0, SEEK_END) == 0);
                int nFile = ftell(file);
                assert (nFile != -1);
                rewind(file);

                char* buf = malloc(nFile + 1);
                int nRead = fread(buf, nFile, 1, file);
                assert (nRead == 1);
                buf[nFile] = 0;

                // Load declarations from the file.
                parser_t* state_parse
                 = shivver_parse_alloc(buf);

                obj_t* obj_parse
                 = shivver_parse_decl(state_parse);

                if (obj_parse == 0)
                {       printf("error: %s\n", state_parse->error_str);
                        shivver_parse_free(state_parse);
                        return 0;
                }
                shivver_prim_console_printl (obj_parse);
                printf("\n");
                free(buf);
        }

        shivver_console_start();
        return 0;
}



