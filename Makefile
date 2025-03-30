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

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# **************************************************************************** #
#                                  TARGETS                                     #
# **************************************************************************** #

# Default target: build everything
all: $(LIBFT) $(NAME)

bonus: $(LIBFT) $(NAME)

# Linking the executable
$(NAME): $(OBJ_DIR) $(OBJS)
	@echo "Linking $(NAME)..."
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(READLINE) -o $@

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

.PHONY: all bonus clean fclean re
