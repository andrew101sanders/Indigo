import os

def list_files_in_directory_to_textfile(directory, output_file):
    """
    Lists all files in a given directory and its subdirectories, and writes the content
    of each file to a text file along with the filename and relative path as a header.
    """
    with open(output_file, 'w', encoding='utf-8') as outfile:
        for root, dirs, files in os.walk(directory):
            for filename in files:
                file_path = os.path.join(root, filename)
                relative_path = os.path.relpath(file_path, directory)
                outfile.write(f'{relative_path}:\n\n')
                try:
                    with open(file_path, 'r', encoding='utf-8') as infile:
                        outfile.write(infile.read())
                except UnicodeDecodeError:
                    outfile.write(f"Unable to read file: {relative_path} (possibly binary)\n")
                outfile.write('\n\n')

# Example usage
directory = 'F:\\General_Project_Repositories\\Indigo\\Indigo\\src'  # Replace with your directory path
output_file = 'output.txt'  # Replace with your desired output file name
list_files_in_directory_to_textfile(directory, output_file)