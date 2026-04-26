#include "RBTree.h"
#include <algorithm>
#include <cctype>
#include <stack>
#include <map>

using namespace NativeTree;

RBTree::RBTree() {
    nil = std::make_shared<TreeNode>("", Color::BLACK);
    root = nil;
}

// Вставка
void RBTree::insert(const std::string& key) {
    if (key.empty()) return;

    std::string lowerKey = key;
    std::transform(lowerKey.begin(), lowerKey.end(), lowerKey.begin(), [](unsigned char c) { return std::tolower(c); });

    auto y = nil;
    auto x = root;

    while (x != nil) {
        y = x;
        if (lowerKey == x->key) {
            x->count++;
            return;
        }
        x = (lowerKey < x->key) ? x->left : x->right;
    }

    auto z = std::make_shared<TreeNode>(lowerKey);
    z->left = nil;
    z->right = nil;
    z->parent = y;

    if (y == nil) {
        root = z;
    }
    else if (z->key < y->key) {
        y->left = z;
    }
    else {
        y->right = z;
    }

    insertFixup(z);
}

// Балансировка после вставки
void RBTree::insertFixup(std::shared_ptr<TreeNode> z) {
    while (z->parent->color == Color::RED) {
        if (z->parent == z->parent->parent->left) {
            auto y = z->parent->parent->right;
            if (y->color == Color::RED) {
                z->parent->color = Color::BLACK;
                y->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(z);
                }
                z->parent->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                rightRotate(z->parent->parent);
            }
        }
        else {
            auto y = z->parent->parent->left;
            if (y->color == Color::RED) {
                z->parent->color = Color::BLACK;
                y->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->color = Color::BLACK;
                z->parent->parent->color = Color::RED;
                leftRotate(z->parent->parent);
            }
        }
    }
    root->color = Color::BLACK;
}

// Левый поворот
void RBTree::leftRotate(std::shared_ptr<TreeNode> x) {
    auto y = x->right;
    x->right = y->left;

    if (y->left != nil)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == nil)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

// Правый поворот
void RBTree::rightRotate(std::shared_ptr<TreeNode> y) {
    auto x = y->left;
    y->left = x->right;

    if (x->right != nil)
        x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == nil)
        root = x;
    else if (y == y->parent->right)
        y->parent->right = x;
    else
        y->parent->left = x;

    x->right = y;
    y->parent = x;
}

// Поиск узла
std::shared_ptr<TreeNode> RBTree::searchNode(const std::string& key) {
    std::string lowerKey = key;
    std::transform(lowerKey.begin(), lowerKey.end(), lowerKey.begin(), [](unsigned char c) { return std::tolower(c); });

    auto x = root;
    while (x != nil) {
        if (lowerKey == x->key)
            return x;
        x = (lowerKey < x->key) ? x->left : x->right;
    }
    return nil;
}

// Поиск количества
int RBTree::search(const std::string& key) {
    auto node = searchNode(key);
    return (node != nil) ? node->count : 0;
}

// Полное удаление узла
void RBTree::remove(const std::string& key) {
    auto z = searchNode(key);
    if (z == nil) return;

    if (z->count > 1) {
        z->count--;
        return;
    }

    auto y = z;
    auto y_original_color = y->color;
    std::shared_ptr<TreeNode> x;

    if (z->left == nil) {
        x = z->right;
        transplant(z, z->right);
    }
    else if (z->right == nil) {
        x = z->left;
        transplant(z, z->left);
    }
    else {
        y = minimum(z->right);
        y_original_color = y->color;
        x = y->right;

        if (y->parent == z) {
            if (x != nil) x->parent = y;
        }
        else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (y_original_color == Color::BLACK) {
        deleteFixup(x);
    }
}

// Балансировка после удаления
void RBTree::deleteFixup(std::shared_ptr<TreeNode> x) {
    while (x != root && x->color == Color::BLACK) {
        if (x == x->parent->left) {
            auto w = x->parent->right;

            if (w->color == Color::RED) {
                w->color = Color::BLACK;
                x->parent->color = Color::RED;
                leftRotate(x->parent);
                w = x->parent->right;
            }

            if (w->left->color == Color::BLACK && w->right->color == Color::BLACK) {
                w->color = Color::RED;
                x = x->parent;
            }
            else {
                if (w->right->color == Color::BLACK) {
                    w->left->color = Color::BLACK;
                    w->color = Color::RED;
                    rightRotate(w);
                    w = x->parent->right;
                }

                w->color = x->parent->color;
                x->parent->color = Color::BLACK;
                w->right->color = Color::BLACK;
                leftRotate(x->parent);
                x = root;
            }
        }
        else { // Симметричный случай
            auto w = x->parent->left;

            if (w->color == Color::RED) {
                w->color = Color::BLACK;
                x->parent->color = Color::RED;
                rightRotate(x->parent);
                w = x->parent->left;
            }

            if (w->right->color == Color::BLACK && w->left->color == Color::BLACK) {
                w->color = Color::RED;
                x = x->parent;
            }
            else {
                if (w->left->color == Color::BLACK) {
                    w->right->color = Color::BLACK;
                    w->color = Color::RED;
                    leftRotate(w);
                    w = x->parent->left;
                }

                w->color = x->parent->color;
                x->parent->color = Color::BLACK;
                w->left->color = Color::BLACK;
                rightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = Color::BLACK;
}

// Найти минимальный узел в поддереве
std::shared_ptr<TreeNode> RBTree::minimum(std::shared_ptr<TreeNode> node) {
    while (node->left != nil) {
        node = node->left;
    }
    return node;
}

// Заменить поддерево
void RBTree::transplant(std::shared_ptr<TreeNode> u, std::shared_ptr<TreeNode> v) {
    if (u->parent == nil) {
        root = v;
    }
    else if (u == u->parent->left) {
        u->parent->left = v;
    }
    else {
        u->parent->right = v;
    }
    if (v != nil) {
        v->parent = u->parent;
    }
}

// Очистка
void RBTree::clear() {
    root = nil;
}

// Получение всех слов
std::vector<std::string> RBTree::getAllWords() {
    std::vector<std::string> result;
    std::stack<std::shared_ptr<TreeNode>> s;
    auto current = root;

    while (current != nil || !s.empty()) {
        while (current != nil) {
            s.push(current);
            current = current->left;
        }

        current = s.top();
        s.pop();

        result.push_back(current->key);
        current = current->right;
    }

    return result;
}

// Топ слов
std::vector<std::pair<std::string, int>> RBTree::getTopWords(int n) {
    std::vector<std::shared_ptr<TreeNode>> nodes;
    std::stack<std::shared_ptr<TreeNode>> s;
    auto current = root;

    while (current != nil || !s.empty()) {
        while (current != nil) {
            s.push(current);
            current = current->left;
        }

        current = s.top();
        s.pop();

        nodes.push_back(current);
        current = current->right;
    }

    std::sort(nodes.begin(), nodes.end(), [](const std::shared_ptr<TreeNode>& a, const std::shared_ptr<TreeNode>& b) {
            return a->count > b->count;
        });

    std::vector<std::pair<std::string, int>> result;
    int limit = std::min(n, (int)nodes.size());

    for (int i = 0; i < limit; i++) {
        result.push_back({ nodes[i]->key, nodes[i]->count });
    }

    return result;
}

// Общее количество слов
int RBTree::getTotalWords() {
    int total = 0;
    std::stack<std::shared_ptr<TreeNode>> s;
    auto current = root;

    while (current != nil || !s.empty()) {
        while (current != nil) {
            s.push(current);
            current = current->left;
        }

        current = s.top();
        s.pop();

        total += current->count;
        current = current->right;
    }

    return total;
}

// Уникальных слов
int RBTree::getUniqueWords() {
    int unique = 0;
    std::stack<std::shared_ptr<TreeNode>> s;
    auto current = root;

    while (current != nil || !s.empty()) {
        while (current != nil) {
            s.push(current);
            current = current->left;
        }

        current = s.top();
        s.pop();

        unique++;
        current = current->right;
    }

    return unique;
}

void RBTree::inorderTraversal(std::shared_ptr<TreeNode> node, std::vector<std::shared_ptr<TreeNode>>& result) {
    if (node == nil) return;

    inorderTraversal(node->left, result);
    result.push_back(node);
    inorderTraversal(node->right, result);
}

// Вспомогательный метод для получения слов по длине
void RBTree::getWordsByLength(std::shared_ptr<TreeNode> node, std::vector<std::pair<std::string, int>>& result, int minLen, int maxLen) {
    if (node == nil) return;

    getWordsByLength(node->left, result, minLen, maxLen);

    int wordLength = (int)node->key.length();
    if (wordLength >= minLen && wordLength <= maxLen) {
        result.push_back({ node->key, node->count });
    }

    getWordsByLength(node->right, result, minLen, maxLen);
}

// Массовая вставка слов из вектора
void RBTree::insertMultiple(const std::vector<std::string>& words) {
    for (const auto& word : words) {
        if (!word.empty()) {
            insert(word);
        }
    }
}

// Получение слов по диапазону длины
std::vector<std::pair<std::string, int>> RBTree::getWordsByLengthRange(int minLen, int maxLen) {
    std::vector<std::pair<std::string, int>> result;
    getWordsByLength(root, result, minLen, maxLen);
    return result;
}

// Получение слов, начинающихся с определенной буквы
std::vector<std::pair<std::string, int>> RBTree::getWordsStartingWith(char letter) {
    std::vector<std::shared_ptr<TreeNode>> allNodes;
    inorderTraversal(root, allNodes);

    std::vector<std::pair<std::string, int>> result;
    char lowerLetter = std::tolower(static_cast<unsigned char>(letter));

    for (const auto& node : allNodes) {
        if (!node->key.empty() && std::tolower(static_cast<unsigned char>(node->key[0])) == lowerLetter) {
            result.push_back({ node->key, node->count });
        }
    }

    return result;
}

// Проверка наличия слов, начинающихся с буквы
bool RBTree::hasWordsStartingWith(char letter) {
    std::vector<std::shared_ptr<TreeNode>> allNodes;
    inorderTraversal(root, allNodes);

    char lowerLetter = std::tolower(static_cast<unsigned char>(letter));

    for (const auto& node : allNodes) {
        if (!node->key.empty() && std::tolower(static_cast<unsigned char>(node->key[0])) == lowerLetter) {
            return true;
        }
    }

    return false;
}

// Получение средней длины слов
double RBTree::getAverageWordLength() {
    std::vector<std::shared_ptr<TreeNode>> allNodes;
    inorderTraversal(root, allNodes);

    if (allNodes.empty()) return 0.0;

    long long totalLetters = 0;
    int totalWords = 0;

    for (const auto& node : allNodes) {
        totalLetters += node->key.length() * node->count;
        totalWords += node->count;
    }

    return (totalWords > 0) ? static_cast<double>(totalLetters) / totalWords : 0.0;
}

// Получение самого длинного слова
std::string RBTree::getLongestWord() {
    std::vector<std::shared_ptr<TreeNode>> allNodes;
    inorderTraversal(root, allNodes);

    if (allNodes.empty()) return "";

    std::string longest = allNodes[0]->key;
    size_t maxLength = longest.length();

    for (const auto& node : allNodes) {
        if (node->key.length() > maxLength) {
            longest = node->key;
            maxLength = longest.length();
        }
    }

    return longest;
}

// Получение самого короткого слова
std::string RBTree::getShortestWord() {
    std::vector<std::shared_ptr<TreeNode>> allNodes;
    inorderTraversal(root, allNodes);

    if (allNodes.empty()) return "";

    std::string shortest = allNodes[0]->key;
    size_t minLength = shortest.length();

    for (const auto& node : allNodes) {
        if (node->key.length() < minLength) {
            shortest = node->key;
            minLength = shortest.length();
        }
    }

    return shortest;
}

// Получение статистики по длинам слов
std::vector<std::pair<int, int>> RBTree::getWordLengthDistribution() {
    std::vector<std::shared_ptr<TreeNode>> allNodes;
    inorderTraversal(root, allNodes);

    std::map<int, int> lengthCounts;

    for (const auto& node : allNodes) {
        int length = static_cast<int>(node->key.length());
        lengthCounts[length] += node->count;
    }

    std::vector<std::pair<int, int>> result;
    for (const auto& pair : lengthCounts) {
        result.push_back(pair);
    }

    std::sort(result.begin(), result.end(), [](const std::pair<int, int>& a, const std::pair<int, int>& b) {return a.first < b.first; });

    return result;
}

// Получение общего количества букв во всех словах
int RBTree::getTotalLetters() {
    std::vector<std::shared_ptr<TreeNode>> allNodes;
    inorderTraversal(root, allNodes);

    int totalLetters = 0;
    for (const auto& node : allNodes) {
        totalLetters += static_cast<int>(node->key.length()) * node->count;
    }

    return totalLetters;
}

// Получение самой частой длины слова
int RBTree::getModeWordLength() {
    auto distribution = getWordLengthDistribution();

    if (distribution.empty()) return 0;

    int modeLength = distribution[0].first;
    int maxCount = distribution[0].second;

    for (const auto& pair : distribution) {
        if (pair.second > maxCount) {
            maxCount = pair.second;
            modeLength = pair.first;
        }
    }

    return modeLength;
}

// Получение медианной длины слова
double RBTree::getMedianWordLength() {
    std::vector<std::shared_ptr<TreeNode>> allNodes;
    inorderTraversal(root, allNodes);

    if (allNodes.empty()) return 0.0;

    std::vector<int> allLengths;
    for (const auto& node : allNodes) {
        int length = static_cast<int>(node->key.length());
        for (int i = 0; i < node->count; i++) {
            allLengths.push_back(length);
        }
    }

    if (allLengths.empty()) return 0.0;

    std::sort(allLengths.begin(), allLengths.end());

    size_t n = allLengths.size();
    if (n % 2 == 0) {
        return (allLengths[n / 2 - 1] + allLengths[n / 2]) / 2.0;
    }
    else {
        return static_cast<double>(allLengths[n / 2]);
    }
}