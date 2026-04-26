#pragma once
#include <string>
#include <vector>
#include <memory>

namespace NativeTree {
    enum class Color { RED, BLACK };

    struct TreeNode {
        std::string key;
        int count;
        Color color;
        std::shared_ptr<TreeNode> left;
        std::shared_ptr<TreeNode> right;
        std::shared_ptr<TreeNode> parent;

        TreeNode(const std::string& k, Color c = Color::RED)
            : key(k), count(1), color(c),
            left(nullptr), right(nullptr), parent(nullptr) {
        }
    };

    class RBTree {
    private:
        std::shared_ptr<TreeNode> root;
        std::shared_ptr<TreeNode> nil;

        void leftRotate(std::shared_ptr<TreeNode> x);
        void rightRotate(std::shared_ptr<TreeNode> y);
        void insertFixup(std::shared_ptr<TreeNode> z);
        std::shared_ptr<TreeNode> searchNode(const std::string& key);

        void transplant(std::shared_ptr<TreeNode> u, std::shared_ptr<TreeNode> v);
        void deleteFixup(std::shared_ptr<TreeNode> x);
        std::shared_ptr<TreeNode> minimum(std::shared_ptr<TreeNode> node);


        void inorderTraversal(std::shared_ptr<TreeNode> node,
            std::vector<std::shared_ptr<TreeNode>>& result);
        void getWordsByLength(std::shared_ptr<TreeNode> node,
            std::vector<std::pair<std::string, int>>& result,
            int minLen, int maxLen);

    public:
        RBTree();

        // Основные операции
        void insert(const std::string& key);
        int search(const std::string& key);
        void remove(const std::string& key);
        void clear();

        // Получение данных
        std::vector<std::string> getAllWords();
        std::vector<std::pair<std::string, int>> getTopWords(int n = 5);

        // Массовая вставка слов из вектора (для анализа текста)
        void insertMultiple(const std::vector<std::string>& words);

        // Получение слов по длине (для анализа распределения)
        std::vector<std::pair<std::string, int>> getWordsByLengthRange(int minLen, int maxLen);

        // Получение слов, начинающихся с определенной буквы (для алфавитного указателя)
        std::vector<std::pair<std::string, int>> getWordsStartingWith(char letter);

        // Проверка наличия слов, начинающихся с буквы (для активации кнопок)
        bool hasWordsStartingWith(char letter);

        // Получение средней длины слов
        double getAverageWordLength();

        // Получение самого длинного слова
        std::string getLongestWord();

        // Получение самого короткого слова
        std::string getShortestWord();

        // Получение статистики по длинам слов
        std::vector<std::pair<int, int>> getWordLengthDistribution();

        // Статистика
        int getTotalWords();
        int getUniqueWords();

        // Получение общего количества букв во всех словах
        int getTotalLetters();

        // Получение моды (самой частой длины слова)
        int getModeWordLength();

        // Получение медианной длины слова
        double getMedianWordLength();
    };
}