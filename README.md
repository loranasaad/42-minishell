*This project has been created as part of the 42 curriculum by loasaad, latabagl.*

# Minishell — As beautiful as a shell

## Description

A Unix shell implementation in C, built as part of the 42 curriculum. Minishell 
replicates core bash behavior: parsing and executing commands, managing processes 
and file descriptors, handling pipes, redirections, environment variables, and signals.

This repository contains the **mandatory part only**. No bonus features are included.

## Features

- Interactive prompt with command history
- Command execution via `PATH`, relative, and absolute paths
- Quote handling: single quotes `'...'` and double quotes `"..."` with `$` expansion
- Redirections: `<`, `>`, `>>`, `<<` (heredoc)
- Pipes `|`
- Environment variable expansion (`$VAR`, `$?`)
- Signal handling: `ctrl-C`, `ctrl-D`, `ctrl-\`
- Built-in commands: `echo -n`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`

## Instructions

### Compile
```bash
make
```

### Run
```bash
./minishell
```

### Clean
```bash
make clean    # remove object files
make fclean   # remove object files and binary
make re       # fclean + compile
```

> Requires `readline`. On macOS: `brew install readline`

## Usage Examples
```bash
echo "$HOME"
echo '$HOME'
ls -la | grep minishell
cat < infile | grep test > outfile
cat << EOF
hello
world
EOF
echo $?
```

## Authors
```
loasaad
latabagl
```
