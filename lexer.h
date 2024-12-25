#pragma once
#include "Token.h"
#include "hash_table.h"
#include "Function.h"
#include "dfa.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <cctype>

using namespace std;

class Lexer {
private:
	HashTable hashTable;

	void getNextToken(vector<char>& alphabet, string& s, bool& flag, int& count) {
		if (s != "") {
			if (flag) {
				if (count != 2) {
					hashTable.insert(Token(TokenType::ERROR, s));
					flag = 0;
					count = 0;
					s.clear();
					return;
				}
			}
			if (dfa(countStatesComplexComplx, alphabet, finalStatesComplexComplx, transitFunctionComplexComplx).isAccept(s)) {
				hashTable.insert(Token(TokenType::KEYWORD, s));
				s.clear();
				return;
			}
			else if (dfa(countStatesInt, alphabet, finalStatesInt, transitFunctionInt).isAccept(s)) {
				hashTable.insert(Token(TokenType::KEYWORD, s));
				s.clear();
				return;
			}
			else if (dfa(countStatesMain, alphabet, finalStatesMain, transitFunctionMain).isAccept(s)) {
				hashTable.insert(Token(TokenType::ID, s));
				s.clear();
				return;
			}
			else if (dfa(countStatesComplex, alphabet, finalStatesComplex, transitFunctionComplex).isAccept(s))
			{
				hashTable.insert(Token(TokenType::FUNCTION, s));
			}
			else if (dfa(countStatesComplexCabs, alphabet, finalStatesComplexCabs, transitFunctionComplexCabs).isAccept(s))
			{
				hashTable.insert(Token(TokenType::FUNCTION, s));
			}
			else if (dfa(countStatesVar, alphabet, finalStatesVar, transitFunctionVar).isAccept(s)) {
				hashTable.insert(Token(TokenType::ID, s));
				s.clear();
				return;
			}
			else if (dfa(countStatesConst, alphabet, finalStatesConst, transitFunctionConst).isAccept(s))
			{
				hashTable.insert(Token(TokenType::CONST, s));
				s.clear();
				return;
			}
			hashTable.insert(Token(TokenType::ERROR, s));
			s.clear();
		}
	}

public:
	void Alphabet(vector<char>& alphabet, string& alp) {
		for (char c = 'a'; c <= 'z'; c++) {
			alphabet.push_back(c);
		}
		for (char c = 'A'; c <= 'Z'; c++) {
			alphabet.push_back(c);
		}
		for (char c = '0'; c <= '9'; c++) {
			alphabet.push_back(c);
		}
		alphabet.push_back('.');
		alphabet.push_back('+');
		alphabet.push_back('-');
		alphabet.push_back(',');
		alphabet.push_back(';');
		alphabet.push_back('?');
		alphabet.push_back('!');
		alphabet.push_back('"');
		alphabet.push_back('(');
		alphabet.push_back(')');
		alphabet.push_back('{');
		alphabet.push_back('}');
		alphabet.push_back('=');
		alp = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!?";
	}

	bool IsValidChar(char c, const std::string& alphabet) {
		for (char validChar : alphabet) {
			if (c == validChar) {
				return true;
			}
		}
		return false;
	}

	Lexer(ifstream& cin) {
		vector<char> alphabet;
		string alp;
		Alphabet(alphabet, alp);

		if (cin.eof()) {
			hashTable.insert(Token(TokenType::ERROR, "FILE"));
			return;
		}
		string str; char temp; 	bool f = 0; int c = 0;
		while (cin.get(temp)) {
			if (str == "CMPLX")
			{
				char temp2;
				string str2 = "(";
				while (cin.get(temp2))
				{
					if (temp2 == ')')
					{
						str2 += temp2;
						break;
					}
					str2 += temp2;
				}
				hashTable.insert(Token(TokenType::CONST, str2));
				str = "";
			}
			else if (temp == ',') {
				getNextToken(alphabet, str, f, c);
				string u = "";
				u += temp;
				hashTable.insert(Token(TokenType::SEPARATORS, u));
			}
			else if (temp == '(') {
				getNextToken(alphabet, str, f, c);
				hashTable.insert(Token(TokenType::SEPARATORS, "("));
			}
			else if (temp == ')') {
				getNextToken(alphabet, str, f, c);
				hashTable.insert(Token(TokenType::SEPARATORS, ")"));
			}
			else if (temp == ' ') {
				getNextToken(alphabet, str, f, c);
			}
			else if (temp == '\n') {
				getNextToken(alphabet, str, f, c);
			}
			else if (temp == '+' || temp == '-' || temp == '=') {
				getNextToken(alphabet, str, f, c);
				string o = "";
				o += temp;
				hashTable.insert(Token(TokenType::OPERATOR, o));
			}
			else if (temp == '"' || f == 1) {
				if (temp == '"') {
					c += 1;
				}
				f = 1;
				str += temp;
			}
			else if (!IsValidChar(temp, alp)) {
				getNextToken(alphabet, str, f, c);
				string e = "";
				e += temp;
				hashTable.insert(Token(TokenType::ERROR, e));
			}
			else {
				str += temp;
			}
		}
		if (!str.empty()) {
			getNextToken(alphabet, str, f, c);
		}
	}
	HashTable getHashTable()
	{
		return this->hashTable;
	}
};