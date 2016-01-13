{
  "includes": [
      "common.gypi"
  ],
  "targets": [
    {
      "target_name": "tests",
      "type": "executable",
      "sources": [
        "test/unit.cpp",
        "test/t/mutating_visitor.cpp",
        "test/t/optional.cpp",
        "test/t/recursive_wrapper.cpp",
        "test/t/variant.cpp"
      ],
      "xcode_settings": {
        "SDKROOT": "macosx",
        "SUPPORTED_PLATFORMS":["macosx"]
      },
      "include_dirs": [
          "./",
          "test/include"
      ]
    }
  ]
}
