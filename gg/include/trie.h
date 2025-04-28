/**
 * trie.h - Trie data structure for file tracking
 * 
 * The Trie is used to efficiently track file paths in the repository.
 * It enables prefix-based searching and visualization of the file structure.
 */

#ifndef TRIE_H
#define TRIE_H

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <ncurses.h>

// File status enum
enum class FileStatus {
    UNTRACKED,
    STAGED,
    COMMITTED,
    MODIFIED
};

// Trie node for file paths
class TrieNode {
public:
    std::map<char, std::shared_ptr<TrieNode>> children;
    bool is_file_end;
    FileStatus status;
    
    TrieNode() : is_file_end(false), status(FileStatus::UNTRACKED) {}
};

// Trie for file tracking
class Trie {
private:
    std::shared_ptr<TrieNode> root;
    
    // Helper functions
    bool insert_recursive(std::shared_ptr<TrieNode> node, const std::string& path, size_t index, FileStatus status);
    bool search_recursive(std::shared_ptr<TrieNode> node, const std::string& path, size_t index) const;
    void get_all_files_recursive(std::shared_ptr<TrieNode> node, std::string current_path, 
                                std::vector<std::pair<std::string, FileStatus>>& files) const;
    void draw_recursive(WINDOW* win, std::shared_ptr<TrieNode> node, std::string prefix, 
                       std::string path, int& y, int x, bool is_last) const;

public:
    Trie();
    
    // Core operations
    bool insert(const std::string& path, FileStatus status);
    bool search(const std::string& path) const;
    bool update_status(const std::string& path, FileStatus status);
    FileStatus get_status(const std::string& path) const;
    std::vector<std::pair<std::string, FileStatus>> get_all_files() const;
    std::vector<std::string> get_files_by_status(FileStatus status) const;
    
    // Visualization
    void draw(WINDOW* win, int start_y, int start_x) const;
    
    // Serialization
    std::string serialize() const;
    static Trie deserialize(const std::string& data);
};

#endif // TRIE_H
