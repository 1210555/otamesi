# マイマインスイーパー

## 概要

C++と SFML を用いて作成した 2D マインスイーパー風ゲームです。  
左クリックでマスを開き、右クリックで旗を立てることができます。

## 使用技術

- **言語**：C++
- **開発環境**：Visual Studio Code（MSYS2 + UCRT64 環境）
- **ライブラリ**：SFML（Simple and Fast Multimedia Library）

## ビルド方法（MSYS2 UCRT64 環境で実行）

```bash
g++ src/main.cpp src/Game.cpp src/Field.cpp src/GameUI.cpp src/GameRenderer.cpp -o MyMinesweeper -lsfml-graphics -lsfml-window -lsfml-system
実行方法
bash
./MyMinesweeper.exe
※ .exe をダブルクリックしても動作しない場合があります。MSYS2のターミナルから実行してください。

操作方法
左クリック：マスを開く

右クリック：旗を立てる

Resetボタン：最初やゲームオーバー時、勝利時にウィンドウ上部の「Reset」を押すことでリスタート可能
```
