/**
 * kermit.cpp - Implementation of the 'gg kermit' and 'gg commit' commands
 * 
 * Creates a new commit in the DAG with the staged changes, updating
 * multiple data structures to track the commit history.
 */

#include "../include/gg.h"
#include "../include/trie.h"
#include "../include/dag.h"
#include "../include/skiplist.h"
#include <iostream>
#include <filesystem>
#include <sstream>
#include <ncurses.h>

namespace fs = std::filesystem;

// Quips for commits
const std::vector<std::string> QuipGenerator::commit_quips = {
    "Kermit successful. A legendary commit. Or a legendary disaster. Time will tell.",
    "Changes committed. What could possibly go wrong? (Don't answer that.)",
    "Commit recorded. This is why we can't have nice things.",
    "Commit successful. Your boss will never know how long this really took you.",
    "Immortalized in the DAG. Your bugs are now part of history.",
    "Commit completed. The author of this code shall remain anonymous (smart choice).",
    "Committed! Your future self will wonder what you were thinking.",
    "Commit successful. Pretending we understand the code we just wrote."
};

// Create a new commit with staged changes
bool commit_changes(const std::string& message) {
    // Check if we're in a gg repository
    if (!fs::exists(".gg")) {
        std::cerr << "Error: Not a gg repository (or any of the parent directories)" << std::endl;
        return false;
    }
    
    // Load repository state
    GGRepo repo = GGRepo::load();
    
    // Check if there are staged changes
    std::vector<std::string> staged_files = repo.file_trie->get_files_by_status(FileStatus::STAGED);
    if (staged_files.empty()) {
        std::cerr << "Error: No staged changes to commit" << std::endl;
        return false;
    }
    
    // Initialize UI
    init_ui();
    clear();
    
    // Display header
    display_header("Creating New Commit");
    
    // Display the commit message
    attron(COLOR_PAIR(COLOR_DEFAULT));
    mvprintw(2, 2, "Message: \"%s\"", message.c_str());
    attroff(COLOR_PAIR(COLOR_DEFAULT));
    
    // Generate a new commit ID
    std::string commit_id = generate_commit_id();
    
    // Create a new commit
    Commit new_commit;
    new_commit.id = commit_id;
    new_commit.message = message;
    new_commit.author = "gg"; // In a real implementation, this would be configurable
    new_commit.timestamp = get_current_time_string();
    new_commit.parent_ids.push_back(repo.head_commit);
    
    // Draw current DAG (before update)
    int row = 5;
    int col = 2;
    
    WINDOW* dag_before_win = newwin(10, 50, row, col);
    box(dag_before_win, 0, 0);
    mvwprintw(dag_before_win, 0, 2, " Commit Graph (Before) ");
    repo.commit_graph->draw(dag_before_win, repo.head_commit);
    wrefresh(dag_before_win);
    
    row += 11;
    
    // Draw Skip List before update
    WINDOW* skip_before_win = newwin(8, 50, row, col);
    box(skip_before_win, 0, 0);
    mvwprintw(skip_before_win, 0, 2, " Skip List (Before) ");
    repo.commit_list->draw(skip_before_win, 1, 2);
    wrefresh(skip_before_win);
    
    // Add staged files to commit
    attron(COLOR_PAIR(COLOR_STAGED));
    mvprintw(3, 2, "Staged files:");
    int file_row = 4;
    for (const auto& file : staged_files) {
        mvprintw(file_row++, 4, "- %s", file.c_str());
        
        // In a real implementation, we would calculate file content hashes
        // and store them in the commit's file_snapshots map
        new_commit.file_snapshots[file] = "dummy_hash_" + file;
        
        // Update file status in trie
        repo.file_trie->update_status(file, FileStatus::COMMITTED);
    }
    attroff(COLOR_PAIR(COLOR_STAGED));
    
    // Add new commit to DAG
    repo.commit_graph->add_commit(new_commit, {repo.head_commit});
    repo.head_commit = commit_id;
    
    // Update current branch to point to new commit
    repo.branches->update_commit(repo.current_branch, commit_id, new_commit.timestamp);
    
    // Add commit to skip list
    repo.commit_list->insert(commit_id, new_commit.timestamp);
    
    // Column for "after" panels
    col += 52;
    row = 5;
    
    // Draw updated DAG
    WINDOW* dag_after_win = newwin(10, 50, row, col);
    box(dag_after_win, 0, 0);
    mvwprintw(dag_after_win, 0, 2, " Commit Graph (After) ");
    repo.commit_graph->draw(dag_after_win, repo.head_commit);
    wrefresh(dag_after_win);
    
    row += 11;
    
    // Draw Skip List after update
    WINDOW* skip_after_win = newwin(8, 50, row, col);
    box(skip_after_win, 0, 0);
    mvwprintw(skip_after_win, 0, 2, " Skip List (After) ");
    repo.commit_list->draw(skip_after_win, 1, 2);
    wrefresh(skip_after_win);
    
    // Animate the commit creation
    for (int i = 0; i < 3; i++) {
        attron(COLOR_PAIR(COLOR_HIGHLIGHT));
        mvprintw(file_row + 1, 2, "Creating commit... %s", std::string(i + 1, '.').c_str());
        attroff(COLOR_PAIR(COLOR_HIGHLIGHT));
        refresh();
        napms(300); // Sleep for 300ms
    }
    
    // Save repository state
    repo.save();
    
    // Display success message and quip
    attron(COLOR_PAIR(COLOR_SUCCESS));
    mvprintw(24, 2, "[gg] Kermit successful: \"%s\"", message.c_str());
    mvprintw(25, 2, "Commit ID: %s", commit_id.c_str());
    attroff(COLOR_PAIR(COLOR_SUCCESS));
    
    // Display a random quip
    mvprintw(26, 2, "%s", QuipGenerator::get_random_quip(QuipGenerator::commit_quips).c_str());
    
    // Refresh and wait for user input
    refresh();
    getch();
    
    // Clean up UI
    delwin(dag_before_win);
    delwin(skip_before_win);
    delwin(dag_after_win);
    delwin(skip_after_win);
    cleanup_ui();
    
    return true;
}

// Command handler for 'gg kermit' and 'gg commit'
bool commit_command(int argc, char* argv[]) {
    // Check for -m message argument
    if (argc < 3 || strcmp(argv[1], "-m") != 0) {
        std::cerr << "Error: Missing commit message" << std::endl;
        std::cerr << "Usage: gg kermit -m \"<message>\"" << std::endl;
        std::cerr << "   or: gg commit -m \"<message>\"" << std::endl;
        return false;
    }
    
    return commit_changes(argv[2]);
}
