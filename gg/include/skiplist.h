/**
 * skiplist.h - Skip List for efficient commit traversal
 * 
 * The Skip List provides O(log n) search complexity for commit history
 * traversal, with a simpler implementation than balanced trees.
 */

#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <string>
#include <vector>
#include <memory>
#include <random>
#include <ncurses.h>

// Maximum level for the skip list
#define MAX_LEVEL 16

// Skip List node for commit references
class SkipNode {
public:
    std::string commit_id;
    std::string timestamp;
    std::vector<std::shared_ptr<SkipNode>> forward;
    
    SkipNode(const std::string& commit_id, const std::string& timestamp, int level);
    SkipNode(); // For sentinel node
};

// Skip List for efficient commit traversal
class SkipList {
private:
    std::shared_ptr<SkipNode> header;
    int level;
    std::mt19937 rng;
    std::uniform_real_distribution<double> dist;
    
    // Helper methods
    int random_level();
    void draw_node(WINDOW* win, std::shared_ptr<SkipNode> node, int y, int x,
                  std::vector<std::shared_ptr<SkipNode>>& level_nodes, int current_level) const;

public:
    SkipList();
    
    // Core operations
    bool insert(const std::string& commit_id, const std::string& timestamp);
    bool search(const std::string& commit_id) const;
    bool remove(const std::string& commit_id);
    
    // Traversal operations
    std::vector<std::string> get_all_commits() const;
    std::string get_latest_commit() const;
    std::string get_next_commit(const std::string& commit_id) const;
    std::string get_prev_commit(const std::string& commit_id) const;
    
    // Visualization
    void draw(WINDOW* win, int start_y, int start_x) const;
    
    // Serialization
    std::string serialize() const;
    static SkipList deserialize(const std::string& data);
};

#endif // SKIPLIST_H
