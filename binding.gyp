{
  "targets": [
    {
      "target_name": "Sudoku",
      "type": "executable",
      "sources": [
        "src/Main.cpp",
        "src/BTSolver.cpp",
        "src/Constraint.cpp",
        "src/ConstraintNetwork.cpp",
        "src/Domain.cpp",
        "src/SudokuBoard.cpp",
        "src/Trail.cpp",
        "src/Variable.cpp"
      ],
      "cflags": ["-Wall", "-std=c++11"],
      "conditions": [
        [ "OS=='mac'", {
            "xcode_settings": {
                "OTHER_CPLUSPLUSFLAGS" : ["-std=c++11","-stdlib=libc++"],
                "OTHER_LDFLAGS": ["-stdlib=libc++"],
                "MACOSX_DEPLOYMENT_TARGET": "10.7" }
            }
        ]
      ]
    }
  ]
}
