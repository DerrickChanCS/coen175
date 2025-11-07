# COEN 175 - Simple C Compiler

This is the lab component to Santa Clara University's compilers class, implementing a complete compiler for **Simple C** (a subset of the C programming language) through six progressive phases.

## Overview

The Simple C compiler is built incrementally, with each phase adding new functionality while building upon previous work. The final result is a complete compiler that translates Simple C source code into x86 assembly language.

**Language Features Supported:**
- Basic data types (int, char, void) with pointer support
- Variables and arrays
- Functions with parameters and return values
- Structures
- Control flow (if/else, while, for, switch/case, break, continue, return)
- Expressions with standard C operators
- Comments and string literals

## Phase Breakdown

### Phase 1: Lexical Analysis (Tokenization)
**Purpose:** Convert source code into a stream of tokens

**Key Components:**
- `lexer.l` - Flex specification file defining token patterns
- `lexer.cpp` - Generated lexical analyzer (created from lexer.l)
- Token definitions for keywords, identifiers, operators, literals

**What it does:**
- Recognizes C keywords (int, char, if, while, etc.)
- Identifies operators (+, -, *, /, ==, !=, etc.)
- Handles string and character literals
- Processes comments (ignores them)
- Reports lexical errors

**Input:** Simple C source code  
**Output:** Stream of classified tokens

### Phase 2: Enhanced Lexical Analysis
**Purpose:** Improved tokenization with better error handling

**Enhancements from Phase 1:**
- More robust token classification
- Better error reporting mechanisms
- Enhanced handling of edge cases
- Improved comment processing

**Key Files:**
- Enhanced `lexer.l` and `lexer.h`
- `lexmain.cpp` - Main driver for standalone lexer testing

### Phase 3: Syntax Analysis + Basic Semantics
**Purpose:** Parse tokens into Abstract Syntax Tree and perform basic semantic analysis

**New Components:**
- `parser.cpp` - Recursive descent parser implementing Simple C grammar
- `scope.cpp/.h` - Symbol table and scope management
- `symbol.cpp/.h` - Symbol representation (variables, functions)
- `type.cpp/.h` - Type system implementation
- `tokens.h` - Token type definitions

**What it adds:**
- **Syntax Analysis:** Parses token stream according to Simple C grammar
- **Symbol Tables:** Tracks variables and functions with proper scoping
- **Type Checking:** Basic type compatibility checking
- **AST Construction:** Builds tree representation of program structure
- **Scope Management:** Handles nested scopes (global, function, block)

**Input:** Token stream  
**Output:** Abstract Syntax Tree + Symbol tables

### Phase 4: Semantic Analysis
**Purpose:** Complete semantic checking and validation

**New Components:**
- `checker.cpp/.h` - Comprehensive semantic checker
- Enhanced symbol and type management
- Function signature validation

**What it adds:**
- **Type Checking:** Full type compatibility verification
- **Declaration/Definition Checking:** Ensures variables/functions are properly declared
- **Function Call Validation:** Parameter count and type checking
- **Scope Resolution:** Proper variable lookup in nested scopes
- **Error Reporting:** Detailed semantic error messages

**Enhanced from Phase 3:**
- More rigorous type system
- Better error detection and reporting
- Function overloading restrictions
- Array and pointer type checking

### Phase 5: Intermediate Code Generation
**Purpose:** Generate intermediate representation and prepare for code generation

**New Components:**
- `Tree.cpp/.h` - Complete AST node implementations
- `generate.cpp` - Initial code generation framework
- Expression evaluation system

**What it adds:**
- **Tree Construction:** Full AST with all node types (expressions, statements)
- **Expression Evaluation:** Handling arithmetic, logical, and assignment expressions
- **Statement Processing:** Control flow statement representation
- **Storage Planning:** Preparation for memory allocation

**Architecture:**
- Object-oriented AST design
- Visitor pattern for tree traversal
- Separation of parsing and code generation concerns

### Phase 6: Target Code Generation (Complete Compiler)
**Purpose:** Generate executable x86 assembly code

**New Components:**
- `generator.cpp/.h` - Complete x86 code generator
- `allocator.cpp` - Memory allocation and stack management
- `Label.cpp/.h` - Label management for control flow
- `machine.h` - Target machine definitions

**What it adds:**
- **Assembly Generation:** Translates AST to x86 assembly instructions
- **Storage Allocation:** Manages stack frames and variable storage
- **Register Management:** Efficient use of CPU registers
- **Control Flow:** Proper handling of jumps, loops, and conditionals
- **Function Calls:** Stack management for function invocations
- **Memory Management:** Proper addressing for variables and arrays

**Output:** Complete x86 assembly code that can be assembled and executed

## Build System

Each phase includes a `Makefile` with appropriate dependencies:

```bash
# Build the compiler for any phase
make

# Clean build artifacts
make clean

# Remove all generated files
make clobber
```

## Testing

Each phase includes comprehensive test suites:
- `tests/` directory with Simple C test programs
- Expected output files (`.out` extension)
- Testing scripts (`tester.sh`, `test.sh`)

## Project Evolution

| Phase | Input | Output | Key Addition |
|-------|--------|--------|--------------|
| 1 | Source Code | Tokens | Lexical Analysis |
| 2 | Source Code | Tokens | Enhanced Tokenization |
| 3 | Source Code | AST + Symbols | Parsing + Symbol Tables |
| 4 | Source Code | Validated AST | Semantic Checking |
| 5 | Source Code | IR Trees | Intermediate Representation |
| 6 | Source Code | x86 Assembly | Complete Code Generation |

The progression demonstrates how compilers work in practice, building complexity gradually while maintaining clean interfaces between phases.
