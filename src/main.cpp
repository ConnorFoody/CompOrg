#ifndef __MAIN__
#define __MAIN__

#include <gtest/gtest.h>
#include <iostream>
int main(int argc, char** argv){
	// filter flag controls which tests get run
	// see code.google.com/p/googletest/wiki/AdvancedGuide#Selecting_Tests for more detail
	// you can type "DISABLED_" before a test name to disable it
	::testing::GTEST_FLAG(filter) = "TestHW2.*";

	::testing::GTEST_FLAG(print_time) = false;

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

#endif