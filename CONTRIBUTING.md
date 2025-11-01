# Contributing to ScaleChord

Thank you for your interest in contributing to ScaleChord! This document provides guidelines and instructions for contributing to the project.

## 🎯 Code of Conduct

This project adheres to the Contributor Covenant Code of Conduct. By participating, you are expected to uphold this code:
- Be respectful and inclusive
- Welcome diverse perspectives
- Report unacceptable behavior to maintainers
- Focus on constructive feedback

## 🚀 Getting Started

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 19+)
- CMake 3.16+
- Python 3.8+ (for testing scripts)

### Setup

1. **Fork the repository**
   ```bash
   # Go to https://github.com/thabanghutamo/production-works
   # Click "Fork" button
   ```

2. **Clone your fork**
   ```bash
   git clone https://github.com/YOUR_USERNAME/production-works.git
   cd production-works
   ```

3. **Add upstream remote**
   ```bash
   git remote add upstream https://github.com/thabanghutamo/production-works.git
   ```

4. **Build the project**
   ```bash
   cd plugin
   mkdir build && cd build
   cmake -DCMAKE_BUILD_TYPE=Release ..
   cmake --build . -j$(nproc)
   ```

5. **Run tests**
   ```bash
   ctest --output-on-failure
   ```

## 📋 Development Workflow

### Creating a Branch

```bash
# Update main branch
git checkout main
git pull upstream main

# Create feature branch
git checkout -b feature/your-feature-name
# or for bug fixes
git checkout -b fix/your-bug-fix-name
```

**Branch naming conventions:**
- `feature/description` - New features
- `fix/description` - Bug fixes
- `docs/description` - Documentation updates
- `refactor/description` - Code refactoring
- `perf/description` - Performance improvements

### Making Changes

1. **Write clear, focused commits**
   ```bash
   git commit -m "feat: add new feature

   - Detailed description of changes
   - Explanation of why this is needed
   - Any breaking changes noted"
   ```

2. **Keep commits atomic** - Each commit should be a single logical change

3. **Write descriptive commit messages**
   - First line: Present tense, imperative mood
   - 50 characters or less for subject
   - Blank line after subject
   - Detailed body (72 characters per line)

### Commit Message Format

```
<type>(<scope>): <subject>

<body>

<footer>
```

**Types:**
- `feat` - A new feature
- `fix` - A bug fix
- `docs` - Documentation changes
- `style` - Code style changes (formatting, semicolons, etc.)
- `refactor` - Code refactoring
- `perf` - Performance improvements
- `test` - Adding or updating tests
- `chore` - Build system, dependencies, etc.

**Scopes:**
- `core` - Core harmonic processing
- `ui` - User interface
- `midi` - MIDI processing
- `presets` - Preset system
- `juce` - JUCE integration
- `tests` - Test suite
- `docs` - Documentation
- `build` - Build system

### Code Quality Standards

#### C++ Code Style

- **Naming Conventions**
  ```cpp
  class MyClass { };           // PascalCase for classes
  void myFunction() { }         // camelCase for functions
  int myVariable;              // camelCase for variables
  const int MY_CONSTANT = 42;  // UPPER_CASE for constants
  ```

- **Formatting**
  ```cpp
  // 120 character line limit
  // 4 space indentation
  // Braces on same line for functions
  // Braces on new line for classes/structs
  
  if (condition) {
    // code
  } else {
    // code
  }
  ```

- **Comments**
  ```cpp
  // Single line comments for brief explanations
  
  /*
   * Multi-line comments for detailed explanations
   * or license headers
   */
  
  /// Documentation comments (Doxygen style)
  int getValue(); ///< Return the value
  ```

#### Real-Time Safety

All code in audio callback paths must be real-time safe:
- ❌ No `new` / `delete`
- ❌ No `std::string`, `std::vector`, `std::map` in callbacks
- ❌ No file I/O
- ❌ No system calls
- ✅ Pre-allocated memory only
- ✅ Stack allocations only
- ✅ Deterministic operations

#### Performance

- Keep the audio processing thread < 5% CPU usage
- Avoid allocations in audio callback
- Use efficient algorithms (O(n) preferred over O(n²))
- Profile before and after optimization changes

### Testing

#### Running Tests

```bash
cd plugin/build
ctest --output-on-failure
```

#### Writing Tests

1. **Create test file** in `plugin/tests/`
   ```cpp
   #include <cassert>
   #include "YourHeader.h"
   
   int main() {
     // Arrange
     MyClass obj;
     
     // Act
     int result = obj.compute();
     
     // Assert
     assert(result == expectedValue);
     
     return 0;
   }
   ```

2. **Add to CMakeLists.txt**
   ```cmake
   add_executable(test_feature tests/test_feature.cpp)
   target_link_libraries(test_feature scalechord_core)
   add_test(NAME feature_tests COMMAND test_feature)
   ```

3. **Test coverage requirements**
   - All public functions must be tested
   - Edge cases should be covered
   - Integration tests for module interactions

#### Test Guidelines

- Use descriptive test names: `test_chordAnalyzer_detectsMajorTriad`
- Test one thing per test case
- Use assertions for verification
- Mock external dependencies if needed

### Documentation

#### Updating Documentation

1. **API Documentation** - Update inline comments with Doxygen format:
   ```cpp
   /// Brief description
   /// @param param1 Description of param1
   /// @return Description of return value
   /// @throws Description of exceptions
   int myFunction(int param1);
   ```

2. **User Documentation** - Update `.md` files in root directory
   - RELEASE_*.md for user guides
   - DEVELOPMENT.md for developers
   - ARCHITECTURE.md for system design

3. **Comments** - Keep code comments up-to-date with changes

#### Documentation Standards

- Use clear, simple language
- Include code examples
- Document edge cases and limitations
- Keep examples working and tested

## 🔄 Submitting Changes

### Creating a Pull Request

1. **Push your branch**
   ```bash
   git push origin feature/your-feature-name
   ```

2. **Create pull request on GitHub**
   - Title: Clear, descriptive title
   - Description: Use PR template
   - Link related issues
   - Reference any relevant discussions

3. **PR Guidelines**
   - One feature per PR
   - Keep PRs focused and reasonably sized
   - Include tests for new functionality
   - Update documentation
   - Pass all checks (CI/CD, tests, formatting)

### Code Review Process

1. **Automated checks must pass**
   - GitHub Actions CI/CD
   - All tests passing
   - No compiler warnings

2. **Code review by maintainers**
   - Follow-up on feedback
   - Make requested changes
   - Re-request review

3. **Approval and merge**
   - At least one approval required
   - Squash and merge recommended
   - Delete branch after merge

### Review Checklist

Reviewers will check:
- ✅ Code quality and style
- ✅ Real-time safety (if applicable)
- ✅ Performance impact
- ✅ Test coverage
- ✅ Documentation updates
- ✅ No breaking changes (unless major version)

## 🐛 Reporting Bugs

### Before Reporting

- Check existing issues for duplicates
- Verify bug is reproducible
- Test with latest development version
- Gather system information

### How to Report

Use the Bug Report template on GitHub Issues with:
1. Clear description of the bug
2. Steps to reproduce
3. Expected behavior
4. Actual behavior
5. System information (OS, DAW, version)
6. Error messages or logs

## 💡 Suggesting Features

### Before Suggesting

- Check existing features and discussions
- Verify alignment with project goals
- Consider impact on existing features

### How to Suggest

Use the Feature Request template on GitHub Issues with:
1. Clear description of feature
2. Use case and benefits
3. Proposed implementation approach
4. Alternatives considered
5. Music/harmonic context if applicable

## 📚 Additional Resources

- [DEVELOPMENT.md](../plugin/DEVELOPMENT.md) - Development setup
- [ARCHITECTURE.md](../plugin/ARCHITECTURE.md) - System architecture
- [RELEASE_PARAMETER_REFERENCE.md](../RELEASE_PARAMETER_REFERENCE.md) - Parameter guide
- [CMakeLists.txt](../plugin/CMakeLists.txt) - Build configuration

## 📞 Questions?

- **GitHub Discussions** - General questions and discussions
- **GitHub Issues** - Bug reports and feature requests
- **Email** - [maintainer email if applicable]

## 🙏 Thank You!

Thank you for contributing to ScaleChord. Your efforts help make this project better for all musicians and producers!

---

**Last Updated**: November 1, 2025
**Version**: 1.0.0
