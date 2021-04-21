//
// Created by Ron on 13-Sep-19.
//

#include <iostream>
#include "HashMap.hpp"
#include <fstream>
#include <sstream>

/**
 * Checks if a string is a valid positive integer
 * @param string string to validate
 * @return integer representation of string if valid; -1 otherwise;
 */
int checkNumber(std::string const &str)
{
    if (str[0] == '0' && str.length() != 1)
    {
        return -1;
    }
    int res = 0;
    for (char c: str)
    {
        if (c < '0' || c > '9')
        {
            return -1;
        }
        res *= 10;
        int num = c - '0';
        res += num;
    }
    return res;
}

/**
 * Validates the number of columns in line.
 * @param line line in database
 * @return number of columns in line.
 */
int validateColumn(const std::string &line)
{
    int delimiterCounter = 0;
    for (char c: line)
    {
        if (c == ',')
        {
            ++delimiterCounter;
        }
    }
    return delimiterCounter;
}

/**
 * Parses the database file.
 * @param database database file
 * @param phrases vector of phrases, string
 * @param scores vector of scores, int
 */
void
parseDatabase(std::ifstream &database, std::vector<std::string> &phrases, std::vector<int> &scores)
{
    std::string line;
    while (getline(database, line))
    {
        std::string phrase;
        int score = 0;
        std::istringstream iss(line);
        int delimiterCounter = validateColumn(line);
        if (delimiterCounter != 1)
        {
            throw std::invalid_argument("Invalid input");
        }
        int column = 0;
        while (getline(iss, line, ','))
        {
            ++column;
            if (column == 1)
            {
                phrase = line;
                if (phrase.empty())
                {
                    throw std::invalid_argument("Invalid input");
                }
                continue;
            }
            if (column == 2)
            {
                score = checkNumber(line);
                if (score < 0)
                {
                    throw std::invalid_argument("Invalid input");
                }
            }
        }
        phrases.push_back(phrase);
        scores.push_back(score);
    }
}

/**
 * Parses the message file.
 * @param message message file
 * @param map HashMap of pairs
 * @return total score of message
 */
int parseMessage(std::ifstream &message, HashMap<std::string, int> &map)
{
    int score = 0;
    // read the message into string and convert it to lower case, then remove all newlines.
    std::string messageText((std::istreambuf_iterator<char>(message)),
                            std::istreambuf_iterator<char>());
    std::transform(messageText.begin(), messageText.end(), messageText.begin(), [](unsigned char c)
    { return std::tolower(c); });
    std::replace(messageText.begin(), messageText.end(), '\n', ' ');
    // go over pairs and calculate the score of the message
    for (auto const &p: map)
    {
        std::string toFind = p.first;
        std::transform(toFind.begin(), toFind.end(), toFind.begin(), [](unsigned char c)
        { return std::tolower(c); });
        size_t step = toFind.size();
        size_t pos = 0, count = 0;
        while ((pos = messageText.find(toFind, pos)) != std::string::npos)
        {
            pos += step;
            ++count;
        }
        score += count * p.second;
    }
    return score;
}

/**
 * @param file input file
 * @return true if file is empty; false otherwise.
 */
bool isEmpty(std::ifstream &file)
{
    return file.peek() == std::ifstream::traits_type::eof();
}

/**
 * Main driver of the program.
 * @param argc number of arguments
 * @param argv arguments array
 * @return EXIT_FAILURE in cases of invalid arguments, or memory error; EXIT_SUCCESS otherwise.
 */
int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: SpamDetector <database path> <message path> <threshold>" << std::endl;
        return EXIT_FAILURE;
    }
    std::ifstream database(argv[1]);
    std::ifstream message(argv[2]);
    int threshold = checkNumber(argv[3]);
    if (database.fail() || message.fail() || threshold <= 0)
    {
        database.close();
        message.close();
        std::cerr << "Invalid input" << std::endl;
        return EXIT_FAILURE;
    }
    if (isEmpty(message) || isEmpty(database))
    {
        std::cout << "NOT_SPAM" << std::endl;
        return EXIT_SUCCESS;
    }
    std::vector<std::string> phrases;
    std::vector<int> scores;
    int score = 0;
    try
    {
        parseDatabase(database, phrases, scores);
        database.close();
        HashMap<std::string, int> map(phrases, scores);
        score = parseMessage(message, map);
        message.close();
    }
    catch (std::bad_alloc &e)
    {
        database.close();
        message.close();
        std::cerr << "Memory allocation failed." << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::invalid_argument &e)
    {
        database.close();
        message.close();
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    threshold <= score ? std::cout << "SPAM" << std::endl : std::cout << "NOT_SPAM" << std::endl;
    return EXIT_SUCCESS;

}