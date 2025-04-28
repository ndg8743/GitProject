/**
 * add.cpp - Implementation of the 'gg add' command
 * 
 * Stages a file for commit by adding it to the repository's tracking system,
 * updating the Trie and Bloom Filter data structures.
 */

#include "../include/gg.h"
#include "../include/trie.h"
#include "../include/bloom.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <ncurses.h>

namespace fs = std::filesystem;

// Quips for adding files
const std::vector<std::string> QuipGenerator::add_quips = {
    "If this crashes, just blame it on 'works on my machine.'",
    "File staged. Hope you know what you're doing. I sure don't.",
    "Added to staging area. The point of no return is getting closer.",
    "File tracked. Now it can never escape your oversight. Muhahaha!",
    "Staged. And in a few more commands, your bugs become immortalized.",
    "File added. One step closer to blaming someone else when it breaks.",
    "File staged. Your future self will wonder why you did this.",
    "Successfully tracked. Future you will either thank you or curse your name."
};

// Add a file to the staging area
bool add_file(const std::string& filepath) {
    // Check if we're in a gg repository
    if (!fs::exists(".gg")) {
        std::cerr << "Error: Not a gg repository (or any of the parent directories)" << std::endl;
        return false;
    }
    
    // Check if file exists
    if (!fs::exists(filepath)) {
        std::cerr << "Error: File '" << filepath << "' does not exist" << std::endl;
        return false;
    }
    
    // Load repository state
    GGRepo repo = GGRepo::load();
    
    // Initialize UI
    init_ui();
    clear();
    
    // Display header
    display_header("Adding File to Repository");
    
    // Display the file being added
    attron(COLOR_PAIR(COLOR_DEFAULT));
    mvprintw(2, 2, "File: %s", filepath.c_str());
    attroff(COLOR_PAIR(COLOR_DEFAULT));
    
    // Draw initial data structures - pre-update
    int row = 5;
    int col = 2;
    
    // Draw Trie before update
    WINDOW* trie_before_win = newwin(8, 40, row, col);
    box(trie_before_win, 0, 0);
    mvwprintw(trie_before_win, 0, 2, " File Structure (Before) ");
    repo.file_trie->draw(trie_before_win, 1, 2);
    wrefresh(trie_before_win);
    
    row += 9;
    
    // Draw Bloom Filter before update
    WINDOW* bloom_before_win = newwin(8, 40, row, col);
    box(bloom_before_win, 0, 0);
    mvwprintw(bloom_before_win, 0, 2, " Bloom Filter (Before) ");
    repo.file_filter->draw(bloom_before_win, 1, 2, 36, 6);
    wrefresh(bloom_before_win);
    
    // Update the data structures
    repo.file_trie->insert(filepath, FileStatus::STAGED);
    repo.file_filter->add(filepath);
    
    // Column for "after" panels
    col += 42;
    row = 5;
    
    // Draw Trie after update
    WINDOW* trie_after_win = newwin(8, 40, row, col);
    box(trie_after_win, 0, 0);
    mvwprintw(trie_after_win, 0, 2, " File Structure (After) ");
    repo.file_trie->draw(trie_after_win, 1, 2);
    wrefresh(trie_after_win);
    
    row += 9;
    
    // Draw Bloom Filter after update
    WINDOW* bloom_after_win = newwin(8, 40, row, col);
    box(bloom_after_win, 0, 0);
    mvwprintw(bloom_after_win, 0, 2, " Bloom Filter (After) ");
    repo.file_filter->draw(bloom_after_win, 1, 2, 36, 6);
    wrefresh(bloom_after_win);
    
    // Animate the insertion into the Trie
    // (This would be a more complex animation in a real implementation)
    for (int i = 0; i < 3; i++) {
        attron(COLOR_PAIR(COLOR_HIGHLIGHT));
        mvprintw(3, 2, "Updating data structures... %s", std::string(i + 1, '.').c_str());
        attroff(COLOR_PAIR(COLOR_HIGHLIGHT));
        refresh();
        napms(300); // Sleep for 300ms
    }
    
    // Save repository state
    repo.save();
    
    // Display success message and a quip
    attron(COLOR_PAIR(COLOR_SUCCESS));
    mvprintw(24, 2, "[gg] Staged %s.", filepath.c_str());
    attroff(COLOR_PAIR(COLOR_SUCCESS));
    
    // Display a random quip
    mvprintw(25, 2, "%s", QuipGenerator::get_random_quip(QuipGenerator::add_quips).c_str());
    
    // Refresh and wait for user input
    refresh();
    getch();
    
    // Clean up UI
    delwin(trie_before_win);
    delwin(bloom_before_win);
    delwin(trie_after_win);
    delwin(bloom_after_win);
    cleanup_ui();
    
    return true;
}

// Command handler for 'gg add'
bool add_command(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: No file specified" << std::endl;
        std::cerr << "Usage: gg add <file>" << std::endl;
        return false;
    }
    
    return add_file(argv[1]);
}
