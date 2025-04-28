/**
 * disjoint.h - Disjoint Set (Union-Find) for merge conflict resolution
 * 
 * The Disjoint Set data structure is used for efficiently tracking
 * connected components during branch merges to resolve conflicts.
 */

#ifndef DISJOINT_H
#define DISJOINT_H

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <ncurses.h>

// Disjoint Set node
class DisjointNode {
public:
    std::string key;
    std::shared_ptr<DisjointNode> parent;
    int rank;
    
    DisjointNode(const std::string& key);
};

// Disjoint Set (Union-Find) for merge conflict resolution
class DisjointSet {
private:
    std::unordered_map<std::string, std::shared_ptr<DisjointNode>> nodes;
    
    // Helper methods
    std::shared_ptr<DisjointNode> find_set_node(std::shared_ptr<DisjointNode> node);
    void draw_node(WINDOW* win, std::shared_ptr<DisjointNode> node, int y, int x,
                  std::unordered_map<std::string, std::pair<int, int>>& positions) const;

public:
    DisjointSet();
    
    // Core operations
    void make_set(const std::string& key);
    std::string find_set(const std::string& key);
    bool union_sets(const std::string& key1, const std::string& key2);
    
    // Connected component operations
    bool are_connected(const std::string& key1, const std::string& key2);
    std::vector<std::vector<std::string>> get_connected_components() const;
    
    // For merge conflict resolution
    struct MergeConflict {
        std::string file_path;
        std::string base_content;
        std::string ours_content;
        std::string theirs_content;
        bool resolved;
        std::string resolution;
    };
    
    std::vector<MergeConflict> detect_conflicts(const std::string& branch1, const std::string& branch2);
    bool resolve_conflict(const std::string& file_path, const std::string& resolution);
    
    // Visualization
    void draw(WINDOW* win, int start_y, int start_x) const;
    
    // Serialization
    std::string serialize() const;
    static DisjointSet deserialize(const std::string& data);
};

#endif // DISJOINT_H
