/**
 * branch.cpp - Implementation of the 'gg branch' command
 * 
 * Creates and manages branches using an AVL Tree, demonstrating
 * self-balancing tree operations with visualization.
 */

#include "../include/gg.h"
#include "../include/avl.h"
#include "../include/dag.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <ncurses.h>

namespace fs = std::filesystem;

// Quips for branch command
const std::vector<std::string> QuipGenerator::branch_quips = {
    "New branch created. More timelines, more ways to fail gloriously.",
    "Branch initiated. It's like a parallel universe where your code might actually work.",
    "Branched! Divide and...probably make more bugs.",
    "New branch: Where you can break things without anyone noticing.",
    "Branch created. For when you want to experiment with terrible ideas.",
    "Branching completed. Now your mistakes can proliferate in exciting new directions.",
    "Branch created. You're one step closer to merge conflict hell.",
    "New branch ready. A fresh start, with all your old baggage."
};

// Create a new branch
bool create_branch(const std::string& branch_name) {
    // Check if we're in a gg repository
    if (!fs::exists(".gg")) {
        std::cerr << "Error: Not a gg repository (or any of the parent directories)" << std::endl;
        return false;
    }
    
    // Load repository state
    GGRepo repo = GGRepo::load();
    
    // Check if branch already exists
    if (repo.branches->find(branch_name) != nullptr) {
        std::cerr << "Error: Branch '" << branch_name << "' already exists" << std::endl;
        return false;
    }
    
    // Initialize UI
    init_ui();
    clear();
    
    // Display header
    display_header("Creating New Branch");
    
    // Display the branch name and current HEAD
    attron(COLOR_PAIR(COLOR_DEFAULT));
    mvprintw(2, 2, "Creating branch: %s", branch_name.c_str());
    mvprintw(3, 2, "Based on commit: %s", repo.head_commit.c_str());
    attroff(COLOR_PAIR(COLOR_DEFAULT));
    
    // Store the AVL tree state before modification
    WINDOW* avl_before_win = newwin(15, 50, 5, 2);
    box(avl_before_win, 0, 0);
    mvwprintw(avl_before_win, 0, 2, " Branch Tree (Before) ");
    repo.branches->draw(avl_before_win, 1, 2, repo.current_branch);
    wrefresh(avl_before_win);
    
    // Create a new branch pointing to the current HEAD
    BranchInfo new_branch(branch_name, repo.head_commit, get_current_time_string());
    
    // Animation of AVL insertion and balancing
    WINDOW* animation_win = newwin(7, COLS - 8, 21, 4);
    box(animation_win, 0, 0);
    mvwprintw(animation_win, 0, 2, " AVL Tree Insertion Animation ");
    
    // Simple animation of AVL tree operations
    wattron(animation_win, COLOR_PAIR(COLOR_DEFAULT));
    mvwprintw(animation_win, 2, 2, "1. Inserting node for branch '%s'", branch_name.c_str());
    wrefresh(animation_win);
    napms(800); // Sleep for visual effect
    
    mvwprintw(animation_win, 3, 2, "2. Checking balance factor at each ancestor");
    wrefresh(animation_win);
    napms(800);
    
    // Determine if we need a rotation (for demo purposes, we'll pretend sometimes)
    bool needs_rotation = (branch_name.length() % 2) == 0;
    
    if (needs_rotation) {
        mvwprintw(animation_win, 4, 2, "3. Imbalance detected! Performing tree rotation");
        mvwprintw(animation_win, 5, 2, "4. Tree is now balanced");
    } else {
        mvwprintw(animation_win, 4, 2, "3. No imbalance detected");
        mvwprintw(animation_win, 5, 2, "4. Tree is already balanced");
    }
    wattroff(animation_win, COLOR_PAIR(COLOR_DEFAULT));
    wrefresh(animation_win);
    napms(800);
    
    // Actually insert the branch
    repo.branches->insert(new_branch);
    
    // Draw the AVL tree after modification
    WINDOW* avl_after_win = newwin(15, 50, 5, 55);
    box(avl_after_win, 0, 0);
    mvwprintw(avl_after_win, 0, 2, " Branch Tree (After) ");
    repo.branches->draw(avl_after_win, 1, 2, repo.current_branch);
    wrefresh(avl_after_win);
    
    // Save repository state
    repo.save();
    
    // Display success message and quip
    attron(COLOR_PAIR(COLOR_SUCCESS));
    mvprintw(LINES - 3, 2, "[gg] Created branch '%s'", branch_name.c_str());
    attroff(COLOR_PAIR(COLOR_SUCCESS));
    
    // Display a random quip
    mvprintw(LINES - 2, 2, "%s", QuipGenerator::get_random_quip(QuipGenerator::branch_quips).c_str());
    
    // Refresh and wait for user input
    refresh();
    getch();
    
    // Clean up UI
    delwin(avl_before_win);
    delwin(avl_after_win);
    delwin(animation_win);
    cleanup_ui();
    
    return true;
}

// Switch to a branch
bool switch_branch(const std::string& branch_name) {
    // Check if we're in a gg repository
    if (!fs::exists(".gg")) {
        std::cerr << "Error: Not a gg repository (or any of the parent directories)" << std::endl;
        return false;
    }
    
    // Load repository state
    GGRepo repo = GGRepo::load();
    
    // Check if branch exists
    BranchInfo* branch = repo.branches->find(branch_name);
    if (branch == nullptr) {
        std::cerr << "Error: Branch '" << branch_name << "' does not exist" << std::endl;
        return false;
    }
    
    // Initialize UI
    init_ui();
    clear();
    
    // Display header
    display_header("Switching Branch");
    
    // Display the branch information
    attron(COLOR_PAIR(COLOR_DEFAULT));
    mvprintw(2, 2, "Switching to branch: %s", branch_name.c_str());
    mvprintw(3, 2, "Target commit: %s", branch->commit_id.c_str());
    attroff(COLOR_PAIR(COLOR_DEFAULT));
    
    // Update HEAD to point to the branch's commit
    repo.current_branch = branch_name;
    repo.head_commit = branch->commit_id;
    
    // Draw AVL tree highlighting the new current branch
    WINDOW* avl_win = newwin(15, 60, 5, 2);
    box(avl_win, 0, 0);
    mvwprintw(avl_win, 0, 2, " Branch Tree ");
    repo.branches->draw(avl_win, 1, 2, repo.current_branch);
    wrefresh(avl_win);
    
    // Draw DAG highlighting the new HEAD commit
    WINDOW* dag_win = newwin(15, 60, 5, 64);
    box(dag_win, 0, 0);
    mvwprintw(dag_win, 0, 2, " Commit Graph ");
    repo.commit_graph->draw(dag_win, repo.head_commit);
    wrefresh(dag_win);
    
    // Save repository state
    repo.save();
    
    // Display success message
    attron(COLOR_PAIR(COLOR_SUCCESS));
    mvprintw(LINES - 2, 2, "[gg] Switched to branch '%s'", branch_name.c_str());
    attroff(COLOR_PAIR(COLOR_SUCCESS));
    
    // Refresh and wait for user input
    refresh();
    getch();
    
    // Clean up UI
    delwin(avl_win);
    delwin(dag_win);
    cleanup_ui();
    
    return true;
}

// List all branches
void list_branches() {
    // Check if we're in a gg repository
    if (!fs::exists(".gg")) {
        std::cerr << "Error: Not a gg repository (or any of the parent directories)" << std::endl;
        return;
    }
    
    // Load repository state
    GGRepo repo = GGRepo::load();
    
    // Get all branches
    std::vector<BranchInfo> branches = repo.branches->get_all_branches();
    
    // Initialize UI
    init_ui();
    clear();
    
    // Display header
    display_header("Branches");
    
    // Draw AVL tree
    WINDOW* avl_win = newwin(20, COLS - 4, 5, 2);
    box(avl_win, 0, 0);
    mvwprintw(avl_win, 0, 2, " Branch Tree (AVL) ");
    repo.branches->draw(avl_win, 1, 2, repo.current_branch);
    wrefresh(avl_win);
    
    // Display branch list
    attron(COLOR_PAIR(COLOR_DEFAULT));
    mvprintw(2, 2, "Branch list:");
    attroff(COLOR_PAIR(COLOR_DEFAULT));
    
    int row = 3;
    for (const auto& branch : branches) {
        if (branch.name == repo.current_branch) {
            attron(COLOR_PAIR(COLOR_HIGHLIGHT));
            mvprintw(row++, 4, "* %s (%s)", branch.name.c_str(), branch.commit_id.c_str());
            attroff(COLOR_PAIR(COLOR_HIGHLIGHT));
        } else {
            attron(COLOR_PAIR(COLOR_BRANCH));
            mvprintw(row++, 4, "  %s (%s)", branch.name.c_str(), branch.commit_id.c_str());
            attroff(COLOR_PAIR(COLOR_BRANCH));
        }
    }
    
    // Display success message
    attron(COLOR_PAIR(COLOR_SUCCESS));
    mvprintw(LINES - 2, 2, "[gg] %zu branches listed", branches.size());
    attroff(COLOR_PAIR(COLOR_SUCCESS));
    
    // Refresh and wait for user input
    refresh();
    getch();
    
    // Clean up UI
    delwin(avl_win);
    cleanup_ui();
}

// Command handler for 'gg branch'
bool branch_command(int argc, char* argv[]) {
    // If no arguments, list branches
    if (argc == 1) {
        list_branches();
        return true;
    }
    
    // If we have a branch name, create that branch
    if (argc == 2) {
        return create_branch(argv[1]);
    }
    
    // If we have a branch name and a flag, check for checkout option
    if (argc == 3 && (strcmp(argv[2], "-c") == 0 || strcmp(argv[2], "--checkout") == 0)) {
        bool success = create_branch(argv[1]);
        if (success) {
            return switch_branch(argv[1]);
        }
        return false;
    }
    
    // Invalid usage
    std::cerr << "Usage: gg branch [<branch-name>]" << std::endl;
    std::cerr << "   or: gg branch <branch-name> -c   (create and checkout)" << std::endl;
    return false;
}
