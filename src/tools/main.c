
#include "tools/main.h"
#include "shivver/codec/ascii.h"

int main(int argc, char** argv)
{
        printf("shivver, version 0.1: http://shivver.org  :? for help\n");
        shivver_heapInit(1024 * 1024);

        obj_t* obj_module = 0;
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

                obj_module
                 = shivver_parse_module_zero
                        (state_parse);

                if (obj_module == 0)
                {       printf("error: %s\n", state_parse->error_str);
                        shivver_parse_free(state_parse);
                        return 0;
                }
                free(buf);
        }

        shivver_console_start(obj_module);
        return 0;
}
