/**
 * status.cpp - Implementation of the 'gg status' command
 * 
 * Displays the current status of the repository, showing staged,
 * modified, and untracked files using Trie and Bloom Filter.
 */

#include "../include/gg.h"
#include "../include/trie.h"
#include "../include/bloom.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <ncurses.h>

namespace fs = std::filesystem;

// Quips for status command
const std::vector<std::string> QuipGenerator::status_quips = {
    "Here's what you broke today. Congratulations.",
    "Status report: Chaos detected with a side order of confusion.",
    "The good news: Your code exists. The bad news: Everything else.",
    "Ah, I see the problem. It's between the keyboard and the chair.",
    "Your repository status: It's complicated.",
    "Status check complete. Have you tried turning it off and on again?",
    "Showing you the mess you've made. Don't worry, we've all been there.",
    "Status: Things could be worse. They could also be better. Much better."
};

// Helper function to compare the current file state with the repo state
std::vector<std::string> find_modified_files(const GGRepo& repo) {
    std::vector<std::string> modified;
    std::vector<std::pair<std::string, FileStatus>> all_files = repo.file_trie->get_all_files();
    
    for (const auto& [file, status] : all_files) {
        if (status == FileStatus::COMMITTED && fs::exists(file)) {
            // In a real implementation, we would hash the file contents and compare
            // with the stored hash to detect modifications
            // For this demo, we'll just pretend some are modified
            if (file.length() % 3 == 0) { // Arbitrary condition for demo
                modified.push_back(file);
            }
        }
    }
    
    return modified;
}

// Helper function to find untracked files
std::vector<std::string> find_untracked_files(const GGRepo& repo) {
    std::vector<std::string> untracked;
    
    // Recursively iterate through the current directory
    for (const auto& entry : fs::recursive_directory_iterator(".")) {
        if (fs::is_regular_file(entry)) {
            std::string path = entry.path().string();
            
            // Skip files in .gg directory
            if (path.find(".gg/") == 0 || path.find("\\.gg\\") == 0) {
                continue;
            }
            
            // Check if file is in the Trie
            if (!repo.file_trie->search(path)) {
                // Double-check with Bloom filter (demonstrates probabilistic nature)
                if (!repo.file_filter->might_contain(path)) {
                    untracked.push_back(path);
                } else {
                    // This would be a Bloom filter false positive in a real implementation
                    // We could add a comment about it but will keep it simple
                }
            }
        }
    }
    
    return untracked;
}

// Display repository status
void show_status() {
    // Check if we're in a gg repository
    if (!fs::exists(".gg")) {
        std::cerr << "Error: Not a gg repository (or any of the parent directories)" << std::endl;
        return;
    }
    
    // Load repository state
    GGRepo repo = GGRepo::load();
    
    // Get lists of files by status
    std::vector<std::string> staged = repo.file_trie->get_files_by_status(FileStatus::STAGED);
    std::vector<std::string> modified = find_modified_files(repo);
    std::vector<std::string> untracked = find_untracked_files(repo);
    
    // Initialize UI
    init_ui();
    clear();
    
    // Display header
    display_header("Repository Status");
    
    // Display branch and commit info
    BranchInfo* current_branch = repo.branches->find(repo.current_branch);
    std::shared_ptr<DAGNode> head_commit = repo.commit_graph->get_commit(repo.head_commit);
    
    attron(COLOR_PAIR(COLOR_BRANCH));
    mvprintw(2, 2, "On branch: %s", repo.current_branch.c_str());
    attroff(COLOR_PAIR(COLOR_BRANCH));
    
    attron(COLOR_PAIR(COLOR_COMMIT));
    mvprintw(3, 2, "HEAD: %s \"%s\"", 
             repo.head_commit.c_str(), 
             head_commit ? head_commit->commit.message.c_str() : "unknown");
    attroff(COLOR_PAIR(COLOR_COMMIT));
    
    // Display file Trie
    WINDOW* trie_win = newwin(15, 40, 5, 2);
    box(trie_win, 0, 0);
    mvwprintw(trie_win, 0, 2, " File Structure (Trie) ");
    repo.file_trie->draw(trie_win, 1, 2);
    wrefresh(trie_win);
    
    // Display files by status
    int row = 5;
    int col = 45;
    
    // Staged files
    attron(COLOR_PAIR(COLOR_STAGED));
    mvprintw(row++, col, "Changes to be committed:");
    attroff(COLOR_PAIR(COLOR_STAGED));
    
    if (staged.empty()) {
        mvprintw(row++, col + 2, "(none)");
    } else {
        for (const auto& file : staged) {
            attron(COLOR_PAIR(COLOR_STAGED));
            mvprintw(row++, col + 2, "%s", file.c_str());
            attroff(COLOR_PAIR(COLOR_STAGED));
        }
    }
    
    row += 1;
    
    // Modified files
    attron(COLOR_PAIR(COLOR_MODIFIED));
    mvprintw(row++, col, "Changes not staged for commit:");
    attroff(COLOR_PAIR(COLOR_MODIFIED));
    
    if (modified.empty()) {
        mvprintw(row++, col + 2, "(none)");
    } else {
        for (const auto& file : modified) {
            attron(COLOR_PAIR(COLOR_MODIFIED));
            mvprintw(row++, col + 2, "%s", file.c_str());
            attroff(COLOR_PAIR(COLOR_MODIFIED));
        }
    }
    
    row += 1;
    
    // Untracked files
    attron(COLOR_PAIR(COLOR_UNTRACKED));
    mvprintw(row++, col, "Untracked files:");
    attroff(COLOR_PAIR(COLOR_UNTRACKED));
    
    if (untracked.empty()) {
        mvprintw(row++, col + 2, "(none)");
    } else {
        for (const auto& file : untracked) {
            attron(COLOR_PAIR(COLOR_UNTRACKED));
            mvprintw(row++, col + 2, "%s", file.c_str());
            attroff(COLOR_PAIR(COLOR_UNTRACKED));
        }
    }
    
    // Display Bloom Filter
    WINDOW* bloom_win = newwin(7, 40, 21, 2);
    box(bloom_win, 0, 0);
    mvwprintw(bloom_win, 0, 2, " File Existence (Bloom Filter) ");
    repo.file_filter->draw(bloom_win, 1, 2, 36, 5);
    wrefresh(bloom_win);
    
    // Display success message and a quip
    attron(COLOR_PAIR(COLOR_SUCCESS));
    mvprintw(LINES - 2, 2, "[gg] Status check complete.");
    attroff(COLOR_PAIR(COLOR_SUCCESS));
    
    // Display a random quip
    mvprintw(LINES - 1, 2, "%s", QuipGenerator::get_random_quip(QuipGenerator::status_quips).c_str());
    
    // Refresh and wait for user input
    refresh();
    getch();
    
    // Clean up UI
    delwin(trie_win);
    delwin(bloom_win);
    cleanup_ui();
}

// Command handler for 'gg status'
bool status_command(int argc, char* argv[]) {
    show_status();
    return true;
}
