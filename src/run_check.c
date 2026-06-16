/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 11:24:10 by mbatty            #+#    #+#             */
/*   Updated: 2026/06/12 13:10:16 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <dirent.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

bool	is_process_running(const char *name)
{
	DIR *dir = opendir("/proc");
	if (!dir)
		return (false);

	struct dirent	*dirent = NULL;

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
				continue ;
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
