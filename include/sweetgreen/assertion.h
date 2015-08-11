#ifndef _SWEETGREEN_ASSERTION_H_
#define _SWEETGREEN_ASSERTION_H_

#include "restrict_include.h"
#include "format.h"

struct sweetgreen_assertion {
	struct sweetgreen_operands operands;
	enum sweetgreen_result result;
	int line;
	const char* expectation;
	struct sweetgreen_assertion* next;
};

struct sweetgreen_assertion* sweetgreen_assertion_new(const char* l_name, const char* r_name, int line, const char* expectation, enum sweetgreen_result result) {
	struct sweetgreen_assertion* assertion = (struct sweetgreen_assertion*)malloc(sizeof(struct sweetgreen_assertion));
	assertion->next = NULL;
	assertion->operands.first_name = l_name;
	assertion->operands.second_name = r_name;
	assertion->line = line;
	assertion->result = result;
	assertion->expectation = expectation;
	return assertion;
}

void sweetgreen_assertion_print_fail_information(struct sweetgreen_assertion* assertion, FILE* output) {
	char line_str[48];
	memset(line_str, 0, 48);
	sprintf(line_str, SWEETGREEN_MAGENTA("(line: ") SWEETGREEN_MAGENTABOLD("%i") SWEETGREEN_MAGENTA(")"), assertion->line);
	fprintf(output, " -> [" SWEETGREEN_RED("%s%s%s") "] %s\n",
		assertion->operands.first_name,
		assertion->operands.second_name ? ", " : "",
		assertion->operands.second_name ? assertion->operands.second_name : "",
		line_str
	);
}

int sweetgreen_assertion_test(FILE* output, struct sweetgreen_assertion* assertion) {
	fprintf(output, "%s %s: %s\n", 
		assertion->result == PASSED ? SWEETGREEN_PASS_SYMBOL : SWEETGREEN_FAIL_SYMBOL,
		assertion->expectation,
		assertion->result ? SWEETGREEN_RED("fail") : SWEETGREEN_GREEN("ok")
	);
	
	if(assertion->result) { sweetgreen_assertion_print_fail_information(assertion, output); }

	return assertion->result != PASSED;
}

#endif