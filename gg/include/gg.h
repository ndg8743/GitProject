/**
 * gg.h - Global header file for the gg version control system
 * 
 * A command-line based version control system that teaches Git concepts visually
 * and demonstrates advanced data structures.
 */

#ifndef GG_H
#define GG_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <ctime>
#include <random>
#include <sstream>
#include <functional>
#include <algorithm>
#include <filesystem>

// Platform-specific settings
#ifdef _WIN32
#define PATH_SEPARATOR '\\'
#define REPO_DIR ".gg"
#else
#define PATH_SEPARATOR '/'
#define REPO_DIR ".gg"
#endif

// Forward declarations of data structure classes
class Trie;
class DAG;
class AVLTree;
class SkipList;
class DisjointSet;
class BloomFilter;

// Simplified version without ncurses
#ifdef NCURSES_DISABLED
typedef struct _win_st WINDOW;
#else
#include <ncurses.h>
#endif

// Colors for ncurses
enum Color {
    COLOR_DEFAULT = 1,
    COLOR_HEADER,
    COLOR_SUCCESS,
    COLOR_ERROR,
    COLOR_STAGED,
    COLOR_MODIFIED,
    COLOR_UNTRACKED,
    COLOR_BRANCH,
    COLOR_COMMIT,
    COLOR_HIGHLIGHT
};

// Global state of the gg repository
struct GGRepo {
    std::string current_branch;
    std::string head_commit;
    std::shared_ptr<Trie> file_trie;
    std::shared_ptr<DAG> commit_graph;
    std::shared_ptr<AVLTree> branches;
    std::shared_ptr<SkipList> commit_list;
    std::shared_ptr<DisjointSet> merge_sets;
    std::shared_ptr<BloomFilter> file_filter;
    bool exists() const;
    static GGRepo load();
    void save() const;
};

// Repository operations
bool init_repository();
bool add_file(const std::string& filepath);
bool commit_changes(const std::string& message);
bool create_branch(const std::string& name);
bool merge_branch(const std::string& name);
void show_status();
void show_log();
void learn_topic(const std::string& topic);

// UI functions
void init_ui();
void cleanup_ui();
void display_header(const std::string& title);
void display_success(const std::string& message);
void display_error(const std::string& message);
void display_quip(const std::vector<std::string>& quips);
void draw_commit_graph(WINDOW* win, const DAG& dag, const std::string& current_commit);
void draw_file_trie(WINDOW* win, const Trie& trie);
void draw_branch_tree(WINDOW* win, const AVLTree& tree, const std::string& current_branch);
void draw_skip_list(WINDOW* win, const SkipList& list);
void refresh_display();

// Funny quips generator
class QuipGenerator {
public:
    static const std::vector<std::string> bruh_quips;
    static const std::vector<std::string> add_quips;
    static const std::vector<std::string> commit_quips;
    static const std::vector<std::string> status_quips;
    static const std::vector<std::string> log_quips;
    static const std::vector<std::string> branch_quips;
    static const std::vector<std::string> merge_quips;
    static const std::vector<std::string> learn_quips;
    
    static std::string get_random_quip(const std::vector<std::string>& quips);
};

// Utility functions
std::string generate_commit_id();
std::string get_current_time_string();
std::vector<std::string> split_string(const std::string& str, char delimiter);
bool file_exists(const std::string& filepath);
std::string read_file_contents(const std::string& filepath);
void write_file_contents(const std::string& filepath, const std::string& contents);

// Learn function declarations
void learn_dag();
void learn_trie();
void learn_avl();
void learn_skiplist();

#endif // GG_H
