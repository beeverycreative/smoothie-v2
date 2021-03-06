#include <nuttx/config.h>
#include <nuttx/init.h>
#include <nuttx/arch.h>
#include <sys/boardctl.h>

#include <stdio.h>

#include <vector>
#include <tuple>
#include <functional>

#include "../Unity/src/unity.h"
#include "TestRegistry.h"

static std::function<void(void)> setup_fnc;
void setUp(void)
{
    if(setup_fnc)
        setup_fnc();
}

static std::function<void(void)> teardown_fnc;
void tearDown(void)
{
    if(teardown_fnc)
        teardown_fnc();
}

static std::function<void(void)> test_wrapper_fnc;
static void test_wrapper(void)
{
    test_wrapper_fnc();
}

static int test_runner(void)
{
    auto tests= TestRegistry::instance().get_tests();
    printf("There are %d registered tests...\n", tests.size());
    for(auto& i : tests) {
        printf("  %s\n", std::get<1>(i));
    }

    UnityBegin("TestUnits");

    for(auto i : tests) {
        TestBase *fnc= std::get<0>(i);
        const char *name= std::get<1>(i);
        int ln= std::get<2>(i);
        Unity.TestFile= std::get<3>(i);
        test_wrapper_fnc= std::bind(&TestBase::test, fnc);
        bool st= std::get<4>(i);
        if(st) {
            setup_fnc= std::bind(&TestBase::setUp, fnc);
            teardown_fnc= std::bind(&TestBase::tearDown, fnc);
        }else{
            setup_fnc= nullptr;
            teardown_fnc= nullptr;
        }

        UnityDefaultTestRun(test_wrapper, name, ln);
    }

    return (UnityEnd());
}

static int run_tests(int argc, char *argv[])
{
    // do C++ initialization for static constructors first
    up_cxxinitialize();

    printf("Starting tests...\n");
    int ret = test_runner();
    printf("Done\n");
    return ret;
}

extern "C" int smoothie_main(int argc, char *argv[])
{
    int ret = boardctl(BOARDIOC_INIT, 0);
    if(OK != ret) {
        printf("ERROR: BOARDIOC_INIT falied\n");
    }

    task_create("tests", SCHED_PRIORITY_DEFAULT,
                10000,
                (main_t)run_tests,
                (FAR char * const *)NULL);


    return 1;
}
