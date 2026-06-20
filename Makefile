NAME :=	War

CC :=	cc
CFLAGS := -MP -MMD -Wall -Wextra -Werror # -fsanitize=address -fno-omit-frame-pointer
LFLAGS :=

###

INCLUDE_DIRS :=	inc/				\
				inc/server/			\
				inc/list/			\

C_SRCS :=	src/main.c				\
		src/infect.c				\
		src/check.c					\
		src/crawl.c					\
		src/payload.c				\
		src/utils.c					\
		src/daemon.c				\
		src/service.c				\
		src/sha256.c				\
		src/server/server.c			\
		src/server/server_clients.c	\
		src/server/server_update.c	\
		src/server/server_utils.c	\
		src/list/list_node.c		\
		src/list/list.c				\
		src/run_check.c				\
		src/fingerprint.c			\

ASM_SRCS :=

###

INCLUDE_DIRS :=	$(addprefix -I, $(INCLUDE_DIRS))

###

OBJ_DIR :=	obj


OBJS =	$(C_SRCS:%.c=$(OBJ_DIR)/%.o)
OBJS +=	$(ASM_SRCS:%.asm=$(OBJ_DIR)/%.o)
DEPS =	$(C_SRCS:%.c=$(OBJ_DIR)/%.d)

###

TPUT 					= tput -T xterm-256color
_RESET 					:= $(shell $(TPUT) sgr0)
_BOLD 					:= $(shell $(TPUT) bold)
_ITALIC 				:= $(shell $(TPUT) sitm)
_UNDER 					:= $(shell $(TPUT) smul)
_GREEN 					:= $(shell $(TPUT) setaf 2)
_YELLOW 				:= $(shell $(TPUT) setaf 3)
_RED 					:= $(shell $(TPUT) setaf 1)
_GRAY 					:= $(shell $(TPUT) setaf 8)
_PURPLE 				:= $(shell $(TPUT) setaf 5)

compile:
	@make -j all --no-print-directory

all: $(NAME)

$(NAME): $(OBJS)
	@echo 'Linking $(_BOLD)$(NAME)$(_RESET)'
	@$(CC) $(CFLAGS) -no-pie $(LFLAGS) $(INCLUDE_DIRS) -o $@ $^

$(OBJ_DIR)/%.o: %.asm
	@echo 'Compiling $(_BOLD)$<$(_RESET)'
	@$(ASM) -f elf64 $< -o $@
	@echo 'Compiled $(_BOLD)$<$(_RESET)'

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo 'Compiling $(_BOLD)$<$(_RESET)'
	@$(CC) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@
	@echo 'Compiled $(_BOLD)$<$(_RESET)'

re: fclean compile

fclean: clean
	@echo 'Removed $(_BOLD)$(NAME)$(_RESET)'
	@rm -rf $(NAME)

clean:
	@echo 'Removed $(_BOLD)$(OBJ_DIR)$(_RESET)'
	@rm -rf $(OBJ_DIR)

.PHONY: all clean fclean re compile

-include $(DEPS)
