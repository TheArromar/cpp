#ifndef UTILS_HPP
#define UTILS_HPP

#include "config.hpp"
#include <string>

void ltrim(std::string &s);
void rtrim(std::string &s);
void trim(std::string &s);
std::string lower_copy(const std::string& s);
bool is_alnum(char c);
int idNumericPart(const char* id);

// CSV parsing
bool parse_csv2(const std::string& line, std::string& a, std::string& b);
bool parse_csv3(const std::string& line, std::string& a, std::string& b, std::string& c);

// Tokenization
int  splitSkills(const std::string& skillsCSV, char out[][MAX_TOKEN_LEN], int maxOut);
int  tokensFromRole(const std::string& role, char out[][MAX_TOKEN_LEN], int maxOut);
bool tokenExists(char arr[][MAX_TOKEN_LEN], int n, const char* t);

// Formatting helpers
void buildQueryString(char query[][MAX_TOKEN_LEN], int qn, std::string& out);

#endif // UTILS_HPP
