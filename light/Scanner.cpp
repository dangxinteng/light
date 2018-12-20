#include"Scanner.h"

std::vector<Token>& Scanner::ScanTokens()
{
	unsigned current = -1;
	unsigned line = 1;
	unsigned preline = 1;
	unsigned linebegin = 0;
	Token token(Token::END_OF_LINE, 0, 0);
	while (char c = m_source[++current])
	{
		if (preline < line)
		{
			linebegin = current;
			preline = line;
		}
		token.row = line;
		token.col = current - linebegin + 1;
		switch (c)
		{
		case '(': token.t = Token::LEFT_PAREN; break;
		case ')': token.t = Token::RIGHT_PAREN; break;
		case '{': token.t = Token::LEFT_BRACE; break;
		case '}': token.t = Token::RIGHT_BRACE; break;
		case ',': token.t = Token::COMMA; break;
		case '.': token.t = Token::DOT; break;
		case ';': token.t = Token::SEMICOLON; break;
		case '-': token.t = Token::MINUS; break;
		case '+': token.t = Token::PLUS; break;
		case '*': token.t = Token::STAR; break;
		case '/':
			if (m_source[current + 1] != '\0' && m_source[current + 1] == '/')
				while (m_source[current + 1] != '\0' && m_source[current + 1] != '\n')
					current++;
			else
				token.t = Token::SLASH;
			continue;
		case '!':
			if (m_source[current + 1] != '\0' && m_source[current + 1] == '=')
			{
				token.t = Token::BANG_EQUAL;
				current++;
			}
			else
				token.t = Token::BANG;
			break;
		case '=':
			if (m_source[current + 1] != '\0' && m_source[current + 1] == '=')
			{
				token.t = Token::EQUAL_EQUAL;
				current++;
			}
			else
				token.t = Token::EQUAL;
			break;
		case '<':
			if (m_source[current + 1] != '\0' && m_source[current + 1] == '=')
			{
				token.t = Token::LESS_EQUAL;
				current++;
			}
			else
				token.t = Token::LESS;
			break;
		case '>':
			if (m_source[current + 1] != '\0' && m_source[current + 1] == '=')
			{
				token.t = Token::GREATER_EQUAL;
				current++;
			}
			else
				token.t = Token::GREATER;
			break;

		case ' ':
		case '\r':
		case '\t':
			// Ignore whitespace.                      
			continue;

		case '\n':
			line++;
			continue;

		case '"':
		{
			while (m_source[++current] != '"' && m_source[current] != '\0')
			{
				if (m_source[current] == '\n') line++;

			}
			if (m_source[current] == '\0')
				throw Error(line + "Unterminated string.");

			token.t = Token::STRING;
			token.value = Object(m_source.substr(token.col + linebegin, current - token.col - linebegin));
			break;
		}
		default:
			if (IsDigit(c))
			{
				while (IsDigit(m_source[current])) current++;

				if (m_source[current] == '.' && IsDigit(m_source[current + 1]))
				{
					current++;
					while (IsDigit(m_source[current])) current++;
				}

				token.t = Token::NUMBER;
				token.value = Object(stod(m_source.substr(token.col + linebegin - 1, current - token.col - linebegin + 1)));

				current--;//取完数字后回滚到数字末尾位置
			}
			else if (IsAlpha(c))
			{
				while (IsAlphaNumeric(m_source[current + 1]))
					current++;
				std::string text = m_source.substr(token.col + linebegin - 1, current - token.col - linebegin + 2);
				Token::TokenType t = m_keywords[text];
				if (!t)
				{
					token.t = Token::IDENTIFIER;
					token.value = Object(text);
				}
				else
				{
					token.t = t;
					switch (t)
					{
					case Token::TRUE:
						token.value = Object(true);
						break;
					case Token::FALSE:
						token.value = Object(false);
						break;
					case Token::NIL:
						token.value = Object(nullptr);
						break;
					case Token::THIS:
						token.value = Object(std::string("this"));
						break;
					}					
				}					
			}
			else
				throw Error("valid character!");
		}
		AddToken(token);
	}
	token = { Token::END_OF_LINE, line, current + 1 };
	AddToken(token);
	return m_tokensVector;
}
