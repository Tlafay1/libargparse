#include "tests.hpp"

TEST(ArgumentParser, SingleOption)
{
    t_list *a_list = nullptr;
    t_list *o_list = nullptr;

    const char *argv[] = {
        "program",
        "--output",
        "value",
        NULL};

    static t_argo options[] = {
        {'v', "verbose", "verbose", "Enable verbose mode", NO_ARG},
        {'t', "test", "test", "Display a test", NO_ARG},
        {'o', "output", "output", "Specify output file", ONE_ARG},
        {'i', "input", "input", "Specify input file", INF_ARG},
        {0, nullptr, nullptr, nullptr, NO_ARG}};

    static t_argp argp = {
        .options = options,
        .args_doc = "INPUT_FILE OUTPUT_FILE",
        .doc = "This is a sample program to demonstrate how to use argp to parse command line arguments.",
    };

    parse_args(&argp, argv, &a_list, &o_list);
    ASSERT_NE(a_list, nullptr);
    ASSERT_NE(o_list, nullptr);

    t_argr *argr;

    argr = get_next_option(&o_list);
    ASSERT_NE(argr, nullptr);

    ASSERT_EQ(argr->option->sflag, 'o');
    ASSERT_STREQ(argr->option->lflag, "output");
    ASSERT_STREQ(argr->values[0], "value");
}

TEST(ArgumentParser, SingleArgument)
{
    t_list *a_list = nullptr;
    t_list *o_list = nullptr;

    const char *argv[] = {
        "program",
        "argument",
        NULL};

    static t_argo options[] = {{0, nullptr, nullptr, nullptr, NO_ARG}};

    static t_argp argp = {
        .options = options,
        .args_doc = "INPUT_FILE OUTPUT_FILE",
        .doc = "This is a sample program to demonstrate how to use argp to parse command line arguments.",
    };

    parse_args(&argp, argv, &a_list, &o_list);
    ASSERT_NE(a_list, nullptr);
    ASSERT_NE(o_list, nullptr);
    t_argr *argr;

    argr = get_next_arg(&a_list);
    ASSERT_NE(argr, nullptr);

    ASSERT_STREQ(argr->values[0], "argument");
}

TEST(ArgumentParser, OptionThatDoesntExist)
{
    t_list *a_list = nullptr;
    t_list *o_list = nullptr;

    const char *argv[] = {
        "program",
        "--nonexistent",
        "value",
        NULL};

    static t_argo options[] = {{0, nullptr, nullptr, nullptr, NO_ARG}};

    static t_argp argp = {
        .options = options,
        .args_doc = "INPUT_FILE OUTPUT_FILE",
        .doc = "This is a sample program to demonstrate how to use argp to parse command line arguments.",
    };

    ::testing::internal::CaptureStdout();
    parse_args(&argp, argv, &a_list, &o_list);
    std::string output = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "program: unrecognized option '--nonexistent'\nTry 'program --help' for more information\n");
    ASSERT_EQ(a_list, nullptr);
    ASSERT_EQ(o_list, nullptr);
}

TEST(ArgumentParser, TestArgsCount)
{
    t_list *a_list = nullptr;
    t_list *o_list = nullptr;

    const char *argv[] = {
        "program",
        "--output",
        "value",
        "argument1",
        "argument2",
        NULL};

    static t_argo options[] = {
        {'v', "verbose", "verbose", "Enable verbose mode", NO_ARG},
        {'t', "test", "test", "Display a test", NO_ARG},
        {'o', "output", "output", "Specify output file", ONE_ARG},
        {'i', "input", "input", "Specify input file", INF_ARG},
        {0, nullptr, nullptr, nullptr, NO_ARG}};

    static t_argp argp = {
        .options = options,
        .args_doc = "INPUT_FILE OUTPUT_FILE",
        .doc = "This is a sample program to demonstrate how to use argp to parse command line arguments.",
    };

    parse_args(&argp, argv, &a_list, &o_list);
    ASSERT_EQ(args_count(a_list), 2);
}

TEST(ArgumentParser, TestOptionsCount)
{
    t_list *a_list = nullptr;
    t_list *o_list = nullptr;

    const char *argv[] = {
        "program",
        "--output",
        "value",
        "argument1",
        "--test",
        "argument2",
        NULL};

    static t_argo options[] = {
        {'v', "verbose", "verbose", "Enable verbose mode", NO_ARG},
        {'t', "test", "test", "Display a test", NO_ARG},
        {'o', "output", "output", "Specify output file", ONE_ARG},
        {'i', "input", "input", "Specify input file", INF_ARG},
        {0, nullptr, nullptr, nullptr, NO_ARG}};

    static t_argp argp = {
        .options = options,
        .args_doc = "INPUT_FILE OUTPUT_FILE",
        .doc = "This is a sample program to demonstrate how to use argp to parse command line arguments.",
    };

    parse_args(&argp, argv, &a_list, &o_list);
    ASSERT_EQ(options_count(o_list), 2);
}

TEST(ArgumentParser, ArgumentAfterNoArgOption)
{
    t_list *a_list = nullptr;
    t_list *o_list = nullptr;

    static t_argo options[] = {
        {'v', "verbose", "verbose", "Enable verbose mode", NO_ARG},
        {0, nullptr, nullptr, nullptr, NO_ARG}};

    static t_argp argp = {
        .options = options,
        .args_doc = "INPUT_FILE OUTPUT_FILE",
        .doc = "This is a sample program to demonstrate how to use argp to parse command line arguments.",
    };

    const char *argv[] = {
        "program",
        "-v",
        "argument",
        NULL};

    parse_args(&argp, argv, &a_list, &o_list);
    ASSERT_NE(a_list, nullptr);
    t_argr *argr;

    argr = get_next_arg(&a_list);
    ASSERT_NE(argr, nullptr);

    ASSERT_STREQ(argr->values[0], "argument");
    ASSERT_EQ(get_next_arg(&a_list), nullptr);
}