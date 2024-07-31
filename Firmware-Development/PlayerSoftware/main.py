import os
from video_player import VideoPlayer
from utils import get_supported_files

def main(directory):
    supported_files = get_supported_files(directory)
    if not supported_files:
        print("No supported video files found in the directory.")
        return

    print("Supported video files:")
    for file in supported_files:
        print(file)

    while True:
        for file_path in supported_files:
            print("Playing :", file_path)
            player = VideoPlayer(file_path)
            player.play()

if __name__ == "__main__":
    main("/sdcard")
