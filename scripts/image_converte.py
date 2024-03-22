import cv2
import os
import numpy as np
from PIL import Image


def resize_and_convert_to_grayscale_bmp(input_image,
                                        output_path,
                                        target_size=(800, 480),
                                        threshold=128):
    # 画像を読み込む
    image = cv2.imread(input_image, cv2.IMREAD_UNCHANGED)

    if image is None:
        print("画像の読み込みに失敗しました")
        return

    # 画像の元のサイズを取得
    height, width = image.shape[:2]

    # アスペクト比を維持しながら、目標サイズに合わせてリサイズ
    aspect_ratio = width / height
    target_width, target_height = target_size
    target_aspect_ratio = target_width / target_height

    if aspect_ratio > target_aspect_ratio:
        # 幅が目標サイズより大きくならないように調整
        new_width = target_width
        new_height = int(target_width / aspect_ratio)
    else:
        # 高さが目標サイズより大きくならないように調整
        new_height = target_height
        new_width = int(target_height * aspect_ratio)

    # 画像サイズを表示
    print(f"リサイズ前の画像サイズ: {width} x {height}")
    print(f"リサイズ後の画像サイズ: {new_width} x {new_height}")

    resized_image = cv2.resize(image, (new_width, new_height), interpolation=cv2.INTER_AREA)

    # 2値画像をPIL形式に変換
    pil_image = Image.fromarray(resized_image)
    image = pil_image.convert("1", dither=Image.FLOYDSTEINBERG)

    # 出力パスに拡張子を追加
    output_path_with_extension = output_path + ".bmp"

    # 画像を保存
    image.save(output_path_with_extension)

    print(f"リサイズして2値化したBMP画像を保存しました: {output_path_with_extension}")

    return output_path_with_extension


if __name__ == '__main__':

    input_dir = "./weather_pic"
    output_dir = "output"
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # 指定したディレクトリ内にあるファイル一覧を取得する
    file_list = []
    for file in os.listdir(input_dir):
        if file.endswith(".svg"):
            file_list.append(file)

    count = 0
    for file in file_list:
        input_image = os.path.join(input_dir, file)
        print(f"処理対象の画像: {input_image}")
        # 出力画像名を作成
        output_image = os.path.join(output_dir, f"{count}")

        resize_and_convert_to_grayscale_bmp(input_image, output_image)

        count += 1
