import pathlib
import sys
import argparse

def create_cpp_content_file(directory_path: str, output_file: str) -> bool:
    """
    Recursively find all .cpp files in directory_path and write their paths and contents to output_file.
    Returns True if successful, False otherwise.
    """
    try:
        # Convert directory_path to a Path object and check if it's a valid directory
        directory = pathlib.Path(directory_path)
        if not directory.exists() or not directory.is_dir():
            print(f"Error: '{directory_path}' is not a valid directory")
            return False

        # Open the output file
        with open(output_file, 'w', encoding='utf-8') as out_file:
            # Recursively iterate through the directory
            for file_path in directory.rglob('*.cpp'):
                if file_path.is_file():
                    try:
                        # Write file path as a header
                        out_file.write(f"\n=== {file_path} ===\n")
                        # Read and write the file content
                        with open(file_path, 'r', encoding='utf-8') as cpp_file:
                            content = cpp_file.read()
                            out_file.write(content)
                            out_file.write("\n")  # Add newline after content
                    except (IOError, UnicodeDecodeError) as e:
                        print(f"Warning: Could not read '{file_path}': {e}")
                        out_file.write(f"Warning: Could not read file: {e}\n")

        print(f"Successfully wrote .cpp file contents to '{output_file}'")
        return True

    except PermissionError as e:
        print(f"Permission error: {e}")
        return False
    except IOError as e:
        print(f"IO error: {e}")
        return False
    except Exception as e:
        print(f"Unexpected error: {e}")
        return False

def main():

    # Run the function with provided or default arguments
    if not create_cpp_content_file('src', 'code.txt'):
        sys.exit(1)

if __name__ == "__main__":
    main()
