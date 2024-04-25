#include "tests.hpp"

TEST(ArgumentParser, SingleOption)
{
    t_list *head = nullptr;

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

    parse_args(&argp, argv, &head);
    ASSERT_NE(head, nullptr);
    t_argr *argr;

    argr = get_next_option(&head);
    ASSERT_NE(argr, nullptr);

    ASSERT_EQ(argr->option->sflag, 'o');
    ASSERT_STREQ(argr->option->lflag, "output");
    ASSERT_STREQ(argr->values[0], "value");
}

TEST(ArgumentParser, SingleArgument)
{
    t_list *head = nullptr;

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

    parse_args(&argp, argv, &head);
    ASSERT_NE(head, nullptr);
    t_argr *argr;

    argr = get_next_arg(&head);
    ASSERT_NE(argr, nullptr);

    ASSERT_STREQ(argr->values[0], "argument");
}

TEST(ArgumentParser, OptionThatDoesntExist)
{
    t_list *head = nullptr;

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
    parse_args(&argp, argv, &head);
    std::string output = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "program: unrecognized option '--nonexistent'\nTry 'program --help' for more information\n");
    ASSERT_EQ(head, nullptr);
}