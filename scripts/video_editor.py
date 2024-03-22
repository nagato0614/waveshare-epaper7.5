import cv2
import os
import numpy as np
from PIL import Image


class VideoEditor:
    def __init__(self, video_file):
        self.video_file = video_file
        self.video = cv2.VideoCapture(video_file)
        self.video_title = os.path.splitext(os.path.basename(video_file))[0]
        self.image_dir = "images"

        # 保存先のディレクトリを作成
        if not os.path.exists(self.image_dir):
            os.makedirs(self.image_dir)

    def __del__(self):
        self.close_video()

    def close_video(self):
        self.video.release()

    def get_image(self, timestamp):
        """
        動画から指定したタイムスタンプの画像を取得
        :param timestamp: 単位 [ms]
        :return:
        """
        self.video.set(cv2.CAP_PROP_POS_MSEC, timestamp)
        ret, frame = self.video.read()

        if ret:
            image_name = f"{self.video_title}_{timestamp}.jpg"
            image_path = os.path.join(self.image_dir, image_name)
            cv2.imwrite(image_path, frame)
            print(f"画像を保存しました: {image_path}")
            return image_path
        else:
            print("画像の取得に失敗しました")

def convert_to_grayscale_bmp(input_image, output_path, threshold=128):
    # 画像を読み込む
    image = cv2.imread(input_image)

    if image is None:
        print("画像の読み込みに失敗しました")
        return

    # グレースケールに変換
    grayscale_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # 2値化処理
    _, binary_image = cv2.threshold(grayscale_image, threshold, 255, cv2.THRESH_BINARY)

    # 2値画像をPIL形式に変換
    pil_image = Image.fromarray(binary_image)
    image = pil_image.convert("1")

    # 出力パスに拡張子を追加
    output_path_with_extension = output_path + ".bmp"

    # 画像を保存
    image.save(output_path_with_extension)

    print(f"2値化したBMP画像を保存しました: {output_path_with_extension}")

    return output_path_with_extension

def convert_to_grayscale_dots(input_image, output_path):
    # 画像を読み込む
    image = Image.open(input_image)

    # グレースケールに変換
    grayscale_image = image.convert("L")

    # ディザリングを適用
    dithered_image = grayscale_image.convert("1", dither=Image.FLOYDSTEINBERG)

    # 画像を保存
    dithered_image.save(output_path)

    print(f"ディザリングを適用した画像を保存しました: {output_path}")
    return output_path


def resize_image(input_image, output_path, target_size=(800, 480)):
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
        # 高さを目標サイズに合わせてリサイズ
        new_height = target_height
        new_width = int(new_height * aspect_ratio)
    else:
        # 幅を目標サイズに合わせてリサイズ
        new_width = target_width
        new_height = int(new_width / aspect_ratio)

    resized_image = cv2.resize(image, (new_width, new_height), interpolation=cv2.INTER_AREA)

    # 目標サイズの空の画像を作成
    # 画像のチャンネル数が3の場合
    if len(image.shape) == 3:
        result_image = np.zeros((target_height, target_width, image.shape[2]), dtype=image.dtype)
    else:
        # 画像のチャンネル数が1の場合
        result_image = np.zeros((target_height, target_width), dtype=image.dtype)

    # リサイズした画像を中央に配置し、はみ出した部分を切り取る
    y_offset = (new_height - target_height) // 2
    x_offset = (new_width - target_width) // 2
    result_image = resized_image[y_offset:y_offset+target_height, x_offset:x_offset+target_width]

    # 入力画像の拡張子を取得
    _, extension = os.path.splitext(input_image)

    # 出力パスに拡張子を追加
    output_path_with_extension = output_path + extension

    # 画像を保存
    # bmpの場合は圧縮なしで保存
    cv2.imwrite(output_path_with_extension, result_image)

    print(f"リサイズした画像を保存しました: {output_path_with_extension}")

    return output_path_with_extension

if __name__ == '__main__':
    # 動画ファイルを指定してビデオエディターを作成
    video_file = "video/usagi_dance.mp4"
    editor = VideoEditor(video_file)

    # 3秒から15までの画像を100msごとに取得
    for t in range(3000, 4000, 100):
        image_path = editor.get_image(t)

        # 画像をリサイズ
        resized_img = resize_image(image_path, f"images/resize_{t}", target_size=(800, 480))

        # 画像を2値化してBMP画像として保存
        # bmp_img = convert_to_grayscale_bmp(resized_img, f"images/binary_{t}")

        # 画像をディザリングしてドット絵風に変換
        dots_img = convert_to_grayscale_dots(resized_img, f"images/dots_{t}.bmp")


    # ビデオエディターを閉じる
    editor.close_video()