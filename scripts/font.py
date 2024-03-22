import os

import numpy as np
from PIL import Image, ImageFont, ImageDraw


def display_character(character, font_name="Symbol.ttf"):
    # 画像サイズの設定
    image_size = (24, 24)

    # 黒い背景の画像を作成
    image = Image.new("RGB", image_size, (0, 0, 0))

    # 描画オブジェクトの作成
    draw = ImageDraw.Draw(image)

    # フォントの設定
    font_size = 20
    font = ImageFont.truetype(font_name, font_size, index=0)

    # 文字の大きさを取得
    text_size = draw.textbbox(character, font=font)

    # 文字を描画
    draw.text((0, 0), character, font=font, fill=(255, 255, 255))

    # 画像をnumpy配列に変換
    image_np = np.array(image)

    return image_np

def display_all_characters(font_name="Symbol.ttf", start=0x0000, end=0x10ffff):
    """
    指定したフォントに含まれるすべての文字を表示する関数
    :param font_name: フォントファイルの名前
    :param start: Unicodeの開始範囲
    :param end: Unicodeの終了範囲
    """

    # char ディレクトリを作成
    if not os.path.exists("char"):
        os.makedirs("char")
        print("char ディレクトリを作成しました")
    else:
        print("char ディレクトリはすでに存在します")

    for code in range(start, end):
        print(f"Displaying character: {code}")
        character = chr(code)
        try:
            img = display_character(character, font_name)

            # char ディレクトリに画像を保存
            img = Image.fromarray(img)
            save_path = f"char/{code}.png"
            print(f"Saving character {character} to {save_path}")
            img.save(save_path)
        except Exception as e:
            print(f"Error displaying character {character}: {e}")


if __name__ == '__main__':
    # 表示する文字を指定
    character = "あ"
    font_name = "/System/Library/Fonts/ヒラギノ丸ゴ ProN W4.ttc"

    # 文字を表示
    display_all_characters(font_name)
