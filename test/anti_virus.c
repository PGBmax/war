#include <stdio.h>
#include <unistd.h>

#include <dirent.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

bool	is_process_running(const char *name)
{
	DIR *dir = opendir("/proc");
	if (!dir)
		return (false);

	struct dirent   *dirent = NULL;

	do
	{
		dirent = readdir(dir);
		if (dirent)
		{
			char	buf[4096] = {0};

			snprintf(buf, sizeof(buf) - 1, "/proc/%s/cmdline", dirent->d_name);

			int fd = open(buf, O_RDONLY);
			if (fd == -1)
			{
				continue ;
			}

			memset(buf, 0, sizeof(buf));

			ssize_t bytes = read(fd, buf, sizeof(buf) - 1);
			if (bytes == -1)
			{
				close(fd);
				closedir(dir);
				return (false);
			}

			if (strstr(buf, name))
			{
				close(fd);
				closedir(dir);
				return (true);
			}

			close(fd);
		}
	} while (dirent != NULL);

	closedir(dir);

	return (false);
}

int	g_sig = 0;

void	sig_handler(int sig)
{
	g_sig = sig;
}

int	main(void)
{
	signal(SIGINT, sig_handler);

	printf("Super Duper Cool Antivirus! PID: %d\n", getpid());
	while (g_sig == 0)
	{
		if (is_process_running("War"))
			printf("Detected War!\n");
		sleep(1);
	}
	printf("\nexit\n");
}