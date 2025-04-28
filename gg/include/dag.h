/**
 * dag.h - Directed Acyclic Graph for commit history
 * 
 * The DAG represents the commit history in gg, allowing for non-linear
 * history tracking and visualization of the project's evolution.
 */

#ifndef DAG_H
#define DAG_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <ncurses.h>

// Commit structure
struct Commit {
    std::string id;
    std::string message;
    std::string author;
    std::string timestamp;
    std::vector<std::string> parent_ids;
    std::map<std::string, std::string> file_snapshots; // filename -> content hash

    Commit() = default;
    Commit(const std::string& id, const std::string& message, 
           const std::string& author, const std::string& timestamp);
};

// Node in the commit graph
class DAGNode {
public:
    Commit commit;
    std::vector<std::shared_ptr<DAGNode>> parents;
    std::vector<std::shared_ptr<DAGNode>> children;
    
    DAGNode(const Commit& commit);
    void add_parent(std::shared_ptr<DAGNode> parent);
    void add_child(std::shared_ptr<DAGNode> child);
};

// Directed Acyclic Graph for commits
class DAG {
private:
    std::map<std::string, std::shared_ptr<DAGNode>> nodes; // commit_id -> node
    std::shared_ptr<DAGNode> head;
    
    // Helper methods for visualization
    void draw_node(WINDOW* win, std::shared_ptr<DAGNode> node, int y, int x, 
                  std::map<std::string, std::pair<int, int>>& positions, 
                  const std::string& current_commit) const;
    void calculate_positions(std::shared_ptr<DAGNode> node, 
                            std::map<std::string, std::pair<int, int>>& positions, 
                            int& max_depth, int depth, int& x_offset) const;

public:
    DAG();
    
    // Core operations
    std::shared_ptr<DAGNode> add_commit(const Commit& commit, 
                                       const std::vector<std::string>& parent_ids);
    std::shared_ptr<DAGNode> get_commit(const std::string& commit_id) const;
    bool set_head(const std::string& commit_id);
    std::shared_ptr<DAGNode> get_head() const;
    
    // Graph traversal
    std::vector<std::shared_ptr<DAGNode>> get_ancestors(const std::string& commit_id) const;
    std::vector<std::shared_ptr<DAGNode>> breadth_first_search() const;
    std::vector<std::shared_ptr<DAGNode>> depth_first_search() const;
    
    // Merging
    std::shared_ptr<DAGNode> merge_branches(const std::string& commit_id, 
                                           const std::string& message);
    
    // Visualization
    void draw(WINDOW* win, const std::string& current_commit) const;
    
    // Serialization
    std::string serialize() const;
    static DAG deserialize(const std::string& data);
};

#endif // DAG_H
