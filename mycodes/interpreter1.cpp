#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>

class Token {
public:
    std::string _type;
    std::string _value;

    Token(std::string type = "ND", std::string value = "ND") {
        _type = type;
        _value = value;
    }

    std::string toString() {
        return "Token(" + _type + ", " + _value + ")";
    }
};

class Interpreter {
public:
    std::string text;
    int pos = 0;
    char current_char;
    Token current_token;

    Interpreter(std::string input_text) {
        text = input_text;
        if (!text.empty()) {
            current_char = text[0];
        }
    }

    void error() {
        throw std::runtime_error("Invalid syntax");
    }

    void advance() {
        pos += 1;
        if (pos > text.length() - 1) {
            current_char = '\0';
        } else {
            current_char = text[pos];
        }
    }

    void skip_whitespace() {
        while (pos < text.length() && std::isspace(current_char)) {
            advance();
        }
    }

    int integer() {
        std::string result = "";
        while (pos < text.length() && std::isdigit(current_char)) {
            result += current_char;
            advance();
        }
        return std::stof(result);
    }

    Token get_next_token() {
        while (current_char != '\0') {
            if (std::isspace(current_char)) {
                skip_whitespace();
                continue;
            } else if (std::isdigit(current_char)) {
                return Token("INTEGER", std::to_string(integer()));
            } else if (current_char == '+') {
                advance();
                return Token("PLUS", "+");
            } else if (current_char == '-') {
                advance();
                return Token("MINUS", "-");
            } else if (current_char == '*') {
                advance();
                return Token("MULT", "*");
            } else if (current_char == '/') {
                advance();
                return Token("DIVIDE", "/");
            } else {
                error();
            }
        }
        return Token("EOF", "");
    }

    void eat(std::string token_type) {
        if (current_token._type == token_type) {
            current_token = get_next_token();
        } else {
            error();
        }
    }

    float factor() {
        Token token = current_token;
        if (token._type == "INTEGER") {
            eat("INTEGER");
            return std::stof(token._value); 
        } else {
            error();
            return 0.0;  
        }
    }

    float term() {
        float result = factor();

        while (current_token._type == "MULT" || current_token._type == "DIVIDE") {
            Token token = current_token;
            if (token._type == "MULT") {
                eat("MULT");
                result *= factor();
            } else if (token._type == "DIVIDE") {
                eat("DIVIDE");
                int denominator = factor();
                if (denominator == 0) {
                    throw std::runtime_error("Division by zero");
                }
                result /= denominator;
            }
        }
        return result;
    }

    float expr() {
        current_token = get_next_token();
        float result = term();

        while (current_token._type == "PLUS" || current_token._type == "MINUS") {
            Token token = current_token;
            if (token._type == "PLUS") {
                eat("PLUS");
                result += term();
            } else if (token._type == "MINUS") {
                eat("MINUS");
                result -= term();
            }
        }

        return result;
    }
};

int main() {
    std::string text_input;
    float result;

    while (true) {
        try {
            std::cout << "calc> ";
            std::getline(std::cin, text_input);

            if (text_input.empty()) {
                continue;
            }

            Interpreter interpreter(text_input);
            result = interpreter.expr();
            std::cout << result << std::endl;
        } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
        }
    }

    return 0;
}
