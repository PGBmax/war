#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>

typedef struct s_footer
{
    char    tstamp[21];
    char    sep;
    char    counter[21];
}    t_footer;

int    ft_atoi(const char *nptr)
{
    int    i;
    int    nb;

    nb = 0;
    i = 0;
    while (nptr[i] >= '0' && nptr[i] <= '9')
    {
        nb = nb * 10 + (nptr[i] - 48);
        i++;
    }
    return (nb);
}

#define LOWER_HEX        "0123456789"

static void    ft_itoa_64_rec(char *buf, uint64_t n, uint64_t *i)
{
    if (n <= 9)
    {
        buf[*i] = LOWER_HEX[n % 10];
        (*i)++;
        return ;
    }
    ft_itoa_64_rec(buf, n / 10, i);
    ft_itoa_64_rec(buf, n % 10, i);
}

void    ft_itoa_64(char *buf, uint64_t n)
{
    uint64_t    i = 0;

    ft_itoa_64_rec(buf, n, &i);
}

void call_loop(int fd)
{
    t_footer    footer = {.tstamp = {0}, .counter = {0}, .sep = 0};

    struct timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);

    memcpy(footer.counter, "00000000000000000000000", 21);
    memcpy(footer.tstamp, "00000000000000000000000", 21);
    footer.sep = ':';

    char    buf[21] = {0};
    ft_itoa_64(buf, ts.tv_nsec);
    memcpy(footer.tstamp + (20 - strlen(buf)), buf, strlen(buf));
    memset(buf, 0, sizeof(buf));
    ft_itoa_64(buf, ts.tv_sec);
    memcpy(footer.tstamp, buf, strlen(buf));

    memset(buf, 0, sizeof(buf));
    ft_itoa_64(buf, ft_atoi(footer.counter) + 1);
    memcpy(footer.counter + (20 - strlen(buf)), buf, strlen(buf));

    write(fd, &footer, sizeof(t_footer));
    write(fd, "\n", 1);
    memset(buf, 0, sizeof(buf));
}

int    main(void)
{
    int fd = open("test.txt", O_RDWR);
    for (int i = 0; i < 50; ++i)
    {
        call_loop(fd);
    }
    close(fd);
}