/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 14:43:37 by mbatty            #+#    #+#             */
/*   Updated: 2026/06/16 11:33:32 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"

#include <string.h>
#include <stdlib.h>

char	*strjoin(char const *s1, char const *s2)
{
	char	*dest;
	size_t	len;

	if (!s1 || !s2)
		return (0);
	len = (strlen(s1) + strlen(s2) + 1);
	dest = malloc(len * sizeof(char));
	if (dest == NULL)
		return (NULL);
	strncpy(dest, s1, len);
	strncat(dest, s2, len);
	return ((char *)dest);
}

char *remove_last_n(char *str, size_t n)
{
    size_t len;

    if (!str)
	{
        return NULL;}

	char *true_str = strdup(str);
    len = strlen(true_str);

    if (n >= len)
        true_str[0] = '\0';
    else
        true_str[len - n] = '\0';

    return true_str;
}

static int	ft_intlen(int n)
{
	int	count;

	count = 0;
	if (n == -2147483648)
		return (12);
	if (n < 0)
	{
		n *= -1;
		count++;
	}
	while (n > 9)
	{
		n = n / 10;
		count++;
	}
	count = count + 2;
	return (count);
}

char	*ft_itoa(int n)
{
	char	*str;
	if (n == -2147483648)
	{
		str = strdup("-2147483648");
		return (str);
	}
	int len = ft_intlen(n);
	str = malloc(len * sizeof(char));
	if (!str)
		return (NULL);
	str[len - 1] = 0;
	if (n < 0)
	{
		str[0] = '-';
		n *= -1;
	}
	while (--len > 0 && str[len - 1] != '-')
	{
		str[len - 1] = (n % 10) + '0';
		n = n / 10;
	}
	return (str);
}