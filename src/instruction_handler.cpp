/**
 * X10 (https://github.com/UnexomWid/X10)
 *
 * This project is licensed under the MIT license.
 * Copyright (c) 2018-2019 UnexomWid (https://uw.exom.dev)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "instruction_handler.h"

#include <string>

std::unordered_map<char, Instruction> instruction_list;

Instruction INSTRUCTION_VALUE_INCREMENT('+', VALUE_INCREMENT);
Instruction INSTRUCTION_VALUE_DECREMENT('-', VALUE_DECREMENT);
Instruction INSTRUCTION_VALUE_OPERATION('(', VALUE_OPERATION);

Instruction INSTRUCTION_INDEX_INCREMENT('>', INDEX_INCREMENT);
Instruction INSTRUCTION_INDEX_DECREMENT('<', INDEX_DECREMENT);

Instruction INSTRUCTION_UNCERTAINTY_START('?', UNCERTAINTY_START);
Instruction INSTRUCTION_UNCERTAINTY_END('!', UNCERTAINTY_END);

Instruction INSTRUCTION_LOOP_START('{', LOOP_START);
Instruction INSTRUCTION_LOOP_END('}', LOOP_END);

Instruction INSTRUCTION_OUTPUT_WRITE('^', OUTPUT_WRITE);

Instruction INSTRUCTION_INPUT_READ('V', INPUT_READ);
Instruction INSTRUCTION_INPUT_ADD('v', INPUT_ADD);
Instruction INSTRUCTION_INPUT_XOR('x', INPUT_XOR);
Instruction INSTRUCTION_INPUT_AND('&', INPUT_AND);
Instruction INSTRUCTION_INPUT_OR('|', INPUT_OR);

Instruction INSTRUCTION_FILE_OPEN('F', FILE_OPEN);
Instruction INSTRUCTION_FILE_CLOSE('f', FILE_CLOSE);

void initializeInstructions() {
	instruction_list[INSTRUCTION_VALUE_INCREMENT.getIdentifier()] = INSTRUCTION_VALUE_INCREMENT;
	instruction_list[INSTRUCTION_VALUE_DECREMENT.getIdentifier()] = INSTRUCTION_VALUE_DECREMENT;
	instruction_list[INSTRUCTION_VALUE_OPERATION.getIdentifier()] = INSTRUCTION_VALUE_OPERATION;

	instruction_list[INSTRUCTION_INDEX_INCREMENT.getIdentifier()] = INSTRUCTION_INDEX_INCREMENT;
	instruction_list[INSTRUCTION_INDEX_DECREMENT.getIdentifier()] = INSTRUCTION_INDEX_DECREMENT;

	instruction_list[INSTRUCTION_UNCERTAINTY_START.getIdentifier()] = INSTRUCTION_UNCERTAINTY_START;
	instruction_list[INSTRUCTION_UNCERTAINTY_END.getIdentifier()] = INSTRUCTION_UNCERTAINTY_END;

	instruction_list[INSTRUCTION_LOOP_START.getIdentifier()] = INSTRUCTION_LOOP_START;
	instruction_list[INSTRUCTION_LOOP_END.getIdentifier()] = INSTRUCTION_LOOP_END;

	instruction_list[INSTRUCTION_OUTPUT_WRITE.getIdentifier()] = INSTRUCTION_OUTPUT_WRITE;

	instruction_list[INSTRUCTION_INPUT_READ.getIdentifier()] = INSTRUCTION_INPUT_READ;
	instruction_list[INSTRUCTION_INPUT_ADD.getIdentifier()] = INSTRUCTION_INPUT_ADD;
	instruction_list[INSTRUCTION_INPUT_XOR.getIdentifier()] = INSTRUCTION_INPUT_XOR;
	instruction_list[INSTRUCTION_INPUT_AND.getIdentifier()] = INSTRUCTION_INPUT_AND;
	instruction_list[INSTRUCTION_INPUT_OR.getIdentifier()] = INSTRUCTION_INPUT_OR;

    instruction_list[INSTRUCTION_FILE_OPEN.getIdentifier()] = INSTRUCTION_FILE_OPEN;
    instruction_list[INSTRUCTION_FILE_CLOSE.getIdentifier()] = INSTRUCTION_FILE_CLOSE;
}

bool findInstruction(char id, Instruction &instr) {
	if (instruction_list.find(id) != instruction_list.end()) {
		instr = instruction_list[id];
		return true;
	}
	return false;
}

uint8_t parseNum(POINTER_INFO) {
	// This function expects a NUMBER_START character at the beginning.
	// That's why characters should be extracted carefully, with peek() instead of get().
	// NUMBER_START characters should not be discarded/ignored somewhere else, except at the beginning of this function (see below).

	if (script.get() != NUMBER_START)
		throw std::runtime_error("Expected number start");

	bool neg = false; // Negative number.
	bool val = false; // Value at index.
	bool ind = false; // At index.
	bool add = false; // Add.
	bool sub = false; // Subtract.

	char op = script.peek(); // Number start or something else. Don't discard/ignore it yet (see above).

	if (op == NUMBER_END) {
		script.ignore(1); // Not a number start.
		return 0;
	}
	if (op == NUMBER_MODIFIER_NEGATIVE) {
		script.ignore(1); // Not a number start.
		neg = true;
		op = script.peek();
	}

	if (op == NUMBER_MODIFIER_VALUE_AT) {
		script.ignore(1); // Not a number start.
		val = true;
		op = script.peek();
	}

	if (op == NUMBER_MODIFIER_INDEX) {
		script.ignore(1); // Not a number start.
		ind = true;
		op = script.peek();
	}

	if (op == '+') {
		script.ignore(1); // Not a number start.
		add = true;
		op = script.peek();
	}
	else if (op == '-') {
		script.ignore(1); // Not a number start.
		sub = true;
		op = script.peek();
	}

	if (!isdigit(op)) {
		if (op == NUMBER_END) {
			script.ignore(1); // Not a number start.

			if (!ind || add || sub)
				throw std::runtime_error("Expected number");

			if (val) {
				return (uint8_t)(neg ? (-1) * pointer.at(index) : pointer.at(index));
			}
			else {
				return (uint8_t)(neg ? (-1) * index : index);
			}
		}
		else if (op == NUMBER_START) { // Number start.
			if (ind) {
				if (val) {
					if (add)
						return (uint8_t) (neg ? (-1) * pointer.at(index + parseNum(POINTER_INFO_PARAMS)) : pointer.at(index + parseNum(POINTER_INFO_PARAMS)));
					if (sub)
						return (uint8_t)(neg ? (-1) * pointer.at(index - parseNum(POINTER_INFO_PARAMS)) : pointer.at(index - parseNum(POINTER_INFO_PARAMS)));
					return (uint8_t)(neg ? (-1) * pointer.at(parseNum(POINTER_INFO_PARAMS)) : pointer.at(parseNum(POINTER_INFO_PARAMS)));
				}
				else {
					if (add)
						return (uint8_t)(neg ? (-1) * (index + parseNum(POINTER_INFO_PARAMS)) : index + parseNum(POINTER_INFO_PARAMS));
					if (sub)
						return (uint8_t)(neg ? (-1) * (index - parseNum(POINTER_INFO_PARAMS)) : index - parseNum(POINTER_INFO_PARAMS));
					return (uint8_t)(neg ? (-1) * parseNum(POINTER_INFO_PARAMS) : parseNum(POINTER_INFO_PARAMS));
				}
			}
			else {
				return (uint8_t)(neg ? (-1) * parseNum(POINTER_INFO_PARAMS) : parseNum(POINTER_INFO_PARAMS));
			}
		}
	}
	else {
		script.ignore(1); // Not a number start.

		std::string res;
		res.push_back(op);

		while (isdigit(op = script.get())) {
			res.push_back(op);
		}

		if(op != NUMBER_END)
			throw std::runtime_error("Expected number end");
		while (script.peek() == NUMBER_END && !script.eof())
			script.ignore(1);

		uint32_t num = stoi(res);

		if (ind) {
			if (val) {
				if (add)
					return (uint8_t)(neg ? (-1) * pointer.at(index + num) : pointer.at(index + num));
				if (sub)
					return (uint8_t)(neg ? (-1) * pointer.at(index - num) : pointer.at(index - num));
				return (uint8_t)(neg ? (-1) * pointer.at(num) : pointer.at(num));
			}
			else {
				if (add)
					return (uint8_t)(neg ? (-1) * (index + parseNum(POINTER_INFO_PARAMS)) : index + num);
				if (sub)
					return (uint8_t)(neg ? (-1) * (index - num) : index - num);
				return (uint8_t)(neg ? (-1) * num : num);
			}
		}
		else {
			return (uint8_t)(neg ? (-1) * num : num);
		}
	}

	return 0;
}

bool parseExpression(POINTER_INFO) {
	uint8_t left = parseNum(POINTER_INFO_PARAMS);

	std::string relational_operator;
	while (script.peek() != NUMBER_START && !script.eof())
		relational_operator.push_back(script.get());

	uint8_t right = parseNum(POINTER_INFO_PARAMS);

	bool expression;
	if (relational_operator == RELATIONAL_EQUAL) // Equal.
		expression = left == right;
	else if (relational_operator == RELATIONAL_NOT_EQUAL) // Not Equal.
		expression = left != right;
	else if (relational_operator == RELATIONAL_GREATER_THAN) // Greater Than.
		expression = left > right;
	else if (relational_operator == RELATIONAL_GREATER_THAN_OR_EQUAL) // Greater Than or Equal.
		expression = left >= right;
	else if (relational_operator == RELATIONAL_LESS_THAN) // Less Than.
		expression = left < right;
	else if (relational_operator == RELATIONAL_LESS_THAN_OR_EQUAL) // Less Than or Equal.
		expression = left <= right;
	else throw std::runtime_error("Invalid relational operator");

	std::string conditional_operator;
	std::streampos before = script.tellg();

	for (char c : CONDITIONAL_AND) // AND.
		if (script.peek() == c)
			conditional_operator.push_back(script.get());
	if (conditional_operator != CONDITIONAL_AND) {
		script.seekg(before);
		conditional_operator = "";
		for (char c : CONDITIONAL_OR) // OR.
			if (script.peek() == c)
				conditional_operator.push_back(script.get());
		if (conditional_operator != CONDITIONAL_OR) {
			script.seekg(before);
			conditional_operator = "";
			for (char c : CONDITIONAL_XOR) // XOR.
				if (script.peek() == c)
					conditional_operator.push_back(script.get());
			if (conditional_operator != CONDITIONAL_XOR)
				script.seekg(before); // Nothing.
		}
	}

	if (conditional_operator == CONDITIONAL_AND)
		return expression && parseExpression(POINTER_INFO_PARAMS);
	if (conditional_operator == CONDITIONAL_OR)
		return parseExpression(POINTER_INFO_PARAMS) || expression; // Force parse, even if 'expression' is true, to move the stream cursor forwards.
	if (conditional_operator == CONDITIONAL_XOR)
		return expression != parseExpression(POINTER_INFO_PARAMS);
	return expression;
}

void VALUE_INCREMENT(POINTER_INFO) {
	pointer.at(index)++;
}

void VALUE_DECREMENT(POINTER_INFO) {
	pointer.at(index)--;
}

void VALUE_OPERATION(POINTER_INFO) {
	char op = 0;
	if (script.peek() != NUMBER_START) { // Apply to current index.
		script.get(op);

		switch (op) {
			case '$': {
				pointer.at(index) = parseNum(POINTER_INFO_PARAMS);
				break;
			}
			case '+': {
				pointer.at(index) += parseNum(POINTER_INFO_PARAMS);
				break;
			}
			case '-': {
				pointer.at(index) -= parseNum(POINTER_INFO_PARAMS);
				break;
			}
			case '*': {
				pointer.at(index) *= parseNum(POINTER_INFO_PARAMS);
				break;
			}
			case '/': {
				pointer.at(index) /= parseNum(POINTER_INFO_PARAMS);
				break;
			}
			case '%': {
				pointer.at(index) %= parseNum(POINTER_INFO_PARAMS);
				break;
			}
			case 'x': {
				pointer.at(index) ^= parseNum(POINTER_INFO_PARAMS);
				break;
			}
			case '&': {
				pointer.at(index) &= parseNum(POINTER_INFO_PARAMS);
				break;
			}
			case '|': {
				pointer.at(index) |= parseNum(POINTER_INFO_PARAMS);
				break;
			}
			default: {
				throw std::runtime_error("Invalid operator");
				break;
			}
		}
	}
	else {
		uint32_t new_index = parseNum(POINTER_INFO_PARAMS); // New index.
		while (pointer.size() <= new_index) // Prevents 'invalid vector<t> subscript'.
			pointer.push_back(0); // Pad with 0s until the new index is reached.

		script.get(op);

		switch (op) {
			case '$': {
				pointer.at(new_index) = parseNum(POINTER_INFO_PARAMS);
				break;
			}
			case '+': {
				pointer.at(new_index) += parseNum(POINTER_INFO_PARAMS);
				break;
			}
			case '-': {
				pointer.at(new_index) -= parseNum(POINTER_INFO_PARAMS);
				break;
			}
			case '*': {
				pointer.at(new_index) *= parseNum(POINTER_INFO_PARAMS);
				break;
			}
			case '/': {
				pointer.at(new_index) /= parseNum(POINTER_INFO_PARAMS);
				break;
			}
			case '%': {
				pointer.at(index) %= parseNum(POINTER_INFO_PARAMS);
				break;
			}
			case 'x': {
				pointer.at(new_index) ^= parseNum(POINTER_INFO_PARAMS);
				break;
			}
			case '&': {
				pointer.at(new_index) &= parseNum(POINTER_INFO_PARAMS);
				break;
			}
			case '|': {
				pointer.at(new_index) |= parseNum(POINTER_INFO_PARAMS);
				break;
			}
			default: {
				throw std::runtime_error("Invalid operator");
				break;
			}
		}
	}

	script.ignore(1); // Ending round bracket.
}

void INDEX_INCREMENT(POINTER_INFO) {
	++index;
	if (index > pointer.size() - 1)
		pointer.push_back(0);
}

void INDEX_DECREMENT(POINTER_INFO) {
	--index;
}

void UNCERTAINTY_START(POINTER_INFO) {
	if (!parseExpression(POINTER_INFO_PARAMS)) { // Skip uncertainty.
		int open_count = 1;
		while (open_count > 0) {
			char c = script.get();
			if (c == '?')
				open_count++;
			else if (c == '!')
				open_count--;
		}
	}
	else ++uncertainty_count;
}

void UNCERTAINTY_END(POINTER_INFO) {
	if (uncertainty_count == 0)
		throw std::runtime_error("Unexpected uncertainty end");

	--uncertainty_count;
}

void LOOP_START(POINTER_INFO) {
	loop_stack.push(script.tellg()); // Add the position to the stack.

	if (!parseExpression(POINTER_INFO_PARAMS)) { // Skip loop.
		loop_stack.pop(); // Remove the position from the stack.
		uint32_t open_count = 1;
		while (open_count > 0)
		{
			char c = script.get();
			if (c == '{')
				++open_count;
			else if (c == '}')
				--open_count;
		}
	}
}

void LOOP_END(POINTER_INFO)
{
	uint32_t after_loop = script.tellg();

	script.seekg(loop_stack.top());

	if (!parseExpression(POINTER_INFO_PARAMS)) { // Skip loop.
		loop_stack.pop(); // Remove the position from the stack.
		script.seekg(after_loop);
	}
}

void OUTPUT_WRITE(POINTER_INFO) {
	char format = script.peek();

	if (format != 'n' && format != 'c' && format != '_' && format != '\\') {
        (file_output != nullptr ? (*file_output) : output) << pointer.at(index);
		return;
	}

	while ((format == 'n' || format == 'c' || format == '_' || format == '\\') && !script.eof()) {
		script.ignore(1);

		if (format == 'n') {
            (file_output != nullptr ? (*file_output) : output) << (uint16_t) pointer.at(index);
		}
		else if (format == 'c') {
            (file_output != nullptr ? (*file_output) : output) << (char) pointer.at(index);
		}
		else if (format == '_') {
            (file_output != nullptr ? (*file_output) : output) << ' ';
		}
		else {
            (file_output != nullptr ? (*file_output) : output) << '\n';
		}

		format = script.peek();
	}
}

void INPUT_READ(POINTER_INFO) {
	uint16_t num = 0;
    (file_input != nullptr ? (*file_input) : input) >> num;
	pointer.at(index) = (uint8_t) num;
}

void INPUT_ADD(POINTER_INFO) {
    uint16_t num = 0;
    (file_input != nullptr ? (*file_input) : input) >> num;
	pointer.at(index) += (uint8_t) num;
}

void INPUT_XOR(POINTER_INFO) {
    uint16_t num = 0;
    (file_input != nullptr ? (*file_input) : input) >> num;
	pointer.at(index) ^= (uint8_t) num;
}

void INPUT_AND(POINTER_INFO) {
    uint16_t num = 0;
    (file_input != nullptr ? (*file_input) : input) >> num;
	pointer.at(index) &= (uint8_t) num;
}

void INPUT_OR(POINTER_INFO) {
    uint16_t num = 0;
    (file_input != nullptr ? (*file_input) : input) >> num;
	pointer.at(index) |= (uint8_t) num;
}

void FILE_OPEN(POINTER_INFO) {
    char mode = script.peek();

    if (mode != 'v' && mode != '^')
        throw std::runtime_error("Expected filename open mode");

    script.ignore(1);
    if(script.peek() != '\"')
        throw std::runtime_error("Expected starting quotes");
    script.ignore(1);

    std::string filename;

    while(script.peek() != '\"' && !script.eof())
        filename.push_back(script.get());

    if(script.eof())
        throw std::runtime_error("Expected ending quotes");

    script.ignore(1);

    if(mode == 'v') {
        if(file_input != nullptr)
            file_input -> close();
        file_input = new std::ifstream(filename);
    } else {
        if(file_output != nullptr)
            file_output -> close();
        file_output = new std::ofstream(filename);
    }
}

void FILE_CLOSE(POINTER_INFO) {
    char mode = script.peek();

    if (mode != 'v' && mode != '^')
        throw std::runtime_error("Expected filename close mode");

    script.ignore(1);

    if(mode == 'v') {
        if(file_input != nullptr) {
            file_input -> close();
            delete file_input;
            file_input = nullptr;
        } else throw std::runtime_error("No file opened with read mode");
    } else {
        if(file_output != nullptr) {
            file_output -> close();
            delete file_output;
            file_output = nullptr;
        } else throw std::runtime_error("No file opened with write mode");
    }
}