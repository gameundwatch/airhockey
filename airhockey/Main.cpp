
# include <Siv3D.hpp> // OpenSiv3D v0.4.3
# include <cmath>

void Main()
{
	// 背景を水色にする
	Scene::SetBackground(Palette::Lawngreen);

	// ゲーム状態

	bool isPackOn = false;
	const Font press(40);
	const Font gui(20);

	// スコア
	const Font score(60);
	int score_Red	= 0;
	int score_Blue	= 0;

	Rect Stage(200, 40, 400, 520);

	//パドルの可動域
	Rect Zone_Red(210, 50, 380, 250);
	Rect Zone_Blue(210, 300, 380, 250);
	// ゴール
	Line Goal_Red(300, 50, 500, 50);
	Line Goal_Blue(300, 550, 500, 550);

	// パドル
	bool grab = false;
	Circle Paddle_Red(400, 100, 25);
	Circle Paddle_Blue(400, 500, 25);

	// パック
	bool collision = false;
	bool grabbedPack = false;
	Circle Pack(400, 300, 20);
	Circle grabRange(400, 300, 50);

	// 速さ
	double speed = 20.0;
	// 速度
	Vec2 PackVelocity(40 , 40);

	// 音
	const Audio audio_Hit(U"sounds/hit.wav");
	const Audio audio_Bounce(U"sounds/bounce.wav");
	const Audio audio_Goal(U"sounds/goal.wav");

	// 敵
	double level = 0;
	double delta_x = 0;
	double delta_y = 0;
	double move_x = 0;
	double move_y = 0;

	const double deltaTime = Scene::DeltaTime();

	while (System::Update())
	{

		if (!isPackOn)
		{

			score(score_Red).drawAt({ 700,100 }, Palette::Black);
			score(score_Blue).drawAt({ 700,500 }, Palette::Black);

			Stage.draw(Palette::White).drawFrame(0, 8, Palette::Darkgray);

			Zone_Red.draw(Palette::Lightpink);
			Zone_Blue.draw(Palette::Lightblue);
			Goal_Red.draw(10, Palette::Red);
			Goal_Blue.draw(10, Palette::Blue);

			Pack.draw(Palette::White).drawFrame(4, 0, Palette::Orange);
			Paddle_Blue.setPos(Cursor::Pos());
			if (Zone_Blue.contains(Paddle_Blue))
			{
				Paddle_Blue.movedBy(2, 2).drawShadow(Vec2(0, 0), 12, 0, ColorF(0.0, 1));
				Paddle_Blue.draw().drawFrame(8, 0, Palette::Cadetblue);
			}
			Paddle_Red.draw().drawFrame(8, 0, Palette::Palevioletred);

			press(U"LEFT CLICK TO START.").drawAt(Scene::Center(), ColorF(0.2, 0.2, 0.2, 0.5 + Periodic::Sine0_1(1s) * 0.5));

			gui(U"ENEMY LEVEL").draw(10, 180, Palette::Black);
			SimpleGUI::Slider(level, 0.01, 0.2, Vec2(10, 220));

			if (SimpleGUI::Button(U"Reset", Vec2(50, 500)))
			{
				// Move the cat's coordinates to a random position in the screen
				score_Blue = 0;
				score_Red = 0;
			}

			if (MouseL.down() && Zone_Blue.contains(Paddle_Blue)) {
				isPackOn = true;
			}

			continue;
		}

		// テキストを画面の中心に描く
		score(score_Red).drawAt({700,100}, Palette::Black);
		score(score_Blue).drawAt({700,500}, Palette::Black);

		Stage.draw(Palette::White).drawFrame(0, 8, Palette::Darkgray);

		Zone_Red.draw(Palette::Lightpink);
		Zone_Blue.draw(Palette::Lightblue);
		Goal_Red.draw(10, Palette::Red);
		Goal_Blue.draw(10, Palette::Blue);

		Pack.draw(Palette::White).drawFrame(4, 0, Palette::Orange);

		// 可動域外でパドルが消えるように
		if (Zone_Blue.contains(Paddle_Blue))
		{
			if (!grab)
			{
				Paddle_Blue.movedBy(2,2).drawShadow(Vec2(0, 0), 12, 0, ColorF(0.0, 1));
			}
			Paddle_Blue.draw().drawFrame(8, 0, Palette::Cadetblue);
		}

		Paddle_Red.draw().drawFrame(8, 0, Palette::Palevioletred);

		// パックの動作

		ClearPrint();
		PackVelocity.x = Clamp(PackVelocity.x, -5000.0, 5000.0);
		PackVelocity.y = Clamp(PackVelocity.y, -5000.0, 5000.0);

		Pack.moveBy(PackVelocity * Scene::DeltaTime());

		// 壁との衝突
		Pack.x = Clamp(Pack.x, 220.0, 580.0);
		Pack.y = Clamp(Pack.y, 60.0, 540.0);

		if (Pack.y <= 60 && PackVelocity.y < 0)
		{
			audio_Bounce.playOneShot();
			PackVelocity.y *= -1;
		}
		if (Pack.y >= 540 && PackVelocity.y > 0)
		{
			audio_Bounce.playOneShot();
			PackVelocity.y *= -1;
		}
		if (Pack.x <= 220 && PackVelocity.x < 0)
		{
			audio_Bounce.playOneShot();
			PackVelocity.x *= -1;
		}
		if (Pack.x >= 580 && PackVelocity.x > 0)
		{
			audio_Bounce.playOneShot();
			PackVelocity.x *= -1;
		}

		// パドルの動作
		Paddle_Blue.setPos(Cursor::Pos());

		if (MouseL.pressed()) 
		{
			grab = true;
		}
		else if (MouseL.up())
		{
			grab = false;
		}


		if (MouseL.down() && Paddle_Blue.intersects(Pack) && Zone_Blue.contains(Paddle_Blue))
		{
			grabbedPack = true;
		}

		// 掴み
		if (grabbedPack && 220 < Pack.x && Pack.x < 580 && 60 < Pack.y)
		{
			PackVelocity.x = 0;
			PackVelocity.y = 0;
			Pack.moveBy(Cursor::Delta());
		}

		if (grab && Zone_Blue.contains(Paddle_Blue) )
		{
			// パックとパドルの衝突
			if ((Paddle_Blue.intersects(Pack) && Zone_Blue.contains(Paddle_Blue)) && !grabbedPack)
			{
				collision = true;
			}
			else
			{
				collision = false;
			}
		}
		else
		{
			grabbedPack = false;
		}

		// 衝突
		if (collision)
		{
			audio_Hit.playOneShot();
			PackVelocity.x = (Pack.x - Paddle_Blue.x) * (Cursor::Delta().x * Cursor::Delta().x + speed);
			PackVelocity.y = (Pack.y - Paddle_Blue.y) * (Cursor::Delta().y * Cursor::Delta().y + speed);
		}

		// 青の得点
		if (Goal_Red.intersects(Pack)) {
			audio_Goal.playOneShot();
			score_Blue += 1;
			isPackOn = false;
			Pack.setPos(400,200);
			PackVelocity.set(Random(-40, 40), -40);
		}

		// 赤の得点
		if (Goal_Blue.intersects(Pack)) {
			audio_Goal.playOneShot();
			score_Red += 1;
			isPackOn = false;
			Pack.setPos(400, 400);
			PackVelocity.set(Random(-40,40), 40);
		}

		// 敵のAI
		delta_x = Pack.x - Paddle_Red.x;
		delta_y = Pack.y - Paddle_Red.y;
		move_x = delta_x * level;
		move_y = delta_y * level;

		if (Paddle_Red.intersects(Pack) && Zone_Red.contains(Paddle_Red)) {
			audio_Hit.playOneShot();
			PackVelocity.x = delta_x * (move_x * move_x * level + speed);
			PackVelocity.y = delta_y * (move_y * move_y * level + speed);
		}

		if (Zone_Red.contains(Pack)) {
			Paddle_Red.moveBy(delta_x * 0.1, delta_y * level);
		}
		else
		{
			Paddle_Red.moveBy((400 - Paddle_Red.x) * level, (100 - Paddle_Red.y) * level);
		}

		// [A] キーが押されたら
		if (KeyA.down())
		{
			// Hello とデバッグ表示する
			Print << U"Hello!";
		}

	}
}

