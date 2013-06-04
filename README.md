# Reaperのアクションを呼び出すプログラム

windows専用です。

## 配布物構成

 * README.txt          このファイル
 * reaper_play.exe     reaperの再生を実行するプログラム
 * reaper_action.exe   reaperの任意のアクションを実行するプログラム
 * play.bat            batファイルのサンプル
 * reaper_play.c       ソース・コード
 * reaper_action.c     同上
 * reaper_play.afh     AutoHotKeyでのサンプル
 * reaper_action.afh   同上

## 使い方

コマンドラインからアクションIDを指定して呼び出します。(.exeは省略可)


> reaper_action.exe アクションID


他のオプションはありません。


### 注意事項

アクションIDを打ち間違えて実行したりすると、意図しないアクションが実行されてしまいます。
多くのアクションはUndoが可能ですが、保存等やり直しができないアクションもあるかもしれません。
よく使うアクションは、ショートカットやバッチ・ファイルの作成をお勧めします。


## 利用例

複数のアクションを組み合わせる場合は、Reaperのカスタム・アクションが使えます。
以下は、定時にトラックを鳴らすシステムの設定例


 * プロジェクトを準備 (以下の２つのトラックを持つプロジェクトを想定)
    * Track 01 (例)始業のチャイム
    * Track 02 (例)就業のチャイム

 * カスタム・アクションを作成する。[REAPER]

    * Reaperのメニュー アクション(A) -> アクションリストを開く(?)
    * カスタムアクションの新規作成
      トラック1をソロで最初から再生するカスタムアクションの例

        * Track: Unsolo all tracks
        * Track: Toggle solo for track 01
        * Transport: Go to start of project
        * Transport: play

    * 同様に、トラック2をソロで再生するカスタムアクションも作成します


 * バッチ・ファイルを作成する。[Windows]

    拡張子 .bat のファイルを作成します。短いのでメモ帳等でok

```bat:reaper_play.bat
@echo off
reaper_action アクションID
```

 アクションIDはReaperのアクションリストから確認します。
 Reaper拡張やカスタム・アクション等のID(括弧つきで書かれてる)は、
 環境によってIDの値が違うことがあるかもしれません。（未確認）


  * タスク・スケジューラーに登録 [Windows]
    Windowsのバージョンによっていくつか登録方法があるようです。
    細かな設定が多数あるようなので、適当に検索して見つけた記事に丸投げ。

    タスク・スケジューラで数分おきにプログラムを繰り返し実行する
    http://www.atmarkit.co.jp/fwin2k/win2ktips/1332reptsk/reptsk.html

    * タスク1 毎時 0分に Track01を再生する プログラムを実行
    * タスク2 毎時 45分に Track02を再生する プログラムを実行


    ※ バッチ・ファイルを作らなくても、
    タスクの設定で、指定時間に起動するプログラムやスクリプトを指定し、
    起動時の引数を設定する事も出来ます。

    ※ カスタム・アクションを用いた応用例ということで、
    2トラックを別々に個別のタスクから再生する方法を挙げましたが、
    0分と45分で同じプログラムを実行する場合は、1つのタスク内で複数のトリガを設定できます。

    ※ タスクのトリガは、毎週土曜日の22:00から一定時間の間、1時間毎に指定のプログラムを実行
    という形で登録可能です。

    reaper_action.exe は、Reaperが起動してなければ何もしないプログラムなので、
    タスクの有効・無効を切り替えなくても、登録したままで問題ありませんが、イベントログに記録が残るので、
    出来るだけ必要な時間帯のみ実行されるように設定する方がよいです。



## ソースコード解説

Reaperの再生アクションを引き起こすプログラムを例に解説します。
reaper_action.c は、このプログラムを、引数を取れるように変更したものです。

コンパイルにはTiny C Compiler (http://bellard.org/tcc/) を用います。
コンパイル方法は、コマンドラインから tcc.exe reaper_play.c -luser32


```c:
// reaper_play.c

#include <windows.h>

#define REAPER_ACTION_PLAY 1007

int main(void)
{
  // (1) Reaperのウィンドウを探す
  // "REAPERwnd" がメインウィンドウのクラス名です。
  //
  // ※ 複数のReaperを立ち上げているような状況は考慮してません。
  //
  HWND reaper = FindWindow("REAPERwnd", NULL);

  // Reaperが起動中ならば…
  if (reaper != NULL)
  {
    // (2) Reaperのメインウィンドウをアクティブにする
    //
    // ※ トラックを再生する場合、一時的にでもアクティブにしないと
    //    トラックのアイテムがOFFLINEのままなので、再生はされますが、音が出ません。
    //
    SendMessage(reaper, WM_ACTIVATE, WA_ACTIVE, 0);

    // (3) Reaperへコマンドを送る
    //
    // 実行するアクションのIDは、Reaperのアクション・リストから探してください
    // 右クリックメニューから、アクションIDの表示/コピーが出来ます。
    //
    SendMessage(reaper, WM_COMMAND, REAPER_ACTION_PLAY, 0);
  }

  return 0;
}
```

## その他のユーティリティ


### AutoHotKey

  http://www.autohotkey.com/

   外部プログラムにキーを送るスクリプト等が簡単に記述できます。
   アクションにキーボード・ショートカットを設定する事でも、アクション呼び出しが実現できます。

```afk:
   ; reaper_play.ahk
   IfWinExist ahk_class REAPERwnd
   {
    WinActivate
    Send, !{Space} ; ALT+SPACEキーをReaperに送る
   }
```

   reaper_action.c と同じ方法でアクションのコマンド送るスクリプト
   AutoHotKey では以下のようになります。


```afk:
   ; reaper_action.ahk
   argc = %0%
   action = %1%
   IfWinExist ahk_class REAPERwnd
   {
    WinActivate
    if (argc == 1) {
      SendMessage 0x111, action, ,
    }
   }
```


### SWS/S&M拡張

  http://www.standingwaterstudios.com/

   いろいろな拡張機能を纏めた物みたいです。

   カスタム・アクションでのRepeatボタンのon/offや、プロジェクトtabの切り替え等、
   Reaper標準でありそうでなかったアクションも多数追加されます。

