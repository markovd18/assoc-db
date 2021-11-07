/**
 * Created by <a href='mailto:markovd@students.zcu.cz'>David Markov</a> on 7.11.21.
 */

#pragma once
 /// If we are on older compiler (eg. gcc-8), there is not a <filesystem> header defined.
 /// It is in experimental state yet, so we need to include it from <experimental/filesystem>
#if (__has_include(<filesystem>))
#include <filesystem>
namespace filesystem = std::filesystem;
#else
#include <experimental/filesystem>
namespace filesystem = std::experimental::filesystem;
#endif