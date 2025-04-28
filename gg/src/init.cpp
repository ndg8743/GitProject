/**
 * init.cpp - Implementation of the 'gg bruh' command
 * 
 * Initializes a gg repository by creating a .gg folder with necessary
 * configuration and data structure initialization.
 */

#include "../include/gg.h"
#include "../include/trie.h"
#include "../include/dag.h"
#include "../include/avl.h"
#include "../include/skiplist.h"
#include "../include/disjoint.h"
#include "../include/bloom.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <ctime>
#include <random>
#include <ncurses.h>

namespace fs = std::filesystem;

// Quips for repository initialization
const std::vector<std::string> QuipGenerator::bruh_quips = {
    "Ready to bruh responsibly. Or not.",
    "Repository initialized. Now go break something.",
    "gg repo created. It's not a phase, mom, it's version control.",
    "Initialization complete. Let the chaos begin.",
    "Repository created. Time to commit to your commitment issues.",
    "Repo vibing initiated. Your code can now travel through time.",
    "Your repo is ready. We promise this is easier than relationships.",
    "Repository initialized. This is where the fun begins. Or the bugs. Probably bugs."
};

// Creates the .gg directory and initializes all data structures
bool init_repository() {
    // Check if .gg already exists
    if (fs::exists(".gg")) {
        std::cerr << "Error: Repository already exists in this directory." << std::endl;
        return false;
    }
    
    // Initialize UI
    init_ui();
    
    // Create the .gg directory and subdirectories
    try {
        fs::create_directory(".gg");
        fs::create_directory(".gg/objects");
        fs::create_directory(".gg/refs");
        fs::create_directory(".gg/refs/heads");
    } catch (const std::exception& e) {
        cleanup_ui();
        std::cerr << "Error creating repository: " << e.what() << std::endl;
        return false;
    }
    
    // Initialize GGRepo structure
    GGRepo repo;
    
    // Create initial commit (root)
    Commit initial_commit;
    initial_commit.id = generate_commit_id();
    initial_commit.message = "Initial commit";
    initial_commit.author = "gg";
    initial_commit.timestamp = get_current_time_string();
    
    // Initialize data structures
    repo.file_trie = std::make_shared<Trie>();
    repo.commit_graph = std::make_shared<DAG>();
    repo.branches = std::make_shared<AVLTree>();
    repo.commit_list = std::make_shared<SkipList>();
    repo.merge_sets = std::make_shared<DisjointSet>();
    repo.file_filter = std::make_shared<BloomFilter>();
    
    // Add initial commit to DAG
    repo.commit_graph->add_commit(initial_commit, {});
    repo.head_commit = initial_commit.id;
    
    // Add 'main' branch pointing to initial commit
    BranchInfo main_branch("main", initial_commit.id, initial_commit.timestamp);
    repo.branches->insert(main_branch);
    repo.current_branch = "main";
    
    // Add commit to skip list for traversal
    repo.commit_list->insert(initial_commit.id, initial_commit.timestamp);
    
    // Save repository state
    repo.save();
    
    // Create a visualization of the newly initialized repository
    clear();
    
    // Display header
    display_header("Repository Initialized");
    
    // Draw initial data structures
    int row = 5;
    int col = 2;
    
    // Draw DAG
    WINDOW* dag_win = newwin(8, 40, row, col);
    box(dag_win, 0, 0);
    mvwprintw(dag_win, 0, 2, " Commit Graph (DAG) ");
    repo.commit_graph->draw(dag_win, repo.head_commit);
    wrefresh(dag_win);
    
    row += 9;
    
    // Draw Branch AVL Tree
    WINDOW* avl_win = newwin(8, 40, row, col);
    box(avl_win, 0, 0);
    mvwprintw(avl_win, 0, 2, " Branches (AVL Tree) ");
    repo.branches->draw(avl_win, 1, 2, repo.current_branch);
    wrefresh(avl_win);
    
    col += 42;
    row = 5;
    
    // Draw empty trie
    WINDOW* trie_win = newwin(8, 40, row, col);
    box(trie_win, 0, 0);
    mvwprintw(trie_win, 0, 2, " File Structure (Trie) ");
    repo.file_trie->draw(trie_win, 1, 2);
    wrefresh(trie_win);
    
    row += 9;
    
    // Draw empty bloom filter
    WINDOW* bloom_win = newwin(8, 40, row, col);
    box(bloom_win, 0, 0);
    mvwprintw(bloom_win, 0, 2, " File Existence (Bloom Filter) ");
    repo.file_filter->draw(bloom_win, 1, 2, 36, 6);
    wrefresh(bloom_win);
    
    // Display success message and a quip
    attron(COLOR_PAIR(COLOR_SUCCESS));
    mvprintw(24, 2, "[gg] Repository initialized.");
    attroff(COLOR_PAIR(COLOR_SUCCESS));
    
    // Display a random quip
    mvprintw(25, 2, "%s", QuipGenerator::get_random_quip(QuipGenerator::bruh_quips).c_str());
    
    // Refresh and wait for user input
    refresh();
    getch();
    
    // Clean up UI
    delwin(dag_win);
    delwin(avl_win);
    delwin(trie_win);
    delwin(bloom_win);
    cleanup_ui();
    
    return true;
}

// Command handler for 'gg bruh'
bool init_command(int argc, char* argv[]) {
    return init_repository();
}

// Generate a random commit ID (for fun, similar to git's hashes)
std::string generate_commit_id() {
    static const char hex_chars[] = "0123456789abcdef";
    static std::mt19937 rng{std::random_device{}()};
    static std::uniform_int_distribution<size_t> dist{0, 15};
    
    std::string id;
    id.reserve(8);
    
    for (int i = 0; i < 8; ++i) {
        id += hex_chars[dist(rng)];
    }
    
    return id;
}

// Get current time as formatted string
std::string get_current_time_string() {
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);
    return std::string(buffer);
}

// Initialize ncurses UI
void init_ui() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    
    // Initialize colors if terminal supports them
    if (has_colors()) {
        start_color();
        init_pair(COLOR_DEFAULT, COLOR_WHITE, COLOR_BLACK);
        init_pair(COLOR_HEADER, COLOR_BLACK, COLOR_WHITE);
        init_pair(COLOR_SUCCESS, COLOR_GREEN, COLOR_BLACK);
        init_pair(COLOR_ERROR, COLOR_RED, COLOR_BLACK);
        init_pair(COLOR_STAGED, COLOR_YELLOW, COLOR_BLACK);
        init_pair(COLOR_MODIFIED, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(COLOR_UNTRACKED, COLOR_CYAN, COLOR_BLACK);
        init_pair(COLOR_BRANCH, COLOR_BLUE, COLOR_BLACK);
        init_pair(COLOR_COMMIT, COLOR_GREEN, COLOR_BLACK);
        init_pair(COLOR_HIGHLIGHT, COLOR_BLACK, COLOR_CYAN);
    }
}

// Clean up ncurses UI
void cleanup_ui() {
    endwin();
}

// Display a header bar
void display_header(const std::string& title) {
    attron(COLOR_PAIR(COLOR_HEADER));
    mvhline(0, 0, ' ', COLS);
    mvprintw(0, (COLS - title.length()) / 2, "%s", title.c_str());
    attroff(COLOR_PAIR(COLOR_HEADER));
    refresh();
}

// Display a success message
void display_success(const std::string& message) {
    attron(COLOR_PAIR(COLOR_SUCCESS));
    mvprintw(LINES - 2, 2, "%s", message.c_str());
    attroff(COLOR_PAIR(COLOR_SUCCESS));
    refresh();
}

// Display an error message
void display_error(const std::string& message) {
    attron(COLOR_PAIR(COLOR_ERROR));
    mvprintw(LINES - 2, 2, "%s", message.c_str());
    attroff(COLOR_PAIR(COLOR_ERROR));
    refresh();
}

// Display a quip
void display_quip(const std::vector<std::string>& quips) {
    mvprintw(LINES - 1, 2, "%s", QuipGenerator::get_random_quip(quips).c_str());
    refresh();
}

// Get a random quip from a category
std::string QuipGenerator::get_random_quip(const std::vector<std::string>& quips) {
    if (quips.empty()) {
        return "No quips available. Even the joke generator is on strike.";
    }
    
    static std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<size_t> dist{0, quips.size() - 1};
    return quips[dist(rng)];
}

// Implementation of GGRepo methods
bool GGRepo::exists() const {
    return fs::exists(".gg");
}

GGRepo GGRepo::load() {
    // Placeholder for loading repository state from disk
    GGRepo repo;
    // This would deserialize all data structures from .gg directory
    return repo;
}

void GGRepo::save() const {
    // Placeholder for saving repository state to disk
    // This would serialize all data structures to .gg directory
}
