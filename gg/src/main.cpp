#include "../include/gg.h"
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    // Simple command-line interface without ncurses
    std::cout << "gg - Git Learning CLI Tool" << std::endl;
    std::cout << "===========================" << std::endl;
    std::cout << std::endl;

    if (argc < 2) {
        std::cout << "Usage: gg <command> [options]" << std::endl;
        std::cout << std::endl;
        std::cout << "Commands:" << std::endl;
        std::cout << "  bruh                    Initialize a new repository" << std::endl;
        std::cout << "  add <file>              Stage a file" << std::endl;
        std::cout << "  kermit -m \"message\"     Commit changes" << std::endl;
        std::cout << "  status                  Show repository status" << std::endl;
        std::cout << "  log                     Show commit history" << std::endl;
        std::cout << "  branch [name]           Create or list branches" << std::endl;
        std::cout << "  merge <branch>          Merge a branch" << std::endl;
        std::cout << "  learn <topic>           Learn about a topic" << std::endl;
        std::cout << std::endl;
        std::cout << "For more information, see the README.md file." << std::endl;
        return 0;
    }

    std::string command = argv[1];

    if (command == "bruh") {
        std::cout << "Initializing a new repository..." << std::endl;
        std::cout << "This feature is not available in the simplified version." << std::endl;
    } else if (command == "add") {
        if (argc < 3) {
            std::cout << "Error: No file specified" << std::endl;
            std::cout << "Usage: gg add <file>" << std::endl;
            return 1;
        }
        std::cout << "Staging file: " << argv[2] << std::endl;
        std::cout << "This feature is not available in the simplified version." << std::endl;
    } else if (command == "kermit" || command == "commit") {
        if (argc < 3 || std::string(argv[2]) != "-m") {
            std::cout << "Error: Missing commit message" << std::endl;
            std::cout << "Usage: gg kermit -m \"<message>\"" << std::endl;
            std::cout << "   or: gg commit -m \"<message>\"" << std::endl;
            return 1;
        }
        if (argc < 4) {
            std::cout << "Error: Missing commit message" << std::endl;
            std::cout << "Usage: gg kermit -m \"<message>\"" << std::endl;
            std::cout << "   or: gg commit -m \"<message>\"" << std::endl;
            return 1;
        }
        std::cout << "Committing changes with message: " << argv[3] << std::endl;
        std::cout << "This feature is not available in the simplified version." << std::endl;
    } else if (command == "status") {
        std::cout << "Showing repository status..." << std::endl;
        std::cout << "This feature is not available in the simplified version." << std::endl;
    } else if (command == "log") {
        std::cout << "Showing commit history..." << std::endl;
        std::cout << "This feature is not available in the simplified version." << std::endl;
    } else if (command == "branch") {
        if (argc < 3) {
            std::cout << "Listing branches..." << std::endl;
        } else {
            std::cout << "Creating branch: " << argv[2] << std::endl;
        }
        std::cout << "This feature is not available in the simplified version." << std::endl;
    } else if (command == "merge") {
        if (argc < 3) {
            std::cout << "Error: No branch specified" << std::endl;
            std::cout << "Usage: gg merge <branch-name>" << std::endl;
            return 1;
        }
        std::cout << "Merging branch: " << argv[2] << std::endl;
        std::cout << "This feature is not available in the simplified version." << std::endl;
    } else if (command == "learn") {
        if (argc < 3) {
            std::cout << "Error: No topic specified" << std::endl;
            std::cout << "Usage: gg learn <topic>" << std::endl;
            std::cout << "Topics: dag, trie, avl, skiplist, disjointset, bloom" << std::endl;
            return 1;
        }
        
        std::string topic = argv[2];
        
        if (topic == "dag") {
            std::cout << "Learning about DAG (Directed Acyclic Graph)..." << std::endl;
            learn_dag();
        } else if (topic == "trie") {
            std::cout << "Learning about Trie..." << std::endl;
            learn_trie();
        } else if (topic == "avl") {
            std::cout << "Learning about AVL Tree..." << std::endl;
            learn_avl();
        } else if (topic == "skiplist") {
            std::cout << "Learning about Skip List..." << std::endl;
            learn_skiplist();
        } else if (topic == "disjoint" || topic == "disjointset") {
            std::cout << "Learning about Disjoint Set..." << std::endl;
            std::cout << "This feature is not available in the simplified version." << std::endl;
        } else if (topic == "bloom") {
            std::cout << "Learning about Bloom Filter..." << std::endl;
            std::cout << "This feature is not available in the simplified version." << std::endl;
        } else {
            std::cout << "Error: Unknown topic '" << topic << "'" << std::endl;
            std::cout << "Topics: dag, trie, avl, skiplist, disjointset, bloom" << std::endl;
            return 1;
        }
    } else {
        std::cout << "Error: Unknown command '" << command << "'" << std::endl;
        std::cout << "Run 'gg' without arguments to see available commands." << std::endl;
        return 1;
    }

    return 0;
}
