import pickle
import cv2
import os
import numpy as np

PC_HEADER = np.dtype([('num_points', '<u4')])

def save_img():
    img = cv2.imread('src/demos/tests/car/car.png', cv2.IMREAD_GRAYSCALE)
    img_gray = cv2.resize(img, None, fx=0.5, fy=0.5)

    with open("src/demos/tests/car/car.bin", "wb") as newFile:

        img_byte_array = bytearray(img_gray)
        newFile.write(img_byte_array)

def save_point_cloud(point_cloud_data, output_path):

    num_points_bts = np.array([(point_cloud_data.shape[0])], dtype=PC_HEADER).tobytes()
    x = point_cloud_data['x']
    y = point_cloud_data['y']
    z = point_cloud_data['z']

    all_bytes = num_points_bts + x.tobytes() + y.tobytes() + z.tobytes()

    with open(output_path, "wb") as output_file:

        output_file.write(all_bytes)

    # x.tobytes()

    pass

def extract_dataset(dataset_root, result_root):

    lidar_folder_name = "ouster64_bfc_xyzit"

    if not os.path.exists(results_root):
        os.makedirs(results_root)

    lidar_folder_path = os.path.join(dataset_root, lidar_folder_name)
    lidar_files = {f: os.path.join(lidar_folder_path, f) for f in os.listdir(lidar_folder_path) if f.endswith(".pkl")}

    lidar_output_path = os.path.join(result_root, "lidar")

    if not os.path.exists(lidar_output_path):
        os.makedirs(lidar_output_path)

    for file_name, file_path in lidar_files.items():
        a = 1

        file_output_path = os.path.join(lidar_output_path, file_name.rstrip(".pkl") + ".bin")

        with open(file_path, 'rb') as f:
            data = pickle.load(f)

            save_point_cloud(data, file_output_path)
    a = 1
    


if __name__ == "__main__":

    dataset_root = "./leddar_dataset/20200706_171559_part27_1170_1370"
    results_root = "./leddar_dataset/20200706_171559_part27_1170_1370/output"
    

    extract_dataset(dataset_root, results_root)
    

    a = 1
