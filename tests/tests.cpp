#include "tests.hpp"

TEST(ArgumentParser, ParseArguments) {
    int argc = 3;
    char *argv[] = {
        (char *) "program",
        (char *) "--option",
        (char *) "value",
        NULL
    };

    static t_argo options[] = {
        {'v', "verbose", "verbose", "Enable verbose mode", NO_ARG},
        {'t', "test", "test", "Display a test", NO_ARG},
        {'o', "output", "output", "Specify output file", ONE_ARG},
        {'i', "input", "input", "Specify input file", INF_ARG},
        {0}
    };

    static t_argp argp = {
        .options = options,
        .args_doc = "INPUT_FILE OUTPUT_FILE",
        .doc = "This is a sample program to demonstrate how to use argp to parse command line arguments.",
    };

    t_list *head = parse_args(&argp, argc, argv);
    ASSERT_NE(head, nullptr);
    t_argr *argr;
    t_argo *argo;

    argr = get_next_option(head);
    ASSERT_NE(argr, nullptr);
    
    argo = argr->option;
    ASSERT_EQ(argo->sflag, 'o');
    ASSERT_STREQ(argo->lflag, "output");
}