#!/bin/bash

# ANSI Color Codes for better readability
RED="\033[1;31m"
GREEN="\033[1;32m"
YELLOW="\033[1;33m"
BLUE="\033[1;34m"
CYAN="\033[1;36m"
RESET="\033[0m"

MINISHELL="./minishell"

# Check if minishell binary exists
if [ ! -x "$MINISHELL" ]; then
    echo -e "${RED}Error: $MINISHELL not found or not executable.${RESET}"
    exit 1
fi

# Helper function to run tests
run_test() {
    local test_name="$1"
    local commands="$2"

    # Remove "exit" command from displayed input
    local display_commands=$(echo "$commands" | sed '/exit/d')

    echo -e "${YELLOW}------------------------------------------"
    echo -e "Test: ${CYAN}$test_name${YELLOW}"
    echo -e "------------------------------------------${RESET}"

    echo -e "${BLUE}Input commands:${RESET}"
    echo -e "${GREEN}$display_commands${RESET}"
    echo -e "${YELLOW}------------------------------------------${RESET}"

    echo -e "${BLUE}Minishell Output:${RESET}"
    printf "%b\nexit\n" "$commands" | "$MINISHELL"

    echo -e "${YELLOW}------------------------------------------${RESET}\n"
}

# -----------------------------
# Test 1: Simple echo
# -----------------------------
run_test "Simple echo" "\
echo Hello World
"

# -----------------------------
# Test 2: Echo with double quotes & variations
# -----------------------------
run_test "Echo with quotes" "\
echo \"Hello 42\"
\"ec\"\"ho\" Hello
ec\"ho\" world
ec\"h\"o\"\"
echo\"\$USER\"
echo \"\$USER\"
echo\"\$USER\$\"
"

# -----------------------------
# Test 3: Extra spaces before/after command
# -----------------------------
run_test "Extra spaces" "\
   ls   -l
"

# -----------------------------
# Test 4: Empty input (just press enter)
# -----------------------------
run_test "Empty input" "\

"

# -----------------------------
# Test 5: Nonexistent command (should show error)
# -----------------------------
run_test "Nonexistent command" "\
foobar123
"

# -----------------------------
# Test 6: Environment variable expansion
# -----------------------------
run_test "Environment variable" "\
echo \$HOME
echo \$USER
echo \$PATH
echo \$dilara
"

# -----------------------------
# Test 7: Built-in commands (cd and pwd)
# -----------------------------
run_test "Builtin cd and pwd" "\
pwd
cd ..
pwd
"

# -----------------------------
# Test 8: Redirection (create, read, and cleanup a file)
# -----------------------------
run_test "Redirection" "\
echo Hello > testfile.txt
cat testfile.txt
rm testfile.txt
"

# -----------------------------
# Test 9: Pipe usage
# -----------------------------
run_test "Pipe command" "\
echo Hello | cat
"

# -----------------------------
# Test 10: Multiple commands separated by semicolon
# -----------------------------
run_test "Multiple commands" "\
echo First; echo Second; echo Third
"

# -----------------------------
# Test 11: Syntax error (unclosed quote)
# -----------------------------
run_test "Syntax error (unclosed quote)" "\
echo \"Unclosed quote
\"should not appear\"
"

# -----------------------------
# Test 12: Pipe with missing command before the pipe
# -----------------------------
run_test "Pipe with missing command" "\
| ls
cmd >
echo dilara &&
&& echo hello
"

# -----------------------------
# Test 13: Input containing only spaces
# -----------------------------
run_test "Only spaces" "\


"

echo -e "${GREEN}All tests executed.${RESET}"
