import cv2
import numpy as np
from pathlib import Path
import pickle

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
    original_number_of_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
    frame_index = 0

    # Read and extract all frames
    while True:
        
        ret, frame = cap.read()

        if frame_index % 100 == 0:
            print(f"Processing frame {frame_index} out of {original_number_of_frames}, which is {frame_index/original_number_of_frames*100:.2f}%")
        frame_index += 1

        # Break the loop if we have reached the end of the video
        if not ret:
            break

        if prev_frame is not None:
            dq = np.abs(frame.astype(np.int64) - prev_frame.astype(np.int64))

            d = np.sum(dq)
            md_adj = d / np.count_nonzero(dq)
            md = np.mean(dq)

            # print(f"Frame {idx} diff: {d}, mean: {md}, adjusted mean: {md_adj}")

            # cond = (frame == prev_frame).all()
            # cond = md_adj < 1.0
            cond = md < 1.0
            if cond:
                # print(f"Duplicate frame found: {idx}")
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

    new_number_of_frames = len(frames)

    file_size = Path(output_file_full_path).stat().st_size
    print(f"New file size in mb: {file_size / 1024 / 1024}")
    print(f"Original number of frames: {original_number_of_frames}")
    print(f"New number of frames: {new_number_of_frames}")

    # Release the VideoWriter
    video_writer.release()

def save_non_duplicate_frames_to_pickle(mov_file_path):
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
    original_number_of_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
    frame_index = 0

    # Read and extract all frames
    while True:
        
        ret, frame = cap.read()

        if frame_index % 100 == 0:
            print(f"Processing frame {frame_index} out of {original_number_of_frames}, which is {frame_index/original_number_of_frames*100:.2f}%")
        frame_index += 1

        # Break the loop if we have reached the end of the video
        if not ret:
            break

        if prev_frame is not None:
            dq = np.abs(frame.astype(np.int64) - prev_frame.astype(np.int64))

            d = np.sum(dq)
            dc = np.count_nonzero(dq)
            md_adj = d / dc
            md = np.mean(dq)

            print(f"Frame {idx} diff: {d}, mean: {md}, count: {dc}, adjusted mean: {md_adj}")

            # cond = (frame == prev_frame).all()
            # cond = md_adj < 1.0
            # cond = md < 1.0
            cond = d < 100000
            if cond:
                # print(f"Duplicate frame found: {idx}")
                continue

        # Append the frame to the list
        frames.append(frame)
        # print(f"Frame appended: {idx}")
        idx += 1
        prev_frame = frame

    # Release the video capture object
    cap.release()

    output_folder = "/".join(mov_file_path.split("/")[0:-1])
    output_name = mov_file_path.split("/")[-1].split(".")[0] + "_nodup.pickle"
    output_file_full_path = f"{output_folder}/{output_name}"

    with open(output_file_full_path, "wb") as f:
        pickle.dump(frames, f)


def save_from_pickle(mov_file_path, stride):

    pickle_path = mov_file_path.replace(".mov", "_nodup.pickle")

    output_folder = "/".join(mov_file_path.split("/")[0:-1])
    output_name = mov_file_path.split("/")[-1].split(".")[0] + "_nodup.mp4"
    output_file_full_path = f"{output_folder}/{output_name}"

    with open(pickle_path, "rb") as f:
        frames = pickle.load(f)

    height = frames[0].shape[0]
    width = frames[0].shape[1]

    fps = 58

    video_writer = cv2.VideoWriter(
        output_file_full_path, cv2.VideoWriter_fourcc(*"mp4v"), fps, (width, height)
    )


    for idx, image_file in enumerate(frames):
        if idx % stride == 0:
            video_writer.write(image_file)

    # Release the VideoWriter
    video_writer.release()

    file_size = Path(output_file_full_path).stat().st_size
    print(f"New file size in mb: {file_size / 1024 / 1024}")

def convert_to_mp4(mov_file_path):
    cap = cv2.VideoCapture(mov_file_path)

    # Check if the video file is opened successfully
    if not cap.isOpened():
        print("Error: Could not open the video file.")
        exit()

    output_folder = "/".join(mov_file_path.split("/")[0:-1])
    output_name = mov_file_path.split("/")[-1].split(".")[0] + ".mp4"
    output_file_full_path = f"{output_folder}/{output_name}"

    frames = []

    idx = 0

    while True:
        
        ret, frame = cap.read()

        # Break the loop if we have reached the end of the video
        if not ret:
            break
        
        idx += 1

        if idx % 3 != 0:
            continue

        frames.append(frame)
        print(f"Frame appended: {idx}")
        

    height = frames[0].shape[0]
    width = frames[0].shape[1]

    fps = 58

    video_writer = cv2.VideoWriter(
        output_file_full_path, cv2.VideoWriter_fourcc(*"mp4v"), fps, (width, height)
    )

    for image_file in frames:
        video_writer.write(image_file)

    video_writer.release()


if __name__ == "__main__":

    movie_name = "GuiDynamicSystem.mov"
    movie_path = "/Users/daniel/work/dvs/movies_trimmed/" + movie_name
    convert_to_mp4(movie_path)

    exit()

    movies = [["AdDataset.mov", 3], 
              ["BouncingBalls.mov", 3], 
              ["Boxes.mov", 3], 
              ["Duck.mov", 3], 
              ["DuoplotLogo.mov", 3], 
              ["FilteringWithSlider.mov", 3], 
              ["GuiDynamicSystem.mov", 3], 
              ["Hexagon.mov", 3], 
              ["Hyperboloid.mov", 3], 
              ["IMU.mov", 3], 
              ["Joints.mov", 3], 
              ["Klein1.mov", 3], 
              ["LinesAndDots.mov", 3], 
              ["LissaJous.mov", 3], 
              ["Lorenz.mov", 3], 
              ["Map.mov", 3], 
              ["Marbles.mov", 3], 
              ["Particles.mov", 3], 
              ["PidTuner.mov", 3], 
              ["PointCloud.mov", 3], 
              ["ScatterCluster.mov", 3], 
              ["ScatterSmallPoints.mov", 3], 
              ["ScatterVaryingPointSize.mov", 3], 
              ["Series.mov", 3], 
              ["Slam.mov", 3], 
              ["Slingshot.mov", 3], 
              ["SolarSystem.mov", 3], 
              ["Sphere.mov", 3], 
              ["SpringStructure.mov", 3], 
              ["Surf.mov", 3], 
              ["Surfs.mov", 3], 
              ["ThreeBodyProblem.mov", 3], 
              ["WaveEquation.mov", 3]]

    movies = [["Slingshot.mov", 2]]

    for idx in range(len(movies)):

        movie_name = movies[idx][0]
        movie_path = "/Users/daniel/work/dvs/movies/" + movie_name
        stride = movies[idx][1]

        file_size = Path(movie_path).stat().st_size
        print(f"Original file size in mb: {file_size / 1024 / 1024}")
        
        # save_non_duplicate_frames_to_pickle(movie_path)

        save_from_pickle(movie_path, stride)
