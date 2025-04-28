#!/bin/bash
# gg - Automated installation script for Linux/macOS
# This script checks for and installs required dependencies before building the project

echo "========================================"
echo "gg - Git Learning CLI Tool Installer"
echo "========================================"
echo ""

# Function to check if a command exists
command_exists() {
  command -v "$1" >/dev/null 2>&1
}

# Check for C++ compiler
echo "Checking for C++ compiler..."
if command_exists g++; then
  echo "✅ C++ compiler found."
else
  echo "❌ C++ compiler not found. Installing..."
  
  if command_exists apt-get; then
    # Debian/Ubuntu
    sudo apt-get update
    sudo apt-get install -y build-essential
  elif command_exists brew; then
    # macOS with Homebrew
    brew install gcc
  elif command_exists dnf; then
    # Fedora
    sudo dnf install -y gcc-c++
  elif command_exists yum; then
    # CentOS
    sudo yum install -y gcc-c++
  elif command_exists pacman; then
    # Arch Linux
    sudo pacman -S --noconfirm gcc
  else
    echo "❌ Could not install C++ compiler. Please install manually."
    exit 1
  fi
  
  echo "✅ C++ compiler installed."
fi

# Check for ncurses
echo "Checking for ncurses library..."
if [ -f "/usr/include/ncurses.h" ] || [ -f "/usr/local/include/ncurses.h" ] || [ -f "/opt/homebrew/include/ncurses.h" ]; then
  echo "✅ ncurses library found."
else
  echo "❌ ncurses library not found. Installing..."
  
  if command_exists apt-get; then
    # Debian/Ubuntu
    sudo apt-get update
    sudo apt-get install -y libncurses5-dev libncursesw5-dev
  elif command_exists brew; then
    # macOS with Homebrew
    brew install ncurses
  elif command_exists dnf; then
    # Fedora
    sudo dnf install -y ncurses-devel
  elif command_exists yum; then
    # CentOS
    sudo yum install -y ncurses-devel
  elif command_exists pacman; then
    # Arch Linux
    sudo pacman -S --noconfirm ncurses
  else
    echo "❌ Could not install ncurses. Please install manually."
    exit 1
  fi
  
  echo "✅ ncurses library installed."
fi

# Create directories if they don't exist
echo "Setting up build directories..."
mkdir -p obj bin

# Build the project
echo "Building gg..."
make clean
make

if [ $? -eq 0 ]; then
  echo ""
  echo "========================================"
  echo "✅ gg has been successfully installed!"
  echo "========================================"
  echo ""
  echo "Now you can run gg using:"
  echo "  ./bin/gg [command] [options]"
  echo ""
  echo "Try some of these commands to get started:"
  echo "  ./bin/gg vibe                    # Initialize a new repository"
  echo "  ./bin/gg add <file>              # Stage a file"
  echo "  ./bin/gg kermit -m \"message\"     # Commit changes"
  echo "  ./bin/gg learn dag               # Learn about DAGs"
  echo ""
  echo "For more information, see the README.md file."
else
  echo "❌ Build failed. Please check the error messages above."
  exit 1
fi
