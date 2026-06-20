/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fingerprint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/20 09:37:04 by mbatty            #+#    #+#             */
/*   Updated: 2026/06/20 11:08:45 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>

#define LOWER_HEX		"0123456789abcdef"

static void	ft_itoa_hex_rec(char *buf, uint64_t n, uint64_t *i)
{
	if (n <= 15)
	{
		buf[*i] = LOWER_HEX[n % 16];
		(*i)++;
		return ;
	}
	ft_itoa_hex_rec(buf, n / 16, i);
	ft_itoa_hex_rec(buf, n % 16, i);
}

void	ft_itoa_hex(char *buf, uint64_t n)
{
	uint64_t	i = 0;

	ft_itoa_hex_rec(buf, n, &i);
}

int	get_fingerprint(char *buf)
{
	memset(buf, 0, 34);

	char	*sec = buf;
	memcpy(sec, "0000000000000000", 16);

	char	*nsec = buf + 17;
	memcpy(nsec, "0000000000000000", 16);

	buf[16] = ':';

	struct timespec	ts;
	if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
		return (-1);

	char	buf1[17];

	memset(buf1, 0, sizeof(buf1));
	ft_itoa_hex(buf1, ts.tv_sec);
	memcpy(sec + (16 - strlen(buf1)), buf1, strlen(buf1));

	memset(buf1, 0, sizeof(buf1));
	ft_itoa_hex(buf1, ts.tv_nsec);
	memcpy(nsec + (16 - strlen(buf1)), buf1, strlen(buf1));
	return (0);
}
