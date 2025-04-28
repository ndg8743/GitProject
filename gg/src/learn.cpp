/**
 * learn.cpp - Implementation of the 'gg learn' command
 * 
 * Provides interactive tutorials with ASCII art visualizations
 * for each of the data structures used in the gg version control system.
 */

#include "../include/gg.h"
#include "../include/trie.h"
#include "../include/dag.h"
#include "../include/avl.h"
#include "../include/skiplist.h"
#include "../include/disjoint.h"
#include "../include/bloom.h"
#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <vector>
#include <ncurses.h>

// Quips for learn command
const std::vector<std::string> QuipGenerator::learn_quips = {
    "No cycles allowed. It's not your friend group drama.",
    "Learning complete. You're not Linus Torvalds yet, but you're closer.",
    "Knowledge acquired. Applying it correctly is a different story.",
    "Understanding gained. Now using it wisely... well, good luck with that.",
    "Tutorial finished. You've taken your first step into a larger world.",
    "You've learned something today. Try not to forget it tomorrow.",
    "Education complete. You're now 0.1% smarter than before.",
    "Learning accomplished. Your CS professor would be slightly less disappointed."
};

// Helper for drawing boxes with padding and borders
void draw_box(WINDOW* win, int y, int x, int height, int width, 
              const std::string& title, const std::string& content) {
    // Draw box
    box(win, 0, 0);
    
    // Draw title if provided
    if (!title.empty()) {
        mvwprintw(win, 0, 2, " %s ", title.c_str());
    }
    
    // Display content with word wrapping
    std::istringstream content_stream(content);
    std::string word;
    int current_y = y;
    int current_x = x;
    
    while (content_stream >> word) {
        // Check if word fits on current line
        if (current_x + word.length() >= width - 2) {
            current_y++;
            current_x = x;
            
            // Check if we've gone beyond box height
            if (current_y >= height - 1) {
                break;
            }
        }
        
        mvwprintw(win, current_y, current_x, "%s", word.c_str());
        current_x += word.length() + 1; // +1 for space
    }
    
    wrefresh(win);
}

// Function prototypes for all learn topics
void learn_dag();
void learn_trie();
void learn_avl();
void learn_skiplist();
void learn_disjoint();
void learn_bloom();

// Learn about Disjoint Set
void learn_disjoint() {
    clear();
    display_header("Learn: Disjoint Set (Union-Find)");
    
    // Create main content window
    WINDOW* content_win = newwin(LINES - 7, COLS - 4, 3, 2);
    box(content_win, 0, 0);
    
    // Disjoint Set introduction
    mvwprintw(content_win, 1, 2, "DISJOINT SET (UNION-FIND)");
    mvwprintw(content_win, 2, 2, "========================");
    mvwprintw(content_win, 4, 2, "Disjoint Sets track non-overlapping groups efficiently:");
    mvwprintw(content_win, 5, 2, "* Determine if elements belong to the same group");
    mvwprintw(content_win, 6, 2, "* Merge groups with Union operation");
    mvwprintw(content_win, 7, 2, "* Find which group an element belongs to quickly");
    
    // ASCII art of a Disjoint Set
    mvwprintw(content_win, 10, 5, "A Disjoint Set with three groups:");
    
    // Group 1
    mvwprintw(content_win, 12, 10, "Group 1");
    mvwprintw(content_win, 13, 10, "    A    ");
    mvwprintw(content_win, 14, 10, "   / \\   ");
    mvwprintw(content_win, 15, 10, "  B   C  ");
    
    // Group 2
    mvwprintw(content_win, 12, 30, "Group 2");
    mvwprintw(content_win, 13, 30, "    D    ");
    mvwprintw(content_win, 14, 30, "   / \\   ");
    mvwprintw(content_win, 15, 30, "  E   F  ");
    
    // Group 3
    mvwprintw(content_win, 12, 50, "Group 3");
    mvwprintw(content_win, 13, 50, "    G    ");
    mvwprintw(content_win, 14, 50, "   /     ");
    mvwprintw(content_win, 15, 50, "  H      ");
    
    // Animation effect for Union operation
    attron(COLOR_PAIR(COLOR_HIGHLIGHT));
    mvprintw(LINES - 3, 2, "Press any key to see Union operation animation...");
    attroff(COLOR_PAIR(COLOR_HIGHLIGHT));
    refresh();
    getch();
    
    // Clear previous animation message
    attron(COLOR_PAIR(COLOR_DEFAULT));
    mvprintw(LINES - 3, 2, "Performing Union(Group 1, Group 2):                      ");
    attroff(COLOR_PAIR(COLOR_DEFAULT));
    
    // Highlight the groups to be merged
    wattron(content_win, COLOR_PAIR(COLOR_HIGHLIGHT));
    mvwprintw(content_win, 12, 10, "Group 1");
    mvwprintw(content_win, 13, 10, "    A    ");
    mvwprintw(content_win, 14, 10, "   / \\   ");
    mvwprintw(content_win, 15, 10, "  B   C  ");
    
    mvwprintw(content_win, 12, 30, "Group 2");
    mvwprintw(content_win, 13, 30, "    D    ");
    mvwprintw(content_win, 14, 30, "   / \\   ");
    mvwprintw(content_win, 15, 30, "  E   F  ");
    wattroff(content_win, COLOR_PAIR(COLOR_HIGHLIGHT));
    wrefresh(content_win);
    napms(1000);
    
    // Draw the merged groups
    wclear(content_win);
    box(content_win, 0, 0);
    
    mvwprintw(content_win, 1, 2, "DISJOINT SET (UNION-FIND)");
    mvwprintw(content_win, 2, 2, "========================");
    mvwprintw(content_win, 4, 2, "After Union, elements from both groups are in one set:");
    
    // Merged Group
    wattron(content_win, COLOR_PAIR(COLOR_HIGHLIGHT));
    mvwprintw(content_win, 10, 20, "Merged Group (Group 1 + Group 2)");
    mvwprintw(content_win, 12, 30, "        A        ");
    mvwprintw(content_win, 13, 30, "       / \\       ");
    mvwprintw(content_win, 14, 30, "      B   C      ");
    mvwprintw(content_win, 15, 30, "          |      ");
    mvwprintw(content_win, 16, 30, "          D      ");
    mvwprintw(content_win, 17, 30, "         / \\     ");
    mvwprintw(content_win, 18, 30, "        E   F    ");
    wattroff(content_win, COLOR_PAIR(COLOR_HIGHLIGHT));
    
    // Group 3 (unchanged)
    mvwprintw(content_win, 12, 50, "Group 3");
    mvwprintw(content_win, 13, 50, "    G    ");
    mvwprintw(content_win, 14, 50, "   /     ");
    mvwprintw(content_win, 15, 50, "  H      ");
    
    wrefresh(content_win);
    
    // Key points about Disjoint Sets
    WINDOW* info_win = newwin(12, 50, LINES - 16, COLS - 54);
    box(info_win, 0, 0);
    mvwprintw(info_win, 0, 2, " Disjoint Set Properties in gg ");
    
    mvwprintw(info_win, 2, 2, "1. Used for merge conflict resolution");
    mvwprintw(info_win, 3, 2, "2. Very fast Union and Find operations");
    mvwprintw(info_win, 4, 2, "3. Uses path compression for efficiency");
    mvwprintw(info_win, 5, 2, "4. Union by rank/size for balanced trees");
    mvwprintw(info_win, 6, 2, "5. Near constant-time operations");
    mvwprintw(info_win, 7, 2, "6. Useful for connected components problems");
    mvwprintw(info_win, 9, 2, "gg uses Disjoint Sets to track related files");
    mvwprintw(info_win, 10, 2, "during merge conflict resolution.");
    
    wrefresh(info_win);
    
    // Display a quip
    attron(COLOR_PAIR(COLOR_DEFAULT));
    mvprintw(LINES - 2, 2, "%s", QuipGenerator::get_random_quip(QuipGenerator::learn_quips).c_str());
    attroff(COLOR_PAIR(COLOR_DEFAULT));
    
    // Wait for user input
    attron(COLOR_PAIR(COLOR_DEFAULT));
    mvprintw(LINES - 3, 2, "Press any key to continue...");
    attroff(COLOR_PAIR(COLOR_DEFAULT));
    refresh();
    getch();
    
    delwin(content_win);
    delwin(info_win);
}

// Learn about Bloom Filter
void learn_bloom() {
    clear();
    display_header("Learn: Bloom Filter");
    
    // Create main content window
    WINDOW* content_win = newwin(LINES - 7, COLS - 4, 3, 2);
    box(content_win, 0, 0);
    
    // Bloom Filter introduction
    mvwprintw(content_win, 1, 2, "BLOOM FILTER");
    mvwprintw(content_win, 2, 2, "============");
    mvwprintw(content_win, 4, 2, "Bloom Filters provide space-efficient probabilistic membership testing:");
    mvwprintw(content_win, 5, 2, "* Fast check if element might exist in a set");
    mvwprintw(content_win, 6, 2, "* No false negatives (if it says not there, it's not there)");
    mvwprintw(content_win, 7, 2, "* Possible false positives (might say exists when it doesn't)");
    mvwprintw(content_win, 8, 2, "* Extremely space efficient");
    
    // ASCII art of a Bloom Filter
    mvwprintw(content_win, 10, 5, "A Bloom Filter with bit array:");
    mvwprintw(content_win, 12, 5, "Hash functions: h1, h2, h3");
    mvwprintw(content_win, 13, 5, "Bit array:      [0][0][1][0][1][0][0][1][0][0][1][0][0][0][0][1]");
    mvwprintw(content_win, 14, 5, "Index:           0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15");
    
    // Animation effect for adding a new element
    attron(COLOR_PAIR(COLOR_HIGHLIGHT));
    mvprintw(LINES - 3, 2, "Press any key to see element addition animation...");
    attroff(COLOR_PAIR(COLOR_HIGHLIGHT));
    refresh();
    getch();
    
    // Clear previous animation message
    attron(COLOR_PAIR(COLOR_DEFAULT));
    mvprintw(LINES - 3, 2, "Adding 'file.txt' to the Bloom Filter:               ");
    attroff(COLOR_PAIR(COLOR_DEFAULT));
    
    // Calculate hash values for the element
    wattron(content_win, COLOR_PAIR(COLOR_HIGHLIGHT));
    mvwprintw(content_win, 16, 5, "h1('file.txt') = 3  -> Set bit 3");
    wrefresh(content_win);
    napms(800);
    
    mvwprintw(content_win, 17, 5, "h2('file.txt') = 9  -> Set bit 9");
    wrefresh(content_win);
    napms(800);
    
    mvwprintw(content_win, 18, 5, "h3('file.txt') = 12 -> Set bit 12");
    wattroff(content_win, COLOR_PAIR(COLOR_HIGHLIGHT));
    wrefresh(content_win);
    napms(800);
    
    // Update the bit array visualization
    wattron(content_win, COLOR_PAIR(COLOR_HIGHLIGHT));
    mvwprintw(content_win, 13, 19, "1"); // Update bit 3
    mvwprintw(content_win, 13, 43, "1"); // Update bit 9
    mvwprintw(content_win, 13, 55, "1"); // Update bit 12
    wattroff(content_win, COLOR_PAIR(COLOR_HIGHLIGHT));
    wrefresh(content_win);
    
    // Key points about Bloom Filters
    WINDOW* info_win = newwin(12, 50, LINES - 16, COLS - 54);
    box(info_win, 0, 0);
    mvwprintw(info_win, 0, 2, " Bloom Filter Properties in gg ");
    
    mvwprintw(info_win, 2, 2, "1. Used for fast file existence checking");
    mvwprintw(info_win, 3, 2, "2. Much more space-efficient than hash tables");
    mvwprintw(info_win, 4, 2, "3. Can't store or retrieve the actual elements");
    mvwprintw(info_win, 5, 2, "4. Multiple hash functions reduce collision chances");
    mvwprintw(info_win, 6, 2, "5. False positive rate can be tuned");
    mvwprintw(info_win, 7, 2, "6. Deletion not possible in basic implementation");
    mvwprintw(info_win, 9, 2, "gg uses Bloom Filters to quickly verify if a file");
    mvwprintw(info_win, 10, 2, "might be tracked by the repository.");
    
    wrefresh(info_win);
    
    // Display a quip
    attron(COLOR_PAIR(COLOR_DEFAULT));
    mvprintw(LINES - 2, 2, "%s", QuipGenerator::get_random_quip(QuipGenerator::learn_quips).c_str());
    attroff(COLOR_PAIR(COLOR_DEFAULT));
    
    // Wait for user input
    attron(COLOR_PAIR(COLOR_DEFAULT));
    mvprintw(LINES - 3, 2, "Press any key to continue...");
    attroff(COLOR_PAIR(COLOR_DEFAULT));
    refresh();
    getch();
    
    delwin(content_win);
    delwin(info_win);
}

// Command handler for 'gg learn'
bool learn_command(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: No topic specified" << std::endl;
        std::cerr << "Usage: gg learn <topic>" << std::endl;
        std::cerr << "Topics: dag, trie, avl, skiplist, disjointset, bloom" << std::endl;
        return false;
    }
    
    std::string topic = argv[1];
    
    // Convert to lowercase for case-insensitive comparison
    std::transform(topic.begin(), topic.end(), topic.begin(), 
                   [](unsigned char c) { return std::tolower(c); });
    
    // Map topic to learning function
    std::map<std::string, std::function<void()>> topic_map = {
        {"dag", learn_dag},
        {"trie", learn_trie},
        {"avl", learn_avl},
        {"skiplist", learn_skiplist},
        {"skiplist", learn_skiplist},
        {"disjoint", learn_disjoint},
        {"disjointset", learn_disjoint},
        {"bloom", learn_bloom}
    };
    
    auto it = topic_map.find(topic);
    if (it == topic_map.end()) {
        std::cerr << "Error: Unknown topic '" << topic << "'" << std::endl;
        std::cerr << "Topics: dag, trie, avl, skiplist, disjointset, bloom" << std::endl;
        return false;
    }
    
    // Call the appropriate learning function
    it->second();
    return true;
}
