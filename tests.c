#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "helpers.h"

static void test_get_hw_info(void **state)
{
    hwinfo *hw = NULL;
    hw = malloc(sizeof(hwinfo));
    assert_non_null(hw);
    hw = get_hw_info();
    assert_int_not_equal(hw->ncpu, 0);
    assert_int_not_equal(hw->memsize, 0);
    assert_int_not_equal(hw->uptime, 0);
}

static void test_get_os_info(void **state)
{
    osinfo *os = malloc(sizeof(osinfo));
    assert_non_null(os);
    os = get_os_info();
    assert_string_not_equal(os->os_version, "");
    assert_string_not_equal(os->os_family, "");
    assert_string_not_equal(os->os_name, "");
    assert_string_not_equal(os->os_arch, "");
}

static void test_dirfs(void **state)
{
    char *p = dirfs("/");
    assert_string_not_equal(p, "");
}

#ifdef __linux__
static void test_lookup_fstype(void **state)
{
    char *type = lookup_fstype(1);
    assert_string_equal(type, "<fs type unknown>");
}
#endif

static void test_get_proc_uptime(void **state)
{
    pid_t pid = 1;
    long long int t = 0;
    t = get_proc_uptime(pid);
    assert_int_not_equal(t, 0);
}

int main(void){
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_get_hw_info),
        cmocka_unit_test(test_get_os_info),
        cmocka_unit_test(test_get_proc_uptime),
        cmocka_unit_test(test_dirfs),
#ifdef __linux__
        cmocka_unit_test(test_lookup_fstype),
#endif
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
