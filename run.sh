rm main
rm -r data1
rm -r data2
rm -r img_data1
rm -r img_data2
rm  data1.mp4
rm  data2.mp4
rm  data1.gif
rm  data2.gif

g++ -o main ./main.cpp

mkdir -p data1
mkdir -p data2

./main
python plot.py