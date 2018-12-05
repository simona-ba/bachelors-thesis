#pragma once

# ifdef GLW_STATIC
#   define GLW_API
# elif GLW_EXPORT
#   define GLW_API __declspec(dllexport)
# else
#   define GLW_API __declspec(dllimport)
# endif

//! OpenGL wrapper library namespace. Provides simplified interface for using OpenGL features.
namespace glw {}
