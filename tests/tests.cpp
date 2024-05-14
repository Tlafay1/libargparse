#include "tests.hpp"

TEST(ArgumentParser, SingleOption)
{
    t_args *args;

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

    parse_args(&argp, argv, &args);
    ASSERT_NE(args->args, nullptr);
    ASSERT_NE(args->options, nullptr);
    ASSERT_NE(args->args_original, nullptr);
    ASSERT_NE(args->options_original, nullptr);

    t_argr *argr;

    argr = get_next_option(args);
    ASSERT_NE(argr, nullptr);

    ASSERT_EQ(argr->option->sflag, 'o');
    ASSERT_STREQ(argr->option->lflag, "output");
    ASSERT_STREQ(argr->values[0], "value");

    free_args(args);
}

TEST(ArgumentParser, SingleArgument)
{
    t_args *args;

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

    int ret = parse_args(&argp, argv, &args);
    ASSERT_NE(ret, 1);
    ASSERT_NE(args->args, nullptr);
    ASSERT_NE(args->options, nullptr);
    ASSERT_NE(args->args_original, nullptr);
    ASSERT_NE(args->options_original, nullptr);

    t_argr *argr;

    argr = get_next_arg(args);
    ASSERT_NE(argr, nullptr);

    ASSERT_STREQ(argr->values[0], "argument");

    free_args(args);
}

TEST(ArgumentParser, OptionThatDoesntExist)
{
    t_args *args;

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
    int ret = parse_args(&argp, argv, &args);
    ASSERT_NE(ret, 0);
    std::string output = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "program: unrecognized option '--nonexistent'\nTry 'program --help' for more information\n");

    free_args(args);
}

TEST(ArgumentParser, TestArgsCount)
{
    t_args *args;

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

    int ret = parse_args(&argp, argv, &args);
    ASSERT_EQ(ret, 0);
    ASSERT_EQ(args_count(args), 2);

    free_args(args);
}

TEST(ArgumentParser, TestOptionsCount)
{
    t_args *args;

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

    int ret = parse_args(&argp, argv, &args);
    ASSERT_EQ(ret, 0);
    ASSERT_EQ(options_count(args), 2);

    free_args(args);
}

TEST(ArgumentParser, ArgumentAfterNoArgOption)
{
    t_args *args;

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

    int ret = parse_args(&argp, argv, &args);
    ASSERT_NE(ret, 1);
    t_argr *argr;

    argr = get_next_arg(args);
    ASSERT_NE(argr, nullptr);

    ASSERT_STREQ(argr->values[0], "argument");
    ASSERT_EQ(get_next_arg(args), nullptr);

    free_args(args);
}

// TEST(ArgumentParser, FreeArgsIfListIsNull)
// {
//     t_args *args = nullptr;
//     free_args(args);
//     free_args(a_list);
//     ASSERT_EQ(a_list, nullptr);
// }