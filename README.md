## Features Implemented So Far

### 1. User Input Handling
- Used the `readline` function to read user input.
- Handled `CTRL+D` (EOF) to ensure the program exits gracefully.
- Displayed the prompt as `minishell>`.

### 2. Empty Input Management
- Empty inputs (`""`) are ignored, and the shell continues waiting for valid commands.

### 3. Argument Check
- Ensures the shell runs with exactly one argument (`./minishell`).
- Displays an error message for additional arguments.

---

## Planned Features

### 1. Tokenization
- Split user input into meaningful pieces (tokens) for further processing.
  - Example:
    ```plaintext
    Input: ls -la | grep "test"
    Tokens: ["ls", "-la", "|", "grep", "test"]
    ```
- Store tokens in a `t_token` structure using a linked list.

### 2. Parsing
- Parse tokens to identify:
  - Pipes (`|`)
  - Redirections (`>`, `<`)
  - Environment variable expansions (`$VAR`)
- Build an **Abstract Syntax Tree (AST)** for structured command representation.

### 3. Command Execution
- Execute commands based on tokens or the AST.
  - **Simple Commands:** Execute commands like `ls`, `pwd`.
  - **Redirections:** Handle input/output redirection (`>`, `<`).
  - **Pipes:** Support command chaining using pipes (`|`).

### 4. Environment Variable Management
- Convert `envp` into a manipulatable linked list.
- Support variable expansion for constructs like `$HOME` and `$PATH`.
- Use the `PATH` environment variable to locate and execute commands.

### 5. Built-in Commands
- Implement the following built-in commands:
  - `echo`
  - `cd`
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`

### 6. Signal Handling
- Gracefully handle user-generated signals like:
  - **`CTRL+C`**: Interrupt the current command and return to the prompt.
  - **`CTRL+\`**: Ignore without disrupting the shell.

### 7. Memory Management
- Write cleanup functions for:
  - Tokens
  - AST
  - Environment variables
- Free all dynamically allocated memory at the end of each command execution loop.

### 8. Error Handling
- Display meaningful error messages for:
  - Invalid commands
  - Improper use of pipes or redirections
- Track errors using an error code variable (e.g., `g_minishell.error_code`).

### 9. Command History
- Save command history using `add_history`.
- Allow users to navigate previous commands with arrow keys.

---

## Roadmap

### Prioritized Development Steps
1. Tokenization: Process user input and break it into meaningful tokens.
2. Parsing: Transform tokens into a structured form (AST).
3. Command Execution: Execute basic commands.
4. Environment Variables: Implement `$PATH` and other variable management.
5. Built-in Commands: Add the shell's core functionalities.
6. Memory Management and Error Handling: Ensure robust memory usage and clear error messages.
7. Bonus Features: Expand with advanced functionalities like logical operators (`&&`, `||`) and wildcard expansion (`*`).

https://chatgpt.com/share/676fe80c-a718-800d-8865-fc69c7b165c1