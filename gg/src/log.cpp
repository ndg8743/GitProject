/**
 * log.cpp - Implementation of the 'gg log' command
 * 
 * Displays the commit history by traversing the DAG using BFS,
 * with Skip List for efficient navigation.
 */

#include "../include/gg.h"
#include "../include/dag.h"
#include "../include/skiplist.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <queue>
#include <ncurses.h>

namespace fs = std::filesystem;

// Quips for log command
const std::vector<std::string> QuipGenerator::log_quips = {
    "From small beginnings come large bugs.",
    "Behold, the archaeology of your mistakes!",
    "Every commit tells a story. Usually a horror story.",
    "History doesn't repeat, but sometimes your bugs do.",
    "Scrolling through the past. Try not to cringe too hard.",
    "Your commit history: A timeline of questionable decisions.",
    "This is your code's family tree. It's got some interesting branches.",
    "Ah, the paper trail of panic and confusion. Beautiful, isn't it?"
};

// Show commit history
void show_log() {
    // Check if we're in a gg repository
    if (!fs::exists(".gg")) {
        std::cerr << "Error: Not a gg repository (or any of the parent directories)" << std::endl;
        return;
    }
    
    // Load repository state
    GGRepo repo = GGRepo::load();
    
    // Initialize UI
    init_ui();
    clear();
    
    // Display header
    display_header("Commit History");
    
    // Get commit history using BFS
    std::vector<std::shared_ptr<DAGNode>> commits = repo.commit_graph->breadth_first_search();
    
    // Prepare for scrollable window
    int max_row = 5;
    int max_col = 2;
    int win_height = LINES - 8;
    int win_width = COLS - 4;
    
    // Create window for scrollable commit history
    WINDOW* pad = newpad(commits.size() * 4 + 2, win_width);
    
    // Draw DAG
    WINDOW* dag_win = newwin(LINES - max_row - 3, win_width, max_row, max_col);
    box(dag_win, 0, 0);
    mvwprintw(dag_win, 0, 2, " Commit Graph (DAG) ");
    repo.commit_graph->draw(dag_win, repo.head_commit);
    wrefresh(dag_win);
    
    // Move to next row for log
    max_row += LINES - max_row - 2;
    
    // Display branch and commit info
    attron(COLOR_PAIR(COLOR_BRANCH));
    mvprintw(2, 2, "On branch: %s", repo.current_branch.c_str());
    attroff(COLOR_PAIR(COLOR_BRANCH));
    
    attron(COLOR_PAIR(COLOR_DEFAULT));
    mvprintw(3, 2, "Commit history (newest first):");
    attroff(COLOR_PAIR(COLOR_DEFAULT));
    
    // Draw commit history in the pad
    int pad_row = 0;
    for (const auto& commit_node : commits) {
        const Commit& commit = commit_node->commit;
        
        // Check if this is the current HEAD
        bool is_head = (commit.id == repo.head_commit);
        
        // Find which branches point to this commit
        std::vector<std::string> branches_on_commit;
        std::vector<BranchInfo> all_branches = repo.branches->get_all_branches();
        for (const auto& branch : all_branches) {
            if (branch.commit_id == commit.id) {
                branches_on_commit.push_back(branch.name);
            }
        }
        
        // Draw commit info with appropriate color
        if (is_head) {
            wattron(pad, COLOR_PAIR(COLOR_HIGHLIGHT));
        } else {
            wattron(pad, COLOR_PAIR(COLOR_COMMIT));
        }
        
        // Display commit ID and message
        mvwprintw(pad, pad_row, 0, "* %s - \"%s\"", 
                 commit.id.c_str(), commit.message.c_str());
        
        // Display branches pointing to this commit
        if (!branches_on_commit.empty()) {
            std::string branch_str = " (";
            if (is_head) {
                branch_str += "HEAD -> ";
            }
            
            for (size_t i = 0; i < branches_on_commit.size(); ++i) {
                branch_str += branches_on_commit[i];
                if (i < branches_on_commit.size() - 1) {
                    branch_str += ", ";
                }
            }
            branch_str += ")";
            
            mvwprintw(pad, pad_row, commit.id.length() + commit.message.length() + 6, "%s", 
                     branch_str.c_str());
        } else if (is_head) {
            mvwprintw(pad, pad_row, commit.id.length() + commit.message.length() + 6, " (HEAD)");
        }
        
        if (is_head) {
            wattroff(pad, COLOR_PAIR(COLOR_HIGHLIGHT));
        } else {
            wattroff(pad, COLOR_PAIR(COLOR_COMMIT));
        }
        
        // Display commit details
        wattron(pad, COLOR_PAIR(COLOR_DEFAULT));
        mvwprintw(pad, pad_row + 1, 4, "Author: %s", commit.author.c_str());
        mvwprintw(pad, pad_row + 2, 4, "Date: %s", commit.timestamp.c_str());
        
        // Display parent commit IDs
        if (!commit.parent_ids.empty()) {
            std::string parents = "Parents: ";
            for (size_t i = 0; i < commit.parent_ids.size(); ++i) {
                parents += commit.parent_ids[i];
                if (i < commit.parent_ids.size() - 1) {
                    parents += ", ";
                }
            }
            mvwprintw(pad, pad_row + 3, 4, "%s", parents.c_str());
        }
        
        // Draw connection line to parent
        if (pad_row + 4 < static_cast<int>(commits.size() * 4)) {
            mvwprintw(pad, pad_row + 3, 0, " |");
        }
        
        wattroff(pad, COLOR_PAIR(COLOR_DEFAULT));
        
        pad_row += 4;
    }
    
    // Display success message and quip
    attron(COLOR_PAIR(COLOR_SUCCESS));
    mvprintw(LINES - 2, 2, "[gg] Showing commit history.");
    attroff(COLOR_PAIR(COLOR_SUCCESS));
    
    // Display a random quip
    mvprintw(LINES - 1, 2, "%s", QuipGenerator::get_random_quip(QuipGenerator::log_quips).c_str());
    
    // Display scrolling instructions
    attron(COLOR_PAIR(COLOR_DEFAULT));
    mvprintw(max_row, 2, "Use UP/DOWN arrows to scroll, 'q' to quit");
    attroff(COLOR_PAIR(COLOR_DEFAULT));
    
    // Set up box for pad area
    attron(COLOR_PAIR(COLOR_DEFAULT));
    mvhline(max_row + 1, max_col, ACS_HLINE, win_width);
    mvhline(LINES - 3, max_col, ACS_HLINE, win_width);
    mvvline(max_row + 1, max_col, ACS_VLINE, LINES - max_row - 3);
    mvvline(max_row + 1, max_col + win_width - 1, ACS_VLINE, LINES - max_row - 3);
    mvaddch(max_row + 1, max_col, ACS_ULCORNER);
    mvaddch(max_row + 1, max_col + win_width - 1, ACS_URCORNER);
    mvaddch(LINES - 3, max_col, ACS_LLCORNER);
    mvaddch(LINES - 3, max_col + win_width - 1, ACS_LRCORNER);
    attroff(COLOR_PAIR(COLOR_DEFAULT));
    
    // Set up for pad scrolling
    int pad_pos = 0;
    int pad_height = LINES - max_row - 4;
    int pad_max = pad_row > pad_height ? pad_row - pad_height : 0;
    
    // Display the pad
    prefresh(pad, pad_pos, 0, max_row + 2, max_col + 1, LINES - 4, max_col + win_width - 2);
    
    // Handle scrolling
    int ch;
    bool quit = false;
    
    while (!quit) {
        ch = getch();
        
        switch (ch) {
            case KEY_UP:
                if (pad_pos > 0) {
                    pad_pos--;
                    prefresh(pad, pad_pos, 0, max_row + 2, max_col + 1, LINES - 4, max_col + win_width - 2);
                }
                break;
                
            case KEY_DOWN:
                if (pad_pos < pad_max) {
                    pad_pos++;
                    prefresh(pad, pad_pos, 0, max_row + 2, max_col + 1, LINES - 4, max_col + win_width - 2);
                }
                break;
                
            case 'q':
            case 'Q':
                quit = true;
                break;
        }
    }
    
    // Clean up UI
    delwin(pad);
    delwin(dag_win);
    cleanup_ui();
}

// Command handler for 'gg log'
bool log_command(int argc, char* argv[]) {
    show_log();
    return true;
}
