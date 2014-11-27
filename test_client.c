#include <stdio.h>
#include "minunit.h"
#include "clientFunctions.h"
#include "unsignedFunctions.h"
#include <stdlib.h>

int tests_run = 0;

static char * test_client() {
	authentication("shipa", "jenia");
	mu_assert("error, cannot watch client accounts", watchClientAccounts(2));
	mu_assert("error, cannot watch account balance", watchAccountBalance(1, 1));
	mu_assert("error, cannot watch account cards", watchAccountCards(1, 1));
	mu_assert("error, char id", watchClientAccounts('a'));
	mu_assert("error, negative id", watchClientAccounts(-100));
	mu_assert("error, hex id", watchClientAccounts(0x23321));
	mu_assert("error, non integer id", watchClientAccounts(23.222));
	mu_assert("error, invalid card id for account id", watchAccountCards(3, 1));
	mu_assert("error, null pointer", watchClientAccounts(NULL));
	return 0;
}


static char * all_tests() {
	mu_run_test(test_client);
	return 0;
}

int main() {
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