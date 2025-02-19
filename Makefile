#******************************************************************************#
#                               VARIABLES                                      #
#******************************************************************************#

# Executable name
NAME        = minishell

# Compiler and flags
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g -fsanitize=address
READLINE    = -I /opt/homebrew/opt/readline/include/readline -lreadline # -L/opt/homebrew/opt/readline/lib
#READLINE    = -I/usr/include/readline -lreadline

# Directories
OBJ_DIR     = obj/
SRC_DIR     = src/
LIBFT_DIR   = libft/
LIBFT       = $(LIBFT_DIR)libft.a

# Source and object files
SRCS        = $(SRC_DIR)main.c \
              $(SRC_DIR)tokenizer.c  \
			  $(SRC_DIR)tokenizer_utils.c \
			  $(SRC_DIR)tokenizer_appender.c \
			  $(SRC_DIR)tokenizer_handler.c \
			  $(SRC_DIR)tokenize_word.c \
			  $(SRC_DIR)parse_command.c \
			  $(SRC_DIR)parsing_utils.c \
			  $(SRC_DIR)ast_built.c \
			  $(SRC_DIR)ast_node_create.c \
			  $(SRC_DIR)free_token_ast.c \
			  $(SRC_DIR)env_variable.c \
			  $(SRC_DIR)debug.c \
			  $(SRC_DIR)init_shell.c \
			  $(SRC_DIR)parse_expression.c \
			  $(SRC_DIR)parse_redirections.c \
			  $(SRC_DIR)parse_error.c \
			  $(SRC_DIR)syntax_error_check.c \
			  $(SRC_DIR)handle_quote.c \



OBJS        = $(SRCS:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

#******************************************************************************#
#                               MAIN TARGETS                                   #
#******************************************************************************#

# Default target: build everything
all: $(LIBFT) $(NAME)

bonus: $(LIBFT)_bonus $(NAME)

# Linking the executable
$(NAME): $(OBJ_DIR) $(OBJS)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(READLINE) -o $(NAME)

#******************************************************************************#
#                               LIBFT TARGET                                   #
#******************************************************************************#

# Compile libft
$(LIBFT):
	@echo "Building libft..."
	@$(MAKE) -C $(LIBFT_DIR)

$(LIBFT)_bonus:
	@echo "Building libft bonus..."
	@$(MAKE) bonus -C $(LIBFT_DIR)

#******************************************************************************#
#                               COMPILE RULES                                  #
#******************************************************************************#

# Ensure object directory exists
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Compile source files to object files
$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_DIR)
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

#******************************************************************************#
#                               SPECIAL TARGETS                                #
#******************************************************************************#

# Indicate phony targets
.PHONY: all clean fclean re bonus
