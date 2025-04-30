# Mini UNIX Shell in C++

A simplified UNIX-like shell implementation demonstrating advanced systems programming concepts in C++.

## Features

- **Command Execution**: Execute standard UNIX commands
- **I/O Redirection**: Support for `>`, `<`, and `>>` operators
- **Pipeline Support**: Chain commands with the `|` operator
- **Background Processes**: Run commands in the background with `&`
- **Built-in Commands**: Includes `cd`, `exit`, `help`, and `jobs`
- **Process Tracking**: Monitors and reports on process PIDs and statuses
- **Environment Variables**: Expands variables like `$HOME` in commands

## Technical Implementation

The shell demonstrates knowledge of:
- Process control (`fork()`, `exec()`, `wait()`)
- File descriptor manipulation
- Signal handling
- Pipeline creation and management
- Memory management
- String parsing and tokenization
- File I/O operations

## Building and Running

```bash
# Build the shell
make

# Run the shell
make run

# Or directly:
./mini_shell
```

## Usage Examples

```bash
# Basic commands
ls -la
pwd

# I/O redirection
ls > file_list.txt
cat < input.txt
ls -la >> log.txt

# Pipelines
ls -la | grep ".cpp" | sort

# Background processes
sleep 10 &

# View running jobs
jobs

# Change directory
cd /tmp

# Get help
help

# Exit the shell
exit
```

## Extension Possibilities

- Job control (`fg`, `bg` commands)
- Command history with up/down arrows
- Tab completion
- Custom built-in commands with C++ optimization
- Resource monitoring

## Implementation Details

The shell is built using a class-based architecture:
- `MiniShell`: Main shell class handling the command loop
- `Command`: Represents a parsed command with arguments and I/O specifications
- `Process`: Tracks background process information

Key system calls utilized:
- `fork()`: Creates new processes
- `execvp()`: Executes commands
- `pipe()`: Creates pipelines between commands
- `dup2()`: Redirects standard I/O
- `waitpid()`: Waits for process completion
- `chdir()`: Changes directories
- `signal()`: Handles signals

## Future Improvements

- Support for environment variable setting (`export VAR=value`)
- Wildcard expansion
- Command history persistence
- Customizable prompt
- Script execution

## Learning Value

This project demonstrates:
- Deep understanding of UNIX process model
- File descriptor manipulation
- Signal handling
- Low-level I/O operations
- String parsing and command interpretation
- Memory management in C++
