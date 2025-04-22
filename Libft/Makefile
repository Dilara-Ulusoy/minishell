# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: htopa <htopa@student.hive.fi>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/19 10:32:48 by dakcakoc          #+#    #+#              #
#    Updated: 2025/04/22 21:02:31 by htopa            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ANSI color codes
GREEN = \033[32m
YELLOW = \033[33m
CYAN = \033[36m
RESET = \033[0m

NAME = libft.a

SRCS = ft_atoi.c ft_itoa.c ft_bzero.c ft_isalnum.c ft_isalpha.c ft_isascii.c \
       ft_isdigit.c ft_isprint.c ft_memset.c ft_memcpy.c ft_memmove.c ft_memchr.c ft_memcmp.c ft_putchar_fd.c \
       ft_putnbr_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_strlen.c ft_strlcat.c ft_strlcpy.c ft_strncmp.c ft_strrchr.c ft_strdup.c ft_strjoin.c ft_split.c ft_strmapi.c\
       ft_tolower.c ft_toupper.c ft_calloc.c ft_strnstr.c ft_strchr.c ft_substr.c ft_strtrim.c ft_striteri.c \
	   ft_printf.c ft_printf_utils.c ft_atoi_longlong.c ft_itoa_longlong.c\

HEADER = libft.h

# Command to create a static library archive
AR = ar rcs

# Compiler to use
CC = cc

# Compilation flags
CFLAGS = -Wall -Wextra -Werror

# Object files
OBJS = ${SRCS:.c=.o}

# Default target
all: $(NAME)

# Create static library archive
$(NAME): ${OBJS}
	@echo "$(GREEN)Creating $(NAME)...$(RESET)"
	@$(AR) $@ $?

# Compile source files to object files
%.o: %.c
	@echo "$(GREEN)Compiling $<...$(RESET)"
	@$(CC) -c $(CFLAGS) $< -o $@

# Clean object files and bonus files
clean:
	@echo "$(CYAN)Cleaning object files...$(RESET)"
	@rm -f $(OBJS)

# Full clean
fclean: clean
	@echo "$(CYAN)Cleaning $(NAME)...$(RESET)"
	@rm -f $(NAME)

# Rebuild everything
re: fclean all

# Phony targets
.PHONY: all clean fclean re
