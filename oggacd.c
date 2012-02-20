#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>
#include <err.h>

#include <vorbis/vorbisfile.h>

enum {
        MAXLEN = 70 * 60,       /* 70 min */
        LINELEN = 1024
};

void            usage(void);
int          getlen(char *path, double *len);

int
main(int argc, char *argv[])
{
        char           *line;
        double          sumlen = 0;
        int             debug = 0;
        char            delim = '\n';
        char            opt;

        while ((opt = getopt(argc, argv, "d0h")) != -1) {
                switch (opt) {
                case 'd':
                        debug = 1;
                        break;
                case '0':
                        delim = '\0';
                        break;
                case 'h':
                        usage();
                        return (EX_OK);
                default:
                        usage();
                        return (EX_USAGE);
                }
        }

        if ((line = malloc(LINELEN)) == NULL)
                err(EX_SOFTWARE, "malloc(%d)", LINELEN);

        while (fgets(line, LINELEN, stdin) != NULL) {
                char           *p;
                double          l;

                if ((p = strchr(line, (int)'\n')) != NULL)
                        *p = '\0';

                if (getlen(line, &l) == -1)
			continue;

                if (debug)
                        fprintf(stderr, "%s: %s: %f total: %f max: %d\n",
                                getprogname(), line, l, sumlen + l, MAXLEN);

                if ((sumlen += l) > MAXLEN)
                        break;

		fputs(line, stdout);
		putchar(delim);
        }
        if (ferror(stdin))
                errx(EX_SOFTWARE, "gets");

        free(line);

        return (EX_OK);
}

int
getlen(char *path, double *len)
{
        OggVorbis_File  vf;

        assert(path != NULL);

        if (ov_fopen(path, &vf) != 0) {
                warn("ov_fopen(\"%s\", ...)", path);
                return (-1);
        }
        if ((*len = ov_time_total(&vf, -1)) == OV_EINVAL) {
                errx(EX_SOFTWARE, "ov_time_total");
        }
        if (ov_clear(&vf) != 0)
                errx(EX_SOFTWARE, "ov_clear");

        return (0);
}

void
usage(void)
{
        fputs("usage:\n", stderr);
        fprintf(stderr, "\t%s -h\n", getprogname());
        fprintf(stderr, "\t%s [-d0]\n", getprogname());
}
