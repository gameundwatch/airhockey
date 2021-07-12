
# include <Siv3D.hpp> // OpenSiv3D v0.4.3

void Main()
{
	// 背景を水色にする
	Scene::SetBackground(Palette::Lawngreen);

	// スコア
	const Font score(60);
	int score_Red	= 0;
	int score_Blue	= 0;


	Rect Stage(200, 0, 400, 600);

	//パドルの可動域
	Rect Zone_Red(210, 10, 380, 280);
	Rect Zone_Blue(210, 310, 380, 280);
	// ゴール
	Line Goal_Red(300, 10, 500, 10);
	Line Goal_Blue(300, 590, 500, 590);

	// パドル
	Circle Paddle_Red(400, 40, 20);
	Circle Paddle_Blue(400, 560, 20);

	// パック
	Circle Pack(400, 300, 10);
	// 速さ
	double speed = 10.0;
	double cursor_speed = 0;
		// 速度
	Vec2 PackVelocity(0, speed);

	const double deltaTime = Scene::DeltaTime();

	while (System::Update())
	{

		// テキストを画面の中心に描く
		score(score_Red).drawAt({700,100}, Palette::Black);
		score(score_Blue).drawAt({700,500}, Palette::Black);

		Stage.draw(Palette::White).drawFrame(5, 0, Palette::Darkgray);

		Zone_Red.draw(Palette::Lightpink);
		Zone_Blue.draw(Palette::Lightblue);
		Goal_Red.draw(10, Palette::Red);
		Goal_Blue.draw(10, Palette::Blue);

		Paddle_Blue.setPos(Cursor::Pos());

		// 可動域外でパドルが消えるように
		if (Zone_Blue.contains(Paddle_Blue))
		{
			Paddle_Blue.draw().drawFrame(2, 0, Palette::Cadetblue);
		}

		Paddle_Red.draw().drawFrame(2, 0, Palette::Palevioletred);

		// パックの動作
		Pack.moveBy(PackVelocity * Scene::DeltaTime());
		if (Pack.y < 10 && PackVelocity.y < 0 )
		{
			PackVelocity.y *= -1;
		}
		if (Pack.y > 590 && PackVelocity.y > 0)
		{
			PackVelocity.y *= -1;
		}
		if (Pack.x < 210 && PackVelocity.x < 0)
		{
			PackVelocity.x *= -1;
		}
		if (Pack.x > 590 && PackVelocity.x > 0)
		{
			PackVelocity.x *= -1;
		}

		if (Paddle_Blue.intersects(Pack) && Zone_Blue.contains(Paddle_Blue)) {
			PackVelocity.x = (Pack.x - Paddle_Blue.x) * (Cursor::Delta().x * Cursor::Delta().y + speed);
			PackVelocity.y = (Pack.y - Paddle_Blue.y) * (Cursor::Delta().x * Cursor::Delta().y + speed);
		}
		if (Paddle_Red.intersects(Pack) && Zone_Red.contains(Paddle_Red)) {
			PackVelocity.x = (Pack.x - Paddle_Red.x) * (Cursor::Delta().x * Cursor::Delta().y + speed);
			PackVelocity.y = (Pack.y - Paddle_Red.y) * (Cursor::Delta().x * Cursor::Delta().y + speed);
		}

		Pack.draw(Palette::White).drawFrame(2, 0, Palette::Orange);

		// 青の得点
		if (Goal_Red.intersects(Pack)) {
			score_Blue += 1;
			Pack.setPos(400,200);
			PackVelocity.set(-20, -20);
		}

		// 赤の得点
		if (Goal_Blue.intersects(Pack)) {
			score_Red += 1;
			Pack.setPos(400, 400);
			PackVelocity.set(20, 20);
		}

		// 敵のAI
		// Paddle_Red.x = Pack.x - Paddle_Red.x * deltaTime * 0.1;
		// Paddle_Red.y = Pack.y - Paddle_Red.y * deltaTime * 0.1;

		// [A] キーが押されたら
		if (KeyA.down())
		{
			// Hello とデバッグ表示する
			Print << U"Hello!";
		}

	}
}

//
// = アドバイス =
// Debug ビルドではプログラムの最適化がオフになります。
// 実行速度が遅いと感じた場合は Release ビルドを試しましょう。
// アプリをリリースするときにも、Release ビルドにするのを忘れないように！
//
// 思ったように動作しない場合は「デバッグの開始」でプログラムを実行すると、
// 出力ウィンドウに詳細なログが表示されるので、エラーの原因を見つけやすくなります。
//
// = お役立ちリンク =
//
//
// OpenSiv3D リファレンス
// https://siv3d.github.io/ja-jp/
//
// チュートリアル
// https://siv3d.github.io/ja-jp/tutorial/basic/
//
// よくある間違い
// https://siv3d.github.io/ja-jp/articles/mistakes/
//
// サポートについて
// https://siv3d.github.io/ja-jp/support/support/
//
// Siv3D ユーザコミュニティ Slack への参加
// https://siv3d.github.io/ja-jp/community/community/
//
// 新機能の提案やバグの報告
// https://github.com/Siv3D/OpenSiv3D/issues
//
