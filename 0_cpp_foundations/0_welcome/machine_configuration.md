# VS Code Configuration

## Style

[Linter](https://marketplace.visualstudio.com/items?itemName=mine.cpplint) (Uses [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html) )

**TODO** look up function and class naming conventions, linter doesn't seem to enforce that

## Intellisense

Setup [c_cpp_properties.json](https://code.visualstudio.com/docs/cpp/config-linux#_cc-configurations).  This config worked for udacity project which uses g++ and c++17
```
{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "/usr/local/include/io2d/**",
                "/usr/include/cairo/**",
                "${workspaceFolder}/**"
            ],
            "defines": [],
            "compilerPath": "/usr/bin/g++",
            "cStandard": "c17",
            "cppStandard": "gnu++17",
            "intelliSenseMode": "linux-gcc-x64"
        }
    ],
    "version": 4
}
```

## Dependencies

- **TODO**

## Extensions

- C/C++ by Microsoft
- C/C++ Extension Pack by Microsoft
- Clang-Format by Xaver Hallauer (not sure what this is doing)
- CMake by twxs
- CMake Tools by Microsoft
- cpplint by mine

## Debugging

**TODO**

## Other

- **TODO** settings.json
- **TODO** tasks.json
