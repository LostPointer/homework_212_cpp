#include "utils.hpp"

#include <stack>
#include <stdexcept>

std::vector<std::string> SplitStringOld(const std::string &data) {
  std::vector<std::string> result;

  std::string word;
  for (size_t i = 0; i < data.size(); ++i) {
    if (data[i] != ' ' && data[i] != '\t') {
      word.push_back(data[i]);
    } else if (!word.empty()) {
      result.push_back(std::move(word));
      word.clear();
    }
  }
  if (!word.empty()) {
    result.push_back(word);
  }

  return result;
}

namespace {

bool IsOpeningBracket(char c) {
  return c == '(';
}

bool IsClosingBracket(char c) {
  return c == ')';
}

bool IsWhitespace(char c) {
  return c == ' ' || c == '\t';
}

}

std::vector<std::string> SplitString(const std::string& data) {
  std::stack<char> brackets;
  std::vector<std::string> tokens;
  std::string token;
  for (char c : data) {
    if (IsWhitespace(c) && brackets.empty()) {
      if (token.empty()) continue;
      
      tokens.push_back(std::move(token));
      token.clear();
    }
    else if (IsOpeningBracket(c)) {
      if (!token.empty() && brackets.empty()) {
        tokens.push_back(std::move(token));
        token.clear();
      }
      
      brackets.push(c);
      token += c;
    }
    else if (IsClosingBracket(c)) {
      if (brackets.empty())
        throw std::runtime_error("unexpected closing brackets");
      
      brackets.pop();
      token += c;

      if (brackets.empty()) {
        tokens.push_back(std::move(token));
        token.clear();
      }
    }
    else {
      token += c;
    }
  }

  if (!token.empty())
    tokens.push_back(token);
  
  if (!brackets.empty())
    throw std::runtime_error("no closing brackets");

  return tokens;
}