# RaceGame

Developed with Unreal Engine 4

##フォルダ構造

作成したコンテンツは全てDevelopフォルダに格納しています。

- Content
  -Collections
  -Developers
    - fuwarrin
      - Collections
        - BP
          - BaseBP　プロトタイプ作成時のフォルダ
          - BaseCpp　C＋＋で基底クラスを作成したBPをまとめたフォルダ
            - Item　アイテムのBP
          - Data 　GameInstanceやBPFunctionEnumなど
          - ImportData  csvファイルのimportデータ
          - Material  マテリアル
          - Test  テスト用のデータ
          - UWG Widget

- 改修予定リスト
 - アイテム位置の調整（クライアント側でサーバ側がアイテムを使用する差異サーバー側へアイテムが出減してしまっている）
 - チープな見た目改修（エフェクトなど実装）
 - アイテムの外部データから読み込める用にする。
 - HOST側が一度ゲームを抜けてしまうとHOST側の再接続ができなくなるバグ
