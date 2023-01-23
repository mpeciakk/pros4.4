#ifndef PROS_TESTS_HPP
#define PROS_TESTS_HPP

#include <Lib/Log.hpp>
#include <Lib/String.hpp>

#define EXPECT(value, expectedValue) return expect(value, expectedValue, __FUNCTION__, __FILE__, __LINE__)

#define TESTS_COUNT 8

int expect(int value, int expectedValue, const char *function, const char *file, int line) {
    if (value == expectedValue) {
        klog(3, "%s [%s:%d]: Success", function, file, line);
        return 0;
    } else {
        klog(2, "%s [%s:%d]: Got %d instead of %d", function, file, line, value, expectedValue);
        return -1;
    }
}

int expect(const char *value, const char *expectedValue, const char *function, const char *file, int line) {
    if (strcmp(value, expectedValue) == 0) {
        klog(3, "%s [%s:%d]: Success", function, file, line);
        return 0;
    } else {
        klog(2, "%s [%s:%d]: Got %s instead of %s", function, file, line, value, expectedValue);
        return -1;
    }
}

int strlenTest() {
    EXPECT(strlen("some string"), 11);
}

int memsetTest() {
    char str[10] = "123456789";

    memset(str + 2, '.', 3 * sizeof(char));

    EXPECT(str, "123...789");
}

int memcpyTest() {
    char str[10] = "917284618";
    char str2[10];
    memcpy(str2, str, 10);

    EXPECT(str2, str);
}

int strcmpTest() {
    char str[10] = "jskw9s07f";
    char str2[10] = "jskw9s07f";

    EXPECT(strcmp(str, str2), 0);
}

int strcmpTest2() {
    char str[11] = "hiod1902ed";
    char str2[11] = "hiop1902ed";

    EXPECT(strcmp(str, str2), -12);
}

int strcpyTest() {
    char str[10] = "917284618";
    char str2[10];
    strcpy(str2, str);

    EXPECT(str2, str);
}

int itosTest() {
    int n = 916243;
    char str[countDigit(n)];
    itos(n, str, 10);

    EXPECT(str, "916243");
}

int countDigitTest() {
    EXPECT(countDigit(12849172), 8);
}

int (*tests[TESTS_COUNT])(){&memsetTest, &strlenTest, &memcpyTest, &strcmpTest, &strcmpTest2, &strcpyTest, &countDigitTest,
                            &itosTest};

void performTests() {
    klog(3, "-----------------------Performing Tests-----------------------");

    int passed = 0;
    int total = 0;

    for (auto &test: tests) {
        klog(3, "Test %d/%d", passed + 1, TESTS_COUNT);

        if (test() == 0) {
            passed++;
        }

        total++;
    }

    klog(3, "Passed %d/%d tests", passed, total);
}

#endif
