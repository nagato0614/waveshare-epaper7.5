import os
from datetime import datetime
from pytube import YouTube


class YouTubeDownloader:
    def __init__(self, url):
        self.url = url
        self.video_dir = "video"

        # 保存先のディレクトリを作成
        if not os.path.exists(self.video_dir):
            os.makedirs(self.video_dir)

    def download(self):
        try:
            # YouTubeの動画をダウンロード
            yt = YouTube(self.url)
            stream = yt.streams.get_highest_resolution()

            # 動画のタイトルを取得
            title = yt.title

            # タイトルが取得できない場合は日時と時間をファイル名にする
            if not title:
                now = datetime.now()
                title = now.strftime("%Y%m%d_%H%M%S")

            # ファイル名から特殊文字と空白を削除
            title = "".join(c for c in title if c.isalnum() or c in [" ", "-", "_"])
            title = title.replace(" ", "_")

            # 動画を保存
            file_path = os.path.join(self.video_dir, f"{title}.mp4")
            stream.download(output_path=self.video_dir, filename=f"{title}.mp4")

            print(f"動画をダウンロードしました: {file_path}")

        except Exception as e:
            print(f"ダウンロード中にエラーが発生しました: {str(e)}")

if __name__ == '__main__':
    # YouTubeの動画URLを指定してダウンローダーを作成
    url = ""
    downloader = YouTubeDownloader(url)

    # 動画をダウンロード
    downloader.download()