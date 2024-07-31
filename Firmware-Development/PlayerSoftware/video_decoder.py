import ffmpeg
import numpy as np

class VideoDecoder:
    def __init__(self, file_path):
        self.file_path = file_path
        self.process = None
        self.width = 0
        self.height = 0
        self.init_ffmpeg()

    def init_ffmpeg(self):
        probe = ffmpeg.probe(self.file_path)
        video_info = next(stream for stream in probe['streams'] if stream['codec_type'] == 'video')
        self.width = int(video_info['width'])
        self.height = int(video_info['height'])
        
        # Enable hardware acceleration (example for VAAPI, adjust as needed for your platform)
        self.process = (
            ffmpeg
            .input(self.file_path)
            .output('pipe:', format='rawvideo', pix_fmt='yuv420p', s=f'{self.width}x{self.height}')
            .global_args('-hwaccel', 'drm')
            .run_async(pipe_stdout=True, pipe_stderr=True)
        )

    def get_video_dimensions(self):
        return self.width, self.height

    def read_frame(self):
        in_bytes = self.process.stdout.read(self.width * self.height * 3 // 2)  # YUV420p
        if not in_bytes:
            return None
        frame = np.frombuffer(in_bytes, np.uint8).reshape((self.height * 3 // 2, self.width))
        y_plane = frame[:self.height]
        u_plane = frame[self.height:self.height + self.height // 4].reshape((self.height // 2, self.width // 2))
        v_plane = frame[self.height + self.height // 4:].reshape((self.height // 2, self.width // 2))
        return y_plane, u_plane, v_plane

    def close(self):
        self.process.stdout.close()
        self.process.stderr.close()
        self.process.wait()
