import os
import socket
import argparse
import socket
import threading
import time

import numpy as np
import cv2

import logging
logger = logging.getLogger(__name__)
logging.basicConfig(level=logging.ERROR)


def recv_frame(sock):
    # Read 4 bytes for frame size (little endian)
    size_bytes = sock.recv(4)
    if not size_bytes:
        logger.info('Connection closed by peer while reading frame size.')
        return
    if len(size_bytes) < 4:
        logger.error(f'Received {len(size_bytes)} bytes for frame size, expected 4. Connection might be unstable or closed.')
        return

    frame_size = int.from_bytes(size_bytes, 'little')
    logger.debug(f'Receiving {frame_size} bytes...')
    
    data = b''
    while len(data) < frame_size:

        # Receive frame data
        chunk = sock.recv(frame_size - len(data))
        if not chunk:
            logger.info('Connection closed by peer before receiving all data.')
            return
        data += chunk

    return data

def stream_to_ipython(ip, port, delay=0.01):
    import ipywidgets as widgets
    from IPython.display import display

    image_widget = widgets.Image(format='jpeg')  # ESP32 sends JPEG
    display(image_widget)
    
    should_loop = threading.Event()
    def stream_loop():
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
            sock.connect((ip, port))
            should_loop.set()

            while should_loop.is_set():
                frame = recv_frame(sock)
                if frame is None:
                    logger.info("Stream loop terminated due to disconnected or bad frame.")
                    break
                image_widget.value = frame
                time.sleep(delay)

    thread = threading.Thread(target=stream_loop)
    thread.start()
    return should_loop

def stream_to_directory(ip, port, directory, prefix='img_', delay=0.1):
    os.makedirs(directory, exist_ok=True)

    logger.debug(f'Streaming {ip}:{port} JPEGs into directory {directory}')

    should_loop = threading.Event()
    def stream_loop():
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((ip, port))
            should_loop.set()     

            counter = 0   

            while should_loop.is_set():
                jpeg_data = recv_frame(s)
                if jpeg_data is None:
                    logger.info("Stream loop terminated due to disconnected or bad frame.")
                    break
                outfile = os.path.join(directory, f'{prefix}{counter}.jpg')
                with open(outfile, 'wb') as f:
                    f.write(jpeg_data)
                counter += 1
                time.sleep(delay)

    thread = threading.Thread(target=stream_loop)
    thread.start()
    return should_loop

def stream_to_file(ip, port, outfile, fps=30):

    logger.debug(f'Streaming {ip}:{port} into {outfile}')
    
    should_loop = threading.Event()
    def stream_loop():
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((ip, port))
            should_loop.set()

            writer = None
            try:
                while should_loop.is_set():
                    jpeg_data = recv_frame(s)
                    if jpeg_data is None:
                        logger.info("Stream loop terminated due to disconnected or bad frame.")
                        break

                    # Decode JPEG bytes to image
                    np_data = np.frombuffer(jpeg_data, dtype=np.uint8)
                    frame = cv2.imdecode(np_data, cv2.IMREAD_COLOR)

                    if frame is None:
                        logger.warning("Failed to decode frame, skipping.")
                        continue

                    # Initialize video writer on first frame
                    if writer is None:
                        height, width, _ = frame.shape
                        fourcc = cv2.VideoWriter_fourcc(*'mp4v')
                        writer = cv2.VideoWriter(outfile, fourcc, fps, (width, height))

                    writer.write(frame)

            finally:
                logger.critical(f'Releasing file {outfile}')

                if writer:
                    writer.release()

    thread = threading.Thread(target=stream_loop)
    thread.start()
    return should_loop

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Stream JPEG frames from socket and save to video.")
    parser.add_argument("--ip", type=str, default='192.168.1.149', help="IP address of the server")
    parser.add_argument("--port", type=int, default=1337, help="Port number of the server")
    parser.add_argument("--outfile", type=str, required=True, help="Path to output video file (e.g. output.mp4)")
    parser.add_argument("--fps", type=int, default=30, help="Frames per second for output video")

    args = parser.parse_args()

    stream_to_file(ip=args.ip, port=args.port, outfile=args.outfile, fps=args.fps)