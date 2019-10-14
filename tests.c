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
  /* FIXME */
}

static void test_get_os_info(void **state)
{
  osinfo *os = malloc(sizeof(osinfo));
  assert_non_null(os);
  os = get_os_info();
  /* FIXME */
}

static void test_dirfs(void **state)
{
  char *p = NULL;
  p = dirfs("/");
  assert_non_null(p);
}

static void test_lookup_fstype(void **state)
{
  char *fs = NULL;
  char* lookup_fstype( long fstype );
  p = dirfs("/");
  assert_non_null(p);
}

static void test_proc_uptime(void **state)
{
  int pid = 1;
  int t = 0;
  t = get_proc_uptime(pid);
  assert_int_not_equal(t, 0);
}

static void test_get_proc_uptime(void **state)
{
  pid_t pid = 1;
  long long int t = 0;
  t = proc_uptime(pid);
  assert_int_not_equal(t, 0);
}

static void test_get_proc_info(void **state)
{
  pid_t pid = 1;
  int i = 0;
  i = get_proc_info(pid);
  assert_int_not_equal(i, 0);
}

int main(void){

  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_get_hw_info),
    cmocka_unit_test(test_get_os_info),
    cmocka_unit_test(test_dirfs),
    cmocka_unit_test(test_lookup_fstype),
    cmocka_unit_test(test_get_proc_uptime),
    cmocka_unit_test(test_get_proc_info),
    cmocka_unit_test(test_proc_uptime)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
