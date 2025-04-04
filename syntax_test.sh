#!/bin/bash

# List of test cases for invalid syntax
test_cases=(
    "&& echo hello"                    # Invalid: && at the start
    "echo hello ||"                    # Invalid: || at the end
    "echo hello || && echo world"      # Invalid: Consecutive logical operators
    "echo 'hello"                      # Invalid: Unmatched single quote
    "echo \"hello"                    # Invalid: Unmatched double quote
    "ls >"                              # Invalid: Redirection without target
    "| ls"                              # Invalid: Pipe at the start
    "ls |"                              # Invalid: Pipe at the end
    "ls ||| grep 'text'"               # Invalid: Consecutive pipes
    "(echo hello"                      # Invalid: Unmatched parenthesis
    "echo hello)"                      # Invalid: Unmatched parenthesis
    "echo hello\\"                     # Invalid: Trailing escape character
    "ls > < file"                      # Invalid: Invalid redirection combination
    "echo 'hello world\""             # Invalid: Mixed unmatched single and double quotes
    "ls <<< file"                      # Invalid: Triple redirection
    ""                                 # Invalid: Empty command
    "echo hello &&"                    # Invalid: Logical operator at the end
    "ls < file >"                      # Invalid: Incomplete redirection
    "|"                                # Invalid: Single pipe
    "<< file"                          # Invalid: Heredoc without end marker
    "("                                # Invalid: Single open parenthesis
    ")"                                # Invalid: Single close parenthesis
    "ls & echo"                        # Invalid: Single background operator

    
)

