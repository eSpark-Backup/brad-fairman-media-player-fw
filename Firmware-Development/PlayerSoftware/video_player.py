import sdl2
import sdl2.ext
import ctypes
import time
from datetime import datetime

from video_decoder import VideoDecoder

class VideoPlayer:
    def __init__(self, file_path):
        self.file_path = file_path
        self.width = 0
        self.height = 0
        self.window = None
        self.renderer = None
        self.texture = None
        self.decoder = None

    def init_sdl(self):
        sdl2.ext.init()
        self.decoder = VideoDecoder(self.file_path)
        self.width, self.height = self.decoder.get_video_dimensions()
        self.window = sdl2.SDL_CreateWindow(
            b"Video Playback",
            sdl2.SDL_WINDOWPOS_CENTERED,
            sdl2.SDL_WINDOWPOS_CENTERED,
            self.width,
            self.height,
            sdl2.SDL_WINDOW_FULLSCREEN_DESKTOP
        )
        self.renderer = sdl2.SDL_CreateRenderer(self.window, -1, sdl2.SDL_RENDERER_ACCELERATED | sdl2.SDL_RENDERER_PRESENTVSYNC)
        self.texture = sdl2.SDL_CreateTexture(
            self.renderer,
            sdl2.SDL_PIXELFORMAT_IYUV,
            sdl2.SDL_TEXTUREACCESS_STREAMING,
            self.width,
            self.height
        )

    def play(self):
        self.init_sdl()
        try:
            counter = 0
            now = datetime.now()
            current_time = now.strftime("%H:%M:%S")
            print("Current Time =", current_time)
            totaltime = 0

            while True:
                start_time = time.time()
                frame = self.decoder.read_frame()
                processing_time = time.time() - start_time
                totaltime+=processing_time
                # print(f"Processing time: {processing_time:.2f} seconds")
                if frame is None:
                    print("Frame is None")
                    break
                self.update_texture(frame)
                self.render_frame()
                counter+=1
                if counter % 25 == 0:
                    print("25 frames in", totaltime)
                    totaltime=0
                if counter == 25 * 60:
                    while True:
                        now = datetime.now()
                        current_time = now.strftime("%H:%M:%S")
                        print("Current Time =", current_time)
                        print(f'in while')
                        time.sleep(1)
                if self.handle_events():
                    break
        finally:
            self.cleanup()

    def update_texture(self, frame):
        y_plane, u_plane, v_plane = frame
        y_plane_ptr = y_plane.ctypes.data_as(ctypes.POINTER(ctypes.c_uint8))
        u_plane_ptr = u_plane.ctypes.data_as(ctypes.POINTER(ctypes.c_uint8))
        v_plane_ptr = v_plane.ctypes.data_as(ctypes.POINTER(ctypes.c_uint8))

        sdl2.SDL_UpdateYUVTexture(
            self.texture,
            None,
            y_plane_ptr, self.width,
            u_plane_ptr, self.width // 2,
            v_plane_ptr, self.width // 2
        )

    def render_frame(self):
        sdl2.SDL_RenderClear(self.renderer)
        sdl2.SDL_RenderCopy(self.renderer, self.texture, None, None)
        sdl2.SDL_RenderPresent(self.renderer)

    def handle_events(self):
        events = sdl2.ext.get_events()
        for event in events:
            if event.type == sdl2.SDL_QUIT:
                return True
        return False

    def cleanup(self):
        self.decoder.close()
        sdl2.SDL_DestroyTexture(self.texture)
        sdl2.SDL_DestroyRenderer(self.renderer)
        sdl2.SDL_DestroyWindow(self.window)
        sdl2.ext.quit()
