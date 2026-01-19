from pathlib import Path
import sys


print("Populating sources...")


def is_in_cmake_folder(p: Path, base_folder: Path):
    for parent in p.parents:
        if parent == base_folder:
            break
        if (parent / "CMakeLists.txt").is_file():
            return True
    return False


# args
# src = Path(sys.argv[1])
src_list = [Path("./src/"), Path("./lib/")]
out_name = "sources.cmake"
extensions = [
    ".c",
    ".h",
    ".cc",
    ".hh",
    ".cpp",
    ".hpp",
    ".cxx",
    ".hxx",
    ".cu",
    ".m",
    ".mm",
    ".hm",
    ".inl",
    ".inc",
    ".tpp",
    ".txx",
    ".ipp",
]

for src in src_list:
    cmake_files = [p for p in src.rglob("CMakeLists.txt") if p.is_file()]
    cmake_folders = [p.parent for p in cmake_files]

    print(src, "- these folders will be affected:\n", cmake_folders)
    if len(sys.argv) > 1 and sys.argv[1] != "-y":
        _ = sys.stdout.write("proceed? [y/*] ")
        if input().lower() != "y":
            print("aborted")
            exit()

    for folder in cmake_folders:
        source_files = [
            p.relative_to(folder)
            for p in folder.rglob("*")
            if p.suffix.lower() in extensions
            and p.is_file()
            and not is_in_cmake_folder(p, folder)
        ]

        source_files_str = [str(p) for p in sorted(source_files)]
        with open(folder / out_name, "w+") as out:
            _ = out.write("set(SOURCES\n  ")
            _ = out.write("\n  ".join(source_files_str) + "\n")
            _ = out.write(")")
