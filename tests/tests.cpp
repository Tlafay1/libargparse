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
    ASSERT_EQ(ret, 0);
    t_argr *argr;

    argr = get_next_arg(args);
    ASSERT_NE(argr, nullptr);

    ASSERT_STREQ(argr->values[0], "argument");
    ASSERT_EQ(get_next_arg(args), nullptr);

    free_args(args);
}

TEST(ArgumentParser, LflagNullDoesNotExist)
{
    t_args *args;

    static t_argo options[] = {
        {'v', NULL, "verbose", "verbose output", NO_ARG},
        {'?', NULL, "help", "print help and exit", NO_ARG},
        {0, nullptr, nullptr, nullptr, NO_ARG}};

    static t_argp argp = {
        .options = options,
        .args_doc = "[options] <destination>",
        .doc = ""};

    const char *argv[] = {
        "program",
        "--iweufhwe",
        NULL};

    ::testing::internal::CaptureStdout();
    int ret = parse_args(&argp, argv, &args);
    ASSERT_NE(ret, 0);
    std::string output = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "program: unrecognized option '--iweufhwe'\nTry 'program --help' for more information\n");
}

TEST(ArgumentParser, LflagNullExistsWithLflag)
{
    t_args *args;

    static t_argo options[] = {
        {'v', NULL, "verbose", "verbose output", NO_ARG},
        {'?', NULL, "help", "print help and exit", NO_ARG},
        {0, nullptr, nullptr, nullptr, NO_ARG}};

    static t_argp argp = {
        .options = options,
        .args_doc = "[options] <destination>",
        .doc = ""};

    const char *argv[] = {
        "program",
        "--verbose",
        NULL};

    ::testing::internal::CaptureStdout();
    int ret = parse_args(&argp, argv, &args);
    ASSERT_NE(ret, 0);
    std::string output = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "program: unrecognized option '--verbose'\nTry 'program --help' for more information\n");
}

TEST(ArgumentParser, LflagNullExistsWithSflag)
{
    t_args *args;

    static t_argo options[] = {
        {'v', NULL, "verbose", "verbose output", NO_ARG},
        {'?', NULL, "help", "print help and exit", NO_ARG},
        {0, nullptr, nullptr, nullptr, NO_ARG}};

    static t_argp argp = {
        .options = options,
        .args_doc = "[options] <destination>",
        .doc = ""};

    const char *argv[] = {
        "program",
        "-v",
        NULL};

    int ret = parse_args(&argp, argv, &args);
    ASSERT_EQ(ret, 0);

    t_argr *argr;

    argr = get_next_option(args);
    ASSERT_NE(argr, nullptr);

    ASSERT_EQ(argr->option->sflag, 'v');
    ASSERT_EQ(argr->option->lflag, nullptr);

    free_args(args);
}

TEST(ArgumentParser, SflagNullDoesNotExist)
{
    t_args *args;

    static t_argo options[] = {
        {0, "verbose", "verbose", "verbose output", NO_ARG},
        {0, "help", "help", "print help and exit", NO_ARG},
        {0, nullptr, nullptr, nullptr, NO_ARG}};

    static t_argp argp = {
        .options = options,
        .args_doc = "[options] <destination>",
        .doc = ""};

    const char *argv[] = {
        "program",
        "-x",
        NULL};

    ::testing::internal::CaptureStdout();
    int ret = parse_args(&argp, argv, &args);
    ASSERT_NE(ret, 0);
    std::string output = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "program: invalid option -- 'x'\nTry 'program --help' for more information\n");
}

TEST(ArgumentParser, SflagNullExistsWithLflag)
{
    t_args *args;

    static t_argo options[] = {
        {0, "verbose", "verbose", "verbose output", NO_ARG},
        {0, "help", "help", "print help and exit", NO_ARG},
        {0, nullptr, nullptr, nullptr, NO_ARG}};

    static t_argp argp = {
        .options = options,
        .args_doc = "[options] <destination>",
        .doc = ""};

    const char *argv[] = {
        "program",
        "--verbose",
        NULL};

    int ret = parse_args(&argp, argv, &args);
    ASSERT_EQ(ret, 0);

    t_argr *argr;

    argr = get_next_option(args);
    ASSERT_NE(argr, nullptr);

    ASSERT_EQ(argr->option->sflag, 0);
    ASSERT_STREQ(argr->option->lflag, "verbose");

    free_args(args);
}

TEST(ArgumentParser, SflagNullExistsWithSflag)
{
    t_args *args;

    static t_argo options[] = {
        {0, "verbose", "verbose", "verbose output", NO_ARG},
        {0, "help", "help", "print help and exit", NO_ARG},
        {0, nullptr, nullptr, nullptr, NO_ARG}};

    static t_argp argp = {
        .options = options,
        .args_doc = "[options] <destination>",
        .doc = ""};

    const char *argv[] = {
        "program",
        "-v",
        NULL};

    ::testing::internal::CaptureStdout();
    int ret = parse_args(&argp, argv, &args);
    ASSERT_NE(ret, 0);
    std::string output = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "program: invalid option -- 'v'\nTry 'program --help' for more information\n");
}

TEST(ArgumentParser, LflagwithEqualOneArg)
{
    t_args *args;

    static t_argo options[] = {
        {'v', "verbose", "verbose", "verbose output", NO_ARG},
        {'s', "size", "size", "size", ONE_ARG},
        {'?', "help", "help", "print help and exit", NO_ARG},
        {0, nullptr, nullptr, nullptr, NO_ARG}};

    static t_argp argp = {
        .options = options,
        .args_doc = "[options] <destination>",
        .doc = ""};

    const char *argv[] = {
        "program",
        "--size=10",
        NULL};

    int ret = parse_args(&argp, argv, &args);
    ASSERT_EQ(ret, 0);

    t_argr *argr;

    argr = get_next_option(args);
    ASSERT_NE(argr, nullptr);

    ASSERT_STREQ(argr->option->lflag, "size");

    ASSERT_STREQ(argr->values[0], "10");

    free_args(args);
}

TEST(ArgumentParser, LflagwithEqualInfArg)
{
    t_args *args;

    static t_argo options[] = {
        {'v', "verbose", "verbose", "verbose output", NO_ARG},
        {'s', "size", "size", "size", INF_ARG},
        {'?', "help", "help", "print help and exit", NO_ARG},
        {0, nullptr, nullptr, nullptr, NO_ARG}};

    static t_argp argp = {
        .options = options,
        .args_doc = "[options] <destination>",
        .doc = ""};

    const char *argv[] = {
        "program",
        "--size=10",
        NULL};

    int ret = parse_args(&argp, argv, &args);
    ASSERT_EQ(ret, 0);

    t_argr *argr;

    argr = get_next_option(args);
    ASSERT_NE(argr, nullptr);

    ASSERT_STREQ(argr->option->lflag, "size");

    ASSERT_STREQ(argr->values[0], "10");

    free_args(args);
}

TEST(ArgumentParser, LflagWithEqualNoArgs)
{
    t_args *args;

    static t_argo options[] = {
        {'v', "verbose", "verbose", "verbose output", NO_ARG},
        {'s', "size", "size", "size of the file", NO_ARG},
        {'?', "help", "help", "print help and exit", NO_ARG},
        {0, nullptr, nullptr, nullptr, NO_ARG}};

    static t_argp argp = {
        .options = options,
        .args_doc = "[options] <destination>",
        .doc = ""};

    const char *argv[] = {
        "program",
        "--size=12",
        NULL};

    ::testing::internal::CaptureStdout();
    int ret = parse_args(&argp, argv, &args);
    ASSERT_NE(ret, 0);
    std::string output = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "program: option 'size' doesn't allow an argument\nTry 'program --help' for more information\n");
}

TEST(ArgumentParser, LflagWithEqualNoArgsEmptyValue)
{
    t_args *args;

    static t_argo options[] = {
        {'v', "verbose", "verbose", "verbose output", NO_ARG},
        {'s', "size", "size", "size of the file", NO_ARG},
        {'?', "help", "help", "print help and exit", NO_ARG},
        {0, nullptr, nullptr, nullptr, NO_ARG}};

    static t_argp argp = {
        .options = options,
        .args_doc = "[options] <destination>",
        .doc = ""};

    const char *argv[] = {
        "program",
        "--size=",
        NULL};

    ::testing::internal::CaptureStdout();
    int ret = parse_args(&argp, argv, &args);
    ASSERT_NE(ret, 0);
    std::string output = ::testing::internal::GetCapturedStdout();
    ASSERT_EQ(output, "program: option 'size' doesn't allow an argument\nTry 'program --help' for more information\n");
}

TEST(ArgumentParser, NegativeNumberInsteadOfSflag)
{
    t_args *args;

    static t_argo options[] = {
        {'v', "verbose", "verbose", "verbose output", NO_ARG},
        {'s', "size", "size", "size of the file", ONE_ARG},
        {'?', "help", "help", "print help and exit", NO_ARG},
        {0, nullptr, nullptr, nullptr, NO_ARG}};

    static t_argp argp = {
        .options = options,
        .args_doc = "[options] <destination>",
        .doc = ""};

    const char *argv[] = {
        "program",
        "-s",
        "-10",
        NULL};

    int ret = parse_args(&argp, argv, &args);
    ASSERT_EQ(ret, 0);

    t_argr *argr;

    argr = get_next_option(args);
    ASSERT_NE(argr, nullptr);

    ASSERT_STREQ(argr->option->lflag, "size");
    ASSERT_STREQ(argr->values[0], "-10");

    free_args(args);
}

TEST(ArgumentParser, SflagWithOneArgNotProvided)
{
    t_args *args;

    static t_argo options[] = {
        {'s', "size", "size", "size of the file", ONE_ARG},
        {0, nullptr, nullptr, nullptr, NO_ARG}};

    static t_argp argp = {
        .options = options,
        .args_doc = "[options] <destination>",
        .doc = ""};

    const char *argv[] = {
        "program",
        "-s",
        NULL};

    int ret = parse_args(&argp, argv, &args);
    ASSERT_NE(ret, 0);
}

TEST(ArgumentParser, LflagWithOneArgNotProvided)
{
    t_args *args;

    static t_argo options[] = {
        {'s', "size", "size", "size of the file", ONE_ARG},
        {0, nullptr, nullptr, nullptr, NO_ARG}};

    static t_argp argp = {
        .options = options,
        .args_doc = "[options] <destination>",
        .doc = ""};

    const char *argv[] = {
        "program",
        "--size",
        NULL};

    int ret = parse_args(&argp, argv, &args);
    ASSERT_NE(ret, 0);
}

TEST(LeaksCheck, InvalidSflag)
{
    t_args *args;

    static t_argo options[] = {
        {'c', "count", "count", "stop after <count> replies", ONE_ARG},
        {'i', "interval", "interval", "wait NUMBER seconds between sending each packet", ONE_ARG},
        {'q', "quiet", "quiet", "quiet output", NO_ARG},
        {'s', "size", "data size", "use <size> as number of data bytes to be sent", ONE_ARG},
        {'t', "ttl", "time to live", "define time to live", ONE_ARG},
        {'v', "verbose", "verbose", "verbose output", NO_ARG},
        {'?', "help", "help", "print help and exit", NO_ARG},
        {0, NULL, NULL, NULL, NO_ARG}};

    static t_argp argp = {
        .options = options,
        .args_doc = "[options] <destination>",
        .doc = ""};

    const char *argv[] = {
        "./program",
        "-va",
        NULL};

    ::testing::internal::CaptureStdout();
    int ret = parse_args(&argp, argv, &args);
    ::testing::internal::GetCapturedStdout();
    ASSERT_NE(ret, 0);
}

TEST(LeaksCheck, InvalidLflag)
{
    t_args *args;

    static t_argo options[] = {
        {'c', "count", "count", "stop after <count> replies", ONE_ARG},
        {'i', "interval", "interval", "wait NUMBER seconds between sending each packet", ONE_ARG},
        {'q', "quiet", "quiet", "quiet output", NO_ARG},
        {'s', "size", "data size", "use <size> as number of data bytes to be sent", ONE_ARG},
        {'t', "ttl", "time to live", "define time to live", ONE_ARG},
        {'v', "verbose", "verbose", "verbose output", NO_ARG},
        {'?', "help", "help", "print help and exit", NO_ARG},
        {0, NULL, NULL, NULL, NO_ARG}};

    static t_argp argp = {
        .options = options,
        .args_doc = "[options] <destination>",
        .doc = ""};

    const char *argv[] = {
        "./program",
        "--verbosea",
        NULL};

    ::testing::internal::CaptureStdout();
    int ret = parse_args(&argp, argv, &args);
    ::testing::internal::GetCapturedStdout();
    ASSERT_NE(ret, 0);
}

TEST(LeaksCheck, PingTest)
{
    t_args *args;

    static t_argo options[] = {
        {'c', "count", "count", "stop after <count> replies", ONE_ARG},
        {'i', "interval", "interval", "wait NUMBER seconds between sending each packet", ONE_ARG},
        {'q', "quiet", "quiet", "quiet output", NO_ARG},
        {'s', "size", "data size", "use <size> as number of data bytes to be sent", ONE_ARG},
        {'t', "ttl", "time to live", "define time to live", ONE_ARG},
        {'v', "verbose", "verbose", "verbose output", NO_ARG},
        {'?', "help", "help", "print help and exit", NO_ARG},
        {0, NULL, NULL, NULL, NO_ARG}};

    static t_argp argp = {
        .options = options,
        .args_doc = "[options] <destination>",
        .doc = ""};

    const char *argv[] = {
        "./program",
        "localhost",
        "-c",
        "4",
        "-va",
        "--size=64",
        NULL};

    ::testing::internal::CaptureStdout();
    int ret = parse_args(&argp, argv, &args);
    ::testing::internal::GetCapturedStdout();
    ASSERT_NE(ret, 0);
}