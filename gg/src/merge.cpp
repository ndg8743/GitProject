/**
 * merge.cpp - Implementation of the 'gg merge' command
 * 
 * Merges one branch into another, using Disjoint Sets for conflict resolution
 * and demonstrating how connected components can track related changes.
 */

#include "../include/gg.h"
#include "../include/dag.h"
#include "../include/disjoint.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <ncurses.h>

namespace fs = std::filesystem;

// Quips for merge command
const std::vector<std::string> QuipGenerator::merge_quips = {
    "Merge completed. No conflicts detected. Either you're lucky or something's missing.",
    "Branches merged. The DAG grows more complex, just like your problems.",
    "Merge successful. Your branches are now one. Unlike your focus.",
    "Branches combined without incident. That's suspiciously convenient.",
    "Merge complete. Somewhere, a CS professor is shedding a tear of joy.",
    "Merged without conflicts. Don't worry, the bugs will reveal themselves later.",
    "Branches successfully united. The code still works. For now.",
    "Merge complete. Remember when you thought version control was boring?"
};

// Simulate finding potential conflicts between branches
std::vector<DisjointSet::MergeConflict> find_conflicts(const GGRepo& repo, 
                                                      const std::string& source_branch) {
    // This is a simplified simulation of conflict detection
    // In a real implementation, we would compare file snapshots
    std::vector<DisjointSet::MergeConflict> conflicts;
    
    // Get branch info
    BranchInfo* source = repo.branches->find(source_branch);
    BranchInfo* target = repo.branches->find(repo.current_branch);
    
    if (!source || !target) {
        return conflicts;
    }
    
    // Get commits for branches
    std::shared_ptr<DAGNode> source_commit = repo.commit_graph->get_commit(source->commit_id);
    std::shared_ptr<DAGNode> target_commit = repo.commit_graph->get_commit(target->commit_id);
    
    if (!source_commit || !target_commit) {
        return conflicts;
    }
    
    // Find common ancestor (in a real implementation, we'd traverse the DAG)
    // Here we'll just pretend the initial commit is the common ancestor
    std::vector<std::shared_ptr<DAGNode>> all_commits = repo.commit_graph->breadth_first_search();
    std::shared_ptr<DAGNode> common_ancestor = all_commits.empty() ? nullptr : all_commits.back();
    
    // Get all files in both branches
    std::vector<std::pair<std::string, FileStatus>> all_files = repo.file_trie->get_all_files();
    
    // For demo purposes, create a simulated conflict on every 3rd file
    for (size_t i = 0; i < all_files.size(); ++i) {
        if (i % 3 == 0 && all_files[i].second == FileStatus::COMMITTED) {
            DisjointSet::MergeConflict conflict;
            conflict.file_path = all_files[i].first;
            conflict.base_content = "Base content"; // Would be from common ancestor
            conflict.ours_content = "Our changes"; // Would be from target branch
            conflict.theirs_content = "Their changes"; // Would be from source branch
            conflict.resolved = false;
            
            conflicts.push_back(conflict);
        }
    }
    
    return conflicts;
}

// Merge one branch into the current branch
bool merge_branch(const std::string& source_branch) {
    // Check if we're in a gg repository
    if (!fs::exists(".gg")) {
        std::cerr << "Error: Not a gg repository (or any of the parent directories)" << std::endl;
        return false;
    }
    
    // Load repository state
    GGRepo repo = GGRepo::load();
    
    // Check if source branch exists
    BranchInfo* source = repo.branches->find(source_branch);
    if (source == nullptr) {
        std::cerr << "Error: Branch '" << source_branch << "' does not exist" << std::endl;
        return false;
    }
    
    // Check that we're not trying to merge a branch into itself
    if (repo.current_branch == source_branch) {
        std::cerr << "Error: Cannot merge a branch into itself" << std::endl;
        return false;
    }
    
    // Find potential merge conflicts
    std::vector<DisjointSet::MergeConflict> conflicts = find_conflicts(repo, source_branch);
    
    // Initialize UI
    init_ui();
    clear();
    
    // Display header
    display_header("Merging Branches");
    
    // Display the branches being merged
    attron(COLOR_PAIR(COLOR_DEFAULT));
    mvprintw(2, 2, "Merging '%s' into '%s'", source_branch.c_str(), repo.current_branch.c_str());
    attroff(COLOR_PAIR(COLOR_DEFAULT));
    
    // Draw DAG before merge
    WINDOW* dag_before_win = newwin(10, 50, 4, 2);
    box(dag_before_win, 0, 0);
    mvwprintw(dag_before_win, 0, 2, " Commit Graph (Before) ");
    repo.commit_graph->draw(dag_before_win, repo.head_commit);
    wrefresh(dag_before_win);
    
    // Draw Disjoint Set before merge
    WINDOW* disjoint_before_win = newwin(10, 50, 15, 2);
    box(disjoint_before_win, 0, 0);
    mvwprintw(disjoint_before_win, 0, 2, " Disjoint Set (Before) ");
    repo.merge_sets->draw(disjoint_before_win, 1, 2);
    wrefresh(disjoint_before_win);
    
    // Handle conflicts if any
    bool has_conflicts = !conflicts.empty();
    
    if (has_conflicts) {
        attron(COLOR_PAIR(COLOR_ERROR));
        mvprintw(4, 55, "Conflicts detected!");
        attroff(COLOR_PAIR(COLOR_ERROR));
        
        // Display conflicts in a window
        WINDOW* conflict_win = newwin(15, 60, 6, 55);
        box(conflict_win, 0, 0);
        mvwprintw(conflict_win, 0, 2, " Merge Conflicts ");
        
        int row = 1;
        for (size_t i = 0; i < conflicts.size() && row < 14; ++i) {
            const auto& conflict = conflicts[i];
            wattron(conflict_win, COLOR_PAIR(COLOR_ERROR));
            mvwprintw(conflict_win, row++, 2, "Conflict in file: %s", conflict.file_path.c_str());
            wattroff(conflict_win, COLOR_PAIR(COLOR_ERROR));
            
            wattron(conflict_win, COLOR_PAIR(COLOR_DEFAULT));
            mvwprintw(conflict_win, row++, 4, "Base: %s", conflict.base_content.c_str());
            mvwprintw(conflict_win, row++, 4, "Ours: %s", conflict.ours_content.c_str());
            mvwprintw(conflict_win, row++, 4, "Theirs: %s", conflict.theirs_content.c_str());
            wattroff(conflict_win, COLOR_PAIR(COLOR_DEFAULT));
            
            row++; // Add a blank line between conflicts
        }
        
        wrefresh(conflict_win);
        
        // In a real implementation, we would prompt for conflict resolution
        // For demo, we'll auto-resolve by taking "our" changes
        napms(1000); // Wait to show conflicts
        
        wattron(conflict_win, COLOR_PAIR(COLOR_SUCCESS));
        mvwprintw(conflict_win, 13, 2, "Auto-resolving conflicts...");
        wattroff(conflict_win, COLOR_PAIR(COLOR_SUCCESS));
        wrefresh(conflict_win);
        
        napms(1000); // Wait to show resolution
        
        delwin(conflict_win);
        
        // Update disjoint set to resolve conflicts
        for (const auto& conflict : conflicts) {
            // Create sets for each file involved
            repo.merge_sets->make_set(conflict.file_path + "_ours");
            repo.merge_sets->make_set(conflict.file_path + "_theirs");
            
            // Union them to mark as resolved
            repo.merge_sets->union_sets(conflict.file_path + "_ours", conflict.file_path + "_theirs");
        }
    } else {
        attron(COLOR_PAIR(COLOR_SUCCESS));
        mvprintw(4, 55, "No conflicts detected!");
        attroff(COLOR_PAIR(COLOR_SUCCESS));
    }
    
    // Create a new merge commit
    Commit merge_commit;
    merge_commit.id = generate_commit_id();
    merge_commit.message = "Merge branch '" + source_branch + "' into " + repo.current_branch;
    merge_commit.author = "gg";
    merge_commit.timestamp = get_current_time_string();
    merge_commit.parent_ids.push_back(repo.head_commit);
    merge_commit.parent_ids.push_back(source->commit_id);
    
    // Update the graph
    repo.commit_graph->add_commit(merge_commit, {repo.head_commit, source->commit_id});
    
    // Update HEAD and branch
    repo.head_commit = merge_commit.id;
    repo.branches->update_commit(repo.current_branch, merge_commit.id, merge_commit.timestamp);
    
    // Update skip list
    repo.commit_list->insert(merge_commit.id, merge_commit.timestamp);
    
    // Draw updated DAG
    WINDOW* dag_after_win = newwin(10, 50, 4, 55);
    box(dag_after_win, 0, 0);
    mvwprintw(dag_after_win, 0, 2, " Commit Graph (After) ");
    repo.commit_graph->draw(dag_after_win, repo.head_commit);
    wrefresh(dag_after_win);
    
    // Draw updated Disjoint Set
    WINDOW* disjoint_after_win = newwin(10, 50, 15, 55);
    box(disjoint_after_win, 0, 0);
    mvwprintw(disjoint_after_win, 0, 2, " Disjoint Set (After) ");
    repo.merge_sets->draw(disjoint_after_win, 1, 2);
    wrefresh(disjoint_after_win);
    
    // Save repository state
    repo.save();
    
    // Display success message and quip
    attron(COLOR_PAIR(COLOR_SUCCESS));
    mvprintw(LINES - 3, 2, "[gg] Merged branch '%s' into '%s'", 
             source_branch.c_str(), repo.current_branch.c_str());
    mvprintw(LINES - 2, 2, "Merge commit: %s", merge_commit.id.c_str());
    attroff(COLOR_PAIR(COLOR_SUCCESS));
    
    // Display a random quip
    mvprintw(LINES - 1, 2, "%s", QuipGenerator::get_random_quip(QuipGenerator::merge_quips).c_str());
    
    // Refresh and wait for user input
    refresh();
    getch();
    
    // Clean up UI
    delwin(dag_before_win);
    delwin(disjoint_before_win);
    delwin(dag_after_win);
    delwin(disjoint_after_win);
    cleanup_ui();
    
    return true;
}

// Command handler for 'gg merge'
bool merge_command(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: No branch specified" << std::endl;
        std::cerr << "Usage: gg merge <branch-name>" << std::endl;
        return false;
    }
    
    return merge_branch(argv[1]);
}
