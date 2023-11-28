import cv2
import os


def images_to_video(directory_path, output_video_path="output_video.mp4", fps=24):
    # Get the list of PNG files in the directory
    image_files = [f for f in os.listdir(directory_path) if f.endswith(".png")]

    # Sort the files to maintain order
    image_files.sort()

    # Get the first image to set the video size
    first_image = cv2.imread(os.path.join(directory_path, image_files[0]))
    height, width, layers = first_image.shape

    # Create a VideoWriter object
    video_writer = cv2.VideoWriter(
        output_video_path, cv2.VideoWriter_fourcc(*"mp4v"), fps, (width, height)
    )

    # Loop through the images and add them to the video
    for image_file in image_files:
        image_path = os.path.join(directory_path, image_file)
        frame = cv2.imread(image_path)
        video_writer.write(frame)

    # Release the VideoWriter
    video_writer.release()

    print(f"Video created at {output_video_path}")


if __name__ == "__main__":
    images_to_video(
        "/Users/danielpi/work/dvs/demo_screenshots_output/ad_dataset",
        output_video_path="/Users/danielpi/work/dvs/demo_screenshots_output/ad_dataset/output_video.mp4",
        fps=24,
    )
