import os

def list_files_in_directory_to_textfile(directory, output_file):
    """
    Lists all files in a given directory and writes the content of each file to a text file
    along with the filename as a header.
    """
    with open(output_file, 'w') as outfile:
        for filename in os.listdir(directory):
            if os.path.isfile(os.path.join(directory, filename)):
                outfile.write(f'{filename}:\n\n')
                with open(os.path.join(directory, filename), 'r') as infile:
                    outfile.write(infile.read())
                    outfile.write('\n\n')

# Example usage
directory = 'F:\General_Project_Repositories\Indigo\GameEngine\src'  # Replace with your directory path
output_file = 'output.txt'  # Replace with your desired output file name
list_files_in_directory_to_textfile(directory, output_file)
