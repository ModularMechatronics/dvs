import pickle
import cv2
import os
import numpy as np

PC_HEADER = np.dtype([('num_points', '<u4')])
IMG_HEADER = np.dtype([('width', '<u4'), ('height', '<u4')])

def save_point_cloud(point_cloud_data, output_path):

    num_points_bts = np.array([(point_cloud_data.shape[0])], dtype=PC_HEADER).tobytes()
    x = point_cloud_data['x']
    y = point_cloud_data['y']
    z = point_cloud_data['z']

    all_bytes = num_points_bts + x.tobytes() + y.tobytes() + z.tobytes()

    with open(output_path, "wb") as output_file:

        output_file.write(all_bytes)

def save_image(image_file_path, output_path):

    img_original = cv2.imread(image_file_path)
    img = cv2.resize(img_original, None, fx=0.5, fy=0.5)

    height, width, _ = img.shape

    r_channel = img[:, :, 2]
    g_channel = img[:, :, 1]
    b_channel = img[:, :, 0]

    dimension_bts = np.array([(width, height)], dtype=IMG_HEADER).tobytes()
    all_bytes = dimension_bts + r_channel.tobytes() + g_channel.tobytes() + b_channel.tobytes()

    with open(output_path, "wb") as output_file:
        output_file.write(all_bytes)

def extract_dataset(dataset_root, result_root):

    lidar_folder_name = "ouster64_bfc_xyzit"
    image_folder_name = "flir_bfc_img"

    if not os.path.exists(results_root):
        os.makedirs(results_root)

    lidar_folder_path = os.path.join(dataset_root, lidar_folder_name)
    lidar_files = {f: os.path.join(lidar_folder_path, f) for f in os.listdir(lidar_folder_path) if f.endswith(".pkl")}

    image_folder_path = os.path.join(dataset_root, image_folder_name)
    image_files = {f: os.path.join(image_folder_path, f) for f in os.listdir(image_folder_path) if f.endswith(".jpg")}

    lidar_output_path = os.path.join(result_root, "lidar")
    image_output_path = os.path.join(result_root, "img")

    if not os.path.exists(lidar_output_path):
        os.makedirs(lidar_output_path)
    
    if not os.path.exists(image_output_path):
        os.makedirs(image_output_path)

    """for file_name, file_path in lidar_files.items():

        file_output_path = os.path.join(lidar_output_path, file_name.rstrip(".pkl") + ".bin")

        with open(file_path, 'rb') as f:
            data = pickle.load(f)

            save_point_cloud(data, file_output_path)"""
    
    for file_name, file_path in image_files.items():

        file_output_path = os.path.join(image_output_path, file_name.rstrip(".jpg") + ".bin")

        save_image(file_path, file_output_path)

    a = 1
    


if __name__ == "__main__":

    dataset_root = "./leddar_dataset/20200706_171559_part27_1170_1370"
    results_root = "./leddar_dataset/20200706_171559_part27_1170_1370/output"
    

    extract_dataset(dataset_root, results_root)
    

    a = 1
