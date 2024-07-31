import os

SUPPORTED_EXTENSIONS = ('.mp4', '.mkv', '.avi', '.mov','.webm')  # Add other supported extensions as needed

def get_supported_files(directory):
    return [os.path.join(directory, f) for f in os.listdir(directory) if f.endswith(SUPPORTED_EXTENSIONS)]
