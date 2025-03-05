NAME = my_ping

SRC_DIR = src
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

CC = clang
CFLAGS = -g -Wall -Wextra -Werror -O0


.PHONY: all
all: help

.PHONY: help
help: ## Prints help for targets with comments
	@echo "Available Rules:"
	@cat $(MAKEFILE_LIST) | grep -E '^[a-zA-Z_-]+:.*?## .*$$' | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)


.PHONY: build
build: $(NAME) ## Builds the binary

.PHONY: clean
clean: ## Cleans transitive dependencies
	@rm -f $(OBJS)

.PHONY: fclean 
fclean: clean ## Cleans transitive dependencies and the binary
	@rm -f $(NAME)

.PHONY: re
re: fclean build ## Rebuilds the binary


.PHONY: t
t: $(NAME) ## Runs the binary for test
	@./$(NAME)
