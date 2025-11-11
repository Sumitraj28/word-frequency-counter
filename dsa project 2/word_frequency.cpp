// Word Frequency Counter in C++ (DSA-focused)
// Build: g++ -std=c++17 -O2 -o word_frequency word_frequency.cpp
// Run (file): ./word_frequency path/to/file.txt
// Run (stdin): cat file.txt | ./word_frequency

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using Word = std::string;
using Frequency = int;

// Normalize a token: keep alphanumerics, convert to lowercase.
static std::string normalizeToken(const std::string &token) {
    std::string normalized;
    normalized.reserve(token.size());
    for (unsigned char ch : token) {
        if (std::isalnum(ch)) {
            normalized.push_back(static_cast<char>(std::tolower(ch)));
        } else if (std::isspace(ch)) {
            normalized.push_back(' ');
        }
    }
    return normalized;
}

// Tokenize text into words (alphanumeric sequences), lowercased.
static std::vector<Word> tokenize(const std::string &text) {
    std::vector<Word> words;
    std::string cleaned = text;
    // Replace non-alnum with space and lowercase in-place for efficiency
    for (char &ch : cleaned) {
        unsigned char uch = static_cast<unsigned char>(ch);
        if (std::isalnum(uch)) {
            ch = static_cast<char>(std::tolower(uch));
        } else {
            ch = ' ';
        }
    }

    std::istringstream iss(cleaned);
    std::string word;
    while (iss >> word) {
        words.push_back(word);
    }
    return words;
}

// Count word frequencies using unordered_map (hash table)
static std::unordered_map<Word, Frequency> countFrequencies(const std::vector<Word> &words) {
    std::unordered_map<Word, Frequency> freq;
    freq.reserve(words.size());
    for (const auto &w : words) {
        ++freq[w];
    }
    return freq;
}

// Convert map to vector and sort by frequency desc, then word asc
static std::vector<std::pair<Word, Frequency>> sortByFrequency(const std::unordered_map<Word, Frequency> &freq) {
    std::vector<std::pair<Word, Frequency>> entries;
    entries.reserve(freq.size());
    for (const auto &kv : freq) entries.emplace_back(kv.first, kv.second);
    std::sort(entries.begin(), entries.end(), [](const auto &a, const auto &b) {
        if (a.second != b.second) return a.second > b.second; // higher count first
        return a.first < b.first; // tie-break by word
    });
    return entries;
}

static std::string readAllStdin() {
    std::ostringstream oss;
    oss << std::cin.rdbuf();
    return oss.str();
}

static std::string readAllFile(const std::string &path) {
    std::ifstream fin(path);
    if (!fin) return {};
    std::ostringstream oss;
    oss << fin.rdbuf();
    return oss.str();
}

int main(int argc, char **argv) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string inputText;
    if (argc >= 2) {
        inputText = readAllFile(argv[1]);
        if (inputText.empty()) {
            std::cerr << "Error: Could not read file or file is empty.\n";
            return 1;
        }
    } else {
        // Read from stdin
        inputText = readAllStdin();
        if (inputText.empty()) {
            std::cerr << "Usage: ./word_frequency <file.txt>\n"
                         "       cat file.txt | ./word_frequency\n";
            return 2;
        }
    }

    auto words = tokenize(inputText);
    if (words.empty()) {
        std::cout << "No valid words found.\n";
        return 0;
    }

    auto freq = countFrequencies(words);
    auto sorted = sortByFrequency(freq);

    // Output top N (or all)
    const std::size_t topN = sorted.size(); // change to 20 if only top 20 needed
    std::size_t totalWords = words.size();

    std::cout << "Total Words: " << totalWords << "\n";
    for (std::size_t i = 0; i < topN; ++i) {
        const auto &p = sorted[i];
        std::cout << p.first << ": " << p.second << "\n";
    }

    return 0;
}

