/**
 * main.cpp - CLI entrypoint for gg version control system
 * 
 * Handles command-line argument parsing and dispatches to the appropriate
 * command implementation.
 */

#include "../include/gg.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <cstring>

// Function prototypes for commands
bool init_command(int argc, char* argv[]);
bool add_command(int argc, char* argv[]);
bool commit_command(int argc, char* argv[]);
bool status_command(int argc, char* argv[]);
bool log_command(int argc, char* argv[]);
bool branch_command(int argc, char* argv[]);
bool merge_command(int argc, char* argv[]);
bool learn_command(int argc, char* argv[]);

// Help message
void show_help() {
    std::cout << "gg - A Visual Git Learning Tool\n\n";
    std::cout << "Usage: gg <command> [options]\n\n";
    std::cout << "Commands:\n";
    std::cout << "  bruh                Initialize a new gg repository\n";
    std::cout << "  add <file>          Stage file for commit\n";
    std::cout << "  kermit -m <msg>     Commit staged changes\n";
    std::cout << "  commit -m <msg>     Same as kermit\n";
    std::cout << "  status              Show repository status\n";
    std::cout << "  log                 Show commit history\n";
    std::cout << "  branch <name>       Create a new branch\n";
    std::cout << "  merge <branch>      Merge branch into current branch\n";
    std::cout << "  learn <topic>       Learn about git concepts\n\n";
    std::cout << "Topics for learn:\n";
    std::cout << "  dag                 Directed Acyclic Graph (commit history)\n";
    std::cout << "  trie                Trie data structure (file tracking)\n";
    std::cout << "  avl                 AVL Tree (branch management)\n";
    std::cout << "  skiplist            Skip List (efficient commit traversal)\n";
    std::cout << "  disjointset         Disjoint Set (merge conflict resolution)\n";
    std::cout << "  bloom               Bloom Filter (file existence checking)\n";
}

// Version message
void show_version() {
    std::cout << "gg - Version 1.0.0\n";
    std::cout << "A Visual Git Learning Tool\n";
}

// Command dispatcher table
std::map<std::string, std::function<bool(int, char**)>> command_table = {
    {"bruh", init_command},
    {"add", add_command},
    {"kermit", commit_command},
    {"commit", commit_command},
    {"status", status_command},
    {"log", log_command},
    {"branch", branch_command},
    {"merge", merge_command},
    {"learn", learn_command}
};

int main(int argc, char* argv[]) {
    // Handle no arguments
    if (argc < 2) {
        show_help();
        return 1;
    }
    
    // Handle help and version flags
    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        show_help();
        return 0;
    }
    
    if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0) {
        show_version();
        return 0;
    }
    
    // Look up command in dispatch table
    auto cmd_it = command_table.find(argv[1]);
    if (cmd_it == command_table.end()) {
        std::cerr << "Error: Unknown command '" << argv[1] << "'\n";
        std::cerr << "Use 'gg --help' to see available commands.\n";
        return 1;
    }
    
    // Execute the command handler
    try {
        bool success = cmd_it->second(argc - 1, argv + 1);
        return success ? 0 : 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
