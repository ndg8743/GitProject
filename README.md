# gg - Git Learning CLI Tool

A command-line based version control system that teaches Git concepts visually through the terminal, demonstrating advanced data structures with ASCII art visualizations.

## Overview

`gg` is an educational tool designed to help users learn version control concepts and understand the data structures that power Git. It uses ASCII art and live visualizations through the ncurses library to illustrate how repositories, commits, branches, and merges work behind the scenes.

## Quick Start (One-Command Installation)

### On Linux/macOS:
```bash
# Download and run the installer
git clone https://github.com/yourusername/gg.git
cd gg
chmod +x install.sh
./install.sh
```

### On Windows:
```
# Download the repository
git clone https://github.com/yourusername/gg.git
cd gg

# Right-click on install.bat and select "Run as administrator"
```

The installation script will automatically:
1. Check for and install required dependencies (C++ compiler, ncurses)
2. Compile the project
3. Provide instructions for getting started

## Features

- Visual demonstrations of repository operations
- Live ASCII art showing data structure changes
- Educational tutorials for each data structure
- Informative (and sometimes humorous) feedback
- Compatible with all terminal environments

## Data Structures

`gg` demonstrates the following advanced data structures:

1. **Directed Acyclic Graph (DAG)** - Used for representing commit history
   - Located in `dag.h`/`dag.cpp`
   - Enables non-linear history tracking
   - Supports branching and merging operations

2. **Trie** - Used for efficient file path storage and querying
   - Located in `trie.h`/`trie.cpp`
   - Tracks file status (staged, committed, modified)
   - Enables prefix-based operations

3. **AVL Tree** - Self-balancing binary search tree for branch management
   - Located in `avl.h`/`avl.cpp`
   - Maintains branches in sorted order
   - Demonstrates tree rotations for balancing

4. **Skip List** - Probabilistic data structure for efficient commit traversal
   - Located in `skiplist.h`/`skiplist.cpp`
   - Provides O(log n) search with simpler implementation than balanced trees
   - Visualizes "express lanes" concept

5. **Disjoint Set (Union-Find)** - Used for merge conflict resolution
   - Located in `disjoint.h`/`disjoint.cpp`
   - Efficiently tracks connected components
   - Demonstrates path compression and union by rank

6. **Bloom Filter** - Space-efficient probabilistic data structure
   - Located in `bloom.h`/`bloom.cpp`
   - Used for fast file existence checking
   - Shows trade-offs between space and accuracy

## Manual Compilation Instructions

### Requirements

- C++17 compatible compiler (g++ or clang++)
- ncurses library
- pthread library
- make

### Building from source

```bash
# Clone the repository
git clone https://github.com/yourusername/gg.git
cd gg

# Compile the project
make

# Optional: Install system-wide (may require sudo)
make install
```

## Running Instructions

After compilation, you can run `gg` directly from the bin directory:

```bash
./bin/gg [command] [options]
```

Or if you installed it:

```bash
gg [command] [options]
```

## Commands

- `gg bruh` - Initialize a new repository
- `gg add <file>` - Stage a file for commit
- `gg kermit -m "message"` - Commit staged changes (alias: `gg commit`)
- `gg status` - Show repository status
- `gg log` - Show commit history
- `gg branch <name>` - Create a new branch
- `gg merge <branch>` - Merge a branch into current branch
- `gg learn <topic>` - Learn about the data structures

### Learning Topics

- `gg learn dag` - Learn about Directed Acyclic Graphs
- `gg learn trie` - Learn about Tries
- `gg learn avl` - Learn about AVL Trees
- `gg learn skiplist` - Learn about Skip Lists
- `gg learn disjointset` - Learn about Disjoint Sets
- `gg learn bloom` - Learn about Bloom Filters

## Example Workflow

```bash
# Initialize a new repository
gg bruh

# Create and stage a file
echo "Hello, world!" > hello.txt
gg add hello.txt

# Commit the changes
gg kermit -m "Initial commit"

# Check repository status
gg status

# Create a branch for new features
gg branch feature1

# View commit history
gg log

# Learn about DAGs
gg learn dag
```

## Project Structure

```
gg/
├── bin/                 # Compiled binary
├── include/             # Header files
│   ├── gg.h             # Global headers
│   ├── trie.h           # File tracking Trie
│   ├── dag.h            # Commit graph DAG
│   ├── avl.h            # Branch AVL tree
│   ├── skiplist.h       # Skip list for commits
│   ├── disjoint.h       # Union-Find for merges
│   └── bloom.h          # Bloom filter for file existence
├── src/                 # Source files
│   ├── main.cpp         # CLI entrypoint, argument parsing
│   ├── init.cpp         # Implements gg bruh
│   ├── add.cpp          # Implements gg add
│   ├── kermit.cpp       # Implements gg kermit / gg commit
│   ├── status.cpp       # Implements gg status
│   ├── log.cpp          # Implements gg log
│   ├── branch.cpp       # Implements gg branch
│   ├── merge.cpp        # Implements gg merge
│   └── learn.cpp        # Implements gg learn
├── obj/                 # Object files (generated)
├── install.sh           # Automated installer for Linux/macOS
├── install.bat          # Automated installer for Windows
├── Makefile             # Build script
└── README.md            # This file
```

## For Complete Beginners

If you're completely new to programming and terminals, simply:

1. Download the project
2. Run the installer for your operating system:
   - On Windows: Double-click `install.bat` and follow the prompts
   - On Mac/Linux: Open Terminal, navigate to the gg folder, and run `./install.sh`
3. The installer will set up everything needed to run gg

## Notes

- This is an educational tool, not a production-grade version control system
- It focuses on visualizing and explaining the data structures behind Git
- The storage format is not compatible with Git
