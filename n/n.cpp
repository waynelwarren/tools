#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define FN "/home/wayne/n/notes.txt"

static char scratch[2000];

void usage(void)
{
    fprintf(stderr, "usage: n text of note\n"
                    "       n -g \"regx [regx]\"\n"
                    "       n -t [lines]\n"
                    "       n -v\n"
                    "  where:\n"
                    "       -g grep notes for all \"regx\"\n"
                    "       -t print tail of notes, default 20 lines\n"
                    "       -v edit notes\n"
                    "  macros:\n"
                    "       @pwd current directory\n"
                    );
    exit(1);
}

void n_grep(int argc, char *argv[])
{
    for (int i=0; i<argc; i++) {
        strcpy(scratch, "grep ");
        strcat(scratch, argv[i]);
        strcat(scratch, " " FN);
        system(scratch);
    }
}

void n_tail(int lines)
{
    strcpy(scratch, "tail -n ");
    sprintf(scratch+strlen(scratch), "%d " FN, lines);
    system(scratch);
}

void log_line(FILE *fp, int argc, char *argv[])
{
    struct tm *ltime;
    const time_t secs = time(NULL);
    ltime = localtime(&secs);
    strftime(scratch, sizeof(scratch), "%F %T", ltime);
    fprintf(fp, "%s|", scratch);
    for (int i=0; i<argc; i++) {
        char *str;
        if (strcasecmp(argv[i], "@pwd") == 0)
            str = get_current_dir_name();
        else
            str = argv[i];
        fprintf(fp, " %s", str);
    }
    fprintf(fp, "\n");
}

void n_append(int argc, char *argv[])
{
    FILE *fp = fopen(FN, "a");
    if (fp == NULL) {
        fprintf(stderr, "Unable to open " FN "\n");
        exit(1);
    }
    if (argc == 0) {
        char line[300];
        char *av[1];
        av[0] = line;
        fprintf(stderr, "ready to read stdin\n");
        while (fgets(line, sizeof(line)-1, stdin)) {
            printf("%s", line);
            line[strlen(line)-1] = '\0';
            log_line(fp, 1, av);
        }
    }
    else {
        log_line(fp, argc, argv);
    }
    fclose(fp);
}

void n_vi(void)
{
    system("vi " FN);
}

int main(int argc, char *argv[])
{
    int lines;

    if (argc == 1)
        n_append(0, NULL);
    else if (strncmp(argv[1], "-h", 2) == 0)
        usage();
    else if (strcmp(argv[1], "-g") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Need regx\n");
            usage();
        }
        n_grep(argc-2, argv+2);
    }
    else if (strcmp(argv[1], "-t") == 0) {
        if (argc >= 3)
            lines = atoi(argv[2]);
        else
            lines = 30;
        n_tail(lines);
    }
    else if (strcmp(argv[1], "-v") == 0)
        n_vi();
    else
        n_append(argc-1, argv+1);

    exit(0);
}
