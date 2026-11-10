# Useful C++ Experiments

A collection of small Windows/C++ learning projects: cursor APIs, keyboard state, process output, filesystem traversal and parsing. Each directory is a separate example. Original directory names are retained so older links still work.

The collection now has one C++17 CMake build, a source index and explicit launch modes for examples that interact with the desktop. Generated Visual Studio caches and build outputs are excluded from version control.

## Build

Windows, CMake 3.16+ and a C++17 compiler:

```sh
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Executables appear in `build/` or `build/Release/`, depending on the generator. The combined build is checked with MinGW-w64. The original Visual Studio solutions remain alongside each example.

## Explore the examples

| Example | Target / usage | What it demonstrates |
| --- | --- | --- |
| [Cout to File](Cout%20to%20File/Project2/main.cpp) | `cout-to-file OUTPUT` | Scoped `std::cout` redirection and restoration; overwrites the specified output |
| [Cursor Capture and Replay](Cursor%20Capture%20and%20Replay/Next/main.cpp) | `cursor-replay --record` or `--replay` | 100 cursor samples at 10 ms intervals; replay explicitly moves the pointer |
| [Cursor Surveillance](Cursor_Surveilance/Cursor_Surveilance) | `cursor-surveillance --once` or `--watch` | Read-only primary/secondary monitor classification |
| [Hide Cursor](Hide%20Windows%20Cursor%20and%20bring%20it%20Back/Project1/maincpp.cpp) | `hide-cursor --hide` | Temporary system arrow replacement with restoration after 15 seconds or Ctrl+C |
| [Command Output Buffer](ISystemCommandCmdBuffer/ISystemCommandCmdBuffer) | `command-buffer NEW_OUTPUT_FILE` | Interface-based capture of a fixed echo command; removes the temporary file |
| [Key Sequence](KeyToggle%20KeySequenceBuffer/KeyToggle) | `key-sequence --watch` | Press-edge detection for Ctrl, R, I in order; Escape exits |
| [Filesystem Scan](Multithreaded%20Filesystem%20Operator/Project2) | `filesystem-scan --help` | Chosen-directory traversal and complete worker partitioning |
| [Pipe Capture](Popen/Popen/main.cpp) | `pipe-capture` | Read a fixed child command's output as data and check its exit status |
| [AES Block Demo](Test%20AES/Test%20AES) | `aes-demo` | A complete 16-byte AES-256 ECB roundtrip with owned output buffers |
| [Mouse Hook](Windows%20Cursor%20Hook/TESTLL) | `mouse-hook --help` | Scoped low-level mouse hook and bounded callback queue |
| [scanf](sscanf%20parse/Project3/Quelle.cpp) | `scanf-demo` | Bounded string conversion and checked field count |

Cursor replay and watch modes stop on Escape. The hide example restores the system cursor on ordinary exit; forced termination may bypass that cleanup. The command-buffer API accepts **trusted shell commands** and is not a sandbox or timeout-enabled process runner. Output filenames are handled as file paths rather than interpolated into shell code.

## Maintained standalone versions

The filesystem and mouse-hook examples include the current source from their focused repositories:

- [Multithreaded filesystem operations](https://github.com/Rick-laboratory/Multithreaded-Exceptionfree-Filesystem-Operations)
- [Low-level mouse hook](https://github.com/Rick-laboratory/Windows-LL-Mouse-Hook-Callback)

Those repositories contain the fuller API documentation and dedicated regression tests. Other related projects include [cursor position/color detection](https://github.com/Rick-laboratory/Windows-CursorPosition-ColorDetection) and [spline cursor movement](https://github.com/Rick-laboratory/Windows-SetCursorPos-Splinemovement).

## Historical material and checks

`decent and tested/` contains the original text snippets. The directory name is historical: the snippets are not included in the current build or validation. In particular, the Linux X11 note is not a maintained cross-platform implementation.

The legacy AES implementation is kept as an educational dependency. The demo no longer reads a 16-byte block from a six-byte string buffer. Its one-block roundtrip is not a cryptographic audit; ECB and the public sample key are not suitable for protecting application traffic.

All eleven executable targets build locally. CTest checks parsing, pipe output, output-buffer restoration, key-sequence state and the complete AES block. Interactive cursor movement, system cursor replacement and prolonged hooks are not run by the tests.

No repository-wide license file is currently included. Existing source comments and historical material are retained.
