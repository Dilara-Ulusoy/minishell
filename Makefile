# **************************************************************************** #
#                                  CONFIG                                      #
# **************************************************************************** #

NAME     := minishell
CC       := cc
CFLAGS   := -Wall -Wextra -Werror
READLINE := -lreadline -I /opt/homebrew/opt/readline/include/readline
#READLINE    = -I/usr/include/readline -lreadline

SRC_DIR  := src
OBJ_DIR  := obj
LIBFT_DIR:= Libft
LIBFT    := $(LIBFT_DIR)/libft.a

SRCS := \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/tokenizer.c \
	$(SRC_DIR)/tokenizer_utils.c \
	$(SRC_DIR)/tokenizer_handler.c \
	$(SRC_DIR)/tokenizer_appender.c \
	$(SRC_DIR)/tokenize_word.c \
	$(SRC_DIR)/syntax_error_check_1.c \
	$(SRC_DIR)/syntax_error_check_2.c \
	$(SRC_DIR)/special_cases.c \
	$(SRC_DIR)/signal_1.c \
	$(SRC_DIR)/signal_2.c \
	$(SRC_DIR)/set_pipe.c \
	$(SRC_DIR)/set_pipe_single_builtin.c \
	$(SRC_DIR)/run_pid.c \
	$(SRC_DIR)/run_builtins.c \
	$(SRC_DIR)/print_error_messages.c \
	$(SRC_DIR)/parsing_utils.c \
	$(SRC_DIR)/parse_quotes.c \
	$(SRC_DIR)/parse_expression.c \
	$(SRC_DIR)/parse_error.c \
	$(SRC_DIR)/parse_redirections.c \
	$(SRC_DIR)/parse_command.c \
	$(SRC_DIR)/init_shell.c \
	$(SRC_DIR)/heredoc_1.c \
	$(SRC_DIR)/heredoc_2.c \
	$(SRC_DIR)/helper_utils.c \
	$(SRC_DIR)/handle_quote.c \
	$(SRC_DIR)/get_num_commands.c \
	$(SRC_DIR)/get_command_array.c \
	$(SRC_DIR)/ft_unset.c \
	$(SRC_DIR)/ft_set.c \
	$(SRC_DIR)/ft_pwd.c \
	$(SRC_DIR)/ft_putstr_with_quotes_fd.c \
	$(SRC_DIR)/ft_getenv.c \
	$(SRC_DIR)/ft_exit.c \
	$(SRC_DIR)/ft_env.c \
	$(SRC_DIR)/ft_cd.c \
	$(SRC_DIR)/free_utils.c \
	$(SRC_DIR)/free_token.c \
	$(SRC_DIR)/free_ast.c \
	$(SRC_DIR)/execute_commands.c \
	$(SRC_DIR)/exec_utils.c \
	$(SRC_DIR)/exec_utils_2.c \
	$(SRC_DIR)/env_variable.c \
	$(SRC_DIR)/env_var_utils.c \
	$(SRC_DIR)/env_var_utils_2.c \
	$(SRC_DIR)/cmd_parts_utils.c \
	$(SRC_DIR)/check_and_run_builtins.c \
	$(SRC_DIR)/check_and_run_builtins_single.c \
	$(SRC_DIR)/builtin_utils.c \
	$(SRC_DIR)/ast_node_create.c \
	$(SRC_DIR)/arrange_in_out_files.c \
	$(SRC_DIR)/arg_struct_utils.c \
	$(SRC_DIR)/ast_built.c \

OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
# **************************************************************************** #
#                                  TARGETS                                     #
# **************************************************************************** #

# Default target: build everything
all: $(LIBFT) $(NAME)

# Linking the executable
$(NAME): $(OBJ_DIR) $(OBJS)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(READLINE) -o $@

$(LIBFT):
	@echo "Building libft..."
	@$(MAKE) -C $(LIBFT_DIR)

#******************************************************************************#
#                               COMPILE RULES                                  #
#******************************************************************************#

# Ensure object directory exists
$(OBJ_DIR):
	@mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

#******************************************************************************#
#                               CLEANUP TARGETS                                #
#******************************************************************************#

# Clean object files
clean:
	@echo "Cleaning object files..."
	@rm -f $(OBJS)
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

# Full clean, including executables
fclean: clean
	@echo "Full clean..."
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

# Rebuild everything from scratch
re: fclean all

.PHONY: all clean fclean re
