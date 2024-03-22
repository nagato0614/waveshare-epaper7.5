import os
import sys
from svglib.svglib import svg2rlg
from reportlab.graphics import renderPM
from image_converte import resize_and_convert_to_grayscale_bmp
from concurrent.futures import ThreadPoolExecutor

THREADS_NUM = 8  # 並列処理のスレッド数を定義


def convert_svg_to_png(filename, output_size=(100, 100)):
    filename_without_ext = os.path.splitext(os.path.basename(filename))[0]
    drawing = svg2rlg(filename)

    if output_size:
        drawing.width, drawing.height = output_size

    # output directoryがなければ作成
    if not os.path.exists('output'):
        os.makedirs('output')

    output_file_path = "output/" + filename_without_ext + ".png"

    renderPM.drawToFile(drawing, output_file_path, fmt="PNG")
    return output_file_path


def process_svg_file(filename, output_size):
    print('Converting', filename, 'to PNG')
    png_file = convert_svg_to_png('svg/' + filename, output_size)

    output_image = os.path.join('output', os.path.splitext(filename)[0])
    resize_and_convert_to_grayscale_bmp(png_file, output_image)
    print('Converted', filename, 'to PNG and BMP')


if __name__ == "__main__":
    args = sys.argv
    output_size = None

    if len(args) == 3:
        output_size = (float(args[1]), float(args[2]))

    # svg ディレクトリ内の.svgファイルを取得
    svg_files = [filename for filename in os.listdir('svg') if filename.endswith('.svg')]

    # ThreadPoolExecutorを使用して並列処理
    with ThreadPoolExecutor(max_workers=THREADS_NUM) as executor:
        futures = [executor.submit(process_svg_file, filename, output_size) for filename in
                   svg_files]

        # 全てのタスクが完了するまで待機
        for future in futures:
            future.result()