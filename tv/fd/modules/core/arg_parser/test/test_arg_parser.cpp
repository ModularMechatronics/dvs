#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "core/arg_parser.h"

TEST(TestArgParser, TestNoLeadingDash)
{
    int argc = 4;
    const char* argv0[] = {"/a_path/to/executable", "--a-flag", "--p=512", "q"};
    const char* argv1[] = {"/a_path/to/executable", "--a-flag", "--p=512", "-qa"};

    ArgumentOptions opts0, opts1;

    opts0.addFlagArg("a-flag", "A random flag")
    .addFlagArg("p", "Something")
    .addFlagArg("q", "Something");

    opts0.addFlagArg("a-flag", "A random flag")
    .addFlagArg("p", "Something")
    .addFlagArg("qa", "Something");

    try
    {
        ArgParser arg_parser(opts0, argc, argv0);
        FAIL() << "Expected std::exception to be thrown";
    }
    catch(std::exception const & err)
    {
        EXPECT_EQ(err.what(), std::string("Argument too short: q"));
    }

    try
    {
        ArgParser arg_parser(opts1, argc, argv1);
        FAIL() << "Expected std::exception to be thrown";
    }
    catch(std::exception const & err)
    {
        EXPECT_EQ(err.what(), std::string("Argument should be provided with leading \"--\", got: -qa"));
    }
}

TEST(TestArgParser, TestArgNotPresent)
{
    int argc = 2;
    const char* argv[] = {"/a_path/to/executable", "--a-flag"};

    ArgumentOptions opts;

    opts.addFlagArg("a-flag", "A random flag")
    .addValuedArg("path", "A path");

    try
    {
        ArgParser arg_parser(opts, argc, argv);
        FAIL() << "Expected std::exception to be thrown";
    }
    catch(std::exception const & err)
    {
        EXPECT_EQ(err.what(), std::string("Required argument \"path\" was not provided!"));
    }
}

TEST(TestArgParser, TestBasic)
{
    int argc = 8;
    const char* argv[] = {"/path/to/executable", "--a-flag", "--path=a_path", "--path2=a_path2",
        "--parameter=3", "--float-parameter=0.34", "--empty-arg=", "--bool-arg=true"};

    ArgumentOptions opts;

    opts.addFlagArg("a-flag", "A random flag")
    .addFlagArg("a-non-present-flag", "A random flag")
    .addValuedArg("path", "A path")
    .addValuedArg("path2", "A path")
    .addValuedArg("a-non-present-path", "A path", OptionNecessity::kOptional)
    .addValuedArg("parameter", "A parameter")
    .addValuedArg("empty-arg", "Empty arg")
    .addValuedArg("bool-arg", "Bool arg")
    .addValuedArg("float-parameter", "Float arg");

    ArgParser const arg_parser(opts, argc, argv);
    ASSERT_TRUE(arg_parser.hasFlag("a-flag"));
    ASSERT_FALSE(arg_parser.hasFlag("a-non-present-flag"));

    ASSERT_EQ(arg_parser["path"].valueOr<std::string>("default_path"), "a_path");
    ASSERT_EQ(arg_parser["path2"].valueOr<std::string>("default_path"), "a_path2");
    ASSERT_EQ(arg_parser["a-non-present-path"].valueOr<std::string>("default_path"), "default_path");
    ASSERT_EQ(arg_parser["parameter"].valueOr<float>(4), 3);
    ASSERT_EQ(arg_parser["empty-arg"].valueOr<std::string>("default_value"), "");
    ASSERT_EQ(arg_parser["bool-arg"].valueOr<bool>(false), true);
    ASSERT_EQ(arg_parser["float-parameter"].valueOr<float>(2.34f), 0.34f);
}

TEST(TestArgParser, TestUnsolicitedArgument)
{
    int argc = 4;
    const char* argv[] = {"/a_path/to/executable", "--a-flag", "--path=a_path", "--unsolicited-arg=a_path"};

    ArgumentOptions opts;

    opts.addFlagArg("a-flag", "A random flag")
    .addValuedArg("path", "A path");

    try
    {
        ArgParser arg_parser(opts, argc, argv);
        FAIL() << "Expected std::exception to be thrown";
    }
    catch(std::exception const & err)
    {
        EXPECT_EQ(err.what(), std::string("Provided argument \"--unsolicited-arg=a_path\" was not defined in the argument options!"));
    }
}

TEST(TestArgParser, TestIncorrectForm)
{
    int argc = 3;
    const char* argv0[] = {"/a_path/to/executable", "--a-flag=true", "--path=a_path"};
    const char* argv1[] = {"/a_path/to/executable", "--a-flag", "--path"};

    ArgumentOptions opts;

    opts.addFlagArg("a-flag", "A random flag")
    .addValuedArg("path", "A path");

    try
    {
        ArgParser arg_parser(opts, argc, argv0);
        FAIL() << "Expected std::exception to be thrown";
    }
    catch(std::exception const & err)
    {
        EXPECT_EQ(err.what(), std::string("Provided argument \"--a-flag=true\" is supplied as a valued argument, but the corresponding option is specified to be a flag argument!"));
    }

    try
    {
        ArgParser arg_parser(opts, argc, argv1);
        FAIL() << "Expected std::exception to be thrown";
    }
    catch(std::exception const & err)
    {
        EXPECT_EQ(err.what(), std::string("Provided argument \"--path\" is supplied as a flag argument, but the corresponding option is specified to be a valued argument!"));
    }
}

TEST(TestArgParser, TestNecessity)
{
    int argc0 = 2;
    int argc1 = 3;
    const char* argv0[] = {"/a_path/to/executable", "--path=a_path"};
    const char* argv1[] = {"/a_path/to/executable", "--path=a_path", "--path2=a_path"};

    ArgumentOptions opts;

    opts.addValuedArg("path", "A path", OptionNecessity::kRequired)
    .addValuedArg("path2", "A path", OptionNecessity::kOptional);
    
    ArgParser arg_parser0(opts, argc0, argv0);
    ArgParser arg_parser1(opts, argc1, argv1);

}

TEST(TestArgParser, TestPrintHelp)
{
    // TODO
    // std::ostringstream local_stream;

    int argc = 3;
    const char* argv[] = {"/path/to/executable", "--a-flag", "--path=a_path"};

    ArgumentOptions opts;

    opts.addFlagArg("a-flag", "A random flag")
    .addValuedArg("path", "A path");

    ArgParser arg_parser(opts, argc, argv);

    arg_parser.printHelp();
}
