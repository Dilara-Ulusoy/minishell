#******************************************************************************#
#                               VARIABLES                                      #
#******************************************************************************#

# Executable name
NAME        = minishell

# Compiler and flags
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g -fsanitize=address
READLINE    = -L/opt/homebrew/opt/readline/lib -I/opt/homebrew/opt/readline/include -lreadline -lhistory

# Directories
OBJ_DIR     = obj/
SRC_DIR     = src/
LIBFT_DIR   = libft/
LIBFT       = $(LIBFT_DIR)libft.a

# Source and object files
SRCS        = $(SRC_DIR)main.c \
              $(SRC_DIR)tokenization.c  \
			  $(SRC_DIR)tokenization_utils.c \
			  $(SRC_DIR)utils.c \

OBJS        = $(SRCS:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

#******************************************************************************#
#                               MAIN TARGETS                                   #
#******************************************************************************#

# Default target: build everything
all: $(LIBFT) $(NAME)

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
.PHONY: all clean fclean re
