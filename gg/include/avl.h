/**
 * avl.h - AVL Tree for branch management
 * 
 * The self-balancing AVL Tree is used to efficiently store, search,
 * and visualize branches in the repository.
 */

#ifndef AVL_H
#define AVL_H

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <ncurses.h>

// Branch info structure
struct BranchInfo {
    std::string name;
    std::string commit_id;
    std::string creation_time;
    std::string latest_commit_time;
    
    BranchInfo() = default;
    BranchInfo(const std::string& name, const std::string& commit_id, 
              const std::string& creation_time);
};

// AVL Tree node
class AVLNode {
public:
    BranchInfo branch;
    int height;
    std::shared_ptr<AVLNode> left;
    std::shared_ptr<AVLNode> right;
    
    AVLNode(const BranchInfo& branch);
    int balance_factor() const;
};

// AVL Tree for branches
class AVLTree {
private:
    std::shared_ptr<AVLNode> root;
    std::map<std::string, std::shared_ptr<AVLNode>> branch_map; // name -> node
    
    // Helper methods
    int height(std::shared_ptr<AVLNode> node) const;
    std::shared_ptr<AVLNode> right_rotate(std::shared_ptr<AVLNode> y);
    std::shared_ptr<AVLNode> left_rotate(std::shared_ptr<AVLNode> x);
    std::shared_ptr<AVLNode> insert_node(std::shared_ptr<AVLNode> node, const BranchInfo& branch);
    std::shared_ptr<AVLNode> delete_node(std::shared_ptr<AVLNode> root, const std::string& branch_name);
    std::shared_ptr<AVLNode> min_value_node(std::shared_ptr<AVLNode> node) const;
    void in_order_traversal(std::shared_ptr<AVLNode> node, std::vector<BranchInfo>& branches) const;
    void draw_node(WINDOW* win, std::shared_ptr<AVLNode> node, int y, int x, 
                  int h_offset, const std::string& current_branch) const;

public:
    AVLTree();
    
    // Core operations
    bool insert(const BranchInfo& branch);
    bool remove(const std::string& branch_name);
    BranchInfo* find(const std::string& branch_name) const;
    bool update_commit(const std::string& branch_name, const std::string& commit_id, 
                      const std::string& commit_time);
    
    // Tree traversal
    std::vector<BranchInfo> get_all_branches() const;
    
    // Visualization
    void draw(WINDOW* win, int start_y, int start_x, const std::string& current_branch) const;
    
    // Serialization
    std::string serialize() const;
    static AVLTree deserialize(const std::string& data);
};

#endif // AVL_H
