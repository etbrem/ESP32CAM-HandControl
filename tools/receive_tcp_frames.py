import os
import socket
import argparse
import logging

import numpy as np
import cv2


IP = '192.168.1.149'
PORT = 1337
OUTPUT_FILE = 'received_frame.jpg'


logger = logging.getLogger(__name__)

def recv_image(sock):
    # Read 4 bytes for frame size (little endian)
    size_bytes = sock.recv(4)
    if len(size_bytes) < 4:
        raise RuntimeError('Failed to read frame size')

    frame_size = int.from_bytes(size_bytes, 'little')
    print(f'Receiving {frame_size} bytes...')
    
    data = b''
    while len(data) < frame_size:

        # Receive frame data
        chunk = sock.recv(frame_size - len(data))
        if not chunk:
            raise RuntimeError('Connection closed before receiving all data')
        data += chunk

    return data

def stream_to_file(outfile, ip=IP, port=PORT, fps=30):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((ip, port))

        writer = None

        try:
            while True:
                jpeg_data = recv_image(s)

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

        except Exception as e:
            logger.error(f'Recv frame error: {e}')
        finally:
            if writer:
                writer.release()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Stream JPEG frames from socket and save to video.")
    parser.add_argument("--ip", type=str, default=IP, help="IP address of the server")
    parser.add_argument("--port", type=int, default=PORT, help="Port number of the server")
    parser.add_argument("--outfile", type=str, required=True, help="Path to output video file (e.g. output.mp4)")
    parser.add_argument("--fps", type=int, default=30, help="Frames per second for output video")

    args = parser.parse_args()

    stream_to_file(args.outfile, ip=args.ip, port=args.port, fps=args.fps)