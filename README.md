# readIR-RPi

## What is this ?
Raspberry Pi用の赤外線信号を受信し, それを保存するCプログラム.  
GPIOピン配置はRaspberry Pi 3 Model Bと同様のもの(以降これらをRPiと書く)なら動く(と思う).

## My environment
- Hardware: Raspberry Pi 3 Model B, Raspberry Pi Zero WH
- OS: Raspbian Stretch lite

## Build
実行ファイルを出力するディレクトリで`cmake`するとMakefileができる.  
Makefileが出力されたディレクトリで `make` する.  

もし, cmakeがインストールされていないなら `sudo apt install cmake` する.

## How to use
赤外線センサ(OSRB38C9AA)をRPiに繋ぐ.
赤外線センサのOutputはreadIR.cの
```objectivec
#define READPIN
```
で定義されたGPIOピンに接続する(デフォルトはGPIOの23番ピン).  

これで `./readIR <データを保存するファイル名>`を実行し, 赤外線センサに向かって赤外線リモコンから信号を送るとそれを指定したファイルに保存される.

## Attentions
このソフトウェアを使用したことによる機器の故障などの損害については一切の責任を負いません.