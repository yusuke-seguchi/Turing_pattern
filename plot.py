import numpy as np
import matplotlib.pyplot as plt
import glob
import os
from natsort import natsorted
import cv2
from PIL import Image

# matplotlibのデフォルトパラメータ設定(rcParams)
# rcParamsで種々のパラメータを設定できる
# ここで一括で設定しておくと楽。（変えたい部分は後から個別に変更できる）
plt.rcParams['font.family'] ='Arial'       # 使用するフォント
plt.rcParams['font.size'] = 16             # フォントの大きさ
plt.rcParams['axes.linewidth'] = 1.0       # 軸の線幅edge linewidth。囲みの太さ
plt.rcParams['xtick.direction'] = 'in'     # x軸の目盛線が内向き('in')か外向き('out')か双方向か('inout')
plt.rcParams['ytick.direction'] = 'in'     # y軸の目盛線が内向き('in')か外向き('out')か双方向か('inout')
plt.rcParams["xtick.top"] = True           # 上部に目盛り線を描くかどうか
plt.rcParams["xtick.bottom"] = True        # 下部に目盛り線を描くかどうか
plt.rcParams["ytick.left"] = True          # 左部に目盛り線を描くかどうか
plt.rcParams["ytick.right"] = True         # 右部に目盛り線を描くかどうか
plt.rcParams["xtick.major.size"] = 5.0     # x軸主目盛り線の長さ
plt.rcParams["ytick.major.size"] = 5.0     # y軸主目盛り線の長さ
plt.rcParams["xtick.minor.visible"] = True # x軸副目盛り線を描くかどうか
plt.rcParams["ytick.minor.visible"] = True # y軸副目盛り線を描くかどうか
plt.rcParams["xtick.minor.size"] = 3.0     # x軸副目盛り線の長さ
plt.rcParams["ytick.minor.size"] = 3.0     # y軸副目盛り線の長さ

nx = 128
ny = 128
pstep = 100
fps = 20

color = ["viridis", "cividis"]
dirpath = ["data1", "data2"]
for i in range(len(dirpath)):
    os.makedirs(f"./img_{dirpath[i]}", exist_ok=True)
    path = natsorted(glob.glob(f"./{dirpath[i]}/*.csv"))
    for j in range(len(path)):
        aaa = np.loadtxt(path[j]).reshape(nx, ny)
        fig = plt.figure(figsize=(8,6))
        plt.imshow(np.flipud(aaa), aspect=1, cmap=color[i])
        plt.axis("off")
        plt.title(f"{j*pstep} step")
        plt.colorbar()
        fig.savefig(f"./img_{dirpath[i]}/{j*pstep}.png", bbox_inches="tight", dpi=200)
        plt.clf()
        plt.close()

img_array=[]
for i in range(len(dirpath)):
    files =  natsorted(glob.glob(f"./img_{dirpath[i]}/*.png"))
    for j in range(0, len(files)):
        img = cv2.imread(files[j])
        height, width, layers = img.shape
        size = (width, height)
        img_array.append(img)
        
    name = f'./{dirpath[i]}.mp4' #完成した動画の名前
    out = cv2.VideoWriter(name, cv2.VideoWriter_fourcc('m','p','4','v'), fps, size)

    for j in range(len(img_array)):
        out.write(img_array[j])
    out.release()


#画像を入れる箱を準備
pictures=[]
#画像を箱に入れていく
for i in range(len(dirpath)):
    files =  natsorted(glob.glob(f"./img_{dirpath[i]}/*.png"))
    for n in range(len(files)):
        img = Image.open(files[n])
        pictures.append(img)
    #gifアニメを出力する
    pictures[0].save(f'{dirpath[i]}.gif',save_all=True, append_images=pictures[1:],
    optimize=True, duration=50, loop=0)