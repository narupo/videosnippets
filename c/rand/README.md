# C言語のrand()の使い方

rand()は乱数を返す関数。
srand()で乱数のシードを初期化することができる。

# minからmaxの範囲の乱数を得る式

    min + rand() % (max - min + 1)
