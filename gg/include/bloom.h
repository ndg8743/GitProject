/**
 * bloom.h - Bloom Filter for file existence checking
 * 
 * The Bloom Filter provides a space-efficient probabilistic data structure
 * for quickly checking if a file is being tracked in the repository.
 */

#ifndef BLOOM_H
#define BLOOM_H

#include <string>
#include <vector>
#include <array>
#include <functional>
#include <ncurses.h>

// Bloom Filter for rapid existence checks
class BloomFilter {
private:
    static const size_t DEFAULT_SIZE = 1024;
    static const size_t HASH_FUNCTIONS = 3;
    
    std::vector<bool> bit_array;
    size_t size;
    std::array<std::hash<std::string>, HASH_FUNCTIONS> hash_functions;
    size_t item_count;
    
    // Hash functions
    size_t hash1(const std::string& key) const;
    size_t hash2(const std::string& key) const;
    size_t hash3(const std::string& key) const;
    
    // Helper methods
    void draw_bits(WINDOW* win, int start_y, int start_x, int width, int height) const;

public:
    BloomFilter(size_t size = DEFAULT_SIZE);
    
    // Core operations
    void add(const std::string& item);
    bool might_contain(const std::string& item) const;
    void clear();
    
    // Statistics
    double false_positive_probability() const;
    size_t get_item_count() const;
    size_t get_size() const;
    
    // Visualization
    void draw(WINDOW* win, int start_y, int start_x, int width, int height) const;
    
    // Serialization
    std::string serialize() const;
    static BloomFilter deserialize(const std::string& data);
};

#endif // BLOOM_H
