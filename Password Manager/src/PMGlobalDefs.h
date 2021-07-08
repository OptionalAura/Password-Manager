#pragma once
#ifndef PMGLOBAL
#define PMGLOBAL
#ifdef PM_DEBUG
#include <iostream>
//Prints X to the console in debug mode, does nothing in release. (Current: Debug)
#define LOG(x) std::cout << x << std::endl
#else
//Prints X to the console in debug mode, does nothing in release. (Current: Release)
#define LOG(x)
#endif
#endif