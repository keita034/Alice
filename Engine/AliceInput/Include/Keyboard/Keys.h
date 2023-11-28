#pragma once

#undef DELETE

enum class Keys
{
	//無し
	NONE = 0x00,

	//エスケープ
	ESC = 0x01,

	//1
	ONE = 0x02,
	//2
	TWO = 0x03,
	//3
	THREE = 0x04,
	//4
	FOUR = 0x05,
	//5
	FIVE = 0x06,
	//6
	SIX = 0x07,
	//7
	SEVEN = 0x08,
	//8
	EIGHT = 0x09,
	//9
	NINE = 0x0A,
	//0
	ZERO = 0x0B,

	//Q
	Q = 0x10,
	//W
	W = 0x11,
	//E
	E = 0x12,
	//R
	R = 0x13,
	//T
	T = 0x14,
	//Y
	Y = 0x15,
	//U
	U = 0x16,
	//I
	I = 0x17,
	//O
	O = 0x18,
	//P
	P = 0x19,
	//A
	A = 0x1E,
	//S
	S = 0x1F,
	//D
	D = 0x20,
	//F
	F = 0x21,
	//G
	G = 0x22,
	//H
	H = 0x23,
	//J
	J = 0x24,
	//K
	K = 0x25,
	//L
	L = 0x26,
	//Z
	Z = 0x2C,
	//X
	X = 0x2D,
	//C
	C = 0x2E,
	//V
	V = 0x2F,
	//B
	B = 0x30,
	//N
	N = 0x31,
	//M
	M = 0x32,

	//ファンクション1
	F1 = 0x3B,
	//ファンクション2
	F2 = 0x3C,
	//ファンクション3
	F3 = 0x3D,
	//ファンクション4
	F4 = 0x3E,
	//ファンクション5
	F5 = 0x3F,
	//ファンクション6
	F6 = 0x40,
	//ファンクション7
	F7 = 0x41,
	//ファンクション8
	F8 = 0x42,
	//ファンクション9
	F9 = 0x43,
	//ファンクション10
	F10 = 0x44,
	//ファンクション11
	F11 = 0x57,
	//ファンクション12
	F12 = 0x58,
	//ファンクション13
	F13 = 0x64,
	//ファンクション14
	F14 = 0x65,
	//ファンクション15
	F15 = 0x66,

	//1(テンキー)
	NUM1 = 0x4F,
	//2(テンキー)
	NUM2 = 0x50,
	//3(テンキー)
	NUM3 = 0x51,
	//4(テンキー)
	NUM4 = 0x4B,
	//5(テンキー)
	NUM5 = 0x4C,
	//6(テンキー)
	NUM6 = 0x4D,
	//7(テンキー)
	NUM7 = 0x47,
	//8(テンキー)
	NUM8 = 0x48,
	//9(テンキー)
	NUM9 = 0x49,
	//0(テンキー)
	NUM0 = 0x52,
	//マイナス(テンキー)
	NUMMINUS = 0x4A,
	//プラス(テンキー)
	NUMPLUS = 0x4E,
	//ドット(テンキー)
	NUMDOT = 0x53,
	//エンター(テンキー)
	NUMENTER = 0x9C,
	//スラッシュ(テンキー)
	NUMSLASH = 0xB5,
	//イコール(テンキー)
	NAMNEQUAL = 0x8D,
	//コンマ(テンキー)
	NUMCOMMA = 0xB3,

	//上
	UP = 0xC8,
	//下
	DOWN = 0xD0,
	//左
	LEFT = 0xCB,
	//右
	RIGHT = 0xCD,

	//左シフト
	LEFTSHIFT = 0x2A,
	//右シフト
	RIGHTSHIFT = 0x36,
	//左コントロール
	LEFTCTRL = 0x1D,
	//右コントロール
	RIGHTCTRL = 0x9D,
	//アスタリスク
	ASTERISK = 0x37,
	//オルト
	ALT = 0x38,
	//右オルト
	RIGHTALT = 0xB8,
	//スペース
	SPACE = 0x39,
	//エンター
	ENTER = 0x1C,
	//タブ
	TAB = 0x0F,
	//プラス
	PLUS = 0x27,
	//マイナス
	MINUS = 0x0C,
	//イコール
	EQUARL = 0x0D,
	//バックスペース
	BACKSPACE = 0x0E,
	//スラッシュ
	SLASH = 0x35,
	//バックスラッシュ
	BACKSLASH = 0x2B,
	//シングルクォーテーション
	APOSTROPHE = 0x28,
	//変換
	CONVERT = 0x79,
	//無変換
	NONCONVERT = 0x7B,
	//左ブラケット{
	LEFTBLACKECT = 0x1A,
	//右ブラケット}
	RIGHTBLACKECT = 0x1B,
	//ページアップ
	PAGEUP = 0xC9,
	//ページダウン
	PAGEDOWN = 0xD1,
	//インサート
	INSERT = 0xD2,
	//デリート
	DELETE = 0xD3,
	//左ウィンドウズ
	LWINDOW = 0xDB,
	//右ウィンドウズ
	RWINDOW = 0xDC,
	//エンド
	END = 0xCF,
	//ポーズ
	PAUSE = 0xC5,
	//ホーム
	HOME = 0xC7,
	//メニュー
	MENU = 0xDD,
	//キャプスロック
	CAPITAL = 0x3A,
	//ナムロック
	NUMLOCK = 0x45,
	//スクロールロック
	SCROLLLOCK = 0x46,
	//小なり
	LESSTHAN = 0x33,
	//大なり
	MORETHAN = 0x34,
	//アットマーク
	AT = 0x91,
	//コロン
	COLON = 0x92,
	//アンダーバー
	UNDERLINE = 0x93,
	//円
	YEN = 0x7D,
	//カナ
	KANA = 0x70,

	//ウェブストップ
	WEBSTOP = 0xE8,
	//ウェブバック
	WEBBACK = 0xEA,
	//ウェブホーム
	WEBHOME = 0xB2,
	//メディアストップ
	MEDIASTOP = 0xA4,
	//メディアセレクト
	MEDIASELECT = 0xED,
	//ボリュームダウン
	VOLUMEDOWN = 0xAE,
	//ボリュームアップ
	VOLUMEUP = 0xB0,
	//デッド
	GRAVE = 0x29,
	//102キーボード用スラッシュ
	OEM102,
	//ポルトガル語キーボード:?
	ABNTC1 = 0x73,
	//ポルトガル語キーボード:テンキードット
	ABNTC2 = 0x7E,
	//キャレット
	CIRCUMFLEX = 0x90,
	//漢字
	KANJI = 0x94,
	//ストップ
	STOP = 0x95,
	//AX(AX規格のESC?)
	AX = 0x96,
	//J3100のなんかのキー
	UNLABEBEL = 0x97,
	//ネクストトラック
	NEXTTRACK = 0x99,
	//カリキュレータ
	CALCULATOR = 0xA1,
	//プレイ
	PLAY = 0xA2,
	//システムリクエスト
	SYSRQ = 0xB7,
	//パワー
	POWER = 0xDE,
	//スリープ
	SLEEP = 0xDF,
	//ウェイク
	WAKE = 0xE3,
	//サーチ
	SEARCH = 0xE5,
	//フェイヴァリト
	FAVORITES = 0xE6,
	//リフレッシュ
	REFRESH = 0xE7,
	//フォワード
	FORWARD = 0xE9,
	//コンピューター
	COMPUTER = 0xEB,
	//メール
	MAIL = 0xEC,
};