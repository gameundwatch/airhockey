
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
	Circle Paddle_Red(400, 40, 25);
	Circle Paddle_Blue(400, 560, 25);

	// パック
	Circle Pack(400, 300, 20);
	// 速さ
	double speed = 20.0;
	// 速度
	Vec2 PackVelocity(40 , 40);

	// 音
	const Audio audio_Hit(U"sounds/hit.wav");
	const Audio audio_Bounce(U"sounds/bounce.wav");
	const Audio audio_Goal(U"sounds/goal.wav");

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

		// 壁との衝突
		if (Pack.y < 20 && PackVelocity.y < 0 )
		{
			audio_Bounce.playOneShot();
			PackVelocity.y *= -1;
		}
		if (Pack.y > 580 && PackVelocity.y > 0)
		{
			audio_Bounce.playOneShot();
			PackVelocity.y *= -1;
		}
		if (Pack.x < 220 && PackVelocity.x < 0)
		{
			audio_Bounce.playOneShot();
			PackVelocity.x *= -1;
		}
		if (Pack.x > 580 && PackVelocity.x > 0)
		{
			audio_Bounce.playOneShot();
			PackVelocity.x *= -1;
		}

		// パックとパドルの衝突
		if (Paddle_Blue.intersects(Pack) && Zone_Blue.contains(Paddle_Blue)) {
			audio_Hit.playOneShot();
			PackVelocity.x = (Pack.x - Paddle_Blue.x) * (Cursor::Delta().x * Cursor::Delta().y + speed);
			PackVelocity.y = (Pack.y - Paddle_Blue.y) * (Cursor::Delta().x * Cursor::Delta().y + speed);
		}
		if (Paddle_Red.intersects(Pack) && Zone_Red.contains(Paddle_Red)) {
			audio_Hit.playOneShot();
			PackVelocity.x = (Pack.x - Paddle_Red.x) * ((Pack.x - Paddle_Red.x) * 0.1 * (Pack.y - Paddle_Red.y) * 0.1 + speed);
			PackVelocity.y = (Pack.y - Paddle_Red.y) * ((Pack.x - Paddle_Red.x) * 0.1 * (Pack.y - Paddle_Red.y) * 0.1 + speed);
		}

		Pack.draw(Palette::White).drawFrame(2, 0, Palette::Orange);

		// 青の得点
		if (Goal_Red.intersects(Pack)) {
			audio_Goal.playOneShot();
			score_Blue += 1;
			Pack.setPos(400,200);
			PackVelocity.set(-40, -40);
		}

		// 赤の得点
		if (Goal_Blue.intersects(Pack)) {
			audio_Goal.playOneShot();
			score_Red += 1;
			Pack.setPos(400, 400);
			PackVelocity.set(40, 40);
		}

		// 敵のAI

		if (Zone_Red.contains(Pack)) {
			Paddle_Red.moveBy((Pack.x - Paddle_Red.x) * 0.1, (Pack.y - Paddle_Red.y) * 0.1);
		}
		else
		{
			Paddle_Red.moveBy((400 - Paddle_Red.x) * 0.01, (100 - Paddle_Red.y) * 0.01);
		}

		// [A] キーが押されたら
		if (KeyA.down())
		{
			// Hello とデバッグ表示する
			Print << U"Hello!";
		}

	}
}

