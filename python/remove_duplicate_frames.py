import cv2
import numpy as np


def remove_duplicate_frames(mov_file_path):
    # Open the video file
    cap = cv2.VideoCapture(mov_file_path)

    # Check if the video file is opened successfully
    if not cap.isOpened():
        print("Error: Could not open the video file.")
        exit()

    # Create a list to store frames
    frames = []

    prev_frame = None

    idx = 0
    tot_size = 4539240

    # Read and extract all frames
    while True:
        ret, frame = cap.read()

        # Break the loop if we have reached the end of the video
        if not ret:
            break

        if prev_frame is not None:
            dq = np.abs(frame.astype(np.int64) - prev_frame.astype(np.int64))

            d = np.sum(dq)
            md_adj = d / np.count_nonzero(dq)
            md = np.mean(dq)

            print(f"Frame {idx} diff: {d}, mean: {md}, adjusted mean: {md_adj}")

            # cond = (frame == prev_frame).all()
            # cond = md_adj < 1.0
            cond = md < 1.0
            if cond:
                print(f"Duplicate frame found: {idx}")
                continue

        # Append the frame to the list
        frames.append(frame)
        # print(f"Frame appended: {idx}")
        idx += 1
        prev_frame = frame

    # Release the video capture object
    cap.release()

    height = frames[0].shape[0]
    width = frames[0].shape[1]

    fps = 24

    # output_folder = "/Users/danielpi/work/dvs/demo_screenshots_output/"

    output_folder = "/".join(mov_file_path.split("/")[0:-1])
    output_name = mov_file_path.split("/")[-1].split(".")[0] + "_nodup.mp4"
    output_file_full_path = f"{output_folder}/{output_name}"

    video_writer = cv2.VideoWriter(
        output_file_full_path, cv2.VideoWriter_fourcc(*"mp4v"), fps, (width, height)
    )

    for image_file in frames:
        video_writer.write(image_file)

    # Release the VideoWriter
    video_writer.release()


if __name__ == "__main__":
    remove_duplicate_frames(
        "/Users/danielpi/Desktop/Screen Recording 2023-11-28 at 12.29.23.mov"
    )
