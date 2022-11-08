## About
This shows how to setup VSCode for C++ development on Windows 10 and M1 Macbooks, using `LLVM`/`clang` for compiling, `clangd` for language server and intellisense, `vcpkg` for package/library management, and `CMake`/`Ninja` for general project management and buildfile generation.

It includes a basic example of REST API interaction and JSON processing using libraries I believe to be good for simplicity and non-advanced usage.

As an infrequent C++ developer, this is a rough guide that probably could be improved or revised.

## VSCode (Basically) Required Extensions
1. C/C++ (https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
2. clangd (https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd)
3. CMake (https://marketplace.visualstudio.com/items?itemName=twxs.cmake)
4. CMake Tools (https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)

## VSCode Optional Extensions
1. Thunder Client (https://marketplace.visualstudio.com/items?itemName=rangav.vscode-thunder-client)
    - Similar to "Postman" but built in, lets you make REST requests without code

## Setup on Windows 10
1. Install and configure Git
2. Install VSCode, as well as the extensions mentioned above.
3. Download and run `Build Tools for Visual Studio 2022` under the `Tools for Visual Studio` collapsible at the bottom of the page at https://visualstudio.microsoft.com/downloads/
4. Select the `Desktop Development with C++` option, it should be about 4GB.
    - Make note of what version of windows SDK it installs, for instance, `10.0.19041.0`
5. Download and run `Windows x64 Installer` from https://cmake.org/download/
    - Do not add CMake to the system PATH
6. Click `download` on the latest non-git version at https://releases.llvm.org/, then scroll down to LLVM-##.#.#-win64.exe, download, and run.
    - Smartscreen will block it, click `More Info` and `Run Anyway` to bypass
    - Do not add LLVM to the system PATH
7. Create a new folder for VCPKG to reside in. A good choice is `C:\Bin`.
8. Open a new PowerShell window AS ADMINISTRATOR and 
    - `cd` to that folder (i.e. `C:\Bin`) 
    - Run `git clone https://github.com/Microsoft/vcpkg.git`
    - Your folder structure should now look like `C:\Bin\vcpkg`, not `C:\Bin\vcpkg\vcpkg`.
    - In the same powershell, run `.\vcpkg\bootstrap-vcpkg.bat`
    - Finally, run `.\vcpkg\vcpkg integrate install`
    - Close the PowerShell window
9. Download the latest Ninja release, typically named `ninja-win.zip`, from https://github.com/ninja-build/ninja/releases
    - Extract the EXE inside to a new folder. A good choice is `C:\Bin\ninja`.
10. In the Start menu, search for and open `Edit the system environment variables`.
11. Click Environment Variables at the bottom.
12. Under System variables click "New" and add the following "Variable name" and "Variable value" pairs. Adjust as needed. You can ensure they exist by clicking `Browse File` in the window that pops up (or `Browse Directory`, for values that point to a folder):
    - Name `CPP_CLANGD_PATH`        Value `C:\Program Files\LLVM\bin\clangd.exe`
    - Name `CPP_CMAKE_PATH`         Value `C:\Program Files\CMake\bin\cmake.exe`
    - Name `CPP_GENERATOR_PATH`     Value `C:\Bin\ninja\ninja.exe`
    - Name `CPP_VCPKG_ROOT`         Value `C:\Bin\vcpkg`
    - Name `CPP_WINSDK_ROOT`        Value `C:\Program Files (x86)\Windows Kits\10\bin\10.0.19041.0\x86`
    - Name `VCPKG_DEFAULT_TRIPLET`  Value `x64-windows` (Not a file/folder, just a string value)
13. Under System variables, scroll through to find the `Path` variable, and click `Edit`
    - Add `%CPP_WINSDK_ROOT%`
    - Add `%CPP_VCPKG_ROOT%`
14. Open a new PowerShell. Run the commands below. If any raise an error, i.e. `The term is not recognized as the name of a cmdlet, funct...`, something was done incorrectly, so carefully re-evaluate the steps above.
    - `.$env:CPP_CLANGD_PATH --version`
    - `.$env:CPP_CMAKE_PATH --version`
    - `.$env:CPP_GENERATOR_PATH --version`
    - `vcpkg --version`
    - `.$env:CPP_VCPKG_ROOT\vcpkg --version`
    - `rc /h`
    - `.$env:CPP_WINSDK_ROOT\rc /h`

## Setup on VSCode (Universal)
1. Before proceeding, if any VSCode window was open during the setup above, close it and reopen it.
1. Control+Shift+P to bring up palette view, type `settings`, then search and open `Open User Settings (JSON)`.
2. Paste in the following config, modifying only if needed:
```jsonc
  //#########
  //## C++ ##
  //#########

  // Disable formatting and intellisense engine in Microsoft C/C++ extension (provides debugging)
  // This way you can have the other features of the extension but use clangd for LSP
  "C_Cpp.formatting": "Disabled",
  "C_Cpp.intelliSenseEngine": "Disabled",

  //Configure .clangd arguments
  "clangd.checkUpdates": false, // Disable 'New clangd is available, download?' prompt, downloads to a totally different folder
  "clangd.onConfigChanged": "restart",
  "clangd.path": "${env:CPP_CLANGD_PATH}",
  "clangd.arguments": [
    "-background-index", // Index project code in the background and persist index on disk.
    "-header-insertion=iwyu", // Add #include directives when accepting code completions.  Options: "iwyu" (Include what you use), "never"
    "-header-insertion-decorators", // Prepend a circular dot or space before the completion label, depending on whether an include line will be inserted or not
    "--function-arg-placeholders" //When disabled, completions contain only parentheses for function calls. When enabled, completions also contain placeholders for method parameters
  ],
  
  //Configure cmake arguments
  "cmake.buildDirectory": "${workspaceFolder}/build/${buildKit}/${buildType}",
  "cmake.cmakePath": "${env:CPP_CMAKE_PATH}",
  "cmake.configureArgs": [
    "-Wno-dev" // Suppress warnings about CMake files you didnt make
  ],
  "cmake.configureOnOpen": true, // Personal choice, runs the `cmake configure` command on vscode start
  "cmake.configureSettings": {
    "CMAKE_EXPORT_COMPILE_COMMANDS": 1,
    "CMAKE_TOOLCHAIN_FILE": "${env:CPP_VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake",
    "CMAKE_MAKE_PROGRAM": "${env:CPP_GENERATOR_PATH}"
  },
  "cmake.copyCompileCommands": "${workspaceFolder}/compile_commands.json",
  "cmake.environment": {
    "VERBOSE": "0" // Set VERBOSE to 1 if you want more information from builds.
  },
  "cmake.generator": "Ninja",
  "cmake.sourceDirectory": "${workspaceFolder}",

  "[cpp]": {
    "editor.inlayHints.enabled": "off" // Can get annoying for cpp specficially
  }
```
3. Restart VSCode.
4. In the bottom left, there will be a button saying something about a kit, likely `No active kit`. Click it.
    - Alternatively, Ctrl+Shift+P and search `CMake: Select a Kit`.
5. Click `[Scan for kits]`.
6. Do the previous step again, this time looking for the appropriate option, `Clang #### x86_64-pc-windows-msvc`, pointing to the LLVM path. Do not select `Clang-cl`, theres an unknown malfunction with vcpkg while using that.
7. Open a terminal (`Ctrl+\~` in VSCode). Install the two packages with vcpkg:
    - `vcpkg install cpr`
    - `vcpkg install nlohmann-json`
8. Go to the `CMakeLists.txt` and Save. This will trigger a configure. Ideally, any errors on this page should be cleared and the Output console should show `Build files have been written to...`.
9. Go to the `main.cpp` file ane Save. This should trigger a refresh, clearing any error squiggles.
10. In the bottom left, click Build. This should run successfully.
    - Alternatively, Ctrl+Shift+P and search `CMake: Build`.
11. Set a breakpoint somewhere in the code, ideally on the last statement (currently a `std::cout` on line 24). You can do this by hovering to the left of the line number, and a red dot should appear that you can click.
12. In the bottom left, click the bug icon to the right of the play icon, and to the left of some text that might say `[all]`.
    - Alternatively, Ctrl+Shift+P and search `CMake: Debug`.
13. The program should halt on the line you've specified. Variables should be visible in the debug panel to the left, and you should be able to execute basic code (i.e., `r.status`) by clicking the Debug Console tab at the bottom and writing it in.

## New Projects
You can start new projects fairly easily now:
1. Create an empty folder outside this project.
2. Open the folder with VSCode.
3. Open the Command Palette with Ctrl+Shift+P (Or View->Command Palette).
4. Run `CMake: Quick Start`.
5. Select the proper kit based on the setup steps above.
6. Enter a project name (ideally without spaces, like `PascalCase` or `kebab-case`).
7. Select `Executable`.
8. There will be a warning in the bottom right about `CMakeLists.txt was not found in...`, just click the X, as it should be there now.
9. If on Git, copy the `.gitignore` file from this project to your new project.
10. If on Git, commit what you have. Something like "Initial commit" for the message.
11. Install any libraries you want with vcpkg (`vcpkg install xyz`) and in the output they should provide "CMake targets" to paste into `CMakeLists.txt`.
    - It may ask you to add `target_link_libraries(main, ...` which will likely fail. Replace `main` with `${PROJECT_NAME}`, which is defined at the top of the CMake file.
12. Optionally, remove the `include(CTest)` and `enabled_testing()` lines if you wish to remove the `Run CTest` button in the bottom left. 
    - Be sure to remove the `build` folder and rebuild after doing this, or it will give warnings about test failures.
14. If on Git, you should commit after ensuring basic functionality from the libraries you installed. Something like "Added initial dependencies" for the message.

## Additional Settings
Custom settings that are personal preference.

Control+Shift+P to bring up palette view, type `settings`, and open `Open User Settings (JSON)`

General settings, not needed, add as you wish and experiment. Mouse over a setting in VSCode for more details.
```jsonc
  // Theme/Visual
  "workbench.colorTheme": "Default Dark+",
  "workbench.colorCustomizations": {
    "editorGutter.background": "#383838"
  },
  "editor.showFoldingControls": "always",
  "editor.renderFinalNewline": false,
  "editor.rulers": [
    {
      "column": 100,
      "color": "#5ab1ab21"
    },
    {
      "column": 120,
      "color": "#ff000046"
    }
  ],
  "window.title": "${dirty}${rootName}",
  
  // Git
  "git.autofetch": true,
  "git.confirmSync": false,
  "git.enableSmartCommit": true,
  "git.ignoreRebaseWarning": true,
  
  // Misc
  "explorer.confirmDragAndDrop": false,
  "workbench.startupEditor": "none",
  "files.watcherExclude": {
    "**/.git/objects/**": true,
    "**/.git/subtree-cache/**": true,
    "**/.cache/**": true, // C++
    "**/build/**": true // C++
  },
  "search.exclude": {
    "**/.git/**": true,
    "**/.cache/**": true, // C++
    "**/build/**": true // C++
  },
  
