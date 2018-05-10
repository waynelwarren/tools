#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pwd.h>

static char scratch[2000];
static char sep[30];
static char notesfile[200];

void usage(void)
{
    fprintf(stderr, "usage: n text of note\n"
                    "       n -g \"regx [regx]\"\n"
                    "       n -t [lines]\n"
                    "       n -v\n"
                    "       n -c cmd-line\n"
                    "       n -l cmd-line\n"
                    "       n -h\n"
                    "       program | n\n"
                    "       n < file.txt\n"
                    "  where:\n"
                    "       -g    - grep notes for all \"regx\"\n"
                    "       -t    - print tail of notes, default 20 lines\n"
                    "       -v    - edit notes\n"
                    "       -h    - this help message\n"
                    "       -c    - execute cmd-line & note all stdout\n"
                    "       -l    - execute cmd-line, don't note stdout\n"
                    "       stdin - note line created for each stdin line\n"
                    "  macros:\n"
                    "       @pwd current directory\n"
                    );
    exit(1);
}

void n_grep(int argc, char *argv[])
{
    for (int i=0; i<argc; i++) {
        strcpy(scratch, "grep -i ");
        strcat(scratch, argv[i]);
        strcat(scratch, " ");
        strcat(scratch, notesfile);
        system(scratch);
    }
}

void n_tail(int lines)
{
    strcpy(scratch, "tail -n ");
    sprintf(scratch+strlen(scratch), "%d %s", lines, notesfile);
    system(scratch);
}

void log_line(FILE *fp, int argc, char *argv[], bool incl_cwd)
{
    struct tm *ltime;
    const time_t secs = time(NULL);
    ltime = localtime(&secs);
    strftime(scratch, sizeof(scratch), "%F %T", ltime);
    fprintf(fp, "%s%s", scratch, sep);

    for (int i=0; i<argc; i++) {
        char *str;
        if (strncasecmp(argv[i], "@pwd", 4) == 0)
            str = get_current_dir_name();
        else
            str = argv[i];
        fprintf(fp, " %s", str);
    }

    if (incl_cwd)
        fprintf(fp, " [%s]", get_current_dir_name());

    fprintf(fp, "\n");
}

void n_append(int argc, char *argv[])
{
    FILE *fp = fopen(notesfile, "a");
    if (fp == NULL) {
        fprintf(stderr, "Unable to open %s\n", notesfile);
        exit(1);
    }
    if (argc == 0) {
        char line[300];
        char *av[1];
        strcpy(sep, "|OUT>    ");
        av[0] = line;
        while (fgets(line, sizeof(line)-1, stdin)) {
            printf("%s", line);
            line[strlen(line)-1] = '\0';
            log_line(fp, 1, av, false);
        }
        strcpy(sep, "|");
    }
    else {
        log_line(fp, argc, argv, false);
    }
    fclose(fp);
}

void n_vi(void)
{
    strcpy(scratch, "vi ");
    strcat(scratch, notesfile);
    system(scratch);
}

void n_cmd(int argc, char *argv[], bool log_output)
{
    char cmd[2000];
    FILE *fp = fopen(notesfile, "a");
    if (fp == NULL) {
        fprintf(stderr, "Unable to open %s\n", notesfile);
        exit(1);
    }

    cmd[0] = '\0';
    for (int i=0; i<argc; i++) {
        strcat(cmd, argv[i]);
        strcat(cmd, " ");
    }

    char *av[1];
    av[0] = cmd;
    if (log_output)
        strcpy(sep, "|CMD$ ");
    else
        strcpy(sep, "|RUN$ ");
    log_line(fp, 1, av, true);
    strcpy(sep, "|");
    fclose(fp);

    if (log_output)
        strcat(cmd, " | n");
    system(cmd);
}

int main(int argc, char *argv[])
{
    int lines;
    const char *homedir;

    if ((homedir = getenv("HOME")) == NULL)
        homedir = getpwuid(getuid())->pw_dir;

    strcpy(notesfile, homedir);
    strcat(notesfile, "/n");

    strcpy(scratch, "mkdir -p ");
    strcat(scratch, notesfile);
    system(scratch);

    strcat(notesfile, "/notes.txt");

    strcpy(sep, "|");
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
    else if (strcmp(argv[1], "-c") == 0 || strcmp(argv[1], "-l") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Need cmd\n");
            usage();
        }
        bool log_output = false;
        if (argv[1][1] == 'c')
            log_output = true;
        n_cmd(argc-2, argv+2, log_output);
    }
    else
        n_append(argc-1, argv+1);

    exit(0);
}
