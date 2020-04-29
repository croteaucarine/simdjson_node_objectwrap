{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ 
          "/simdjson/src/simdjson.cpp",
          "/src/addon.cpp" 
        ],
      'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")"],
      'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
      "default_configuration": "Release",
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-O3", "-fno-exceptions", "-std=gnu++0x", "-std=gnu++1y"],
      "cflags_cc+": ["-O3", "-std=c++17"],
      "xcode_settings": {
        "OTHER_CFLAGS": ["-std=c++17"],
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
      },
      "msvs_settings": {
        "VCCLCompilerTool": {
          "AdditionalOptions": ["/std:c++17"]
        }
      },
      "include_dirs": ["<!@(node -p \"require('node-addon-api').include\")"],
      "defines": ["NAPI_CPP_EXCEPTIONS"]
    }
  ]
}
