#include <stdio.h>
#include "minunit.h"
#include "unsignedFunctions.h"

int tests_run = 0;


static char * test_authentication() {
	mu_assert("error, open database", openDataBase() == 0);
	mu_assert("error, admin authentication", authentication("admin", "pass1") == '1');
	mu_assert("error, operator authentication", authentication("oper", "pass2") == '2');
	mu_assert("error, user authentication", authentication("shipa", "jenia") == '3');
	mu_assert("error, empty input", authentication("", "") == 'u');
	mu_assert("error, wrong input", authentication("admin\0password", "**_88") == 'u');
	mu_assert("error, wrong input", authentication("TRUE", "") == 'u');
	mu_assert("error, long login", authentication("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", "") == 'u');
	mu_assert("error, long password", authentication("", "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA") == 'u');
	mu_assert("error, unreal authentication", authentication("sa", "pass1") == 'u');
	mu_assert("error, operator authentication", closeDataBase() == 0);

	return 0;
}


static char * all_tests() {
	mu_run_test(test_authentication);
	return 0;
}


int startTests() {
	char *result = all_tests();
	if (result != 0) {
		printf("%s\n", result);
	} else {
		printf("ALL TESTS PASSED\n");
	}
	printf("Tests run: %d\n", tests_run);
	system("pause");
	return result != 0;
}

